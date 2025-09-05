#include "../include/include.h"

/*
 * 	_ _ H O S T N A M E
 *
 * get hostname and write this in
 * name with len n.
 */
int
__hostname(char *name, size_t n)
{
	assert(n);
	assert(name);

	if (gethostname(name,n)==0)
		return __OK;
	snprintf(name,n,"???");

	return __ERR;
}
