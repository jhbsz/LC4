#!/bin/sh
#
#    Settings:
#        VLAN#1 - 
#            VID=1
#            GRP=P0,P1,P2,P5
#            TAG=P0,P5
#            UNT=P1,P2
#
#        VLAN#2 -
#            VID=2
#            GRP=P0,P3
#            TAG=P0
#            UNT=P3
#
#        VLAN#3 -
#            VID=3
#            GRP=P0,P4,P5
#            TAG=P0,P5
#            UNT=P4
#

#   Steps:
#--  Set Port to check mode  --
#switch greg w 0x660 0x14027e
#switch greg w 0x66c 0x14027d
#switch greg w 0x678 0x14027b
#switch greg w 0x684 0x140277
#switch greg w 0x690 0x14026f
#switch greg w 0x69c 0x14025f

#--  Set Port to Secure mode  --
switch greg w 0x660 0x14037e
switch greg w 0x66c 0x14037d
switch greg w 0x678 0x14037b
switch greg w 0x684 0x140377
switch greg w 0x690 0x14036f
switch greg w 0x69c 0x14035f

#--  Set Default CVid  --
switch greg w 0x420 0x10001
switch greg w 0x428 0x10001
switch greg w 0x430 0x10001
switch greg w 0x438 0x20001
switch greg w 0x440 0x30001
switch greg w 0x448 0x30001

#--  Create VLAN entry  --
switch greg w 0x614 0x80000001
switch greg w 0x614 0x80000002
switch greg w 0x610 0x1bbd60
switch greg w 0x614 0x80010002
switch greg w 0x610 0x1bf7e0
switch greg w 0x614 0x80020002
switch greg w 0x610 0x1b9fe0
switch greg w 0x614 0x80030002


