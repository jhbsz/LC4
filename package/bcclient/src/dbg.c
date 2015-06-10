/*******************************************************************************************************************************
 *
 *	Copyright (c) 2007  QNAP Systems, Inc.  All Rights Reserved.
 *
 *	FILE:
 *		dbg.c
 *
 *	Abstract: 
 *		Common helper debug funcion implementation.
 *
 *	HISTORY:
 *		03/26/07	meiji chang re-create
 *
 ********************************************************************************************************************************/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdarg.h>
#include <sys/syslog.h>
#include <sys/stat.h>


	static void
	debug_msg (const char *fmt, va_list ap)
	{
#ifdef DEBUG
		vfprintf (stderr, fmt, ap);
#endif
	}

	void
	DBG (const char *fmt, ...)
	{
#ifdef DEBUG
		va_list ap;
		va_start (ap, fmt);
		debug_msg (fmt, ap);
		va_end (ap);
#endif
	}
	
