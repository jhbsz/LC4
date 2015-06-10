#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <stdlib.h>

#include <linux/autoconf.h>

#include "ra_ioctl.h"

#ifdef CONFIG_RT2860V2_AP_MEMORY_OPTIMIZATION
#define RT_SWITCH_HELP		0
#define RT_TABLE_MANIPULATE	0
#else
#define RT_SWITCH_HELP		1
#define RT_TABLE_MANIPULATE	1
#endif

#define VLAN_SWITCH_PHY_PORT_NUMBER 5
#define VLAN_SWITCH_PORT_NUMBER (VLAN_SWITCH_PHY_PORT_NUMBER + 2) 
#define VLAN_SWITCH_TAG_NUMBER 7
#define VLAN_REMOVE_TAG_MASK 0x0000007f

#define AR8316_NUM_PORTS        6
#define AR8316_NUM_VLANS        4096
#define BIT(x)                        (1UL << (x))
#define BITS(_s, _n)                  (((1UL << (_n)) - 1) << _s)
#define AR8316_REG_VTU                0x0040
#define AR8316_VTU_OP                 BITS(0, 3)
#define AR8316_VTU_OP_NOOP            0x0
#define AR8316_VTU_OP_FLUSH           0x1
#define AR8316_VTU_OP_LOAD            0x2
#define AR8316_VTU_OP_PURGE           0x3
#define AR8316_VTU_OP_REMOVE_PORT     0x4
#define AR8316_VTU_OP_READ_NEXT       0x5
#define AR8316_VTU_OP_READ_ONE        0x6
#define AR8316_VTU_ACTIVE             BIT(3)
#define AR8316_VTU_FULL               BIT(4)
#define AR8316_VTU_PORT               BITS(8, 4)
#define AR8316_VTU_PORT_S             8
#define AR8316_VTU_VID                BITS(16, 12)
#define AR8316_VTU_VID_S              16
#define AR8316_VTU_PRIO               BITS(28, 3)
#define AR8316_VTU_PRIO_S             28
#define AR8316_VTU_PRIO_EN            BIT(31)
#define AR8316_REG_VTU_DATA           0x0044
#define AR8316_VTUDATA_MEMBER         BITS(0, 10)
#define AR8316_VTUDATA_VALID          BIT(11)
#define AR8316_PORT_VLAN_MODE         BITS(30, 2)
#define AR8316_PORT_VLAN_MODE_S       30
#define AR8316_PORT_REG_BASE          0x100
#define AR8316_PORT_OFFSET(_i)        (AR8316_PORT_REG_BASE * (_i + 1))
#define AR8316_REG_PORT_CTRL(_i)      (AR8316_PORT_OFFSET(_i) + 0x0004)
#define AR8316_REG_PORT_VLAN(_i)      (AR8316_PORT_OFFSET(_i) + 0x0008)
#define AR8316_PORT_EG_S              8
#define AR8316_PORT_EG                BITS(8, 2)
#define AR8316_PORT_VID               BITS(0, 12)
#define AR8316_PORT_MODE              BITS(30, 2)
#define AR8316_PORT_MODE_S            30
#define AR8316_PORT_MEMBER            BITS(16, 6)
#define AR8316_PORT_MEMBER_S          16

int esw_fd;

int (*add_vt)(int argc, char *argv[]);
int (*remove_vt)(int argc, char *argv[]);
int (*clean_vt)(int argc, char *argv[]);
int (*show_vt)(int argc, char *argv[]);
int (*set_port)(int argc, char *argv[]);
int (*show_port)(int argc, char *argv[]);

int ar8316_add_vt(int argc, char *argv[]);
int ar8316_remove_vt(int argc, char *argv[]);
int ar8316_show_vt(int argc, char *argv[]);
int ar8316_clean_vt(int argc, char *argv[]);
int ar8316_set_port(int argc, char *argv[]);
int ar8316_show_port(int argc, char *argv[]);

void switch_init(void)
{
    esw_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (esw_fd < 0) {
        perror("socket");
        exit(0);
    }
#if defined(AR8316)
#define PORT_0_STATUS 0x100
#define PORT_1_STATUS 0x200
#define PORT_2_STATUS 0x300
#define PORT_3_STATUS 0x400
#define PORT_4_STATUS 0x500
#define PORT_5_STATUS 0x600
    add_vt = ar8316_add_vt;
    remove_vt = ar8316_remove_vt;
    clean_vt = ar8316_clean_vt;
    show_vt = ar8316_show_vt;
    set_port = ar8316_set_port;
    show_port = ar8316_show_port;
#elif defined(AR8328)
#define PORT_0_STATUS 0x7c
#define PORT_1_STATUS 0x80
#define PORT_2_STATUS 0x84
#define PORT_3_STATUS 0x88
#define PORT_4_STATUS 0x8c
#define PORT_5_STATUS 0x90
    add_vt = ar8316_add_vt;
    remove_vt = ar8316_remove_vt;
    clean_vt = ar8316_clean_vt;
    show_vt = ar8316_show_vt;
    set_port = ar8316_set_port;
    show_port = ar8316_show_port;
#else
#define PORT_0_STATUS 0x100
#define PORT_1_STATUS 0x200
#define PORT_2_STATUS 0x300
#define PORT_3_STATUS 0x400
#define PORT_4_STATUS 0x500
#define PORT_5_STATUS 0x600
    add_vt = NULL;
    remove_vt = NULL;
    clean_vt = NULL;
    show_vt = NULL;
    set_port = NULL;
    show_port = NULL;
#endif
}

void switch_fini(void)
{
    close(esw_fd);
}

void usage(char *cmd)
{
#if RT_SWITCH_HELP
    printf("Usage:\n");
    printf(" %s dump                                     - dump switch table\n", cmd);
    printf(" %s add [mac] [portmap]                      - add an entry to switch table\n", cmd);
    printf(" %s add [mac] [portmap] [vlan idx]           - add an entry to switch table\n", cmd);
    printf(" %s add [mac] [portmap] [vlan idx] [age]     - add an entry to switch table\n", cmd);
    printf(" %s del [mac]                                - delete an entry from switch table\n", cmd);
    printf(" %s del [mac] [vlan idx]                     - delete an entry from switch table\n", cmd);
    printf(" %s vlan dump                                - dump switch table\n", cmd);
    /*printf(" %s vlan set [vid] [name] [portmap] [tagmap] - \*/
    printf(" %s vlan set [group] [vid] [portmap] [tagmap] - \
            set vlan id and associated member \n\
            \t[portmap] [p0|p1|p2|p3|p4|p5(WAN-)|GMAC|CPU]\n\
            \t[tagmap]  [p0|p1|p2|p3|p4|p5(WAN-)|GMAC|CPU]\n", cmd);
    printf(" %s switch vlan port [portid] [pvid]         - set PVID for each port\n", cmd); 
    printf(" %s reg r [offset]                           - register read from offset\n", cmd);
    printf(" %s reg w [offset] [value]                   - register write value to offset\n", cmd);
    printf(" %s greg r [offset]                           - register read from offset\n", cmd);
    printf(" %s greg w [offset] [value]                   - register write value to offset\n", cmd);
    printf(" %s greg br [offset]                          - register read from offset\n", cmd);
    printf(" %s greg bw [offset] [value]                  - register write value to offset\n", cmd);
    printf(" %s gmii r [phyid] [regaddr]                  - gmii read \n", cmd);
    printf(" %s gmii w [phyid] [regaddr] [value]          - gmii write\n", cmd);
    printf(" %s mii r [phyid] [regaddr]			  - mii read\n", cmd);
    printf(" %s mii w [phyid] [regaddr] [value]		  - mii write\n",cmd);
    printf(" %s portst [link|spd|duplex|fc] [portid]   - get port status\n",cmd);
    printf(" %s extportst [link|spd|duplex|fc] [portid]   - get external port status\n",cmd);

#endif
    printf(" %s gvlan [set|remove|clean|dump]  [vid] [portmember:p0|p1|p2|p3|p4|p5(cpu)]            - set giga switch vlan table\n", cmd);
    printf(" %s gport [dump|pid] [pvid] [tag] [mode] [pmemb:p0|p1|p2|p3|p4|p5(cpu)]         - giga port setting\n", cmd);
    switch_fini();
    exit(0);
}

int reg_read(int offset, int *value)
{
    struct ifreq ifr;
    esw_reg reg;

    if (value == NULL)
        return -1;
    reg.off = offset;
    strncpy(ifr.ifr_name, "eth2", 5);
    ifr.ifr_data = &reg;
    if (-1 == ioctl(esw_fd, RAETH_ESW_REG_READ, &ifr)) {
        perror("ioctl");
        close(esw_fd);
        exit(0);
    }
    *value = reg.val;
    return 0;
}

int reg_write(int offset, int value)
{
    struct ifreq ifr;
    esw_reg reg;

    reg.off = offset;
    reg.val = value;
    strncpy(ifr.ifr_name, "eth2", 5);
    ifr.ifr_data = &reg;
    if (-1 == ioctl(esw_fd, RAETH_ESW_REG_WRITE, &ifr)) {
        perror("ioctl");
        close(esw_fd);
        exit(0);
    }
    return 0;
}

int greg_read(int offset, int *value)
{
    struct ifreq ifr;
    esw_reg reg;

    if (value == NULL)
        return -1;
    reg.off = offset;
    strncpy(ifr.ifr_name, "eth2", 5);
    ifr.ifr_data = &reg;
    if (-1 == ioctl(esw_fd, RAETH_EXT_SW_REG_READ, &ifr)) {
        perror("ioctl");
        close(esw_fd);
        exit(0);
    }
    *value = reg.val;
    return 0;
}

int greg_write(int offset, int value)
{
    struct ifreq ifr;
    esw_reg reg;

    reg.off = offset;
    reg.val = value;
    strncpy(ifr.ifr_name, "eth2", 5);
    ifr.ifr_data = &reg;
    if (-1 == ioctl(esw_fd, RAETH_EXT_SW_REG_WRITE, &ifr)) {
        perror("ioctl");
        close(esw_fd);
        exit(0);
    }
    return 0;
}

#if RT_TABLE_MANIPULATE
void table_dump(void)
{
    int i, j, value, mac;

    reg_write(REG_ESW_TABLE_SEARCH, 0x1);
    printf("hash  port(0:6)  vid  age  pxy  in   mac-address\n");
    for (i = 0; i < 0x400; i++) {
        while(1) {
            reg_read(REG_ESW_TABLE_STATUS0, &value);
            if (value & 0x1) { //search_rdy
                if ((value & 0x70) == 0) {
                    printf("found an unused entry (age = 3'b000), please check!\n");
                    return;
                }
                printf("%03x:  ", (value >> 22) & 0x3ff); //hash_addr_lu
                j = (value >> 12) & 0x7f; //r_port_map
                printf("%c", (j & 0x01)? '1':'-');
                printf("%c", (j & 0x02)? '1':'-');
                printf("%c", (j & 0x04)? '1':'-');
                printf("%c", (j & 0x08)? '1':'-');
                printf("%c ", (j & 0x10)? '1':'-');
                printf("%c", (j & 0x20)? '1':'-');
                printf("%c", (j & 0x40)? '1':'-');
                printf("    %2d", (value >> 7) & 0xf); //r_vid
                printf("    %1d", (value >> 4) & 0x7); //r_age_field
                printf("    %c", (value & 0x8)? 'y':'n'); //r_proxy
                printf("   %c", (value & 0x4)? 'y':'n'); //r_mc_ingress

                reg_read(REG_ESW_TABLE_STATUS2, &mac);
                printf("  %08x", mac);
                reg_read(REG_ESW_TABLE_STATUS1, &mac);
                printf("%04x\n", (mac & 0xffff));
                if (value & 0x2) {
                    printf("end of table %d\n", i);
                    return;
                }
                break;
            }
            else if (value & 0x2) { //at_table_end
                printf("found the last entry %d (not ready)\n", i);
                return;
            }
            usleep(5000);
        }
        reg_write(REG_ESW_TABLE_SEARCH, 0x2); //search for next address
    }
}

void table_add(int argc, char *argv[])
{
    int i, j, value;
    char tmpstr[9];

    if (!argv[2] || strlen(argv[2]) != 12) {
        printf("MAC address format error, should be of length 12\n");
        return;
    }
    strncpy(tmpstr, argv[2], 8);
    tmpstr[8] = '\0';
    value = strtoul(tmpstr, NULL, 16);
    reg_write(REG_ESW_WT_MAC_AD2, value);

    strncpy(tmpstr, argv[2]+8, 4);
    tmpstr[4] = '\0';
    value = strtoul(tmpstr, NULL, 16);
    reg_write(REG_ESW_WT_MAC_AD1, value);

    if (!argv[3] || strlen(argv[3]) != 7) {
        printf("portmap format error, should be of length 7\n");
        return;
    }
    j = 0;
    for (i = 0; i < 7; i++) {
        if (argv[3][i] != '0' && argv[3][i] != '1') {
            printf("portmap format error, should be of combination of 0 or 1\n");
            return;
        }
        j += (argv[3][i] - '0') * (1 << i);
    }
    value = j << 12; //w_port_map

    if (argc > 4) {
        j = strtoul(argv[4], NULL, 0);
        if (j < 0 || 15 < j) {
            printf("wrong member index range, should be within 0~15\n");
            return;
        }
        value += (j << 7); //w_index
    }

    if (argc > 5) {
        j = strtoul(argv[5], NULL, 0);
        if (j < 1 || 7 < j) {
            printf("wrong age range, should be within 1~7\n");
            return;
        }
        value += (j << 4); //w_age_field
    }
    else
        value += (7 << 4); //w_age_field

    value += 1; //w_mac_cmd
    reg_write(REG_ESW_WT_MAC_AD0, value);

    for (i = 0; i < 20; i++) {
        reg_read(REG_ESW_WT_MAC_AD0, &value);
        if (value & 0x2) { //w_mac_done
            printf("done.\n");
            return;
        }
        usleep(1000);
    }
    if (i == 20)
        printf("timeout.\n");
}

void table_del(int argc, char *argv[])
{
    int i, j, value;
    char tmpstr[9];

    if (!argv[2] || strlen(argv[2]) != 12) {
        printf("MAC address format error, should be of length 12\n");
        return;
    }
    strncpy(tmpstr, argv[2], 8);
    tmpstr[8] = '\0';
    value = strtoul(tmpstr, NULL, 16);
    reg_write(REG_ESW_WT_MAC_AD2, value);

    strncpy(tmpstr, argv[2]+8, 4);
    tmpstr[4] = '\0';
    value = strtoul(tmpstr, NULL, 16);
    reg_write(REG_ESW_WT_MAC_AD1, value);

    value = 0;
    if (argc > 3) {
        j = strtoul(argv[3], NULL, 0);
        if (j < 0 || 15 < j) {
            printf("wrong member index range, should be within 0~15\n");
            return;
        }
        value += (j << 7); //w_index
    }

    value += 1; //w_mac_cmd
    reg_write(REG_ESW_WT_MAC_AD0, value);

    for (i = 0; i < 20; i++) {
        reg_read(REG_ESW_WT_MAC_AD0, &value);
        if (value & 0x2) { //w_mac_done
            printf("done.\n");
            return;
        }
        usleep(1000);
    }
    if (i == 20)
        printf("timeout.\n");
}

void vlan_dump(void)
{
    int i, vid, value, vtag;

    printf("idx   vid  portmap  vtag\n");
    for (i = 0; i < 8; i++) {
        reg_read(REG_ESW_VLAN_ID_BASE + 4*i, &vid);
        reg_read(REG_ESW_VLAN_MEMB_BASE + 4*(i/2), &value);
        reg_read(REG_ESW_VLAN_TAG_REMOVE, &vtag);

        printf(" %2d  %4d  ", 2*i, vid & 0xfff);
        if (i%2 == 0) {
            printf("%c", (value & 0x00000001)? '1':'-');
            printf("%c", (value & 0x00000002)? '1':'-');
            printf("%c", (value & 0x00000004)? '1':'-');
            printf("%c", (value & 0x00000008)? '1':'-');
            printf("%c", (value & 0x00000010)? '1':'-');
            printf("%c", (value & 0x00000020)? '1':'-');
            printf("%c  ", (value & 0x00000040)? '1':'-');

            printf("%c", (vtag & 0x00000001)? '-':'1');
            printf("%c", (vtag & 0x00000002)? '-':'1');
            printf("%c", (vtag & 0x00000004)? '-':'1');
            printf("%c", (vtag & 0x00000008)? '-':'1');
            printf("%c", (vtag & 0x00000010)? '-':'1');
            printf("%c", (vtag & 0x00000020)? '-':'1');
            printf("%c\n", (vtag & 0x00000040)? '-':'1');
        }
        else {
            printf("%c", (value & 0x00010000)? '1':'-');
            printf("%c", (value & 0x00020000)? '1':'-');
            printf("%c", (value & 0x00040000)? '1':'-');
            printf("%c", (value & 0x00080000)? '1':'-');
            printf("%c", (value & 0x00100000)? '1':'-');
            printf("%c", (value & 0x00200000)? '1':'-');
            printf("%c  ", (value & 0x00400000)? '1':'-');

            printf("%c", (vtag & 0x00000001)? '-':'1');
            printf("%c", (vtag & 0x00000002)? '-':'1');
            printf("%c", (vtag & 0x00000004)? '-':'1');
            printf("%c", (vtag & 0x00000008)? '-':'1');
            printf("%c", (vtag & 0x00000010)? '-':'1');
            printf("%c", (vtag & 0x00000020)? '-':'1');
            printf("%c\n", (vtag & 0x00000040)? '-':'1');
        }
        printf(" %2d  %4d  ", 2*i+1, ((vid & 0xfff000) >> 12));
        if (i%2 == 0) {
            printf("%c", (value & 0x00000100)? '1':'-');
            printf("%c", (value & 0x00000200)? '1':'-');
            printf("%c", (value & 0x00000400)? '1':'-');
            printf("%c", (value & 0x00000800)? '1':'-');
            printf("%c", (value & 0x00001000)? '1':'-');
            printf("%c", (value & 0x00002000)? '1':'-');
            printf("%c  ", (value & 0x00004000)? '1':'-');

            printf("%c", (vtag & 0x00000001)? '-':'1');
            printf("%c", (vtag & 0x00000002)? '-':'1');
            printf("%c", (vtag & 0x00000004)? '-':'1');
            printf("%c", (vtag & 0x00000008)? '-':'1');
            printf("%c", (vtag & 0x00000010)? '-':'1');
            printf("%c", (vtag & 0x00000020)? '-':'1');
            printf("%c\n", (vtag & 0x00000040)? '-':'1');
        }
        else {
            printf("%c", (value & 0x01000000)? '1':'-');
            printf("%c", (value & 0x02000000)? '1':'-');
            printf("%c", (value & 0x04000000)? '1':'-');
            printf("%c", (value & 0x08000000)? '1':'-');
            printf("%c", (value & 0x10000000)? '1':'-');
            printf("%c", (value & 0x20000000)? '1':'-');
            printf("%c  ", (value & 0x40000000)? '1':'-');

            printf("%c", (vtag & 0x00000001)? '-':'1');
            printf("%c", (vtag & 0x00000002)? '-':'1');
            printf("%c", (vtag & 0x00000004)? '-':'1');
            printf("%c", (vtag & 0x00000008)? '-':'1');
            printf("%c", (vtag & 0x00000010)? '-':'1');
            printf("%c", (vtag & 0x00000020)? '-':'1');
            printf("%c\n", (vtag & 0x00000040)? '-':'1');
        }
    }
}

/* convert flag portmember sequence */
int conv_portmap_flag(char *string, int length)
{
    int i = 0;
    int retval;
    char *tmp = (char *) malloc (sizeof(char) * (length + 1));

    for(i = 0; i < length; i++) {
        tmp[i] = string[length -i -1];
    }

    *(tmp + length) = '\0';
    retval = strtol(tmp, NULL, 2);
    free(tmp);
    tmp = NULL;
    return retval;
}

/* setup Ralink ESW port-based vlan */
/* switch vlan port [port_num] [PVID] */
void vlan_portset(int argc, char *argv[]) 
{
    int portid, pvid, temp_portid, temp_pvid, value;

    /* argc should be 5 */    
    if (argc != 5) {
        printf("insufficient arguments!\n");
        return;
    }

    /* setup portid */
    portid = strtoul(argv[3], NULL, 0);
    if (portid < 0 || 6 < portid) {
        printf("wrong member index range, should be within 0~6\n");
        return;
    }

    /* setup pvid */
    pvid = strtoul(argv[4], NULL, 0);
    if (pvid < 0 || 0xfff < pvid) {
        printf("wrong vlan portid range, should be within 0~4095\n");
        return;
    }   

    /* select which port and write pvid back */
    reg_read(REG_ESW_VLAN_PORT_ID_BASE + 4*(portid/2), &value);
    if (portid % 2 == 0) {
        value &= 0xfff000;
        value |= pvid;
    }
    else {
        value &= 0xfff;
        value |= (pvid << 12);
    }
    reg_write(REG_ESW_VLAN_PORT_ID_BASE + 4*(portid/2), value);
}

/* setup vlan */
void vlan_set(int argc, char *argv[])
{
    int i, pormap, value, temp_flag, ori_flag;
    int idx, vid, pmap, pmem_length;

    /* argc should be 6 or 7 */
    if (argc !=6 && argc != 7) {
        printf("insufficient arguments!\n");
        return;
    }
    idx = strtoul(argv[3], NULL, 0);
    if (idx < 0 || 15 < idx) {
        printf("wrong member index range, should be within 0~15\n");
        return;
    }
    vid = strtoul(argv[4], NULL, 0);
    if (vid < 0 || 0xfff < vid) {
        printf("wrong vlan id range, should be within 0~4095\n");
        return;
    }

    /* get the port member length */
    pmem_length = strlen(argv[5]);
    if (pmem_length != VLAN_SWITCH_PORT_NUMBER) {
        printf("portmap format error, should be of length %d\n", VLAN_SWITCH_PORT_NUMBER);
        return;
    }

    /* set vlan identifier */
    reg_read(REG_ESW_VLAN_ID_BASE + 4*(idx/2), &value);
    if (idx % 2 == 0) {
        value &= 0xfff000;
        value |= vid;
    }
    else {
        value &= 0xfff;
        value |= (vid << 12);
    }
    reg_write(REG_ESW_VLAN_ID_BASE + 4*(idx/2), value);

    /* portmap of port member format check */
    pormap = 0;
    for (i = 0; i < VLAN_SWITCH_PORT_NUMBER; i++) {
        if (argv[5][i] != '0' && argv[5][i] != '1') {
            printf("portmap format error, should be of combination of 0 or 1\n");
            return;
        }
        pormap += (argv[5][i] - '0') * (1 << i);
    }

    /* set vlan member */
    /* Fix me: for different vlan port number, the register layout will be different.
       It is hard to adjust the code from ralink 5 port switch to other kind of switches. */
    reg_read(REG_ESW_VLAN_MEMB_BASE + 4*(idx/4), &value);
    if (idx % 4 == 0) {
        value &= 0xffffff00;
        value |= pormap;
    }
    else if (idx % 4 == 1) {
        value &= 0xffff00ff;
        value |= (pormap << 8);
    }
    else if (idx % 4 == 2) {
        value &= 0xff00ffff;
        value |= (pormap << 16);
    }
    else {
        value &= 0x00ffffff;
        value |= (pormap << 24);
    }
    reg_write(REG_ESW_VLAN_MEMB_BASE + 4*(idx/4), value);

    if(argc == 7) {
        if (strlen(argv[6]) != VLAN_SWITCH_TAG_NUMBER) {
            printf("tag format error, should be of length 7\n");
            return;
        }
        /* reverse bit */
        temp_flag = conv_portmap_flag(argv[6], VLAN_SWITCH_PORT_NUMBER);
        /* set vlan remove flag */
        reg_read(REG_ESW_VLAN_TAG_REMOVE, &ori_flag);

        /* fetch the original value of vlan tag */
        ori_flag = ori_flag & (~pormap);

        /* fetch new vlan tag value */
        temp_flag = ~temp_flag & pormap;

        /* merge new vlan flag value with original flag */
        value = ori_flag | temp_flag;

        /* write back the value of vlan flag to register */
        reg_write(REG_ESW_VLAN_TAG_REMOVE, value);
    }
}
#endif
//switch gmii r [phyid] [reg_num] [mask]
int gmii_read(int argc, char *argv[])
{
    ra_mii_ioctl_data mii_r_param;
    struct ifreq ifr;
    unsigned short int mask = 0xffff;
    //strtol(char* nptr, char ** endptr, base)
    mii_r_param.phy_id = strtol(argv[3], NULL, 16);
    mii_r_param.reg_num = strtol(argv[4], NULL, 16);
    //with mask
    if (argc > 5) {
        mask = strtol(argv[5], NULL, 16);
    }

    strncpy(ifr.ifr_name, "eth2", 5);
    ifr.ifr_data = &mii_r_param;
    if (-1 == ioctl(esw_fd, RAETH_RGMII_READ, &ifr)) {
        perror("ioctl");
        close(esw_fd);
        exit(0);
    }
    printf("%x\n", mii_r_param.val_out & mask);
    return 0;
}
//switch gmii w [phyid] [reg_num] [value] [mask]
int gmii_write(int argc, char *argv[])
{
    ra_mii_ioctl_data mii_w_param;
    struct ifreq ifr;
    unsigned short int mask = 0xffff;
    unsigned short int orival = 0;

    mii_w_param.phy_id = strtol(argv[3], NULL, 16);
    mii_w_param.reg_num = strtol(argv[4], NULL, 16);
    strncpy(ifr.ifr_name, "eth2", 5);
    ifr.ifr_data = &mii_w_param;
    //with mask, only modify some bits
    if (argc > 6 ) {
        mask = strtol(argv[6], NULL, 16);
        //read out the original value and clear mask bits
        if (-1 == ioctl(esw_fd, RAETH_RGMII_READ, &ifr)) {
            perror("ioctl");
            close(esw_fd);
            exit(0);
        }
        orival = mii_w_param.val_out & (~mask);
    }
    mii_w_param.val_in = strtol(argv[5], NULL, 16);
    mii_w_param.val_in = (mii_w_param.val_in & mask) | orival;
    printf("Phy %x reg %x write val %x/%x\n", 
            mii_w_param.phy_id, mii_w_param.reg_num,
            mii_w_param.val_in & mask, mask);
    mii_w_param.val_out = 0;
    strncpy(ifr.ifr_name, "eth2", 5);
    ifr.ifr_data = &mii_w_param;
    if (-1 == ioctl(esw_fd, RAETH_RGMII_WRITE, &ifr)) {
        perror("ioctl");
        close(esw_fd);
        exit(0);
    }
    return 0;
}
//switch mii r [phyid] [reg_num] [mask]
int mii_read(int argc, char *argv[])
{
    ra_mii_ioctl_data mii_r_param;
    struct ifreq ifr;
    unsigned short int mask = 0xffff;

    mii_r_param.phy_id = strtol(argv[3], NULL, 16);
    mii_r_param.reg_num = strtol(argv[4], NULL, 16);
    //with mask
    if (argc > 5) {
        mask = strtol(argv[5], NULL, 16);
    }
    strncpy(ifr.ifr_name, "eth2", 5);
    ifr.ifr_data = &mii_r_param;
    if (-1 == ioctl(esw_fd, RAETH_MII_READ, &ifr)) {
        perror("ioctl");
        close(esw_fd);
        exit(0);
    }
    printf("%x\n", mii_r_param.val_out & mask);
    return 0;
}
//switch mii w [phyid] [reg_num] [value] [value]
int mii_write(int argc, char *argv[]){

    ra_mii_ioctl_data mii_w_param;
    struct ifreq ifr;
    unsigned short int mask = 0xffff;
    unsigned short int orival = 0;

    mii_w_param.phy_id = strtol(argv[3], NULL, 16);
    mii_w_param.reg_num = strtol(argv[4], NULL, 16);
    
    strncpy(ifr.ifr_name, "eth2", 5);
    ifr.ifr_data = &mii_w_param;
    //with mask
    if (argc > 6 ) {
        mask = strtol(argv[6], NULL, 16);
        if (-1 == ioctl(esw_fd, RAETH_MII_READ, &ifr)) {
            perror("ioctl");
            close(esw_fd);
            exit(0);
        }
        orival = mii_w_param.val_out & (~mask);
    }
    
    mii_w_param.val_in = strtol(argv[5], NULL, 16);
    mii_w_param.val_in = (mii_w_param.val_in & mask) | orival;
    printf("Phy %x reg %x write val %x/%x\n", 
            mii_w_param.phy_id, mii_w_param.reg_num,
            mii_w_param.val_in & mask, mask);
    mii_w_param.val_out = 0;
    strncpy(ifr.ifr_name, "eth2", 5);
    ifr.ifr_data = &mii_w_param;
    if (-1 == ioctl(esw_fd, RAETH_MII_WRITE, &ifr)) {
        perror("ioctl");
        close(esw_fd);
        exit(0);
    }
}

static int
ar8316_wait_bit(int reg, unsigned int mask, unsigned int val)
{
    int timeout = 20;
    unsigned int tmp;

    greg_read(reg, &tmp);
    while ((tmp & mask) != val) {
        if (timeout-- <= 0) {
            printf("ar8316: timeout waiting for operation to complete\n");
            return 1;
        }
        greg_read(reg, &tmp);
    }
    return 0;
}

int ar8316_add_vt(int argc, char *argv[])
{
    unsigned int op = 0, val = 0;
    int pormap = 0, vid, i;
    /* Add an entry into ar8316 vlan table */
    printf("ar8316 add vt\n");
    if (ar8316_wait_bit(AR8316_REG_VTU, AR8316_VTU_ACTIVE, 0)) {
        printf("switch busy\n");
        return 1;
    }
    /* set vlan table operation control mode */
    op |= AR8316_VTU_OP_LOAD;
    /* get vid */
    if(argv[3]) {
        vid = atoi(argv[3]);
        if (vid > AR8316_NUM_VLANS || vid < 0) {
            printf("vlan id is not correct!\n");
        }
        op |= (vid << AR8316_VTU_VID_S);
    } else {
        printf("vlan id is empty!\n");
        return 1;
    }
    /* get vlan port member portmap of port member format check */
    if(argv[4]) {
        for (i = 0; i < AR8316_NUM_PORTS; i++) {
            if (argv[4][i] != '0' && argv[4][i] != '1') {
                printf("portmap format error, should be of combination of 0 or 1\n");
                return 1;
            }
            if( i == (AR8316_NUM_PORTS - 1)) {
                pormap = (pormap << 1) + (argv[4][i] - '0');
                break;
            }
            pormap += (argv[4][i] - '0') * (1 << i);
        }
    }else {
        printf("port member is empty!\n");
        return 1;
    }
    printf("port member:%x\n", pormap);

    val |= pormap;
    val &= AR8316_VTUDATA_MEMBER;
    val |= AR8316_VTUDATA_VALID;
    printf("vlan table data:%x\n", val);
    greg_write(AR8316_REG_VTU_DATA, val);

    /* set register to start vlan table operation */
    op |= AR8316_VTU_ACTIVE;
    printf("vlan table reg:%x\n", op);
    greg_write(AR8316_REG_VTU, op);

    return 0;
}

int ar8316_remove_vt(int argc, char *argv[])
{
    unsigned int op = 0;
    int vid;
    /* Remove an entry from ar8316 vlan table */
    printf("ar8316 remove vt\n");
    if (ar8316_wait_bit(AR8316_REG_VTU, AR8316_VTU_ACTIVE, 0)) {
        printf("switch busy\n");
        return 1;
    }
    /* set vlan table operation control mode */
    op |= AR8316_VTU_OP_PURGE;
    /* get vid */
    if(argv[3]) {
        vid = atoi(argv[3]);
        if (vid > AR8316_NUM_VLANS || vid < 0) {
            printf("vlan id is not correct!\n");
        }
        op |= (vid << AR8316_VTU_VID_S);
    } else {
        printf("vlan id is empty!\n");
        return 1;
    }
    /* set register to start vlan table operation */
    op |= AR8316_VTU_ACTIVE;
    printf("vlan table reg:%x\n", op);
    greg_write(AR8316_REG_VTU, op);

    return 0;
}

int ar8316_clean_vt(int argc, char *argv[])
{
    unsigned int op = 0;
    /* Remove all entry from ar8316 vlan table */
    printf("ar8316 clean vt\n");
    if (ar8316_wait_bit(AR8316_REG_VTU, AR8316_VTU_ACTIVE, 0)) {
        printf("switch busy\n");
        return 1;
    }
    /* set vlan table operation control mode */
    op |= AR8316_VTU_OP_FLUSH;
    /* set register to start vlan table operation */
    op |= AR8316_VTU_ACTIVE;
    printf("vlan table reg:%x\n", op);
    greg_write(AR8316_REG_VTU, op);
    return 0;
}

int ar8316_get_next(int vid, unsigned int *res, unsigned int *res_data)
{
    unsigned int op = 0;
    /* check is switch busy? */
    if (ar8316_wait_bit(AR8316_REG_VTU, AR8316_VTU_ACTIVE, 0)) {
        printf("switch busy\n");
        return 1;
    }
    /* set vlan table operation control */
    op |= AR8316_VTU_OP_READ_NEXT;
    /* set vid for start search */
    op |= (vid << AR8316_VTU_VID_S);
    /* set register to start vlan table operation */
    op |= AR8316_VTU_ACTIVE;
    greg_write(AR8316_REG_VTU, op);

    /* waiting for switch search completed */
    if (ar8316_wait_bit(AR8316_REG_VTU, AR8316_VTU_ACTIVE, 0)) {
        printf("switch busy\n");
        return 1;
    }
    /* check search result */
    greg_read(AR8316_REG_VTU, &op);
    if ((op & AR8316_VTU_VID) == 0) {
        return 1;
    }
    *res = op;
    greg_read(AR8316_REG_VTU_DATA, &op);
    *res_data = op;
    return 0;
}

int ar8316_read_one(int vid, unsigned int *res, unsigned int *res_data)
{
    /* read one specify vlan table entry */
    return 0;
}

int ar8316_show_vt(int argc, char *argv[])
{
    int i = 0, idx = 0;
    unsigned int op, val;
    unsigned int tmp;
    /* Show all entry from ar8316 vlan table */
    printf("ar8316 show vt\n");
    /* get one vlan table entry */
    printf("idx\tvid\tportmap\t\n");
    while (!ar8316_get_next(i, &op, &val))
    {
        tmp = (op & AR8316_VTU_VID) >> AR8316_VTU_VID_S;
        printf("%d\t%d\t", idx, tmp);
        i = tmp;
        tmp = (val & AR8316_VTUDATA_MEMBER);
        printf("%c", (tmp & 0x02)? '1':'-');
        printf("%c", (tmp & 0x04)? '1':'-');
        printf("%c", (tmp & 0x08)? '1':'-');
        printf("%c", (tmp & 0x10)? '1':'-');
        printf("%c ", (tmp & 0x20)? '1':'-');
        printf("%c\n", (tmp & 0x01)? '1':'-');
        idx++;
    }
    return 0;
}

int ar8316_set_port(int argc, char *argv[])
{
    /* set ar8316 port base vlan register */
    int pid = atoi(argv[2]) + 1, pvid = atoi(argv[3]), i;
    int tag = atoi(argv[4]), mode = atoi(argv[5]);
    unsigned int value = 0, pormap = 0;

    /* We only have 6 ports */
    if (pid > AR8316_NUM_PORTS || pid < 0) {
        printf("switch port number is not correct\n");
        return 1;
    }

    if (pvid > AR8316_NUM_VLANS) {
        printf("port vlan id is out of range\n");
        return 1;
    }

    /* Use port map domain */
    if (mode < 2) {
        /* We only have 5 port + 1 uplink port members */
        /* get vlan port member portmap of port member
           format check */
        if(argv[6]) {
            /* check port member do not out of range */
            if (strlen(argv[6]) > AR8316_NUM_PORTS) {
                printf("port member is not correct\n");
                return 1;
            }
            for (i = 0; i < AR8316_NUM_PORTS; i++) {
                if (argv[6][i] != '0' && argv[6][i] != '1') {
                    printf("portmap format error, should be of combination of 0 or 1\n");
                    return 1;
                }
                if( i == (AR8316_NUM_PORTS - 1)) {
                    pormap = (pormap << 1) + (argv[6][i] - '0');
                    break;
                }
                pormap += (argv[6][i] - '0') * (1 << i);
            }
        }else {
            printf("port member is empty!\n");
            return 1;
        }
    }


    if(pid == AR8316_NUM_PORTS)
        pid = 0;
    /* write port-based VLAN register */
    greg_read(AR8316_REG_PORT_VLAN(pid), &value);
    value = (value & ~AR8316_PORT_VID) | pvid;
    value = (value & ~AR8316_PORT_MEMBER) | (pormap << AR8316_PORT_MEMBER_S);
    value = (value & ~AR8316_PORT_VLAN_MODE) |
        (mode << AR8316_PORT_VLAN_MODE_S);
    printf("Write giga port %d pvid %d member %x mode %x\n",
            pid, pvid, pormap, mode);
    printf("Write giga reg %x with value %x\n",
            AR8316_REG_PORT_VLAN(pid), value);
    greg_write(AR8316_REG_PORT_VLAN(pid), value);
    /* write
     * port-control
     * register */
    greg_read(AR8316_REG_PORT_CTRL(pid), &value);
    value = (value & ~AR8316_PORT_EG) | (tag << AR8316_PORT_EG_S);
    printf("Write giga reg %x with value %x\n",
            AR8316_REG_PORT_CTRL(pid), value);
    greg_write(AR8316_REG_PORT_CTRL(pid), value);

    return 0;
}

int ar8316_show_port(int argc, char *argv[])
{
    /* show ar8316 port base vlan register */
    int i, idx = 0;
    unsigned int val1, val2;
    printf("port\tpvid\ttag\tmode\tportmember\n");
    for(i = 0; i < AR8316_NUM_PORTS; i++) {
        unsigned int tmp;
        printf("%d\t", i);
        idx++;
        if( idx == AR8316_NUM_PORTS) idx = 0;
        /* read port-base VLAN register */
        greg_read(AR8316_REG_PORT_VLAN(idx), &val1);
        /* read port-control register */
        greg_read(AR8316_REG_PORT_CTRL(idx), &val2);
        printf("%d\t", (val1 & AR8316_PORT_VID));
        printf("%d\t", (val2 & AR8316_PORT_EG) >> AR8316_PORT_EG_S);
        printf("%d\t", (val1 & AR8316_PORT_MODE) >> AR8316_PORT_MODE_S);
        tmp = ((val1 & AR8316_PORT_MEMBER) >> AR8316_PORT_MEMBER_S);
        printf("%c", (tmp & 0x02)? '1':'-');
        printf("%c", (tmp & 0x04)? '1':'-');
        printf("%c", (tmp & 0x08)? '1':'-');
        printf("%c", (tmp & 0x10)? '1':'-');
        printf("%c ", (tmp & 0x20)? '1':'-');
        printf("%c\n", (tmp & 0x01)? '1':'-');
    }
    return 0;
}


/* switch portst [link|spd|duplex|fc] [portid] */
int get_port_status(int argc, char *argv[])
{
    int idx, poll_target, value;
    unsigned int bitmask = 0x0001;

    //argc should be 4
    if (argc != 4)
        usage(argv[0]);
    
    //check which kind of info is polled
    if (!strncmp(argv[2], "link", 5)){
        poll_target = 1;
    }
    else if (!strncmp(argv[2], "spd", 4)){
        poll_target = 2;
    }
    else if (!strncmp(argv[2], "duplex", 7)){
        poll_target = 3;
    }
    else if (!strncmp(argv[2], "fc", 3)){
        poll_target = 4;
    }
    else
        usage(argv[0]);

    //check port range
    idx = atoi(argv[3]);
    if (idx < 0 || 4 < idx){
        printf("portid is out of range, should be 0~4\n");
        return 0;
    }
    //read register vlaue for port status(offset:0x80)
    reg_read(REG_ESW_POA, &value);
 
    switch (poll_target){
        case 1:
            value = value >> 25;
            value = (value >> idx) & bitmask;
            //printf("Port %d link state is %d\n", idx, value);
            printf("%d\n", value);
            break;
        case 2:
            value = (value >> idx) & bitmask;
            //printf("Port %d speed state is %d\n", idx, value);
            printf("%d\n", value);
            break;
        case 3:
            value = value >> 9;
            value = (value >> idx) & bitmask;
            //printf("Port %d duplex state is %d\n", idx, value);
            printf("%d\n", value);
            break;
        case 4:
            value = value >> 16;
            value = (value >> idx) & bitmask;
            //printf("Port %d flow control state is %d\n", idx, value);
            printf("%d\n", value);
            break;
        default:
            break;
    }
}

/* switch extportst [link|spd|duplex|fc] [portid] */
/* for AR8316 only */
int get_extport_status(int argc, char *argv[])
{
    int idx, poll_target, value;
    unsigned int bitmask, bitmask2;
    bitmask = 0x0001;
    bitmask2 = 0x0003;

    //argc should be 4
    if (argc != 4)
        usage(argv[0]);
    
    //check which kind of info is polled
    if (!strncmp(argv[2], "link", 5)){
        poll_target = 1;
    }
    else if (!strncmp(argv[2], "spd", 4)){
        poll_target = 2;
    }
    else if (!strncmp(argv[2], "duplex", 7)){
        poll_target = 3;
    }
    else if (!strncmp(argv[2], "fc", 3)){
        poll_target = 4;
    }
    else
        usage(argv[0]);

    //check port range and read the port status reg accordingly.
    idx = atoi(argv[3]);

    if (idx == 0){
        greg_read(PORT_0_STATUS, &value);
    }
    else if (idx == 1){
        greg_read(PORT_1_STATUS, &value);
    }
    else if (idx == 2){
        greg_read(PORT_2_STATUS, &value);
    }
    else if (idx == 3){
        greg_read(PORT_3_STATUS, &value);
    }
    else if (idx == 4){
        greg_read(PORT_4_STATUS, &value);
    }
    else if (idx == 5){
        greg_read(PORT_5_STATUS, &value);
    }
    else { 
        printf("portid is out of range, should be 0~5\n");
        return -1;
    }
 
    switch (poll_target) {
        case 1:
            value = value >> 8;
            value = value & bitmask;
            //printf("Port %d link state is %d\n", idx, value);
            printf("%d\n", value);
            break;
        case 2:
            value = value & bitmask2;
            //printf("Port %d speed state is %d\n", idx, value);
            printf("%d\n", value);
            break;
        case 3:
            value = value >> 6;
            value = value & bitmask;
            //printf("Port %d duplex state is %d\n", idx, value);
            printf("%d\n", value);
            break;
        case 4:
            value = value >> 4;
            value = value & bitmask2;
            //printf("Port %d flow control state is %d\n", idx, value);
            printf("%d\n", value);
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[])
{
    switch_init();

    if (argc < 2)
        //usage will exit(0) the process
        usage(argv[0]);
#if RT_TABLE_MANIPULATE
    //switch dump
    if (argc == 2) {
        if (!strncmp(argv[1], "dump", 5))
            table_dump();
        else
            usage(argv[0]);
    }
    //switch add
    else if (!strncmp(argv[1], "add", 4))
        table_add(argc, argv);
    //switch del
    else if (!strncmp(argv[1], "del", 4))
        table_del(argc, argv);
    //switch vlan dump/set/port
    else if (!strncmp(argv[1], "vlan", 5)) {
        if (argc < 3)
            usage(argv[0]);
        //argc >= 3
        //switch vlan dump 
        if (!strncmp(argv[2], "dump", 5))
            vlan_dump();
        //switch vlan set [pvid] [vid] [portmap] [tagged]
        else if (!strncmp(argv[2], "set", 4))
            vlan_set(argc, argv);
        //switch vlan port [portid] [pvid]
        else if (!strncmp(argv[2], "port", 5))
            vlan_portset(argc, argv);
        else
            usage(argv[0]);
    }
#endif
    //switch reg r/w
    else if (!strncmp(argv[1], "reg", 4)) {
        int off, val=0;
        //switch reg r/w [offset]
        if (argc < 4)
            usage(argv[0]);
        //switch reg r
        if (!strncmp(argv[2], "r", 2)){
            if (argc != 4)
                usage(argv[0]);
            off = strtoul(argv[3], NULL, 16);
            reg_read(off, &val);
            printf("switch reg read offset=%x, value=%x\n", off, val);
        }
        //switch reg w [offset] [value]
        else if (!strncmp(argv[2], "w", 2)){
            if (argc != 5)
                usage(argv[0]);
            off = strtoul(argv[3], NULL, 16);
            val = strtoul(argv[4], NULL, 16);
            printf("switch reg write offset=%x, value=%x\n", off, val);
            reg_write(off, val);
        }
        else
            usage(argv[0]);
    }
    //switch greg r/w/br/bw
    else if (!strncmp(argv[1], "greg", sizeof("greg"))) {
        int off, val=0;
        if (argc < 4)
            usage(argv[0]);
        //switch greg r [offset]
        if (!strncmp(argv[2], "r", 2)){
            if (argc != 4)
                usage(argv[0]);
            off = strtoul(argv[3], NULL, 16);
            greg_read(off, &val);
            printf("switch giga reg read offset=%x, value=%x\n", off, val);
        }
        //switch greg w [offset] [value]
        else if (!strncmp(argv[2], "w", 2)){
            if (argc != 5)
                usage(argv[0]);
            off = strtoul(argv[3], NULL, 16);
            val = strtoul(argv[4], NULL, 16);
            printf("switch  giga reg write offset=%x, value=%x\n", off, val);
            greg_write(off, val);
        }
        /*
        //switch greg br [offset]
        else if ((argv[2][0] == 'b') && (argv[2][1] == 'r')) {
            unsigned long int bitmask = 0x1 << 63;
            int i = 0;
            char buf[65];
            off = strtoul(argv[3], NULL, 16);
            greg_read(off, &val);

            while (bitmask && (i < 64)) {
                buf[i] = (bitmask & val) ? '1' : '0';
                i ++;
                buf[i] = ' ';
                i ++;
                bitmask = bitmask >> 1;
            }
            buf[64] = 0;
            printf("%s\n", buf);
        }
        //switch greg bw [offset] [value]
        else if ((argv[2][0] == 'b') && (argv[2][1] == 'w')) {
            if (argc != 5)
                usage(argv[0]);
            off = strtoul(argv[3], NULL, 16);
            val = strtoul(argv[4], NULL, 2);
            printf("switch reg write offset=%x, value=%x\n", off, val);
            greg_write(off, val);
        }
        */
        else
            usage(argv[0]);
    }
    //switch gmii r [phyid] [phyreg] [mask]
    //switch gmii w [phyid] [phyreg] [value] [mask]
    else if (!strncmp(argv[1], "gmii", sizeof("gmii"))) {
        
        if (argc < 5) {
            usage(argv[0]);
        }
        //for argc >= 5
        if (!strncmp(argv[2], "r", 2)) {
            gmii_read(argc, argv);
        }
        else if (!strncmp(argv[2], "w", 2)) {
            gmii_write(argc, argv);
        }
        else
            usage(argv[0]);
    }
    //switch mii r [phyid] [phyreg] [mask]
    //switch mii w [phyid] [phyreg] [vlaue] [mask]
    else if (!strncmp(argv[1], "mii", sizeof("mii"))) {
    
        if (argc < 5) {
            usage(argv[0]);
        }
        //for argc >= 5
        if (!strncmp(argv[2], "r", 2)) {
            mii_read(argc, argv);
        }
        else if (!strncmp(argv[2], "w", 2)) {
            mii_write(argc, argv);
        }
        else
            usage(argv[0]);
     }
    //set VLAN on AR8316 via VLAN table operation
    else if (!strncmp(argv[1], "gvlan", 5)) {
        if (argc < 3)
            usage(argv[0]);
        if (!strncmp(argv[2], "set", 4)) {
            if(*add_vt) {
                add_vt(argc, argv);
            }
            else
                printf("no support target platform\n");

        }
        else if (!strncmp(argv[2], "remove", 7))  {
            if(*remove_vt) {
                remove_vt(argc, argv);
            }
            else
                printf("no support target platform\n");
        }
        else if (!strncmp(argv[2], "clean", 6)) {
            if(*clean_vt) {
                clean_vt(argc, argv);
            }
            else
                printf("no support target platform\n");
        }
        else if (!strncmp(argv[2], "dump", 5)) {
            if(*show_vt) {
                show_vt(argc, argv);
            }
            else
                printf("no support target platform\n");
        }
        else
            usage(argv[0]);
    }
    //set VLAN on AR8316 via port-based VLAN
    else if (!strncmp(argv[1], "gport", 6)) {
        if (argc < 3)
            usage(argv[0]);
        if (!strncmp(argv[2], "dump", 5)) {
            if(*show_port) {
                show_port(argc, argv);
            }
            else
                printf("no support target platform\n");
        } 
        else {
            if(*set_port) {
                set_port(argc, argv);
            }
            else
                printf("no support target platform\n");
        }
    }
    //get port status
    //switch portst [link|spd|duplex|fc] [portid]
    else if (!strncmp(argv[1], "portst", 7)) {
        get_port_status(argc, argv);
    }
    //get external port status
    //switch extportst [link|spd|duplex|fc] [portid]
    else if (!strncmp(argv[1], "extportst", 10)) {
        get_extport_status(argc, argv);
    }
    else
        usage(argv[0]);

    switch_fini();
    return 0;
}

