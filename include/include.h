#ifndef __INCLUDE_H
#define __INCLUDE_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <locale.h>
#include <wchar.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include <sys/utsname.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

#ifdef __FreeBSD__
#include <sys/sysctl.h>
#endif

#define ANSI_RESET "\x1b[0m"

typedef enum {
    ANSI_S_BOLD      =1u<<0,
    ANSI_S_DIM       =1u<<1,
    ANSI_S_ITALIC    =1u<<2,
    ANSI_S_UNDERLINE =1u<<3,
    ANSI_S_STRIKE    =1u<<4,
    ANSI_S_INVERSE   =1u<<5,
    ANSI_S_OVERLINE  =1u<<6
} ansi_style_t;

typedef struct __FETCHSTYLE
{
	const char	*fg;
	const char	*bg;
	u_int		style;
} FETCHSTYLE;

typedef struct __FETCHLINE
{
	const char	*desc;				/* description */
	int		space;				/* space after desc */
	FETCHSTYLE	style;				/* style for desc */
	int		(*value)(char *,size_t);	/* function for get value */
	const char	*resvalue;			/* reserved value */
	FETCHSTYLE	styleval;			/* style for value */
	int		spacel;				/* space from logo */
	int		skip;				/* skip n lines before this */
} FETCHLINE;

typedef struct __FETCHDATA
{
	size_t		linesn;
	const char	**logo;
	size_t		logon;
} FETCHDATA;

typedef struct __FETCH
{
	FETCHDATA data;
	FETCHLINE *lines;
} FETCH;

size_t hex2ansi(const char *hex, bool foreground, bool prefer_truecolor,
		bool allow_base16, char *out, size_t outsz);
size_t set_styleansi(char *out, size_t outsz, u_int flags);
size_t unset_styleansi(char *out, size_t outsz, u_int flags);
size_t ansistrlen(const char *s);

#define __OK (int)0
#define __ERR (int)-1

int __hostname(char *name, size_t n);
int __uptime(char *uptime, size_t n);
int __osinfo(char *osinfo, size_t n);
int __kernel(char *kernel, size_t n);
int __terminal(char *term, size_t n);
int __memory(char *mem, size_t n);
int __cpu(char *cpu, size_t n);

#endif
