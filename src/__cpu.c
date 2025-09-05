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
	FILE	*fp;
	char	line[256];
	char	mbuf[256];
	char	sbuf[256];

	assert(n);
	assert(cpu);

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
}

