#  list source files
QID_SRCS = qid_system.c qid_httpc.c qid_api.c mcn_api.c qcloud_gen_apirequest.c qcloud_dao.c
MCN_SRCS = mcn_api.c

# convert to obj files
QID_OBJS = $(subst .c,.o,$(QID_SRCS))
MCN_OBJS = $(subst .c,.o,$(MCN_SRCS))

# output
QID_OUTPUT = libqcloud.so
MCN_OUTPUT = libmcn.so

# include common makefile settings
include ../../inc.mk

# modify flags
EXTRA_CFLAGS += -I./include -I../utils/include

build: ${QID_OUTPUT} ${MCN_OUTPUT}

${QID_OUTPUT}: ${QID_OBJS}
	${CC} -s -shared -o ${QID_OUTPUT} ${QID_OBJS}

${MCN_OUTPUT}: ${MCN_OBJS}
	${CC} -s -shared -o ${MCN_OUTPUT} $(MCN_OBJS)

clean:
	rm -f *.o ${QID_OUTPUT} ${MCN_OUTPUT}

%.o: %.c
	${CC} ${CFLAGS} ${EXTRA_CFLAGS} -c -o $@ $<
