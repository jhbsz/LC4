#ifndef _NVRAM_FW_H_
#define _NVRAM_FW_H_

typedef enum
{
    NVRAM_FW_0_0_0 = 0,
    NVRAM_FW_1_6_5,
    NVRAM_FW_1_6_6,
    NVRAM_FW_1_6_7,
    NVRAM_FW_1_7_0,
    NVRAM_FW_1_7_1,
    NVRAM_FW_1_7_2,
    NVRAM_FW_1_7_3,
    NVRAM_FW_1_7_4,
    NVRAM_FW_1_7_5,
    NVRAM_FW_1_7_6,
    NVRAM_FW_1_7_7,
    NVRAM_FW_1_7_8,
    NVRAM_FW_1_7_9,
    NVRAM_FW_1_7_10,
    NVRAM_FW_1_7_11,
    NVRAM_FW_2_0_0,
    NVRAM_FW_2_0_1,
    NVRAM_FW_2_0_2,
    NVRAM_FW_2_0_3,
    NVRAM_FW_2_0_4,
    NVRAM_FW_2_0_5,
    NVRAM_FW_2_0_6,
    NVRAM_FW_2_0_7,
    NVRAM_FW_2_0_8,
    NVRAM_FW_2_0_9,
    NVRAM_FW_2_0_10,
    NVRAM_FW_2_0_11,
    NVRAM_FW_2_0_12,
    NVRAM_FW_2_0_13,
    NVRAM_FW_2_0_14,
    NVRAM_FW_2_0_15,
    NVRAM_FW_3_0_0,
    NVRAM_FW_3_0_1,
    NVRAM_FW_3_0_2,
    NVRAM_FW_3_0_3,
    NVRAM_FW_3_0_4,
    NVRAM_FW_3_0_5,
    NVRAM_FW_3_0_6,
    NVRAM_FW_3_0_7,
    NVRAM_FW_3_0_8,
    NVRAM_FW_3_0_9,
    NVRAM_FW_3_0_10,
    NVRAM_FW_3_0_11,
    NVRAM_FW_3_0_12,
    NVRAM_FW_3_0_13,
    NVRAM_FW_3_0_14,
    NVRAM_FW_3_0_15,
    NVRAM_FW_3_0_16,
    NVRAM_FW_3_0_17,
    NVRAM_FW_3_0_18,
    NVRAM_FW_3_0_19,
    NVRAM_FW_3_0_20,
    NVRAM_FW_3_1_0,
    NVRAM_FW_3_1_1,
    NVRAM_FW_3_1_2,
    NVRAM_FW_3_1_3,
    NVRAM_FW_3_1_4,
    NVRAM_FW_3_1_5,
    NVRAM_FW_3_1_6,
    NVRAM_FW_3_1_7,
    NVRAM_FW_3_1_8,
    NVRAM_FW_3_1_9,
    NVRAM_FW_3_1_10,
    NVRAM_FW_3_2_0,
    NVRAM_FW_3_2_1,
    NVRAM_FW_3_2_2,
    NVRAM_FW_INVALID,
} nvram_fw_version;

struct nvram_fw_tuple {
    char *fw_str;
    nvram_fw_version fw_version;
    int (*fw_upgrade_func)(void); 
    int (*fw_downgrade_func)(void); 
};


#endif /* _NVRAM_FW_H_ */

