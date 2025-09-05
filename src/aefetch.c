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

#include "../config.h"


/*
 *	B E G I N
 *
 * begins fetch: allocates
 * memory for its size, sets
 * the location, and returns
 * the object.
 */
inline static FETCH *BEGIN(void)
{
	FETCH *f;
	if (!(f=calloc(1,sizeof(FETCH))))
		return NULL;
	memset(f,0,sizeof(FETCH));
	setlocale(LC_ALL,"");
	return f;
}




/*
 *	C O N F I G
 *
 * applies config to FETCH
 * object
 */
inline static void CONFIG(FETCH *f)
{
	assert(f);
	f->lines=LINES;
	f->data.logo=LOGO;
	f->data.linesn=sizeof(LINES)/sizeof(LINES[0]);
	f->data.logon=sizeof(LOGO)/sizeof(LOGO[0]);
}




/*
 * 	C O M P I L E
 *
 * displays fetch along with
 * the logo and all styling;
 * main function
 */
inline static void COMPILE(FETCH *f)
{
	char	res[65535];
	char	desc[2048];
	char	val[4028];
	char	tval[2048];
	char	logo[2048];

	char	bg[512];
	char	fg[512];
	char	s_on[64];
	char	s_off[64];
	char	skip[512];

	size_t	n;
	size_t	n1;
	size_t	logon;
	size_t	off;

	assert(f);

	for (n=off=0;n!=f->data.logon;n++)
		if ((ansistrlen(f->data.logo[n]))>off)
			off=ansistrlen(f->data.logo[n]);
	off++;

	for (n=logon=0;n<f->data.linesn;n++) {
		if (f->lines[n].skip>0) {
			for (n1=0;n1<f->lines[n].skip;n1++) {
				if (logon!=f->data.logon) {
					fprintf(stdout,"%s\n",f->data.logo[logon]);
					++logon;
				}
				else
					fprintf(stdout,"\n");
			}
		}

		bzero(logo,sizeof(logo));
		bzero(s_on,sizeof(s_on));
		bzero(s_off,sizeof(s_off));
		bzero(fg,sizeof(fg));
		bzero(bg,sizeof(bg));
		bzero(skip,sizeof(bg));

		if (logon!=f->data.logon) {
			snprintf(logo,sizeof(logo),"%s",f->data.logo[logon]);
			++logon;
		}

		set_styleansi(s_on,sizeof(s_on),f->lines[n].style.style);
		unset_styleansi(s_off,sizeof(s_off),f->lines[n].style.style);
		hex2ansi(f->lines[n].style.fg,true,true,false,fg,sizeof(fg));
		hex2ansi(f->lines[n].style.bg,false,true,false,bg,sizeof(bg));

		n1=f->lines[n].space;
		if (n1>=sizeof(skip))
			n1=sizeof(skip)-1;
		memset(skip,' ',n1);
		skip[n1]='\0';

		snprintf(desc,sizeof(desc),"%s%s%s%s%s%s%s",bg,fg,s_on,
			(f->lines[n].desc)?f->lines[n].desc:"",
			s_off,ANSI_RESET,skip);

		bzero(s_on,sizeof(s_on));
		bzero(s_off,sizeof(s_off));
		bzero(fg,sizeof(fg));
		bzero(bg,sizeof(bg));
		bzero(tval,sizeof(bg));
		bzero(val,sizeof(bg));
		bzero(res,sizeof(res));
		bzero(skip,sizeof(skip));

		set_styleansi(s_on,sizeof(s_on),f->lines[n].styleval.style);
		unset_styleansi(s_off,sizeof(s_off),f->lines[n].styleval.style);
		hex2ansi(f->lines[n].styleval.fg,true,true,false,fg,sizeof(fg));
		hex2ansi(f->lines[n].styleval.bg,false,true,false,bg,sizeof(bg));

		if (f->lines[n].value)
			f->lines[n].value(tval,sizeof(tval));
		else {
			if (f->lines[n].resvalue)
				snprintf(tval,sizeof(tval),"%s",f->lines[n].resvalue);
		}

		snprintf(val,sizeof(val),"%s%s%s%s%s%s",bg,fg,s_on,
			tval,s_off,ANSI_RESET);

		if (off>ansistrlen(logo))
			n1=off-ansistrlen(logo)-1;
		n1+=f->lines[n].spacel;
		if (n1>=sizeof(skip))
			n1=sizeof(skip)-1;
		memset(skip,' ',n1);
		skip[n1]='\0';

		snprintf(res,sizeof(res),"%s%s%s%s",logo,skip,desc,val);
		fprintf(stdout,"%s\n",res);
	}
	while (logon!=f->data.logon) {
		fprintf(stdout,"%s\n",f->data.logo[logon]);
		++logon;
	}
}




/*
 * 	E N D
 *
 * clears the memory from
 * under FETCH, returns 0
 */
inline static int END(FETCH* f)
{
	assert(f);
	free(f);
	return 0;
}




/*
 * 	M A I N ()
 */
int main(int c, char **av)
{
	FETCH *f=BEGIN();
	CONFIG(f);
	COMPILE(f);
	return END(f);
}
