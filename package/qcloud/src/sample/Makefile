#  list source files
SAMPLE_SRCS = qcloud_sample.c

# convert to obj files
SAMPLE_OBJS = $(subst .c,.o,$(SAMPLE_SRCS))

# output
SAMPLE_OUTPUT = qcloud_sample

# include common makefile settings
include ../inc.mk

# modify flags
EXTRA_CFLAGS += -I../libs/qid/include -I../libs/utils/include
EXTRA_LDFLAGS += -L../libs/qid -lqcloud -lmcn -L../libs/utils -lqcloud_util -ljson-c -lcurl -lsqlite3 -lm -lz

build: ${SAMPLE_OUTPUT}

${SAMPLE_OUTPUT}: ${SAMPLE_OBJS}
	${CC} -s -o ${SAMPLE_OUTPUT} $(SAMPLE_OBJS) ${LDFLAGS} ${EXTRA_LDFLAGS}

clean:
	rm -f *.o ${SAMPLE_OUTPUT}

%.o: %.c
	${CC} ${CFLAGS} ${EXTRA_CFLAGS} -c -o $@ $<
