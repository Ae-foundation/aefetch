#include "../include/include.h"

/*
 * 	_ _ O S I N F O
 *
 * get distr name (PRETTY) from
 * os-release and write this in
 * osinfo with len n.
 */
int
__osinfo(char *osinfo, size_t n)
{
	FILE	*fp;
	char	line[256];
	char	*np;

	assert(n);
	assert(osinfo);

	if (!(fp=fopen("/etc/os-release","r")))
		return __ERR;
	while (fgets(line,sizeof(line),fp)) {
		if (strncmp(line,"PRETTY_NAME=",12)!=0)
			continue;
		np=strchr(line,'"')+1;
		np[strcspn(np,"\n")-1]='\0';
		snprintf(osinfo,n,"%s",np);
		break;
	}

	fclose(fp);
	return __OK;;
}
