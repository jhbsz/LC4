#
# Makefile for the webs package directory
#
# Copyright (c) Go Ahead, 1995-1999
#

SUBDIRS=LINUX cgi-src

all:
	for i in $(SUBDIRS) ; do make -C $$i $@ || exit $?; done

clean:
	for i in $(SUBDIRS) ; do make -C $$i clean ; done