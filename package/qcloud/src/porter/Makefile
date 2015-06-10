#  list source files
PORTER_SRCS = porter_ev.c porter.c

# convert to obj files
PORTER_OBJS = $(subst .c,.o,$(PORTER_SRCS))

# output
PORTER_OUTPUT = porter

# include common makefile settings
include ../inc.mk

# modify flags
EXTRA_CFLAGS += -I../libs/qid/include -I../libs/utils/include -I./include
EXTRA_LDFLAGS += -L../libs/qid -lqcloud -lmcn -L../libs/utils -lqcloud_util -ljson-c -lcurl -lsqlite3 -lm -lz

build: ${PORTER_OUTPUT}

${PORTER_OUTPUT}: ${PORTER_OBJS}
	${CC} -s -o ${PORTER_OUTPUT} $(PORTER_OBJS) ${LDFLAGS} ${EXTRA_LDFLAGS}

clean:
	rm -f *.o ${PORTER_OUTPUT}

%.o: %.c
	${CC} ${CFLAGS} ${EXTRA_CFLAGS} -c -o $@ $<
