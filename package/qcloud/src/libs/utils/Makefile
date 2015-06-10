#  list source files
UTILS_SRCS = log.c buff.c error_code.c util.c sha1.c conf.c util_aes.c qcloud_util.c 

# convert to obj files
UTILS_OBJS = $(subst .c,.o,$(UTILS_SRCS))

# output
UTILS_OUTPUT = libqcloud_util.so

# include common makefile settings
include ../../inc.mk

# modify flags
EXTRA_CFLAGS += -I./include -DUSE_HOSTCC

build: ${UTILS_OUTPUT}

${UTILS_OUTPUT}: ${UTILS_OBJS}
	${CC} -s -shared -o ${UTILS_OUTPUT} ${UTILS_OBJS}

clean:
	rm -f *.o ${UTILS_OUTPUT}

%.o: %.c
	${CC} ${CFLAGS} ${EXTRA_CFLAGS} -c -o $@ $<
