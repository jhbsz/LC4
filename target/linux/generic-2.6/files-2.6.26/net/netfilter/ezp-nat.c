#include <linux/ip.h>
#include <linux/socket.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/imq.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <net/dst.h>
#include <net/ip.h>
#include <net/route.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_core.h>
#include <net/netfilter/nf_queue.h>
#include <linux/netfilter_ipv4.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>

#define MOD_INC_USE_COUNT
#define MOD_DEC_USE_COUNT
/* Make sure this hook is incurred before nat pre-route hook */
#define EZP_NAT_PREROUTE_PRIORITY (NF_IP_PRI_NAT_DST - 3)
#define EZP_NAT_FORWARD_PRIORITY (NF_IP_PRI_FIRST + 1)
#define EZP_NAT_INPUT_PRIORITY (NF_IP_PRI_FILTER + 1)
#define EZP_NAT_OUTPUT_PRIORITY (NF_IP_PRI_FILTER - 3)
#define EZP_NAT_OUTPUT_LAST_PRIORITY (NF_IP_PRI_NAT_SRC + 1)
#define EZP_NAT_IMQ_INITIALISED_FLAG (IMQ_F_ENQUEUE >> 1)
#define EZP_NAT_IMQ_INITIALISED_MASK (~EZP_NAT_IMQ_INITIALISED_FLAG)

#define ASSERT_DEBUG(expr) if(!(expr)) {printk("%s at %s:%s\n", #expr, __FILE__, __LINE__);}
#define ASSERT(Expr)\
        if(!(Expr)){            \
                    printk("Assertion Failed! %s,%s,%s, line = %d\n",\
#Expr,__FILE__,__FUNCTION__,__LINE__);  \
                }

#define EZP_NAT_INITIALISED_BIT (IP_NAT_MANIP_DST + 1)
#define EZP_NAT_DROP_BIT (IP_NAT_MANIP_DST + 2)
#define TCPHDR(skbuff) ((struct tcphdr*)((u_int32_t *)(skbuff->nh.iph) + skbuff->nh.iph->ihl))

#define EZP_NAT_DEBUG_TRACE 3
#define EZP_WRITE_MAX_SIZE 64

/* We use the same nat flags variable to represent LOCAL traffic. */
#define EZP_IP_LOCAL_IN 4
#define EZP_IP_LOCAL_OUT 5

/* Flag to enable/disable ezp-nat fast path.
 * */
char ezp_nat_enable_flag = 0;
char ezp_nat_queue_enable_flag = 0;
struct proc_dir_entry *proc_ezp_nat;
struct proc_dir_entry *proc_ezp_nat_queue;
char p[EZP_WRITE_MAX_SIZE];

int ezp_nat_proc_read(char *page, char **start, off_t offset, int count, int *eof, void *data)
{
    char *p = page;
    int len;

    if (offset > 0) {
        *eof = 1;
        return 0;
    }

    *(p++) = ezp_nat_enable_flag + '0';
    *(p++) = '\n';
    *(p++) = 0;

    len = 2;
    return len;
}

int ezp_nat_proc_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
    if (!count) {
        return -EINVAL;
    }

    if (count > EZP_WRITE_MAX_SIZE) {
        count = EZP_WRITE_MAX_SIZE;
    }
    
    /* Copy the data from user space. */
    if (copy_from_user(p, buffer, count)) {
        return -EFAULT;
    }
    
    switch (p[0]){
    case '0':
        ezp_nat_enable_flag = 0;
        break;
    case '1':
        ezp_nat_enable_flag = 1;
        break;
    default:
        break;
    }
    return 1;
}

int ezp_nat_queue_proc_read(char *page, char **start, off_t offset, int count, int *eof, void *data)
{
    char *p = page;
    int len;

    if (offset > 0) {
        *eof = 1;
        return 0;
    }

    *(p++) = ezp_nat_queue_enable_flag + '0';
    *(p++) = '\n';
    *(p++) = 0;

    len = 2;
    return len;
}

int ezp_nat_queue_proc_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
    
    if (!count) {
        return -EINVAL;
    }

    if (count > EZP_WRITE_MAX_SIZE) {
        count = EZP_WRITE_MAX_SIZE;
    }
    
    /* Copy the data from user space. */
    if (copy_from_user(p, buffer, count)) {
        return -EFAULT;
    }
    
    switch (p[0]){
    case '0':
        ezp_nat_queue_enable_flag = 0;
        break;
    case '1':
        ezp_nat_queue_enable_flag = 1;
        break;
    default:
        break;
    }
    return 1;
}

/* after ipt_filter */
static unsigned int ezp_nat_pre_hook(unsigned int hooknum, 
        struct sk_buff *skb, const struct net_device *indev,
        const struct net_device *outdev, 
        int (*okfn)(struct sk_buff *))
{
    struct nf_conn *ct;
    enum ip_conntrack_info ctinfo;
    int ret = NF_ACCEPT;
    enum ip_conntrack_dir dir;
    __u32 dnat_addr = 0, snat_addr = 0;    
    int* nat_flag;
    struct dst_entry** dst_to_use = NULL;
    struct iphdr *iph = ip_hdr(skb);
    struct icmphdr *hdr = icmp_hdr(skb);
    struct tcphdr *tcph = tcp_hdr(skb);
    /* EZP: enum nf_nat_manip_type maniptype = HOOK2MANIP(hooknum); */

    if(!ezp_nat_enable_flag){
        return NF_ACCEPT;
    }
    ct = nf_ct_get(skb, &ctinfo);
    if (!ct) {
        if (iph->protocol == IPPROTO_ICMP
                && hdr->type == ICMP_REDIRECT)
            return NF_DROP;
        return NF_ACCEPT;
    }
    
    /* TCP or UDP. */
    if ((iph->protocol != IPPROTO_TCP) &&
            (iph->protocol != IPPROTO_UDP) ) {
        return NF_ACCEPT;
    }

    /* bypass multicast and broadcast */
    if ((ipv4_is_lbcast(ip_hdr(skb)->daddr)) ||
            (ipv4_is_multicast(ip_hdr(skb)->daddr))) {
        return NF_ACCEPT;
    }

    if ((iph->protocol == IPPROTO_TCP) && 
            ((tcp_flag_word(tcph) & (TCP_FLAG_RST | TCP_FLAG_SYN)) == 
             TCP_FLAG_SYN)) {
        return NF_ACCEPT;
    }
    /* Make sure it is confirmed. */
    if (!nf_ct_is_confirmed(ct)) {
        return NF_ACCEPT;
    } 
    /* We comment out this part since
    ((tcp_flag_word((*pskb)->h.th) == TCP_FLAG_SYN) ||
     * 1. conntrack establishing is a 2 way process, but after routing, we have
     * established routing entry and address resolution table, so we don't
     * need to check ESTABLISH state.
     * 2. With establishing state, we need to go through forward state and 
     * routing several times. It may occur that our holded entry may be
     * replaced. */
    /*
    if ((ctinfo != IP_CT_ESTABLISHED) &&
        (ctinfo != IP_CT_ESTABLISHED+IP_CT_IS_REPLY)) {
        return NF_ACCEPT;
    }
    */
    dir = CTINFO2DIR(ctinfo);
    if (dir == IP_CT_DIR_ORIGINAL) {
        if (!ct->orgdir_dst) {
            return NF_ACCEPT;
        } else {
            nat_flag = &ct->orgdir_rid;
            if (!(*nat_flag & ((1 << IP_NAT_MANIP_DST) | 
			(1 << IP_NAT_MANIP_SRC) |
			(1 << EZP_IP_LOCAL_IN)))) {
                return NF_ACCEPT;
            }
            /* Check only in forward case and ignore input case */
            if (!(*nat_flag & (1 << EZP_IP_LOCAL_IN))) {
                if ((!ct->orgdir_dst->hh) && (!ct->orgdir_dst->neighbour)) {
                    printk("%s:orig dst and neighbour null dir\n",__FUNCTION__);
                    return NF_ACCEPT;
                }
            }
            if (skb->dst) {
                /* skb might has its own dst already. 
                 * e.g. output to local input */
                dst_release(skb->dst);
            } 
            skb->protocol = htons(ETH_P_IP);
            skb->dst = ct->orgdir_dst;
            /* XXX: */
            skb->dev = ct->orgdir_dst->dev;
            /* skb uses this dst_entry */
            dst_use(skb->dst, jiffies);
            dst_to_use = &ct->orgdir_dst;
        }
    } else {
        /* IP_CT_DIR_REPLY */
        if (!ct->replydir_dst) {
            return NF_ACCEPT;
        } else {
            nat_flag = &ct->replydir_rid;
            if (!(*nat_flag & ((1 << IP_NAT_MANIP_DST) | 
			(1 << IP_NAT_MANIP_SRC) |
			(1 << EZP_IP_LOCAL_IN)))) {
                return NF_ACCEPT;
            }
            /* Check only in forward case and ignore input case */
            if (!(*nat_flag & (1 << EZP_IP_LOCAL_IN))) {
                if ((!ct->replydir_dst->hh) && (!ct->replydir_dst->neighbour)) {
                    printk("%s:reply dst and neighbour null\n",__FUNCTION__);
                    return NF_ACCEPT;
                }
            } 
            if (skb->dst) {
                /* skb might has its own dst already. */
                /* e.g. output to local input */
                dst_release(skb->dst);
            } 
            skb->protocol = htons(ETH_P_IP);
            skb->dst = ct->replydir_dst;
            /* XXX: */
            skb->dev = ct->replydir_dst->dev;
            /* skb uses this dst_entry */
            dst_use(skb->dst, jiffies);
            dst_to_use = &ct->replydir_dst;
        }
    }

    /* After this point, every "return NF_ACCEPT" action need to release
     * holded dst entry. So we use "goto release_dst_and_return" to handle the
     * action commonly. */
    /* EZP:
    if (!nf_nat_initialized(ct, maniptype)) {
        goto release_dst_and_return;
    }
    */
    /* If we have helper, we need to go original path until conntrack
     * confirmed */
    if(nfct_help(ct)){
        goto release_dst_and_return;
    }

    if (dir == IP_CT_DIR_ORIGINAL) {
        (skb)->imq_flags = ct->ct_orig_imq_flags;
    }
    else{
        (skb)->imq_flags = ct->ct_repl_imq_flags;
    }

    /* PRE_ROUTING NAT */
    /* Assume DNAT conntrack is ready. */
    if ((*nat_flag & (1 << IP_NAT_MANIP_DST))){
        dnat_addr = iph->daddr;
        ret = nf_nat_packet(ct, ctinfo, NF_INET_PRE_ROUTING, skb);
        if (ret != NF_ACCEPT) {
            goto release_dst_and_return;
        }
        if (dnat_addr == iph->daddr) {
            *nat_flag &= ~(1 << IP_NAT_MANIP_DST);
        }
    } 
    /* INPUT */
    if ((*nat_flag & (1 << EZP_IP_LOCAL_IN))){
        /* TODO: use ip_local_deliver_finish() and add ip_defrag(). */
        /* XXX: Not sure this will hit or not. */
        /*
         *	Reassemble IP fragments.
         */

        if (ip_hdr(skb)->frag_off & htons(IP_MF | IP_OFFSET)) {
            if (ip_defrag(skb, IP_DEFRAG_LOCAL_DELIVER)) {
                /* If return value is not 0, defrag error */
                /* return 0; */
                /* XXX: return NF_STOLEN? */
                goto release_dst_and_return;
            }
        }
        /* For INPUT path, there is no need to check dst_mtu but defrag.
        if (skb->len > dst_mtu(&((struct rtable*)skb->dst)->u.dst)) {
            goto release_dst_and_return;
        }*/
        if (ezp_nat_queue_enable_flag) {
        if ((skb)->imq_flags & IMQ_F_ENQUEUE) {
            struct nf_hook_ops *elem = nf_get_imq_ops();
            /* With to apply IMQ, we have to check the IMQ flag, if the flag is
             * set, we have to enquene this skb and leave it to IMQ*/
            if (elem != NULL) {
                nf_queue(skb, (struct list_head*)elem, AF_INET, 
                        NF_INET_POST_ROUTING, 
                        (struct net_device*)indev, 
                        (struct net_device*)
                            ((struct rtable*)skb->dst)->u.dst.dev, 
                        ip_local_deliver_finish, NF_ACCEPT >> NF_VERDICT_BITS);
                return NF_STOLEN;
            }
        }
        }
        ret = ip_local_deliver_finish(skb);
        return NF_STOLEN;
    }

    /* POST_ROUTING NAT */
    /* Assume SNAT conntrack is ready. */
    if (((*nat_flag) & (1 << IP_NAT_MANIP_SRC))){
        snat_addr = iph->saddr;
        ret = nf_nat_packet(ct, ctinfo, NF_INET_POST_ROUTING, skb);
        if (ret != NF_ACCEPT) {
            goto release_dst_and_return;
        }
        if (snat_addr == iph->saddr) {
            *nat_flag &= ~(1 << IP_NAT_MANIP_SRC);
        }
    }
    
    ASSERT(skb->dst);
    ASSERT(skb->dst->dev);

    /* Compare socket buffer length with routing PMTU, if false, 
     *we have to do refregment */
    if (skb->len > dst_mtu(&((struct rtable*)skb->dst)->u.dst)) {
        goto release_dst_and_return;
    }

    if (ezp_nat_queue_enable_flag) {
    if ((skb)->imq_flags & IMQ_F_ENQUEUE) {
        struct nf_hook_ops *elem = nf_get_imq_ops();
        /* With to apply IMQ, we have to check the IMQ flag, if the flag is
         * set, we have to enquene this skb and leave it to IMQ*/
        if (elem != NULL) {
            nf_queue(skb, (struct list_head*)elem, AF_INET, NF_INET_POST_ROUTING, 
                    (struct net_device*)indev, 
                    (struct net_device*)((struct rtable*)skb->dst)->u.dst.dev, 
                    ip_finish_output2, NF_ACCEPT >> NF_VERDICT_BITS);
            return NF_STOLEN;
        }
    }
    }
    ret = ip_finish_output2(skb);
    /* After calling ip_finish_output2, we need to return to netfilter with
     * NF_STOLEN. */
    return NF_STOLEN;
release_dst_and_return:
    /* Release dst entry and fallback to netfilter original flow. */
    ASSERT(ct);
    ASSERT(*dst_to_use);
    dst_release(*dst_to_use);
    *dst_to_use = NULL;
    return NF_ACCEPT;
}

static unsigned int ezp_nat_output_hook(unsigned int hooknum, 
        struct sk_buff *skb, const struct net_device *indev,
        const struct net_device *outdev, 
        int (*okfn)(struct sk_buff *))
{
    struct nf_conn *ct;
    enum ip_conntrack_info ctinfo;
    int ret = NF_ACCEPT;
    enum ip_conntrack_dir dir;
    __u32 snat_addr = 0;    
    int* nat_flag;
    struct dst_entry** dst_to_use = NULL;
    struct iphdr *iph = ip_hdr(skb);
    struct icmphdr *hdr = icmp_hdr(skb);
    struct tcphdr *tcph = tcp_hdr(skb);
    /* EZP: enum nf_nat_manip_type maniptype = HOOK2MANIP(hooknum); */

    if(!ezp_nat_enable_flag){
        return NF_ACCEPT;
    }
    ct = nf_ct_get(skb, &ctinfo);
    if (!ct) {
        if (iph->protocol == IPPROTO_ICMP
                && hdr->type == ICMP_REDIRECT)
            return NF_DROP;
        return NF_ACCEPT;
    }
    
    /* TCP or UDP. */
    if ((iph->protocol != IPPROTO_TCP) &&
            (iph->protocol != IPPROTO_UDP) ) {
        return NF_ACCEPT;
    }

    /* bypass multicast and broadcast */
    if ((ipv4_is_lbcast(ip_hdr(skb)->daddr)) ||
            (ipv4_is_multicast(ip_hdr(skb)->daddr))) {
        return NF_ACCEPT;
    }

    if ((iph->protocol == IPPROTO_TCP) && 
            ((tcp_flag_word(tcph) & (TCP_FLAG_RST | TCP_FLAG_SYN)) == 
             TCP_FLAG_SYN)) {
        return NF_ACCEPT;
    }
    /* Make sure it is confirmed.  */
    if (!nf_ct_is_confirmed(ct)) {
        return NF_ACCEPT;
    } 
    /* We comment out this part since
    ((tcp_flag_word((*pskb)->h.th) == TCP_FLAG_SYN) ||
     * 1. conntrack establishing is a 2 way process, but after routing, we have
     * established routing entry and address resolution table, so we don't
     * need to check ESTABLISH state.
     * 2. With establishing state, we need to go through forward state and 
     * routing several times. It may occur that our holded entry may be
     * replaced. */
    /*
    if ((ctinfo != IP_CT_ESTABLISHED) &&
        (ctinfo != IP_CT_ESTABLISHED+IP_CT_IS_REPLY)) {
        return NF_ACCEPT;
    }
    */
    dir = CTINFO2DIR(ctinfo);
    if (dir == IP_CT_DIR_ORIGINAL) {
        if (!ct->orgdir_dst) {
            return NF_ACCEPT;
        } else {
            nat_flag = &ct->orgdir_rid;
            if (!(*nat_flag & ((1 << IP_NAT_MANIP_SRC) |
			(1 << EZP_IP_LOCAL_OUT)))) {
                return NF_ACCEPT;
            }
            if ((!ct->orgdir_dst->hh) && (!ct->orgdir_dst->neighbour)) {
                printk("%s:orig dst and neighbour null dir\n",__FUNCTION__);
                return NF_ACCEPT;
            }
            if (skb->dst) {
                /* skb might has its own dst already. 
                 * e.g. output to local input */
                dst_release(skb->dst);
            } 

            skb->protocol = htons(ETH_P_IP);
            skb->dst = ct->orgdir_dst;
            skb->dev = ct->orgdir_dst->dev;
            /* skb uses this dst_entry */
            dst_use(skb->dst, jiffies);
            dst_to_use = &ct->orgdir_dst;
        }
    } else {
        /* IP_CT_DIR_REPLY */
        if (!ct->replydir_dst) {
            return NF_ACCEPT;
        } else {
            nat_flag = &ct->replydir_rid;
            if (!(*nat_flag & ((1 << IP_NAT_MANIP_SRC) |
			(1 << EZP_IP_LOCAL_OUT)))) {
                return NF_ACCEPT;
            }
            if ((!ct->replydir_dst->hh) && (!ct->replydir_dst->neighbour)) {
                printk("%s:reply dst and neighbour null\n",__FUNCTION__);
                return NF_ACCEPT;
            }
            if (skb->dst) {
                /* skb might has its own dst already. 
                 * e.g. output to local input */
                dst_release(skb->dst);
            } 
            skb->protocol = htons(ETH_P_IP);
            skb->dst = ct->replydir_dst;
            skb->dev = ct->replydir_dst->dev;
            /* skb uses this dst_entry */
            dst_use(skb->dst, jiffies);
            dst_to_use = &ct->replydir_dst;
        }
    }

    /* After this point, every "return NF_ACCEPT" action need to release
     * holded dst entry. So we use "goto release_dst_and_return" to handle the
     * action commonly. */
    /* EZP:
    if (!nf_nat_initialized(ct, maniptype)) {
        goto release_dst_and_return;
    }
    */
    /* If we have helper, we need to go original path until conntrack
     * confirmed */
    if(nfct_help(ct)){
        goto release_dst_and_return;
    }

    if (dir == IP_CT_DIR_ORIGINAL) {
        (skb)->imq_flags = ct->ct_orig_imq_flags;
    }
    else{
        (skb)->imq_flags = ct->ct_repl_imq_flags;
    }

    /* XXX: Loopback! */
    /* POST_ROUTING NAT */
    if (((*nat_flag) & (1 << IP_NAT_MANIP_SRC))){
        snat_addr = iph->saddr;
        ret = nf_nat_packet(ct, ctinfo, NF_INET_POST_ROUTING, skb);
        if (ret != NF_ACCEPT) {
            goto release_dst_and_return;
        }
        if (snat_addr == iph->saddr) {
            *nat_flag &= ~(1 << IP_NAT_MANIP_SRC);
        }
    }
    
    ASSERT(skb->dst);
    ASSERT(skb->dst->dev);

    /* Compare socket buffer length with routing PMTU, if false, 
     *we have to do refregment */
    if (skb->len > dst_mtu(&((struct rtable*)skb->dst)->u.dst)) {
        goto release_dst_and_return;
    }

    if (ezp_nat_queue_enable_flag) {
    if ((skb)->imq_flags & IMQ_F_ENQUEUE) {
        struct nf_hook_ops *elem = nf_get_imq_ops();
        /* With to apply IMQ, we have to check the IMQ flag, if the flag is
         * set, we have to enquene this skb and leave it to IMQ*/
        if (elem != NULL) {
            nf_queue(skb, (struct list_head*)elem, AF_INET, NF_INET_POST_ROUTING, 
                    (struct net_device*)indev, 
                    (struct net_device*)((struct rtable*)skb->dst)->u.dst.dev, 
                    ip_finish_output2, NF_ACCEPT >> NF_VERDICT_BITS);
            return NF_STOLEN;
        }
    }
    }
    ret = ip_finish_output2(skb);
    /* After calling ip_finish_output2, we need to return to netfilter with
     * NF_STOLEN. */
    return NF_STOLEN;
release_dst_and_return:
    /* Release dst entry and fallback to netfilter original flow. */
    ASSERT(ct);
    ASSERT(*dst_to_use);
    dst_release(*dst_to_use);
    *dst_to_use = NULL;
    return NF_ACCEPT;
}

static unsigned int
ezp_nat_fw_hook(unsigned int hooknum, struct sk_buff *skb,
                             const struct net_device *indev,
                             const struct net_device *outdev,
	                     int (*okfn)(struct sk_buff *))
{
    struct nf_conn *ct;
    enum ip_conntrack_info ctinfo;
    enum ip_conntrack_dir dir;
    struct iphdr *iph = ip_hdr(skb);
    
    if(!ezp_nat_enable_flag){
        return NF_ACCEPT;
    }
    ct = nf_ct_get(skb, &ctinfo);
    if(!ct){
        return NF_ACCEPT;
    }
    dir = CTINFO2DIR(ctinfo);
    ASSERT(skb->dst);
    ASSERT(skb->dst->dev);
    /* TCP or UDP. */
    if ((iph->protocol != IPPROTO_TCP) &&
            (iph->protocol != IPPROTO_UDP) ) {
        return NF_ACCEPT;
    }

    /* bypass multicast and broadcast */
    if ((ipv4_is_lbcast(ip_hdr(skb)->daddr)) ||
            (ipv4_is_multicast(ip_hdr(skb)->daddr))) {
        return NF_ACCEPT;
    }

    /* Make sure it is confirmed. */
    if (!nf_ct_is_confirmed(ct)) {
        return NF_ACCEPT;
    }

    if ( dir == IP_CT_DIR_ORIGINAL ) {
        /* If routing table cache and address resolution table are released,
         * or the socket buffer is allocated a new routing table, we have to 
         * release our original holded dst.*/
        if (ct->orgdir_dst && (ct->orgdir_dst != skb->dst)) {
            dst_release(ct->orgdir_dst);
            ct->orgdir_dst = NULL;
        }

        if ( !ct->orgdir_dst) {
            ct->orgdir_dst = skb->dst;
            /* this conntrack uses this dst_entry */
            dst_use(ct->orgdir_dst, jiffies);
            ct->orgdir_rid |= ((1 << IP_NAT_MANIP_SRC) | (1 << IP_NAT_MANIP_DST));
        }
    } 
    else if ( dir == IP_CT_DIR_REPLY ) {
        /* If routing table cache and address resolution table are released,
         * or the socket buffer is allocated a new routing table, we have to 
         * release our original holded dst.*/
        if (ct->replydir_dst && (ct->replydir_dst != skb->dst)) {
            dst_release(ct->replydir_dst);
            ct->replydir_dst = NULL;
        }

        if ( !ct->replydir_dst) {
            ct->replydir_dst = skb->dst;
            /* this conntrack uses this dst_entry */
            dst_use(ct->replydir_dst, jiffies);
            ct->replydir_rid |= ((1 << IP_NAT_MANIP_SRC) | (1 << IP_NAT_MANIP_DST));
        }
    }
    return NF_ACCEPT;
}

/* Set up the nat flags for nat_flags. */
static unsigned int
ezp_nat_input_last_hook(unsigned int hooknum, struct sk_buff *skb,
                             const struct net_device *indev,
                             const struct net_device *outdev,
	                     int (*okfn)(struct sk_buff *))
{
    struct nf_conn *ct;
    enum ip_conntrack_info ctinfo;
    enum ip_conntrack_dir dir;
    struct iphdr *iph = ip_hdr(skb);
    
    if(!ezp_nat_enable_flag){
        return NF_ACCEPT;
    }
    ct = nf_ct_get(skb, &ctinfo);
    if(!ct){
        return NF_ACCEPT;
    }
    /* Make sure it is confirmed. */
    if (!nf_ct_is_confirmed(ct)) {
        return NF_ACCEPT;
    }
    dir = CTINFO2DIR(ctinfo);
    ASSERT(skb->dst);
    ASSERT(skb->dst->dev);
    /* TCP or UDP. */
    if ((iph->protocol != IPPROTO_TCP) &&
            (iph->protocol != IPPROTO_UDP) ) {
        return NF_ACCEPT;
    }

    /* bypass multicast and broadcast */
    if ((ipv4_is_lbcast(ip_hdr(skb)->daddr)) ||
            (ipv4_is_multicast(ip_hdr(skb)->daddr))) {
        return NF_ACCEPT;
    }

    if ( dir == IP_CT_DIR_ORIGINAL ) {
        /* If routing table cache and address resolution table are released,
         * or the socket buffer is allocated a new routing table, we have to 
         * release our original holded dst.*/
        if (ct->orgdir_dst && (ct->orgdir_dst != skb->dst)) {
            dst_release(ct->orgdir_dst);
            ct->orgdir_dst = NULL;
        }

        if ( !ct->orgdir_dst) {
            ct->orgdir_dst = skb->dst;
            /* this conntrack uses this dst_entry */
            dst_use(ct->orgdir_dst, jiffies);
            /* Only INPUT and DNAT will be checked. */
            ct->orgdir_rid |= ((1 << EZP_IP_LOCAL_IN) | (1 << IP_NAT_MANIP_DST));
        }
    } 
    else if ( dir == IP_CT_DIR_REPLY ) {
        /* If routing table cache and address resolution table are released,
         * or the socket buffer is allocated a new routing table, we have to 
         * release our original holded dst.*/
        if (ct->replydir_dst && (ct->replydir_dst != skb->dst)) {
            dst_release(ct->replydir_dst);
            ct->replydir_dst = NULL;
        }

        if ( !ct->replydir_dst) {
            ct->replydir_dst = skb->dst;
            /* this conntrack uses this dst_entry */
            dst_use(ct->replydir_dst, jiffies);
            /* Only INPUT and DNAT will be checked. */
            ct->replydir_rid |= ((1 << EZP_IP_LOCAL_IN) | (1 << IP_NAT_MANIP_DST));
        }
    }
    return NF_ACCEPT;
}

/* Set up the nat flags for nat_flags. */
static unsigned int
ezp_nat_output_last_hook(unsigned int hooknum, struct sk_buff *skb,
                             const struct net_device *indev,
                             const struct net_device *outdev,
	                     int (*okfn)(struct sk_buff *))
{
    struct nf_conn *ct;
    enum ip_conntrack_info ctinfo;
    enum ip_conntrack_dir dir;
    struct iphdr *iph = ip_hdr(skb);
    
    if(!ezp_nat_enable_flag){
        return NF_ACCEPT;
    }
    ct = nf_ct_get(skb, &ctinfo);
    if(!ct){
        return NF_ACCEPT;
    }
    /* Make sure it is confirmed. */
    if (!nf_ct_is_confirmed(ct)) {
        return NF_ACCEPT;
    }
    dir = CTINFO2DIR(ctinfo);
    ASSERT(skb->dst);
    ASSERT(skb->dst->dev);
    /* TCP or UDP. */
    if ((iph->protocol != IPPROTO_TCP) &&
            (iph->protocol != IPPROTO_UDP) ) {
        return NF_ACCEPT;
    }

    /* bypass multicast and broadcast */
    if ((ipv4_is_lbcast(ip_hdr(skb)->daddr)) ||
            (ipv4_is_multicast(ip_hdr(skb)->daddr))) {
        return NF_ACCEPT;
    }

    if ( dir == IP_CT_DIR_ORIGINAL ) {
        /* If routing table cache and address resolution table are released,
         * or the socket buffer is allocated a new routing table, we have to 
         * release our original holded dst.*/
        if (ct->orgdir_dst && (ct->orgdir_dst != skb->dst)) {
            dst_release(ct->orgdir_dst);
            ct->orgdir_dst = NULL;
        }

        if ( !ct->orgdir_dst) {
            ct->orgdir_dst = skb->dst;
            /* this conntrack uses this dst_entry */
            dst_use(ct->orgdir_dst, jiffies);
            /* Only OUTPUT and SNAT will be checked. */
            ct->orgdir_rid |= ((1 << EZP_IP_LOCAL_OUT) | (1 << IP_NAT_MANIP_SRC));
        }
    } 
    else if ( dir == IP_CT_DIR_REPLY ) {
        /* If routing table cache and address resolution table are released,
         * or the socket buffer is allocated a new routing table, we have to 
         * release our original holded dst.*/
        if (ct->replydir_dst && (ct->replydir_dst != skb->dst)) {
            dst_release(ct->replydir_dst);
            ct->replydir_dst = NULL;
        }

        if ( !ct->replydir_dst) {
            ct->replydir_dst = skb->dst;
            /* this conntrack uses this dst_entry */
            dst_use(ct->replydir_dst, jiffies);
            /* Only OUTPUT and SNAT will be checked. */
            ct->replydir_rid |= ((1 << EZP_IP_LOCAL_OUT) | (1 << IP_NAT_MANIP_SRC));
        }
    }
    return NF_ACCEPT;
}

static struct nf_hook_ops ezp_nat_pre_ops =
{
    .hook       = ezp_nat_pre_hook,
    .owner      = THIS_MODULE,
    .pf         = PF_INET,
    .hooknum    = NF_INET_PRE_ROUTING,
    .priority   = EZP_NAT_PREROUTE_PRIORITY,
};

static struct nf_hook_ops ezp_nat_fw_ops =
{
    .hook       = ezp_nat_fw_hook,
    .owner      = THIS_MODULE,
    .pf         = PF_INET,
    .hooknum    = NF_INET_FORWARD,
    .priority   = EZP_NAT_FORWARD_PRIORITY,
};

static struct nf_hook_ops ezp_nat_output_ops =
{
    .hook       = ezp_nat_output_hook,
    .owner      = THIS_MODULE,
    .pf         = PF_INET,
    .hooknum    = NF_INET_LOCAL_OUT,
    .priority   = EZP_NAT_OUTPUT_PRIORITY,
};

static struct nf_hook_ops ezp_nat_output_last_ops =
{
    .hook       = ezp_nat_output_last_hook,
    .owner      = THIS_MODULE,
    .pf         = PF_INET,
    .hooknum    = NF_INET_LOCAL_OUT,
    .priority   = EZP_NAT_OUTPUT_LAST_PRIORITY,
};

static struct nf_hook_ops ezp_nat_input_last_ops =
{
    .hook       = ezp_nat_input_last_hook,
    .owner      = THIS_MODULE,
    .pf         = PF_INET,
    .hooknum    = NF_INET_LOCAL_IN,
    .priority   = EZP_NAT_INPUT_PRIORITY,
};

static int __init ezp_nat_init(void)
{
    if (nf_register_hook(&ezp_nat_pre_ops) < 0) {
        printk("%s:%d\n", __FILE__, __LINE__);
        goto error;
    }

    if (nf_register_hook(&ezp_nat_output_ops) < 0) {
        printk("%s:%d\n", __FILE__, __LINE__);
        goto error;
    }

    if (nf_register_hook(&ezp_nat_fw_ops) < 0) {
        printk("%s:%d\n", __FILE__, __LINE__);
        goto error;
    }

    if (nf_register_hook(&ezp_nat_input_last_ops) < 0) {
        printk("%s:%d\n", __FILE__, __LINE__);
        goto error;
    }

    if (nf_register_hook(&ezp_nat_output_last_ops) < 0) {
        printk("%s:%d\n", __FILE__, __LINE__);
        goto error;
    }

    proc_ezp_nat = create_proc_entry("ezp_nat_ctrl", 0644, NULL);
    if (proc_ezp_nat) {
        proc_ezp_nat->read_proc = &ezp_nat_proc_read;
        proc_ezp_nat->write_proc = &ezp_nat_proc_write;
        proc_ezp_nat->data = NULL;
        printk("EZ Packet turboNAT nat proc created.\n");
    }
    else {
        printk("EZ Packet turboNAT nat proc create fail.\n");
    }
    proc_ezp_nat_queue = create_proc_entry("ezp_nat_queue_ctrl", 0644, NULL);
    if (proc_ezp_nat_queue) {
        proc_ezp_nat_queue->read_proc = &ezp_nat_queue_proc_read;
        proc_ezp_nat_queue->write_proc = &ezp_nat_queue_proc_write;
        proc_ezp_nat_queue->data = NULL;
        printk("EZ Packet turboNAT queue proc created.\n");
    }
    else {
        printk("EZ Packet turboNAT queue proc create fail.\n");
    }
    printk("EZ Packet turboNAT registered.\n");
    MOD_INC_USE_COUNT;
    return 0;
error:
    return -EINVAL;
}

static void __exit ezp_nat_destroy(void)
{
    printk("EZ Packet turboNAT deregistered.\n");
    if (proc_ezp_nat) {
        remove_proc_entry("ezp_nat_ctrl", NULL);
        printk("EZ Packet turboNAT proc removed.\n");
    }
    if (proc_ezp_nat_queue) {
        remove_proc_entry("ezp_nat_queue_ctrl", NULL);
        printk("EZ Packet turboNAT queue proc removed.\n");
    }
    nf_unregister_hook(&ezp_nat_output_last_ops);
    nf_unregister_hook(&ezp_nat_input_last_ops);
    nf_unregister_hook(&ezp_nat_output_ops);
    nf_unregister_hook(&ezp_nat_fw_ops);
    nf_unregister_hook(&ezp_nat_pre_ops);
    MOD_DEC_USE_COUNT;
}

module_init(ezp_nat_init);
module_exit(ezp_nat_destroy);


