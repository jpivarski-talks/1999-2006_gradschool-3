/* Jon Urheim - Added AIX option
/*
 *
 * $Id: msubs.c,v 1.2 2002/09/05 19:40:01 dpp Exp $
 *
 * $Log: msubs.c,v $
 * Revision 1.2  2002/09/05 19:40:01  dpp
 *      -> removed unused section because it creates unresolved reference
 *         when trying to create a Java shared objec
 *
 * Revision 1.1  1999/05/28 03:11:12  lyon
 * Needed XTS routines from cleoks
 *
 * Revision 1.2  1998/05/04 16:03:53  clib
 * Add conditional for rs6000. (urheim)
 *
 * Revision 1.1  1996/09/11 09:27:55  clib
 * Add msubs to cleoks.
 *
 *
 */
/*	msubs.c  Attempt to use Motif toolkit */

#define MSUBS
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "unix_vms.h"

#ifndef MIN
#define MIN(x,y)        ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x,y)        ((x) > (y) ? (x) : (y))
#endif

#ifdef VMS
#include <decw$include/StringDefs.h>
#include <decw$include/Xatom.h>
#include <decw$include/Xm.h>
#include <decw$include/Translate.h>
#include <decw$include/cursorfont.h>
#else
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/DrawingA.h>
#include <Xm/RowColumn.h>
#include <Xm/SelectioB.h>
#include <Xm/Label.h>
#include <Xm/Text.h>
#include <Xm/CascadeBG.h>
/*
   #include <X11/Translate.h>
   #include <X11/cursorfont.h>
*/
#endif

static  int no_x=0;             /* is 1 if only postscript output desired */
static	Arg arglist[10];
static  int xtargc=0;
static  char **xtargv=0;
static	Boolean first = True;
static	Widget toplvl=NULL, main_window=NULL, work_window=NULL, menu_bar=NULL,
	dialog_box, db_label, db_txt, db_defbtn, db_canbtn;
static	Widget children[5];
static	XtAppContext	appcntxt; /* Application context of this application */
static	Boolean	user_event;	/* Last event of user interest */
static	Widget	user_widget;	/* Widget number of last event of interest */
static	int	user_typ;	/* user event type ( actually button #) */
static  union {
           float user_x;
           char  chrs[4];
        } u_x;
static  union {
           float user_y;
           KeySym ksym;
        } u_y;

static	Display	*pdisp = NULL; /* Pointer to display struct. */
static	Screen	*pscrn = NULL; /* Pointer to screen struct. */
static	Drawable drwid = NULL; /* Either Window or Pixmap */
static	Window	wxid = NULL;   /* Window id */
static  Window wdg_wid = NULL; /* For use in Selections */
static  Atom wdgatom, widatom; /* For use in Selections */
static  int own_sel = False;   /* For use in Selections */
static  XtTranslations wrkw_trnsl1, wrkw_trnsl2;
static  int dragon=0;  /* If 1, enables pointer dragging */
static	Pixmap	pixid = NULL;  /* Backing pixmap id */
static	GC	gc = NULL;     /* Pointer to graphics context struct.*/
static	Cursor	crsrid = NULL; /* Cursor id of cursor used in main window */
static	Colormap	color_map;     /* Default screen colormap */
static	Visual		*visual; /* Pointer to visual data struct. */
static	XGCValues	xgcvl;	 /* Structure of GC values for main wind. */
static	XFontStruct	*font_data; /* pointer to font data struct */
static  int             is_def_font=0; /* is loaded font the default font ? */
                                       /* !! if so, do not XFreeFont it !! */

static	Dimension  scrnw, scrnh;    /* Screen width, height */
static	int scrnd;		/* Depth of screen in planes */
/* static	Position   wx = 10; x-coord of window origin w.r.t. screen */
/* static	Position   wy = 30; y-coord of window origin w.r.t. screen */
 static	float xpxmm, ypxmm;	/* Hor. & vert. res. (pixels/mm) of screen */

static	float maxsize;	/* size of window in mm. */
static	Dimension wpixw, wpixh;	/* Window width & height (pixels) */
static	float wusrw, wusrh,	/* User width & height of windows */
	wscalx, wscaly,	/* Width & height user scale factors */
			/*                (pixels/user-unit) */
	xcrnr, ycrnr;	/* Coords. of lower left corner of window w.r.t. */
			/*                    user origin, in user units */

static struct ps_pix_struct {
   Pixel	pixel;
   float	gryval;
} ps_pixels[100];
static int num_pixels = 2;

void setup_xps(void);

/*
void txt_action_proc(Widget wdgt, XEvent *event, String *params,
		      Cardinal *npar)
{
   static int insp;
   static Arg arglst[] = {
      {XmNinsertionPosition, (XtArgVal) &insp}
   };

   if (!strcmp(*params, "^U")) {
      XtGetValues(wdgt, arglst, 1);
      if (insp > 0)
	 XmSTextReplace(wdgt, 0, insp, "");
   }
   return;
}
*/

static int bmx1, bmy1, bmx, bmy, bmw, bmh, old_ls, bp=0;

void wrkw_action_proc(Widget wdgt, XEvent *event, String *params,
                      Cardinal *num_params)
{
   int nchar;

   if (wdgt==work_window) {
      user_widget = NULL;
      user_event = True;
      if (event->type==ButtonPress) {
	 if (!bp) {
	    u_x.user_x = event->xbutton.x / wscalx + xcrnr;
	    u_y.user_y = ycrnr + wusrh - event->xbutton.y / wscaly;
	    bmx = bmx1 = event->xbutton.x;
	    bmy = bmy1 = event->xbutton.y;
	    bp = event->xbutton.button;
	    user_typ = bp;
	    if (dragon) {
	       XSetFunction(pdisp, gc, GXxor);
	       old_ls = xgcvl.line_style;
	       xgcvl.line_style = LineDoubleDash;
	       XChangeGC(pdisp, gc, GCLineStyle, &xgcvl);
	       bmw = 0;
	       bmh = 0;
	       XDrawRectangle(pdisp, wxid, gc, bmx, bmy, bmw, bmh);
	    }
	 }
	 else
	    user_event = False;
      }
      else if (event->type == ButtonRelease) {
	 if (event->xbutton.button == bp) {
	    if (dragon) {
	       XDrawRectangle(pdisp, wxid, gc, bmx, bmy, bmw, bmh);
	       XSetFunction(pdisp, gc, xgcvl.function);
	       xgcvl.line_style = old_ls;
	       XChangeGC(pdisp, gc, GCLineStyle, &xgcvl);
	    }
	    u_x.user_x = event->xbutton.x / wscalx + xcrnr;
	    u_y.user_y = ycrnr + wusrh - event->xbutton.y / wscaly;
	    user_typ = -bp;
	    bp = 0;
	 }
	 else
	    user_event = False;
      }
      else if (event->type==KeyPress) {
	 nchar = XLookupString(event, u_x.chrs, 4,
			       &u_y.ksym, (XComposeStatus *) NULL);
	 if (nchar == 1)
	    user_typ = 6;
	 else
	    user_typ = 7;
      }
      else if ((event->xany.type == Expose)&&(event->xexpose.window == wxid)) {
	 if (xgcvl.function != GXcopy)
	    XSetFunction(pdisp, gc, GXcopy);
	 XCopyArea(pdisp, pixid, wxid, gc,
		   event->xexpose.x, event->xexpose.y,
		   event->xexpose.width, event->xexpose.height,
		   event->xexpose.x, event->xexpose.y);
	 if (xgcvl.function != GXcopy)
	    XSetFunction(pdisp, gc, xgcvl.function);
	 if (event->xexpose.count == 0) {
/*
	    u_x.user_x = event->xexpose.x / wscalx + xcrnr;
	    u_y.user_y = ycrnr + wusrh - event->xexpose.y / wscaly;
*/
	    user_typ = 0;
	 }
	 else
	    user_event = False;
      }
      else
       user_event = False;
   }
   return;
}

void wrkw_bm_action_proc(Widget wdgt, XEvent *event, String *params,
                      Cardinal *num_params)
{
   int x2, y2;

   XDrawRectangle(pdisp, wxid, gc, bmx, bmy, bmw, bmh);
   x2 = event->xmotion.x;
   y2 = event->xmotion.y;

   if (x2 < bmx1) {
      bmx = x2;
      bmw = bmx1-x2;
   }
   else {
      bmx = bmx1;
      bmw = x2-bmx;
   }

   if (y2 < bmy1) {
      bmy = y2;
      bmh = bmy1-y2;
   }
   else {
      bmy = bmy1;
      bmh = y2-bmy;
   }

   XDrawRectangle(pdisp, wxid, gc, bmx, bmy, bmw, bmh);
   return;
}

XtCallbackProc db_callback(Widget wid, XtPointer client_data,
			   XmSelectionBoxCallbackStruct *call_data)
{
   user_widget = wid;
   user_typ = call_data->reason;
   user_event = True;
   return;
}

/*
cdj: this isn't called by anyone and is causing problems
when building a dynamically loaded object
void set_args(unsigned int argc, char **argv)
{
   xtargc = argc;
   xtargv = argv;
   return;
}
#if defined(VMS) || defined(_AIX)
void set_args_c_(void)
{
   return;
}
#else
#ifdef _HPUX_SOURCE
void set_args_c_(void)
{
   extern unsigned int __argc_value;
   extern char **__argv_value;

   xtargc = __argc_value;
   xtargv = __argv_value;
   return;
}
#else
void set_args_c_(void)
{
   extern unsigned int __Argc;
   extern char **__Argv;

   xtargc = __Argc;
   xtargv = __Argv;
   return;
}
#endif
#endif
*/
Boolean cvrt_proc(Widget cur_w, Atom *selatmp, Atom *targatmp,
   Atom *rettypp, XtPointer *retvalp, unsigned long *retlenp, int *retfmtp)
{
   if (cur_w == toplvl) {
      if (*selatmp == widatom) {
         if (*targatmp == XA_WINDOW) {
            if (wdg_wid) {
               *rettypp = XA_WINDOW;
               *retvalp = (char *) &wdg_wid;
               *retlenp = 1;
               *retfmtp = 8 * sizeof(Window);
               return(True);
            }
         }
      }
   }
   return(False);
}

XtLoseSelectionProc lose_proc(Widget lose_w, Atom *selatmp)
{
   if (lose_w == toplvl) {
      if (*selatmp == widatom) {
         own_sel = False;
         wdg_wid = NULL;
      }
   }
   return;
}

XtSelectionDoneProc done_proc(Widget wdg, Atom *setatm, Atom *targatm)
{
   return;
}

XtSelectionCallbackProc get_proc(Widget req_w, XtPointer client_data,
   Atom *selatmp, Atom *rep_typ, XtPointer value, unsigned long *length,
   int *fmtp)
{
   char *ptr = (char *) value;
   XPropertyEvent *eventp = (XPropertyEvent *) client_data;
   Widget wdg = NULL;

   if (ptr && (*selatmp == wdgatom) && (*rep_typ == XA_STRING) &&
       (*fmtp == 8)) {
      wdg = XtNameToWidget(toplvl, ptr);
      if (wdg) {
         if (wdg_wid = XtWindow(wdg)) {
            if (own_sel || XtOwnSelection(toplvl, widatom, eventp->time,
                                          cvrt_proc, lose_proc, done_proc)) {
               own_sel = True;
               XChangeProperty(pdisp, RootWindowOfScreen(pscrn), widatom,
                               XA_WINDOW, 8 * sizeof(Window),
                               PropModeReplace, (char *) &wdg_wid, 1);
            }
            else {
               own_sel = False;
               wdg_wid = NULL;
            }
         }
      }
      XtFree(value);
   }
   return;
}

void pltopn(char *dpynam, char *banr, float maxsiz,
	     float xsiz, float ysiz,
	     float xorg, float yorg)
{
/*   unsigned int argc = 0; */
   char *argv[10];
   XEvent event;
   static XtResource resources[] = {
      {XtNfont, XtCFont, XtRFontStruct, sizeof(XFontStruct *),
       (Cardinal) 0, XtRString, "XtDefaultFont"},
   };
   static XtCallbackRec db_callback_rec[] = {
      {(XtCallbackProc) db_callback, (XtPointer) 0},
      {(XtCallbackProc) 0, (XtPointer) 0}
   };
   static WidgetList list_children;
   static Cardinal num_children;
   static Arg arglst[] = {
      {XmNchildren, (XtArgVal) &list_children},
      {XmNnumChildren, (XtArgVal) &num_children}
   };
   int i;
   Widget txt_w=NULL, help_w, add_choice(Widget, char *);
   static String wrkw_strnsl1 = "<BtnDown>: wrkw_action_proc()\n \
                                 <BtnUp>: wrkw_action_proc()\n \
                                 <Expose>: wrkw_action_proc()\n \
                                 <Key>: wrkw_action_proc()\n";
   static String wrkw_strnsl2 = "<BtnMotion>: wrkw_bm_action_proc()\n \
                                 <BtnDown>: wrkw_action_proc()\n \
                                 <BtnUp>: wrkw_action_proc()\n \
                                 <Expose>: wrkw_action_proc()\n \
                                 <Key>: wrkw_action_proc()\n";
   static String txt_trnsl = "#override\n\
      Shift<Key>osfDelete: delete-next-character()\n\
	   <Key>osfDelete: delete-previous-character()\n\
	       Ctrl<Key>a: beginning-of-line()\n\
	       Ctrl<Key>e: end-of-line()\n\
	       Ctrl<Key>k: delete-to-end-of-line()\n\
	       Ctrl<Key>u: delete-to-start-of-line()\n\
	       Ctrl<Key>w: delete-previous-word()\n";

   static XtActionsRec xtsubs_acttbl[] = {
      {"wrkw_bm_action_proc", (XtActionProc) wrkw_bm_action_proc},
      {"wrkw_action_proc", (XtActionProc) wrkw_action_proc}
   };

/*   static char fallback_rc[] = ""; */
   float mmhgt;

   if (no_x) {
      scrnw = 14 * 300;
      scrnh = 11 * 300;
      xpxmm = ((float) scrnw)/(14.0 *25.4);
      ypxmm = ((float) scrnh)/(11.0 *25.4);
   }
   else if (first) {
      if (!xtargc || !xtargv) {
         xtargc = 0;
         xtargv = argv;
         xtargv[xtargc++] = banr;
         if (strlen(dpynam) > 0) {
            if (strcmp(dpynam, "LOCAL") && strcmp(dpynam, "local")) {
               xtargv[xtargc++] = "-display";
               xtargv[xtargc++] = dpynam;
            }
         }
         xtargv[xtargc++] = "+synchronous";
         xtargv[xtargc] = NULL;
      }
      XtSetArg(arglist[0], XtNallowShellResize, True);
      toplvl = XtAppInitialize(&appcntxt, banr,
                               NULL, 0,
                               &xtargc, xtargv,
                               NULL,
                               arglist, 1);

      XtAppAddActions(appcntxt, xtsubs_acttbl, XtNumber(xtsubs_acttbl));
   
      pdisp = XtDisplay(toplvl);
      pscrn = XtScreen(toplvl);
      visual = DefaultVisualOfScreen(pscrn);
      scrnw = WidthOfScreen(pscrn);
      scrnh = HeightOfScreen(pscrn);
      scrnd = DefaultDepthOfScreen(pscrn);
      mmhgt = (float) HeightMMOfScreen(pscrn);
      xpxmm = ((float) scrnw)/((float) (WidthMMOfScreen(pscrn)));
      ypxmm = ((float) scrnh)/mmhgt;

      first = False;
   }

   if (!no_x && !toplvl) {
      printf("No top level application shell, exiting.\n");
      exit(-1);
   }

   if (!no_x) {
      is_def_font = 0;
      i = 0;
      XtSetArg(arglist[i], XmNwidth, (Dimension) 0);	i++;
      XtSetArg(arglist[i], XmNheight, (Dimension) 0);	i++;
      main_window = XmCreateMainWindow(toplvl, XtName(toplvl),  arglist, i);
/*
      XtAugmentTranslations(main_window,
			    XtParseTranslationTable(mw_trnsl_string));
*/

      wusrw = xsiz;
      wusrh = ysiz;
      maxsize = maxsiz;
      if (maxsize > (mmhgt-15.0))
         maxsize = mmhgt - 15.0;
      wpixw = maxsize * xpxmm;
      wpixh = maxsize * ypxmm;
      wscalx = wpixw / wusrw;
      wscaly = wpixh / wusrh;
      xcrnr = xorg;
      ycrnr = yorg;

      i = 0;
      XtSetArg(arglist[i], XmNwidth, wpixw);	i++;
      XtSetArg(arglist[i], XmNheight, wpixh);	i++;
      XtSetArg(arglist[i], XmNbackground,
	       (Pixel) WhitePixelOfScreen(pscrn)); i++;
      XtSetArg(arglist[i], XmNforeground,
	       (Pixel) BlackPixelOfScreen(pscrn)); i++;
/*
      XtSetArg(arglist[i], XmNresizePolicy, XmRESIZE_NONE);	i++;
*/
      work_window = XmCreateDrawingArea(main_window, "work_window", arglist, i);
      children[0] = work_window;
      wrkw_trnsl1 = XtParseTranslationTable(wrkw_strnsl1);
      wrkw_trnsl2 = XtParseTranslationTable(wrkw_strnsl2);
      dragon = 0;
      XtOverrideTranslations(work_window, wrkw_trnsl1);

      i = 0;
/*
      XtSetArg(arglist[i], XmNspacing, (Dimension) 10);	i++;
      XtSetArg(arglist[i], XmNorientation, XmHORIZONTAL);	i++;
*/
      menu_bar = XmCreateMenuBar(main_window, "menu_bar", arglist, i);
      children[1] = menu_bar;

      i = 0;
      XtSetArg(arglist[i], XmNworkWindow, work_window);	i++;
      XtSetArg(arglist[i], XmNmenuBar, menu_bar);		i++;
      XtSetValues(main_window, arglist, i);

      XtManageChildren(children, 2);
      XtManageChild(main_window);

      i = 0;
      XtSetArg(arglist[i], XmNresizePolicy, XmRESIZE_ANY);	i++;
      XtSetArg(arglist[i], XmNdefaultPosition, True);		i++;
      XtSetArg(arglist[i], XmNokCallback, db_callback_rec);	i++;
      XtSetArg(arglist[i], XmNcancelCallback, db_callback_rec);  i++;
      XtSetArg(arglist[i], XmNdialogStyle, XmDIALOG_APPLICATION_MODAL);	i++;
      XtSetArg(arglist[i], XmNtextTranslations,
	       XtParseTranslationTable(txt_trnsl)); i++;
      dialog_box = XmCreatePromptDialog(main_window, banr, arglist, i);
/*
      XtGetValues(dialog_box, arglst, 2);
      for (i=0; i<num_children; i++) {
	 if (XtClass(list_children[i]) == xmTextWidgetClass) {
	    txt_w = list_children[i];
	    break;
	 }
      }
*/
      txt_w = XmSelectionBoxGetChild(dialog_box, XmDIALOG_TEXT);
      if (txt_w) {
	 i = 0;
	 XtSetArg(arglist[i], XmNresizeWidth, True);	i++;
/*
	 XtSetArg(arglist[i], XmNtranslations,
		  XtParseTranslationTable(txt_trnsl));	i++;
*/
	 XtSetValues(txt_w, arglist, i);
/*	 XtOverrideTranslations(txt_w, XtParseTranslationTable(txt_trnsl)); */
      }

      help_w = XmSelectionBoxGetChild(dialog_box, XmDIALOG_HELP_BUTTON);
      if (help_w)
	 XtUnmanageChild(help_w);

      XtRealizeWidget(toplvl);

      wdgatom = XInternAtom(pdisp, "XTSUBS_WIDGET", False);
      widatom = XInternAtom(pdisp, "XTSUBS_WID", False);
      XSelectInput(pdisp, RootWindowOfScreen(pscrn), PropertyChangeMask);

      XFlush(pdisp);

      XtGetApplicationResources(work_window, &font_data, resources,
                                XtNumber(resources), NULL, 0);
      is_def_font = 1;
      xgcvl.font = font_data->fid;

      wxid = XtWindow(work_window);
   }

   setup_xps();

   do {
      XtAppNextEvent(appcntxt, &event);
      XtDispatchEvent(&event);
   } while (event.type != Expose);

   return;
}
#ifdef VMS
void pltopn_c(VMSstrptr dpynam_p, VMSstrptr banr_p, float *maxsiz_p,
	    float *xsiz_p, float *ysiz_p,
	    float *xorg_p, float *yorg_p)
{
   char *dpynam, *banr;
   init_VMStoC ;
   dpynam = VMStoC(dpynam_p);
   banr = VMStoC(banr_p);

   pltopn(dpynam, banr, *maxsiz_p, *xsiz_p, *ysiz_p, *xorg_p, *yorg_p);

   return;
}
#else
void pltopn_c_(char *dpynam_p, char *banr_p, float *maxsiz_p,
	    float *xsiz_p, float *ysiz_p,
	    float *xorg_p, float *yorg_p,
	    int dpynam_l, int banr_l)
{
   char *dpynam, *banr;
   init_F77toC ;
   dpynam = F77toC(dpynam_p, dpynam_l);
   banr = F77toC(banr_p, banr_l);

   pltopn(dpynam, banr, *maxsiz_p, *xsiz_p, *ysiz_p, *xorg_p, *yorg_p);

   return;
}
#endif

Widget start_pulldown_menu(Widget parent, char *title)
{
   int i;
   static Widget par;
   Widget pdwn_menu_entry, pdwn_menu;
   static Arg arglst[] = {
      {XmNsubMenuId, (XtArgVal) &par}
   };

   if (no_x)
      return NULL;

   if (!parent)
      par = menu_bar;
   else
      XtGetValues(parent, arglst, 1);

   pdwn_menu = XmCreatePulldownMenu(par, "pdwn_menu", NULL, 0);
   i = 0;
   XtSetArg(arglist[i], XmNlabelString, XmStringCreateLtoR(title,
	    XmSTRING_DEFAULT_CHARSET));	i++;
   XtSetArg(arglist[i], XmNsubMenuId, pdwn_menu);	i++;
   pdwn_menu_entry = XmCreateCascadeButtonGadget(par, "pdwn_menu_entry",
						 arglist, i);
   XtManageChild(pdwn_menu_entry);

   return pdwn_menu_entry;
}
#ifdef VMS
Widget start_pulldown_menu_c(Widget *parent_p, VMSstrptr title_p)
{
   char *title;
   init_VMStoC ;
   title = VMStoC(title_p);

   return start_pulldown_menu(*parent_p, title);
}
#else
Widget start_pulldown_menu_c_(Widget *parent_p, char *title_p, int title_l)
{
   char *title;
   init_F77toC ;
   title = F77toC(title_p, title_l);

   return start_pulldown_menu(*parent_p, title);
}
#endif

XtCallbackProc choice_callback(Widget wgt, int *tag,
			       XmAnyCallbackStruct *cb_data)
{
   user_widget = wgt;
   user_typ = 1;
   user_event = True;
   return;
}

Widget add_choice(Widget parent, char *title)
{
   int i;
   static Widget par;
   Widget pushbtn;
   static Arg arglst[] = {
      {XmNsubMenuId, (XtArgVal) &par}
   };
   static XtCallbackRec callback_rec[] = {
      {(XtCallbackProc) choice_callback, (XtPointer) 0},
      {(XtCallbackProc) 0, (XtPointer) 0}
   };

   if (no_x)
      return NULL;

   i = 0;
   XtSetArg(arglist[0], XmNactivateCallback, callback_rec);
   XtSetArg(arglist[1], XmNlabelString, XmStringCreateLtoR(title,
	    XmSTRING_DEFAULT_CHARSET));
   if (!parent)
      par = menu_bar;
   else
      XtGetValues(parent, arglst, 1);
   pushbtn = XmCreateCascadeButtonGadget(par, title, arglist, 2);
   XtManageChild(pushbtn);
   return pushbtn;
}
#ifdef VMS
Widget add_choice_c(Widget *parent_p, VMSstrptr title_p)
{
   char *title;
   init_VMStoC ;
   title = VMStoC(title_p);

   return add_choice(*parent_p, title);
}
#else
Widget add_choice_c_(Widget *parent_p, char *title_p, int title_l)
{
   char *title;
   init_F77toC ;
   title = F77toC(title_p, title_l);

   return add_choice(*parent_p, title);
}
#endif

void hide_choice(Widget wdgt)
{
   if (no_x)
      return;

   XtUnmanageChild(wdgt);
   return;
}
#ifdef VMS
void hide_choice_c(Widget *choice_p)
{
   hide_choice(*choice_p);
   return;
}
#else
void hide_choice_c_(Widget *choice_p)
{
   hide_choice(*choice_p);
   return;
}
#endif

void show_choice(Widget wdgt)
{
   if (no_x)
      return;

   XtManageChild(wdgt);
   return;
}
#ifdef VMS
void show_choice_c(Widget *choice_p)
{
   show_choice(*choice_p);
   return;
}
#else
void show_choice_c_(Widget *choice_p)
{
   show_choice(*choice_p);
   return;
}
#endif

void unsensitize_choice(Widget wdgt)
{
   if (no_x)
      return;

   if (wdgt)
      XtSetSensitive(wdgt, False);
   else {
      XtSetSensitive(menu_bar, False);
      XFlush(pdisp);
   }
   return;
}
#ifdef VMS
void unsensitize_choice_c(Widget *choice_p)
{
   unsensitize_choice(*choice_p);
   return;
}
#else
void unsensitize_choice_c_(Widget *choice_p)
{
   unsensitize_choice(*choice_p);
   return;
}
#endif

void resensitize_choice(Widget wdgt)
{
   if (no_x)
      return;

   if (wdgt)
      XtSetSensitive(wdgt, True);
   else
      XtSetSensitive(menu_bar, True);
   return;
}
#ifdef VMS
void resensitize_choice_c(Widget *choice_p)
{
   resensitize_choice(*choice_p);
   return;
}
#else
void resensitize_choice_c_(Widget *choice_p)
{
   resensitize_choice(*choice_p);
   return;
}
#endif

void change_label(Widget wdgt, char *title)
{
   if (no_x)
      return;

   XtSetArg(arglist[0], XmNlabelString, XmStringCreateLtoR(title,
	    XmSTRING_DEFAULT_CHARSET));
   XtSetValues(wdgt, arglist, 1);
   return;
}
#ifdef VMS
void change_label_c(Widget *choice_p, VMSstrptr title_p)
{
   char *title;
   init_VMStoC ;
   title = VMStoC(title_p);

   change_label(*choice_p, title);
   return;
}
#else
void change_label_c_(Widget *choice_p, char *title_p, int title_l)
{
   char *title;
   init_F77toC ;
   title = F77toC(title_p, title_l);

   change_label(*choice_p, title);
   return;
}
#endif

Widget remove_choice(Widget wdgt)
{
   if (no_x)
      return NULL;

   if (wdgt) {
/*
      if (!strcmp((wdgt)->core.name, "pdwn_menu_entry"))
	 XtDestroyWidget(((PulldownWidget) wdgt)->pulldown.submenu);
*/
      XtDestroyWidget(wdgt);
      wdgt = NULL;
   }
   return (Widget) NULL;
}
#ifdef VMS
Widget remove_choice_c(Widget *choice_p)
{
   return remove_choice(*choice_p);
}
#else
Widget remove_choice_c_(Widget *choice_p)
{
   return remove_choice(*choice_p);
}
#endif

void next_event(Widget *evtwid, int *evttyp, float *evtx, float *evty)
{
   XEvent event;
   static Time proptime;

   if (no_x)
      return;

   user_event = False;
   user_widget = NULL;
   user_typ = 0;
   do {
      XtAppNextEvent(appcntxt, &event);
      if ((event.type == PropertyNotify) &&
          (event.xproperty.atom == wdgatom) &&
          (event.xproperty.state == PropertyNewValue)) {
         proptime = event.xproperty.time;
         XtGetSelectionValue(toplvl, wdgatom, XA_STRING, get_proc,
                             &proptime, event.xproperty.time);
      }
      XtDispatchEvent(&event);
      if (user_event) {
	 *evtwid = user_widget;
	 *evttyp = user_typ;
	 if (user_typ) {
            memcpy(evtx, &u_x.user_x, 4);
            memcpy(evty, &u_y.user_y, 4);
	 }
	 break;
      }
   } while (True);

   XFlush(pdisp);
   return;
}
#ifdef VMS
void next_event_c(Widget *evtwid, int *evttyp, float *evtx, float *evty)
{
   next_event(evtwid, evttyp, evtx, evty);
   return;
}
#else
void next_event_c_(Widget *evtwid, int *evttyp, float *evtx, float *evty)
{
   next_event(evtwid, evttyp, evtx, evty);
   return;
}
#endif

int get_usrtxt(char *prompt, char *initxt, char *usrtxt, int maxtxt)
{
   static XmString strptr;
   char *txtptr;
   int i, lenstr, evttyp;
   float evtx, evty;
   Widget evtwid;
   static Arg arglst[] = {
      {XmNtextString, (XtArgVal) &strptr}
   };

   if (no_x) {
      strncpy(usrtxt, initxt, maxtxt-1);
      return strlen(usrtxt);
   }

   if (bp) {
      XDrawRectangle(pdisp, wxid, gc, bmx, bmy, bmw, bmh);
      XSetFunction(pdisp, gc, xgcvl.function);
      xgcvl.line_style = old_ls;
      XChangeGC(pdisp, gc, GCLineStyle, &xgcvl);
      bp = 0;
   }

   lenstr = strlen(initxt);

   i = 0;
   XtSetArg(arglist[i], XmNselectionLabelString,
	    XmStringCreateLtoR(prompt, XmSTRING_DEFAULT_CHARSET));	i++;
   XtSetArg(arglist[i], XmNtextString,
	    XmStringCreateLtoR(initxt, XmSTRING_DEFAULT_CHARSET));	i++;
   XtSetValues(dialog_box, arglist, i);

   XtManageChild(dialog_box);
   do {
      next_event(&evtwid, &evttyp, &evtx, &evty);
      if (evtwid == dialog_box) {
	 if (evttyp==XmCR_CANCEL) {
	    XtUnmanageChild(dialog_box);
	    return -1;
	 }
	 else if (evttyp==XmCR_OK) {
	    XtGetValues(dialog_box, arglst, 1);
	    if (XmStringGetLtoR(strptr, XmSTRING_DEFAULT_CHARSET, &txtptr)) {
	       strncpy(usrtxt, txtptr, maxtxt-1);
	       XmStringFree(strptr);
	       XtUnmanageChild(dialog_box);
	       return strlen(usrtxt);
	    }
	    XtUnmanageChild(dialog_box);
	    printf("get_usrtext: No text segment\n");
	    return -1;
	 }
      }
   } while (True);
}
#ifdef VMS
int get_usrtxt_c(VMSstrptr prompt_p, VMSstrptr initxt_p, VMSstrptr usrtxt_p)
{
   int usrlen;
   char *prompt, *initxt, *usrtxt;
   init_VMStoC ;
   prompt = VMStoC(prompt_p);
   initxt = VMStoC(initxt_p);
   usrtxt = adr_CtoVMS(usrtxt_p);
   usrlen = len_CtoVMS(usrtxt_p);

   return get_usrtxt(prompt, initxt, usrtxt, usrlen);
}
#else
int get_usrtxt_c_(char *prompt_p, char *initxt_p, char *usrtxt_p,
		  int prompt_l, int initxt_l, int usrtxt_l)
{
   char *prompt, *initxt;
   init_F77toC ;
   prompt = F77toC(prompt_p, prompt_l);
   initxt = F77toC(initxt_p, initxt_l);

   return get_usrtxt(prompt, initxt, usrtxt_p, usrtxt_l);
}
#endif

int get_keyname(KeySym keysym, char *keyname, int maxlen)
{
   char *retname;
   int retlen;

   if (no_x)
      return NULL;

   retname = XKeysymToString(keysym);
   if (retname) {
      retlen = strlen(retname);
      strncpy(keyname, retname, maxlen-1);
      if (retlen < maxlen)
         return retlen;
      else {
         keyname[maxlen-1] = '\0';
         return maxlen-1;
      }
   }
   else {
      return 0;
   }
}
#ifdef VMS
int get_keyname_c(int *keysym, VMSstrptr keynam_p)
{
   int keylen;
   char *keynam;
   init_VMStoC ;
   keynam = adr_CtoVMS(keynam_p);
   keylen = len_CtoVMS(keynam_p);

   return get_keyname(*keysym, keynam, keylen);
}
#else
int get_keyname_c_(int *keysym, char *keynam_p, int keynam_l)
{
   return get_keyname(*keysym, keynam_p, keynam_l);
}
#endif

XtInputCallbackProc input_cb_proc(XtPointer client_data, int *src,
                                  XtInputId *iid)
{
   printf("Input ready for iid:%ul, src:%d\n", *iid, *src);
   return;
}

XtInputId add_input(int src, int condition)
{
   return XtAppAddInput(appcntxt, src, condition, input_cb_proc,
                        (XtPointer) NULL);
}
/* ************************************************************************** */
#include "xps.h"
