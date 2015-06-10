#
#
# Makefile for the linux version of udps
#
EXEC = udps 
OBJS = udps.o 

#CFLAGS += -DDEBUG 
CFLAGS += 

#CONF_H  = $(LINUX_DIR)/include/linux/autoconf.h
 
all: $(EXEC) 
$(EXEC): $(OBJS) $(CONF_H)
	$(CC) $(CFLAGS) -o $@ $(OBJS) 

clean:
	-rm -f $(EXEC) *.o
