#include "../include/include.h"


/*
 *	B Y T E S F M T
 *
 * convert <n> bytes to formatted
 * string presintation.
 */
inline static const char *bytesfmt(size_t n)
{
	const char *units[]={"B","KiB","MiB","GiB",
				"TiB","PiB","EiB"};
	double		c=(double)n;
	static char	fmt[32];

	for (n=0;c>=1024&&n<6;n++)
		c/=1024;

	snprintf(fmt,sizeof(fmt),
		"%.2f %s",c,units[n]);

	return fmt;
}




/*
 * 	_ _ M E M O R Y
 *
 * get RAM in string with fmt
 * <used / total>; dynamic size
 */
int
__memory(char *mem, size_t n)
{
	FILE	*fp;
	char	line[256];
	u_long	tot,free,buff,
		used,cached;

	assert(n);
	assert(mem);

	if (!(fp=fopen("/proc/meminfo","r")))
		return __ERR;
	while (fgets(line,sizeof(line),fp)) {
		if (sscanf(line,"MemTotal: %lu kB",&tot)==1)
			continue;
		if (sscanf(line,"MemFree: %lu kB", &free)==1)
			continue;
		if (sscanf(line,"Buffers: %lu kB", &buff)==1)
			continue;
		if (sscanf(line,"Cached: %lu kB",&cached)==1)
			continue;
	}
	fclose(fp);

	used=tot-free-buff-cached;

	/* to bytes */
	used*=1024UL;
	tot*=1024UL;

	snprintf(line,sizeof(line),"%s",bytesfmt(used));
	snprintf(mem,n,"%s / %s",line,bytesfmt(tot));
	return __OK;;
}
