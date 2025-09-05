#include "../include/include.h"

/*
 * 	_ _ T E R M I N A L
 *
 * get terminal from variable
 * TERM in env; used getenv()
 */
int
__terminal(char *term, size_t n)
{
	const char *tp;

	assert(n);
	assert(term);

	if (!(tp=getenv("TERM")))
		return __ERR;
	snprintf(term,n,"%s",getenv("TERM"));

	return __OK;;
}

