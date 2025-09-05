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

