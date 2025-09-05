/*
 * Copyright (c) 2025, ktotonokto, AE-FOUNDATION
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
