/*
 * include/asm-arm/plat-oxnas/oxnas_net.h
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
#if !defined(__OXNAS_NET_H__)
#define __OXNAS_NET_H__

#include <linux/bio.h>

// The amount of header to copy from a receive packet into the skb buffer to
// normally avoid having to pull from fragments during receive stack processing
static const int GMAC_HLEN = 74;

typedef struct frag_list_entry {
	struct list_head head;
	struct bio_vec   bio_vec;
} frag_list_entry_t;

static inline void frag_list_entry_ctor(
	struct kmem_cache *cache,
	void              *object)
{
	frag_list_entry_t *entry = object;
	INIT_LIST_HEAD(&entry->head);
}

typedef struct oxnas_net_rx_context {
	struct list_head   page_info;
	int				   page_cnt;
	struct kmem_cache *frag_cache;
} oxnas_net_rx_context_t;

extern void oxnas_net_rx_actor(
	read_descriptor_t *desc,
	struct sk_buff    *skb,
	u32                offset);

extern void release_netdma_net_frags(oxnas_net_rx_context_t *context);

#endif        //  #if !defined(__GMAC_H__)
