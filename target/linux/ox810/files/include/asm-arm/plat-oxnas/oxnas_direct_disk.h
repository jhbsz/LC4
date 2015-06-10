/*
 * include/asm-arm/plat-oxnas/oxnas_direct_disk.h
 *
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
 */
#include <linux/fs.h>
#include <asm/plat-oxnas/dma.h>
#include <asm/plat-oxnas/oxnas_net.h>

#ifndef __OXNAS_DIRECT_DISK_H__
#define __OXNAS_DIRECT_DISK_H__

typedef struct getbmap {
	long long bmv_offset;	/* file offset of segment in blocks */
	long long bmv_block;	/* starting block (64-bit daddr_t)  */
	long long bmv_length;	/* length of segment, blocks	    */
	long	  bmv_count;	/* # of entries in array incl. 1st  */
	long	  bmv_entries;	/* # of entries filled in (output). */
} getbmap_t;

typedef struct getbmapx {
	long long bmv_offset;	/* file offset of segment in blocks */
	long long bmv_block;	/* starting block (64-bit daddr_t)  */
	long long bmv_length;	/* length of segment, blocks	    */
	long	  bmv_count;	/* # of entries in array incl. 1st  */
	long	  bmv_entries;	/* # of entries filled in (output). */
	long	  bmv_iflags;	/* input flags (1st structure)	    */
	long	  bmv_oflags;	/* output flags (after 1st structure)*/
	long	  bmv_unused1;	/* future use			    */
	long	  bmv_unused2;	/* future use			    */
} getbmapx_t;

typedef struct oxnas_filemap_info {
	getbmapx_t			 *map;
	int					  cur_map_entry;
	long long			  cur_map_entry_offset;	/* in sectors */
} oxnas_filemap_info_t;

typedef struct oxnas_direct_disk_dma {
	struct ata_prd		 *dma_sg_prds;
	oxnas_dma_sg_entry_t *dma_sg_descs;
	int					  dma_sg_cnt;
	oxnas_dma_channel_t	 *dma_chan;
} oxnas_direct_disk_dma_t;

typedef struct oxnas_direct_disk_context {
	oxnas_filemap_info_t	filemap_info;
	oxnas_direct_disk_dma_t dma_info;
} oxnas_direct_disk_context_t;

extern int oxnas_direct_disk_init(
	oxnas_direct_disk_context_t *disk_context);

extern int oxnas_direct_disk_write(
	oxnas_net_rx_context_t 	  	*context,
	oxnas_direct_disk_context_t	*disk_context,
	ssize_t						 length,
	int							 port,
	sector_t					 part_offset);

extern int oxnas_bio_disk_write(
	oxnas_net_rx_context_t *context,
	oxnas_filemap_info_t   *filemap,
	ssize_t					length,
	struct block_device	   *bdev);

extern void oxnas_direct_disk_cleanup(
	oxnas_direct_disk_context_t *disk_context);

#endif        //  #ifndef __OXNAS_DIRECT_DISK_H__
