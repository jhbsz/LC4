/*
 * drivers/md/dm-ox-crypt.c
 *
 * This file interfaces the low-level drivers for the hardware AES support
 * in the OX800/OX810 with the device-mapper framework 
 *
 * Copyright (C) 2003 Christophe Saout <christophe@saout.de>
 * Copyright (C) 2004 Clemens Fruhwirth <clemens@endorphin.org>
 * Copyright (C) 2006 Red Hat, Inc. All rights reserved.
 * Copyright (C) 2008 Oxford Semiconductor Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/err.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/mempool.h>
#include <linux/slab.h>
#include <linux/crypto.h>
#include <linux/workqueue.h>
#include <linux/backing-dev.h>
#include <asm/atomic.h>
#include <linux/scatterlist.h>
#include <asm/page.h>
#include <asm-arm/plat-oxnas/cipher.h>

#include "dm.h"

#define DM_MSG_PREFIX "ox-crypt"
#define MESG_STR(x) x, sizeof(x)

/*
 * per bio private data
 */
struct oxcrypt_io {
	struct dm_target *target;
	struct bio *base_bio;
	struct work_struct work;
	atomic_t pending;
	int error;
};

/*
 * context holding the current state of a multi-part conversion
 */
struct convert_context {
	struct bio *bio_in;
	struct bio *bio_out;
	unsigned int offset_in;
	unsigned int offset_out;
	unsigned int idx_in;
	unsigned int idx_out;
	sector_t sector;
	int write;
};

struct oxoxcrypt_config;

/*
 * Crypt: maps a linear range of a block device
 * and encrypts / decrypts at the same time.
 */
enum flags { DM_CRYPT_SUSPENDED, DM_CRYPT_KEY_VALID };
struct oxcrypt_config {
	struct dm_dev *dev;
	sector_t start;

	/*
	 * pool for per bio private data and
	 * for encryption buffer pages
	 */
	mempool_t *io_pool;
	mempool_t *page_pool;
	struct bio_set *bs;

	struct workqueue_struct *io_queue;
	struct workqueue_struct *crypt_queue;
	/*
	 * crypto related data
	 */
	sector_t iv_offset;
	unsigned int iv_size;

	struct crypto_blkcipher *tfm;
	unsigned long flags;
	u8 key[OX800DPE_KEYSIZE]; /* size of key is fixed by hardware */
    u8 iv_key[OX800DPE_KEYSIZE];
};

#define MIN_IOS        256
#define MIN_POOL_PAGES 32
#define MIN_BIO_PAGES  8

static struct kmem_cache *_oxcrypt_io_pool;

static void clone_init(struct oxcrypt_io *, struct bio *);



static int
crypt_convert_scatterlist(struct oxcrypt_config *cc, struct scatterlist *out,
                          struct scatterlist *in, unsigned int length,
                          int write, sector_t sector)
{
	int r;
	u8 iv[OX800DPE_KEYSIZE];
    u8* pri = cc->key;
    u8* twe = cc->iv_key;

    memset(iv, 0, sizeof(iv));
    /*
     * The IV generation algorithm, implemented in hardware, uses amongst other 
     * things the first 32-bits of the sector number. 
     *
     */
    *(u32 *)iv = cpu_to_le32(sector & 0xffffffff);

    /* all to the low-level driver */
    if (write) {
        r = ox800_aeslrw_encrypt(in, out, 1, iv, pri, twe);
    } else {
        r = ox800_aeslrw_decrypt(in, out, 1, iv, pri, twe);
    }
    
    /* clear remanents of the key */
    memset(iv, 0, sizeof(iv));

    //printk("back\n");
    if (r < 0) {
        printk(KERN_ERR"crypt_convert_scatterlist: core driver returned error %d\n",r);
    }
    
	return r;
}

static void crypt_convert_init(struct oxcrypt_config *cc,
			       struct convert_context *ctx,
			       struct bio *bio_out, struct bio *bio_in,
			       sector_t sector, int write)
{
	ctx->bio_in = bio_in;
	ctx->bio_out = bio_out;
	ctx->offset_in = 0;
	ctx->offset_out = 0;
	ctx->idx_in = bio_in ? bio_in->bi_idx : 0;
	ctx->idx_out = bio_out ? bio_out->bi_idx : 0;
	ctx->sector = sector + cc->iv_offset;
	ctx->write = write;
}

/*
 * Encrypt / decrypt data from one bio to another one (can be the same one)
 */
static int crypt_convert(struct oxcrypt_config *cc,
			 struct convert_context *ctx)
{
	int r = 0;

	while(ctx->idx_in < ctx->bio_in->bi_vcnt &&
	      ctx->idx_out < ctx->bio_out->bi_vcnt) {
		struct bio_vec *bv_in = bio_iovec_idx(ctx->bio_in, ctx->idx_in);
		struct bio_vec *bv_out = bio_iovec_idx(ctx->bio_out, ctx->idx_out);
		struct scatterlist sg_in, sg_out;

		sg_init_table(&sg_in, 1);
		sg_set_page(&sg_in, bv_in->bv_page, 1 << SECTOR_SHIFT, bv_in->bv_offset + ctx->offset_in);

		sg_init_table(&sg_out, 1);
		sg_set_page(&sg_out, bv_out->bv_page, 1 << SECTOR_SHIFT, bv_out->bv_offset + ctx->offset_out);

		ctx->offset_in += sg_in.length;
		if (ctx->offset_in >= bv_in->bv_len) {
			ctx->offset_in = 0;
			ctx->idx_in++;
		}

		ctx->offset_out += sg_out.length;
		if (ctx->offset_out >= bv_out->bv_len) {
			ctx->offset_out = 0;
			ctx->idx_out++;
		}

		r = crypt_convert_scatterlist(cc, &sg_out, &sg_in, sg_in.length,
					      ctx->write, ctx->sector);
		if (r < 0)
			break;

		ctx->sector++;
	}

	return r;
}

static void dm_crypt_bio_destructor(struct bio *bio)
{
	struct oxcrypt_io *io = bio->bi_private;
	struct oxcrypt_config *cc = io->target->private;

	bio_free(bio, cc->bs);
}

/*
 * Generate a new unfragmented bio with the given size
 * This should never violate the device limitations
 * May return a smaller bio when running out of pages
 */
static struct bio *crypt_alloc_buffer(struct oxcrypt_io *io, unsigned size)
{
	struct oxcrypt_config *cc = io->target->private;
	struct bio *clone;
	unsigned int nr_iovecs = (size + PAGE_SIZE - 1) >> PAGE_SHIFT;
	gfp_t gfp_mask = GFP_NOIO | __GFP_HIGHMEM;
	unsigned i, len;
	struct page *page;

	clone = bio_alloc_bioset(GFP_NOIO, nr_iovecs, cc->bs);
	if (!clone)
		return NULL;

	clone_init(io, clone);

	for(i = 0; i < nr_iovecs; i++) {
		page = mempool_alloc(cc->page_pool, gfp_mask);
		if (!page)
			break;

		/*
		 * if additional pages cannot be allocated without waiting,
		 * return a partially allocated bio, the caller will then try
		 * to allocate additional bios while submitting this partial bio
		 */
		if (i == (MIN_BIO_PAGES - 1))
			gfp_mask = (gfp_mask | __GFP_NOWARN) & ~__GFP_WAIT;

		len = (size > PAGE_SIZE) ? PAGE_SIZE : size;

		if (!bio_add_page(clone, page, len, 0)) {
			mempool_free(page, cc->page_pool);
			break;
		}

		size -= len;
	}

	if (!clone->bi_size) {
		bio_put(clone);
		return NULL;
	}

	return clone;
}

static void crypt_free_buffer_pages(struct oxcrypt_config *cc, struct bio *clone)
{
	unsigned int i;
	struct bio_vec *bv;

	for(i = 0; i < clone->bi_vcnt; i++) {
		bv = bio_iovec_idx(clone, i);
		BUG_ON(!bv->bv_page);
		mempool_free(bv->bv_page, cc->page_pool);
		bv->bv_page = NULL;
	}
}

/*
 * One of the bios was finished. Check for completion of
 * the whole request and correctly clean up the buffer.
 */
static void crypt_dec_pending(struct oxcrypt_io *io, int error)
{
	struct oxcrypt_config *cc = (struct oxcrypt_config *) io->target->private;

	if (error < 0)
		io->error = error;

	if (!atomic_dec_and_test(&io->pending))
		return;

	bio_endio(io->base_bio, io->error);

	mempool_free(io, cc->io_pool);
}

/*
 * kcryptd/kcryptd_io:
 *
 * Needed because it would be very unwise to do decryption in an
 * interrupt context.
 *
 * kcryptd performs the actual encryption or decryption.
 *
 * kcryptd_io performs the IO submission.
 *
 * They must be separated as otherwise the final stages could be
 * starved by new requests which can block in the first stages due
 * to memory allocation.
 */
static void kcryptd_do_work(struct work_struct *work);
static void kcryptd_do_crypt(struct work_struct *work);

static void kcryptd_queue_io(struct oxcrypt_io *io)
{
	struct oxcrypt_config *cc = io->target->private;

	INIT_WORK(&io->work, kcryptd_do_work);
	queue_work(cc->io_queue, &io->work);
}

static void kcryptd_queue_crypt(struct oxcrypt_io *io)
{
	struct oxcrypt_config *cc = io->target->private;

	INIT_WORK(&io->work, kcryptd_do_crypt);
	queue_work(cc->crypt_queue, &io->work);
}

static void crypt_endio(struct bio *clone, int error)
{
	struct oxcrypt_io *io = clone->bi_private;
	struct oxcrypt_config *cc = io->target->private;
	unsigned read_io = bio_data_dir(clone) == READ;

	if (unlikely(!bio_flagged(clone, BIO_UPTODATE) && !error))
		error = -EIO;

	/*
	 * free the processed pages
	 */
	if (!read_io) {
		crypt_free_buffer_pages(cc, clone);
		goto out;
	}

	if (unlikely(error))
		goto out;

	bio_put(clone);
	kcryptd_queue_crypt(io);
	return;

out:
	bio_put(clone);
	crypt_dec_pending(io, error);
}

static void clone_init(struct oxcrypt_io *io, struct bio *clone)
{
	struct oxcrypt_config *cc = io->target->private;

	clone->bi_private = io;
	clone->bi_end_io  = crypt_endio;
	clone->bi_bdev    = cc->dev->bdev;
	clone->bi_rw      = io->base_bio->bi_rw;
	clone->bi_destructor = dm_crypt_bio_destructor;
}

static void process_read(struct oxcrypt_io *io)
{
	struct oxcrypt_config *cc = io->target->private;
	struct bio *base_bio = io->base_bio;
	struct bio *clone;
	sector_t sector = base_bio->bi_sector - io->target->begin;

	atomic_inc(&io->pending);

	/*
	 * The block layer might modify the bvec array, so always
	 * copy the required bvecs because we need the original
	 * one in order to decrypt the whole bio data *afterwards*.
	 */
	clone = bio_alloc_bioset(GFP_NOIO, bio_segments(base_bio), cc->bs);
	if (unlikely(!clone)) {
		crypt_dec_pending(io, -ENOMEM);
		return;
	}

	clone_init(io, clone);
	clone->bi_idx = 0;
	clone->bi_vcnt = bio_segments(base_bio);
	clone->bi_size = base_bio->bi_size;
	clone->bi_sector = cc->start + sector;
	memcpy(clone->bi_io_vec, bio_iovec(base_bio),
	       sizeof(struct bio_vec) * clone->bi_vcnt);

	generic_make_request(clone);
}

static void process_write(struct oxcrypt_io *io)
{
	struct oxcrypt_config *cc = io->target->private;
	struct bio *base_bio = io->base_bio;
	struct bio *clone;
	struct convert_context ctx;
	unsigned remaining = base_bio->bi_size;
	sector_t sector = base_bio->bi_sector - io->target->begin;

	atomic_inc(&io->pending);

	crypt_convert_init(cc, &ctx, NULL, base_bio, sector, 1);

	/*
	 * The allocated buffers can be smaller than the whole bio,
	 * so repeat the whole process until all the data can be handled.
	 */
	while (remaining) {
		clone = crypt_alloc_buffer(io, remaining);
		if (unlikely(!clone)) {
			crypt_dec_pending(io, -ENOMEM);
			return;
		}

		ctx.bio_out = clone;
		ctx.idx_out = 0;

		if (unlikely(crypt_convert(cc, &ctx) < 0)) {
			crypt_free_buffer_pages(cc, clone);
			bio_put(clone);
			crypt_dec_pending(io, -EIO);
			return;
		}

		/* crypt_convert should have filled the clone bio */
		BUG_ON(ctx.idx_out < clone->bi_vcnt);

		clone->bi_sector = cc->start + sector;
		remaining -= clone->bi_size;
		sector += bio_sectors(clone);

		/* Grab another reference to the io struct
		 * before we kick off the request */
		if (remaining)
			atomic_inc(&io->pending);

		generic_make_request(clone);

		/* Do not reference clone after this - it
		 * may be gone already. */

		/* out of memory -> run queues */
		if (remaining)
			congestion_wait(WRITE, HZ/100);
	}
}

static void process_read_endio(struct oxcrypt_io *io)
{
	struct oxcrypt_config *cc = io->target->private;
	struct convert_context ctx;

	crypt_convert_init(cc, &ctx, io->base_bio, io->base_bio,
			   io->base_bio->bi_sector - io->target->begin, 0);

	crypt_dec_pending(io, crypt_convert(cc, &ctx));
}

static void kcryptd_do_work(struct work_struct *work)
{
	struct oxcrypt_io *io = container_of(work, struct oxcrypt_io, work);

	if (bio_data_dir(io->base_bio) == READ)
		process_read(io);
}

static void kcryptd_do_crypt(struct work_struct *work)
{
	struct oxcrypt_io *io = container_of(work, struct oxcrypt_io, work);

	if (bio_data_dir(io->base_bio) == READ)
		process_read_endio(io);
	else
		process_write(io);
}

/*
 * Decode key from its hex representation
 */
static int crypt_decode_key(u8 *key, char *hex, unsigned int size)
{
	char buffer[3];
	char *endp;
	unsigned int i;

	buffer[2] = '\0';

	for(i = 0; i < size; i++) {
		buffer[0] = *hex++;
		buffer[1] = *hex++;

		key[i] = (u8)simple_strtoul(buffer, &endp, 16);

		if (endp != &buffer[2])
			return -EINVAL;
	}

	if (*hex != '\0')
		return -EINVAL;

	return 0;
}

/*
 * Construct an encryption mapping:
 * <key> <key> <iv_offset> <dev_path> <start>
 */
static int oxcrypt_ctr(struct dm_target *ti, unsigned int argc, char **argv)
{
	struct oxcrypt_config *cc;
	unsigned long long tmpll;

	if (argc != 5) {
		ti->error = "Not enough arguments";
		return -EINVAL;
	}

    /* allocate memory for the encryption contect */
 	cc = kzalloc(sizeof(*cc), GFP_KERNEL);
	if (cc == NULL) {
		ti->error =
			"Cannot allocate transparent encryption context";
		return -ENOMEM;
	}

    /* read the keys in */
	if (crypt_decode_key(cc->key, argv[0], OX800DPE_KEYSIZE) < 0) {
		ti->error = DM_MSG_PREFIX "Error decoding key";
		goto bad_cipher;
	}

	if (crypt_decode_key(cc->iv_key, argv[1], OX800DPE_KEYSIZE) < 0) {
		ti->error = DM_MSG_PREFIX "Error decoding iv key";
		goto bad_cipher;
	}
    
	set_bit(DM_CRYPT_KEY_VALID, &cc->flags);
    
	cc->io_pool = mempool_create_slab_pool(MIN_IOS, _oxcrypt_io_pool);
	if (!cc->io_pool) {
		ti->error = "Cannot allocate crypt io mempool";
		goto bad_cipher;
	}

	cc->page_pool = mempool_create_page_pool(MIN_POOL_PAGES, 0);
	if (!cc->page_pool) {
		ti->error = "Cannot allocate page mempool";
		goto bad_page_pool;
	}

	cc->bs = bioset_create(MIN_IOS, MIN_IOS);
	if (!cc->bs) {
		ti->error = "Cannot allocate crypt bioset";
		goto bad_bs;
	}

    /* check the IV offset */
	if (sscanf(argv[2], "%llu", &tmpll) != 1) {
		ti->error = "Invalid iv_offset sector";
		goto bad_device;
	}
	cc->iv_offset = tmpll;

    /* get the start sector */
	if (sscanf(argv[4], "%llu", &tmpll) != 1) {
		ti->error = "Invalid device sector";
		goto bad_device;
	}
	cc->start = tmpll;

    /* get the device */
	if (dm_get_device(ti, argv[3], cc->start, ti->len, dm_table_get_mode(ti->table), &cc->dev)) {
		ti->error = "Device lookup failed";
		goto bad_device;
	}

    /* Create a work queue for io */
	cc->io_queue = create_singlethread_workqueue("kcryptd_io");
	if (!cc->io_queue) {
		ti->error = "Couldn't create kcryptd io queue";
		goto bad_io_queue;
	}

    /* create a work queue for cryptography */
	cc->crypt_queue = create_singlethread_workqueue("kcryptd");
	if (!cc->crypt_queue) {
		ti->error = "Couldn't create kcryptd queue";
		goto bad_crypt_queue;
	}

	ti->private = cc;
	return 0;

bad_crypt_queue:
	destroy_workqueue(cc->io_queue);
bad_io_queue:
bad_device:
	bioset_free(cc->bs);
bad_bs:
	mempool_destroy(cc->page_pool);
bad_page_pool:
	mempool_destroy(cc->io_pool);
bad_cipher:
	/* Must zero key material before freeing */
	memset(cc, 0, sizeof(*cc) );
	kfree(cc);
	return -EINVAL;
}

static void oxcrypt_dtr(struct dm_target *ti)
{
	struct oxcrypt_config *cc = (struct oxcrypt_config *) ti->private;

	destroy_workqueue(cc->io_queue);
	destroy_workqueue(cc->crypt_queue);

	bioset_free(cc->bs);
	mempool_destroy(cc->page_pool);
	mempool_destroy(cc->io_pool);

	crypto_free_blkcipher(cc->tfm);
	dm_put_device(ti, cc->dev);

	/* Must zero key material before freeing */
	memset(cc->key, 0, OX800DPE_KEYSIZE);
	memset(cc->iv_key, 0, OX800DPE_KEYSIZE);
	kfree(cc);
}

static int oxcrypt_map(struct dm_target *ti, struct bio *bio,
		     union map_info *map_context)
{
	struct oxcrypt_config *cc = ti->private;
	struct oxcrypt_io *io;

	io = mempool_alloc(cc->io_pool, GFP_NOIO);
	io->target = ti;
	io->base_bio = bio;
	io->error = 0;
	atomic_set(&io->pending, 0);

	if (bio_data_dir(io->base_bio) == READ)
		kcryptd_queue_io(io);
	else
		kcryptd_queue_crypt(io);

	return DM_MAPIO_SUBMITTED;
}

static int oxcrypt_status(struct dm_target *ti, status_type_t type,
			char *result, unsigned int maxlen)
{
	struct oxcrypt_config *cc = (struct oxcrypt_config* )ti->private;
	char buffer[32];
	unsigned int sz = 0;

	switch (type) {
	case STATUSTYPE_INFO:
		result[0] = '\0';
		break;

	case STATUSTYPE_TABLE:
        /* encryption type */
        DMEMIT("AES-ECB ");
        
        /* offset encrypted-device start */
		format_dev_t(buffer, cc->dev->bdev->bd_dev);
		DMEMIT(" %llu %s %llu", (unsigned long long)cc->iv_offset,
            buffer, (unsigned long long)cc->start);
		break;
	}
	return 0;
}

static void oxcrypt_postsuspend(struct dm_target *ti)
{
	struct oxcrypt_config *cc = ti->private;

	set_bit(DM_CRYPT_SUSPENDED, &cc->flags);
}

static int oxcrypt_preresume(struct dm_target *ti)
{
	struct oxcrypt_config *cc = ti->private;

	if (!test_bit(DM_CRYPT_KEY_VALID, &cc->flags)) {
		DMERR("aborting resume - crypt key is not set.");
		return -EAGAIN;
	}

	return 0;
}

static void oxcrypt_resume(struct dm_target *ti)
{
	struct oxcrypt_config *cc = ti->private;

	clear_bit(DM_CRYPT_SUSPENDED, &cc->flags);
}

static struct target_type oxcrypt_target = {
	.name   = "ox-crypt",
	.version= {1, 2, 0},
	.module = THIS_MODULE,
	.ctr    = oxcrypt_ctr,
	.dtr    = oxcrypt_dtr,
	.map    = oxcrypt_map,
	.status = oxcrypt_status,
	.postsuspend = oxcrypt_postsuspend,
	.preresume = oxcrypt_preresume,
	.resume = oxcrypt_resume,
};

static int __init dm_oxcrypt_init(void)
{
	int r;

	_oxcrypt_io_pool = KMEM_CACHE(oxcrypt_io, 0);
	if (!_oxcrypt_io_pool)
		return -ENOMEM;

	r = dm_register_target(&oxcrypt_target);
	if (r < 0) {
		DMERR("register failed %d", r);
		kmem_cache_destroy(_oxcrypt_io_pool);
	}

	return r;
}

static void __exit dm_oxcrypt_exit(void)
{
	int r = dm_unregister_target(&oxcrypt_target);

	if (r < 0)
		DMERR("unregister failed %d", r);

	kmem_cache_destroy(_oxcrypt_io_pool);
}

module_init(dm_oxcrypt_init);
module_exit(dm_oxcrypt_exit);

MODULE_AUTHOR("Oxford Semiconductor, based on work of Christophe Saout");
MODULE_DESCRIPTION(DM_NAME " target for hardware encryption / decryption");
MODULE_LICENSE("GPL");
