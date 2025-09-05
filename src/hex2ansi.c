#include "../include/include.h"

static inline bool
parse_hex(const char *s, int *r, int *g, int *b)
{
	unsigned	v[3]={0,0,0};
	size_t		n;
	int		i;
	int		hi;
	int		lo;
	int		c;
	u_int		nib;

	if(!s)
		return false;
	if(*s=='#')
		s++;

	n=strlen(s);
	if(n==6) {
		for(i=0;i<3;++i){
			hi=s[i*2],lo=s[i*2+1];
			if(!isxdigit(hi)||!isxdigit(lo))
				return false;
			v[i]=(unsigned)(strtol((char[]){(char)hi,0},NULL,16)<<4)|
				(unsigned)strtol((char[]){(char)lo,0},NULL,16);
        	}
	}
	else if(n==3) {
		for(i=0;i<3;++i) {
			c=s[i];
			if(!isxdigit(c))
				return false;
			nib=(u_int)strtol((char[]){(char)c,0},NULL,16);
			v[i]=(nib<<4)|nib;
		}
	}
	else
		return false;

	*r=(int)v[0];
	*g=(int)v[1];
	*b=(int)v[2];

	return true;
}

static inline double
srgb_to_linear(double c)
{
	c/=255.0;
	return (c<=0.04045)?(c/12.92):
		pow((c+0.055)/1.055,2.4);
}


inline static void
rgb_to_xyz(double R, double G, double B,
		double *X, double *Y, double *Z)
{
    double r=srgb_to_linear(R);
    double g=srgb_to_linear(G);
    double b=srgb_to_linear(B);

    *X=0.4124564*r+0.3575761*g+0.1804375*b;
    *Y=0.2126729*r+0.7151522*g+0.0721750*b;
    *Z=0.0193339*r+0.1191920*g+0.9503041*b;
}

static inline double
f_lab(double t)
{
	const double d=6.0/29.0;
	const double d3=d*d*d;

	return (t>d3)?cbrt(t):(t/(3*d*d)+4.0/29.0);
}

inline static void
rgb_to_lab(int R, int G, int B, double *L, double *a, double *b)
{
    const double Xn=0.95047,Yn=1.00000,Zn=1.08883;
    double fx,fy,fz;
    double X,Y,Z;

    rgb_to_xyz(R,G,B,&X,&Y,&Z);

    fx=f_lab(X/Xn);
    fy=f_lab(Y/Yn);
    fz=f_lab(Z/Zn);

    *L=116.0*fy-16.0;
    *a=500.0*(fx-fy);
    *b=200.0*(fy-fz);
}

static inline double
deltaE2_Lab(int r1,int g1,int b1,int r2,int g2,int b2)
{
	double L1,a1,b1c,L2,a2,b2c;

	rgb_to_lab(r1,g1,b1,&L1,&a1,&b1c);
	rgb_to_lab(r2,g2,b2,&L2,&a2,&b2c);
	double dL=L1-L2, da=a1-a2, db=b1c-b2c;
	return dL*dL+da*da+db*db;	/* CIE76^2 */
}

static inline int
level6(int i)
{
	static const int L[6]={0,95,135,175,215,255};
	return L[i<0?0:i>5?5:i];
}

inline static void
index_to_rgb_256(int idx, int *r, int *g, int *b)
{
        static const int base16[16][3]={
		{0,0,0},{205,0,0},{0,205,0},{205,205,0},
		{0,0,238},{205,0,205},{0,205,205},{229,229,229},
		{127,127,127},{255,0,0},{0,255,0},{255,255,0},
		{92,92,255},{255,0,255},{0,255,255},{255,255,255}
        };
	int i;
	int k;
	int r6;
	int g6;
	int b6;
	int v;

	if (idx>=16&&idx<=231) {
		k=idx-16;
		r6=k/36;
		g6=(k%36)/6;
		b6=k%6;
		*r=level6(r6);
		*g=level6(g6);
		*b=level6(b6);
	}
	else if (idx>=232&&idx<=255) {
		v=8+10*(idx-232);
		*r=*g=*b=v;
	}
	else {
		i=(idx<0?0:idx>15?15:idx);
		*r=base16[i][0];
		*g=base16[i][1];
		*b=base16[i][2];
	}
}

inline static int
nearest_xterm256_index(int R, int G, int B, bool allow_base16)
{
    double best=1e100;
    int best_idx=16;
    int start=allow_base16?0:16;
    int idx;
    int rr,gg,bb;
    double d2;

    for (idx=start;idx<=255;++idx) {
        index_to_rgb_256(idx,&rr,&gg,&bb);
        d2=deltaE2_Lab(R,G,B,rr,gg,bb);
        if (d2<best) {
		best=d2;
		best_idx=idx;
	}
    }

    return best_idx;
}

inline static bool
terminal_supports_truecolor(void)
{
	const char *term;
	const char *ct;

	ct=getenv("COLORTERM");
	if (ct&&(strstr(ct,"truecolor")||strstr(ct,"24bit")))
		return true;
	term=getenv("TERM");
	if (term&&strstr(term,"-truecolor"))
		return true;

	return false;
}

size_t hex2ansi(const char *hex, bool foreground, bool prefer_truecolor,
		bool allow_base16, char *out, size_t outsz)
{
	int R,G,B;
	int idx;

	if (!out||outsz<8)
		return 0;
	if (!parse_hex(hex,&R,&G,&B))
		return 0;

	/*
	 * true colors
	 * \x1b[38;2;R;G;Bm	or	\x1b[48;2;R;G;Bm
	 */
	if (prefer_truecolor&&terminal_supports_truecolor()) {
		return (size_t)snprintf(out,outsz,"\x1b[%d;2;%d;%d;%dm",
			foreground?38:48,R,G,B);
	}

	/* xterm colors */
	idx=nearest_xterm256_index(R,G,B,allow_base16);
	return (size_t)snprintf(out,outsz,"\x1b[%d;5;%dm",
		foreground?38:48,idx);
}



static int append_code(char *out, size_t outsz, int n, const char *code, int *first)
{
	int rc;
	if (n<0||(size_t)n>=outsz)
		return n;
	rc=snprintf(out+n,outsz-(size_t)n,*first?"%s":";%s",code);
	if (rc<0)
		return n;
	*first=0;
	return n+rc;
}

size_t set_styleansi(char *out, size_t outsz, u_int flags)
{
	int first;
	int n;

	if (!out||outsz<4||flags==0){
		if(out&&outsz)
			out[0]='\0';
		return 0; 
	}

	n=snprintf(out,outsz,"\x1b[");
	first=1;

	if (flags&ANSI_S_BOLD)
		n=append_code(out,outsz,n,"1",&first);
	if (flags&ANSI_S_DIM)
		n=append_code(out,outsz,n,"2",&first);
	if (flags&ANSI_S_ITALIC)
		n=append_code(out,outsz,n,"3",&first);
	if (flags&ANSI_S_UNDERLINE)
		n=append_code(out,outsz,n,"4",&first);
	if (flags&ANSI_S_INVERSE)
		n=append_code(out,outsz,n,"7",&first);
	if (flags&ANSI_S_STRIKE)
		n=append_code(out,outsz,n,"9",&first);
	if (flags&ANSI_S_OVERLINE)
		n=append_code(out,outsz,n,"53",&first);

	if (first) {
		out[0]='\0';
		return 0;
	}

	if (n>=0&&(size_t)n<outsz-1) {
		out[n++]='m';
		out[n]='\0';
	}
	else if (outsz)
		out[outsz-1]='\0';


	return (size_t)n;
}

size_t unset_styleansi(char *out, size_t outsz, u_int flags)
{
	int first;
	int n;

	if (!out||outsz<4||flags==0){
		if(out&&outsz)
			out[0]='\0';
		return 0; 
	}

	n=snprintf(out,outsz,"\x1b[");
	first=1;

	if (flags&(ANSI_S_BOLD|ANSI_S_DIM))
		n=append_code(out,outsz,n,"22",&first);
	if (flags&ANSI_S_ITALIC)
		n=append_code(out,outsz,n,"23",&first);
	if (flags&ANSI_S_UNDERLINE)
		n=append_code(out,outsz,n,"24",&first);
	if (flags&ANSI_S_INVERSE)
		n=append_code(out,outsz,n,"27",&first);
	if (flags&ANSI_S_STRIKE)
		n=append_code(out,outsz,n,"29",&first);
	if (flags&ANSI_S_OVERLINE)
		n=append_code(out,outsz,n,"55",&first);

	if (first) {
		out[0]='\0';
		return 0;
	}

	if (n>=0&&(size_t)n<outsz-1) {
		out[n++]='m';
		out[n]='\0';
	}
	else if (outsz)
		out[outsz-1]='\0';

	return (size_t)n;
}
