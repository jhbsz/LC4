/*
	
	Tomato Firmware
	Copyright (C) 2006 Jonathan Zarate
	
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdarg.h>

#include <bcmnvram.h>
#include "shutils.h"
#include "shared.h"

int find_word(const char *buffer, const char *word)
{
	const char *p, *q;
	int n;
	
	n = strlen(word);
	p = buffer;
	while ((p = strstr(p, word)) != NULL) {
		if ((p == buffer) || (*(p - 1) == ' ') || (*(p - 1) == ',')) {
			q = p + n;
			if ((*q == ' ') || (*q == ',') || (*q == 0)) return 1;
		}
		++p;
	}	
	return 0;
}

/*
static void add_word(char *buffer, const char *word, int max)
{
	if ((*buffer != 0) && (buffer[strlen(buffer) - 1] != ' '))
		strlcat(buffer, " ", max);
	strlcat(buffer, word, max);
}
*/
