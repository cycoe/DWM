/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=12" };
static const char dmenufont[]       = "monospace:size=12";
static const char col_sel_text[]    = "#4e3265";
static const char col_sel_back[]    = "#339933";
static const char col_sel_bord[]    = "#339933";
static const char col_nor_text[]    = "#eeeeee";
static const char col_nor_back[]    = "#4e3265";
static const char col_nor_bord[]    = "#4e3265";
static const char *colors[][3]      = {
	/*               fg            bg            border   */
	[SchemeNorm] = { col_nor_text, col_nor_back, col_nor_bord },
	[SchemeSel]  = { col_sel_text, col_sel_back, col_sel_bord },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance      title       tags mask     isfloating   monitor */
	{ "Gimp",        NULL,          NULL,       0,            1,           -1 },
	{ "mpv",         NULL,          NULL,       0,            1,           -1 },
	{ "Variety",     "variety",     NULL,       0,            1,           -1 },
	{ "Wine",        "wechat.exe",  NULL,       0,            1,           -1 },
	{ "Youdao Dict", "youdao-dict", NULL,       0,            1,           -1 },
	{ "flameshot",   "flameshot",   NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int movespeed   = 50;   /* speed of moving a floating client */
static const int movemargin  = 10;   /* margin between client edge to window edge */
static const int resizespeed = 50;   /* speed for client resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2]        = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]  = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_nor_back, "-nf", col_nor_text, "-sb", col_sel_back, "-sf", col_sel_text, NULL };
static const char *rofimenu[]  = { "rofi", "-show", "drun", "-m", dmenumon, "-show-icons", "-config", "~/.config/rofi/rofi.config", NULL };
static const char *exitmenu[]  = { "/bin/sh", "-c", "~/.config/dwm/exit.sh", NULL };
static const char *termcmd[]   = { "st", NULL };
static const char *ranger[]    = { "st", "-e", "ranger", NULL };
static const char *ncmpcpp[]   = { "st", "-e", "ncmpcpp", NULL };
static const char *flameshot[] = { "flameshot", "gui", NULL };
static const char *variety[]   = { "variety", "--selector", NULL };
static const char *upvol[]     = { "/bin/sh", "-c", "/usr/bin/pactl set-sink-volume 0 +5% && ~/.config/dwm/refbar.sh",  NULL };
static const char *downvol[]   = { "/bin/sh", "-c", "/usr/bin/pactl set-sink-volume 0 -5% && ~/.config/dwm/refbar.sh",  NULL };
static const char *mutevol[]   = { "/bin/sh", "-c", "/usr/bin/pactl set-sink-mute 0 toggle && ~/.config/dwm/refbar.sh", NULL };
static const char *uplight[] = { "/bin/sh", "-c", "xbacklight -inc 10 && ~/.config/dwm/refbar.sh", NULL };
static const char *downlight[] = { "/bin/sh", "-c", "xbacklight -dec 10 && ~/.config/dwm/refbar.sh", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = rofimenu } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_r,      spawn,          {.v = ranger } },
	{ MODKEY,                       XK_n,      spawn,          {.v = ncmpcpp } },
	{ MODKEY,                       XK_p,      spawn,          {.v = flameshot } },
	{ MODKEY,                       XK_g,      spawn,          {.v = variety } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      movex,          {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,      movex,          {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,      movey,          {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movey,          {.i = -1 } },
	{ MODKEY|ControlMask,           XK_h,      resizew,        {.i = -1 } },
	{ MODKEY|ControlMask,           XK_l,      resizew,        {.i = +1 } },
	{ MODKEY|ControlMask,           XK_j,      resizeh,        {.i = +1 } },
	{ MODKEY|ControlMask,           XK_k,      resizeh,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Tab,    zoom,           {0} },
	{ MODKEY,                       XK_Escape, view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_w,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY,                       XK_f,      togglefullscreen, {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_q,      spawn,          {.v = exitmenu} },
	{ MODKEY,                       XK_apostrophe, spawn,      {.v = uplight } },
	{ MODKEY,                       XK_semicolon, spawn,       {.v = downlight } },
	{ MODKEY,                       XK_bracketright, spawn,    {.v = upvol } },
	{ MODKEY,                       XK_bracketleft, spawn,     {.v = downvol } },
	{ 0,                            XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
	{ 0,                            XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                            XF86XK_AudioMute, spawn,   {.v = mutevol } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

