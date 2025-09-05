#ifndef __CONFIG_H
#define __CONFIG_H

#include "include/include.h"

static const char *LOGO[] = {
	"\x1b[1;33;44m               \x1b[1;37;44m__\x1b[1;35;44m.....\x1b[1;37;44m__\x1b[1;33;44m      \x1b[0m",
	"\x1b[1;33;44m           \x1b[1;36;44m.-\x1b[1;35;44m''\x1b[1;33;44m         \x1b[1;35;44m'.\x1b[1;33;44m    \x1b[0m",
	"\x1b[1;33;44m          \x1b[1;32;44m/\x1b[1;33;44m     \x1b[1;36;44m.-\x1b[1;35;44m''\x1b[1;37;44m\"\x1b[1;35;44m'\x1b[1;36;44m-.\x1b[1;33;44m  \x1b[1;37;44m`\x1b[1;35;44m.\x1b[1;33;44m  \x1b[0m",
	"\x1b[1;33;44m    \x1b[1;37;44m__\x1b[1;33;44m   \x1b[1;32;44m/\x1b[1;33;44m     \x1b[1;32;44m/\x1b[1;37;44m________\x1b[1;32;44m\\\x1b[1;33;44m   \x1b[1;32;44m\\ \x1b[0m",
	"\x1b[1;33;44m \x1b[1;35;44m:\x1b[1;31;44m--\x1b[1;35;44m'.\x1b[1;33;44m \x1b[1;32;44m|\x1b[1;33;44m                  \x1b[1;32;44m   |\x1b[0m",
	"\x1b[1;30;46m/ |   \\ |\\    .-------------' \x1b[0m",
	"\x1b[1;33;44m`\"\x1b[1;33;44m \x1b[1;37;44m__\x1b[1;33;44m \x1b[1;32;44m|\x1b[1;33;44m \x1b[1;32;44m|\x1b[1;33;44m \\\\    \x1b[1;31;44m'-\x1b[1;37;44m____\x1b[1;35;44m...\x1b[1;31;44m---\x1b[1;35;44m. \x1b[0m",
	"\x1b[1;33;44m .\x1b[1;35;44m'.''\x1b[1;32;44m|\x1b[1;33;44m \x1b[1;32;44m|\x1b[1;33;44m  \x1b[1;37;44m`\x1b[1;35;44m.\x1b[1;33;44m             \x1b[1;35;44m.'\x1b[1;33;44m  \x1b[0m",
	"\x1b[1;32;44m/\x1b[1;33;44m \x1b[1;32;44m/\x1b[1;33;44m   \x1b[1;32;44m|\x1b[1;33;44m \x1b[1;32;44m|_\x1b[1;33;44m   \x1b[1;37;44m`\x1b[1;35;44m''\x1b[1;31;44m-\x1b[1;35;44m......\x1b[1;33;44m \x1b[1;31;44m-'\x1b[1;33;44m    \x1b[0m",
	"\x1b[1;33;44m\\ \x1b[1;32;44m\\\x1b[1;35;44m._,\x1b[1;32;44m\\\x1b[1;33;44m \x1b[1;35;44m'/\x1b[1;33;44m                    \x1b[0m",
	"\x1b[1;33;44m \x1b[1;35;44m`\"\x1b[1;33;44m--\x1b[1;35;44m'\x1b[1;33;44m                        \x1b[0m"
};

static FETCHLINE LINES[]=
{
	{"OS:",
		1,
		{"#FFFF55","#0000AA",ANSI_S_BOLD},
		__osinfo,
		"Linux",
		{"#55FFFF","#000000",ANSI_S_UNDERLINE|ANSI_S_ITALIC},
		4,2
	},

	{"Hostname:",
		1,
		{"#FFFF55","#0000AA",ANSI_S_BOLD},
		__hostname,
		"",
		{"#55FF55","#000000",ANSI_S_UNDERLINE|ANSI_S_ITALIC},
		4,0
	},

	{"Kernel:",
		1,
		{"#FFFF55","#0000AA",ANSI_S_BOLD},
		__kernel,
		"",
		{"#AAAAAA","#000000",ANSI_S_UNDERLINE|ANSI_S_ITALIC},
		4,0
	},

	{"Uptime:",
		1,
		{"#FFFF55","#0000AA",ANSI_S_BOLD},
		__uptime,
		"",
		{"#55FF55","#000000",ANSI_S_UNDERLINE|ANSI_S_ITALIC},
		4,0
	},

	{"Terminal:",
		1,
		{"#FFFF55","#0000AA",ANSI_S_BOLD},
		__terminal,
		"",
		{"#FF55FF","#000000",ANSI_S_UNDERLINE|ANSI_S_ITALIC},
		4,0
	},

	{"CPU:",
		1,
		{"#FFFF55","#0000AA",ANSI_S_BOLD},
		__cpu,
		"",
		{"#FF5555","#000000",ANSI_S_UNDERLINE|ANSI_S_ITALIC},
		4,0
	},

	{"Memory:",
		1,
		{"#FFFF55","#0000AA",ANSI_S_BOLD},
		__memory,
		"",
		{"#55FF55","#000000",ANSI_S_UNDERLINE|ANSI_S_ITALIC},
		4,0
	},
};

#endif
