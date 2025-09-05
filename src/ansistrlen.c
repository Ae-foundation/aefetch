
/* master fix */
#define _XOPEN_SOURCE 700
typedef unsigned char u_char;
typedef unsigned int u_int;

#include "../include/include.h"

inline static const u_char *skip_csi(const u_char *q)
{
	while (*q&&*q>=0x30&&*q<=0x3F)
		q++;
	while (*q&&*q>=0x20&&*q<=0x2F)
		q++;
	if (*q)
		q++;
	return q;
}

static const u_char*
skip_to_st(const u_char *q, int bel_ok)
{
    while (*q) {
        if (*q==0x1B&&q[1]=='\\') {
		q+=2;
		break;
	}
        if (*q==0x9C) {
		q++;
		break;
	}
        if (bel_ok&&*q==0x07) {
		q++;
		break;
	}
        q++;
    }
    return q;
}

size_t ansistrlen(const char *s)
{
	const u_char*p=(const u_char*)s;
	size_t width=0;
	mbstate_t st;
	wchar_t wc;
	size_t n;
	u_char c;
	int w;

	memset(&st, 0, sizeof st);
	while (*p) {
		c=*p;

		if (c==0x9B) {	/* C1 CSI */
			p=skip_csi(p+1);
			continue;
		}
		if (c==0x9D) {	/* OSC */
			p=skip_to_st(p+1,1);
			continue;
		}
		if (c==0x90||c==0x98||c==0x9E||c==0x9F) {	/* DCS/PM/APC */
			p=skip_to_st(p+1,0);
			continue;
		}

		if (c==0x1B) { /* ESC ... */
			const u_char *q=p+1;
			if (*q=='[') {	/* CSI */
				p=skip_csi(q+1);
				continue;
			}
			if (*q==']') {	/* OSC */
				p=skip_to_st(q+1, 1);
				continue;
			}
			if (*q=='P'||*q=='X'||*q=='^'||*q=='_') {	/* DCS/PM/APC */
				p=skip_to_st(q+1,0);
				continue;
			}

			/* ESC Fe / ESC intermediates + final */
			q++;
			while (*q&&*q>=0x20&&*q<=0x2F)
				q++;
			if (*q)
				q++;
			p=q;
			continue;
		}

		if (c<=0x1F||c==0x7F) {
			p++;
			continue;
		}

		n=mbrtowc(&wc,(const char*)p,MB_CUR_MAX,&st);
		if (n==(size_t)-1||n==(size_t)-2) {
			memset(&st,0,sizeof st);
			p++;
			width+=1;
			continue;
		}
		p+=n;

		w=wcwidth(wc);
		if (w<0)
			w=0;
		width+=(size_t)w;
	}

	return width;
}

