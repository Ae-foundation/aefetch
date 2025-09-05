#include "../include/include.h"

/*
 * 	_ _ U P T I M E
 *
 * get uptime and return this in
 * double.
 */
int
__uptime(char *uptime, size_t n)
{
	FILE	*fp;
	double	t;

	/* not sysinfo() */
	if (!(fp=fopen("/proc/uptime","r")))
		return __ERR;
	if (fscanf(fp,"%lf",&t)!=1) {
		fclose(fp);
		return __ERR;
	}
	fclose(fp);

	snprintf(uptime,n,"%.0f hours, %.1f minutes",
		t/3600,fmod(t,3600)/60.0);

	return __OK;
}

