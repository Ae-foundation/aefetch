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
 * 	_ _ C P U 
 *
 * get CPU INFO in string and
 * write to buffer.
 */
int
__cpu(char *cpu, size_t n)
{
	assert(n);
	assert(cpu);

#ifdef __FreeBSD__
	int	mib[2];
	char	model[256];
	int	cores;
	size_t	siz;

	siz=sizeof(model);
	mib[0]=CTL_HW;
	mib[1]=HW_MODEL;

	if (sysctl(mib,2,model,&siz,NULL,0)==-1)
		return __ERR;
	siz=strlen(model);
	while (siz>0&&(model[siz-1]==' '||model[siz-1]=='\t'||model[siz-1]=='\n'))
		model[--siz]='\0';

	siz=sizeof(cores);
	mib[0]=CTL_HW;
	mib[1]=HW_NCPU;

	if (sysctl(mib,2,&cores,&siz,NULL,0)==-1)
		return __ERR;

	snprintf(cpu,n,"%s (%d)",model,cores);
	return __OK;
#else
	FILE	*fp;
	char	line[256];
	char	mbuf[256];
	char	sbuf[256];

	*mbuf=*sbuf='\0';
	if (!(fp=fopen("/proc/cpuinfo","r")))
		return __ERR;
	while (fgets(line,sizeof(line),fp)) {
		if (!strncmp(line,"model name\t: ",strlen("model name\t: "))) {
			snprintf(mbuf,sizeof(mbuf),"%s",line+strlen("model name\t: "));
			mbuf[strlen(mbuf)-1]='\0';
		}
		if (!strncmp(line,"siblings\t: ",strlen("siblings\t: "))) {
			snprintf(sbuf,sizeof(sbuf),"%s",line+strlen("siblings\t: "));
			sbuf[strlen(sbuf)-1]='\0';
		}
		if (*mbuf&&*sbuf)
			break;
	}

	fclose(fp);
	snprintf(cpu,n,"%s (%s)",mbuf,sbuf);

	return __OK;
#endif
}

