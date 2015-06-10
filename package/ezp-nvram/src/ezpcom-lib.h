#ifndef _EZPCOM_LIB_H
#define _EZPCOM_LIB_H

/* 
 * THE VENDOR TABLE 
 */

#define HOTPLUG_USB 		0x01
#define HOTPLUG_SCSI_GENERIC 	0x02
#define HOTPLUG_BOTH (HOTPLUG_USB|HOTPLUG_SCSI_GENERIC)

#define EZP_COM_VENDOR_MAP(X)                                   \
    X(AUTO, "Auto", 0x3789, 1)                                  \
    X(3GO, "3GO", 0x1e89, 1)         \
    X(ISONIC, "iSonic", 0x19d2, 1)         \
    X(ALINK, "A-Link", 0x1e0e, 1)                               \
    X(ALCATEL, "ALCATEL / Vibo", 0x1c9e, 1)                            \
    X(ALCATEL2, "ALCATEL (Alternative)", 0x1bbb, 1)                            \
    X(AMOI, "AMOI", 0x1614, 1)                            \
    X(ANYDATA, "AnyDATA", 0x16d5, 0)                            \
    X(ASUS, "ASUSTek", 0x0b05, 1)                               \
    X(AUDIOVOX, "AUDIOVOX", 0x0f3d, 0)                          \
    X(BANDLUXE, "Bandluxe", 0x1a8d, 1)                          \
    X(BENQ, "BENQ", 0x19d2, 1)                                  \
    X(CMOTECH, "C-Motech", 0x16da, 1)               \
    X(CINTERION, "Cinterion", 0x1e2d, 1)               \
    X(DLINK, "D-Link", 0x07d1, 1)                          \
    X(DLINK2, "D-Link (Alternative)", 0x1186, 1)                          \
    X(DLINK3, "D-Link (DWR)", 0x2001, 1)                          \
    X(DUPONT, "Dupont/Toshiba (Alternative)", 0x1d09, 1)               \
    X(EPIVALLEY, "EpiValley", 0x1b7d, 1)                          \
    X(HUAWEI, "Huawei", 0x12d1, 1)                              \
    X(HUAWEI2, "Huawei (Alternative)", 0x1033, 1)               \
    X(HUMMER, "Hummer", 0x1ab7, 1)                              \
    X(LG, "LG", 0x1004, 1)                                               \
    X(LONGLUNG, "LONGLUNG", 0x1c9e, 1)                            \
    X(MAXON, "Maxon", 0x16d8, 1)               \
    X(QMI, "Netgear", 0x1199, 1)                              \
    X(NOKIA, "Nokia", 0x0421, 1)               \
    X(NOVATEL_MERLIN, "Novatel", 0x1410, 1)            \
    X(OPTION, "Option / SimCom / SamSung", 0x05c6, 1)           \
    X(OPTION2, "Option (HSO) / AT&T", 0x0af0, 1)                \
    X(QISDA, "Qisda", 0x1da5, 1)                          \
    X(QUANTA, "Quanta / NU / Royal", 0x0408, 1)                              \
    X(UTSTARCOM, "Pantech", 0x106c, 1)         \
    X(UTSTARCOM2, "Pantech", 0x10a9, 1)         \
    X(SAMSUNG, "SamSung", 0x04e9, 0)                            \
    X(SAMSUNG2, "SamSung (Alternative)", 0x04e8, 1)                            \
    X(SIERRA, "Sierra", 0x1199, 1)                              \
    X(SOLOMON, "Solomon", 0x1dd6, 1)         \
    X(SONYERICSSON, "Sony Ericsson", 0x0fce, 1)                 \
    X(Spreadtrum, "Spreadtrum", 0x1782, 1)                                    \
    X(TOSHIBA, "Toshiba", 0x0930, 1)         \
    X(TPLINK, "TP-Link", 0x2357, 1)         \
    X(WINBEST, "WinBest", 0x1c9e, 1)                            \
    X(ZTE, "ZTE", 0x19d2, 1)                                    \
    X(UBLOX, "UBLOX", 0x05c6, 1)                                    \
    X(EMPTY, "", 0x0, 0)                                        \


/* 
 * THE PRODUCT TABLE TO A SPECIFIC VENDOR
 * (1) Device Name
 * (2) Default Device, Target Vendor, Target Device, Target Class, MessageEndpoint, * ResponseEndpoint,
 * (3) DetachStorageOnly, Mode, Interface, Hotplug Point,
 * (4) Device Num, Data, Ctrl,
 * (5) MessageContent, Shown
 *
 * XXX: 
 * I. For the multiple-message devices, the entries (other than the last 
 * message) should be set with 0xF0. The last entries should be set with 0x00.
 * 
 * II. Also, in the first message, flag should be set. Others should be set
 * 0.
 *
 * III. For the express card, the "Default Device" is set as 0xFFFF.
 * 
 */

/* Empty definition. */
#define EZP_COM_MAP_EMPTY(X)                        \
    X("",                                           \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,          \
            0, 0, 0x0, HOTPLUG_BOTH,                  \
            0, 0, 0, "",                            \
            NULL, 0)                                \


#define EZP_COM_MAP_AUTO(X)                       \
    X("Auto",                                       \
            0x5233, EMPTY, 0x0, 0x0, 0x0, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                              \
            0, 0, 0, "",                         \
            NULL, 1 )                               \
    X("",                                           \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,          \
            0, 0, 0x0, HOTPLUG_BOTH,                              \
            0, 0, 0, "",                            \
            NULL, 0)                                \

/* We intentionally use 0x as TargetProduct. */
#define EZP_COM_MAP_HUAWEI(X)                       \
    X("Generic",                                  \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1 )                               \
    X("E3131",       \
            0x14fe, HUAWEI, 0x1001, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_SCSI_GENERIC,                              \
            4, 0, 0, "ttyUSB",                         \
	        "55534243123456780000000000000011062000000000000100000000000000", 1) \
    X("E3131-1",       \
            0x1f01, HUAWEI, 0x1001, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_SCSI_GENERIC,                              \
            3, 0, 0, "ttyUSB",                         \
	        "55534243123456780000000000000011060000000000000000000000000000", 1) \
    X("E122",       \
            0x1446, HUAWEI, 0x1001, 0x0, 0x3, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                             \
            2, 0, 0, "ttyUSB",                         \
            "55534243123456780000000000000011060000000000000000000000000000", 1) \
    X("B970",                                       \
            0xFFFF, HUAWEI, 0x1001, 0x0, 0x0, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E1550",       \
            0x1446, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 0, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
     X("E156G",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E160E",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E160G",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E161",       \
            0x1446, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 0, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("E169",                                    \
            0x1001, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                              \
            3, 0, 0, "ttyUSB",                         \
            NULL, 1 )                               \
    X("E1692",                                    \
            0x1446, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
        X("E169G",                                    \
            0x1001, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                              \
            2, 0, 1, "ttyUSB",                         \
            "55534243123456780000000000000011060000000000000000000000000000", 1) \
    X("E169U",                                    \
            0x1446, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("E170",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E172",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E1750",                                    \
            0x1446, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("E1762",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E176G",                                  \
            0x140c, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1 )                               \
    X("E177",                                  \
            0x1446, HUAWEI, 0x140c, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                              \
            3, 0, 0, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("E180",       \
            0x1414, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E182E",       \
            0x1446, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 0, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("E219",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E220",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E226",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E270",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E353",       \
            0x14fe, HUAWEI, 0x0, 0x0, 0x0f, 0x8f,       \
            0, 0, 0x0, HOTPLUG_BOTH,                              \
            3, 0, 0, "ttyUSB",                         \
            "55534243123456780000000000000011062000000100000000000000000000", 1) \
    X("E367",       \
            0x1446, HUAWEI, 0x14ac, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 0, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("E368",       \
            0x1446, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 0, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("E392",      \
            0x1505, HUAWEI, 0x140c, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                              \
            2, 0, 0, "ttyUSB",                         \
            "55534243123456780000000000000a11062000000000000100000000000000", 0) \
    X("E398",       \
            0x1505, HUAWEI, 0x1506, 0x0, 0x01, 0x81,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 0, "ttyUSB",                         \
            "55534243123456780000000000000011062000000100000000000000000000", 1) \
    X("E800",       \
            0x1003, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("E870",                                       \
            0xFFFF, HUAWEI, 0x1003, 0x0, 0x0, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("EC1260",       \
            0x140b, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("EC1260 (Alternative)",       \
            0x140b, HUAWEI, 0x0, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                         \
            "555342431234567824000000800008FF05B112AEE102000000000000000000", 1) \
    X("EC156",                                  \
            0x1505, HUAWEI, 0x0, 0x00, 0x08, 0x87,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 2, 2, "ttyUSB",                         \
            "55534243123456780000000000000011062000000100000000000000000000", 1) \
    X("EC168C",                                  \
            0x1412, HUAWEI, 0x0, 0x00, 0x08, 0x87,       \
            0, 0, 0x0, HOTPLUG_BOTH ,                               \
            2, 2, 2, "ttyUSB",                         \
            "55534243123456780000000000000011060000000000000000000000000000", 1) \
    X("EC169",                                  \
            0x1446, HUAWEI, 0x1001, 0x0, 0x08, 0x87,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                         \
            "55534243123456780000000000000011060000000000000000000000000000", 1) \
    X("ET128",       \
            0x1da1, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 0, "ttyACM",                         \
            NULL, 1) \
    X("EW660",       \
            0x1001, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                \
    X("K3715",       \
            0x1001, HUAWEI, 0x0, 0x0, 0x0, -1,       \
            1, 1, 0x0, HOTPLUG_BOTH,                             \
            3, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("K3765",                                    \
            0x1520, HUAWEI, 0x1465, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("K4505 (HSPA+)",       \
            0x1521, HUAWEI, 0x1464, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 0, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("K4511 (HSPA+)",       \
            0x14b7, HUAWEI, 0x14cc, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 0, "ttyUSB",                         \
            "55534243123456780000000000000011062000000100000000000000000000", 1) \
    X("T1731",                                  \
            0x1446, HUAWEI, 0x140c, 0x0, 0x1, -1,       \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 0, "ttyUSB",                         \
            "55534243000000000000000000000011060000000000000000000000000000", 1) \
    X("",                                           \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,          \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0)                                \


#define EZP_COM_MAP_HUAWEI2(X)                      \
    X("E630",                                       \
            0x0035, HUAWEI, 0x1003, 0x0, 0x0, -1,   \
            1, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 1)                                  \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \


#define EZP_COM_MAP_OPTION(X)                \
    X("AnyDATA ADU-500A/ADU-510A/ADU-520A",        \
            0x1000, ANYDATA, 0x6502, 0x0, 0x08, -1,\
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("GlobeSurfer Icon 7.2",        \
            0x1000, OPTION2, 0x6901, 0x0, 0x05, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "55534243123456780000000000000601000000000000000000000000000000", 1) \
    X("GlobeSurfer Icon (Vodafone EasyBox)",        \
            0x1000, OPTION2, 0x6600, 0x0, 0x05, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "55534243123456780000000000000601000000000000000000000000000000", 1) \
    X("GlobeSurfer GT MAX 3.6",        \
            0x1000, OPTION2, 0x6600, 0x0, 0x05, 0x84,\
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "55534243123456780000000000000601000000000000000000000000000000", 1) \
    X("GlobeSurfer GT MAX 7.2",        \
            0x1000, OPTION2, 0x6701, 0x0, 0x05, -1,\
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "55534243123456780000000000000601000000000000000000000000000000", 1) \
    X("GlobeSurfer GT EXPRESS 7.2",        \
            0x1000, OPTION2, 0x6701, 0x0, 0x05, 0x84,\
            0, 0, 0x0, HOTPLUG_BOTH,                              \
            2, 0, 1, "ttyUSB",                         \
            "55534243123456780000000000000601000000000000000000000000000000", 1) \
    X("SGH-Z810",        \
            0x1000, SAMSUNG, 0x6601, 0x0, 0x05, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "55534243123456780000000000000616000000000000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \


#define EZP_COM_MAP_OPTION2(X)                \
    X("iCon 401 (HSO)",        \
            0x7401, EMPTY, 0x0, 0xff, 0x01, 0x81,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            4, 3, 1, "ttyHS",                         \
            "55534243785634120100000080000601000000000000000000000000000000", 1) \
    X("AT&T USBConnect Quicksilver (HSO)",        \
            0xd033, EMPTY, 0x0, 0xff, 0x05, -1,\
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyHS",                         \
            "55534243785634120100000080000601000000000000000000000000000000", 1) \
    X("GlobeSurfer Icon 7.2 (HSO)",        \
            0x6911, EMPTY, 0x0, 0xff, 0x05, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyHS",                         \
            "55534243123456780000000000000601000000000000000000000000000000", 1) \
    X("GlobeSurfer HSUPA (HSO)",        \
            0x7011, EMPTY, 0x0, 0xff, 0x05, -1,\
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyHS",                         \
            "55534243785634120100000080000601000000000000000000000000000000", 1) \
    X("Icon 225 (HSO)",        \
            0x6971, EMPTY, 0x0, 0xff, 0x05, -1,\
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 3, 3, "ttyHS",                         \
            "55534243785634120100000080000601000000000000000000000000000000", 1) \
    X("Vodafone K3760 (HSO)",        \
            0x7501, EMPTY, 0x0, 0xff, 0x01, -1,\
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyHS",                         \
            "55534243785634120100000080000601000000000000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \


/*
 * In Australia, pre-paid MF626 is buggy! When the data is on, any access to
 * the control tty causes the modem panic! So in "Generic" and
 * "MF628+/MF626/MF636/MF637" both have the same data and tty nums.
 */ 
/*
 * In South Africa, K3565-Z does the following steps:
 * # Standard SCSI eject
 * #usb_modeswitch -v ${VID} -p ${PID} -M
 * 5553424312345678000000000000061b000000020000000000000000000000 -R 1
 * # Storage passthrough
 * usb_modeswitch -v ${VID} -p ${PID} -M
 * 55534243123456782400000080000C85000000240000000000000000000000 -R 1
 * NB: It seems some ZTE devices require USB reset. e.g. ZTE MF628 from TWN
 * Mobile. We leverage 0x80 at Mode to indicate reset.
 */

#define EZP_COM_MAP_ZTE(X)                      \
    X("Generic",                                       \
            0x2000, ZTE, 0x0031, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("K3565-Z",                                       \
            0x2000, ZTE, 0x0063, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            5, 3, 1, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MC2716",                                       \
            0xfff1, ZTE, 0xfff1, 0x0, 0x04, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 0, "ttyUSB",                         \
            "55534243f8f993882000000080000a85010101180101010101000000000000", 1) \
    X("MF622",                                       \
            0x2000, ZTE, 0x0002, 0x0, 0x04, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 1, "ttyUSB",                         \
            "55534243f8f993882000000080000a85010101180101010101000000000000", 1)\
    X("MF622 (version 2)",                                       \
            0x2000, ZTE, 0x0001, 0x0, 0x0, -1,   \
            1, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 1, "ttyUSB",                         \
            NULL, 1) \
    X("MF626",                                       \
            0x2000, ZTE, 0x0031, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF628",                                       \
            0x2000, ZTE, 0x0015, 0x0, 0x08, -1,   \
            0, 0x80, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 1, "ttyUSB",                         \
            "5553424312345678000000000000061b000000030000000000000000000000", 1) \
    X("MF628+",                                       \
            0x2000, ZTE, 0x0031, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF631",                                       \
            0x0053, ZTE, 0x0117, 0x0, 0x01, -1,   \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            2, 2, 2, "ttyUSB",                         \
            NULL, 1) \
    X("MF633",                                       \
            0x2000, ZTE, 0x0031, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF636",                                       \
            0x2000, ZTE, 0x0031, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF636+",                                       \
            0x2003, ZTE, 0x2003, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 3, 3, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF637",                                       \
            0x2000, ZTE, 0x0031, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF638",                                       \
            0x2000, ZTE, 0x0037, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 1, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF668",                                       \
            0x2000, ZTE, 0x0016, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 1, 1, "ttyUSB",                         \
            NULL, 1) \
    X("MF668 (HSPA+)",                                       \
            0x2000, ZTE, 0x0017, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            4, 2, 1, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("MF820",                                       \
            0x0166, ZTE, 0x0167, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 3, 3, "ttyUSB",                         \
            "55534243e856e2892400000080000685000000240000000000000000000000", 1)   \
    X("MF821",                                       \
            0x0166, ZTE, 0x0257, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 3, 3, "ttyUSB",                         \
            "55534243123456782400000080000685000000240000000000000000000000", 1)   \
        X("MF190",                                       \
            0x0083, ZTE, 0x0117, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                              \
            3, 0, 2, "ttyUSB",                         \
            "55534243e856e2892400000080000685000000240000000000000000000000", 1)   \
    X("MU351",                                       \
            0x0003, ZTE, 0x0037, 0x0, 0x01, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 1, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("ONDA MT503HS",                                       \
            0x2000, ZTE, 0x0002, 0x0, 0x08, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 1, "ttyUSB",                         \
            "55534243b0c8dc812000000080000a85010101180101010101000000000000", 1) \
    X("ONDA MT505UP",                                       \
            0x2000, ZTE, 0x0002, 0x0, 0x03, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 1, "ttyUSB",                         \
            "55534243123456780000010080000a28000000001c00002000000000000000", 1) \
    X("ONDA MT525UP",                                       \
            0x2000, ZTE, 0x0002, 0x0, 0x0, -1,   \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 1, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \


/* Bandluxe has its own mode to switch. It uses the same tty for data and
 * control. We should use ttyUSB0 to set up APN before dialing up.
 */
#define EZP_COM_MAP_BANDLUXE(X)                \
    X("C100",        \
            0xFFFF, BANDLUXE, 0x1002, 0x0, 0x05, 0x82,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("C100 (Alternative)",        \
            0x1000, BANDLUXE, 0x1002, 0x0, 0x05, 0x82,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("C120/C170",        \
            0x1000, BANDLUXE, 0x1002, 0x0, 0x05, 0x82,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("C270",        \
            0x1000, BANDLUXE, 0x1009, 0x0, 0x05, 0x82,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("M152",        \
            0x1007, BANDLUXE, 0x1007, 0x0, 0x05, 0x82,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("M300LC (HSPA+)",        \
            0x100b, BANDLUXE, 0x1002, 0x0, 0x05, 0x82,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                         \
            NULL, 0x3) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

/*
    X("C100/C120/C170",        \
            0x1000, BANDLUXE, 0x1002, 0x0, 0x05, 0x82,  \
            0, 0, 0x0,                                  \
            2, 0, 0, "ttyUSB",                             \
            "55534243109298820000000000000600000000000000000000000000000000", 1) \
    X("C100/C120",        \
            0x1000, BANDLUXE, 0x1002, 0x0, 0x05, 0x82,  \
            0, 0, 0x0,                                  \
            2, 0, 0, "ttyUSB",                          \
            "555342439034a0820000000000000600000000000000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0,                              \
            0, 0, 0, "",                            \
            NULL, 0) \
*/

#define EZP_COM_MAP_EPIVALLEY(X)                \
    X("SEC-8089",        \
            0x0700, EPIVALLEY, 0x0, 0x0, 0x0, -1,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyACM",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

/* Qisda has its own mode to switch. */
#define EZP_COM_MAP_QISDA(X)                \
    X("H21",        \
            0xf000, QISDA, 0x4512, 0x0, 0x0, 0x0,  \
            0, 5, 0x0, HOTPLUG_BOTH,                               \
            3, 1, 1, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

/* 
 * D-Link needs kernel support without switching.. 
 * There are two models of DWM-156
 * - One is switched by ignored the storage kernel module. (Alternative)
 * - Another is switched by the touch mode. 
 */
#define EZP_COM_MAP_DLINK(X)                \
    X("DWM-156",        \
            0xa800, DLINK, 0x3e02, 0x0, 0x0, 0x0,  \
            0, 7, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("DWM-156 (FW 1.0.3)",        \
            0xa800, DLINK, 0x3e02, 0x0, 0x0, 0x0,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("DWM-156 (FW 3.00)",        \
            0xa804, DLINK, 0x7e11, 0x0, 0x0, 0x0,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            4, 2, 2, "ttyUSB",                         \
            NULL, 1) \
    X("DWM-156 (Alternative)",        \
            0x3e02, DLINK, 0x3e02, 0x0, 0x0, 0x0,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

/* D-Link needs kernel support without switching.. */
#define EZP_COM_MAP_DLINK2(X)                \
    X("DWM-652",        \
            0x3e04, DLINK, 0x3e04, 0x0, 0x0, 0x0,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_DLINK3(X)                \
    X("DWM-157",        \
            0xa809, DLINK3, 0x7901, 0x0, 0x1, 0x81,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            1, 1, 1, "ttyACM",                         \
            NULL, 1) \
    X("DWM-157(B1_1)",        \
            0xa706, DLINK3, 0x7d01, 0x0, 0x1, 0x81,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            6, 2, 2, "ttyUSB",                         \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("DWM-157(B1_2)",        \
            0xa707, DLINK3, 0x7d02, 0x0, 0x1, 0x81,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            6, 2, 2, "ttyUSB",                         \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("DWM-157(B1_3)",        \
            0xa708, DLINK3, 0x7d03, 0x0, 0x1, 0x81,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            6, 2, 2, "ttyUSB",                         \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("DWR-510",        \
            0xa805, DLINK, 0x7e12, 0x0, 0x0, 0x0,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            2, 2, 2, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_DUPONT(X)                \
    X("BC2703",        \
            0xFFFF, DUPONT, 0x4000, 0x0, 0x0, 0x0,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("G450",        \
            0x1022, BANDLUXE, 0x1011, 0x0, 0x0, -1,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_NOVATEL_MERLIN(X)                      \
    X("4620L",     \
            0xFFFF, NOVATEL_MERLIN, 0xb005, 0x0, 0x01, -1,   \
            0, 0, 0x5, HOTPLUG_BOTH,                               \
            4, 0, 0, "ttyUSB",                       \
            NULL, 1) \
    X("MC990D",     \
            0x5020, EMPTY, 0x0, 0x0, 0x01, -1,   \
            0, 0, 0x5, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                       \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("Ovation 930D/MC950D",     \
            0x5010, NOVATEL_MERLIN, 0x4400, 0x0, 0x09, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                       \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("Wireless U679",     \
            0x5059, NOVATEL_MERLIN, 0x7031, 0x0, 0x1, 0x81,   \
            0, 0, 0x0, HOTPLUG_BOTH,                              \
            2, 0, 0, "ttyUSB",                       \
            "5553424312345678000000000000061b000000020000000000000000000000", 4) \
    X("Wireless U998",     \
            0x5010, NOVATEL_MERLIN, 0x7030, 0x0, 0x1, 0x81,   \
            0, 0, 0x0, HOTPLUG_BOTH,                             \
            2, 0, 0, "ttyUSB",                       \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("U727 ttyUSB Modem",     \
            0x5010, NOVATEL_MERLIN, 0x4100, 0x0, 0x08, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                       \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("USB551L",     \
            0xb001, NOVATEL_MERLIN, 0xb001, 0x0, 0x0, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 0, "ttyUSB",                       \
            NULL, 4) \
    X("Wireless MC760",     \
            0x5031, NOVATEL_MERLIN, 0x6002, 0x0, 0x08, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                       \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("Wireless U679",     \
            0x5059, NOVATEL_MERLIN, 0x7031, 0x0, 0x1, 0x81,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                       \
            "5553424312345678000000000000061b000000020000000000000000000000", 4) \
    X("Wireless U760",     \
            0x5030, NOVATEL_MERLIN, 0x6000, 0x0, 0x08, -1,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                       \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("Wireless U998",     \
            0x5010, NOVATEL_MERLIN, 0x7030, 0x0, 0x1, 0x81,   \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyUSB",                       \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                          \
            NULL, 0) \


#define EZP_COM_MAP_ALCATEL(X)                \
    X("D200",        \
            0xf000, ALCATEL, 0x9605, 0x0, 0x01, 0x81,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 3, 3, "ttyUSB",                         \
            "55534243123456788000000080000606f50402527000000000000000000000", 1) \
    X("OT-X020/DBM-100HU (Nuton 3.5G)",        \
            0x1001, ALCATEL, 0x6061, 0x0, 0x05, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "55534243123456780000000000000606f50402527000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_ALCATEL2(X)                \
    X(" X220L/X215S",        \
            0xf000, ALCATEL2, 0x0017, 0x0, 0x01, 0x81,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            4, 4, 4, "ttyUSB",                         \
            "55534243123456788000000080000606f50402527000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_WINBEST(X)                \
    X("HSDC-02",        \
            0x6061, WINBEST, 0x6061, 0x0, 0x05, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "55534243123456780000000000000606f50402527000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_SONYERICSSON(X)                \
    X("MD300",        \
            0xd0cf, SONYERICSSON, 0x0, 0x02, 0x0, -1,  \
            0, 3, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyACM",                         \
            NULL, 1) \
    X("MD400",        \
            0xd0e1, SONYERICSSON, 0x0, 0x02, 0x0, -1,  \
            0, 3, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyACM",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_AMOI(X)                \
    X("H01",        \
            0xFFFF, AMOI, 0x0800, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("U2000",        \
            0xFFFF, AMOI, 0x0800, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_ALINK(X)                \
    X("3GU",        \
            0xf000, ALINK, 0x9200, 0x0, 0x01, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 1, "ttyUSB",                         \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \


#define EZP_COM_MAP_HUMMER(X)                \
    X("DTM5731",        \
            0x5700, HUMMER, 0x5731, 0x0, 0x07, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

/* ASUS T500 has two ttyttyUSBs. ttyttyUSB0 for data and ttyttyUSB1 for control. */
#define EZP_COM_MAP_ASUS(X)                \
    X("T500",        \
            0xFFFF, ASUS, 0x0302, 0x0, 0x01, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyACM",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_SOLOMON(X)                \
    X("S3Gm-660",        \
            0x1000, SOLOMON, 0x1002, 0x0, 0x05, 0x82,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "55534243123456781200000080000603000000020000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \


#define EZP_COM_MAP_CMOTECH(X)                \
    X("CGU-628",        \
            0xf000, CMOTECH, 0x6006, 0x0, 0x09, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "55534243d85dd88524000000800008ff524445564348470000000000000000", 1) \
    X("D-50/CDU-680",        \
            0x6803, CMOTECH, 0x6803, 0x0, 0x07, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "555342431234567824000000800008ff524445564348470000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_CINTERION(X)                \
    X("PH8",        \
            0xFFFF, CINTERION, 0x0053, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            5, 5, 4, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

/* Maxon BP3 seems to be a OEM of C-Motech CHx-628. */
#define EZP_COM_MAP_MAXON(X)                \
    X("BP3",        \
            0xFFFF, MAXON, 0x6280, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 1, 1, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \


#define EZP_COM_MAP_NOKIA(X)                \
    X("Internet Stick CS-11",        \
            0x061d, NOKIA, 0x061e, 0x0, 0x0, -1,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyACM",                         \
            NULL, 0x1) \
    X("Internet Stick CS-12",        \
            0x0618, NOKIA, 0x0619, 0x0, 0x0, -1,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyACM",                         \
            NULL, 0x1) \
    X("Internet Stick CS-15",        \
            0x0610, NOKIA, 0x0612, 0x0, 0x0, -1,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 0, "ttyACM",                         \
            NULL, 0x1) \
    X("5800 XpressMusic",                                       \
            0x0154, NOKIA, 0x0, 0x0, 0x0, -1,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "ttyACM",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_QMI(X)                \
    X("AirCard 340U",        \
            0x0fff, SIERRA, 0x9051, 0x0, 0x0, -1,  \
            0, 5, 0x0, HOTPLUG_BOTH,                               \
            2, 2, 2, "ttyUSB",                         \
            NULL, 5) \
    X("AirCard 341U",        \
            0x0fff, SIERRA, 0x9057, 0x0, 0x0, -1,  \
            0, 5, 0x0,  HOTPLUG_BOTH,                              \
            2, 0, 1, "ttyUSB",                         \
            NULL, 5) \
    X("AirCard 341U NAT HIDDEN",        \
            0x0fff, SIERRA, 0x9055, 0x0, 0x0, -1,  \
            0, 5, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            NULL, 5) \

/* Mode (0x1 << 1) but it won't hit the procedure of 'SierraMode' in ezpcom-mode
 * because the kernel mode of sierra has recognized the device so that there
 * is no switch happened. */
#define EZP_COM_MAP_SIERRA(X)                \
    X("AirCard 312U",        \
            0x0fff, SIERRA, 0x68a3, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            5, 2, 2, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 313U",        \
            0x0fff, AUDIOVOX, 0x68aa, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            5, 3, 3, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 330U",        \
            0x0fff, SIERRA, 0x68a3, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            5, 4, 4, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 595",        \
            0x0fff, SIERRA, 0x0019, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 595U",        \
            0x0fff, SIERRA, 0x0120, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 597",        \
            0x0025, SIERRA, 0x0023, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 598U",        \
            0x0025, SIERRA, 0x0025, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 875",        \
            0x0fff, SIERRA, 0x6820, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 0, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 875U",        \
            0x0fff, SIERRA, 0x6821, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 880",        \
            0x0fff, SIERRA, 0x6850, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 880E",        \
            0x0fff, SIERRA, 0x6852, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 880U",        \
            0x0fff, SIERRA, 0x6855, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 881",        \
            0x0fff, SIERRA, 0x6851, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 881E",        \
            0x0fff, SIERRA, 0x6853, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 2, "ttyUSB",                         \
            NULL, 1) \
    X("AirCard 881U",        \
            0x0fff, SIERRA, 0x6856, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 2, "ttyUSB",                         \
            NULL, 1) \
    X("Bell 330U",        \
            0x0fff, SIERRA, 0x68aa, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            4, 3, 3, "ttyUSB",                         \
            NULL, 1) \
    X("Compass 885",        \
            0x0fff, SIERRA, 0x6859, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            7, 4, 3, "ttyUSB",                         \
            NULL, 1) \
    X("Compass 885 (Alternative)",        \
            0x0fff, SIERRA, 0x6880, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            7, 4, 3, "ttyUSB",                         \
            NULL, 1) \
    X("Compass 888",        \
            0x0fff, SIERRA, 0x685a, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            7, 4, 3, "ttyUSB",                         \
            NULL, 1) \
    X("Compass 889",        \
            0x0fff, SIERRA, 0x6890, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            7, 3, 3, "ttyUSB",                         \
            NULL, 1) \
    X("USB 305",        \
            0xFFFF, SIERRA, 0x68a3, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("USB 306 (HSPA+)",        \
            0xFFFF, SIERRA, 0x68a3, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 2, "ttyUSB",                         \
            NULL, 0x2) \
    X("USB 308",        \
            0x68a3, AUDIOVOX, 0x68a3, 0x0, 0x0, -1,  \
            0, 2, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 2, "ttyUSB",                         \
            NULL, 0x2) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \
    
#define EZP_COM_MAP_TOSHIBA(X)                \
    X("G450",        \
            0x0d46, TOSHIBA, 0x0d45, 0x0, 0x05, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "5553424312345678000000000000061b000000020000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \


#define EZP_COM_MAP_UTSTARCOM(X)                \
    X("UM175",        \
            0x3b03, UTSTARCOM, 0x3715, 0x0, 0x05, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "ttyUSB",                         \
            "555342431234567824000000800008ff024445564348470000000000000000", 1) \
    X("UML290",        \
            0x3718, UTSTARCOM, 0x3718, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "ttyACM",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_UTSTARCOM2(X)                \
    X("UML295",        \
            0x6064, UTSTARCOM2, 0x6064, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "ttyACM",                         \
            NULL, 4) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_ISONIC(X)                      \
    X("EDGE CARD",                                       \
            0x2000, ISONIC, 0x0051, 0x0, 0x08, -1,   \
            0, 0x80, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 1, "ttyUSB",                         \
            "5553424312345678000000000000061b000000030000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_3GO(X)                      \
    X("3GO11",                                       \
            0xf000, 3GO, 0x1a20, 0x0, 0x1, 0x81,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 2, 2, "ttyUSB",                         \
            "5553424320099384800000008000060619181a207000000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_SAMSUNG2(X)                      \
    X("i9000",                                       \
            0x6877, SAMSUNG2, 0x0, 0x0, 0x0, -1,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "ttyACM",                         \
            NULL, 1) \
    X("GT-S8500",                                       \
            0x6818, SAMSUNG2, 0x6810, 0x0, 0x0, -1,  \
            0, 4, 0x0, HOTPLUG_BOTH ,                               \
            0, 0, 0, "ttyACM",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,          \
            0, 0, 0x0, HOTPLUG_BOTH ,                               \
            0, 0, 0, "",                            \
            NULL, 0)                                   \

#define EZP_COM_MAP_LG(X)                \
    X("LDU-1900D EV-DO",        \
            0x1000, EMPTY, 0x0, 0x0, 0x08, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH ,                               \
            2, 0, 1, "ttyUSB",                         \
            "55534243123456780000000000000aff554d53434847000000000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH ,                               \
            0, 0, 0, "",                            \
            NULL, 0) \


/*
#define EZP_COM_MAP_QUANTA(X)                \
    X("Q101",        \
            0x1000, QUANTA, 0xea02, 0x0, 0x05, -1,  \
            0, 0x80, 0x0,                              \
            4, 0, 2, "ttyUSB",                         \
            "5553424338474ffc0002000000000a2a000000003300000100000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0,                              \
            0, 0, 0, "",                            \
            NULL, 0) \
 */

#define EZP_COM_MAP_QUANTA(X)                \
    X("Q101",        \
            0x1000, QUANTA, 0xea02, 0x0, 0x0, -1,  \
            0, 6, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 2, "ttyUSB",                         \
            NULL, 1) \
    X("Q101 (Alternative)",        \
            0x1000, QUANTA, 0xea02, 0x0, 0x05, -1,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 2, "ttyUSB",                         \
            NULL, 1) \
    X("Q110",        \
            0xf000, QUANTA, 0xea03, 0x0, 0x0, -1,  \
            0, 7, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("Q110 (Alternative)",        \
            0xf001, QUANTA, 0xea04, 0x0, 0x0, -1,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("Q111",        \
            0xf000, QUANTA, 0xea03, 0x0, 0x0, -1,  \
            0, 7, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("Q120",        \
            0xf003, QUANTA, 0xea09, 0x0, 0x0, -1,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            4, 0, 2, "ttyUSB",                         \
            NULL, 1) \
    X("Taiwan Mobile T1/MDO 588",        \
            0xb003, QUANTA, 0xb003, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            1, 0, 0, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \


#define EZP_COM_MAP_AUDIOVOX(X)                     \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \


#define EZP_COM_MAP_ANYDATA(X)                      \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_BENQ(X)                \
    X("U31",        \
            0x2000, BENQ, 0x0039, 0x0, 0x1, 0x81,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 2, 2, "ttyUSB",                         \
            "55534243123456782000000080000c85010101180101010101000000000000", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0) \


#define EZP_COM_MAP_SAMSUNG(X)                      \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,          \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            0, 0, 0, "",                            \
            NULL, 0)                                   \


#define EZP_COM_MAP_LONGLUNG(X)                \
    X("C5300",        \
            0x9e00, LONGLUNG, 0x9e00, 0x0, 0x02, 0x81,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            3, 3, 3, "ttyUSB",                         \
            "NULL", 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "",                            \
            NULL, 0) \


#define EZP_COM_MAP_TPLINK(X)                \
    X("MA180",        \
            0x0200, TPLINK, 0x0201, 0x0, 0x01, 0x81,  \
            0, 4, 0x0, HOTPLUG_BOTH,                               \
            4, 4, 4, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                               \
            2, 0, 1, "",                            \
            NULL, 0) \

#define EZP_COM_MAP_Spreadtrum(X)                \
    X("SC7702",        \
            0x0002, Spreadtrum, 0x0002, 0x0, 0x01, 0x81,  \
            0, 0, 0x0, HOTPLUG_BOTH,                             \
            3, 3, 3, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                             \
            2, 0, 1, "",                            \
            NULL, 0) \


#define EZP_COM_MAP_UBLOX(X)                \
    X("C200",        \
            0x9004, UBLOX, 0x9004, 0x0, 0x01, 0x81,  \
            0, 0, 0x0, HOTPLUG_BOTH,                             \
            2, 3, 3, "ttyUSB",                         \
            NULL, 1) \
    X("",        \
            0x0, EMPTY, 0x0, 0x0, 0x0, -1,  \
            0, 0, 0x0, HOTPLUG_BOTH,                             \
            2, 0, 1, "",                            \
            NULL, 0) \



#define EZP_COM_VENDOR_NAME_CONVERTER(VNAME,NAME,VID,SHOWN) EZP_COM_VID_##VNAME,
typedef enum {
    EZP_COM_VENDOR_MAP(EZP_COM_VENDOR_NAME_CONVERTER)
} ezp_com_vendor_name_type;

struct ezp_com_vendor_map_entry_type
{
    ezp_com_vendor_name_type vn;
    char *desc;
    int DefaultVendor;
    char shown;
};

struct ezp_com_map_entry_type
{
    char *desc;
    int DefaultProduct;
    ezp_com_vendor_name_type TargetVendorType;
    int TargetProduct;
    int TargetClass;
    int MessageEndpoint;
    int ResponseEndpoint;
    char DetachStorageOnly;
    /* 
     * Huawei(1), Sierra(2), Sony(3), Bandluxe(4), MessageNext(0xF0)
     */
    char Mode; 
    int Interface;
    /* XXX: I intentionally ignore Configuration, AltSetting. */
    char Hotplug; /* Hotplug point: 0x01:USB 0x10:scsi_generic 0x11: both */
    int DeviceNum; 
    int DataTTY; 
    int CtrlTTY;
    char *DevName;
    char *MessageContent;
    /* show in GUI or not */
    char Flag;
};

#define EZP_COM_VENDOR_MAP_CONVERTER(VNAME,NAME,VID,SHOWN) \
    {EZP_COM_VID_##VNAME, NAME, VID, SHOWN},

static struct ezp_com_vendor_map_entry_type ezp_com_vendor_map[] = {
    EZP_COM_VENDOR_MAP(EZP_COM_VENDOR_MAP_CONVERTER)
};

#define EZP_COM_MAP_CONVERTER(NAME,PID,TVNAME,TPID,TCLS,EP,REP,DT,MODE,INTF,HOTPLUG,DNUM,DTTY,CTTY,DEVNMAE,MSG,FLAG) \
   {NAME, PID, EZP_COM_VID_##TVNAME, TPID, TCLS, EP, REP, DT, MODE, INTF, HOTPLUG, DNUM, DTTY, CTTY, DEVNMAE, MSG, FLAG},


#define EZP_COM_MAP_FOR_EACH_VENDOR_CONVERTER(VNAME,NAME,VID,SHOWN) \
static struct ezp_com_map_entry_type ezp_com_map_##VNAME[] = { \
    EZP_COM_MAP_##VNAME(EZP_COM_MAP_CONVERTER) \
}; 

/* Declare tables to each vendor. */
EZP_COM_VENDOR_MAP(EZP_COM_MAP_FOR_EACH_VENDOR_CONVERTER)


#define EZP_COM_POINTER_MAP_CONVERTER(VNAME,NAME,VID,SHOWN) &ezp_com_map_##VNAME[0],


static struct ezp_com_map_entry_type *ezp_com_pointer_map[] = {
    EZP_COM_VENDOR_MAP(EZP_COM_POINTER_MAP_CONVERTER)
};

/* ------------------------------------ */
/* For location & ISP information */
#define EZP_COM_LOCATION_MAP(X)                         \
    X(USA, "USA", 1)                                    \
    X(CANADA, "Canada", 1)                                    \
    X(AUSTRALIA, "Australia", 1)                        \
    X(CHINA, "China", 1)                                \
    X(HONGKONG, "Hongkong", 1)                          \
    X(INDIA, "India", 1)                                    \
    X(MEXICO, "Mexico", 1)                              \
    X(NETHERLANDS, "Netherland", 1)                     \
    X(NORWAY, "Norway", 1)                              \
    X(PHILIPPINES, "Philippines", 1)                    \
    X(RUSSIA, "Russia", 1)                              \
    X(SOUTH_AFRICA, "South Africa", 1)                  \
    X(TAIWAN, "Taiwan", 1)                              \
    X(EMPTY, "", 0)

/*
    X(AUSTRIA, "Austria", 1)                            \
    X(BELGIUM, "Belgium", 1)                            \
    X(BRASIL, "Brasil", 1)                              \
    X(CANADA, "Canada", 1)                              \
    X(CHILE, "Chile", 1)                                \
    X(CROATIA, "Croatia", 1)                            \
    X(CZECH, "Czech", 1)                                \
    X(DENMARK, "Denmark", 1)                            \
    X(EGYPT, "Egypt", 1)                                \
    X(ESTONIA, "Estonia", 1)                            \
    X(FINLAND, "Finland", 1)                            \
    X(FRANCE, "France", 1)                              \
    X(GERMANY, "Germany", 1)                            \
    X(GREECE, "Greece", 1)                              \
    X(HUNGARY, "Hungary", 1)                            \
    X(INDIA, "India", 1)                                \
    X(INDONESIA, "Indonesia", 1)                        \
    X(IRELAND, "Ireland", 1)                            \
    X(ITALY, "Italy", 1)                                \
    X(JAPAN, "Japan", 1)                                \
    X(LITHUANIA, "Lithuania", 1)                        \
    X(LUXEMBOURG, "Luxembourg", 1)                      \
    X(MACEDONIAN, "Macedonian", 1)                      \
    X(MALAYSIA, "Malaysia", 1)                          \
    X(MEXICO, "Mexico", 1)                              \
    X(NETHERLANDS, "Netherlands", 1)                    \
    X(NEW_ZEALAND, "New Zealand", 1)                    \
    X(POLAND, "Poland", 1)                              \
*/


/* 
 * THE ISP TABLE TO A SPECIFIC LOCATION
 * (1) APN
 * (2) PIN
 * (3) User Name
 * (4) Passwd
 * (5) Dialup String
 *
 */

/* Empty definition. */
#define EZP_COM_ISP_MAP_EMPTY(X)                                            \
    X("", "", "", "", "", "", "", 0)

#define EZP_COM_ISP_MAP_TAIWAN(X)                                           \
    X("Chunghwa Telecom", "internet", "", "chap", "", "", "*99#", 1 )       \
    X("Far EasTone (fetims)", "fetims", "", "chap", "", "", "*99#", 1 )     \
    X("Far EasTone (internet)", "internet", "", "chap", "", "", "*99#", 1 ) \
    X("Taiwan Cellular", "internet", "", "chap", "", "", "*99#", 1 )        \
    X("Vibo", "internet", "", "chap", "", "", "*99#", 1 )                   \
    X("", "", "", "", "", "", "", 0)                                        \


#define EZP_COM_ISP_MAP_CHINA(X)                                            \
    X("China Mobile", "cmnet", "", "chap", "", "", "*99#", 1 )              \
    X("China Telecom", "", "", "chap", "", "", "#777", 1 )              \
    X("China Unicom", "", "", "chap", "", "", "*99#", 1 )                   \
    X("China E Surfing", "ctnet", "", "chap", "ctnet@mycdma.cn", "vnet.mobi", "#777", 1 )                   \
    X("", "", "", "", "", "", "", 0)                                        \


#define EZP_COM_ISP_MAP_HONGKONG(X)                                         \
    X("3 HK", "ipc.three.com.hk", "", "chap", "", "", "*99#", 1 )           \
    X("CSL", "internet", "", "chap", "", "", "*99#", 1 )                    \
    X("New World", "ineternet", "", "chap", "", "", "*99#", 1 )             \
    X("People", "internet", "", "chap", "", "", "*99#", 1 )                 \
    X("PCCW", "pccw", "", "chap", "", "", "*99#", 1 )                       \
    X("SmarTone", "internet", "", "chap", "", "", "*99#", 1 )               \
    X("Sunday", "internet", "", "chap", "", "", "*99#", 1 )                 \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_INDIA(X)                                     \
    X("TATA", "TATA", "", "chap", "internet", "internet", "#777", 1 )             \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_MEXICO(X)                                           \
    X("Movistar", "internet.movistar.mx", "", "chap", "movistar", "movistar", "*99#", 1 )   \
    X("Telcel", "internet.itelcel.com", "", "chap", "webgprs", "webgprs20002", "*99#", 1 )     \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_NETHERLANDS(X)                                      \
    X("KPN", "internet", "", "chap", "", "", "*99#", 1 )                    \
    X("Telfort", "internet", "", "chap", "", "", "*99#", 1 )                \
    X("T-Mobile", "internet", "", "chap", "", "", "*99#", 1 )               \
    X("Vodafone", "internet", "", "chap", "", "", "*99#", 1 )               \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_NORWAY(X)                                           \
    X("Netcom Mobil", "Netcom", "", "chap", "", "", "*99***1#", 1 )         \
    X("Telenor Mobil", "Telenor", "", "chap", "", "", "*99***1#", 1 )       \
    X("", "", "", "", "", "", "", 0)                                        \


#define EZP_COM_ISP_MAP_PHILIPPINES(X)                                      \
    X("Globe", "www.globe.com.ph", "", "chap", "globe", "globe", "*99#", 1 ) \
    X("Smart", "internet", "", "chap", "", "", "*99#", 1 )                  \
    X("Sun Cellula", "minternet", "", "chap", "", "", "*99#", 1 )           \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_RUSSIA(X)                                           \
    X("BeeLine", "internet.beeline.ru", "", "chap", "beeline", "beeline", "*99#", 1 )  \
    X("Megafon (NWGSM)", "internet.nw", "", "chap", "", "", "*99#", 1 )     \
    X("MTS", "internet.mts.ru", "", "chap", "mts", "mts", "*99#", 1 )       \
    X("PrimTel", "internet.primtel.ru", "", "chap", "", "", "*99#", 1 )     \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_AUSTRALIA(X)                                        \
    X("iburst", "internet", "", "chap", "", "", "*99#", 1 )                 \
    X("Bigpond", "telstra.bigpond", "", "chap", "", "", "*99#", 1 )         \
    X("Dodo", "dodolns1", "", "chap", "", "", "*99#", 1 )                   \
    X("Exetel", "exetel1", "", "chap", "", "", "*99#", 1 )                  \
    X("Internode", "splns333a1", "", "chap", "", "", "*99#", 1 )            \
    X("Optus", "internet", "", "chap", "", "", "*99#", 1 )                  \
    X("Telstra", "telstra.internet", "", "chap", "", "", "*99#", 1 )        \
    X("Three", "3netaccess", "", "chap", "", "", "*99#", 1 )                \
    X("Three Prepaid", "3services", "", "chap", "", "", "*99#", 1 )         \
    X("TPG", "internet", "", "chap", "", "", "*99#", 1 )                    \
    X("Virgin", "virginbroadband", "", "pap", "", "", "*99***1#", 1)        \
    X("Vodafone", "vfinternet.au", "", "chap", "", "", "*99#", 1 )          \
    X("", "", "", "", "", "", "", 0)                                        \

#define EZP_COM_ISP_MAP_SOUTH_AFRICA(X)                                     \
    X("Cell-C", "internet", "", "chap", "Cellcis", "Cellcis", "*99***16#", 1 ) \
    X("MTN", "", "", "chap", "", "", "*99#", 1 )                            \
    X("Neotel", "", "", "chap", "", "", "#777", 1 ) \
    X("Vodacom", "internet", "", "chap", "", "", "*99***16#", 1 )           \
    X("", "", "", "", "", "", "", 0)                                        \

#if defined(EZP_PROD_BRAND_PROX)

#define EZP_COM_ISP_MAP_CANADA(X)                                      \
    X("Bell Mobility", "inet.bell.ca", "", "chap", "", "", "*99#", 1 ) \
    X("Rogers", "internet.com", "", "chap", "", "", "*99#", 1 ) 	\
    X("Rogers (VPN)", "vpn.com", "", "chap", "", "", "*99#", 1 ) 	\
    X("SaskTel", "inet.stm.sk.ca", "", "chap", "", "", "*99#", 1 ) 	\
    X("Telus", "isp.telus.com", "", "chap", "", "", "*99#", 1 )         \
    X("", "", "", "", "", "", "", 0)                                    \

#else
#define EZP_COM_ISP_MAP_CANADA(X)                                     \
    X("WIND", "internet.wind.com", "", "chap", "", "", "*99#", 1 )             \
    X("MOBILICITY", "wap.davewireless.com", "", "chap", "dave", "password", "*99#", 1 )             \
    X("ROGERS", "internet.rogers.ca", "", "chap", "rogers", "rogers", "*99#", 1 )             \
    X("ROGERS2", "internet.com", "", "chap", "wapuser1", "wap", "*99#", 1 )   \
    X("FIDO", "internet.fido.ca", "", "chap", "ifdo", "ifdo", "*99#", 1 )    \
    X("BELL", "pda.bell.ca", "", "chap", "ifdo", "ifdo", "#777", 1 )             \
    X("TELUS", "sp.telus.com", "", "chap", "", "", "*99#", 1 )             \
    X("", "", "", "", "", "", "", 0)                                        \

#endif


#if defined(EZP_PROD_BRAND_PROX)

#define EZP_COM_ISP_MAP_USA(X)                                              \
    X("AT&T (Broadband)", "Broadband", "", "chap", "", "", "*99#", 1 )  \
    X("AT&T (I2GGOLD)", "I2GOLD", "", "chap", "", "", "*99***1#", 1 ) \
    X("AT&T (Internet)", "Internet", "", "chap", "", "", "*99***1#", 1 ) \
    X("Sprint 3G", "", "", "chap", "", "", "#777", 1 ) \
    X("T-Mobile(new)", "epc.tmobile.com", "", "chap", "", "", "*99***1#", 1 ) \
    X("Verizon 3G", "", "", "chap", "", "", "#777", 1 ) \
    X("Verizon 4G/LTE", "", "", "chap", "", "", "*99***3#", 1 ) \
    X("", "", "", "", "", "", "", 0) \

#else
    #define EZP_COM_ISP_MAP_USA(X)                                              \
    X("AT&T(proxy)", "proxy", "", "chap", "", "", "*99#", 1 )  \
    X("AT&T(cingular)", "isp.cingular", "", "chap", "", "", "*99***1#", 1 ) \
    X("T-Mobile(new)", "epc.tmobile.com", "", "chap", "", "", "*99***1#", 1 ) \
    X("Verizon", "", "", "chap", "", "", "#777", 1 ) \
    X("Rogers", "internet.com", "", "chap", "wapuser1", "wap","*99***1#", 1) \
    X("Sprint", "", "", "chap", "", "", "#777", 1 ) \
    X("", "", "", "", "", "", "", 0) \

#endif

#define EZP_COM_LOCATION_NAME_CONVERTER(LNAME,NAME,SHOWN) EZP_COM_LID_##LNAME,
typedef enum {
    EZP_COM_LOCATION_MAP(EZP_COM_LOCATION_NAME_CONVERTER)
} ezp_com_location_name_type;

struct ezp_com_location_map_entry_type
{
    ezp_com_location_name_type ln;
    char *desc;
    char shown;
};

struct ezp_com_isp_map_entry_type
{
    char *desc;
    char *apn;
    char *pin;
    char *auth;
    char *user_name;
    char *passwd;
    char *dialup_str;
    char shown;
};

#define EZP_COM_LOCATION_MAP_CONVERTER(LNAME,NAME,SHOWN) \
    {EZP_COM_LID_##LNAME, NAME, SHOWN},

static struct ezp_com_location_map_entry_type ezp_com_location_map[] = {
    EZP_COM_LOCATION_MAP(EZP_COM_LOCATION_MAP_CONVERTER)
};

#define EZP_COM_ISP_MAP_CONVERTER(NAME,APN,PIN,AUTH,USER,PW,DIALUP,SHOWN) \
   {NAME, APN, PIN, AUTH, USER, PW, DIALUP, SHOWN},

#define EZP_COM_ISP_MAP_FOR_EACH_LOCATION_CONVERTER(LNAME,NAME,SHOWN) \
static struct ezp_com_isp_map_entry_type ezp_com_isp_map_##LNAME[] = { \
    EZP_COM_ISP_MAP_##LNAME(EZP_COM_ISP_MAP_CONVERTER) \
}; 

/* Declare tables to each location. */
EZP_COM_LOCATION_MAP(EZP_COM_ISP_MAP_FOR_EACH_LOCATION_CONVERTER)


#define EZP_COM_ISP_POINTER_MAP_CONVERTER(LNAME,NAME,SHOWN) &ezp_com_isp_map_##LNAME[0],


static struct ezp_com_isp_map_entry_type *ezp_com_isp_map[] = {
    EZP_COM_LOCATION_MAP(EZP_COM_ISP_POINTER_MAP_CONVERTER)
};

#endif
