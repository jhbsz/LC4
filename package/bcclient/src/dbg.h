/*******************************************************************************************************************************
 *
 *	Copyright (c) 2007  QNAP Systems, Inc.  All Rights Reserved.
 *
 *	FILE:
 *		dbg.h
 *
 *	Abstract: 
 *		Common helper debug funcion.
 *
 *	HISTORY:
 *		03/26/07	meiji chang re-create
 *
 ********************************************************************************************************************************/

#ifndef	__BUG_INCLUDE_H__
#define	__BUG_INCLUDE_H__
 
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdarg.h>
#include <sys/syslog.h>
#include <sys/stat.h>

void debug_msg (const char *fmt, va_list ap);
void DBG (const char *fmt, ...);

#ifdef DEBUG
	
	#define __dbg(format, arg...)            \
	do {\
		FILE *fp; \
		fprintf(stdout, format, ## arg); \
		fp = fopen("/debug.log","a+"); \
		fprintf(fp, format, ## arg); \
		fclose(fp); \
	} while (0);
#else
	#define __dbg(format, arg...)	()	
#endif

#endif		// __BUG_INCLUDE_H__
