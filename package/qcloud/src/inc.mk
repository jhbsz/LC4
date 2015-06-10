# get inc.mk directory
INC_MK_DIR = $(dir $(lastword $(MAKEFILE_LIST)))

# include version file
include ${INC_MK_DIR}/version

# setup version
ifeq ('y${BUILD_NUM}', 'y')
	VERSION=${LIB_VERSION}.${REVISION}
else
	VERSION=${LIB_VERSION}.${BUILD_NUM}
endif
REV="\"${VERSION}\""

# setup build date
BUILDTIME = "\"`date "+%4Y%2m%2d%2H%2M"`\""

# add common CFLAGS
EXTRA_CFLAGS += -g -DMAXLOGFILESIZE=256000 -DENABLE_QNAP_LOG -DBUILDTIME=${BUILDTIME} -DVERSION=${REV}

# add common CFLAGS
EXTRA_LDFLAGS += -lcrypto -lssl
