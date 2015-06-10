#****************************************************************************************************************************************
#
#	Copyright (c) 2008  QNAP Systems, Inc.  All Rights Reserved.
#
#	FILE:
#		Makefile
#
#	Abstract: 
#		Makefile for Quick Setup Server Daemon.
#
#	HISTORY:
#		12/12/00	Meiji Chang - reformated
#		11/01/00	Bird Lee - created
#
#	Definition:
#		Make INPUT:
#			ARCH - platform type, regarding to endian type, etc...
#			MODEL - maybe Model or Internal Model, refer to the model Makefile.  Used to identify model specific configuration.
#			CONFIG_MODEL - user aware model name, used when MODEL was used to specify generic model.
#		Compile time:
#			TS_101 - speciy the one drive model, like TS-101 or TS-109.
#			TS_201 - specify multiple drive models.
#			NVR_FINDER - specify this is a NVR finder.
#			ONE_BAY - specify this is one bay model.
#			FOUR_BAY - specify this is 4 bay model.
#			FIVE_BAY - specify this is 5 bay model.
#			SIX_BAY - specify this is 6 bay model.
#			EIGHT_BAY - specify this is 8 bay model.
#			TWO_NIC - specify this is dual-LAN model.
#			NIC_BONDING_SUPPORT - specify this model is with bonding/standalone NIC settings.
#			USE_NEW_LOG_BY_WILLIAM - specify to compile with new log system developed by William Kao
#****************************************************************************************************************************************
include aximcom_rt5350.mk

LDFLAGS += -L../ezp-nvram -lnvram -lshared -lpthread

#DEBUG_FLAG = -g -DDEBUG
#DEBUG_FLAG = -DDEBUG
INSTALL_PATH = /sbin

# Hardware architecture related configuration
ARCH_FLAG = -DUSE_BIG_ENDIAN 
ifeq ($(ARCH),ppc)
	ARCH_FLAG = -DUSE_BIG_ENDIAN
endif
ifeq ($(ARCH),powerpc)
	ARCH_FLAG = -DUSE_BIG_ENDIAN
endif
ifeq ($(ARCH),arm)
	ARCH_FLAG =
endif
ifeq ($(ARCH),i386)
	ARCH_FLAG =
endif
ifeq ($(ARCH),x86)
	ARCH_FLAG =
endif
ifeq ($(ARCH),mips)
	ARCH_FLAG =
endif

# New implementation should use CONFIG_MODEL instead
# CONFIG_MODEL used to define model related attribues, such as disk number, camera number, ...
ifeq ($(MODEL),MR-102N)
	FINDER_MODEL = ROUTER
endif

TARGET = bcclient

CC = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip
CFLAG = -Wall -O2 -c -D${TARGET_PLATFORM} -DUSE_SHORT_ADMIN_NAME

ifeq ($(FINDER_MODEL),ROUTER)
  CFLAG += $(CFLAGS) -std=gnu99
  ifdef GCC
  CPP = $(GCC)
  endif
  ifndef CPP
  CPP = $(CROSS_COMPILE)gcc -std=gnu99
  endif
  HEADERS = finder.h protocols.h router.h dbg.h encrypt.h
  SOURCES = bcclient.c protocols.c dbg.c router.c encrypt.c
  OBJS = bcclient.o protocols.o dbg.o router.o encrypt.o
endif

all: 	$(OBJS) 
	$(CPP) -Wall -O2  $(LDFLAGS) $(OBJS) -o $(TARGET)

bcclient.o: $(HEADERS) $(SOURCES) 
	$(CPP) $(CFLAG) $(ARCH_FLAG) $(DEBUG_FLAG) bcclient.c -o $@
	
protocols.o: protocols.c protocols.h
	$(CPP) $(CFLAG) $(ARCH_FLAG) $(DEBUG_FLAG) protocols.c -o $@

dbg.o: dbg.c dbg.h
	$(CPP) $(CFLAG) $(ARCH_FLAG) ${BONDING_FLAG} $(DEBUG_FLAG) dbg.c -o $@

router.o: router.c
	$(CPP) $(CFLAG) $(ARCH_FLAG) ${BONDING_FLAG} $(DEBUG_FLAG) router.c -o $@

clean :
	@ echo "Clean files..."
	@ rm -f *.o core *~
