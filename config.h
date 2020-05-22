/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "IBM Plex Mono:size=10" };
static const char dmenufont[]       = "IBM Plex Mono:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spvifm", "-g", "144x41", "-e", "vifm", NULL };
const char *spcmd3[] = {"keepassxc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spvifm",    spcmd2},
	{"keepassxc",   spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   isterminal noswallow monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           0,         0,        -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           0,         0,        -1 },
	{ "st",       NULL,       NULL,       0,            0,           1,         0,        -1 },
	{ NULL,		  "spterm",		NULL,		SPTAG(0),		1,	0, 0,		 -1 },
	{ NULL,		  "spvifm",		NULL,		SPTAG(1),		1,	0, 0,		 -1 },
	{ NULL,		  "keepassxc",	NULL,		SPTAG(2),		1,	0, 0,		 -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "[D]",      deck },
};

/* key definitions */
#define AltMask Mod1Mask
#define SupMask Mod4Mask
#define CTRLSHIFT (ControlMask | ShiftMask)
#define ALTSHIFT (AltMask | ShiftMask)
#define CTRLALT (ControlMask | AltMask)
#define HYPEMOD (ControlMask | ShiftMask | AltMask | SupMask)
#define MEHMOD (AltMask | ShiftMask | ControlMask)
#define SUPALT (AltMask | SupMask)
#define TAGKEYS(KEY,TAG) \
	{ AltMask,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ CTRLALT,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ ALTSHIFT,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ CTRLALT|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ AltMask,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ SupMask,             		XK_r, 	   spawn,          {.v = termcmd } },
	{ AltMask,                       XK_b,      togglebar,      {0} },
	{ AltMask,                       XK_j,      focusstack,     {.i = +1 } },
	{ AltMask,                       XK_k,      focusstack,     {.i = -1 } },
	{ AltMask,                       XK_i,      incnmaster,     {.i = +1 } },
	{ AltMask,                       XK_d,      incnmaster,     {.i = -1 } },
	{ AltMask,                       XK_h,      setmfact,       {.f = -0.05} },
	{ AltMask,                       XK_l,      setmfact,       {.f = +0.05} },
	{ SUPALT,              XK_h,      incrgaps,       {.i = +1 } },
	{ SUPALT,              XK_l,      incrgaps,       {.i = -1 } },
	{ AltMask|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	{ AltMask|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	{ AltMask|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	{ AltMask|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
	{ SUPALT,              XK_0,      togglegaps,     {0} },
	{ AltMask|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ AltMask,                       XK_y,      incrihgaps,     {.i = +1 } },
	{ AltMask,                       XK_o,      incrihgaps,     {.i = -1 } },
	{ CTRLALT,           XK_y,      incrivgaps,     {.i = +1 } },
	{ CTRLALT,           XK_o,      incrivgaps,     {.i = -1 } },
	{ SUPALT,              XK_y,      incrohgaps,     {.i = +1 } },
	{ SUPALT,              XK_o,      incrohgaps,     {.i = -1 } },
	{ ALTSHIFT,             XK_y,      incrovgaps,     {.i = +1 } },
	{ ALTSHIFT,             XK_o,      incrovgaps,     {.i = -1 } },
	{ AltMask,                       XK_Return, zoom,           {0} },
	{ AltMask,                       XK_Tab,    view,           {0} },
	{ ALTSHIFT,             XK_c,      killclient,     {0} },
	{ AltMask,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ AltMask,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ AltMask,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ AltMask,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ AltMask,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ AltMask,                       XK_U,      setlayout,      {.v = &layouts[5]} },
	{ AltMask,                       XK_O,      setlayout,      {.v = &layouts[6]} },
	{ AltMask,                       XK_c,      setlayout,      {.v = &layouts[7]} },
	{ AltMask,                       XK_space,  setlayout,      {0} },
	{ ALTSHIFT,             XK_space,  togglefloating, {0} },
	{ AltMask,			XK_F,		togglefullscreen,	{0} },
	{ AltMask,                       XK_0,      view,           {.ui = ~0 } },
	{ ALTSHIFT,             XK_0,      tag,            {.ui = ~0 } },
	{ AltMask,                       XK_comma,  focusmon,       {.i = -1 } },
	{ AltMask,                       XK_period, focusmon,       {.i = +1 } },
	{ ALTSHIFT,             XK_comma,  tagmon,         {.i = -1 } },
	{ ALTSHIFT,             XK_period, tagmon,         {.i = +1 } },
	{ AltMask,            			XK_y,  	   togglescratch,  {.ui = 0 } },
	{ AltMask,            			XK_u,	   togglescratch,  {.ui = 1 } },
	{ AltMask,            			XK_x,	   togglescratch,  {.ui = 2 } },
	{ AltMask,                       XK_F5,     xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MEHMOD,             XK_q,      quit,           {0} },
	{ HYPEMOD,             XK_q,      quit,           {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         AltMask,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         AltMask,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         AltMask,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            AltMask,         Button1,        tag,            {0} },
	{ ClkTagBar,            AltMask,         Button3,        toggletag,      {0} },
};

