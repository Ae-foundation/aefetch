#include "../include/include.h"

/*
 * 	_ _ K E R N E L
 *
 * get kernel release from
 * uname; used uname()
 */
int
__kernel(char *kernel, size_t n)
{
	struct utsname un;

	assert(n);
	assert(kernel);

	if (uname(&un)!=0)
		return __ERR;
	snprintf(kernel,n,"%s",un.release);
	return __OK;;
}

