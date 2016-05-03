/* vi: set sw=4 ts=4: */
/*
 * Splash - Simple PLeasant Admin SHell
 * by Instant802 Networks
 *
 * Based on Busybox 1.00-pre7 shells/msh.c:
 */
/*
 * Minix shell port for busybox
 *
 * This version of the Minix shell was adapted for use in busybox
 * by Erik Andersen <andersen@codepoet.org>
 *
 * - backtick expansion did not work properly
 *   Jonas Holmberg <jonas.holmberg@axis.com>
 *   Robert Schwebel <r.schwebel@pengutronix.de>
 *   Erik Andersen <andersen@codepoet.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 * Original copyright notice is retained at the end of this file.
 */

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <setjmp.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <sys/ioctl.h>

#include "busybox.h"


#include "ap_interface.h"  

//#include "cmdedit.h"



static int cmd_arg=0;  //wangjianru

static int     splash_cmdedit_read_input(char* promptStr, char* command);
//static char *home_pwd_buf = "";
#define cmdedit_path_lookup getenv("PATH")

/* -------- sh.h -------- */
/*
 * shell
 */

#define	LINELIM	2100
#define	NPUSH	8	/* limit to input nesting */

#undef NOFILE
#define	NOFILE	20	/* Number of open files */
#define	NUFILE	10	/* Number of user-accessible files */
#define	FDBASE	10	/* First file usable by Shell */

/*
 * values returned by wait
 */
#define	WAITSIG(s) ((s)&0177)
#define	WAITVAL(s) (((s)>>8)&0377)
#define	WAITCORE(s) (((s)&0200)!=0)

/*
 * library and system defintions
 */
typedef void xint;	/* base type of jmp_buf, for not broken compilers */

/*
 * shell components
 */

#define	QUOTE	0200

#define	NOBLOCK	((struct op *)NULL)
#define	NOWORD	((char *)NULL)
#define	NOWORDS	((char **)NULL)
#define	NOPIPE	((int *)NULL)


/*
 * Description of a command or an operation on commands.
 * Might eventually use a union.
 */
struct op {
	int	type;	/* operation type, see below */
	char	**words;	/* arguments to a command */
	struct	ioword	**ioact;	/* IO actions (eg, < > >>) */
	struct op *left;
	struct op *right;
	char	*str;	/* identifier for case and for */
};

#define	TCOM	1	/* command */
#define	TPAREN	2	/* (c-list) */
#define	TPIPE	3	/* a | b */
#define	TLIST	4	/* a [&;] b */
#define	TOR	5	/* || */
#define	TAND	6	/* && */
#define	TFOR	7
#define	TDO	8
#define	TCASE	9
#define	TIF	10
#define	TWHILE	11
#define	TUNTIL	12
#define	TELIF	13
#define	TPAT	14	/* pattern in case */
#define	TBRACE	15	/* {c-list} */
#define	TASYNC	16	/* c & */

/*
 * actions determining the environment of a process
 */
#define	BIT(i)	(1<<(i))
#define	FEXEC	BIT(0)	/* execute without forking */

/*
 * flags to control evaluation of words
 */
#define	DOSUB	1	/* interpret $, `, and quotes */
#define	DOBLANK	2	/* perform blank interpretation */
#define	DOGLOB	4	/* interpret [?* */
#define	DOKEY	8	/* move words with `=' to 2nd arg. list */
#define	DOTRIM	16	/* trim resulting string */

#define	DOALL	(DOSUB|DOBLANK|DOGLOB|DOKEY|DOTRIM)

static	char	**dolv;
static	int	dolc;
static	int	exstat;
static  char	gflg;
static  int	interactive;	/* Is this an interactive shell */
static  int	execflg;
static  int	multiline;	/* \n changed to ; */
static  struct	op	*outtree;	/* result from parser */

static	xint	*failpt;
static	xint	*errpt;
static	struct brkcon	*brklist;
static	int	isbreak;
static int newfile(char *s);
static char *findeq(char *cp);
static char *cclass(char *p, int sub);
static void initarea(void);
extern int splash_main(int argc, char **argv);


struct	brkcon {
	jmp_buf	brkpt;
	struct	brkcon	*nextlev;
} ;

/*
 * redirection
 */
struct ioword {
	short	io_unit;	/* unit affected */
	short	io_flag;	/* action (below) */
	char	*io_name;	/* file name */
};
#define	IOREAD	1	/* < */
#define	IOHERE	2	/* << (here file) */
#define	IOWRITE	4	/* > */
#define	IOCAT	8	/* >> */
#define	IOXHERE	16	/* ${}, ` in << */
#define	IODUP	32	/* >&digit */
#define	IOCLOSE	64	/* >&- */

#define	IODEFAULT (-1)	/* token for default IO unit */

static	struct	wdblock	*wdlist;
static	struct	wdblock	*iolist;

/*
 * parsing & execution environment
 */
static struct	env {
	char	*linep;
	struct	io	*iobase;
	struct	io	*iop;
	xint	*errpt;
	int	iofd;
	struct	env	*oenv;
} e;

/*
 * flags:
 * -e: quit on error
 * -k: look for name=value everywhere on command line
 * -n: no execution
 * -t: exit after reading and executing one command
 * -v: echo as read
 * -x: trace
 * -u: unset variables net diagnostic
 */
static	char	*flag;

static	char	*null;	/* null value for variable */
static	int	intr;	/* interrupt pending */

static	char	*trap[_NSIG+1];
static	char	ourtrap[_NSIG+1];
static	int	trapset;	/* trap pending */

static	int	heedint;	/* heed interrupt signals */

static	int	yynerrs;	/* yacc */

static	char	line[LINELIM];
static	char	*elinep;

/*
 * other functions
 */
static int(*inbuilt(char *s))(struct op *);


static char *rexecve (char *c , char **v, char **envp );
static char *space (int n );
static char *strsave (char *s, int a );
static char *evalstr (char *cp, int f );
static char *putn (int n );
//char *itoa (int n );//wangjianru
static char *unquote (char *as );
static struct var *lookup (char *n );
static int rlookup (char *n );
static struct wdblock *glob (char *cp, struct wdblock *wb );
static int my_getc( int ec);
static int subgetc (int ec, int quoted );
static char **makenv (void);
static char **eval (char **ap, int f );
static int setstatus (int s );
static int waitfor (int lastpid, int canintr );

static void onintr (int s ); /* SIGINT handler */

static int newenv (int f );
static void quitenv (void);
static void err (char *s );
static int anys (char *s1, char *s2 );
static int any (int c, char *s );
static void next (int f );
static void setdash (void);
static void onecommand (void);
static void runtrap (int i );
static int gmatch (char *s, char *p );

/*
 * error handling
 */
static void leave (void); /* abort shell (or fail in subshell) */
static void fail (void);	 /* fail but return to process next command */
static void warn (char *s );
static void sig (int i );	 /* default signal handler */



/* -------- area stuff -------- */

#define	REGSIZE		sizeof(struct region)
#define GROWBY		256
//#define	SHRINKBY   64
#undef	SHRINKBY
#define FREE 32767
#define BUSY 0
#define	ALIGN (sizeof(int)-1)


struct region {
	struct	region *next;
	int	area;
};



/* -------- grammar stuff -------- */
typedef union {
	char	*cp;
	char	**wp;
	int	i;
	struct	op *o;
} YYSTYPE;
#define	WORD	256
#define	LOGAND	257
#define	LOGOR	258
#define	BREAK	259
#define	IF	260
#define	THEN	261
#define	ELSE	262
#define	ELIF	263
#define	FI	264
#define	CASE	265
#define	ESAC	266
#define	FOR	267
#define	WHILE	268
#define	UNTIL	269
#define	DO	270
#define	DONE	271
#define	IN	272
#define	YYERRCODE 300

/* flags to yylex */
#define	CONTIN	01	/* skip new lines to complete command */

#define	SYNTAXERR	zzerr()
static struct op *pipeline(int cf );
static struct op *andor(void);
static struct op *c_list(void);
static int synio(int cf );
static void musthave (int c, int cf );
static struct op *simple(void);
static struct op *nested(int type, int mark );
static struct op *command(int cf );
static struct op *dogroup(int onlydone );
static struct op *thenpart(void);
static struct op *elsepart(void);
static struct op *caselist(void);
static struct op *casepart(void);
static char **pattern(void);
static char **wordlist(void);
static struct op *list(struct op *t1, struct op *t2 );
static struct op *block(int type, struct op *t1, struct op *t2, char **wp );
static struct op *newtp(void);
static struct op *namelist(struct op *t );
static char **copyw(void);
static void word(char *cp );
static struct ioword **copyio(void);
static struct ioword *io (int u, int f, char *cp );
static void zzerr(void);
static void yyerror(char *s );
static int yylex(int cf );
static int collect(int c, int c1 );
static int dual(int c );
static void diag(int ec );
static char *tree(unsigned size );

/* -------- var.h -------- */

struct	var {
	char	*value;
	char	*name;
	struct	var	*next;
	char	status;
};
#define	COPYV	1	/* flag to setval, suggesting copy */
#define	RONLY	01	/* variable is read-only */
#define	EXPORT	02	/* variable is to be exported */
#define	GETCELL	04	/* name & value space was got with getcell */

static	struct	var	*vlist;		/* dictionary */

static	struct	var	*homedir;	/* home directory */
static	struct	var	*splash_path;	/* search path for commands
					   that obey the splash contract */
static	struct	var	*path;		/* search path for commands,
					   including those that obey the
				           splash contract. */
static	struct	var	*shell;		/* shell to interpret command files */
static	struct	var	*ifs;		/* field separators */

static int yyparse (void);
static struct var *lookup (char *n );
static void setval (struct var *vp, char *val );
static void nameval (struct var *vp, char *val, char *name );
static void export (struct var *vp );
static void ronly (struct var *vp );
static int isassign (char *s );
static int checkname (char *cp );
static int assign (char *s, int cf );
static void putvlist (int f, int out );
static int eqname (char *n1, char *n2 );

static int execute (struct op *t, int *pin, int *pout, int act );

/* -------- io.h -------- */
/* io buffer */
struct iobuf {
  unsigned id;				/* buffer id */
  char buf[512];			/* buffer */
  char *bufp;				/* pointer into buffer */
  char *ebufp;				/* pointer to end of buffer */
};

/* possible arguments to an IO function */
struct ioarg {
	char	*aword;
	char	**awordlist;
	int	afile;		/* file descriptor */
	unsigned afid;		/* buffer id */
	long	afpos;		/* file position */
	struct iobuf *afbuf;	/* buffer for this file */
};
//static struct ioarg ioargstack[NPUSH];
#define AFID_NOBUF	(~0)
#define AFID_ID		0

/* an input generator's state */
struct	io {
	int	(*iofn)(struct  ioarg *, struct io *);
	struct	ioarg	*argp;
	int	peekc;
	char	prev;		/* previous character read by readc() */
	char	nlcount;	/* for `'s */
	char	xchar;		/* for `'s */
	char	task;		/* reason for pushed IO */
};
//static	struct	io	iostack[NPUSH];
#define	XOTHER	0	/* none of the below */
#define	XDOLL	1	/* expanding ${} */
#define	XGRAVE	2	/* expanding `'s */
#define	XIO	3	/* file IO */

/* in substitution */
#define	INSUB()	(e.iop->task == XGRAVE || e.iop->task == XDOLL)

/*
 * input generators for IO structure
 */
static int nlchar (struct ioarg *ap );
static int strchar (struct ioarg *ap );
static int qstrchar (struct ioarg *ap );
static int filechar (struct ioarg *ap );
static int herechar (struct ioarg *ap );
static int gravechar (struct ioarg *ap, struct io *iop );
static int qgravechar (struct ioarg *ap, struct io *iop );
static int dolchar (struct ioarg *ap );
static int wdchar (struct ioarg *ap );
static void scraphere (void);
static void freehere (int area );
static void gethere (void);
static void markhere (char *s, struct ioword *iop );
static int herein (char *hname, int xdoll );
static int run (struct ioarg *argp, int (*f)(struct ioarg *));

/*
 * IO functions
 */
static int eofc (void);
static int readc (void);
static void unget (int c );
static void ioecho (int c );
static void prs (char *s );
static void prn (unsigned u );
static void closef (int i );
static void closeall (void);

/*
 * IO control
 */
static void pushio (struct ioarg *argp, int (*f)(struct ioarg *));
static int remap (int fd );
static int openpipe (int *pv );
static void closepipe (int *pv );
static struct io *setbase (struct io *ip );

static	struct	ioarg	temparg;	/* temporary for PUSHIO */
#define	PUSHIO(what,arg,gen) ((temparg.what = (arg)),pushio(&temparg,(gen)))
#define	RUN(what,arg,gen) ((temparg.what = (arg)), run(&temparg,(gen)))

/* -------- word.h -------- */

#define	NSTART	16	/* default number of words to allow for initially */

struct	wdblock {
	short	w_bsize;
	short	w_nword;
	/* bounds are arbitrary */
	char	*w_words[1];
};

static struct wdblock *addword (char *wd, struct wdblock *wb );
static struct wdblock *newword (int nw );
static char **getwords (struct wdblock *wb );

/* -------- area.h -------- */

/*
 * storage allocation
 */
static char *getcell (unsigned nbytes );
static void garbage (void);
static void setarea (char *cp, int a );
static int getarea (char *cp );
static void freearea (int a );
static void freecell (char *cp );
static	int	areanum;	/* current allocation area */

#define	NEW(type) (type *)getcell(sizeof(type))
#define	DELETE(obj)	freecell((char *)obj)


/* -------- misc stuff -------- */

static int forkexec (struct op *t, int *pin, int *pout, int act, char **wp, int *pforked );
static int iosetup (struct ioword *iop, int pipein, int pipeout );
static void echo(char **wp );
static struct op **find1case (struct op *t, char *w );
static struct op *findcase (struct op *t, char *w );
static void brkset(struct brkcon *bc );
static int dolabel(struct op *t );
static int dohelp(struct op *t );
static int dowangjr(struct op *t );//wangjianru add
static int donetwork(struct op *t );//wangjianru add
static int dowireless(struct op *t );//wangjianru add
static int dosystem(struct op *t );//wangjianru add
static int dostp(struct op *t );//wangjianru add
//static int dovlan(struct op *t );//wangjianru add
static int dosnmp(struct op *t );//wangjianru add
static int dosyslog(struct op *t );//wangjianru add
static int dochdir(struct op *t );
static int doshift(struct op *t );
static int dologin(struct op *t );
static int doumask(struct op *t );
static int doexec(struct op *t );
static int dodot(struct op *t );
static int dowait(struct op *t );
static int doread(struct op *t );
static int doeval(struct op *t );
static int dotrap(struct op *t );
static int getsig(char *s );
static void setsig (int n, sighandler_t f);
static int getn(char *as );
static int dobreak(struct op *t );
static int docontinue(struct op *t );
static int brkcontin (char *cp, int val );
static int doexit(struct op *t );
static int doexport(struct op *t );
static int doreadonly(struct op *t );
static void rdexp (char **wp, void (*f)(struct var *), int key);
static void badid(char *s );
//static int doset(struct op *t );
//static void varput (char *s, int out );
static int dotimes(struct op *t );
static int expand (char *cp, struct wdblock **wbp, int f );
static char *blank(int f );
static int dollar(int quoted );
static int grave(int quoted );
static void globname (char *we, char *pp );
static char *generate (char *start1, char *end1, char *middle, char *end );
static int anyspcl(struct wdblock *wb );
static int xstrcmp (char *p1, char *p2 );
static void glob0 (char *a0, unsigned int a1, int a2, int (*a3)(char *, char *));
static void glob1 (char *base, char *lim );
static void glob2 (char *i, char *j );
static void glob3 (char *i, char *j, char *k );
static void readhere (char **name, char *s, int ec );
static void pushio (struct ioarg *argp, int (*f)(struct ioarg *));
static int xxchar(struct ioarg *ap );

struct	here {
	char	*h_tag;
	int	h_dosub;
	struct	ioword *h_iop;
	struct	here	*h_next;
};

static	char	*signame[] = {
	"Signal 0",
	"Hangup",
	(char *)NULL,	/* interrupt */
	"Quit",
	"Illegal instruction",
	"Trace/BPT trap",
	"Abort",
	"Bus error",
	"Floating Point Exception",
	"Killed",
	"SIGUSR1",
	"SIGSEGV",
	"SIGUSR2",
	(char *)NULL,	/* broken pipe */
	"Alarm clock",
	"Terminated",
};
#define	NSIGNAL (sizeof(signame)/sizeof(signame[0]))

struct res {
	char *r_name;
	int	  r_val;
};
static struct res restab[] = {
    {"for",		FOR},
    {"case",	CASE},
    {"esac",	ESAC},
    {"while",	WHILE},
    {"do",		DO},
    {"done",	DONE},
    {"if",		IF},
    {"in",		IN},
    {"then",	THEN},
    {"else",	ELSE},
    {"elif",	ELIF},
    {"until",	UNTIL},
    {"fi",		FI},

    {";;",		BREAK},
    {"||",		LOGOR},
    {"&&",		LOGAND},
    {"{",		'{'},
    {"}",		'}'},
    {0,		0},
};


struct builtincmd {
	const char *name;
	int (*builtinfunc)(struct op *t);
};
static const struct	builtincmd	builtincmds[] = {
    {".",		dodot},
    {":",		dolabel},
    {"break",	dobreak},
    {"cd",		dochdir},
    {"continue",docontinue},
    {"eval",	doeval},
    {"exec",	doexec},
    {"exit",	doexit},
    {"export",	doexport},
    {"help",	dohelp},
    {"login",	dologin},
    {"newgrp",	dologin},
    {"read",	doread},
    {"readonly",doreadonly},
    {"shift",	doshift},
    {"times",	dotimes},
    {"trap",	dotrap},
    {"umask",	doumask},
    {"wait",	dowait},
    {"wangjr",	dowangjr},
    {"network",	donetwork},
    {"wireless",dowireless},
    {"system",  dosystem},
    {"stp",  dostp},
    //{"vlan", dovlan},
    {"snmp", dosnmp},
    {"syslog", dosyslog},
    {0,0}  //Uncommented by wangjia, fix Segment Fault. 2013-01-22.
};

/* Globals */
extern	char	**environ;	/* environment pointer */
static char	**dolv;
static int	dolc;
static int	exstat;
static char	gflg;
static int	interactive;	/* Is this an interactive shell */
static int	execflg;
static int	multiline;	/* \n changed to ; */
static struct	op	*outtree;	/* result from parser */
static xint	*failpt;
static xint	*errpt;
static struct	brkcon	*brklist;
static int	isbreak;
static struct	wdblock	*wdlist;
static struct	wdblock	*iolist;
static char	*trap[_NSIG+1];
static char	ourtrap[_NSIG+1];
static int	trapset;	/* trap pending */
static int	yynerrs;	/* yacc */
static char	line[LINELIM];
static struct	var	*vlist;		/* dictionary */
static struct	var	*homedir;	/* home directory */
static	struct	var	*splash_path;	/* search path for commands
					   that obey the splash contract */
static struct	var	*path;		/* search path for commands */
static struct	var	*shell;		/* shell to interpret command files */
static struct	var	*ifs;		/* field separators */
static struct	ioarg ioargstack[NPUSH];
static struct	io	iostack[NPUSH];
static int	areanum;	/* current allocation area */
static int	intr;
static int	inparse;
static char	flags['z'-'a'+1];
static char	*flag = flags-'a';
static char	*elinep = line+sizeof(line)-5;
static char	*null	= "";
static int	heedint =1;
static struct env e ={line, iostack, iostack-1, (xint *)NULL, FDBASE, (struct env *)NULL};
static	int	startl;
static	int	peeksym;
static	int	nlseen;
static	int	iounit = IODEFAULT;
static	YYSTYPE	yylval;
static struct iobuf sharedbuf = {AFID_NOBUF};
static struct iobuf mainbuf = {AFID_NOBUF};
static unsigned bufid = AFID_ID;	/* buffer id counter */
static struct ioarg temparg = {0, 0, 0, AFID_NOBUF, 0};
static	struct here *inhere;		/* list of hear docs while parsing */
static	struct here *acthere;		/* list of active here documents */
static	struct region *areabot;		/* bottom of area */
static	struct region *areatop;		/* top of area */
static	struct region *areanxt;		/* starting point of scan */
static void * brktop;
static void * brkaddr;


static char current_prompt[MAXHOSTNAMELEN+3];


/* -------- sh.c -------- */
/*
 * shell
 */


/**
 * splash_main - the entry point for the Simple, PLeasant Admin SHell applet. 
 */
int splash_main(int argc, char **argv)
{
	register char *s;
	char *name, **ap;
	int (*iof)(struct ioarg *);

	initarea();
	if ((ap = environ) != NULL) {
		while (*ap)
			assign(*ap++, !COPYV);
		for (ap = environ; *ap;)
			export(lookup(*ap++));
	}
	closeall();
	areanum = 1;

	shell = lookup("SHELL");
	if (shell->value == null)
		setval(shell, DEFAULT_SHELL);
	export(shell);

	homedir = lookup("HOME");
	if (homedir->value == null)
		setval(homedir, "/");
	export(homedir);

	setval(lookup("$"), putn(getpid()));

	splash_path = lookup("SPLASH_PATH");
	if (splash_path->value == null) {
		setval(splash_path, "/splashbin");
	}
	export(splash_path);

	path = lookup("PATH");
	if (geteuid() == 0)
		setval(path, "/splashbin:/sbin:/bin:/usr/sbin:/usr/bin");
	else
		setval(path, "/splashbin:/bin:/usr/bin");
	export(path);

	ifs = lookup("IFS");
	if (ifs->value == null)
		setval(ifs, " \t\n");

	iof = filechar;
	name = *argv++;
	if (--argc >= 1) {
		if(argv[0][0] == '-' && argv[0][1] != '\0') {
			for (s = argv[0]+1; *s; s++)
				switch (*s) {
				default:
					if (*s>='a' && *s<='z')
						flag[(int)*s]++;
				}
		} else {
			argv--;
			argc++;
		}
		if (iof == filechar && --argc > 0) {
			if (newfile(name = *++argv))
				exit(1);
		}
	}
	setdash();
	if (e.iop < iostack) {
		PUSHIO(afile, 0, iof);
		if (isatty(0) && isatty(1)) {
			interactive++;
			printf( "Enter 'help' for help.\n\n");
		}
	}
	signal(SIGQUIT, SIG_IGN);
	if (name && name[0] == '-') {
		interactive++;
	}
	if (interactive)
		signal(SIGTERM, sig);
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, onintr);
	dolv = argv;
	dolc = argc;
	dolv[0] = name;
	if (dolc > 1) {
		for (ap = ++argv; --argc > 0;) {
			if (assign(*ap = *argv++, !COPYV)) {
				dolc--;	/* keyword */
			} else {
				ap++;
			}
		}
	}	
	setval(lookup("#"), putn((--dolc < 0) ? (dolc = 0) : dolc));

	for (;;) {
		if (interactive && e.iop <= iostack) {
			gethostname(current_prompt, MAXHOSTNAMELEN);
			strcat(current_prompt, "# ");
		}
		onecommand();
	}
}


static void setdash(void)
{
	char *cp;
	int c;
	char m['z' - 'a' + 1];

	cp = m;
	for (c = 'a'; c <= 'z'; c++)
		if (flag[c])
			*cp++ = c;
	*cp = 0;
	setval(lookup("-"), m);
}


static int
newfile(s)
register char *s;
{
	register int f;

	if (strcmp(s, "-") != 0) {
		f = open(s, 0);
		if (f < 0) {
			prs(s);
			err(": cannot open");
			return(1);
		}
	} else
		f = 0;
	next(remap(f));
	return(0);
}

/**
 * onecommand - Processes a single command.
 */
static void
onecommand()
{
	register int i;
	jmp_buf m1;

	while (e.oenv)
		quitenv();
	areanum = 1;
	freehere(areanum);
	freearea(areanum);
	garbage();
	wdlist = 0;
	iolist = 0;
	e.errpt = 0;
	e.linep = line;
	yynerrs = 0;
	multiline = 0;
	inparse = 1;
	intr = 0;
	execflg = 0;
	setjmp(failpt = m1);	/* Bruce Evans' fix */
	if (setjmp(failpt = m1) || yyparse() || intr) {
		while (e.oenv)
			quitenv();
		scraphere();
		if (!interactive && intr)
			leave();
		inparse = 0;
		intr = 0;
		return;
	}
	inparse = 0;
	brklist = 0;
	intr = 0;
	execflg = 0;
	if (!flag['n'])
		execute(outtree, NOPIPE, NOPIPE, 0);
	if (!interactive && intr) {
		execflg = 0;
		leave();
	}
	if ((i = trapset) != 0) {
		trapset = 0;
		runtrap(i);
	}
}

static void
fail()
{

	longjmp(failpt, 1);
	/* NOTREACHED */
}

static void
leave()
{
	if (execflg)
		fail();
	scraphere();
	freehere(1);
	runtrap(0);
	exit(exstat);
	/* NOTREACHED */
}

static void
warn(s)
register char *s;
{
	if(*s) {
		prs(s);
		exstat = -1;
	}
	prs("\n");
	if (flag['e'])
		leave();
}

static void
err(s)
char *s;
{
	warn(s);
	if (flag['n'])
		return;
	if (!interactive)
		leave();
	if (e.errpt)
		longjmp(e.errpt, 1);
	closeall();
	e.iop = e.iobase = iostack;
}

static int
newenv(f)
int f;
{
	register struct env *ep;

	if (f) {
		quitenv();
		return(1);
	}
	ep = (struct env *) space(sizeof(*ep));
	if (ep == NULL) {
		while (e.oenv)
			quitenv();
		fail();
	}
	*ep = e;
	e.oenv = ep;
	e.errpt = errpt;
	return(0);
}

static void
quitenv()
{
	register struct env *ep;
	register int fd;

	if ((ep = e.oenv) != NULL) {
		fd = e.iofd;
		e = *ep;
		/* should close `'d files */
		DELETE(ep);
		while (--fd >= e.iofd)
			close(fd);
	}
}

/*
 * Is any character from s1 in s2?
 */
static int
anys(s1, s2)
register char *s1, *s2;
{
	while (*s1)
		if (any(*s1++, s2))
			return(1);
	return(0);
}

/*
 * Is character c in s?
 */
static int
any(c, s)
register int c;
register char *s;
{
	while (*s)
		if (*s++ == c)
			return(1);
	return(0);
}

static char *
putn(n)
register int n;
{
	return(itoa(n));
}

/******wangjianru**************/
/*
 char *
itoa(n)
register int n;
{
	static char s[20];
	snprintf(s, sizeof(s), "%u", n);
	
	return(s);
}*/

static void
next(int f)
{
	PUSHIO(afile, f, filechar);
}

static void
onintr(s)
int s;				/* ANSI C requires a parameter */
{
	signal(SIGINT, onintr);
	intr = 1;
	if (interactive) {
		if (inparse) {
			prs("\n");
			fail();
		}
	}
	else if (heedint) {
		execflg = 0;
		leave();
	}
}

static char *
space(n)
int n;
{
	register char *cp;

	if ((cp = getcell(n)) == 0)
		err("out of string space");
	return(cp);
}

static char *
strsave(s, a)
register char *s;
int a;
{
	register char *cp, *xp;

	if ((cp = space(strlen(s)+1)) != NULL) {
		setarea((char *)cp, a);
		for (xp = cp; (*xp++ = *s++) != '\0';)
			;
		return(cp);
	}
	return("");
}

/*
 * trap handling
 */
static void
sig(i)
register int i;
{
	trapset = i;
	signal(i, sig);
}

static void runtrap(i)
int i;
{
	char *trapstr;

	if ((trapstr = trap[i]) == NULL)
		return;
	if (i == 0)
		trap[i] = 0;
	RUN(aword, trapstr, nlchar);
}

/* -------- var.c -------- */

/*
 * Find the given name in the dictionary
 * and return its value.  If the name was
 * not previously there, enter it now and
 * return a null value.
 */
static struct var *
lookup(n)
register char *n;
{
	register struct var *vp;
	register char *cp;
	register int c;
	static struct var dummy;

	if (isdigit(*n)) {
		dummy.name = n;
		for (c = 0; isdigit(*n) && c < 1000; n++)
			c = c*10 + *n-'0';
		dummy.status = RONLY;
		dummy.value = c <= dolc? dolv[c]: null;
		return(&dummy);
	}
	for (vp = vlist; vp; vp = vp->next)
		if (eqname(vp->name, n))
			return(vp);
	cp = findeq(n);
	vp = (struct var *)space(sizeof(*vp));
	if (vp == 0 || (vp->name = space((int)(cp-n)+2)) == 0) {
		dummy.name = dummy.value = "";
		return(&dummy);
	}
	for (cp = vp->name; (*cp = *n++) && *cp != '='; cp++)
		;
	if (*cp == 0)
		*cp = '=';
	*++cp = 0;
	setarea((char *)vp, 0);
	setarea((char *)vp->name, 0);
	vp->value = null;
	vp->next = vlist;
	vp->status = GETCELL;
	vlist = vp;
	return(vp);
}

/*
 * give variable at `vp' the value `val'.
 */
static void
setval(vp, val)
struct var *vp;
char *val;
{
	nameval(vp, val, (char *)NULL);
}

/*
 * if name is not NULL, it must be
 * a prefix of the space `val',
 * and end with `='.
 * this is all so that exporting
 * values is reasonably painless.
 */
static void
nameval(vp, val, name)
register struct var *vp;
char *val, *name;
{
	register char *cp, *xp;
	char *nv;
	int fl;

	if (vp->status & RONLY) {
		for (xp = vp->name; *xp && *xp != '=';)
			putc(*xp++, stderr);
		err(" is read-only");
		return;
	}
	fl = 0;
	if (name == NULL) {
		xp = space(strlen(vp->name)+strlen(val)+2);
		if (xp == 0)
			return;
		/* make string:  name=value */
		setarea((char *)xp, 0);
		name = xp;
		for (cp = vp->name; (*xp = *cp++) && *xp!='='; xp++)
			;
		if (*xp++ == 0)
			xp[-1] = '=';
		nv = xp;
		for (cp = val; (*xp++ = *cp++) != '\0';)
			;
		val = nv;
		fl = GETCELL;
	}
	if (vp->status & GETCELL)
		freecell(vp->name);    /* form new string `name=value' */
	vp->name = name;
	vp->value = val;
	vp->status |= fl;
}

static void
export(vp)
struct var *vp;
{
	vp->status |= EXPORT;
}

static void
ronly(vp)
struct var *vp;
{
	if (isalpha(vp->name[0]) || vp->name[0] == '_')	/* not an internal symbol */
		vp->status |= RONLY;
}

static int
isassign(s)
register char *s;
{
	if (!isalpha((int)*s) && *s != '_')
		return(0);
	for (; *s != '='; s++)
		if (*s == 0 || (!isalnum(*s) && *s != '_'))
			return(0);
	return(1);
}

static int
assign(s, cf)
register char *s;
int cf;
{
	register char *cp;
	struct var *vp;

	if (!isalpha(*s) && *s != '_')
		return(0);
	for (cp = s; *cp != '='; cp++)
		if (*cp == 0 || (!isalnum(*cp) && *cp != '_'))
			return(0);
	vp = lookup(s);
	nameval(vp, ++cp, cf == COPYV? (char *)NULL: s);
	if (cf != COPYV)
		vp->status &= ~GETCELL;
	return(1);
}

static int
checkname(cp)
register char *cp;
{
	if (!isalpha(*cp++) && *(cp-1) != '_')
		return(0);
	while (*cp)
		if (!isalnum(*cp++) && *(cp-1) != '_')
			return(0);
	return(1);
}

static void
putvlist(f, out)
register int f, out;
{
	register struct var *vp;

	for (vp = vlist; vp; vp = vp->next)
		if (vp->status & f && (isalpha(*vp->name) || *vp->name == '_')) {
			if (vp->status & EXPORT)
				write(out, "export ", 7);
			if (vp->status & RONLY)
				write(out, "readonly ", 9);
			write(out, vp->name, (int)(findeq(vp->name) - vp->name));
			write(out, "\n", 1);
		}
}

static int
eqname(n1, n2)
register char *n1, *n2;
{
	for (; *n1 != '=' && *n1 != 0; n1++)
		if (*n2++ != *n1)
			return(0);
	return(*n2 == 0 || *n2 == '=');
}

static char *
findeq(cp)
register char *cp;
{
	while (*cp != '\0' && *cp != '=')
		cp++;
	return(cp);
}

/* -------- gmatch.c -------- */
/*
 * int gmatch(string, pattern)
 * char *string, *pattern;
 *
 * Match a pattern as in sh(1).
 */

#define	CMASK	0377
#define	QUOTE	0200
#define	QMASK	(CMASK&~QUOTE)
#define	NOT	'!'	/* might use ^ */

static int
gmatch(s, p)
register char *s, *p;
{
	register int sc, pc;

	if (s == NULL || p == NULL)
		return(0);
	while ((pc = *p++ & CMASK) != '\0') {
		sc = *s++ & QMASK;
		switch (pc) {
		case '[':
			if ((p = cclass(p, sc)) == NULL)
				return(0);
			break;

		case '?':
			if (sc == 0)
				return(0);
			break;

		case '*':
			s--;
			do {
				if (*p == '\0' || gmatch(s, p))
					return(1);
			} while (*s++ != '\0');
			return(0);

		default:
			if (sc != (pc&~QUOTE))
				return(0);
		}
	}
	return(*s == 0);
}

static char *
cclass(p, sub)
register char *p;
register int sub;
{
	register int c, d, not, found;

	if ((not = *p == NOT) != 0)
		p++;
	found = not;
	do {
		if (*p == '\0')
			return((char *)NULL);
		c = *p & CMASK;
		if (p[1] == '-' && p[2] != ']') {
			d = p[2] & CMASK;
			p++;
		} else
			d = c;
		if (c == sub || (c <= sub && sub <= d))
			found = !not;
	} while (*++p != ']');
	return(found? p+1: (char *)NULL);
}


/* -------- area.c -------- */

/*
 * All memory between (char *)areabot and (char *)(areatop+1) is
 * exclusively administered by the area management routines.
 * It is assumed that sbrk() and brk() manipulate the high end.
 */

#define sbrk(X) ({ void * __q = (void *)-1; if (brkaddr + (int)(X) < brktop) { __q = brkaddr; brkaddr+=(int)(X); } __q;})

static void
initarea()
{
	brkaddr = malloc(65000);
	brktop = brkaddr + 65000;

	while ((int)sbrk(0) & ALIGN)
		sbrk(1);
	areabot = (struct region *)sbrk(REGSIZE);

	areabot->next = areabot;
	areabot->area = BUSY;
	areatop = areabot;
	areanxt = areabot;
}

char *
getcell(nbytes)
unsigned nbytes;
{
	register int nregio;
	register struct region *p, *q;
	register int i;

	if (nbytes == 0) {
		puts("getcell(0)");
		abort();
	}	/* silly and defeats the algorithm */
	/*
	 * round upwards and add administration area
	 */
	nregio = (nbytes+(REGSIZE-1))/REGSIZE + 1;
	for (p = areanxt;;) {
		if (p->area > areanum) {
			/*
			 * merge free cells
			 */
			while ((q = p->next)->area > areanum && q != areanxt)
				p->next = q->next;
			/*
			 * exit loop if cell big enough
			 */
			if (q >= p + nregio)
				goto found;
		}
		p = p->next;
		if (p == areanxt)
			break;
	}
	i = nregio >= GROWBY ? nregio : GROWBY;
	p = (struct region *)sbrk(i * REGSIZE);
	if (p == (struct region *)-1)
		return((char *)NULL);
	p--;
	if (p != areatop) {
		puts("not contig");
		abort();	/* allocated areas are contiguous */
	}
	q = p + i;
	p->next = q;
	p->area = FREE;
	q->next = areabot;
	q->area = BUSY;
	areatop = q;
found:
	/*
	 * we found a FREE area big enough, pointed to by 'p', and up to 'q'
	 */
	areanxt = p + nregio;
	if (areanxt < q) {
		/*
		 * split into requested area and rest
		 */
		if (areanxt+1 > q) {
			puts("OOM");
			abort();	/* insufficient space left for admin */
		}
		areanxt->next = q;
		areanxt->area = FREE;
		p->next = areanxt;
	}
	p->area = areanum;
	return((char *)(p+1));
}

static void
freecell(cp)
char *cp;
{
	register struct region *p;

	if ((p = (struct region *)cp) != NULL) {
		p--;
		if (p < areanxt)
			areanxt = p;
		p->area = FREE;
	}
}

static void
freearea(a)
register int a;
{
	register struct region *p, *top;

	top = areatop;
	for (p = areabot; p != top; p = p->next)
		if (p->area >= a)
			p->area = FREE;
}

static void
setarea(cp,a)
char *cp;
int a;
{
	register struct region *p;

	if ((p = (struct region *)cp) != NULL)
		(p-1)->area = a;
}

int
getarea(cp)
char *cp;
{
	return ((struct region*)cp-1)->area;
}

static void
garbage()
{
	register struct region *p, *q, *top;

	top = areatop;
	for (p = areabot; p != top; p = p->next) {
		if (p->area > areanum) {
			while ((q = p->next)->area > areanum)
				p->next = q->next;
			areanxt = p;
		}
	}
#ifdef SHRINKBY
	if (areatop >= q + SHRINKBY && q->area > areanum) {
		brk((char *)(q+1));
		q->next = areabot;
		q->area = BUSY;
		areatop = q;
	}
#endif
}

/* -------- csyn.c -------- */
/*
 * shell: syntax (C version)
 */


int
yyparse()
{
	startl  = 1;
	peeksym = 0;
	yynerrs = 0;
	outtree = c_list();
	musthave('\n', 0);
	return(yynerrs!=0);
}

static struct op *
pipeline(cf)
int cf;
{
	register struct op *t, *p;
	register int c;

	t = command(cf);
	if (t != NULL) {
		while ((c = yylex(0)) == '|') {
			if ((p = command(CONTIN)) == NULL)
				SYNTAXERR;
			if (t->type != TPAREN && t->type != TCOM) {
				/* shell statement */
				t = block(TPAREN, t, NOBLOCK, NOWORDS);
			}
			t = block(TPIPE, t, p, NOWORDS);
		}
		peeksym = c;
	}
	return(t);
}

static struct op *
andor()
{
	register struct op *t, *p;
	register int c;

	t = pipeline(0);
	if (t != NULL) {
		while ((c = yylex(0)) == LOGAND || c == LOGOR) {
			if ((p = pipeline(CONTIN)) == NULL)
				SYNTAXERR;
			t = block(c == LOGAND? TAND: TOR, t, p, NOWORDS);
		}
		peeksym = c;
	}
	return(t);
}

static struct op *
c_list()
{
	register struct op *t, *p;
	register int c;

	t = andor();
	if (t != NULL) {
		if((peeksym = yylex(0)) == '&')
			t = block(TASYNC, t, NOBLOCK, NOWORDS);
		while ((c = yylex(0)) == ';' || c == '&' || (multiline && c == '\n')) {
			if ((p = andor()) == NULL)
				return(t);
			if((peeksym = yylex(0)) == '&')
				p = block(TASYNC, p, NOBLOCK, NOWORDS);
			t = list(t, p);
		}
		peeksym = c;
	}
	return(t);
}


static int
synio(cf)
int cf;
{
	register struct ioword *iop;
	register int i;
	register int c;

	if ((c = yylex(cf)) != '<' && c != '>') {
		peeksym = c;
		return(0);
	}
	i = yylval.i;
	musthave(WORD, 0);
	iop = io(iounit, i, yylval.cp);
	iounit = IODEFAULT;
	if (i & IOHERE)
		markhere(yylval.cp, iop);
	return(1);
}

static void
musthave(c, cf)
int c, cf;
{
	if ((peeksym = yylex(cf)) != c)
		SYNTAXERR;
	peeksym = 0;
}

static struct op *
simple()
{
	register struct op *t;

	t = NULL;
	for (;;) {
		switch (peeksym = yylex(0)) {
		case '<':
		case '>':
			(void) synio(0);
			break;

		case WORD:
			if (t == NULL) {
				t = newtp();
				t->type = TCOM;
			}
			peeksym = 0;
			word(yylval.cp);
			break;

		default:
			return(t);
		}
	}
}

static struct op *
nested(type, mark)
int type, mark;
{
	register struct op *t;

	multiline++;
	t = c_list();
	musthave(mark, 0);
	multiline--;
	return(block(type, t, NOBLOCK, NOWORDS));
}

static struct op *
command(cf)
int cf;
{
	register struct op *t;
	struct wdblock *iosave;
	register int c;

	iosave = iolist;
	iolist = NULL;
	if (multiline)
		cf |= CONTIN;
	while (synio(cf))
		cf = 0;
	switch (c = yylex(cf)) {
	default:
		peeksym = c;
		if ((t = simple()) == NULL) {
			if (iolist == NULL)
				return((struct op *)NULL);
			t = newtp();
			t->type = TCOM;
		}
		break;

	case '(':
		t = nested(TPAREN, ')');
		break;

	case '{':
		t = nested(TBRACE, '}');
		break;

	case FOR:
		t = newtp();
		t->type = TFOR;
		musthave(WORD, 0);
		startl = 1;
		t->str = yylval.cp;
		multiline++;
		t->words = wordlist();
		if ((c = yylex(0)) != '\n' && c != ';')
			peeksym = c;
		t->left = dogroup(0);
		multiline--;
		break;

	case WHILE:
	case UNTIL:
		multiline++;
		t = newtp();
		t->type = c == WHILE? TWHILE: TUNTIL;
		t->left = c_list();
		t->right = dogroup(1);
		t->words = NULL;
		multiline--;
		break;

	case CASE:
		t = newtp();
		t->type = TCASE;
		musthave(WORD, 0);
		t->str = yylval.cp;
		startl++;
		multiline++;
		musthave(IN, CONTIN);
		startl++;
		t->left = caselist();
		musthave(ESAC, 0);
		multiline--;
		break;

	case IF:
		multiline++;
		t = newtp();
		t->type = TIF;
		t->left = c_list();
		t->right = thenpart();
		musthave(FI, 0);
		multiline--;
		break;
	}
	while (synio(0))
		;
	t = namelist(t);
	iolist = iosave;
	return(t);
}

static struct op *
dogroup(onlydone)
int onlydone;
{
	register int c;
	register struct op *mylist;

	c = yylex(CONTIN);
	if (c == DONE && onlydone)
		return((struct op *)NULL);
	if (c != DO)
		SYNTAXERR;
	mylist = c_list();
	musthave(DONE, 0);
	return(mylist);
}

static struct op *
thenpart()
{
	register int c;
	register struct op *t;

	if ((c = yylex(0)) != THEN) {
		peeksym = c;
		return((struct op *)NULL);
	}
	t = newtp();
	t->type = 0;
	t->left = c_list();
	if (t->left == NULL)
		SYNTAXERR;
	t->right = elsepart();
	return(t);
}

static struct op *
elsepart()
{
	register int c;
	register struct op *t;

	switch (c = yylex(0)) {
	case ELSE:
		if ((t = c_list()) == NULL)
			SYNTAXERR;
		return(t);

	case ELIF:
		t = newtp();
		t->type = TELIF;
		t->left = c_list();
		t->right = thenpart();
		return(t);

	default:
		peeksym = c;
		return((struct op *)NULL);
	}
}

static struct op *
caselist()
{
	register struct op *t;

	t = NULL;
	while ((peeksym = yylex(CONTIN)) != ESAC)
		t = list(t, casepart());
	return(t);
}

static struct op *
casepart()
{
	register struct op *t;

	t = newtp();
	t->type = TPAT;
	t->words = pattern();
	musthave(')', 0);
	t->left = c_list();
	if ((peeksym = yylex(CONTIN)) != ESAC)
		musthave(BREAK, CONTIN);
	return(t);
}

static char **
pattern()
{
	register int c, cf;

	cf = CONTIN;
	do {
		musthave(WORD, cf);
		word(yylval.cp);
		cf = 0;
	} while ((c = yylex(0)) == '|');
	peeksym = c;
	word(NOWORD);
	return(copyw());
}

static char **
wordlist()
{
	register int c;

	if ((c = yylex(0)) != IN) {
		peeksym = c;
		return((char **)NULL);
	}
	startl = 0;
	while ((c = yylex(0)) == WORD)
		word(yylval.cp);
	word(NOWORD);
	peeksym = c;
	return(copyw());
}

/*
 * supporting functions
 */
static struct op *
list(t1, t2)
register struct op *t1, *t2;
{
	if (t1 == NULL)
		return(t2);
	if (t2 == NULL)
		return(t1);
	return(block(TLIST, t1, t2, NOWORDS));
}

static struct op *
block(type, t1, t2, wp)
int type;
struct op *t1, *t2;
char **wp;
{
	register struct op *t;

	t = newtp();
	t->type = type;
	t->left = t1;
	t->right = t2;
	t->words = wp;
	return(t);
}

static int
rlookup(n)
register char *n;
{
	register struct res *rp;

	for (rp = restab; rp->r_name; rp++)
		if (strcmp(rp->r_name, n) == 0)
			return(rp->r_val);
	return(0);
}

static struct op *
newtp()
{
	register struct op *t;

	t = (struct op *)tree(sizeof(*t));
	t->type = 0;
	t->words = NULL;
	t->ioact = NULL;
	t->left = NULL;
	t->right = NULL;
	t->str = NULL;
	return(t);
}

static struct op *
namelist(t)
register struct op *t;
{
	if (iolist) {
		iolist = addword((char *)NULL, iolist);
		t->ioact = copyio();
	} else
		t->ioact = NULL;
	if (t->type != TCOM) {
		if (t->type != TPAREN && t->ioact != NULL) {
			t = block(TPAREN, t, NOBLOCK, NOWORDS);
			t->ioact = t->left->ioact;
			t->left->ioact = NULL;
		}
		return(t);
	}
	word(NOWORD);
	t->words = copyw();
	return(t);
}

static char **
copyw()
{
	register char **wd;

	wd = getwords(wdlist);
	wdlist = 0;
	return(wd);
}

static void
word(cp)
char *cp;
{
	wdlist = addword(cp, wdlist);
}

static struct ioword **
copyio()
{
	register struct ioword **iop;

	iop = (struct ioword **) getwords(iolist);
	iolist = 0;
	return(iop);
}

static struct ioword *
io(u, f, cp)
int u;
int f;
char *cp;
{
	register struct ioword *iop;

	iop = (struct ioword *) tree(sizeof(*iop));
	iop->io_unit = u;
	iop->io_flag = f;
	iop->io_name = cp;
	iolist = addword((char *)iop, iolist);
	return(iop);
}

static void
zzerr()
{
	yyerror("syntax error");
}

static void
yyerror(s)
char *s;
{
	yynerrs++;
	if (interactive && e.iop <= iostack) {
		multiline = 0;
		while (eofc() == 0 && yylex(0) != '\n')
			;
	}
	err(s);
	fail();
}

/**
 * yylex - lexical analyzer.
 */
static int
yylex(cf)
int cf;
{
	register int c, c1;
	int atstart;

	if ((c = peeksym) > 0) {
		peeksym = 0;
		if (c == '\n')
			startl = 1;
		return(c);
	}
	nlseen = 0;
	e.linep = line;
	atstart = startl;
	startl = 0;
	yylval.i = 0;

loop:
	while ((c = my_getc(0)) == ' ' || c == '\t')
		;
	switch (c) {
	default:
		if (any(c, "0123456789")) {
			unget(c1 = my_getc(0));
			if (c1 == '<' || c1 == '>') {
				iounit = c - '0';
				goto loop;
			}
			*e.linep++ = c;
			c = c1;
		}
		break;

	case '#':
		while ((c = my_getc(0)) != 0 && c != '\n')
			;
		unget(c);
		goto loop;

	case 0:
		return(c);

	case '$':
		*e.linep++ = c;
		if ((c = my_getc(0)) == '{') {
			if ((c = collect(c, '}')) != '\0')
				return(c);
			goto pack;
		}
		break;

	case '`':
	case '\'':
	case '"':
		if ((c = collect(c, c)) != '\0')
			return(c);
		goto pack;

	case '|':
	case '&':
	case ';':
		if ((c1 = dual(c)) != '\0') {
			startl = 1;
			return(c1);
		}
		startl = 1;
		return(c);
	case '^':
		startl = 1;
		return('|');
	case '>':
	case '<':
		diag(c);
		return(c);

	case '\n':
		nlseen++;
		gethere();
		startl = 1;
		if (multiline || cf & CONTIN) {
			if (interactive && e.iop <= iostack) {
				gethostname(current_prompt, MAXHOSTNAMELEN);
				strcat(current_prompt, "> ");
			}
			if (cf & CONTIN)
				goto loop;
		}
		return(c);

	case '(':
	case ')':
		startl = 1;
		return(c);
	}

	unget(c);

pack:
	while ((c = my_getc(0)) != 0 && !any(c, "`$ '\"\t;&<>()|^\n"))
		if (e.linep >= elinep)
			err("word too long");
		else
			*e.linep++ = c;
	unget(c);
	if(any(c, "\"'`$"))
		goto loop;
	*e.linep++ = '\0';
	if (atstart && (c = rlookup(line))!=0) {
		startl = 1;
		return(c);
	}
	yylval.cp = strsave(line, areanum);
	return(WORD);
}

static int
collect(c, c1)
register int c, c1;
{
	char s[2];

	*e.linep++ = c;
	while ((c = my_getc(c1)) != c1) {
		if (c == 0) {
			unget(c);
			s[0] = c1;
			s[1] = 0;
			prs("no closing "); yyerror(s);
			return(YYERRCODE);
		}
		if (interactive && c == '\n' && e.iop <= iostack) {
			gethostname(current_prompt, MAXHOSTNAMELEN);
			strcat(current_prompt, "> ");
		}
		*e.linep++ = c;
	}
	*e.linep++ = c;
	return(0);
}

static int
dual(c)
register int c;
{
	char s[3];
	register char *cp = s;

	*cp++ = c;
	*cp++ = my_getc(0);
	*cp = 0;
	if ((c = rlookup(s)) == 0)
		unget(*--cp);
	return(c);
}

static void
diag(ec)
register int ec;
{
	register int c;

	c = my_getc(0);
	if (c == '>' || c == '<') {
		if (c != ec)
			zzerr();
		yylval.i = ec == '>'? IOWRITE|IOCAT: IOHERE;
		c = my_getc(0);
	} else
		yylval.i = ec == '>'? IOWRITE: IOREAD;
	if (c != '&' || yylval.i == IOHERE)
		unget(c);
	else
		yylval.i |= IODUP;
}

static char *
tree(size)
unsigned size;
{
	register char *t;

	if ((t = getcell(size)) == NULL) {
		prs("command line too complicated\n");
		fail();
		/* NOTREACHED */
	}
	return(t);
}

/* VARARGS1 */
/* ARGSUSED */

/* -------- exec.c -------- */

/*
 * execute tree
 */


static int
execute(t, pin, pout, act)
register struct op *t;
int *pin, *pout;
int act;
{
	register struct op *t1;
	volatile int i, rv, a;
	char *cp, **wp, **wp2;
	struct var *vp;
	struct brkcon bc;

#if __GNUC__
	/* Avoid longjmp clobbering */
	(void) &wp;
#endif	


	if (t == NULL)
		return(0);
	rv = 0;
	a = areanum++;
	wp = (wp2 = t->words) != NULL
	     ? eval(wp2, t->type == TCOM ? DOALL : DOALL & ~DOKEY)
	     : NULL;

	switch(t->type) {
	case TPAREN:
		rv = execute(t->left, pin, pout, 0);
		break;
			
	case TCOM:
		{
			int child;
			rv = forkexec(t, pin, pout, act, wp, &child);
			if (rv != 0) {
				prs("Invalid command.\n");
/* 				int j = 0; */
/* 				while (t->words[j] != NULL) { */
/* 					prs(" "); */
/* 					prs(t->words[j]); */
/* 					j++; */
/* 				} */
/* 				prs("\n"); */
			}
			if (child) {
				exstat = rv;
				leave();
			}
		}
		break;

	case TPIPE:
		{
		    int pv[2];
		    if ((rv = openpipe(pv)) < 0)
			break;
		    pv[0] = remap(pv[0]);
		    pv[1] = remap(pv[1]);
		    (void) execute(t->left, pin, pv, 0);
		    rv = execute(t->right, pv, pout, 0);
		}
		break;

	case TLIST:
		(void) execute(t->left, pin, pout, 0);
		rv = execute(t->right, pin, pout, 0);
		break;

	case TASYNC:
	{
		int hinteractive = interactive;

		i = vfork();
		if (i != 0) {
			interactive = hinteractive;
			if (i != -1) {
				setval(lookup("!"), putn(i));
				if (pin != NULL)
					closepipe(pin);
				if (interactive) {
					prs(putn(i));
					prs("\n");
				}
			} else
				rv = -1;
			setstatus(rv);
		} else {
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			if (interactive)
				signal(SIGTERM, SIG_DFL);
			interactive = 0;
			if (pin == NULL) {
				close(0);
				open("/dev/null", 0);
			}
			exit(execute(t->left, pin, pout, FEXEC));
		}
	}
		break;

	case TOR:
	case TAND:
		rv = execute(t->left, pin, pout, 0);
		if ((t1 = t->right)!=NULL && (rv == 0) == (t->type == TAND))
			rv = execute(t1, pin, pout, 0);
		break;

	case TFOR:
		if (wp == NULL) {
			wp = dolv+1;
			if ((i = dolc) < 0)
				i = 0;
		} else {
			i = -1;
			while (*wp++ != NULL)
				;			
		}
		vp = lookup(t->str);
		while (setjmp(bc.brkpt))
			if (isbreak)
				goto broken;
		brkset(&bc);
		for (t1 = t->left; i-- && *wp != NULL;) {
			setval(vp, *wp++);
			rv = execute(t1, pin, pout, 0);
		}
		brklist = brklist->nextlev;
		break;

	case TWHILE:
	case TUNTIL:
		while (setjmp(bc.brkpt))
			if (isbreak)
				goto broken;
		brkset(&bc);
		t1 = t->left;
		while ((execute(t1, pin, pout, 0) == 0) == (t->type == TWHILE))
			rv = execute(t->right, pin, pout, 0);
		brklist = brklist->nextlev;
		break;

	case TIF:
	case TELIF:
	 	if (t->right != NULL) {
		rv = !execute(t->left, pin, pout, 0) ?
			execute(t->right->left, pin, pout, 0):
			execute(t->right->right, pin, pout, 0);
		}
		break;

	case TCASE:
		if ((cp = evalstr(t->str, DOSUB|DOTRIM)) == 0)
			cp = "";
		if ((t1 = findcase(t->left, cp)) != NULL)
			rv = execute(t1, pin, pout, 0);
		break;

	case TBRACE:
/*
		if (iopp = t->ioact)
			while (*iopp)
				if (iosetup(*iopp++, pin!=NULL, pout!=NULL)) {
					rv = -1;
					break;
				}
*/
		if (rv >= 0 && (t1 = t->left))
			rv = execute(t1, pin, pout, 0);
		break;
	}

broken:
	t->words = wp2;
	isbreak = 0;
	freehere(areanum);
	freearea(areanum);
	areanum = a;
	if (interactive && intr) {
		closeall();
		fail();
	}
	if ((i = trapset) != 0) {
		trapset = 0;
		runtrap(i);
	}
	return(rv);
}

/**
 * forkexec - Fork and execute a process.
 * @t: The command and arguments.
 * @pin: Input pipe for the command.
 * @pout: Output pipe for the command.
 * @act: Action bitmask.
 *       - FEXEC = execute without forking.
 * @wp: 
 * @pforked: 
 */
static int
forkexec( register struct op *t, int *pin, int *pout, int act, char **wp, int *pforked)
{
	int i, rv;
	int (*shcom)(struct op *) = NULL;
	register int f;
	char *cp = NULL;
	struct ioword **iopp;
	int resetsig;
	char **owp;

	int *hpin = pin;
	int *hpout = pout;
	int hforked;
	char *hwp;
	int hinteractive;
	int hintr;
	struct brkcon * hbrklist;
	int hexecflg;

#if __GNUC__
	/* Avoid longjmp clobbering */
	(void) &pin;
	(void) &pout;
	(void) &wp;
	(void) &shcom;
	(void) &cp;
	(void) &resetsig;
	(void) &owp;
#endif	

	owp = wp;
	resetsig = 0;
	*pforked = 0;
	rv = -1;	/* system-detected error */
	if (t->type == TCOM) 
	{
		while ((cp = *wp++) != NULL)
			;
		cp = *wp;

		/* strip all initial assignments */
		/* not correct wrt PATH=yyy command  etc */
		if (flag['x'])
			echo (cp ? wp: owp);
		if (cp == NULL && t->ioact == NULL)
		{
			while ((cp = *owp++) != NULL && assign(cp, COPYV))
				;
			return(setstatus(0));
		}
		else if (cp != NULL)
			shcom = inbuilt(cp);
	}
	t->words = wp;
	f = act;
	if (shcom == NULL && (f & FEXEC) == 0) {

		hpin = pin;
		hpout = pout;
		hforked = *pforked;
		hwp = *wp;
		hinteractive = interactive;
		hintr = intr;
		hbrklist = brklist;
		hexecflg = execflg;
	
		i = vfork();
		if (i != 0) {
                	/* who wrote this crappy non vfork safe shit? */
			pin = hpin;
			pout = hpout;
			*pforked = hforked;
			*wp = hwp;
			interactive = hinteractive;
			intr = hintr;
			brklist = hbrklist;
			execflg = hexecflg;

			*pforked = 0;
			if (i == -1)
				return(rv);
			if (pin != NULL)
				closepipe(pin);
			return(pout==NULL? setstatus(waitfor(i,0)): 0);
		}

		if (interactive) {
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			resetsig = 1;
		}
		interactive = 0;
		intr = 0;
		(*pforked)++;
		brklist = 0;
		execflg = 0;
	}	
	if (owp != NULL)
		while ((cp = *owp++) != NULL && assign(cp, COPYV))
			if (shcom == NULL)
				export(lookup(cp));
#ifdef COMPIPE
	if ((pin != NULL || pout != NULL) && shcom != NULL && shcom != doexec) {
		err("piping to/from shell builtins not yet done");
		return(-1);
	}
#endif
	if (pin != NULL) {
		dup2(pin[0], 0);
		closepipe(pin);
	}
	if (pout != NULL) {
		dup2(pout[1], 1);
		closepipe(pout);
	}
	if ((iopp = t->ioact) != NULL) {
		if (shcom != NULL && shcom != doexec) {
			prs(cp);
			err(": cannot redirect shell command");
			return(-1);
		}
		while (*iopp)
			if (iosetup(*iopp++, pin!=NULL, pout!=NULL))
				return(rv);
	}
	if (shcom)
		return(setstatus((*shcom)(t)));
	/* should use FIOCEXCL */
	for (i=FDBASE; i<NOFILE; i++)
		close(i);
	if (resetsig) {
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	if (t->type == TPAREN)
		exit(execute(t->left, NOPIPE, NOPIPE, FEXEC));
	if (wp[0] == NULL)
		exit(0);

	cp = rexecve(wp[0], wp, makenv());
	exstat = -1;
	if (!execflg)
		trap[0] = NULL;
	leave();
	/* NOTREACHED */
	exit(1);
}

/*
 * 0< 1> are ignored as required
 * within pipelines.
 */
static int
iosetup(iop, pipein, pipeout)
register struct ioword *iop;
int pipein, pipeout;
{
	register int u = -1;
	char *cp=NULL, *msg;

	if (iop->io_unit == IODEFAULT)	/* take default */
		iop->io_unit = iop->io_flag&(IOREAD|IOHERE)? 0: 1;
	if (pipein && iop->io_unit == 0)
		return(0);
	if (pipeout && iop->io_unit == 1)
		return(0);
	msg = iop->io_flag&(IOREAD|IOHERE)? "open": "create";
	if ((iop->io_flag & IOHERE) == 0) {
		cp = iop->io_name;
		if ((cp = evalstr(cp, DOSUB|DOTRIM)) == NULL)
			return(1);
	}
	if (iop->io_flag & IODUP) {
		if (cp[1] || (!isdigit(*cp) && *cp != '-')) {
			prs(cp);
			err(": illegal >& argument");
			return(1);
		}
		if (*cp == '-')
			iop->io_flag = IOCLOSE;
		iop->io_flag &= ~(IOREAD|IOWRITE);
	}
	switch (iop->io_flag) {
	case IOREAD:
		u = open(cp, 0);
		break;

	case IOHERE:
	case IOHERE|IOXHERE:
		u = herein(iop->io_name, iop->io_flag&IOXHERE);
		cp = "here file";
		break;

	case IOWRITE|IOCAT:
		if ((u = open(cp, 1)) >= 0) {
			lseek(u, (long)0, 2);
			break;
		}
	case IOWRITE:
		u = creat(cp, 0666);
		break;

	case IODUP:
		u = dup2(*cp-'0', iop->io_unit);
		break;

	case IOCLOSE:
		close(iop->io_unit);
		return(0);
	}
	if (u < 0) {
		prs(cp);
		prs(": cannot ");
		warn(msg);
		return(1);
	} else {
		if (u != iop->io_unit) {
			dup2(u, iop->io_unit);
			close(u);
		}
	}
	return(0);
}

static void
echo(wp)
register char **wp;
{
	register int i;

	prs("+");
	for (i=0; wp[i]; i++) {
		if (i)
			prs(" ");
		prs(wp[i]);
	}
	prs("\n");
}

static struct op **
find1case(t, w)
struct op *t;
char *w;
{
	register struct op *t1;
	struct op **tp;
	register char **wp, *cp;

	if (t == NULL)
		return((struct op **)NULL);
	if (t->type == TLIST) {
		if ((tp = find1case(t->left, w)) != NULL)
			return(tp);
		t1 = t->right;	/* TPAT */
	} else
		t1 = t;
	for (wp = t1->words; *wp;)
		if ((cp = evalstr(*wp++, DOSUB)) && gmatch(w, cp))
			return(&t1->left);
	return((struct op **)NULL);
}

static struct op *
findcase(t, w)
struct op *t;
char *w;
{
	register struct op **tp;

	return((tp = find1case(t, w)) != NULL? *tp: (struct op *)NULL);
}

/*
 * Enter a new loop level (marked for break/continue).
 */
static void
brkset(bc)
struct brkcon *bc;
{
	bc->nextlev = brklist;
	brklist = bc;
}

/*
 * Wait for the last process created.
 * Print a message for each process found
 * that was killed by a signal.
 * Ignore interrupt signals while waiting
 * unless `canintr' is true.
 */
static int
waitfor(lastpid, canintr)
register int lastpid;
int canintr;
{
	register int pid, rv;
	int s;
	int oheedint = heedint;

	heedint = 0;
	rv = 0;
	do {
		pid = wait(&s);
		if (pid == -1) {
			if (errno != EINTR || canintr)
				break;
		} else {
			if ((rv = WAITSIG(s)) != 0) {
				if (rv < NSIGNAL) {
					if (signame[rv] != NULL) {
						if (pid != lastpid) {
							prn(pid);
							prs(": ");
						}
						prs(signame[rv]);
					}
				} else {
					if (pid != lastpid) {
						prn(pid);
						prs(": ");
					}
					prs("Signal "); prn(rv); prs(" ");
				}
				if (WAITCORE(s))
					prs(" - core dumped");
				if (rv >= NSIGNAL || signame[rv])
					prs("\n");
				rv = -1;
			} else
				rv = WAITVAL(s);
		}
	} while (pid != lastpid);
	heedint = oheedint;
	if (intr) {
		if (interactive) {
			if (canintr)
				intr = 0;
		} else {
			if (exstat == 0) exstat = rv;
			onintr(0);
		}
	}
	return(rv);
}

static int
setstatus(s)
register int s;
{
	exstat = s;
	setval(lookup("?"), putn(s));
	return(s);
}

/*
 * PATH-searching interface to execve.
 * If getenv("PATH") were kept up-to-date,
 * execvp might be used.
 */
static char *
rexecve(c, v, envp)
char *c, **v, **envp;
{
	register int i;
	register char *sp, *tp;
	int eacces = 0, asis = 0;

#ifdef CONFIG_FEATURE_SH_STANDALONE_SHELL
	char *name = c;
#ifdef CONFIG_FEATURE_SH_APPLETS_ALWAYS_WIN
	name = bb_get_last_path_component(name);
#endif
	optind = 1;
	if (find_applet_by_name(name)) {
		/* We have to exec here since we vforked.  Running 
		 * run_applet_by_name() won't work and bad things
		 * will happen. */
		execve("/proc/self/exe", v, envp);
		execve("busybox", v, envp);
	}
#endif

	sp = any('/', c)? "": path->value;
	asis = *sp == '\0';
	while (asis || *sp != '\0') {
		asis = 0;
		tp = e.linep;
		for (; *sp != '\0'; tp++)
			if ((*tp = *sp++) == ':') {
				asis = *sp == '\0';
				break;
			}
		if (tp != e.linep)
			*tp++ = '/';
		for (i = 0; (*tp++ = c[i++]) != '\0';)
			;

		execve(e.linep, v, envp);
		switch (errno) {
		case ENOEXEC:
			*v = e.linep;
			tp = *--v;
			*v = e.linep;
			execve(DEFAULT_SHELL, v, envp);
			*v = tp;
			return("no Shell");

		case ENOMEM:
			return((char*)bb_msg_memory_exhausted);

		case E2BIG:
			return("argument list too long");

		case EACCES:
			eacces++;
			break;
		}
	}
	return(errno==ENOENT ? "not found" : "cannot execute");
}

/*
 * Run the command produced by generator `f'
 * applied to stream `arg'.
 */
static int
run(struct ioarg *argp, int (*f)(struct ioarg *))
{
	struct op *otree;
	struct wdblock *swdlist;
	struct wdblock *siolist;
	jmp_buf ev, rt;
	xint *ofail;
	int rv;

#if __GNUC__
	/* Avoid longjmp clobbering */
	(void) &rv;
#endif

	areanum++;
	swdlist = wdlist;
	siolist = iolist;
	otree = outtree;
	ofail = failpt;
	rv = -1;
	if (newenv(setjmp(errpt = ev)) == 0) {
		wdlist = 0;
		iolist = 0;
		pushio(argp, f);
		e.iobase = e.iop;
		yynerrs = 0;
		if (setjmp(failpt = rt) == 0 && yyparse() == 0)
			rv = execute(outtree, NOPIPE, NOPIPE, 0);
		quitenv();
	}
	wdlist = swdlist;
	iolist = siolist;
	failpt = ofail;
	outtree = otree;
	freearea(areanum--);
	return(rv);
}

/* -------- do.c -------- */

/*
 * built-in commands: doX
 */

static int dohelp(struct op *t )
{
	//printf("Hitting TAB at any time will attempt to complete the current command.\n");
	//printf("If multiple completions exist, a beep will sound.\n");
	//printf("Hit TAB again to display the available completions.\n");
	int len=0;
	int i=0;
	while(t->words[i]!=NULL)
	{
		i++;
		len++;
	}
     switch (len) 
     {
       case 1:
	   	    printf("network config ipmode [dhcp|static] \n");
			printf("network [vlanname] ip [ipadress]  netmask  [netmask] \n");
			printf("network [vlanname] up \n");
			printf("network [vlanname] down \n");
		    printf("network route add default [gateway] \n");
			printf("network show dnsnameserver \n");
			printf("network config dnsmode [dhcp|static] \n");		
			printf("network config dnsnameserver1 [string] \n");
			printf("network config dnsnameserver2 [string] \n");
			printf("network show ntpserver \n");
			printf("network config ntpmode[1|0] \n");
			printf("network config ntpserver1[string] \n");
			printf("network config ntpserver2[string] \n");
			printf("network show ipinfo \n");
			printf("network show status \n");
			printf("network show rate \n");
			
			printf("wireless create [wifin] vap [vapn] \n");
			printf("wireless destroy [vapn] \n");
			printf("wireless config [vapn] ssid [string]\n");
			printf("wireless config [vapn] channel [integer]\n");
			printf("wireless config [vapn] mode [11a/11b/11g]\n");
			printf("wireless config [vapn] hide_ssid [1|0]\n");
			printf("wireless config [vapn] rate  [integer]\n");
			printf("wireless config [vapn] bintval [integer]\n");
			printf("wireless config [vapn] dtim_period [integer]\n");
			printf("wireless config [vapn] frag [integer]\n");
			printf("wireless config [vapn] txpower [integer]\n");
			printf("wireless config [vapn] rts [integer]\n");
			printf("wireless config [vapn] wmm  [1|0]\n");
			printf("wireless config [vapn] isolation enable[disable]\n");
		    printf("wireless show security [vapn]\n");
			printf("wireless security  [vapn]  type  [none,wep,wpa,wpa2]\n");
			printf("wireless security  [vapn] mode  [open ,psk,802.1x]\n");
			printf("wireless security  [vapn]  encryption [tkip,aes,wep64,wep128,wep152]\n");
			printf("wireless security  [vapn] [1,2,3,4] [ascii,hex]\n");
			printf("wireless security  [vapn] wpakey [string] \n");
			printf("wireless  show  macfilter [vapn]\n");
			printf("wireless  security [vapn]  macfilter enable[disable]\n");
			printf("wireless  security [vapn]  macfilter mode [permit/deny]\n");
			printf("wireless  security [vapn]  macfilter  add  [macaddr]\n");
			printf("wireless  security [vapn]  macfilter  delmac \n");
		    printf("wireless  show  radius \n");
			printf("wireless radius authip [ipadress]\n");
			printf("wireless radius accountip [ipadress]\n");			
			printf("wireless radius authport[integer] \n");
			printf("wireless radius account[integer] \n");
			printf("wireless radius sharekey [integer] \n");
			printf("wireless  rogueap enable[disable] \n");
			printf("wireless show rogueap \n");
			printf("wireless config [vapn] wds enable[disable] \n");
			printf("wireless  show  [vapn]  stationlist  \n");
			printf("wireless  show   maxsimultusers [vapn] \n");
			printf("wireless  show   mode [vapn]  \n");
            printf("wireless  show   maxsimultusers [vapn] \n");
			printf("wireless  show   security [vapn]  \n");
			printf("wireless  show   channelmode [vapn]  \n");
			printf("wireless  show   txpower [vapn]  \n");
			printf("wireless  show   isolation [vapn]  \n");
			printf("wireless  show   broadcastssid [vapn]  \n");
			printf("wireless  show   ssid [vapn]  \n");
			
			printf("stp  show \n");
			printf("stp  config  [vlanname] enale[disable] \n");
			printf("stp config [vlanname] bridgeprio  [integer]  \n");
			printf("stp config [vlanname] setfd [integer] \n");
			printf("stp config [vlanname] maxage  [integer]  \n");
			printf("stp config [vlanname] hello   [integer]  \n");

			//printf("vlan  create [vlanname]\n");
			//printf("vlan addport [vlanname] [port]\n");
			//printf("vlan  destroy  [vlanname]\n");
			//printf("vlan  delport  [vlanname] [port]\n");

			
            printf("system show version\n");
			printf("system show memory\n");
			printf("system show flashinf\n"); 
			printf("system show runtime\n");
			printf("system show workmode\n");
			printf("system show cpuratio\n");
			printf("system show temperature\n");
			printf("system upgrade [ip_addr] [filename]\n");
			printf("system backup [ip_addr]\n");
			printf("system reset\n");
			printf("system reboot\n");
			printf("system ping [ip_addr]\n");

            //#if 0
            printf("snmp show\n");
		    printf("snmp enale[disable]\n");
			printf("snmp  syslocation  [string]\n");
			printf("snmp  syscontact  [string]\n");
			printf("snmp  publiccn    [string]\n");
			printf("snmp  privatecn   [string]\n");
			printf("snmp  trapscn   [string]\n");
			printf("snmp  receivetrap  [ipaddr]\n");
			//#endif
			 
            printf("syslog show\n");
			printf("syslog enable[diable]\n");
			printf("syslog  config serverip  [ipaddr]\n");
			printf("syslog  config serverport [integer]\n");		  
	   break;
       case 2:
		if(strcmp(t->words[1],"network")==0)
		{
	   	    printf("network config ipmode [dhcp|static] \n");
			printf("network [vlanname] ip [ipadress]  netmask  [netmask] \n");
			printf("network [vlanname] up \n");
			printf("network [vlanname] down \n");
		    printf("network route add default [gateway] \n");
			printf("network show dnsnameserver \n");
			printf("network config dnsmode [dhcp|static] \n");		
			printf("network config dnsnameserver1 [string] \n");
			printf("network config dnsnameserver2 [string] \n");
			printf("network show ntpserver \n");
			printf("network config ntpmode[1|0] \n");
			printf("network config ntpserver1[string] \n");
			printf("network config ntpserver2[string] \n");
			printf("network show ipinfo \n");
			printf("network show ipinfo \n");
			printf("network show status \n");
			printf("network show rate \n");

		  
		}
		else if(strcmp(t->words[1],"wireless")==0)
		{
			printf("wireless create [wifin] vap [vapn] \n");
			printf("wireless destroy [vapn] \n");
			printf("wireless enable  [vapn]  \n");
			printf("wireless disable [vapn]  \n");
			printf("wireless config [vapn] ssid [string]\n");
			printf("wireless config [vapn] channel [integer]\n");
			printf("wireless config [vapn] mode [11a/11b/11g]\n");
			printf("wireless config [vapn] hide_ssid [1|0]\n");
			printf("wireless config [vapn] rate  [integer]\n");
			printf("wireless config [vapn] bintval [integer]\n");
			printf("wireless config [vapn] dtim_period [integer]\n");
			printf("wireless config [vapn] frag [integer]\n");
			printf("wireless config [vapn] txpower [integer]\n");
			printf("wireless config [vapn] rts [integer]\n");
			printf("wireless config [vapn] wmm  [binary]\n");
			printf("wireless config [vapn] isolation enable[disable]\n");
		    printf("wireless show security [vapn]\n");
			printf("wireless security  [vapn]  type  [none,wep,wpa,wpa2]\n");
			printf("wireless security  [vapn] mode  [open ,psk,802.1x]\n");
			printf("wireless security  [vapn]  encryption [tkip,aes,wep64,wep128,wep152]\n");
			printf("wireless security  [vapn] [1,2,3,4] [ascii,hex]\n");
			printf("wireless security  [vapn] wpakey [string] \n");
			printf("wireless  show  macfilter [vapn]\n");
			printf("wireless  security [vapn]  macfilter enable[disable]\n");
			printf("wireless  security [vapn]  macfilter mode [permit/deny]\n");
			printf("wireless  security [vapn]  macfilter  add  [macaddr]\n");
			printf("wireless  security [vapn]  macfilter  delmacn \n");
		    printf("wireless  show  radius \n");
			printf("wireless radius authip [ipadress]\n");
			printf("wireless radius accountip [ipadress]\n");			
			printf("wireless radius authport[integer] \n");
			printf("wireless radius account[integer] \n");
			printf("wireless radius sharekey [integer] \n");
            printf("wireless  rogueap enable[disable] \n");
			printf("wireless show rogueap \n");
			printf("wireless config [vapn] wds enable[disable] \n");
			printf("wireless  show  [vapn]  stationlist  \n");
			printf("wireless  show   maxsimultusers [vapn] \n");
			printf("wireless  show   mode [vapn]  \n");
            printf("wireless  show   maxsimultusers [vapn] \n");
			printf("wireless  show   security [vapn]  \n");
			printf("wireless  show   channelmode [vapn]  \n");
			printf("wireless  show   txpower [vapn]  \n");
			printf("wireless  show   isolation [vapn]  \n");
			printf("wireless  show   broadcastssid [vapn]  \n");
			printf("wireless  show   ssid [vapn]  \n");
			printf("wireless  show   vlanenable [vapn]  \n");
			printf("wireless  show   vlanvid [vapn]  \n");
		}
		else if(strcmp(t->words[1],"stp")==0)
		{	  
			printf("stp  show \n");
			printf("stp  config  [vlanname] enale[disable] \n");
			printf("stp config [vlanname] bridgeprio  [integer]  \n");
			printf("stp config [vlanname] setfd [integer] \n");
			printf("stp config [vlanname] maxage  [integer]  \n");
			printf("stp config [vlanname] hello   [integer]  \n");	  
		}		
		#if 0
		else if(strcmp(t->words[1],"vlan")==0)
		{	  
		    printf("vlan  create [vlanname]\n");
			printf("vlan addport [vlanname] [port]\n");
			printf("vlan  destroy  [vlanname]\n");
			printf("vlan  delport  [vlanname] [port]\n");	  
		}
		#endif
		else if(strcmp(t->words[1],"system")==0)
		{	  
            printf("system show version\n");
			printf("system show memory\n");
			printf("system show flashinf\n"); 
			printf("system show runtime\n");
			printf("system show workmode\n");
			printf("system show cpuratio\n"); 
            printf("system show temperature\n");
		    printf("system upgrade [ip_addr] [filename]\n");
			printf("system backup [ip_addr]\n");
			printf("system reset\n");
			printf("system reboot\n");
			printf("system ping [ip_addr]\n");		  
		}
		//#if 0
		else if(strcmp(t->words[1],"snmp")==0)
		{	
		    printf("snmp show\n");
		    printf("snmp enale[disable]\n");
			printf("snmp  syslocation  [string]\n");
			printf("snmp  syscontact  [string]\n");
			printf("snmp  publiccn    [string]\n");
			printf("snmp  privatecn   [string]\n");
			printf("snmp  trapscn   [string]\n");
			printf("snmp  receivetrap  [ipaddr]\n");	  
		}
		//#endif
		else if(strcmp(t->words[1],"syslog")==0)
		{	  
          printf("syslog show\n");
		  printf("syslog enable[diable]\n");
		  printf("syslog  config serverip  [ipaddr]\n");
		  printf("syslog  config serverport [integer]\n");          	  
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   default:
	   	   printf("invalid commad\n");
	   break;
	}	
	printf("\n");
	return EXIT_SUCCESS;
}

static int dowangjr(struct op *t )
{
	printf("hello world\n");
	printf("\n");
	return EXIT_SUCCESS;
}

int is_ipaddr(char *findstr)
{
	char tmp[16]={0};
	char *npos1;
    char *npos2;
	int num=0;
	for( unsigned int i=0;i<=strlen(findstr)-1;i++ )   
	{   
		if((findstr[i]<48)||(findstr[i]>57))
		{
			if(findstr[i]!='.')
			{
				return -1;
			}
		}
        if((findstr[i]=='.')&&(findstr[i+1]=='.'))
		{
			return -1;
		}
        
		if(findstr[i]=='.')
			num++;
	}
	if(num!=3)
		return -1;
	else
	{
		
		npos1 = strstr(findstr,".");
		strncpy(tmp,findstr,(npos1-findstr));
		if((tmp[0]==48)&&(strlen(tmp)!=1))
		{
			return -1;
		}
		if((atoi(tmp)<=0)||(atoi(tmp)>255))
		{
			return -1;
		}
        
		/***the send part****/
		memset(tmp,0,sizeof(tmp));
        npos2= strstr(npos1+1,".");
        strncpy(tmp,npos1+1,(npos2-npos1-1));
		if((tmp[0]==48)&&(strlen(tmp)!=1))
		{
			return -1;
		}
		if((atoi(tmp)<0)||(atoi(tmp)>255))
		{		
			return -1;
		}
		
		/***the third part****/
        memset(tmp,0,sizeof(tmp));
        npos1= strstr(npos2+1,".");
        strncpy(tmp,npos2+1,(npos1-npos2-1));
		if((tmp[0]==48)&&(strlen(tmp)!=1))
		{
			return -1;
		}
		if((atoi(tmp)<0)||(atoi(tmp)>255))
		{		
			return -1;
		}
		
       	/***the forth part****/
		memset(tmp,0,sizeof(tmp));
		
		if(strcmp(npos1+1,"")!=0)
		{
			strncpy(tmp,npos1+1,findstr+strlen(findstr)-1-npos1);
			if((tmp[0]==48)&&(strlen(tmp)!=1))
		    {
			    return -1;
		    }
			if((atoi(tmp)<0)||(atoi(tmp)>255))
			{		
				return -1;
			}
		}
		else
		{
			return -1;
		}
		
        return 0;
	}

}



static int donetwork(struct op *t )
{
  int len=0;
  int i=0;
  

  while(t->words[i]!=NULL)
  {
      i++;
      len++;
  }
  switch (len) 
   {
       case 3:
	   	if(strcmp(t->words[2],"up")==0)
		{
		  network_set_ifstate(t->words[1],"up");
		}
		else if(strcmp(t->words[2],"down")==0)
		{
		  network_set_ifstate(t->words[1],"down");
		}
		else if(strcmp(t->words[1],"show")==0)
		{
		  if(strcmp(t->words[2],"dnsnameserver")==0)
		  {
		      char dnsmode[10]={0};
			  char dnsnameser[20]={0};
		      network_get_dnsmode(dnsmode,10);
			  printf("dnsmode=%s\n",dnsmode);
			  if(strcmp(dnsmode,"static")==0)
			  {
			      network_get_dns((char *)dnsnameser,1,20);
			      printf("dnsnameserver1=%s\n",dnsnameser);
			      memset(dnsnameser,0,sizeof(dnsnameser));
			      network_get_dns((char *)dnsnameser,2,20);
			      printf("dnsnameserver2=%s\n",dnsnameser);
			  }
		  }
		  else if(strcmp(t->words[2],"ntpserver")==0)
		  {
			  char ntpserver[20]={0};
			  network_get_ntp((char*)ntpserver,1,20);
			  printf("ntpserver1=%s\n",ntpserver);
			  network_get_dns(ntpserver,2,20);
			  printf("ntpserver2=%s\n",ntpserver);
		  }
		  else if(strcmp(t->words[2],"ipinfo")==0)
		  {
             
			 char szVal[50]={0};
			 network_get_ip(szVal,50);
             printf("ip addr :%s\n",szVal);			 
			 printf("Mask:255.255.255.0\n");
	         memset(szVal, 0x00, sizeof(szVal));	
			 network_get_gw(szVal,50);
			 printf("gateway:%s\n",szVal);	
			
			char str_tmp_cmd[200]={0};
	    	FILE   *stream; 
		    memset(szVal, 0x00, sizeof(szVal));	
			strcpy(str_tmp_cmd,"/sbin/ifconfig default|/bin/grep -o -i -e '[0-9a-fA-F]\\{1,2\\}:[0-9a-fA-F]\\{1,2\\}:[0-9a-fA-F]\\{1,2\\}:[0-9a-fA-F]\\{1,2\\}:[0-9a-fA-F]\\{1,2\\}:[0-9a-fA-F]\\{1,2\\}'");
			stream = popen( str_tmp_cmd, "r" );
			fread( szVal, sizeof(char), sizeof(szVal),  stream); 
			fclose( stream );
		    szVal[strlen(szVal)-1]='\0';
			printf("mac addr: %s\n",szVal);
			
		  }
          else if(strcmp(t->words[2],"status")==0)
          {

		  	FILE   *stream; 
			char szVal[50]={0};
			char str_status[50]={0};
			char str_tmp_cmd[100]={0};
		    memset(szVal, 0x00, sizeof(szVal));	
			strcpy(str_tmp_cmd,"{ /sbin/ifconfig default| grep 'UP' ; echo $? ; }");
			stream = popen( str_tmp_cmd, "r" );
			fread( szVal, sizeof(char), sizeof(szVal),  stream); 
			fclose( stream );
		    szVal[strlen(szVal)-1]='\0';
		    if(0==atoi(szVal))
			    strcpy(str_status,"Up");
			else if(1==atoi(szVal)) 	
		        strcpy(str_status,"Disable");
			 printf("%s\n",str_status);
          }

		  else if(strcmp(t->words[2],"rate")==0)
          {

		  	FILE   *stream; 
			 char szVal[50]={0};
			 char str_status[50]={0};
			 char str_tmp_cmd[100]={0};
		    memset(szVal, 0x00, sizeof(szVal));	
			sprintf(str_tmp_cmd,"/usr/sbin/ethtool eth0 |grep  -i 'speed'|/usr/bin/cut -d: -f2");
			stream = popen( str_tmp_cmd, "r" );
			fread( szVal, sizeof(char), sizeof(szVal),  stream); 
			fclose( stream );
		    szVal[strlen(szVal)-1]='\0';
			
			printf("eth0 rate:%s\n",szVal);
             memset(str_tmp_cmd, 0x00, sizeof(str_tmp_cmd));
			sprintf(str_tmp_cmd,"/usr/sbin/ethtool eth1 |grep  -i 'speed'|/usr/bin/cut -d: -f2");
			stream = popen( str_tmp_cmd, "r" );
			fread( szVal, sizeof(char), sizeof(szVal),  stream); 
			fclose( stream );
		    szVal[strlen(szVal)-1]='\0';
			printf("eth1 rate:%s\n",szVal);
			 
          }
		  else
		  {
              printf("invalid commad\n");		  
		  }
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   case 4:
	   	if(strcmp(t->words[2],"ip")==0)
		{
		  if(is_ipaddr(t->words[3])==0)
		  {
		      network_set_ip(t->words[1],t->words[3],"255.255.0.0");
		  }
		  else
		  {
		      printf("please input the correct ip \n");
		  }
		}
		else if(strcmp(t->words[2],"ipmode")==0)
		{
		   
		   if(strcmp(t->words[3],"dhcp")==0)
		   {
		   	    network_set_ipmode("dhcp");
		   }
		   else if(strcmp(t->words[3],"static")==0)
		   {
		   	    network_set_ipmode("static");
		   }
		   else
		   {
		      printf("input the correct ipmode [dhcp|static]\n");
		   }
		   	
		}
		else if(strcmp(t->words[2],"dnsmode")==0)
		{

		   if(strcmp(t->words[3],"dhcp")==0)
		   {
		   	    network_set_dnsmode("dhcp");
		   }
		   else if(strcmp(t->words[3],"static")==0)
		   {
		   	    network_set_dnsmode("static");
		   }
		   else
		   {
		      printf("input the correct dnsmode [dhcp|static]\n");
		   }
		
		}
		else if(strcmp(t->words[2],"dnsnameserver1")==0)
		{

		    if(is_ipaddr(t->words[3])==0)
			{
			    network_set_dns(t->words[3],1);
			}
			else
			{
			    printf("please input the dnsnameserver addr \n");
		    }
		}
		else if(strcmp(t->words[2],"dnsnameserver2")==0)
		{
		    if(is_ipaddr(t->words[3])==0)
			{
			    network_set_dns(t->words[3],2);
			}
			else
			{
			    printf("please input the dnsnameserver addr \n");
			}
		}
		else if(strcmp(t->words[2],"ntpmode")==0)
		{
           if((strcmp(t->words[3],"yes")==0)||(strcmp(t->words[3],"no")==0))
		   {
		       network_set_ntpmode(t->words[3]);
		   }		
		   else
		   {
		      printf("input the correct ntpmode [yes|no] \n");
		   }
		   
           #if 0 
		   if(strcmp(t->words[3],"1")==0)
		   {
		       network_set_ntpmode("yes");
		   }
		   else if(strcmp(t->words[3],"0")==0)
		   {
		       network_set_ntpmode("no");
		   }
		   else
		   {
		      printf("input the correct ntpmode [1|0] \n");
		   }
		   #endif
		}
		else if(strcmp(t->words[2],"ntpserver1")==0)
		{

		  	if(is_ipaddr(t->words[3])==0)
			{
			    network_set_ntp(t->words[3],1);
			}
			else
			{
			    printf("please input the ntpserver addr \n");
			}
		}
	    else if(strcmp(t->words[2],"ntpserver2")==0)
		{

			if(is_ipaddr(t->words[3])==0)
			{
			    network_set_ntp(t->words[3],2);
			}
			else
			{
			    printf("please input the ntpserver addr \n");
			}		   	
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
       case 5:
	   	if((strcmp(t->words[1],"route")==0)&&
		   (strcmp(t->words[2],"add")==0)&&
		   (strcmp(t->words[3],"default")==0))
		{

			if(is_ipaddr(t->words[4])==0)
			{
			    network_set_gw(t->words[4]);
			}
			else
			{
			    printf("please input the gateway addr \n");
			}			  
		}
		else
		{
		   printf("invalid commad\n");
		}	   	
	   break;	
	   case 6:
	   	if((strcmp(t->words[2],"ip")==0)&&
		  (strcmp(t->words[4],"netmask")==0))
		{

         	if((is_ipaddr(t->words[3])==0)&&(is_ipaddr(t->words[5])==0))
			{
			    network_set_ip(t->words[1],t->words[3],t->words[5]);
			}
			else
			{
			    printf("please input the ip ot netmask \n");
			}	
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   default:
	   	   printf("invalid commad\n");
	   break;
	}
	printf("\n");	
	return EXIT_SUCCESS;
}

/*************************************/
/*******is_macaddr*********************/
/*******check the correct mac addr*****/
/*******add  by wangjianru ************/
/*************************************/

int is_macaddr(char *findstr)
{
	if(strlen(findstr)!=17)
	{
		return -1;
	}
	
    for(int i=0; i<strlen(findstr);i++)
	{
		if((i+1)%3==0)
		{
			if(findstr[i]!=':')
				return -1;
		}
		else
		{
			if(((findstr[i]<65)||(findstr[i]>90))
				&&((findstr[i]<97)||(findstr[i]>122))
				&&((findstr[i]<48)||(findstr[i]>57))
				)
			{
				return -1;				
			}	
		}
	}
	return 0;
}

/*************add by wangjianru**************/


int is_number(char *findstr)
{
		
    for(int i=0;i<strlen(findstr);i++)
	{
	    if(i == 0)
		{
			if(findstr[i]==48)
				return -1;
				
		}
		if((findstr[i]<48)||(findstr[i]>57))
			return -1;
	}
	return 0;
}



/***********wireless************

include the basic wireless setup 
and security setup

********************************/


static int dowireless(struct op *t )
{
  int len=0;
  int i=0;
  while(t->words[i]!=NULL)
  {
      i++;
      len++; 
  }
  switch (len) 
   {
       case 3:
	   	if(strcmp(t->words[1],"destroy")==0)
	   	{

	   	  if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	  {
	   	     wireless_destroy_vap(t->words[2]);
	   	  }
		  else
		  {
		      printf("input the correct wlanport [athX]\n");
		  }
	   	}
		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"status")==0))
		{
		   system("/sbin/iwconfig");
		}
		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"radius")==0))
		{
           char tmp[20]={0};
		   wireless_radius_get_authip("1",tmp,20);
		   printf("radius authip= %s\n",tmp);
		   memset(tmp,0,sizeof(tmp));
		   wireless_radius_get_acctip("1",tmp,20);
		   printf("radius accountip= %s\n",tmp);
		   memset(tmp,0,sizeof(tmp));
		   wireless_radius_get_authport("1",tmp,20);
		   printf("radius authport= %s\n",tmp);
           memset(tmp,0,sizeof(tmp));
		   wireless_radius_get_acctport("1",tmp,20);
		   printf("radius accountport= %s\n",tmp);
		   memset(tmp,0,sizeof(tmp));
           wireless_radius_get_sharekey( "1",tmp,20);
		   printf("radius sharekey= %s\n",tmp);
		}
		else if(strcmp(t->words[1],"rogueap")==0)
		{
		   printf("rogueap %s\n",t->words[2]); 
		}
		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"rogueap")==0))
		{
		   printf("show rogueap %s\n",t->words[2]); /***show rogueap****/
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   case 4:
	   	if(strcmp(t->words[1],"create")==0)
	   	{
		   wireless_create_vap(t->words[2],t->words[3]); /*****create a new athx******/
	   	}	
		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"security")==0))
		{
		    char str_tmp[64]={0};
            wireless_sec_get_type(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
		    printf("wireless security type %s\n",str_tmp);
		    if((strcmp(str_tmp,"wpa")==0)||(strcmp(str_tmp,"wpa2")==0))
		    {
		        memset(str_tmp, 0, 64);
		        wireless_sec_get_wpakey(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
			    printf("wireless security wpakey %s\n",str_tmp);
		    }
		    else if(strcmp(str_tmp,"wep")==0)
		    {
		        memset(str_tmp, 0, 64);
			    char str_seq[3]={0};
			  
                wireless_sec_get_wepkey_seq( get_vnum_by_vname(t->words[3]),
                                 (char *)str_seq,
                                 3 );	
		        wireless_sec_get_wepkey( get_vnum_by_vname(t->words[3]),
                             str_seq,
                             (char *)str_tmp,
                             64);
			    printf("wireless wep key %s\n",str_tmp);
	 
		    }
		  
		    memset(str_tmp, 0, 64);
		    wireless_sec_get_mode(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
		    printf("wireless security mode %s\n",str_tmp);
		    memset(str_tmp, 0, 64);
		    wireless_sec_get_encrypt(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
		    printf("wireless security encrypt %s\n",str_tmp);
		  
		}
		else if((strcmp(t->words[1],"radius")==0)&&
			(strcmp(t->words[2],"authip")==0))
		{
           if(is_ipaddr(t->words[3])==0)
           {
		       wireless_radius_set_authip("1",t->words[3]);
           }
		   else
		   {
		      printf("please input the correct ip addr");
		   }
		}
		else if((strcmp(t->words[1],"radius")==0)&&
			(strcmp(t->words[2],"accountip")==0))
		{

           if(is_ipaddr(t->words[3])==0)
           {
		       wireless_radius_set_acctip("1",t->words[3]);
           }
		   else
		   {
		      printf("please input the correct ip addr");
		   }

		}
		else if((strcmp(t->words[1],"radius")==0)&&
			(strcmp(t->words[2],"authport")==0))
		{
		   if(is_number(t->words[3])==0)
           {
		       wireless_radius_set_authport("1",t->words[3]);
           }
		   else
		   {
		      printf("please input the correct authport");
		   }		    
		}
		else if((strcmp(t->words[1],"radius")==0)&&
			(strcmp(t->words[2],"accountport")==0))
		{
		   if(is_number(t->words[3])==0)
           {
		       wireless_radius_set_acctport("1",t->words[3]);
           }
		   else
		   {
		      printf("please input the correct acctport");
		   }		   
		}
		else if((strcmp(t->words[1],"radius")==0)&&
			(strcmp(t->words[2],"sharekey")==0))
		{
		   
		   wireless_radius_set_sharekey("1",t->words[3]);/***radius sharekey  ****/
		}
		else if((strcmp(t->words[1],"radiusb")==0)&&
			(strcmp(t->words[2],"authip")==0))
		{

		    if(is_ipaddr(t->words[3])==0)
           {
		       wireless_radius_set_authip("0",t->words[3]);
           }
		   else
		   {
		      printf("please input the correct ip addr");
		   }
 
		}
		else if((strcmp(t->words[1],"radiusb")==0)&&
			(strcmp(t->words[2],"accountip")==0))
		{
		   
		   if(is_ipaddr(t->words[3])==0)
           {
		       wireless_radius_set_acctip("0",t->words[3]);
           }
		   else
		   {
		      printf("please input the correct ip addr");
		   }
		}
        else if((strcmp(t->words[1],"radiusb")==0)&&
			(strcmp(t->words[2],"authport")==0))
		{
		   if(is_number(t->words[3])==0)
           {
		       wireless_radius_set_authport("0",t->words[3]);
           }
		   else
		   {
		      printf("please input the correct authport");
		   } 
		}
		else if((strcmp(t->words[1],"radiusb")==0)&&
			(strcmp(t->words[2],"accountport")==0))
		{
			if(is_number(t->words[3])==0)
			{
				wireless_radius_set_acctport("0",t->words[3]);
			}
			else
			{
			   printf("please input the correct acctport");
			}	 

		}
		else if((strcmp(t->words[1],"radiusb")==0)&&
			(strcmp(t->words[2],"sharekey")==0))
		{
		   wireless_radius_set_sharekey("0",t->words[3]);
		}
		
		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"macfilter")==0))
		{
		    if((strlen(t->words[3])==4)&&
		  	 t->words[3][0]==97&&
		  	 t->words[3][1]==116&&
		  	 t->words[3][2]==104&&
		  	 t->words[3][3]>=48&&
		  	 t->words[3][3]<=57
		  	)
		    {
		      
			    char str_tmp[30]={0};
				char str_macn[10]={0};
			    wireless_get_macfilter(get_vnum_by_vname(t->words[3]),str_tmp,30);
				printf("macfilter enable %s \n",str_tmp);
				memset(str_tmp, 0, sizeof(str_tmp));
			    wireless_get_macfiltermode(get_vnum_by_vname(t->words[3]),str_tmp,30);
				printf("macfilter mode %s \n",str_tmp);
				for( i=1;i<=20;i++)
				{  
				  memset(str_macn, 0, sizeof(str_macn));
	              memset(str_tmp, 0, sizeof(str_tmp));
				  sprintf(str_macn,"%s%d","mac",i);
				  wireless_sec_get_macfilter(get_vnum_by_vname(t->words[3]),str_macn,str_tmp,30);
				 // if(strlen(str_tmp)!=0)
                  //{
				  printf("mac%d=%s\n",i,str_tmp);
                  //}
				}
		    }
			else
			{
			    printf("input the correct wlanport[athX]\n");
			}
		}
		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[3],"stationlist")==0))
		{
		   if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
		   {
  		       char str_cmd[50]={0};
  			   sprintf(str_cmd,"%s %s %s","wlanconfig",t->words[2],"list");
  		       system(str_cmd);
		   }
		   else
		   {
		        printf("input the correct wlanport[athX]\n");
		   } 
		}

		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"maxsimultusers")==0))
		{
		    char str_tmp[64]={0};
            wireless_get_maxsimultusers(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
			printf("maxsimultusers: %d\n",atoi(str_tmp));

		} 
		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"mode")==0))
		{
		    char str_tmp[64]={0};
            wireless_get_mode(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
			printf("mode: %s\n",str_tmp);

		} 
		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"channelmode")==0))
		{
		    char str_tmp[64]={0};
            wireless_get_channel(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
			if(atoi(str_tmp) == 0)
			{
			    printf("channel mode: autochannel");
			}
			else
			{
			    printf("channel mode: %s\n",str_tmp);
			}
		} 

	    else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"channelmode")==0))
		{
		    char str_tmp[64]={0};
            wireless_get_channel(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
			if(atoi(str_tmp) == 0)
			{
			    printf("channel mode: autochannel");
			}
			else
			{
			    printf("channel mode: %s\n",str_tmp);
			}
		} 

		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"txpower")==0))
		{
		    char str_tmp[64]={0};
            wireless_get_txpower(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
			printf("txpower: %s\n",str_tmp);
		} 
		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"isolation")==0))
		{
		    char str_tmp[64]={0};
            wireless_get_isolation(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
			printf("isolation: %s\n",str_tmp);
		}
		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"broadcastssid")==0))
		{
		    char str_tmp[64]={0};
            wireless_get_broadcastssid(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
			printf("broadcastssid:%s\n",str_tmp);
			
		}
        else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"ssid")==0))
		{
		    char str_tmp[64]={0};
            wireless_get_ssid(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
			printf("ssid:%s\n",str_tmp);
			
		}

	    else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"vlanenable")==0))
		{
		    char str_tmp[64]={0};
            wireless_get_vlan(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
			printf("vlanenable:%s\n",str_tmp);
			
		}
		else if((strcmp(t->words[1],"show")==0)&&
			(strcmp(t->words[2],"vlanvid")==0))
		{
			char str_tmp[64]={0};
            wireless_get_vlanvid(get_vnum_by_vname(t->words[3]),(char *)str_tmp,64);
			printf("vid:%s\n",str_tmp);
			
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
       case 5:
	    if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"ssid")==0))
	   	{
            if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
	   	       if(strlen(t->words[4])<=32)
	   	       {
	   	           wireless_config_ssid(get_vnum_by_vname(t->words[2]),t->words[4]);
	   	       }
			   else
			   {
			       printf("input the correct ssid the max 32\n");
			   }
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }
	   	}
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"hide_ssid")==0))	
		{
		    if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
	   	       if((strcmp(t->words[4],"0")==0)||
			   	(strcmp(t->words[4],"1")==0))
	   	       {
	   	           wireless_config_hidessid(get_vnum_by_vname(t->words[2]),t->words[4]);
	   	       }
			   else
			   {
			       printf("input the correct argument[0|1]\n");
			   }
			   
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }	
		}
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"channel")==0))
		{
		   if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
	   	       if((atoi(t->words[4])>=0)&&
			   	(atoi(t->words[4])<=13))
	   	       {
	   	           wireless_config_channel(get_vnum_by_vname(t->words[2]),t->words[4]);
	   	       }
			   else
			   {
			       printf("input the correct channel[1~13]\n");
			   }
			   
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }	
		    	
		}
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"mode")==0))
		{

		  if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
	   	       if((strcmp(t->words[4],"11a")==0)||
			   	(strcmp(t->words[4],"11b")==0)||
			   	(strcmp(t->words[4],"11g")==0)
			   	)
	   	       {
	   	           wireless_config_mode(get_vnum_by_vname(t->words[2]),t->words[4]);
	   	       }
			   else
			   {
			       printf("input the correct mode[11a|11b|11g]\n");
			   }
			   
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }	
		}
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"rate")==0))
		{
          if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
	   	       if((strcmp(t->words[4],"auto")==0)||
			   	  (strcmp(t->words[4],"1M")==0)||
			   	  (strcmp(t->words[4],"2M")==0)||
			   	  (strcmp(t->words[4],"5.5M")==0)||
			   	  (strcmp(t->words[4],"6M")==0)||
			   	  (strcmp(t->words[4],"9M")==0)||
			   	  (strcmp(t->words[4],"11M")==0)||
			   	  (strcmp(t->words[4],"12M")==0)||
			   	  (strcmp(t->words[4],"18M")==0)||
			   	  (strcmp(t->words[4],"24M")==0)||
			   	  (strcmp(t->words[4],"36M")==0)||
			   	  (strcmp(t->words[4],"48M")==0)||
			   	  (strcmp(t->words[4],"54M")==0)
			   	)
	   	       {
	   	           wireless_config_rate(get_vnum_by_vname(t->words[2]),t->words[4]);
	   	       }
			   else
			   {
			      printf("input the correct rate [auto|1M|2M|5.5M|6M|9M|11M|12M|18M|24M|36M|48M|54M]\n");
			   }
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }	
		}
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"bintval")==0))
		{
  		    if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
				if((is_number(t->words[4])==0)&&
				(atoi(t->words[4])>=25)&&
				 (atoi(t->words[4])<=1000))

	   	       {
	   	           wireless_config_bintval(get_vnum_by_vname(t->words[2]),t->words[4]);
	   	       }
			   else
			   {
			       printf("input the correct bintval [25~1000]\n");
			   }
			   
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }  
		}
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"dtim_period")==0))
		{

  		    if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
				if((is_number(t->words[4])==0)&&
				 (atoi(t->words[4])>=1)&&
				 (atoi(t->words[4])<=15))

	   	       {
	   	           wireless_config_dtim(get_vnum_by_vname(t->words[2]),t->words[4]);
	   	       }
			   else
			   {
			       printf("input the correct bintval [1~15]\n");
			   }
			   
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }  

		}
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"frag")==0))
		{
           if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
				if((is_number(t->words[4])==0)&&
				 (atoi(t->words[4])>=256)&&
				 (atoi(t->words[4])<=2346))
	   	       {
	   	            wireless_config_frag(get_vnum_by_vname(t->words[2]),t->words[4]);
	   	       }
			   else
			   {
			       printf("input the correct frag [256~2346]\n");
			   }
			   
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   } 
		}
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"txpower")==0))
		{

  		    if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
				if((is_number(t->words[4])==0)&&
				  (atoi(t->words[4])>=1)&&
				  (atoi(t->words[4])<=18))
	   	       {
	   	           wireless_config_txpower(get_vnum_by_vname(t->words[2]),t->words[4]);
	   	       }
			   else
			   {
			       printf("input the correct txpower [1~18]\n");
			   }
			   
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }  		   
		}
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"rts")==0))
		{

		  if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
				if((is_number(t->words[4])==0)&&
				 (atoi(t->words[4])>=1)&&
				 (atoi(t->words[4])<=2346))
	   	       {
	   	            wireless_config_rts(get_vnum_by_vname(t->words[2]),t->words[4]);
	   	       }
			   else
			   {
			       printf("input the correct rts [1~2346]\n");
			   }
			   
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   } 
		  
		}
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"wmm")==0))
		{

			if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
	   	       if((strcmp(t->words[4],"0")==0)||(strcmp(t->words[4],"1")==0))

	   	           wireless_config_wmm(get_vnum_by_vname(t->words[2]),t->words[4]);
			   else
			       printf("input the correct wmm[0|1]\n");		   
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }
		}
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"vlan")==0))
		{
		   if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
	   	       if((strcmp(t->words[4],"enable")==0)||(strcmp(t->words[4],"disable")==0))
		          wireless_config_vlan(get_vnum_by_vname(t->words[2]),t->words[4]);
			   else
			      printf("input the correct argument[enable|diable]\n");
		   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }
		}
        else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"vlanvid")==0))
		{
		   if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
		       wireless_config_vlanvid(get_vnum_by_vname(t->words[2]),t->words[4]);
		   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }
		}		
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"noackpolicy")==0))
		{

		  if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
	   	       if((strcmp(t->words[4],"0")==0)||
			   	(strcmp(t->words[4],"1")==0))
	   	       {
	   	          wireless_config_noack(get_vnum_by_vname(t->words[2]),t->words[4]);
	   	       }
			   else
			   {
			       printf("input the correct noackpolicy[0|1]\n");
			   }		   
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }
		   
		}	
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"isolation")==0))
		{
			if((strlen(t->words[2])==4)&&
			   t->words[2][0]==97&&
			   t->words[2][1]==116&&
			   t->words[2][2]==104&&
			   t->words[2][3]>=48&&
			   t->words[2][3]<=57
			  )
			 {
				 if((strcmp(t->words[4],"1")==0)||(strcmp(t->words[4],"0")==0))
				 {
					wireless_config_isolation(get_vnum_by_vname(t->words[2]),t->words[4]);
				 }
				 else
				 {
					 printf("input the correct isolation[1|0]\n");
				 }			 
			 }
			 else
			 {
				 printf("input the correct wlanport[athX]\n");
			 }

		}	
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"maxsimultusers")==0))
		{
			if((strlen(t->words[2])==4)&&
			   t->words[2][0]==97&&
			   t->words[2][1]==116&&
			   t->words[2][2]==104&&
			   t->words[2][3]>=48&&
			   t->words[2][3]<=57
			  )
			 {
				// if(strcmp(t->words[4],"1")
				 //{
					wireless_config_maxsimultusers(get_vnum_by_vname(t->words[2]),t->words[4]);
				 //}
				// else
				// {
					// printf("input the correct maxsimultusers[1|0]\n");
				 //}			 
			 }
			 else
			 {
				 printf("input the correct wlanport[athX]\n");
			 }

		}
		else if((strcmp(t->words[1],"security")==0)&&
			(strcmp(t->words[3],"type")==0))
		{
			if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
			  if((strcmp(t->words[4],"none")==0)||
			  	 (strcmp(t->words[4],"wep")==0)||
			  	 (strcmp(t->words[4],"wpa")==0)||
			  	 (strcmp(t->words[4],"wpa2")==0))
			  {
			      wireless_sec_set_type(get_vnum_by_vname(t->words[2]),t->words[4]);
			  }
			  else
			  {
			       printf("please config wlanport security type[none|wep|wpa|wpa2]\n");
			  }
		  }
		  else
		  {

		      printf("input the correct wlanport[athX]\n");
		  }
		}
		else if((strcmp(t->words[1],"security")==0)&&
			(strcmp(t->words[3],"mode")==0))
		{
		   	if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
		       if((strcmp(t->words[4],"open")==0)||
			  	  (strcmp(t->words[4],"psk")==0)||
			  	  (strcmp(t->words[4],"802.1x")==0)
			  	 )
			   {
			       wireless_sec_set_mode(get_vnum_by_vname(t->words[2]),t->words[4],1);
			   }
			   else
			   {
			       printf("please config wlanport security mode[open|psk|802.1x]\n");
			   }
		   }
		   else
		   {

		       printf("input the correct wlanport[athX]\n");
		   }		   
		}
		else if((strcmp(t->words[1],"security")==0)&&
			(strcmp(t->words[3],"encryption")==0))
		{
		   	if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
		       if((strcmp(t->words[4],"tkip")==0)||
			  	  (strcmp(t->words[4],"aes")==0)||
			  	  (strcmp(t->words[4],"wep64")==0)||
			  	  (strcmp(t->words[4],"wep128")==0)||
			  	  (strcmp(t->words[4],"wep152")==0)
			  	 )
			   {
			      
			       wireless_sec_set_encrypt(get_vnum_by_vname(t->words[2]),t->words[4],1);
			   }
			   else
			   {
			       printf("please config wlanport security mode[tkip|aes|wep64|wep128|wep152]\n");
			   }
		   }
		   else
		   {

		       printf("input the correct wlanport[athX]\n");
		   }
		}
		else if((strcmp(t->words[1],"security")==0)&&
			(strcmp(t->words[3],"wpakey")==0))
		{	
		  	if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
		   {
			   if((strlen(t->words[4])>=8)&&(strlen(t->words[4])<=63))
			   {
			       wireless_sec_set_wpakey(get_vnum_by_vname(t->words[2]),t->words[4],1);/****set the wpa passphrase******/
			   }
			   else
			   {
			      printf("input the correct wpa PassPhrase [8..63]\n");
			   }
		  }
		  else
		  {
		      printf("input the correct wlanport[athX]\n");
		  }
		}
		else if((strcmp(t->words[1],"security")==0)&&
			(strcmp(t->words[3],"macfilter")==0))
		{
			if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   { 		   
			   if (strstr(t->words[4],"delmac")!=NULL)
			   {
			       wireless_security_filter_delmac(get_vnum_by_vname(t->words[2]),t->words[4]);
			   }
			   else if((strcmp(t->words[4],"enable")==0)||(strcmp(t->words[4],"disable")==0))
			   {
			       wireless_security_macfilter(get_vnum_by_vname(t->words[2]),t->words[4]);
			   }
			   else
			   {
			     printf("invalid commad\n");
			   }
		  }
		  else
		  {
               printf("input the correct wlanport[athX]\n");		  
		  }
		}
		else if((strcmp(t->words[1],"config")==0)&&
			(strcmp(t->words[3],"wds")==0))
		{
		   //printf("config%s wds %s\n",t->words[2],t->words[4]);
		   printf(" ----------config wds---------- ");
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   case 6:	
	   	if((strcmp(t->words[1],"security")==0)&&
		  (strcmp(t->words[3],"macfilter")==0)&&
		  (strcmp(t->words[4],"mode")==0))
	   	{
	   	   if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {   
	   	       if((strcmp(t->words[5],"permit")==0)||(strcmp(t->words[5],"prevent")==0))
	   	       {
	   	           wireless_security_filtermode(get_vnum_by_vname(t->words[2]),t->words[5]);
	   	       }
			   else
			   {
			       printf("please config the filtermode[permit|prevent]\n");
			   }
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");	   
		   }		   
	   	}
		else if((strcmp(t->words[1],"security")==0)&&
		  (strcmp(t->words[3],"macfilter")==0)&&
		  (strstr(t->words[4],"addmac")!=NULL))
		{ 

		   if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {
				if(is_macaddr(t->words[5])==0)
	   	       {
	   	            wireless_security_filter_addmac(get_vnum_by_vname(t->words[2]),t->words[4],t->words[5]);
	   	       }
			   else
			   {
			       printf("please input correct mac addr[xx:xx:xx:xx:xx:xx]\n");
			   }
			   
	   	   }
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }          
		}
		else if((strcmp(t->words[1],"security")==0)&&
		  (strcmp(t->words[3],"wepkey[1]")==0)&&
		  (strcmp(t->words[4],"ascii")==0))
		{
			if((strlen(t->words[2])==4)&&
				 t->words[2][0]==97&&
				 t->words[2][1]==116&&
				 t->words[2][2]==104&&
				 t->words[2][3]>=48&&
				 t->words[2][3]<=57
				)
			   {	 
				   char str_type[20]={0};
				   wireless_sec_get_encrypt(get_vnum_by_vname(t->words[2]),str_type,8);
				   if((strstr(str_type,"64")!=NULL)&&
					  (strlen(t->words[5])==5))
				   {	
					   wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"1","ascii",t->words[5],1);
				   }
				   else if((strstr(str_type,"128")!=NULL)&&
						(strlen(t->words[5])==13))
				   {
					   wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"1","ascii",t->words[5],1);
				   }
				   else if((strstr(str_type,"152")!=NULL)&&
						(strlen(t->words[5])==16))
				   {
					 wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"1","ascii",t->words[5],1);
				   }
				   else
				   {
					  printf("input the correct password number\n");
				   }
				}
			   else
			   {
				   printf("input the correct wlanport[athX]\n");
			   }

		}
		else if((strcmp(t->words[1],"security") == 0)&&
		  (strcmp(t->words[3],"wepkey[1]") == 0)&&
		  (strcmp(t->words[4],"hex") == 0))
		{
		  	if((strlen(t->words[2])==4)&&
		  	 t->words[2][0]==97&&
		  	 t->words[2][1]==116&&
		  	 t->words[2][2]==104&&
		  	 t->words[2][3]>=48&&
		  	 t->words[2][3]<=57
		  	)
	   	   {     
	   	       char str_type[20]={0};			  
			   wireless_sec_get_encrypt(get_vnum_by_vname(t->words[2]),str_type,8);;
			   if((strstr(str_type,"64")!=NULL)&&
			   	  (strlen(t->words[5])==10))
			   {	
		           wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"1","hex",t->words[5],1);
			   }
			   else if((strstr(str_type,"128")!=NULL)&&
			   	    (strlen(t->words[5])==26))
			   {
			       wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"1","hex",t->words[5],1);
			   }
			   else if((strstr(str_type,"152")!=NULL)&&
			   	    (strlen(t->words[5])==32))
			   {
			     wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"1","hex",t->words[5],1);
			   }
			   else
			   {
			      printf("input the correct password number\n");
			   }
			}
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }		
		}
		else if((strcmp(t->words[1],"security") == 0)&&
		  (strcmp(t->words[3],"wepkey[2]") == 0)&&
		  (strcmp(t->words[4],"ascii") == 0))
		{
       
		     if((strlen(t->words[2])==4)&&
				 t->words[2][0]==97&&
				 t->words[2][1]==116&&
				 t->words[2][2]==104&&
				 t->words[2][3]>=48&&
				 t->words[2][3]<=57
				)
			   {	 
				   char str_type[20]={0};
				   wireless_sec_get_encrypt(get_vnum_by_vname(t->words[2]),str_type,8);
				   if((strstr(str_type,"64")!=NULL)&&
					  (strlen(t->words[5])==5))
				   {	
					   wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"2","ascii",t->words[5],1);
				   }
				   else if((strstr(str_type,"128")!=NULL)&&
						(strlen(t->words[5])==13))
				   {
					   wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"2","ascii",t->words[5],1);
				   }
				   else if((strstr(str_type,"152")!=NULL)&&
						(strlen(t->words[5])==16))
				   {
					 wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"2","ascii",t->words[5],1);
				   }
				   else
				   {
					  printf("input the correct password number\n");
				   }
				}
			   else
			   {
				   printf("input the correct wlanport[athX]\n");
			   }
		}
		else if((strcmp(t->words[1],"security") == 0)&&
		  (strcmp(t->words[3],"wepkey[2]") == 0)&&
		  (strcmp(t->words[4],"hex") == 0))
		{ 
		     if((strlen(t->words[2])==4)&&
				 t->words[2][0]==97&&
				 t->words[2][1]==116&&
				 t->words[2][2]==104&&
				 t->words[2][3]>=48&&
				 t->words[2][3]<=57
				)
			{		
		   	   char str_type[20]={0};
	   	       wireless_sec_get_encrypt(get_vnum_by_vname(t->words[2]),str_type,8);
			   if((strstr(str_type,"64")!=NULL)&&
			   	  (strlen(t->words[5])==10))
			   {	
		           wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"2","hex",t->words[5],1);
			   }
			   else if((strstr(str_type,"128")!=NULL)&&
			   	    (strlen(t->words[5])==26))
			   {
			       wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"2","hex",t->words[5],1);
			   }
			   else if((strstr(str_type,"152")!=NULL)&&
			   	    (strlen(t->words[5])==32))
			   {
			     wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"2","hex",t->words[5],1);
			   }
			   else
			   {
			      printf("input the correct password number\n");
			   }
			}
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }
		}
		else if((strcmp(t->words[1],"security")==0)&&
		  (strcmp(t->words[3],"wepkey[3]")==0)&&
		  (strcmp(t->words[4],"ascii") == 0))
		{
       
		    	if((strlen(t->words[2])==4)&&
				 t->words[2][0]==97&&
				 t->words[2][1]==116&&
				 t->words[2][2]==104&&
				 t->words[2][3]>=48&&
				 t->words[2][3]<=57
				)
			   {	 
				   char str_type[20]={0};
				   wireless_sec_get_encrypt(get_vnum_by_vname(t->words[2]),str_type,8);
				   if((strstr(str_type,"64")!=NULL)&&
					  (strlen(t->words[5])==5))
				   {	
					   wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"3","ascii",t->words[5],1);
				   }
				   else if((strstr(str_type,"128")!=NULL)&&
						(strlen(t->words[5])==13))
				   {
					   wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"3","ascii",t->words[5],1);
				   }
				   else if((strstr(str_type,"152")!=NULL)&&
						(strlen(t->words[5])==16))
				   {
					 wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"3","ascii",t->words[5],1);
				   }
				   else
				   {
					  printf("input the correct password number\n");
				   }
				}
			   else
			   {
				   printf("input the correct wlanport[athX]\n");
			   }
		}
		else if((strcmp(t->words[1],"security")==0)&&
		  (strcmp(t->words[3],"wepkey[3]")==0)&&
		  (strcmp(t->words[4],"hex")==0))
		{
		    if((strlen(t->words[2])==4)&&
				 t->words[2][0]==97&&
				 t->words[2][1]==116&&
				 t->words[2][2]==104&&
				 t->words[2][3]>=48&&
				 t->words[2][3]<=57
				)
			{		
		   	   char str_type[20]={0};
	   	       wireless_sec_get_encrypt(get_vnum_by_vname(t->words[2]),str_type,8);
			   if((strstr(str_type,"64")!=NULL)&&
			   	  (strlen(t->words[5])==10))
			   {	
		           wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"3","hex",t->words[5],1);
			   }
			   else if((strstr(str_type,"128")!=NULL)&&
			   	    (strlen(t->words[5])==26))
			   {
			       wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"3","hex",t->words[5],1);
			   }
			   else if((strstr(str_type,"152")!=NULL)&&
			   	    (strlen(t->words[5])==32))
			   {
			     wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"3","hex",t->words[5],1);
			   }
			   else
			   {
			      printf("input the correct password number\n");
			   }
			}
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }
		}
		else if((strcmp(t->words[1],"security")==0)&&
		  (strcmp(t->words[3],"wepkey[4]")==0)&&
		  (strcmp(t->words[4],"ascii")==0))
		{
           	 if((strlen(t->words[2])==4)&&
				 t->words[2][0]==97&&
				 t->words[2][1]==116&&
				 t->words[2][2]==104&&
				 t->words[2][3]>=48&&
				 t->words[2][3]<=57
				)
			   {	 
				   char str_type[20]={0};
				   wireless_sec_get_encrypt(get_vnum_by_vname(t->words[2]),str_type,8);
				   if((strstr(str_type,"64")!=NULL)&&
					  (strlen(t->words[5])==5))
				   {	
					   wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"4","ascii",t->words[5],1);
				   }
				   else if((strstr(str_type,"128")!=NULL)&&
						(strlen(t->words[5])==13))
				   {
					   wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"4","ascii",t->words[5],1);
				   }
				   else if((strstr(str_type,"152")!=NULL)&&
						(strlen(t->words[5])==16))
				   {
					 wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"4","ascii",t->words[5],1);
				   }
				   else
				   {
					  printf("input the correct password number\n");
				   }
				}
			   else
			   {
				   printf("input the correct wlanport[athX]\n");
			   }
		}
		else if((strcmp(t->words[1],"security")==0)&&
		  (strcmp(t->words[3],"wepkey[4]")==0)&&
		  (strcmp(t->words[4],"hex")==0))
		{
		   	if((strlen(t->words[2])==4)&&
				 t->words[2][0]==97&&
				 t->words[2][1]==116&&
				 t->words[2][2]==104&&
				 t->words[2][3]>=48&&
				 t->words[2][3]<=57
				)
			{		
		   	   char str_type[20]={0};
	   	       wireless_sec_get_encrypt(get_vnum_by_vname(t->words[2]),str_type,8);
			   if((strstr(str_type,"64")!=NULL)&&
			   	  (strlen(t->words[5])==10))
			   {	
		           wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"4","hex",t->words[5],1);
			   }
			   else if((strstr(str_type,"128")!=NULL)&&
			   	    (strlen(t->words[5])==26))
			   {
			       wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"4","hex",t->words[5],1);
			   }
			   else if((strstr(str_type,"152")!=NULL)&&
			   	    (strlen(t->words[5])==32))
			   {
			     wireless_sec_set_wepkey(get_vnum_by_vname(t->words[2]),"4","hex",t->words[5],1);
			   }
			   else
			   {
			      printf("input the correct password number\n");
			   }
			}
		   else
		   {
		       printf("input the correct wlanport[athX]\n");
		   }
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   default:
	   	   printf("invalid commad\n");
	   break;
	}
	printf("\n");	
	return EXIT_SUCCESS;
}


/***********stp************

include the basic stp setup 

********************************/

static int dostp(struct op *t )
{
	int len=0;
	int i=0;
	while(t->words[i]!=NULL)
	{
		i++;
		len++; 
	}
	  switch (len) 
     {
       case 2:
	   	if(strcmp(t->words[1],"show")==0)
		{
		   char str_tmp[10]={0};
		   stp_get_state(str_tmp,10);
		   printf("stp enable %s\n",str_tmp);
		   memset(str_tmp,0,sizeof(str_tmp));
		   stp_get_bridgeprio(str_tmp,10);
		   printf("stp bridge priority %s\n",str_tmp);
		   memset(str_tmp,0,sizeof(str_tmp));  
		   stp_get_fd(str_tmp,10);
		   printf("bridge forward delay %s\n",str_tmp);
           memset(str_tmp,0,sizeof(str_tmp));  
		   stp_get_hello(str_tmp,10); 
		   printf("hello time %s\n",str_tmp);
		   memset(str_tmp,0,sizeof(str_tmp));  
		   stp_get_maxage(str_tmp,10);
		   printf("max message age %s\n",str_tmp);
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   case 4:
	   	if(strcmp(t->words[1],"config")==0)
		{
		  if((strcmp(t->words[3],"enable")==0)||(strcmp(t->words[3],"disable")==0))
		  {
		      stp_config_enable(t->words[2],t->words[3]);
		  }
		  else
		  {
		      printf("please config  stp [enable|disable]");
		  }
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   case 5:
	   	if((strcmp(t->words[1],"config")==0)&&
		  (strcmp(t->words[3],"bridgeprio")==0))
		{
			if(is_number(t->words[4])==0)
		   {
		       stp_config_bridgeprio(t->words[2],t->words[4]);
		   }
		   else
		   {
		       
		      printf("config spanning-tree priority [0-61440]");
		   }	
		}
		else if((strcmp(t->words[1],"config")==0)&&
		  (strcmp(t->words[3],"setfd")==0))
		{ 
		   if((is_number(t->words[4])==0)&&
		   	(atoi(t->words[4])>=4)&&
		   	(atoi(t->words[4])<=30))
		   {
		       stp_config_setfd(t->words[2],t->words[4]);
		   }
		   else
		   {
		       
		      printf("please config spanning-tree forward-delay [4-30]");
		   }		   		   
		}
		else if((strcmp(t->words[1],"config")==0)&&
		  (strcmp(t->words[3],"maxage")==0))
		{

		   if((is_number(t->words[4])==0)&&
		   	(atoi(t->words[4])>=6)&&
		   	(atoi(t->words[4])<=40))
		   {
		       stp_config_maxage(t->words[2],t->words[4]);
		   }
		   else
		   {
		       
		      printf("please config spanning-tree max-age [6-40]");
		   }			   
		}
	    else if((strcmp(t->words[1],"config")==0)&&
		  (strcmp(t->words[3],"hello")==0))
		{
		   if((is_number(t->words[4])==0)&&
		   	(atoi(t->words[4])>=1)&&
		   	(atoi(t->words[4])<=10))
		   {
		       stp_config_hello(t->words[2],t->words[4]);
		   }
		   else
		   {
		       
		      printf("please config  spanning-tree hello-time [1-10]");
		   }
		}
		else 
		{
		   printf("invalid commad\n");
		}
	   break;
	   default:
	   	   printf("invalid commad\n");
	   break;
	}

	printf("\n");
	return EXIT_SUCCESS;
}


#if 0
/***********vlan************

include the basic vlan setup 

********************************/

static int dovlan(struct op *t )
{
	int len=0;
	int i=0;
	while(t->words[i]!=NULL)
	{
		i++;
		len++; 
	}
    switch (len) 
     {
       case 3:
	    if(strcmp(t->words[1],"create")==0)
		{
		  vlan_create(t->words[2]);
		}
		else if(strcmp(t->words[1],"destroy")==0)
		{
		  vlan_destroy(t->words[2]);
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   case 4:
	   	if(strcmp(t->words[1],"addport")==0)
		{
		  vlan_addport(t->words[2],t->words[3]);
		  //printf(" addport-- %s-%s\n",t->words[2],t->words[3]);
		}
		else if(strcmp(t->words[1],"delport")==0)
		{
		   vlan_delport(t->words[2],t->words[3]);
		   //printf("delport --%s-%s\n",t->words[2],t->words[3]);
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   default:
	   	   printf("invalid commad\n");
	   break;
	}
	
	printf("\n");
	return EXIT_SUCCESS;
}
#endif

/***********system************

include the basic system setup 

********************************/

void CWGetFlashSizeInfo(unsigned short *flashTotal, int *flashFree) 
{  int flashConfigFree;  char temp_str[100];  char str[20];  FILE *fp;
 
 memset(temp_str, 0, 100);
 sprintf(temp_str, "df | awk -F \" \" '/jffs/ {print $4}'");  fp = popen(temp_str, "r");
 if(fp)
 {
  memset(str, 0, 20);
  fgets(str, sizeof(str), fp);
  str[strlen(str)-1] = '\0';
  flashConfigFree = atoi(str);
  pclose(fp);
 }

 *flashTotal = 32;
 *flashFree = ((*flashTotal-15)*1024-128-(1024-flashConfigFree))*8;

  printf("flashTotal:%dMB flashFree:%dKb", *flashTotal, *flashFree);
 
 return;
}


int CWGetCpuUseRatio(int *cpu_use_ratio)
{  
 unsigned int total;  
 float user;
 float nice;
 float system;
 float idle;

 char cpu[21];
 char text[201];

 FILE *fp;

 fp = fopen("/proc/stat", "r");
 while (fgets(text, 200, fp))
 {
  if (strstr(text, "cpu"))
  {
   sscanf(text, "%s %f %f %f %f", cpu, &user, &nice, &system, &idle);
  }
 }
 fclose(fp);

 total = (user + nice + system + idle);
 user = (user / total) * 100;
 nice = (nice / total) * 100;
 system = (system / total) * 100;
 idle = (idle / total) * 100;

 *cpu_use_ratio = 10000-idle*100;
 printf("cpu use ratio:%d %\n", (*cpu_use_ratio)/100); 

 return 0;
}


static int dosystem(struct op *t )
{
	int len=0;
	int i=0;
	while(t->words[i]!=NULL)
	{
		i++;
		len++; 
	}
    switch (len) 
     {
        case 2:
	    if(strcmp(t->words[1],"reset")==0)
		{
		    ap_system_reset();
		}
		else if(strcmp(t->words[1],"reboot")==0)
		{
		    ap_system_reboot();
		}
		#if 0
		
		else if(strcmp(t->words[1],"show")==0)
		{
		     char lll[100]={0};
		     wireless_config_maxsimultusers("ath0","127");
		     ap_system_show();
			 network_get_ntptime();
			 ap_get_macaddressconnectedWithac(lll);
		}
		#endif
		else
		{
		   printf("invalid commad\n");
		}
	   break;
       case 3:
	    if(strcmp(t->words[1],"backup")==0)
		{
		    if(is_ipaddr(t->words[2])==0)
		    {
		        ap_system_backup(t->words[2]);
		    }
		    else
		    {
		        printf("please input the correct ip \n");
		    }
		  
		}
	    else if(strcmp(t->words[1],"ping")==0)
		{
		
		  ap_system_ping(t->words[2]);
		}
		else if(strcmp(t->words[1],"show")==0)
		{
		    if(strcmp(t->words[2],"version")==0)
		    {
			     ap_system_show();
		    }
			else if (strcmp(t->words[2],"memory")==0)
			{

	            char szVal[50]={0};
				char useVal[50]={0};
				char str_tmp_cmd[50]={0};
		    	FILE   *stream; 
			    memset(szVal, 0x00, sizeof(szVal));	
				strcpy(str_tmp_cmd,"/usr/bin/free | /usr/bin/tail -1 |/usr/bin/awk '{print $2}'");
				stream = popen( str_tmp_cmd, "r" );
				fread( szVal, sizeof(char), sizeof(szVal),  stream); 
				fclose( stream );
			    szVal[strlen(szVal)-1]='\0';
				printf("total memory:%d\n",atoi(szVal));

			    memset(useVal, 0x00, sizeof(useVal));
				memset(str_tmp_cmd, 0x00, sizeof(str_tmp_cmd));
               	strcpy(str_tmp_cmd,"/usr/bin/free | /usr/bin/tail -1 |/usr/bin/awk '{print $3}'");
				stream = popen( str_tmp_cmd, "r" );
				fread( useVal, sizeof(char), sizeof(useVal),  stream); 
				fclose( stream );
			    useVal[strlen(useVal)-1]='\0';
				printf("used memory:%d\n",atoi(useVal));

				printf("used percent :%d %\n",(atoi(useVal)*100)/atoi(szVal));
				
				
			}
			else if(strcmp(t->words[2],"flashinfo")==0)
			{

			   unsigned short *flashTotal;
			   int *flashFree;
			   CWGetFlashSizeInfo(flashTotal, flashFree); 
			}
			else if(strcmp(t->words[2],"runtime")==0)
			{

				char szVal[50]={0};
				char str_tmp_cmd[50]={0};
		    	FILE   *stream; 
			    memset(szVal, 0x00, sizeof(szVal));	
				strcpy(str_tmp_cmd,"/bin/cat /proc/uptime |/usr/bin/awk '{print $1}'");
				stream = popen( str_tmp_cmd, "r" );
				fread( szVal, sizeof(char), sizeof(szVal),  stream); 
				fclose( stream );
			    szVal[strlen(szVal)-1]='\0';
				printf("runtime:%d\n",atoi(szVal));
			}
			else if(strcmp(t->words[2],"workmode")==0)
			{

				char szVal[50]={0};
				char str_tmp_cmd[50]={0};
		    	FILE   *stream; 
			    memset(szVal, 0x00, sizeof(szVal));	
				strcpy(str_tmp_cmd,"/bin/cat /proc/sys/net/ipv4/ip_forward");
				stream = popen( str_tmp_cmd, "r" );
				fread( szVal, sizeof(char), sizeof(szVal),  stream); 
				fclose( stream );
			    szVal[strlen(szVal)-1]='\0';
				if(atoi(szVal) == 0)
				{
					printf("workmode:bridge \n");
				}
				else
				{
				   	printf("workmode:route \n");
				}
			
			}
			else if (strcmp(t->words[2],"cpuratio")==0)
			{
			  int *cpu_use_ratio;
			  CWGetCpuUseRatio(cpu_use_ratio);
			}
			else if (strcmp(t->words[2],"temperature")==0)
			{
                printf("temperature: 60\n");
			}
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   case 4:
	    if(strcmp(t->words[1],"upgrade")==0)
		{
		  if(is_ipaddr(t->words[2])==0)
		  {
		       ap_system_upgrade(t->words[2],t->words[3]);
		  }
		  else
		  {
		      printf("please input the correct ip \n");
		  }
		 
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   default:
	   	   printf("invalid commad\n");
	   break;
	}
	
	printf("\n");
	return EXIT_SUCCESS;
}

/***********snmp************

include the basic snmp  setup 

********************************/
//#if 0
static int dosnmp(struct op *t )
{
	int len=0;
	int i=0;
	while(t->words[i]!=NULL)
	{
		i++;
		len++; 
	}
    switch (len) 
     {
       case 2:
	   	  if(strcmp(t->words[1],"show")==0)
	   	  {
	   	    char str_cmp[50]={0};
			snmp_get_trapipaddr(str_cmp,50);
			printf("trapipaddr: %s \n",str_cmp);
	   	  }
		  else
		  {
		     printf("invalid commad\n");
		  }
	   break;
       case 3:
	    if(strcmp(t->words[1],"receivetrap")==0)
		{

		  if(is_ipaddr(t->words[2])==0)
		  {
		     snmp_set_trapipaddr(t->words[2]);		      
		  }
		  else
		  {
		      printf("please input the receivetrap ip \n");
		  }
		  
		}
		else if(strcmp(t->words[1],"syslocation")==0)
		{
		  printf(" snmp  syslocation %s\n",t->words[2]);
		}
		else if(strcmp(t->words[1],"syscontact")==0)
		{
		  printf(" snmp  syscontact %s\n",t->words[2]);
		}
		else if(strcmp(t->words[1],"publiccn")==0)
		{
		  printf(" snmp  publiccn %s\n",t->words[2]);
		}
		else if(strcmp(t->words[1],"privatecn")==0)
		{
		  printf(" snmp  privatecn %s\n",t->words[2]);
		}
		else if(strcmp(t->words[1],"trapscn")==0)
		{
		  printf(" snmp  trapscn %s\n",t->words[2]);
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   default:
	   	   printf("invalid commad\n");
	   break;
	}
	
	printf("\n");
	return EXIT_SUCCESS;
}

//#endif
/***********syslog************

include the basic syslog  setup 

********************************/

static int dosyslog(struct op *t )
{
	int len=0;
	int i=0;
	while(t->words[i]!=NULL)
	{
		i++;
		len++; 
	}
    switch (len) 
     {
       case 2:
	   	 if((strcmp(t->words[1],"enable")==0)||
		  (strcmp(t->words[1],"disable")==0))
	     {
	   	     syslog_set_state(t->words[1]);
	   	 }
		 else if(strcmp(t->words[1],"show")==0)
		 {
		     char str_tmp[20]={0};
		     syslog_get_state(str_tmp,20);
			 if(strcmp(str_tmp,"yes") == 0)
			 {
			    printf("syslog state enable\n");
			 }
			 else
			 {
			    printf("syslog state disable\n");
			 }
			 
			 memset(str_tmp,0,sizeof(str_tmp));
			 syslog_get_ip(str_tmp,20);
			 printf("syslog Server IP %s\n",str_tmp);
			 memset(str_tmp,0,sizeof(str_tmp));
			 syslog_get_port(str_tmp,20);
			 printf("syslog Server port %s\n",str_tmp);
		 }
		 else
		 {
		     printf(" please config set_state[enable|disable]\n");
		 }	   
	   break;
       case 4:
	    if((strcmp(t->words[1],"config")==0)&&
		  (strcmp(t->words[2],"serverip")==0))
		{
		  if(is_ipaddr(t->words[3])==0)
		  {
		     syslog_set_ip(t->words[3],1);
		  }
		  else
		  {
		     printf("please input the correct ip addr\n");
		  }
		}
		else if((strcmp(t->words[1],"config")==0)&&
		  (strcmp(t->words[2],"serverport")==0))
		{
		  if(is_number(t->words[3])==0)
		  {
		      syslog_set_port(t->words[3],1);
		  }
		  else
		  {
		     printf("please input the correct serverport\n");	  
		  }
		}
		else
		{
		   printf("invalid commad\n");
		}
	   break;
	   default:
	   	   printf("invalid commad\n");
	   break;
	}
	
	printf("\n");
	return EXIT_SUCCESS;
}



static int dolabel(struct op *t )
{
	return(0);
}

static int
dochdir(t)
register struct op *t;
{
	register char *cp, *er;

	if ((cp = t->words[1]) == NULL && (cp = homedir->value) == NULL)
		er = ": no home directory";
	else if(chdir(cp) < 0)
		er = ": bad directory";
	else
		return(0);
	prs(cp != NULL? cp: "cd");
	err(er);
	return(1);
}

static int
doshift(t)
register struct op *t;
{
	register int n;

	n = t->words[1]? getn(t->words[1]): 1;
	if(dolc < n) {
		err("nothing to shift");
		return(1);
	}
	dolv[n] = dolv[0];
	dolv += n;
	dolc -= n;
	setval(lookup("#"), putn(dolc));
	return(0);
}

/*
 * execute login and newgrp directly
 */
static int
dologin(t)
struct op *t;
{
	register char *cp;

	if (interactive) {
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	cp = rexecve(t->words[0], t->words, makenv());
	prs(t->words[0]); prs(": "); err(cp);
	return(1);
}

static int
doumask(t)
register struct op *t;
{
	register int i, n;
	register char *cp;

	if ((cp = t->words[1]) == NULL) {
		i = umask(0);
		umask(i);
		for (n=3*4; (n-=3) >= 0;)
			putc('0'+((i>>n)&07), stderr);
		putc('\n', stderr);
	} else {
		for (n=0; *cp>='0' && *cp<='9'; cp++)
			n = n*8 + (*cp-'0');
		umask(n);
	}
	return(0);
}

static int
doexec(t)
register struct op *t;
{
	register int i;
	jmp_buf ex;
	xint *ofail;

	t->ioact = NULL;
	for(i = 0; (t->words[i]=t->words[i+1]) != NULL; i++)
		;
	if (i == 0)
		return(1);
	execflg = 1;
	ofail = failpt;
	if (setjmp(failpt = ex) == 0)
		execute(t, NOPIPE, NOPIPE, FEXEC);
	failpt = ofail;
	execflg = 0;
	return(1);
}

static int
dodot(t)
struct op *t;
{
	register int i;
	register char *sp, *tp;
	char *cp;

	if ((cp = t->words[1]) == NULL)
		return(0);
	sp = any('/', cp)? ":": path->value;
	while (*sp) {
		tp = e.linep;
		while (*sp && (*tp = *sp++) != ':')
			tp++;
		if (tp != e.linep)
			*tp++ = '/';
		for (i = 0; (*tp++ = cp[i++]) != '\0';)
			;
		if ((i = open(e.linep, 0)) >= 0) {
			exstat = 0;
			next(remap(i));
			return(exstat);
		}
	}
	prs(cp);
	err(": not found");
	return(-1);
}

static int
dowait(t)
struct op *t;
{
	register int i;
	register char *cp;

	if ((cp = t->words[1]) != NULL) {
		i = getn(cp);
		if (i == 0)
			return(0);
	} else
		i = -1;
	setstatus(waitfor(i, 1));
	return(0);
}

static int
doread(t)
struct op *t;
{
	register char *cp, **wp;
	register int nb = 0;
	register int  nl = 0;

	if (t->words[1] == NULL) {
		err("Usage: read name ...");
		return(1);
	}
	for (wp = t->words+1; *wp; wp++) {
		for (cp = e.linep; !nl && cp < elinep-1; cp++)
			if ((nb = read(0, cp, sizeof(*cp))) != sizeof(*cp) ||
			    (nl = (*cp == '\n')) ||
			    (wp[1] && any(*cp, ifs->value)))
				break;
		*cp = 0;
		if (nb <= 0)
			break;
		setval(lookup(*wp), e.linep);
	}
	return(nb <= 0);
}

static int
doeval(t)
register struct op *t;
{
	return(RUN(awordlist, t->words+1, wdchar));
}

static int
dotrap(t)
register struct op *t;
{
	register int  n, i;
	register int  resetsig;

	if (t->words[1] == NULL) {
		for (i=0; i<=_NSIG; i++)
			if (trap[i]) {
				prn(i);
				prs(": ");
				prs(trap[i]);
				prs("\n");
			}
		return(0);
	}
	resetsig = isdigit(*t->words[1]);
	for (i = resetsig ? 1 : 2; t->words[i] != NULL; ++i) {
		n = getsig(t->words[i]);
		freecell(trap[n]);
		trap[n] = 0;
		if (!resetsig) {
			if (*t->words[1] != '\0') {
				trap[n] = strsave(t->words[1], 0);
				setsig(n, sig);
			} else
				setsig(n, SIG_IGN);
		} else {
			if (interactive)
				if (n == SIGINT)
					setsig(n, onintr);
				else
					setsig(n, n == SIGQUIT ? SIG_IGN 
							       : SIG_DFL);
			else
				setsig(n, SIG_DFL);
		}
	}
	return(0);
}

static int
getsig(s)
char *s;
{
	register int n;

	if ((n = getn(s)) < 0 || n > _NSIG) {
		err("trap: bad signal number");
		n = 0;
	}
	return(n);
}

static void
setsig( register int n, sighandler_t f)
{
	if (n == 0)
		return;
	if (signal(n, SIG_IGN) != SIG_IGN || ourtrap[n]) {
		ourtrap[n] = 1;
		signal(n, f);
	}
}

static int
getn(as)
char *as;
{
	register char *s;
	register int n, m;

	s = as;
	m = 1;
	if (*s == '-') {
		m = -1;
		s++;
	}
	for (n = 0; isdigit(*s); s++)
		n = (n*10) + (*s-'0');
	if (*s) {
		prs(as);
		err(": bad number");
	}
	return(n*m);
}

static int
dobreak(t)
struct op *t;
{
	return(brkcontin(t->words[1], 1));
}

static int
docontinue(t)
struct op *t;
{
	return(brkcontin(t->words[1], 0));
}

static int
brkcontin(cp, val)
register char *cp;
int val;
{
	register struct brkcon *bc;
	register int nl;

	nl = cp == NULL? 1: getn(cp);
	if (nl <= 0)
		nl = 999;
	do {
		if ((bc = brklist) == NULL)
			break;
		brklist = bc->nextlev;
	} while (--nl);
	if (nl) {
		err("bad break/continue level");
		return(1);
	}
	isbreak = val;
	longjmp(bc->brkpt, 1);
	/* NOTREACHED */
}

static int
doexit(t)
struct op *t;
{
	register char *cp;

	execflg = 0;
	if ((cp = t->words[1]) != NULL)
		setstatus(getn(cp));
	leave();
	/* NOTREACHED */
	return(0);
}

static int
doexport(t)
struct op *t;
{
	rdexp(t->words+1, export, EXPORT);
	return(0);
}

static int
doreadonly(t)
struct op *t;
{
	rdexp(t->words+1, ronly, RONLY);
	return(0);
}

static void rdexp (char **wp, void (*f)(struct var *), int key)
{
	if (*wp != NULL) {
		for (; *wp != NULL; wp++) {
			if (isassign(*wp)) {
				char *cp;
				assign(*wp, COPYV);
				for (cp = *wp; *cp != '='; cp++)
					;
				*cp = '\0';
			}
			if (checkname(*wp))
				(*f)(lookup(*wp));
			else
				badid(*wp);
		}
	} else
		putvlist(key, 1);
}

static void
badid(s)
register char *s;
{
	prs(s);
	err(": bad identifier");
}

#if 0
static int
doset(t)
register struct op *t;
{
	register struct var *vp;
	register char *cp;
	register int n;

	if ((cp = t->words[1]) == NULL) {
		for (vp = vlist; vp; vp = vp->next)
			varput(vp->name, 1);
		return(0);
	}
	if (*cp == '-') {
		/* bad: t->words++; */
		for(n = 0; (t->words[n]=t->words[n+1]) != NULL; n++)
			;
		if (*++cp == 0)
			flag['x'] = flag['v'] = 0;
		else
			for (; *cp; cp++)
				switch (*cp) {
				case 'e':
					if (!interactive)
						flag['e']++;
					break;

				default:
					if (*cp>='a' && *cp<='z')
						flag[(int)*cp]++;
					break;
				}
		setdash();
	}
	if (t->words[1]) {
		t->words[0] = dolv[0];
		for (n=1; t->words[n]; n++)
			setarea((char *)t->words[n], 0);
		dolc = n-1;
		dolv = t->words;
		setval(lookup("#"), putn(dolc));
		setarea((char *)(dolv-1), 0);
	}
	return(0);
}

#endif

#if 0
static void
varput(s, out)
register char *s;
int out;
{
	if (isalnum(*s) || *s == '_') {
		write(out, s, strlen(s));
		write(out, "\n", 1);
	}
}
#endif

/*
 * Copyright (c) 1999 Herbert Xu <herbert@debian.org>
 * This file contains code for the times builtin.
 */
static int dotimes(struct op *t )
{
	struct tms buf;
	long int clk_tck = sysconf(_SC_CLK_TCK);

	times(&buf);
	printf("%dm%fs %dm%fs\n%dm%fs %dm%fs\n",
	       (int) (buf.tms_utime / clk_tck / 60),
	       ((double) buf.tms_utime) / clk_tck,
	       (int) (buf.tms_stime / clk_tck / 60),
	       ((double) buf.tms_stime) / clk_tck,
	       (int) (buf.tms_cutime / clk_tck / 60),
	       ((double) buf.tms_cutime) / clk_tck,
	       (int) (buf.tms_cstime / clk_tck / 60),
	       ((double) buf.tms_cstime) / clk_tck);
	return 0;
}


static int(*inbuilt(char *s))(struct op *)
{
	const struct builtincmd *bp;

	for (bp = builtincmds; bp->name != NULL; bp++)
		if (strcmp(bp->name, s) == 0)
			return(bp->builtinfunc);

	return(NULL);
}

/* -------- eval.c -------- */

/*
 * ${}
 * `command`
 * blank interpretation
 * quoting
 * glob
 */

static char ** eval( char **ap, int f)
{
	struct wdblock *wb;
	char **wp;
	char **wf;
	jmp_buf ev;

#if __GNUC__
	/* Avoid longjmp clobbering */
	(void) &wp;
	(void) &ap;
#endif
	wp = NULL;
	wb = NULL;
	wf = NULL;
	if (newenv(setjmp(errpt = ev)) == 0) {
		while (*ap && isassign(*ap))
			expand(*ap++, &wb, f & ~DOGLOB);
		if (flag['k']) {
			for (wf = ap; *wf; wf++) {
				if (isassign(*wf))
					expand(*wf, &wb, f & ~DOGLOB);
			}
		}
		for (wb = addword((char *)0, wb); *ap; ap++) {
			if (!flag['k'] || !isassign(*ap))
				expand(*ap, &wb, f & ~DOKEY);
		}
		wb = addword((char *)0, wb);
		wp = getwords(wb);
		quitenv();
	} else
		gflg = 1;
	return(gflg? (char **)NULL: wp);
}

/*
 * Make the exported environment from the exported
 * names in the dictionary. Keyword assignments
 * will already have been done.
 */
static char **
makenv()

{
	register struct wdblock *wb;
	register struct var *vp;

	wb = NULL;
	for (vp = vlist; vp; vp = vp->next)
		if (vp->status & EXPORT)
			wb = addword(vp->name, wb);
	wb = addword((char *)0, wb);
	return(getwords(wb));
}

static char *
evalstr(cp, f)
register char *cp;
int f;
{
	struct wdblock *wb;

	wb = NULL;
	if (expand(cp, &wb, f)) {
		if (wb == NULL || wb->w_nword == 0 || (cp = wb->w_words[0]) == NULL)
			cp = "";
		DELETE(wb);
	} else
		cp = NULL;
	return(cp);
}

static int
expand( char *cp, register struct wdblock **wbp, int f)
{
	jmp_buf ev;

#if __GNUC__
	/* Avoid longjmp clobbering */
	(void) &cp;
#endif
	gflg = 0;
	if (cp == NULL)
		return(0);
	if (!anys("$`'\"", cp) &&
	    !anys(ifs->value, cp) &&
	    ((f&DOGLOB)==0 || !anys("[*?", cp))) {
		cp = strsave(cp, areanum);
		if (f & DOTRIM)
			unquote(cp);
		*wbp = addword(cp, *wbp);
		return(1);
	}
	if (newenv(setjmp(errpt = ev)) == 0) {
		PUSHIO(aword, cp, strchar);
		e.iobase = e.iop;
		while ((cp = blank(f)) && gflg == 0) {
			e.linep = cp;
			cp = strsave(cp, areanum);
			if ((f&DOGLOB) == 0) {
				if (f & DOTRIM)
					unquote(cp);
				*wbp = addword(cp, *wbp);
			} else
				*wbp = glob(cp, *wbp);
		}
		quitenv();
	} else
		gflg = 1;
	return(gflg == 0);
}

/*
 * Blank interpretation and quoting
 */
static char *
blank(f)
int f;
{
	register int c, c1;
	register char *sp;
	int scanequals, foundequals;

	sp = e.linep;
	scanequals = f & DOKEY;
	foundequals = 0;

loop:
	switch (c = subgetc('"', foundequals)) {
	case 0:
		if (sp == e.linep)
			return(0);
		*e.linep++ = 0;
		return(sp);

	default:
		if (f & DOBLANK && any(c, ifs->value))
			goto loop;
		break;

	case '"':
	case '\'':
		scanequals = 0;
		if (INSUB())
			break;
		for (c1 = c; (c = subgetc(c1, 1)) != c1;) {
			if (c == 0)
				break;
			if (c == '\'' || !any(c, "$`\""))
				c |= QUOTE;
			*e.linep++ = c;
		}
		c = 0;
	}
	unget(c);
	if (!isalpha(c) && c != '_')
		scanequals = 0;
	for (;;) {
		c = subgetc('"', foundequals);
		if (c == 0 ||
		    f & (DOBLANK && any(c, ifs->value)) ||
		    (!INSUB() && any(c, "\"'"))) {
		        scanequals = 0;
			unget(c);
			if (any(c, "\"'"))
				goto loop;
			break;
		}
		if (scanequals) {
			if (c == '=') {
				foundequals = 1;
				scanequals  = 0;
			}
			else if (!isalnum(c) && c != '_')
				scanequals = 0;
		}
		*e.linep++ = c;
	}
	*e.linep++ = 0;
	return(sp);
}

/*
 * Get characters, substituting for ` and $
 */
static int
subgetc(ec, quoted)
register char ec;
int quoted;
{
	register char c;

again:
	c = my_getc(ec);
	if (!INSUB() && ec != '\'') {
		if (c == '`') {
			if (grave(quoted) == 0)
				return(0);
			e.iop->task = XGRAVE;
			goto again;
		}
		if (c == '$' && (c = dollar(quoted)) == 0) {
			e.iop->task = XDOLL;
			goto again;
		}
	}
	return(c);
}

/*
 * Prepare to generate the string returned by ${} substitution.
 */
static int
dollar(quoted)
int quoted;
{
	int otask;
	struct io *oiop;
	char *dolp;
	register char *s, c, *cp=NULL;
	struct var *vp;

	c = readc();
	s = e.linep;
	if (c != '{') {
		*e.linep++ = c;
		if (isalpha(c) || c == '_') {
			while ((c = readc())!=0 && (isalnum(c) || c == '_'))
				if (e.linep < elinep)
					*e.linep++ = c;
			unget(c);
		}
		c = 0;
	} else {
		oiop = e.iop;
		otask = e.iop->task;
		e.iop->task = XOTHER;
		while ((c = subgetc('"', 0))!=0 && c!='}' && c!='\n')
			if (e.linep < elinep)
				*e.linep++ = c;
		if (oiop == e.iop)
			e.iop->task = otask;
		if (c != '}') {
			err("unclosed ${");
			gflg++;
			return(c);
		}
	}
	if (e.linep >= elinep) {
		err("string in ${} too long");
		gflg++;
		e.linep -= 10;
	}
	*e.linep = 0;
	if (*s)
		for (cp = s+1; *cp; cp++)
			if (any(*cp, "=-+?")) {
				c = *cp;
				*cp++ = 0;
				break;
			}
	if (s[1] == 0 && (*s == '*' || *s == '@')) {
		if (dolc > 1) {
			/* currently this does not distinguish $* and $@ */
			/* should check dollar */
			e.linep = s;
			PUSHIO(awordlist, dolv+1, dolchar);
			return(0);
		} else {	/* trap the nasty ${=} */
			s[0] = '1';
			s[1] = 0;
		}
	}
	vp = lookup(s);
	if ((dolp = vp->value) == null) {
		switch (c) {
		case '=':
			if (isdigit(*s)) {
				err("cannot use ${...=...} with $n");
				gflg++;
				break;
			}
			setval(vp, cp);
			dolp = vp->value;
			break;

		case '-':
			dolp = strsave(cp, areanum);
			break;

		case '?':
			if (*cp == 0) {
				prs("missing value for ");
				err(s);
			} else
				err(cp);
			gflg++;
			break;
		}
	} else if (c == '+')
		dolp = strsave(cp, areanum);
	if (flag['u'] && dolp == null) {
		prs("unset variable: ");
		err(s);
		gflg++;
	}
	e.linep = s;
	PUSHIO(aword, dolp, quoted ? qstrchar : strchar);
	return(0);
}

/*
 * Run the command in `...` and read its output.
 */

static int
grave(quoted)
int quoted;
{
	char *cp;
	register int i;
	int j;
	int pf[2];
	static char child_cmd[LINELIM];
	char *src;
	char *dest;
	int count;
	int ignore;
	int ignore_once;
	char *argument_list[4];

#if __GNUC__
	/* Avoid longjmp clobbering */
	(void) &cp;
#endif
	
	for (cp = e.iop->argp->aword; *cp != '`'; cp++)
		if (*cp == 0) {
			err("no closing `");
			return(0);
		}

	/* string copy with dollar expansion */
	src = e.iop->argp->aword;
	dest = child_cmd;
	count = 0;
	ignore = 0;
	ignore_once = 0;
	while ((*src != '`') && (count < LINELIM)) {
		if (*src == '\'')
			ignore = !ignore;
		if (*src == '\\')
			ignore_once = 1;
		if (*src == '$' && !ignore && !ignore_once) {
			struct var *vp;
			char var_name[LINELIM];
			char alt_value[LINELIM];
			int var_index = 0;
			int alt_index = 0;
			char operator = 0;
			int braces = 0;
			char *value;

			src++;
			if (*src == '{') {
				braces = 1;
				src++;
			}

			var_name[var_index++] = *src++;
			while (isalnum(*src))
				var_name[var_index++] = *src++;
			var_name[var_index] = 0;

			if (braces) {
				switch (*src) {
				case '}':
					break;
				case '-':
				case '=':
				case '+':
				case '?':
					operator = *src;
					break;
				default:
					err("unclosed ${\n");
					return(0);
				}
				if (operator) {	
					src++;
					while (*src && (*src != '}')) {
						alt_value[alt_index++] = *src++;
					}
					alt_value[alt_index] = 0;
					if (*src != '}') {
						err("unclosed ${\n");
						return(0);
					}
				}
				src++;
			}

			vp = lookup(var_name);
			if (vp->value != null)
				value = (operator == '+')? alt_value : vp->value;
			else if (operator == '?') {
				err(alt_value);
				return(0);
			} else if (alt_index && (operator != '+')) {
				value = alt_value;
				if (operator == '=')
					setval(vp, value);
			} else
				continue;

			while (*value && (count < LINELIM)) {
				*dest++ = *value++;
				count++;
			}
		} else {
			*dest++ = *src++;
			count++;
			ignore_once = 0;
		}
	}
	*dest = '\0';
	
	if (openpipe(pf) < 0)
		return(0);
	while ((i = vfork()) == -1 && errno == EAGAIN)
		;
	if (i < 0) {
		closepipe(pf);
		err((char*)bb_msg_memory_exhausted);
		return(0);
	}
	if (i != 0) {
		waitpid(i, NULL, 0);
		e.iop->argp->aword = ++cp;
		close(pf[1]);
		PUSHIO(afile, remap(pf[0]), (int(*)(struct ioarg *))((quoted)? qgravechar: gravechar));
		return(1);
	}
	/* allow trapped signals */
	/* XXX - Maybe this signal stuff should go as well? */
	for (j=0; j<=_NSIG; j++)
		if (ourtrap[j] && signal(j, SIG_IGN) != SIG_IGN)
			signal(j, SIG_DFL);
	
	dup2(pf[1], 1);
	closepipe(pf);

	argument_list[0] = (char *)DEFAULT_SHELL;
	argument_list[1] = "-c";
	argument_list[2] = child_cmd;
	argument_list[3] = 0;

	prs(rexecve(argument_list[0], argument_list, makenv()));
	_exit(1);
}


static char *
unquote(as)
register char *as;
{
	register char *s;

	if ((s = as) != NULL)
		while (*s)
			*s++ &= ~QUOTE;
	return(as);
}

/* -------- glob.c -------- */

/*
 * glob
 */

#define	scopy(x) strsave((x), areanum)
#define	BLKSIZ	512
#define	NDENT	((BLKSIZ+sizeof(struct dirent)-1)/sizeof(struct dirent))

static	struct wdblock	*cl, *nl;
static	char	spcl[] = "[?*";

static struct wdblock *
glob(cp, wb)
char *cp;
struct wdblock *wb;
{
	register int i;
	register char *pp;

	if (cp == 0)
		return(wb);
	i = 0;
	for (pp = cp; *pp; pp++)
		if (any(*pp, spcl))
			i++;
		else if (!any(*pp & ~QUOTE, spcl))
			*pp &= ~QUOTE;
	if (i != 0) {
		for (cl = addword(scopy(cp), (struct wdblock *)0); anyspcl(cl); cl = nl) {
			nl = newword(cl->w_nword*2);
			for(i=0; i<cl->w_nword; i++) { /* for each argument */
				for (pp = cl->w_words[i]; *pp; pp++)
					if (any(*pp, spcl)) {
						globname(cl->w_words[i], pp);
						break;
					}
				if (*pp == '\0')
					nl = addword(scopy(cl->w_words[i]), nl);
			}
			for(i=0; i<cl->w_nword; i++)
				DELETE(cl->w_words[i]);
			DELETE(cl);
		}
		for(i=0; i<cl->w_nword; i++)
			unquote(cl->w_words[i]);
		glob0((char *)cl->w_words, cl->w_nword, sizeof(char *), xstrcmp);
		if (cl->w_nword) {
			for (i=0; i<cl->w_nword; i++)
				wb = addword(cl->w_words[i], wb);
			DELETE(cl);
			return(wb);
		}
	}
	wb = addword(unquote(cp), wb);
	return(wb);
}

static void
globname(we, pp)
char *we;
register char *pp;
{
	register char *np, *cp;
	char *name, *gp, *dp;
	int k;
	DIR *dirp;
	struct dirent *de;
	char dname[NAME_MAX+1];
	struct stat dbuf;

	for (np = we; np != pp; pp--)
		if (pp[-1] == '/')
			break;
	for (dp = cp = space((int)(pp-np)+3); np < pp;)
		*cp++ = *np++;
	*cp++ = '.';
	*cp = '\0';
	for (gp = cp = space(strlen(pp)+1); *np && *np != '/';)
		*cp++ = *np++;
	*cp = '\0';
	dirp = opendir(dp);
	if (dirp == 0) {
		DELETE(dp);
		DELETE(gp);
		return;
	}
	dname[NAME_MAX] = '\0';
	while ((de=readdir(dirp))!=NULL) {
		/* XXX Hmmm... What this could be? (abial) */
		/*
		if (ent[j].d_ino == 0)
			continue;
		*/
		strncpy(dname, de->d_name, NAME_MAX);
			if (dname[0] == '.')
				if (*gp != '.')
					continue;
			for(k=0; k<NAME_MAX; k++)
				if (any(dname[k], spcl))
					dname[k] |= QUOTE;
			if (gmatch(dname, gp)) {
				name = generate(we, pp, dname, np);
				if (*np && !anys(np, spcl)) {
					if (stat(name,&dbuf)) {
						DELETE(name);
						continue;
					}
				}
				nl = addword(name, nl);
			}
	}
	closedir(dirp);
	DELETE(dp);
	DELETE(gp);
}

/*
 * generate a pathname as below.
 * start..end1 / middle end
 * the slashes come for free
 */
static char *
generate(start1, end1, middle, end)
char *start1;
register char *end1;
char *middle, *end;
{
	char *p;
	register char *op, *xp;

	p = op = space((int)(end1-start1)+strlen(middle)+strlen(end)+2);
	for (xp = start1; xp != end1;)
		*op++ = *xp++;
	for (xp = middle; (*op++ = *xp++) != '\0';)
		;
	op--;
	for (xp = end; (*op++ = *xp++) != '\0';)
		;
	return(p);
}

static int
anyspcl(wb)
register struct wdblock *wb;
{
	register int i;
	register char **wd;

	wd = wb->w_words;
	for (i=0; i<wb->w_nword; i++)
		if (anys(spcl, *wd++))
			return(1);
	return(0);
}

static int
xstrcmp(p1, p2)
char *p1, *p2;
{
	return(strcmp(*(char **)p1, *(char **)p2));
}

/* -------- word.c -------- */

static struct wdblock *
newword(nw)
register int nw;
{
	register struct wdblock *wb;

	wb = (struct wdblock *) space(sizeof(*wb) + nw*sizeof(char *));
	wb->w_bsize = nw;
	wb->w_nword = 0;
	return(wb);
}

static struct wdblock *
addword(wd, wb)
char *wd;
register struct wdblock *wb;
{
	register struct wdblock *wb2;
	register int nw;

	if (wb == NULL)
		wb = newword(NSTART);
	if ((nw = wb->w_nword) >= wb->w_bsize) {
		wb2 = newword(nw * 2);
		memcpy((char *)wb2->w_words, (char *)wb->w_words, nw*sizeof(char *));
		wb2->w_nword = nw;
		DELETE(wb);
		wb = wb2;
	}
	wb->w_words[wb->w_nword++] = wd;
	return(wb);
}
static 
char **
getwords(wb)
register struct wdblock *wb;
{
	register char **wd;
	register int nb;

	if (wb == NULL)
		return((char **)NULL);
	if (wb->w_nword == 0) {
		DELETE(wb);
		return((char **)NULL);
	}
	wd = (char **) space(nb = sizeof(*wd) * wb->w_nword);
	memcpy((char *)wd, (char *)wb->w_words, nb);
	DELETE(wb);	/* perhaps should done by caller */
	return(wd);
}

int (*splash_func)(char *, char *);
int	splash_globv;

static void
glob0(a0, a1, a2, a3)
char *a0;
unsigned a1;
int a2;
int (*a3) (char *, char *);
{
	splash_func = a3;
	splash_globv = a2;
	glob1(a0, a0 + a1 * a2);
}

static void
glob1(base, lim)
char *base, *lim;
{
	register char *i, *j;
	int v2;
	char *lptr, *hptr;
	int c;
	unsigned n;


	v2 = splash_globv;

top:
	if ((n=(int)(lim-base)) <= v2)
		return;
	n = v2 * (n / (2*v2));
	hptr = lptr = base+n;
	i = base;
	j = lim-v2;
	for(;;) {
		if (i < lptr) {
			if ((c = (*splash_func)(i, lptr)) == 0) {
				glob2(i, lptr -= v2);
				continue;
			}
			if (c < 0) {
				i += v2;
				continue;
			}
		}

begin:
		if (j > hptr) {
			if ((c = (*splash_func)(hptr, j)) == 0) {
				glob2(hptr += v2, j);
				goto begin;
			}
			if (c > 0) {
				if (i == lptr) {
					glob3(i, hptr += v2, j);
					i = lptr += v2;
					goto begin;
				}
				glob2(i, j);
				j -= v2;
				i += v2;
				continue;
			}
			j -= v2;
			goto begin;
		}


		if (i == lptr) {
			if (lptr-base >= lim-hptr) {
				glob1(hptr+v2, lim);
				lim = lptr;
			} else {
				glob1(base, lptr);
				base = hptr+v2;
			}
			goto top;
		}


		glob3(j, lptr -= v2, i);
		j = hptr -= v2;
	}
}

static void
glob2(i, j)
char *i, *j;
{
	register char *index1, *index2, c;
	int m;

	m = splash_globv;
	index1 = i;
	index2 = j;
	do {
		c = *index1;
		*index1++ = *index2;
		*index2++ = c;
	} while(--m);
}

static void
glob3(i, j, k)
char *i, *j, *k;
{
	register char *index1, *index2, *index3;
	int c;
	int m;

	m = splash_globv;
	index1 = i;
	index2 = j;
	index3 = k;
	do {
		c = *index1;
		*index1++ = *index3;
		*index3++ = *index2;
		*index2++ = c;
	} while(--m);
}

/* -------- io.c -------- */

/*
 * shell IO
 */

static int my_getc( int ec)
{
	register int c;

	if(e.linep > elinep) {
		while((c=readc()) != '\n' && c)
			;
		err("input line too long");
		gflg++;
		return(c);
	}
	c = readc();
	if ((ec != '\'') && (ec != '`') && (e.iop->task != XGRAVE)) {
		if(c == '\\') {
			c = readc();
			if (c == '\n' && ec != '\"')
				return(my_getc(ec));
			c |= QUOTE;
		}
	}
	return(c);
}

static void
unget(c)
int c;
{
	if (e.iop >= e.iobase)
		e.iop->peekc = c;
}

static int
eofc()

{
  return e.iop < e.iobase || (e.iop->peekc == 0 && e.iop->prev == 0);
}

static int
readc()
{
	register int c;

	for (; e.iop >= e.iobase; e.iop--)
		if ((c = e.iop->peekc) != '\0') {
			e.iop->peekc = 0;
			return(c);
		}
		else {
		    if (e.iop->prev != 0) {
		        if ((c = (*e.iop->iofn)(e.iop->argp, e.iop)) != '\0') {
			        if (c == -1) {
				        e.iop++;
				        continue;
			        }
			        if (e.iop == iostack)
				        ioecho(c);
			        return(e.iop->prev = c);
		        }
		        else if (e.iop->task == XIO && e.iop->prev != '\n') {
			        e.iop->prev = 0;
				if (e.iop == iostack)
					ioecho('\n');
			        return '\n';
		        }
		    }
		    if (e.iop->task == XIO) {
			if (multiline)
			    return e.iop->prev = 0;
			if (interactive && e.iop == iostack+1) {
				gethostname(current_prompt, MAXHOSTNAMELEN);
				strcat(current_prompt, "# ");
			}
		    }
		}
	if (e.iop >= iostack)
		return(0);
	leave();
	/* NOTREACHED */
	return(0);
}

static void
ioecho(c)
char c;
{
	if (flag['v'])
		write(2, &c, sizeof c);
}

static void
pushio(struct ioarg *argp, int (*fn)(struct ioarg *))
{
	if (++e.iop >= &iostack[NPUSH]) {
		e.iop--;
		err("Shell input nested too deeply");
		gflg++;
		return;
	}
	e.iop->iofn = (int (*)(struct  ioarg *, struct io *))fn;

	if (argp->afid != AFID_NOBUF)
	  e.iop->argp = argp;
	else {
	  e.iop->argp  = ioargstack + (e.iop - iostack);
	  *e.iop->argp = *argp;
	  e.iop->argp->afbuf = e.iop == &iostack[0] ? &mainbuf : &sharedbuf;
	  if (isatty(e.iop->argp->afile) == 0 &&
	      (e.iop == &iostack[0] ||
	       lseek(e.iop->argp->afile, 0L, 1) != -1)) {
	    if (++bufid == AFID_NOBUF)
	      bufid = AFID_ID;
	    e.iop->argp->afid  = bufid;
	  }
	}

	e.iop->prev  = ~'\n';
	e.iop->peekc = 0;
	e.iop->xchar = 0;
	e.iop->nlcount = 0;
	if (fn == filechar)
		e.iop->task = XIO;
	else if (fn == (int(*)(struct ioarg *))gravechar || fn == (int(*)(struct ioarg *))qgravechar)
		e.iop->task = XGRAVE;
	else
		e.iop->task = XOTHER;
}

static struct io *
setbase(ip)
struct io *ip;
{
	register struct io *xp;

	xp = e.iobase;
	e.iobase = ip;
	return(xp);
}

/*
 * Input generating functions
 */

/*
 * Produce the characters of a string, then a newline, then EOF.
 */
static int
nlchar(ap)
register struct ioarg *ap;
{
	register int c;

	if (ap->aword == NULL)
		return(0);
	if ((c = *ap->aword++) == 0) {
		ap->aword = NULL;
		return('\n');
	}
	return(c);
}

/*
 * Given a list of words, produce the characters
 * in them, with a space after each word.
 */
static int
wdchar(ap)
register struct ioarg *ap;
{
	register char c;
	register char **wl;

	if ((wl = ap->awordlist) == NULL)
		return(0);
	if (*wl != NULL) {
		if ((c = *(*wl)++) != 0)
			return(c & 0177);
		ap->awordlist++;
		return(' ');
	}
	ap->awordlist = NULL;
	return('\n');
}

/*
 * Return the characters of a list of words,
 * producing a space between them.
 */
static int
dolchar(ap)
register struct ioarg *ap;
{
	register char *wp;

	if ((wp = *ap->awordlist++) != NULL) {
		PUSHIO(aword, wp, *ap->awordlist == NULL? strchar: xxchar);
		return(-1);
	}
	return(0);
}

static int
xxchar(ap)
register struct ioarg *ap;
{
	register int c;

	if (ap->aword == NULL)
		return(0);
	if ((c = *ap->aword++) == '\0') {
		ap->aword = NULL;
		return(' ');
	}
	return(c);
}

/*
 * Produce the characters from a single word (string).
 */
static int
strchar(ap)
register struct ioarg *ap;
{
	register int c;

	if (ap->aword == NULL || (c = *ap->aword++) == 0)
		return(0);
	return(c);
}

/*
 * Produce quoted characters from a single word (string).
 */
static int
qstrchar(ap)
register struct ioarg *ap;
{
	register int c;

	if (ap->aword == NULL || (c = *ap->aword++) == 0)
		return(0);
	return(c|QUOTE);
}

/*
 * Return the characters from a file.
 */
static int
filechar(ap)
register struct ioarg *ap;
{
	register int i;
	char c;
	struct iobuf *bp = ap->afbuf;

	if (ap->afid != AFID_NOBUF) {
	  if ((i = ap->afid != bp->id) || bp->bufp == bp->ebufp) {
	    if (i)
	      lseek(ap->afile, ap->afpos, 0);
	    i = safe_read(ap->afile, bp->buf, sizeof(bp->buf));
	    if (i <= 0) {
	      closef(ap->afile);
	      return 0;
	    }
	    bp->id = ap->afid;
	    bp->ebufp = (bp->bufp  = bp->buf) + i;
	  }
	  ap->afpos++;
	  return *bp->bufp++ & 0177;
	}

	if (interactive && isatty(ap->afile)) 
	{
	    static char mycommand[BUFSIZ];
	    static int position = 0, size = 0;

	    while (size == 0 || position >= size)
		{
		    splash_cmdedit_read_input(current_prompt, mycommand);
		    //cmdedit_read_input(current_prompt, mycommand);
		    size = strlen(mycommand);
		    position = 0;
	    }
	    c = mycommand[position];
	    position++;
	    return(c);
	} 
	else 
	{
		i = safe_read(ap->afile, &c, sizeof(c));
		return(i == sizeof(c)? c&0177: (closef(ap->afile), 0));
	}
}

/*
 * Return the characters from a here temp file.
 */
static int
herechar(ap)
register struct ioarg *ap;
{
	char c;


	if (read(ap->afile, &c, sizeof(c)) != sizeof(c)) {
		close(ap->afile);
		c = 0;
	}
	return (c);

}

/*
 * Return the characters produced by a process (`...`).
 * Quote them if required, and remove any trailing newline characters.
 */
static int
gravechar(ap, iop)
struct ioarg *ap;
struct io *iop;
{
	register int c;

	if ((c = qgravechar(ap, iop)&~QUOTE) == '\n')
		c = ' ';
	return(c);
}

static int
qgravechar(ap, iop)
register struct ioarg *ap;
struct io *iop;
{
	register int c;

	if (iop->xchar) {
		if (iop->nlcount) {
			iop->nlcount--;
			return('\n'|QUOTE);
		}
		c = iop->xchar;
		iop->xchar = 0;
	} else if ((c = filechar(ap)) == '\n') {
		iop->nlcount = 1;
		while ((c = filechar(ap)) == '\n')
			iop->nlcount++;
		iop->xchar = c;
		if (c == 0)
			return(c);
		iop->nlcount--;
		c = '\n';
	}
	return(c!=0? c|QUOTE: 0);
}

static void
prs(s)
register char *s;
{
	if (*s)
		write(2, s, strlen(s));
}

static void
prn(u)
unsigned u;
{
	prs(itoa(u));
}

static void
closef(i)
register int i;
{
	if (i > 2)
		close(i);
}

static void
closeall()
{
	register int u;

	for (u=NUFILE; u<NOFILE;)
		close(u++);
}

/*
 * remap fd into Shell's fd space
 */
static int
remap(fd)
register int fd;
{
	register int i;
	int map[NOFILE];

	if (fd < e.iofd) {
		for (i=0; i<NOFILE; i++)
			map[i] = 0;
		do {
			map[fd] = 1;
			fd = dup(fd);
		} while (fd >= 0 && fd < e.iofd);
		for (i=0; i<NOFILE; i++)
			if (map[i])
				close(i);
		if (fd < 0)
			err("too many files open in shell");
	}
	return(fd);
}

static int
openpipe(pv)
register int *pv;
{
	register int i;

	if ((i = pipe(pv)) < 0)
		err("can't create pipe - try again");
	return(i);
}

static void
closepipe(pv)
register int *pv;
{
	if (pv != NULL) {
		close(*pv++);
		close(*pv);
	}
}

/* -------- here.c -------- */

/*
 * here documents
 */

static void
markhere(s, iop)
register char *s;
struct ioword *iop;
{
	register struct here *h, *lh;

	h = (struct here *) space(sizeof(struct here));
	if (h == 0)
		return;
	h->h_tag = evalstr(s, DOSUB);
	if (h->h_tag == 0)
		return;
	h->h_iop = iop;
	iop->io_name = 0;
	h->h_next = NULL;
	if (inhere == 0)
		inhere = h;
	else
		for (lh = inhere; lh!=NULL; lh = lh->h_next)
			if (lh->h_next == 0) {
				lh->h_next = h;
				break;
			}
	iop->io_flag |= IOHERE|IOXHERE;
	for (s = h->h_tag; *s; s++)
		if (*s & QUOTE) {
			iop->io_flag &= ~ IOXHERE;
			*s &= ~ QUOTE;
		}
	h->h_dosub = iop->io_flag & IOXHERE;
}

static void
gethere()
{
	register struct here *h, *hp;

	/* Scan here files first leaving inhere list in place */
	for (hp = h = inhere; h != NULL; hp = h, h = h->h_next)
	  readhere(&h->h_iop->io_name, h->h_tag, h->h_dosub? 0: '\'');

	/* Make inhere list active - keep list intact for scraphere */
	if (hp != NULL) {
	  hp->h_next = acthere;
	  acthere    = inhere;
	  inhere     = NULL;
	}
}

static void
readhere(name, s, ec)
char **name;
register char *s;
int ec;
{
	int tf;
	char tname[30] = ".splash_XXXXXX";
	register int c;
	jmp_buf ev;
	char myline [LINELIM+1];
	char *thenext;

	tf = mkstemp(tname);
	if (tf < 0)
		return;
	*name = strsave(tname, areanum);
	if (newenv(setjmp(errpt = ev)) != 0)
		unlink(tname);
	else {
		pushio(e.iop->argp, (int(*)(struct ioarg *))e.iop->iofn);
		e.iobase = e.iop;
		for (;;) {
		    if (interactive && e.iop <= iostack) {
			    gethostname(current_prompt, MAXHOSTNAMELEN);
			    strcat(current_prompt, "> ");
			}
			thenext = myline;
			while ((c = my_getc(ec)) != '\n' && c) {
				if (ec == '\'')
					c &= ~ QUOTE;
				if (thenext >= &myline[LINELIM]) {
					c = 0;
					break;
				}
				*thenext++ = c;
			}
			*thenext = 0;
			if (strcmp(s, myline) == 0 || c == 0)
				break;
			*thenext++ = '\n';
			write (tf, myline, (int)(thenext-myline));
		}
		if (c == 0) {
			prs("here document `"); prs(s); err("' unclosed");
		}
		quitenv();
	}
	close(tf);
}

/*
 * open here temp file.
 * if unquoted here, expand here temp file into second temp file.
 */
static int
herein(hname, xdoll)
char *hname;
int xdoll;
{
	register int hf;
	int tf;

#if __GNUC__
	/* Avoid longjmp clobbering */
	(void) &tf;
#endif
	if (hname == 0)
		return(-1);
	hf = open(hname, 0);
	if (hf < 0)
		return (-1);
	if (xdoll) {
		char c;
		char tname[30] = ".splash_XXXXXX";
		jmp_buf ev;
	
		tf = mkstemp(tname);
		if (tf < 0)
			return (-1);
		if (newenv(setjmp(errpt = ev)) == 0) {
			PUSHIO(afile, hf, herechar);
			setbase(e.iop);
			while ((c = subgetc(0, 0)) != 0) {
				c &= ~ QUOTE;
				write(tf, &c, sizeof c);
			}
			quitenv();
		} else
			unlink(tname);
		close(tf);
		tf = open(tname, 0);
		unlink(tname);
		return (tf);
	} else
		return (hf);
}

static void
scraphere()
{
	register struct here *h;

	for (h = inhere; h != NULL; h = h->h_next) {
		if (h->h_iop && h->h_iop->io_name)
		  unlink(h->h_iop->io_name);
	}
	inhere = NULL;
}

/* unlink here temp files before a freearea(area) */
static void
freehere(area)
int area;
{
	register struct here *h, *hl;

	hl = NULL;
	for (h = acthere; h != NULL; h = h->h_next)
		if (getarea((char *) h) >= area) {
			if (h->h_iop->io_name != NULL)
				unlink(h->h_iop->io_name);
			if (hl == NULL)
				acthere = h->h_next;
			else
				hl->h_next = h->h_next;
		} else
			hl = h;
}



/*
 * Copyright (c) 1987,1997, Prentice Hall
 * All rights reserved.
 * 
 * Redistribution and use of the MINIX operating system in source and
 * binary forms, with or without modification, are permitted provided
 * that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 * with the distribution.
 * 
 * Neither the name of Prentice Hall nor the names of the software
 * authors or contributors may be used to endorse or promote
 * products derived from this software without specific prior
 * written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS, AUTHORS, AND
 * CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL PRENTICE HALL OR ANY AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


/*
 * Splash command editing
 * by Instant802 Networks
 *
 * Based on Busybox 1.00-pre7 shells/cmdedit.c:
 */
/*
 * Termios command line History and Editting.
 *
 * Copyright (c) 1986-2003 may safely be consumed by a BSD or GPL license.
 * Written by:   Vladimir Oleynik <dzo@simtreas.ru>
 *
 * Used ideas:
 *      Adam Rogoyski    <rogoyski@cs.utexas.edu>
 *      Dave Cinege      <dcinege@psychosis.com>
 *      Jakub Jelinek (c) 1995
 *      Erik Andersen    <andersen@codepoet.org> (Majorly adjusted for busybox)
 *
 * This code is 'as is' with no warranty.
 *
 *
 */

/*
   Usage and Known bugs:
   Terminal key codes are not extensive, and more will probably
   need to be added. This version was created on Debian GNU/Linux 2.x.
   Delete, Backspace, Home, End, and the arrow keys were tested
   to work in an Xterm and console. Ctrl-A also works as Home.
   Ctrl-E also works as End.

   Small bugs (simple effect):
   - not true viewing if terminal size (x*y symbols) less
     size (prompt + editor`s line + 2 symbols)
   - not true viewing if length prompt less terminal width
 */

#ifdef CONFIG_LOCALE_SUPPORT
#define Isprint(c) isprint((c))
#else
#define Isprint(c) ( (c) >= ' ' && (c) != ((unsigned char)'\233') )
#endif

/* Maximum length of the linked list for the command line history */
#define MAX_HISTORY   50

static char *history[MAX_HISTORY+1]; /* history + current */
/* saved history lines */
static int n_history;
/* current pointer to history line */
static int cur_history;

#include <termios.h>
#define setTermSettings(fd,argp) tcsetattr(fd,TCSANOW,argp)
#define getTermSettings(fd,argp) tcgetattr(fd, argp);

/* Current termio and the previous termio before starting sh */
static struct termios initial_settings, new_settings;


static
volatile int cmdedit_termw = 80;        /* actual terminal width */
static
volatile int handlers_sets = 0; /* Set next bites: */

enum {
	SET_ATEXIT = 1,         /* when atexit() has been called
				   and get euid,uid,gid to fast compare */
	SET_WCHG_HANDLERS = 2,  /* winchg signal handler */
	SET_RESET_TERM = 4,     /* if the terminal needs to be reset upon exit */
};


static int cmdedit_x;           /* real x terminal position */
static int cmdedit_y;           /* pseudoreal y terminal position */
static int cmdedit_prmt_len;    /* lenght prompt without colores string */

static int cursor;              /* required global for signal handler */
static int len;                 /* --- "" - - "" - -"- --""-- --""--- */
static char *command_ps;        /* --- "" - - "" - -"- --""-- --""--- */
static const char *cmdedit_prompt; /* --- "" - - "" - -"- --""-- --""--- */


static int my_euid;
static int my_uid;
static int my_gid;

static void cmdedit_setwidth(int w, int redraw_flg);

static void win_changed(int nsig)
{
	static sighandler_t previous_SIGWINCH_handler;  /* for reset */

	/*   emulate      || signal call */
	if (nsig == -SIGWINCH || nsig == SIGWINCH) {
		int width = 0;
		get_terminal_width_height(0, &width, NULL);
		cmdedit_setwidth(width, nsig == SIGWINCH);
	}
	/* Unix not all standart in recall signal */

	if (nsig == -SIGWINCH)          /* save previous handler   */
		previous_SIGWINCH_handler = signal(SIGWINCH, win_changed);
	else if (nsig == SIGWINCH)      /* signaled called handler */
		signal(SIGWINCH, win_changed);  /* set for next call       */
	else                                            /* nsig == 0 */
		/* set previous handler    */
		signal(SIGWINCH, previous_SIGWINCH_handler);    /* reset    */
}

static void cmdedit_reset_term(void)
{
	if ((handlers_sets & SET_RESET_TERM) != 0) {
/* sparc and other have broken termios support: use old termio handling. */
		setTermSettings(fileno(stdin), (void *) &initial_settings);
		handlers_sets &= ~SET_RESET_TERM;
	}
	if ((handlers_sets & SET_WCHG_HANDLERS) != 0) {
		/* reset SIGWINCH handler to previous (default) */
		win_changed(0);
		handlers_sets &= ~SET_WCHG_HANDLERS;
	}
	fflush(stdout);
}


/* special for recount position for scroll and remove terminal margin effect */
static void cmdedit_set_out_char(int next_char)
{

	int c = (int)((unsigned char) command_ps[cursor]);

	if (c == 0)
		c = ' ';        /* destroy end char? */
#ifdef CONFIG_FEATURE_NONPRINTABLE_INVERSE_PUT
	if (!Isprint(c)) {      /* Inverse put non-printable characters */
		if (c >= 128)
			c -= 128;
		if (c < ' ')
			c += '@';
		if (c == 127)
			c = '?';
		printf("\033[7m%c\033[0m", c);
	} else
#endif
		putchar(c);
	if (++cmdedit_x >= cmdedit_termw) {
		/* terminal is scrolled down */
		cmdedit_y++;
		cmdedit_x = 0;

		if (!next_char)
			next_char = ' ';
		/* destroy "(auto)margin" */
		putchar(next_char);
		putchar('\b');
	}
	cursor++;
}

/* Move to end line. Bonus: rewrite line from cursor */
static void input_end(void)
{
	while (cursor < len)
		cmdedit_set_out_char(0);
}

/* Go to the next line */
static void goto_new_line(void)
{
	input_end();
	if (cmdedit_x)
		putchar('\n');
}


static inline void out1str(const char *s)
{
	if ( s )
		fputs(s, stdout);
}

static inline void beep(void)
{
	putchar('\007');
}

/* Move back one charactor */
/* special for slow terminal */
static void input_backward(int num)
{
	if (num > cursor)
		num = cursor;
	cursor -= num;          /* new cursor (in command, not terminal) */

	if (cmdedit_x >= num) {         /* no to up line */
		cmdedit_x -= num;
		if (num < 4)
			while (num-- > 0)
				putchar('\b');

		else
			printf("\033[%dD", num);
	} else {
		int count_y;

		if (cmdedit_x) {
			putchar('\r');          /* back to first terminal pos.  */
			num -= cmdedit_x;       /* set previous backward        */
		}
		count_y = 1 + num / cmdedit_termw;
		printf("\033[%dA", count_y);
		cmdedit_y -= count_y;
		/*  require  forward  after  uping   */
		cmdedit_x = cmdedit_termw * count_y - num;
		printf("\033[%dC", cmdedit_x);  /* set term cursor   */
	}
}

static void put_prompt(void)
{
	out1str(cmdedit_prompt);
	cmdedit_x = cmdedit_prmt_len;   /* count real x terminal position */
	cursor = 0;
	cmdedit_y = 0;                  /* new quasireal y */
}

static void parse_prompt(const char *prmt_ptr)
{
	cmdedit_prompt = prmt_ptr;
	cmdedit_prmt_len = strlen(prmt_ptr);
	put_prompt();
}

/**
 * redraw - draw prompt, editor line, and clear tail
 * @y: the line the terminal is on.
 * @back_cursor: the number of characters back from the end where the cursor
 *               should be placed.
 */
static void redraw(int y, int back_cursor)
{
	if (y > 0)                              /* up to start y */
		printf("\033[%dA", y);
	putchar('\r');
	put_prompt();
	input_end();                            /* rewrite */
	printf("\033[J");                       /* destroy tail after cursor */
	input_backward(back_cursor);
}

/* Delete the char in front of the cursor */
static void input_delete(void)
{
	int j = cursor;

	if (j == len)
		return;

	strcpy(command_ps + j, command_ps + j + 1);
	len--;
	input_end();                    /* rewtite new line */
	cmdedit_set_out_char(0);        /* destroy end char */
	input_backward(cursor - j);     /* back to old pos cursor */
}

/* Delete the char in back of the cursor */
static void input_backspace(void)
{
	if (cursor > 0) {
		input_backward(1);
		input_delete();
	}
}


/* Move forward one charactor */
static void input_forward(void)
{
	if (cursor < len)
		cmdedit_set_out_char(command_ps[cursor + 1]);
}


static void cmdedit_setwidth(int w, int redraw_flg)
{
	cmdedit_termw = cmdedit_prmt_len + 2;
	if (w <= cmdedit_termw) {
		cmdedit_termw = cmdedit_termw % w;
	}
	if (w > cmdedit_termw) {
		cmdedit_termw = w;

		if (redraw_flg) {
			/* new y for current cursor */
			int new_y = (cursor + cmdedit_prmt_len) / w;

			/* redraw */
			redraw((new_y >= cmdedit_y ? new_y : cmdedit_y), len - cursor);
			fflush(stdout);
		}
	}
}

static void cmdedit_init(void)
{
	cmdedit_prmt_len = 0;
	if ((handlers_sets & SET_WCHG_HANDLERS) == 0) {
		/* emulate usage handler to set handler and call yours work */
		win_changed(-SIGWINCH);
		handlers_sets |= SET_WCHG_HANDLERS;
	}

	if ((handlers_sets & SET_ATEXIT) == 0) {
		my_euid = geteuid();
		my_uid = getuid();
		my_gid = getgid();
		handlers_sets |= SET_ATEXIT;
		atexit(cmdedit_reset_term);     /* be sure to do this only once */
	}
}

#if 0
static int is_execute(const struct stat *st)
{
	if ((!my_euid && (st->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) ||
		(my_uid == st->st_uid && (st->st_mode & S_IXUSR)) ||
		(my_gid == st->st_gid && (st->st_mode & S_IXGRP)) ||
		(st->st_mode & S_IXOTH)) return TRUE;
	return FALSE;
}

#endif

enum {
	FIND_EXE_ONLY = 0,
	FIND_DIR_ONLY = 1,	
	FIND_ARG_ONLY = 3,
};

/**
 * path_parse - Parse the splash_path variable.
 * @p: Will be set to point to an array of string pointers.
 * Returns: The number of directories in @p.
 */

#if 0
static int path_parse(char ***p)
{
	int npth;
	const char *tmp;
	//const char *pth = splash_path->value;
	
	const char *pth = "/splashbin:/sbin:/bin:/usr/sbin:/usr/bin";

	if (pth == NULL 
	    || *pth == '\0' /* PATH=<empty> */
	    || (*pth == ':' && *(pth + 1) == '\0') /* PATH=:<empty> */) {
		printf("Unable to get path %s.\n", pth);
		return 0;
	}

	tmp = pth;
	npth = 0;

	/* Count the number of directories. */
	for (;;) {
		npth++;                 /* count words is + 1 count ':' */
		tmp = strchr(tmp, ':');
		if (tmp != NULL) {
			if (*++tmp == '\0') {
				break;  /* :<empty> */
			}
		} else {
			break;
		}
	}

	*p = xmalloc(npth * sizeof(char *));

	tmp = pth;
	(*p)[0] = xstrdup(tmp);
	npth = 1;                       /* count words is + 1 count ':' */

	for (;;) {
		tmp = strchr(tmp, ':');
		if (tmp) {
			(*p)[0][(tmp - pth)] = '\0';    /* ':' -> '\0' */
			if (*++tmp == 0)
				break;                  /* :<empty> */
		} else
			break;
		(*p)[npth++] = &(*p)[0][(tmp - pth)];   /* p[next]=p[0][&'\0'+1] */
	}


	return npth;
}
#endif

#if 0
static char *add_quote_for_spec_chars(char *found)
{
	int l = 0;
	char *s = xmalloc((strlen(found) + 1) * 2);

	while (*found) {
		if (strchr(" `\"#$%^&*()=+{}[]:;\'|\\<>", *found))
			s[l++] = '\\';
		s[l++] = *found++;
	}
	s[l] = 0;
	return s;
}

/**
 * exe_tab_completion - Perform executable tab completion.
 * @cmd: the command to complete.
 * @num_matches: set to the number of matches (elements in the return value).
 * Returns: A pointer to a newly allocated array of strings
 *          that are the possible matches for @cmd.
 */
#endif

#if 0
static char **exe_tab_completion(char *cmd, int *num_matches)
{

	char **matches = 0;
	DIR *dir;
	struct dirent *next_dir;
	char dirbuf[BUFSIZ];
	int nm = *num_matches;
	struct stat st;
	char **paths;
	int npaths;
	int i;
	char *found;

	npaths = path_parse(&paths);

	printf("---------cmd=%s\n",cmd);

	for (i = 0; i < npaths; i++)
	{		
		dir = opendir(paths[i]);
		
		if (!dir)                       /* Don't print an error */
			continue;

		while ((next_dir = readdir(dir)) != NULL) 
		{
			char *str_found = next_dir->d_name;

			/* matched ? */
			if (strncmp(str_found, cmd, strlen(cmd)) != 0)
			{
				continue;
			}

			/* not see .name without .match */
			if (*str_found == '.' && *cmd == '\0') 
			{
				if (*paths[i] == '/' && paths[i][1] == 0
					&& str_found[1] == 0) 
					str_found = "";   /* only "/" */
				else
					continue;
			}
			
			found = concat_path_file(paths[i], str_found);
			/* hmm, remover in progress? */
			if (stat(found, &st) < 0)
				goto cont;
			/* find with dirs ? */
			if (paths[i] != dirbuf)
				strcpy(found, next_dir->d_name);/* only name */
			if (S_ISDIR(st.st_mode))
			{
				/* name is directory      */
				str_found = found;
				found = concat_path_file(found, "");
				free(str_found);
				str_found = add_quote_for_spec_chars(found);
			} 
			else 
			{
				/* not put found file if search only dirs for cd */
				str_found = add_quote_for_spec_chars(found);
				if (is_execute(&st)) {
					strcat(str_found, " ");
				}
			}
			/* Add it to the list */
			matches = xrealloc(matches, (nm + 1) * sizeof(char *));

			matches[nm++] = str_found;
cont:
			free(found);
		}
		closedir(dir);
	}
	free(paths[0]);                 /* allocated memory only in first member */
	free(paths);
	*num_matches = nm;
	return (matches);
}

#endif


 char *cli_cmd[][6]={
 {"network","route","add","default","",NULL},
 {"network","show","dnsnameserver",NULL,NULL,NULL},
 {"network","config","dnsmode","",NULL,NULL},
 {"network","config","dnsnameserver1","",NULL,NULL},
 {"network","config","dnsnameserver2","",NULL,NULL},
 {"network","show","ntpserver",NULL,NULL,NULL},
 {"network","config","ntpmode","",NULL,NULL},
 {"network","config","ntpserver1","",NULL,NULL},
 {"network","config","ntpserver2","",NULL,NULL},
 {"network","config","ipmode","",NULL,NULL},
 {"network","","ip","","netmask",""},
 {"network","","up",NULL,NULL,NULL},	
 {"network","","down",NULL,NULL,NULL},
 {"network","show","ipinfo",NULL,NULL,NULL},
 {"network","show","status",NULL,NULL,NULL},
 {"network","show","rate",NULL,NULL,NULL},
 {"wireless","show","status",NULL,NULL,NULL},
 {"wireless","create","","",NULL,NULL},
 {"wireless","destroy","",NULL,NULL,NULL},
 {"wireless","config","","ssid","",NULL},
 {"wireless","config","","channel","",NULL},
 {"wireless","config","","mode","",NULL},
 {"wireless","config","","hide_ssid","",NULL},
 {"wireless","config","","rate","",NULL},
 {"wireless","config","","bintval","",NULL},
 {"wireless","config","","dtim_period","",NULL},
 {"wireless","config","","frag","",NULL},
 {"wireless","config","","txpower","",NULL},
 {"wireless","config","","rts","",NULL},
 {"wireless","config","","wmm","",NULL},
 {"wireless","config","","noackpolicy","",NULL},
 {"wireless","config","","isolation","",NULL},
 {"wireless","config","","maxsimultusers","",NULL},
 {"wireless","config","","vlan","",NULL},
 {"wireless","config","","vlanvid","",NULL},
 {"wireless","show","maxsimultusers","",NULL,NULL},
 {"wireless","show","mode","",NULL,NULL},
 {"wireless","show","security","",NULL,NULL},
 {"wireless","show","channelmode","",NULL,NULL},
 {"wireless","show","txpower","",NULL,NULL},
 {"wireless","show","isolation","",NULL,NULL}, 
 {"wireless","show","broadcastssid","",NULL,NULL},
 {"wireless","show","ssid","",NULL,NULL},
 {"wireless","show","vlanenable","",NULL,NULL},
 {"wireless","show","vlanvid","",NULL,NULL},
 //{"wireless","security","","wepkey","",NULL},
 {"wireless","security","","type","",NULL},
 {"wireless","security","","mode","",NULL},
 {"wireless","security","","encryption","",NULL},
 {"wireless","security","","wpakey","",NULL},
 {"wireless","security","","wepkey[1]","ascii",""},
 {"wireless","security","","wepkey[1]","hex",""},
 {"wireless","security","","wepkey[2]","ascii",""},
 {"wireless","security","","wepkey[2]","hex",""},
 {"wireless","security","","wepkey[3]","ascii",""},
 {"wireless","security","","wepkey[3]","hex",""},
 {"wireless","security","","wepkey[4]","ascii",""},
 {"wireless","security","","wepkey[4]","hex",""},
 {"wireless","show","macfilter","",NULL,NULL},
 {"wireless","security","","macfilter","",NULL},
 {"wireless","security","","macfilter","mode",""},
 {"wireless","security","","macfilter","addmac",""},
 {"wireless","security","","macfilter","delmac",NULL},
 {"wireless","show","macfilter","",NULL,NULL},
 {"wireless","show","radius",NULL,NULL,NULL},
 {"wireless","radius","authip","",NULL,NULL},
 {"wireless","radius","authport","",NULL,NULL},
 {"wireless","radius","accountip","",NULL,NULL},
 {"wireless","radius","accountport","",NULL,NULL},
 {"wireless","radius","sharekey","",NULL,NULL},
 {"wireless","radiusb","authip","",NULL,NULL},
 {"wireless","radiusb","authport","",NULL,NULL},
 {"wireless","radiusb","accountip","",NULL,NULL},
 {"wireless","radiusb","accountport","",NULL,NULL},
 {"wireless","radiusb","sharekey","",NULL,NULL}, 
 {"wireless","rogueap","",NULL,NULL,NULL},
 {"wireless","show","rogueap",NULL,NULL,NULL},
 {"wireless","show","rogueap",NULL,NULL,NULL},
 {"wireless","config","","wds","",NULL},
 {"wireless","show","","stationlist",NULL,NULL},
 {"stp","show",NULL,NULL,NULL,NULL},
 {"stp","config","","",NULL,NULL},
 {"stp","config","","bridgeprio","",NULL},
 {"stp","config","","setfd","",NULL},
 {"stp","config","","maxage","",NULL},
 {"stp","config","","hello","",NULL},
 #if 0
 {"vlan","create","",NULL,NULL,NULL},
 {"vlan","destroy","",NULL,NULL,NULL},
 {"vlan","addport","","",NULL,NULL},
 {"vlan","delport","","",NULL,NULL},
 #endif
 //{"system","show",NULL,NULL,NULL,NULL},
 {"system","upgrade","","",NULL,NULL},
 {"system","backup","","",NULL,NULL},
 {"system","backup",NULL,"",NULL,NULL},
 {"system","reset",NULL,NULL,NULL,NULL},
 {"system","reboot",NULL,NULL,NULL,NULL},
 {"system","ping","",NULL,NULL,NULL},
 {"system","show","version",NULL,NULL,NULL},
 {"system","show","memory",NULL,NULL,NULL},
 {"system","show","flashinfo",NULL,NULL,NULL},
 {"system","show","runtime",NULL,NULL,NULL},
 {"system","show","workmode",NULL,NULL,NULL},
 {"system","show","cpuratio",NULL,NULL,NULL},
 {"system","show","temperature",NULL,NULL,NULL},
 //#if 0
 {"snmp","",NULL,NULL,NULL,NULL},
 {"snmp","show",NULL,NULL,NULL,NULL},
 {"snmp","receivetrap","",NULL,NULL,NULL},
 {"snmp","syslocation","",NULL,NULL,NULL},
 {"snmp","syscontact","",NULL,NULL,NULL},
 {"snmp","publiccn","",NULL,NULL,NULL},
 {"snmp","privatecn","",NULL,NULL,NULL},
 {"snmp","trapscn","",NULL,NULL,NULL},
 //#endif
 {"syslog","show",NULL,NULL,NULL,NULL},
 {"syslog","config","serverip",NULL,NULL,NULL},
 {"syslog","config","serverport",NULL,NULL,NULL},
 {"syslog","",NULL,NULL,NULL,NULL},
 {"help","network",NULL,NULL,NULL,NULL},
 {"help","wireless",NULL,NULL,NULL,NULL},
 {"help","stp",NULL,NULL,NULL,NULL},
 {"help","vlan",NULL,NULL,NULL,NULL},
 {"help","system",NULL,NULL,NULL,NULL},
 {"help","snmp",NULL,NULL,NULL,NULL},
 {"help","syslog",NULL,NULL,NULL,NULL}
 };

char str_fore[20]={0};

static int find_match(char *matchBuf, int *len_with_quotes);

/*wangjr  08-09-25*/
static char **exe_tab_completion(char *cmd_all, int *num_matches)
{
	char **matches = 0;
	int nm = *num_matches;
	int i;
	int j;
	int k;
	int key_num=1;
	char cmd[6][100]={"","","","","",""};
	int count=0;
	int start=0;
	int end=0;
	int matchtrue=0;
	for(j=0;j<strlen(cmd_all);j++)
	{
	   if((cmd_all[j]!=32)&&(cmd_all[j+1]==32))
	   {
         key_num++; 
	   }
	}
	
    if(key_num == 1)
    {
       strncpy( cmd[0] , cmd_all,strlen(cmd_all));
    }

    else
    {
        for(j=1;j<=key_num;j++)
        {
            k=0;
		    if(j == 1)
		    {
		
		        for(i=0;i<=strlen(cmd_all);i++)
		        {
		            if(cmd_all[i]==32)
		  	            break;
		            cmd[0][i]=cmd_all[i];
		        }		    
		    }
	        else if(j == key_num)
	        { 
	            for(i=strlen(cmd_all)-1;i>=0;i--)
	            {
			        if(cmd_all[i] == 32)
			        {
                        start=i;
				        break;	
			        }
	            }
			    k=0;
			    for(i=start+1;i<strlen(cmd_all);i++)
			    {
			        cmd[j-1][k]=cmd_all[i];
			        k++;
			    }			   	  
	       }
		   else 
	       {
	           count=0;
	           for(i=0; i<=strlen(cmd_all); i++)
	           {
	        
			       if(cmd_all[i]!=32 && cmd_all[i-1]==32)
			       {
			           count++;
			           if(count == j-1)
			               start=i;
			           if(count == j)
			           {
			               end =i;
			               break;
			           }    
	               }
	           }
			   k=0;
		       for(i=start;i<end;i++)
		       {
		           cmd[j-1][k]=cmd_all[i];
		           k++;
		       }
			   for(i=strlen(cmd[j-1]);i>0;i--)
			   {
			     if(cmd[j-1][i-1]==32)
				 	cmd[j-1][i-1]= 0;
				 else
				 	break;	 	
			   }
	       }
	  
	   }

   }

	
	for (i = 0; i < (sizeof(cli_cmd)/sizeof(cli_cmd[0])); i++)
	{		 
	        matchtrue=0;
	        char *str_found = NULL;
	        for(j=1;(j<=key_num)&&(j<=6);j++)
	        {
	          if(cli_cmd[i][j-1] != NULL)
	          {
			     str_found = cli_cmd[i][j-1];
				 if(strcmp(str_found,"")!=0)
				 {
			         if (strncmp(str_found, cmd[j-1], strlen(cmd[j-1])) != 0)
		  	             break;
				 }
			      matchtrue++;
	          }
			  
	        }
			if(matchtrue< key_num)
				continue;
	        		
			/* Add it to the list */
			if(strcmp(str_found,"")!=0)
			{
		    char *tmp = (char *)malloc(strlen(str_found)+1);
		    strcpy(tmp, str_found);
		    matches = xrealloc(matches, (nm + 1) * sizeof(char *));
		    matches[nm++] = tmp;
			}
	}

	
	*num_matches = nm;
	return (matches);	
}



#if 0
/**
 * read_all - Reads all available data from a stream.
 * @buffer: will be set to a newly allocated buffer containing the data.
 *          The size is guaranteed to be at least @buffer_len + 1,
 *          to allow the caller to NULL-terminate the data.
 * @buffer_len: will be set to the amount of data in @buffer.
 * @fd: the file descriptor to read the data from.
 * Returns: 0 if successful, non-0 otherwise.
 */
static int read_all(char **buffer, int *buffer_len, int fd)
{
#define ALLOC_SIZE 512
	char *buf = malloc(ALLOC_SIZE);
	if (buf == NULL) 
	{
		perror("malloc");
		return 1;
	}
	int buf_size = ALLOC_SIZE;
	int buf_len = 0;

	int n;
	while ((n = read(fd, buf + buf_len, buf_size - buf_len)) > 0) 
	{
		buf_len += n;

		if (buf_len == buf_size) 
		{
			char *newbuf;

			if (!(newbuf = realloc(buf, buf_size + ALLOC_SIZE))) 
			{
				perror("realloc");
				free(buf);
				return 2;
			}
			buf_size += ALLOC_SIZE;
			buf = newbuf;
		}
	}
	
	if (n == -1) 
	{
		perror("read");
		free(buf);
		return 3;
	}

	*buffer = buf;
	*buffer_len = buf_len;
	return 0;
}

#endif

#if 0
						/**
 * forkexecvc - Forks and executes a file, capturing its output.
 * @cmdpath: the path to the file to execute.
 * @argv:    the arguments to pass to the command.
 *           Must be terminated by a NULL argument.
 * @buf:     will be set to a newly allocated buffer containing the output.
 *           The size is guaranteed to be at least @buffer_len + 1,
 *           to allow the caller to NULL-terminate the data.
 * @buf_len: will be set to the length of the output buffer.
 */
static int forkexecvc(const char *cmdpath, char *const argv[],
		      char **buf, int *buf_len)
{
	int pid, status;
	int pipe_filedes[2];

	if (cmdpath == NULL) {
		return 1;
	}

	if (lookup("DEBUG")
	    && lookup("DEBUG")->value
	    && strcmp(lookup("DEBUG")->value, "1") == 0) {
		printf("forkexecvc: %s:", cmdpath);
		int i;
		for (i=0; argv[i] != NULL; i++) {
			printf(" \"%s\"", argv[i]);
		}
		printf("\n");
	}

	if (pipe(pipe_filedes) == -1) {
		perror("pipe");
		return 2;
	}

	pid = fork();
	if (pid == -1) {
		close(pipe_filedes[0]);
		close(pipe_filedes[1]);
		return -1;
	}
	if (pid == 0) {
		dup2(pipe_filedes[1], 1);
		close(pipe_filedes[0]);
		close(pipe_filedes[1]);

		execv(cmdpath, argv);
		exit(127);
	}

	waitpid(pid, &status, 0);
	close(pipe_filedes[1]);
	
	int r = read_all(buf, buf_len, pipe_filedes[0]);

	if (lookup("DEBUG")
	    && lookup("DEBUG")->value
	    && strcmp(lookup("DEBUG")->value, "1") == 0) {
		int i;
		printf("Returned: \"");
		for (i = 0; i < *buf_len; i++) {
			putchar((*buf)[i]);
		}
		printf("\"\n");
	}

	close(pipe_filedes[0]);
	return r;
}

#endif

#if 0
/**
 * split - Split a string containing delimeters into an array of strings.
 * @string:  the string containing delimeters.
 * @delim:   the delimiter.
 * @strings: will be set to point to a newly allocated array of
 *           newly allocated strings.
 * Description: Each sub-string of zero or more characters before or
 *              after a delimeter will become an element of the array
 *              of strings.
 * Examples: With a delim of ':', the following splits will be returned:
 *           "" ->   -> ""
 *           "a"     -> "a"
 *           ":"     -> "", ""
 *           "a:"    -> "a", ""
 *           ":a"    -> "", "a"
 *           "::"    -> "", "", ""
 *           "a:b"   -> "a", "b"
 *           "a:b:"  -> "a", "b", ""
 *           ":a:b"  -> "", "a", "b"
 *           ":a:b:" -> "", "a", "b", ""
 * Returns: the number of elements in the array of strings.
 */
static int split(char *string, char delim, char ***strings)
{
	int num_strings = 0;
	char *p = string;
	while (1) {
		num_strings++;
		p = strchr(p, delim);
		if (p == NULL) {
			break;
		}
		p++;
	}

	*strings = malloc(num_strings * sizeof(char *));

	p = string;

	int i;
	for (i=0; i < num_strings; i++) {
		char *q = strchr(p, delim);
		if (q == NULL) {
			q = string + strlen(string);
		}

		(*strings)[i] = malloc((q-p+1) * sizeof(char));
		strncpy((*strings)[i], p, q-p);
		(*strings)[i][q-p] = '\0';

		if (*q == '\0') {
			break;
		}

		p = q+1;
	}

	return num_strings;
}

#endif

#if 0
/**
 * get_completions - Gets the completions of a command line.
 * @cmd_line: the command line to get the completions of.
 * Returns: a newly allocated string containing a list of newline
 *          separated completions for the last token in @cmd_line.
 */
static char *get_completions(char *cmd_line)
{
	char cmdpath[80];

	char **cmd_line_split;
	int cmd_line_split_len = split(cmd_line, ' ', &cmd_line_split);
	
    printf("cmd_line_split[0]=%s\n",cmd_line_split[0]);

	sprintf(cmdpath, "/bin/%s", cmd_line_split[0]);

	char **argv;
	argv = malloc((cmd_line_split_len + 2) * sizeof(char *));

	argv[0] = cmd_line_split[0];
	argv[1] = "--complete";
	int i;
	for (i = 1; i < cmd_line_split_len; i++) 
	{
		argv[i+1] = cmd_line_split[i];
	}
	argv[cmd_line_split_len + 1] = NULL;
	
	char *buf;
	int buf_len;

	forkexecvc(cmdpath, argv, &buf, &buf_len);
	buf[buf_len] = '\0'; // forkexecvc guarantees buf[buf_len] is allocated
	if (buf_len > 0) {
		buf[buf_len-1] = '\0'; /* Discard trailing newline. */
	}

	while (cmd_line_split_len > 0) {
		free(cmd_line_split[--cmd_line_split_len]);
	}
	printf("buf==%s\n",buf);
	free(cmd_line_split);
	free(argv);
	return buf;
}
#endif


#if 0
/**
 * arg_tab_completion - Perform argument tab completion.
 * @arg:         the argument to complete.
 * @num_matches: set to the number of matches (elements in the return value).
 *               Must be 0 before this is called.
 * @cmdline:     the whole command line.
 * Returns: A pointer to a newly allocated array of strings
 *          that are the possible matches for @arg.
 */
static char **arg_tab_completion(char *arg, int *num_matches, char *cmd_line)
{
	char **completions_split;
	char *completions = get_completions(cmd_line);

	*num_matches = split(completions, '\n', &completions_split);
	free(completions);

	if (*num_matches == 1 && strlen(completions_split[0]) == 0) {
		free(completions_split[0]);
		free(completions_split);
		completions_split = NULL;
		*num_matches = 0;
	}

	return completions_split;
}

#endif

static int match_compare(const void *a, const void *b)
{
	return strcmp(*(char **) a, *(char **) b);
}



#define QUOT    (UCHAR_MAX+1)

#define collapse_pos(is, in) { \
	memcpy(int_buf+(is), int_buf+(in), (BUFSIZ+1-(is)-(in))*sizeof(int)); \
	memcpy(pos_buf+(is), pos_buf+(in), (BUFSIZ+1-(is)-(in))*sizeof(int)); }

/**
 * find_match - Find the token to match for completion, and the type of match.
 * @matchBuf: the prefix of the command to match.
 *            Removes all the tokens but the one to be matched.
 *            Also removes any quotes around that token.
 * @len_with_quotes: set to the length of the new @matchBuf,
 *                   if it had the quotes (that were removed).
 * Returns: the find type - FIND_EXE_ONLY or FIND_ARG_ONLY.
 */
static int find_match(char *matchBuf, int *len_with_quotes)
{
	int i, j;
	int command_mode;
	int c, c2;
	int int_buf[BUFSIZ + 1]; /* matchBuf decoded - a backslash followed
				    by a character is replaced by just
				    the character ORred with QUOT. */
	int pos_buf[BUFSIZ + 1]; /* The original index of each element of
				    int_buf in matchBuf. */

	/* set to integer dimension characters and own positions */
	for (i = 0;; i++) {
		int_buf[i] = (int) ((unsigned char) matchBuf[i]);
		if (int_buf[i] == 0) {
			pos_buf[i] = -1;        /* indicator end line */
			break;
		} else {
			pos_buf[i] = i;
		}
	}

	/* mask \+symbol and convert '\t' to ' ' */
	for (i = j = 0; matchBuf[i]; i++, j++)
		if (matchBuf[i] == '\\') {
			collapse_pos(j, j + 1);
			int_buf[j] |= QUOT;
			i++;
#ifdef CONFIG_FEATURE_NONPRINTABLE_INVERSE_PUT
			if (matchBuf[i] == '\t')        /* algorithm equivalent */
				int_buf[j] = ' ' | QUOT;
#endif
		}
#ifdef CONFIG_FEATURE_NONPRINTABLE_INVERSE_PUT
		else if (matchBuf[i] == '\t')
			int_buf[j] = ' ';
#endif

	/* mask "symbols" or 'symbols' */
	/* ORs all characters (except $) inside the quotes with QUOT */
	c2 = 0; /* type of quote we're inside, or 0 for not inside quote */
	for (i = 0; int_buf[i]; i++) {
		c = int_buf[i];
		if (c == '\'' || c == '"') {
			if (c2 == 0) {
				c2 = c;
			} else {
				if (c == c2) {
					c2 = 0;
				} else {
					int_buf[i] |= QUOT;
				}
			}
		} else if (c2 != 0 && c != '$')
			int_buf[i] |= QUOT;
	}

	/* skip first not quote space */
	for (i = 0; int_buf[i]; i++)
		if (int_buf[i] != ' ')
			break;
	if (i)
		collapse_pos(0, i);

	/* set find mode for completion */
	command_mode = FIND_EXE_ONLY;
	for (i = 0; int_buf[i]; i++) {
		if (int_buf[i] == ' ') {
			command_mode = FIND_ARG_ONLY;
			break;
		}
	}

	/* "strlen" */
	for (i = 0; int_buf[i]; i++);
	/* find last word */
	for (--i; i >= 0; i--) {
		c = int_buf[i];
		if (c == ' ') {
			collapse_pos(0, i + 1);
			break;
		}
	}
	/* skip first not quoted '\'' or '"' */
	for (i = 0; int_buf[i] == '\'' || int_buf[i] == '"'; i++);
	/* collapse quote or unquote // or /~ */
	while ((int_buf[i] & ~QUOT) == '/' &&
			((int_buf[i + 1] & ~QUOT) == '/'
			 || (int_buf[i + 1] & ~QUOT) == '~')) {
		i++;
	}

	/* set only match and destroy quotes */
	j = 0;
	for (c = 0; pos_buf[i] >= 0; i++) {
		matchBuf[c++] = matchBuf[pos_buf[i]];
		j = pos_buf[i] + 1;
	}
	matchBuf[c] = 0;
	/* old lenght matchBuf with quotes symbols */
	*len_with_quotes = j ? j - pos_buf[0] : 0;

	return command_mode;
}

#if 0
/*
   display by column original ideas from ls applet,
   very optimize by my :)
*/
static void showfiles(char **matches, int nfiles)
{
	int ncols, row;
	int column_width = 0;
	int nrows = nfiles;

	/* find the longest file name-  use that as the column width */
	for (row = 0; row < nrows; row++) {
		int l = strlen(matches[row]);

		if (column_width < l)
			column_width = l;
	}
	column_width += 2;              /* min space for columns */
	ncols = cmdedit_termw / column_width;

	if (ncols > 1) {
		nrows /= ncols;
		if(nfiles % ncols)
			nrows++;        /* round up fractionals */
		column_width = -column_width;   /* for printf("%-Ns", ...); */
	} else {
		ncols = 1;
	}
	for (row = 0; row < nrows; row++) {
		int n = row;
		int nc;

		for(nc = 1; nc < ncols && n+nrows < nfiles; n += nrows, nc++)
			printf("%*s", column_width, matches[n]);
		printf("%s\n", matches[n]);
	}
}

#endif

#if 0
/**
 * Get a summary of a match.
 * @match:    the match to get a summary of.
 * @cmd_line: the entire command.
 *            The last token should be substituted with @cmd.
 * Returns: A newly allocated string containing the summary.
 *          Should be one-line, not newline terminated.
 */
static char *get_summary(char *match, char *cmd_line)
{
	char cmdpath[80];

	char **cmd_line_split;
	int cmd_line_split_len = split(cmd_line, ' ', &cmd_line_split);

	char *cmd;
	if (cmd_line_split_len == 1) {
		cmd = match;
	} else {
		cmd = cmd_line_split[0];
	}

	sprintf(cmdpath, "/splashbin/%s", cmd);

	char **argv;
	argv = malloc((cmd_line_split_len + 2) * sizeof(char *));

	argv[0] = cmd;
	argv[1] = "--summary";
	int i;
	for (i = 1; i < cmd_line_split_len - 1; i++) {
		argv[i+1] = cmd_line_split[i];
	}
	if (cmd_line_split_len > 1) {
		argv[cmd_line_split_len] = match;
	}
	argv[cmd_line_split_len + 1] = NULL;

	char *buf;
	int buf_len;

	forkexecvc(cmdpath, argv, &buf, &buf_len);
	buf[buf_len] = '\0';

	while (cmd_line_split_len > 0) {
		free(cmd_line_split[--cmd_line_split_len]);
	}
	free(cmd_line_split);
	free(argv);

	return buf;
}

#endif

#if 0
/**
 * showfiles_summary - Shows the files in a single column, with summary.
 * @matches: an array of strings containing the matches.
 * @nfiles:  the number of elements in @matches.
 * @cmdline: the whole command line.
 */
static void showfiles_summary(char **matches, int nfiles, char *cmd_line)
{
	int row;
	int column_width = 0;

	/* find the longest file name-  use that as the column width */
	for (row = 0; row < nfiles; row++) {
		int l = strlen(matches[row]);

		if (column_width < l)
			column_width = l;
	}
	column_width += 2;              /* min space for columns */
	column_width = -column_width;   /* for printf("%-Ns", ...); */

	for (row = 0; row < nfiles; row++) {
		char *match = matches[row];

		/* get rid of trailing space in match */
		char *cmd = strdup(match);
		int cmd_len = strlen(cmd);
		if (cmd_len > 0) {
			cmd[cmd_len-1] = '\0';
		}

		char *summary = get_summary(cmd, cmd_line);
		free(cmd);

		if (strlen(match) == 0) {
			match = "[Enter]";
		}

		printf("%*s%s\n", column_width, match, summary);
		free(summary);
	}
}

#endif

/**
 * input_tab - Handle a tab input by performing tab completion.
 * @lastWasTab: TRUE if the previous character was a tab; FALSE otherwise.
 *              Will be set to TRUE if FALSE, and to FALSE if TRUE
 *              and a completion is found.
 *              A NULL value clears any previous completions.
 */



static char **matches;
static int num_matches;

#if 0
static void add_match(char *matched)
{
	int nm = num_matches;
	int nm1 = nm + 1;

	matches = xrealloc(matches, nm1 * sizeof(char *));
	matches[nm] = matched;
	num_matches++;
}
#endif

int cmd_flag=0;

static void input_tab(int *lastWasTab)
{
    //int sav_cursor = cursor; //wangjianru
	static int match_type;
	static char match_line[BUFSIZ];

    
	if (lastWasTab == NULL) 
	{          /* free all memory */
		if (matches) 
		{
			while (num_matches > 0)
				free(matches[--num_matches]);
			free(matches);
			matches = (char **) NULL;
			match_type = -1;
		}
		return;
	}
	if (! *lastWasTab) 
	{

		char *tmp;
		int len_found;
		char matchBuf[BUFSIZ];
		int find_type;
		int recalc_pos;
		int k;//wangjr
	
		*lastWasTab = TRUE;             /* flop trigger */

		/* cursor is the cursor position */
		/* command_ps is the entire command string typed so far */
		/* match_line gets set to the command with everything
		   past the cursor lopped off. */
		tmp = strncpy(match_line, command_ps, cursor);
		tmp[cursor] = '\0';
		cmd_arg = 0;
		for(k=0; k<strlen(command_ps); k++)
		{
		  if((command_ps[k] !=32 )&&(command_ps[k+1] == 32)) // is ' '
		      cmd_arg++;
		}

		/* Make a local copy of the string -- up
		 * to the position of the cursor */
		/* matchBuf gets set to the command with everything
		   past the cursor lopped off. */
		tmp = strncpy(matchBuf, command_ps, cursor);
		tmp[cursor] = '\0';

		/* matchBuf is set to the token to match.
		   recalc_pos is set to the length of the token
		   in the original line. */
		find_type = find_match(matchBuf, &recalc_pos);

		/* Free up any memory already allocated */
		input_tab(0);
        
		/* Try to match any executable in our path and everything
		 * in the current working directory that matches.  */
		if (!matches)
		{
		   
		    /* matches = exe_tab_completion(matchBuf,
							     &num_matches);*/
            matches = exe_tab_completion(command_ps,
							     &num_matches);
		    match_type = find_type;
		}
		/* Remove duplicate found */
		if (matches) 
		{
			int i, j;
			/* bubble */
			printf("\n"); //wangjianru
			//for (i=0; i<(num_matches-1); i++) 
			for (i=0; i<num_matches; i++) 	
			{
			     cmd_flag=0;
				for (j=i+1; j<num_matches; j++)
				{
				    if (strcmp(matches[i], matches[j])==0)
				    {				    
                       cmd_flag=1;
				    }

				}
				if(cmd_flag==0)
				{ 

				   if(strlen(matches[i])!=0)
				   	{					
					    printf("%-10s\n",matches[i]);
				   	}
				}
			}
			int sav_cursor = cursor;        /* change goto_new_line() */
            *lastWasTab = FALSE;
			/* Go to the next line */
			goto_new_line();
			//showfiles_summary(matches, num_matches, match_line);
			redraw(0, len - sav_cursor);
		}

		/* Did we find exactly one match? */
		if (!matches || num_matches > 1) 
		{
			char *tmp1;

			beep();
			if (!matches)
				return;         /* not found */
			/* sort */
			qsort(matches, num_matches, sizeof(char *), match_compare);

			/* find minimal match */
			tmp = xstrdup(matches[0]);
			for (tmp1 = tmp; *tmp1; tmp1++)
				for (len_found = 1; len_found < num_matches; len_found++)
					if (matches[len_found][(tmp1 - tmp)] != *tmp1) 
					{
						*tmp1 = 0;
						//printf("\n");
						goto found_minimal;
					}
		found_minimal:
			if (*tmp == 0)
			{        /* have unique */
				free(tmp);
				return;
			}
		} 
		else 
		{                        /* one match */
			tmp = matches[0];
			/* for next completion current found */
			*lastWasTab = FALSE;
		}

		/* tmp is set to the total or partial match */
		
		strcpy(str_fore,tmp);  //wangjianru


		len_found = strlen(tmp);
		
		/* have space to placed match? */
		/* len is the total length of the command line */
		if ((len_found - strlen(matchBuf) + len) < BUFSIZ) 
		{
			/* Null terminate command line before the
			   word being matched. */
			command_ps[cursor - recalc_pos] = '\0';
			/* Save the command line after the cursor. */
			strcpy(matchBuf, command_ps + cursor);
			/* Add match on the end of the command line.  */
			strcat(command_ps, tmp);
			/* Replace the command line after the cursor. */
			strcat(command_ps, matchBuf);
			/* Place the cursor at the beginning of the match. */
			input_backward(recalc_pos);
			/* Calculate where the end of the match is. */
			recalc_pos = cursor + len_found;
			/* Update the length of the whole command line.  */
			len = strlen(command_ps);
			/* Rewrite the new command line, placing the cursor
			   at the end of the match.  */
			redraw(cmdedit_y, len - recalc_pos);
		}
		if (tmp != matches[0])
			free(tmp);
		
	}
	else
	{	   
		/* Ok -- the last char was a TAB.  Since they
		 * just hit TAB again, print a list of all the
		 * available choices... */
		 #if 0
		if (matches && num_matches > 0)
		{
			int sav_cursor = cursor;        /* change goto_new_line() */
            *lastWasTab = FALSE;
			/* Go to the next line */
			goto_new_line();
			//showfiles_summary(matches, num_matches, match_line);
			redraw(0, len - sav_cursor);
		}
		#endif
	}
}


static void get_previous_history(void)
{
	if(command_ps[0] != 0 || history[cur_history] == 0) {
		free(history[cur_history]);
		history[cur_history] = xstrdup(command_ps);
	}
	cur_history--;
}

static int get_next_history(void)
{
	int ch = cur_history;

	if (ch < n_history) {
		get_previous_history(); /* save the current history line */
		return (cur_history = ch+1);
	} else {
		beep();
		return 0;
	}
}

enum {
	ESC = 27,
	DEL = 127,
};


/*
 * This function is used to grab a character buffer
 * from the input file descriptor and allows you to
 * a string with full command editing (sortof like
 * a mini readline).
 *
 * The following standard commands are not implemented:
 * ESC-b -- Move back one word
 * ESC-f -- Move forward one word
 * ESC-d -- Delete back one word
 * ESC-h -- Delete forward one word
 * CTL-t -- Transpose two characters
 *
 * Furthermore, the "vi" command editing keys are not implemented.
 *
 */

static int splash_cmdedit_read_input(char *prompt, char cmd[BUFSIZ])
{

	int break_out = 0;
	int lastWasTab = FALSE;
	unsigned char c = 0;

	/* prepare before init handlers */
	cmdedit_y = 0;  /* quasireal y, not true work if line > xt*yt */
	len = 0;
	command_ps = cmd;

	getTermSettings(0, (void *) &initial_settings);
	memcpy(&new_settings, &initial_settings, sizeof(struct termios));
	new_settings.c_lflag &= ~ICANON;        /* unbuffered input */
	/* Turn off echoing and CTRL-C, so we can trap it */
	new_settings.c_lflag &= ~(ECHO | ECHONL | ISIG);
	/* Hmm, in linux c_cc[] not parsed if set ~ICANON */
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;
	/* Turn off CTRL-C, so we can trap it */
#       ifndef _POSIX_VDISABLE
#               define _POSIX_VDISABLE '\0'
#       endif
	new_settings.c_cc[VINTR] = _POSIX_VDISABLE;
	cmd[0] = 0;

	setTermSettings(0, (void *) &new_settings);
	handlers_sets |= SET_RESET_TERM;

	/* Now initialize things */
	cmdedit_init();
	/* Print out the command prompt */
	parse_prompt(prompt);

	while (1) {

		fflush(stdout);                 /* buffered out to fast */

		if (safe_read(0, &c, 1) < 1)
			/* if we can't read input then exit */
			goto prepare_to_die;

		switch (c) {
		case '\n':
		case '\r':
			/* Enter */
			cmd_arg=0;
			goto_new_line();
			break_out = 1;
			break;
		case 1:
			/* Control-a -- Beginning of line */
			input_backward(cursor);
			break;
		case 2:
			/* Control-b -- Move back one character */
			input_backward(1);
			break;
		case 3:
			/* Control-c -- stop gathering input */
			goto_new_line();
			len = 0;
			break_out = -1; /* for control traps */
			break;
		case 4:
			/* Control-d -- Delete one character, or exit
			 * if the len=0 and no chars to delete */
			if (len == 0) {
prepare_to_die:
				printf("exit");
				goto_new_line();
				/* cmdedit_reset_term() called in atexit */
				exit(EXIT_SUCCESS);
			} else {
				input_delete();
			}
			break;
		case 5:
			/* Control-e -- End of line */
			input_end();
			break;
		case 6:
			/* Control-f -- Move forward one character */
			input_forward();
			break;
		case '\b':
		case DEL:
			/* Control-h and DEL */
			input_backspace();
			break;
		case '\t':
			input_tab(&lastWasTab);
			break;
		case 11:
			/* Control-k -- clear to end of line */
			*(cmd + cursor) = 0;
			len = cursor;
			printf("\033[J");
			break;
		case 12:
			/* Control-l -- clear screen */
			printf("\033[H");
			redraw(0, len-cursor);
			break;
		case 14:
			/* Control-n -- Get next command in history */
			if (get_next_history())
				goto rewrite_line;
			break;
		case 16:
			/* Control-p -- Get previous command from history */
			if (cur_history > 0) {
				get_previous_history();
				goto rewrite_line;
			} else {
				beep();
			}
			break;
		case 21:
			/* Control-U -- Clear line before cursor */
			if (cursor) {
				strcpy(cmd, cmd + cursor);
				redraw(cmdedit_y, len -= cursor);
			}
			break;
		case 23:
			/* Control-W -- Remove the last word */
			while (cursor > 0 && isspace(cmd[cursor-1]))
				input_backspace();
			while (cursor > 0 &&!isspace(cmd[cursor-1]))
				input_backspace();
			break;
		case ESC:{
			/* escape sequence follows */
			if (safe_read(0, &c, 1) < 1)
				goto prepare_to_die;
			/* different vt100 emulations */
			if (c == '[' || c == 'O') {
				if (safe_read(0, &c, 1) < 1)
					goto prepare_to_die;
			}
			if (c >= '1' && c <= '9') {
				unsigned char dummy;

				if (safe_read(0, &dummy, 1) < 1)
					goto prepare_to_die;
				if(dummy != '~')
					c = 0;
			}
			switch (c) {
			case '\t':                      /* Alt-Tab */           
				input_tab(&lastWasTab);
				break;
			case 'A':
				/* Up Arrow -- Get previous command from history */
				if (cur_history > 0) {
					get_previous_history();
					goto rewrite_line;
				} else {
					beep();
				}
				break;
			case 'B':
				/* Down Arrow -- Get next command in history */
				if (!get_next_history())
				break;
				/* Rewrite the line with the selected history item */
rewrite_line:
				/* change command */
				len = strlen(strcpy(cmd, history[cur_history]));
				/* redraw and go to end line */
				redraw(cmdedit_y, 0);
				break;
			case 'C':
				/* Right Arrow -- Move forward one character */
				input_forward();
				break;
			case 'D':
				/* Left Arrow -- Move back one character */
				input_backward(1);
				break;
			case '3':
				/* Delete */
				input_delete();
				break;
			case '1':
			case 'H':
				/* Home (Ctrl-A) */
				input_backward(cursor);
				break;
			case '4':
			case 'F':
				/* End (Ctrl-E) */
				input_end();
				break;
			default:
				c = 0;
				beep();
			}
			break;
		}

		default:        /* If it's regular input, do the normal thing */
#ifdef CONFIG_FEATURE_NONPRINTABLE_INVERSE_PUT
			/* Control-V -- Add non-printable symbol */
			if (c == 22) {
				if (safe_read(0, &c, 1) < 1)
					goto prepare_to_die;
				if (c == 0) {
					beep();
					break;
				}
			} else
#endif
			if (!Isprint(c))        /* Skip non-printable characters */
				break;

			if (len >= (BUFSIZ - 2))        /* Need to leave space for enter */
				break;

			len++;

			if (cursor == (len - 1)) {      /* Append if at the end of the line */
				*(cmd + cursor) = c;
				*(cmd + cursor + 1) = 0;
				cmdedit_set_out_char(0);
			} else {                        /* Insert otherwise */
				int sc = cursor;

				memmove(cmd + sc + 1, cmd + sc, len - sc);
				*(cmd + sc) = c;
				sc++;
				/* rewrite from cursor */
				input_end();
				/* to prev x pos + 1 */
				input_backward(cursor - sc);
			}

			break;
		}
		if (break_out)                  /* Enter is the command terminator, no more input. */
			break;

		if (c != '\t')
			lastWasTab = FALSE;
	}

	setTermSettings(0, (void *) &initial_settings);
	handlers_sets &= ~SET_RESET_TERM;

	/* Handle command history log */
	/* cleanup may be saved current command line */
	free(history[MAX_HISTORY]);
	history[MAX_HISTORY] = 0;
	if (len> 0) {                                      /* no put empty line */
		int i = n_history;
			/* After max history, remove the oldest command */
		if (i >= MAX_HISTORY) {
			free(history[0]);
			for(i = 0; i < (MAX_HISTORY-1); i++)
				history[i] = history[i+1];
		}
		history[i++] = xstrdup(cmd);
		cur_history = i;
		n_history = i;
	}

	if (break_out > 0) {
		cmd[len++] = '\n';          /* set '\n' */
		cmd[len] = 0;
	}
#if defined(CONFIG_FEATURE_CLEAN_UP) && defined(CONFIG_FEATURE_COMMAND_TAB_COMPLETION)
	input_tab(0);                           /* strong free */
#endif
	cmdedit_reset_term();
	return len;
}

