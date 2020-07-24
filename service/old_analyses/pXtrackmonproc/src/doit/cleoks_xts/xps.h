/*
 *
 * $Id: xps.h,v 1.1 1999/05/28 03:11:11 lyon Exp $
 *
 * $Log: xps.h,v $
 * Revision 1.1  1999/05/28 03:11:11  lyon
 * Needed XTS routines from cleoks
 *
 * Revision 1.3  1998/05/03 19:55:38  clib
 * Renamed erase() to erase_cleoks() to avoid clash with curses.  Everything
 * that calls this calls it through erase_c_(), which was changed to call
 * erase_cleoks(). (lyon)
 *
 * Revision 1.2  1997/08/18 11:08:55  clib
 * Add set_*_short functions of type short int for calls from Fortran.
 *
 * Revision 1.1  1996/09/11 09:27:56  clib
 * Add msubs to cleoks.
 *
 *
 */
/* Simplified X11 and Postscript interface */

XSetWindowAttributes	xswda;	 /* Structure for setting wind. attr. */
XEvent			event;	 /* X event data structure */

int	local_async_flag;
int	*async_flag = &local_async_flag;
/*
   will be set to button# async'ly. If set_async is called then that argument
   is set async'ly. async_flag is the pointer by which either is set.
*/

int	sptr;
float	str_w_by_h = 1.0; /* Aspect ratio of vec_string chars = width/height */
float	xscal,
	yscal;	/* Used in symbols setting & rescaling */ 

int	lps = False,	/* is True if in Hardcopy mode */
	npage = 0;	/* current page # of hardcopy output */
FILE	*pstrm = NULL;	/* output stream for postscript descriptions */
float	psclx,
	pscly;	/* Postscript units per x/y user unit */
static int vec_font = 15; /* Hershey font in use */
static int ps_vec = 1; /* Use PS fonts or real vector strings (default) */
		       /* in hardcopy mode for the vec_string call */
static float ps_curpt = 14.0; /* Default point size of PS font */
float	sizfact; /* used to rescale mm in PostScript */
/* ************************************************************************** */

GC setup_gc(XGCValues *, Window);
int rfrshw(Window);

void pstrp(char *pstr)
{
   if (strpbrk(pstr, "()\\")) {
      fprintf(pstrm, "(");
      while (*pstr) {
	 if (*pstr == '(' || *pstr == ')' || *pstr == '\\')
	    fprintf(pstrm, "\\");
	 fprintf(pstrm, "%.1s", pstr++);
      }
      fprintf(pstrm, ") ");
   }
   else {
      fprintf(pstrm, "(%s) ", pstr);
   }
   return;
}
/* ************************************************************************** */

void setup_xps(void)
{
   XWindowAttributes w_attr;

   psclx = (612.0 - 40.0)/wusrw;
   pscly = (612.0 - 40.0)/wusrh;
   sizfact = (612.0 - 40.0)*25.4 / (72.0*maxsize);
   ps_pixels[0].pixel = xgcvl.background = 1;
   ps_pixels[0].gryval = 1.0;
   ps_pixels[1].pixel = xgcvl.foreground = 0;
   ps_pixels[1].gryval = 0.0;

   if (!no_x) {
      if (!(gc = setup_gc(&xgcvl, wxid))) {
	 printf("pltopn: Could not create GC for main window.\n");
	 exit(-1);
      }
      psclx = (612.0 - 40.0)/wusrw;
      pscly = (612.0 - 40.0)/wusrh;
      sizfact = (612.0 - 40.0)*25.4 / (72.0*maxsize);
      if(!pixid) {
	 pixid = XCreatePixmap(pdisp, wxid, wpixw, wpixh, scrnd);
	 drwid = pixid;
	 XFillRectangle(pdisp, pixid, gc, 0, 0, wpixw, wpixh);
	 xgcvl.foreground = BlackPixelOfScreen(pscrn);
	 XChangeGC(pdisp, gc, GCForeground, &xgcvl);
	 ps_pixels[0].pixel = xgcvl.background;
	 ps_pixels[0].gryval = 1.0;
	 ps_pixels[1].pixel = xgcvl.foreground;
	 ps_pixels[1].gryval = 0.0;
#ifdef DECWSUBS
	 if (XGetWindowAttributes(pdisp, wxid, &w_attr)) {
	    XSelectInput(pdisp, wxid, 
			 w_attr.your_event_mask | ButtonPressMask);
	    XSelectAsyncInput(pdisp, wxid, ButtonPressMask, rfrshw, wxid);
	 }
#endif
      }
   }
   return;
}
/* ************************************************************************** */

int set_backing(int backing)
{
   if (backing) {
      drwid = pixid;
      return 1;
   }
   else {
      drwid = wxid;
      return 0;
   }
}
#ifdef VMS
int set_backing_c(int *backing_p)
{
   return set_backing(*backing_p);
}
#else
int set_backing_c_(int *backing_p)
{
   return set_backing(*backing_p);
}
#endif
/* ************************************************************************** */

void ring_bell(int vol)
{
   if (!lps && !no_x) {
      XBell(pdisp, vol);
      return;
   }
}
#ifdef VMS
void ring_bell_c(int *vol_p)
{
   ring_bell(*vol_p);
   return;
}
#else
void ring_bell_c_(int *vol_p)
{
   ring_bell(*vol_p);
   return;
}
#endif
/* ************************************************************************** */

void rescale(float xsiz, float ysiz, float xorg, float yorg)
{
   float symsiz;

   if (xsiz > 0.0 && ysiz > 0.0) {
      symsiz = xscal * wscalx * 100.0 / xpxmm;
      wusrw = xsiz;
      wusrh = ysiz;
      wscalx = wpixw/wusrw;
      wscaly = wpixh/wusrh;
      xcrnr = xorg;
      ycrnr = yorg;
      psclx = (612.0-40.0)/wusrw;
      pscly = (612.0-40.0)/wusrh;
      if (sptr) {
	 xscal = symsiz * xpxmm / (100.0 * wscalx);
	 yscal = symsiz * ypxmm / (100.0 * wscaly);
      }
   }
   return;
}
#ifdef VMS
void rescale_c(float *xsiz_p, float *ysiz_p, float *xorg_p, float *yorg_p)
{
   rescale(*xsiz_p, *ysiz_p, *xorg_p, *yorg_p);
   return;
}
#else
void rescale_c_(float *xsiz_p, float *ysiz_p, float *xorg_p, float *yorg_p)
{
   rescale(*xsiz_p, *ysiz_p, *xorg_p, *yorg_p);
   return;
}
#endif
/* ************************************************************************** */

float aspect(void)
{
   if (wpixh > 0.0)
      return (wpixw * ypxmm) / (wpixh * xpxmm);
   else
      return 0.0;
}
#ifdef VMS
float aspect_c(void)
{
   return aspect();
}
#else
float aspect_c_(void)
{
   return aspect();
}
#endif
/* ************************************************************************** */

GC setup_gc(XGCValues *xgcv, Window wid)
{
   Mask font_mask;
   GC gc;

   xgcv->function = GXcopy;
   xgcv->plane_mask = ~0;
   xgcv->background = xgcv->foreground = WhitePixelOfScreen(pscrn);
   xgcv->line_width = 0;
   xgcv->line_style = LineSolid;
   xgcv->cap_style = CapButt;
   xgcv->join_style = JoinMiter;
   xgcv->fill_style = FillSolid;
   xgcv->fill_rule = EvenOddRule;
   xgcv->arc_mode = ArcPieSlice;
   xgcv->ts_x_origin = 0;
   xgcv->ts_y_origin = 0;
   if (!xgcv->font) 
      font_mask = 0;
   else
      font_mask = GCFont;
   xgcv->subwindow_mode = ClipByChildren;
   xgcv->graphics_exposures = True;
   xgcv->clip_x_origin = 0;
   xgcv->clip_y_origin = 0;
   xgcv->clip_mask = None;
   xgcv->dash_offset = 0;
   xgcv->dashes = 4;

   gc =   XCreateGC(pdisp, wid, GCFunction	|
				GCPlaneMask	|
				GCForeground	|
				GCBackground	|
				GCLineWidth	|
				GCLineStyle	|
				GCCapStyle	|
				GCJoinStyle	|
				GCFillStyle	|
				GCFillRule	|
				GCArcMode	|
				GCTileStipXOrigin	|
				GCTileStipYOrigin	|
				font_mask	|
				GCSubwindowMode	|
				GCGraphicsExposures	|
				GCClipXOrigin	|
				GCClipYOrigin	|
				GCClipMask	|
				GCDashOffset	|
				GCDashList,          xgcv);

   if (!xgcv->font) {
      printf("setup_gc: Trying to find default font...\n");
      font_data = XQueryFont(pdisp, XGContextFromGC(gc));
      if (!font_data)
	  printf("setup_gc: Could not get font_data with XQueryFont.\n");
      else {
	 xgcv->font = font_data->fid;
	 is_def_font = 1;
      }
   }

   return gc;
}
/* ************************************************************************** */

int set_drag(int drag)
{
   int old_drag;;

   old_drag = xgcvl.function;
   if (!lps && !no_x) {
      if (old_drag != drag) {
	 dragon = drag;
#ifdef MSUBS
	 if (dragon)
	    XtSetArg(arglist[0], XmNtranslations, wrkw_trnsl2);
	 else
	    XtSetArg(arglist[0], XmNtranslations, wrkw_trnsl1);
#else
	 if (dragon)
	    XtSetArg(arglist[0], XtNtranslations, wrkw_trnsl2);
	 else
	    XtSetArg(arglist[0], XtNtranslations, wrkw_trnsl1);
#endif
	 XtSetValues(work_window, arglist, 1);
      }
   }
   return old_drag;
}
#ifdef VMS
int set_drag_c(int *drag)
{
   return set_drag(*drag);
}
#else
int set_drag_c_(int *drag)
{
   return set_drag(*drag);
}
#endif
/* ************************************************************************** */

void set_async(int *int_ptr)
{
   async_flag = int_ptr;
   return;
}
#ifdef VMS
void set_async_c(int *int_ptr)
{
   set_async(int_ptr);
   return;
}
#else
void set_async_c_(int *int_ptr)
{
   set_async(int_ptr);
   return;
}
#endif
/* ************************************************************************** */

int rfrshw(Window wid)
{
/*
   Position x2, y2;
   static Position exp_x1, exp_y1, exp_x2, exp_y2;
   if (wxid) {
      if (XCheckTypedWindowEvent(pdisp, wid, Expose, &event)) {
	 if (xgcvl.function != GXcopy)
	    XSetFunction(pdisp, gc, GXcopy);
	    XCopyArea(pdisp, pixid, wid, gc,
		      event.xexpose.x, event.xexpose.y,
		      event.xexpose.width, event.xexpose.height,
		      event.xexpose.x, event.xexpose.y);
	 if (xgcvl.function != GXcopy)
	    XSetFunction(pdisp, gc, xgcvl.function);
	 if (event.xexpose.x < exp_x1)
	    exp_x1 = event.xexpose.x;
	 if (event.xexpose.y < exp_y1)
	    exp_y1 = event.xexpose.y;
	 if ((x2 = event.xexpose.x + event.xexpose.width) > exp_x2)
	    exp_x2 = x2;
	 if ((y2 = event.xexpose.y + event.xexpose.height) > exp_y2)
	    exp_y2 = y2;
	 if (event.xexpose.count == 0) {
	    XPutBackEvent(pdisp, &event);
	 }
      }
*/
   if (wxid) {
/*
      if (XCheckTypedWindowEvent(pdisp, wid, Expose, &event)) {
	    XPutBackEvent(pdisp, &event);
      }
      if (XCheckTypedWindowEvent(pdisp, wid, ButtonPress, &event)) {
	 *async_flag = event.xbutton.button;
	 XPutBackEvent(pdisp, &event);
      }
*/
      *async_flag = 1;
   }
   return 0;
}
/* ************************************************************************** */

void wndcls(void)
{
   if (main_window) {
      if (pixid) {
	 XFreePixmap(pdisp, pixid);
	 pixid = NULL;
      }
      if (gc) {
	 XFreeGC(pdisp, gc);
	 gc = NULL;
      }
      XtDestroyWidget(main_window);
      XtUnrealizeWidget(toplvl);
      main_window = NULL;
      wxid = NULL;
      drwid = NULL;
      XFlush(pdisp);
   }
   return;
}
#ifdef VMS
void wndcls_c(void)
{
   wndcls();
   return;
}
#else
void wndcls_c_(void)
{
   wndcls();
   return;
}
#endif
/* ************************************************************************** */

void pltcls(void)
{
   if (main_window)
      wndcls();
   if (crsrid) {
      XFreeCursor(pdisp, crsrid);
      crsrid = NULL;
   }
   if (toplvl) {
      XtDestroyApplicationContext(appcntxt);
      pdisp = NULL;
      toplvl = NULL;
   }
   return;
}
#ifdef VMS
void pltcls_c(void)
{
   pltcls();
   return;
}
#else
void pltcls_c_(void)
{
   pltcls();
   return;
}
#endif
/* ************************************************************************** */

void load_font(char *font_name)
{
static XFontStruct *next_font_data;

   if (!lps && !no_x) {
      if (next_font_data = XLoadQueryFont(pdisp, font_name)) {
	 if (!is_def_font && font_data)
	    XFreeFont(pdisp, font_data);
	 is_def_font = 0;
	 font_data = next_font_data;
	 xgcvl.font = font_data->fid;
	 XChangeGC(pdisp, gc, GCFont, &xgcvl);
      }
      else {
	 printf("pltopn: Could not load font - \"%s\", font not changed.\n",
		font_name);
      }
   }
   else {
      fprintf(pstrm, "/%s loadfnt\n", font_name);
   }
   return;
}
#ifdef VMS
void load_font_c(VMSstrptr font_name_p)
{
   char *font_name;
   init_VMStoC ;
   font_name = VMStoC(font_name_p);

   load_font(font_name);
}
#else
void load_font_c_(char *font_name_p, int font_name_l)
{
   char *font_name;
   init_F77toC ;
   font_name = F77toC(font_name_p, font_name_l);

   load_font(font_name);
}
#endif
/* ************************************************************************** */

int set_function(int gfunc)
{
   int old_func;;

   old_func = xgcvl.function;
   if (!lps && !no_x) {
      if (old_func != gfunc) {
	 xgcvl.function = gfunc;
	 XChangeGC(pdisp, gc, GCFunction, &xgcvl);
      }
   }
   return old_func;
}
#ifdef VMS
int set_function_c(int *gfunc)
{
   return set_function(*gfunc);
}
#else
int set_function_c_(int *gfunc)
{
   return set_function(*gfunc);
}
#endif
/* ************************************************************************** */

float get_gray(Pixel pxl)
{
   int i;

   for (i = 0; i < num_pixels; i++) {
      if (pxl == ps_pixels[i].pixel) {
	 return ps_pixels[i].gryval;
      }
   }
   return 0.0;
}
/* ************************************************************************** */

Pixel set_foreground(Pixel pixel)
{
   Pixel pxl;

   pxl = xgcvl.foreground;
   if (pxl != pixel) {
      xgcvl.foreground = pixel;
      if (!lps && !no_x) {
	 XChangeGC(pdisp, gc, GCForeground, &xgcvl);
      }
      else {
	 fprintf(pstrm, "%.3f setforeground\n", get_gray(pixel));
      }
   }

   return pxl;
}
#ifdef VMS
unsigned long set_foreground_c(unsigned long *pixel)
{
   return set_foreground(*pixel);
}
unsigned set_foreground_c_short(unsigned *pixel)
{
   unsigned long mypixel = *pixel;
   return set_foreground(mypixel);
}
#else
unsigned long set_foreground_c_(unsigned long *pixel)
{
   return set_foreground(*pixel);
}
unsigned set_foreground_c_short_(unsigned *pixel)
{
   unsigned long mypixel = *pixel;
   return set_foreground(mypixel);
}
#endif
/* ************************************************************************** */

unsigned long set_background(unsigned long pixel)
{
   unsigned long pxl;

   pxl = xgcvl.background;
   if (pxl != pixel) {
      xgcvl.background = pixel;
      if (!lps && !no_x)
	 XChangeGC(pdisp, gc, GCBackground, &xgcvl);
      else
	 fprintf(pstrm, "%.3f setbackground\n", get_gray(pixel));
   }
   return pxl;
}
#ifdef VMS
unsigned long set_background_c(unsigned long *pixel)
{
   return set_background(*pixel);
}
unsigned set_background_c_short(unsigned *pixel)
{
   unsigned long mypixel = *pixel;
   return set_background(mypixel);
}
#else
unsigned long set_background_c_(unsigned long *pixel)
{
   return set_background(*pixel);
}
unsigned set_background_c_short_(unsigned *pixel)
{
   unsigned long mypixel = *pixel;
   return set_background(mypixel);
}
#endif
/* ************************************************************************** */

void switch_fb(void)
{
   unsigned long fpxl;

   fpxl = xgcvl.foreground;
   xgcvl.foreground = xgcvl.background;
   xgcvl.background = fpxl;
   if (!lps && !no_x)
      XChangeGC(pdisp, gc, GCForeground | GCBackground, &xgcvl);
   else {
      fprintf(pstrm, "%.3f setforeground\n", get_gray(xgcvl.foreground));
      fprintf(pstrm, "%.3f setbackground\n", get_gray(xgcvl.background));
   }

   return;
}
#ifdef VMS
void switch_fb_c(void)
{
   switch_fb();
   return;
}
#else
void switch_fb_c_(void)
{
   switch_fb();
   return;
}
#endif
/* ************************************************************************** */

void update(void)
{
   static int old_func, new_func;

   if (!lps && !no_x) {
      if (drwid == pixid) {
	 old_func = set_function(GXcopy);
	 XCopyArea(pdisp, pixid, wxid, gc, 0, 0, wpixw, wpixh, 0, 0);
	 new_func = set_function(old_func);
      }
      XFlush(pdisp);
   }
   return;
}
#ifdef VMS
void update_c(void)
{
   update();
   return;
}
#else
void update_c_(void)
{
   update();
   return;
}
#endif
/* ************************************************************************** */

void erase_cleoks(void)
{
   static unsigned long old_foreground, new_foreground;

   if (!lps && !no_x) {
      old_foreground = set_foreground(xgcvl.background);
      XFillRectangle(pdisp, drwid, gc, 0, 0, wpixw, wpixh);
      new_foreground = set_foreground(old_foreground);
/*
      XCopyArea(pdisp, pixid, wxid, gc, 0, 0, wpixw, wpixh, 0, 0);
*/
      XFlush(pdisp);
   }
   return;
}
#ifdef VMS
void erase_c(void)
{
   erase_cleoks();
   return;
}
#else
void erase_c_(void)
{
   erase_cleoks();
   return;
}
#endif
/* ************************************************************************** */

void define_ps_pixel(Pixel pxl)
{
   int i;
   float gry_inc, nxt_gry;

   for (i = 0; i < num_pixels; i++) {
      if (pxl == ps_pixels[i].pixel)
	 return;
   }

   gry_inc = 1.0 / num_pixels;
   ps_pixels[num_pixels++].pixel = pxl;
   nxt_gry = gry_inc;

   for (i = 2; i < num_pixels; i++) {
      ps_pixels[i].gryval = nxt_gry;
      nxt_gry += gry_inc;
   }
}
/* ************************************************************************** */

Pixel define_color(char *color_name)
{
   XColor	scrdef_ret, xctdef_ret;
   Pixel	new_pixel;

   new_pixel = xgcvl.foreground;

   if (no_x) {
      define_ps_pixel(new_pixel);
      return new_pixel;
   }
   if ((visual->class == PseudoColor) |
       (visual->class == DirectColor) |
       (visual->class == GrayScale)   ) {
      if (!color_map)
	 color_map = DefaultColormapOfScreen(pscrn);
      if (XAllocNamedColor(pdisp, color_map, color_name,
			   &scrdef_ret, &xctdef_ret)) {
	 new_pixel = scrdef_ret.pixel;
      }
      else {
	 printf("define_color: Could not allocate named color \"%s\"\n",
		color_name);
      }
   }
   
   define_ps_pixel(new_pixel);

   return new_pixel;
}
#ifdef VMS
unsigned long define_color_c(VMSstrptr color_name_p)
{
   char *color_name;
   init_VMStoC ;
   color_name = VMStoC(color_name_p);

   return define_color(color_name);
}
#else
unsigned long define_color_c_(char *color_name_p, int color_name_l)
{
   char *color_name;
   init_F77toC ;
   color_name = F77toC(color_name_p, color_name_l);

   return define_color(color_name);
}
#endif
/* ************************************************************************** */

unsigned long define_rgb_color(int red, int grn, int blu)
{
   XColor   scr_io;
   Pixel	new_pixel;

   new_pixel = xgcvl.foreground;

   if (no_x) {
      define_ps_pixel(new_pixel);
      return new_pixel;
   }
   if ((visual->class == PseudoColor) |
       (visual->class == DirectColor) |
       (visual->class == GrayScale)   ) {
      if (!color_map)
	 color_map = DefaultColormapOfScreen(pscrn);
      scr_io.red = red;
      scr_io.green = grn;
      scr_io.blue = blu;
      scr_io.flags = DoRed | DoGreen | DoBlue;
      if (XAllocColor(pdisp, color_map, &scr_io)) {
	 new_pixel = scr_io.pixel;
      }
      else {
	 printf("define_rgb_color: Could not allocate RGB color pixel with\n");
	 printf("                  R = %u, G = %u, B = %u\n", red, grn, blu);
      }
   }
   
   define_ps_pixel(new_pixel);

   return new_pixel;
}
#ifdef VMS
unsigned long define_rgb_color_c(int *red, int *grn, int *blu)
{
   return define_rgb_color(*red, *grn, *blu);
}
#else
unsigned long define_rgb_color_c_(int *red, int *grn, int *blu)
{
   return define_rgb_color(*red, *grn, *blu);
}
#endif
/* ************************************************************************** */

float set_vec_string_aspect(float w_by_h)
{
   float old_w_by_h;

   old_w_by_h = str_w_by_h;
   str_w_by_h = w_by_h;
   if (lps || no_x) {
      fprintf(pstrm, "%f setasprat\n", str_w_by_h);
   }
   return old_w_by_h;
}
#ifdef VMS
float set_vec_string_aspect_c(float *w_by_h)
{
   return set_vec_string_aspect(*w_by_h);
}
#else
float set_vec_string_aspect_c_(float *w_by_h)
{
   return set_vec_string_aspect(*w_by_h);
}
#endif
/* ************************************************************************** */

int set_ps_vec(int ips)
{
   int old_ips;

   old_ips = ps_vec;
/* ps_vec = ips; */
   return old_ips;
}
#ifdef VMS
int set_ps_vec_c(int *ips_p)
{
   return set_ps_vec(*ips_p);
}
#else
int set_ps_vec_c_(int *ips_p)
{
   return set_ps_vec(*ips_p);
}
#endif
/* ************************************************************************** */

int set_line_width(int width)
{
   int old_line_width;

   old_line_width = xgcvl.line_width;
   xgcvl.line_width = width;
   if (!lps && !no_x)
      XChangeGC(pdisp, gc, GCLineWidth, &xgcvl);
   else
      fprintf(pstrm, "%f setlinewidth\n", width/4.16);
   return old_line_width;
}
#ifdef VMS
int set_line_width_c(int *width)
{
   return set_line_width(*width);
}
#else
int set_line_width_c_(int *width)
{
   return set_line_width(*width);
}
#endif
/* ************************************************************************** */

void set_line_dashed(unsigned char *dash_list, int nelem)
{
   int i;

   if (!lps && !no_x) {
      if (nelem > 0) {
	 xgcvl.line_style = LineOnOffDash;
	 XChangeGC(pdisp, gc, GCLineStyle, &xgcvl);
	 XSetDashes(pdisp, gc, 0, dash_list, nelem);
      }
      else {
	 xgcvl.line_style = LineSolid;
	 XChangeGC(pdisp, gc, GCLineStyle, &xgcvl);
      }
   }
   else {
      if (nelem > 0) {
	 fprintf(pstrm, "[");
	 for (i = 0; i < nelem; i++) {
	    fprintf(pstrm, "%u ", *dash_list++);
	 }
	 fprintf(pstrm, "] 0 setdash\n");
      }
      else {
	 fprintf(pstrm, "[] 0 setdash\n");
      }
   }
   return;
}
#ifdef VMS
void set_line_dashed_c(unsigned char *dash_list_p, int *nelem_p)
{
   set_line_dashed(dash_list_p, *nelem_p);
   return;
}
#else
void set_line_dashed_c_(unsigned char *dash_list_p, int *nelem_p)
{
   set_line_dashed(dash_list_p, *nelem_p);
   return;
}
#endif
/* ************************************************************************** */

static XCharStruct ovral_ret;

void string(char *text, float x, float y, int xjust, int yjust)
{
   int dir_ret, asc_ret, dsc_ret, chrhgt, xpos, ypos, txtlen;
   float xps, yps, compt=14.0;

   if (lps || no_x) {
      if (ps_curpt != compt) {
	 fprintf(pstrm, "14.0 resclfnt\n");
	 ps_curpt = compt;
      }
      xps = (x - xcrnr) * psclx;
      yps = (y - ycrnr) * pscly;
      pstrp(text);
      fprintf(pstrm, "%d %d %f %f jushow\n", xjust, yjust, xps, yps);
   }
   else {
      xpos = (x-xcrnr) * wscalx;
      ypos = (wusrh+ycrnr-y) * wscaly;
      txtlen = strlen(text);
      XTextExtents(font_data, text, txtlen, &dir_ret, &asc_ret,
		   &dsc_ret, &ovral_ret);
      chrhgt = (ovral_ret.ascent + ovral_ret.descent) / 2 ;
      if (!xjust)
	 xpos = xpos - ovral_ret.width/2;
      else if (xjust > 0)
	 xpos = xpos - ovral_ret.width;
      if (yjust > 0)
	 ypos = ypos + ovral_ret.ascent + (yjust-1)*chrhgt;
      else if (!yjust)
	 ypos = ypos + chrhgt - ovral_ret.descent;
      else
	 ypos = ypos - ovral_ret.descent + (yjust+1)*chrhgt;
      XDrawString(pdisp, drwid, gc, xpos, ypos, text, txtlen);
   }
   return;
}
#ifdef VMS
void string_c(VMSstrptr text_p, float *x_p, float *y_p,
	      int *xjust_p, int *yjust_p)
{
   char *text;
   init_VMStoC ;
   text = VMStoC(text_p);

   string(text, *x_p, *y_p, *xjust_p, *yjust_p);
   return;
}
#else
void string_c_(char *text_p, float *x_p, float *y_p,
	        int *xjust_p, int *yjust_p, int text_l)
{
   char *text;
   init_F77toC ;
   text = F77toC(text_p, text_l);

   string(text, *x_p, *y_p, *xjust_p, *yjust_p);
   return;
}
#endif
/* ************************************************************************** */

void image_string(char *text, float x, float y, int xjust, int yjust)
{
   int dir_ret, asc_ret, dsc_ret, chrhgt, xpos, ypos, txtlen;
   float xps, yps, compt=14.0;

   if (lps || no_x) {
      if (ps_curpt != compt) {
	 fprintf(pstrm, "14.0 resclfnt\n");
	 ps_curpt = compt;
      }
      xps = (x - xcrnr) * psclx;
      yps = (y - ycrnr) * pscly;
      pstrp(text);
      fprintf(pstrm, "%d %d %f %f juishow\n", xjust, yjust, xps, yps);
   }
   else {
      xpos = (x-xcrnr) * wscalx;
      ypos = (wusrh+ycrnr-y) * wscaly;
      txtlen = strlen(text);
      XTextExtents(font_data, text, txtlen, &dir_ret, &asc_ret,
		   &dsc_ret, &ovral_ret);
      chrhgt = (ovral_ret.ascent + ovral_ret.descent) / 2 ;
      if (!xjust)
	 xpos = xpos - ovral_ret.width/2;
      else if (xjust > 0)
	 xpos = xpos - ovral_ret.width;
      if (yjust > 0)
	 ypos = ypos + ovral_ret.ascent + (yjust-1)*chrhgt;
      else if (!yjust)
	 ypos = ypos + chrhgt - ovral_ret.descent;
      else
	 ypos = ypos - ovral_ret.descent + (yjust+1)*chrhgt;
      XDrawImageString(pdisp, drwid, gc, xpos, ypos, text, txtlen);
   }
   return;
}
#ifdef VMS
void image_string_c(VMSstrptr text_p, float *x_p, float *y_p,
	      int *xjust_p, int *yjust_p)
{
   char *text;
   init_VMStoC ;
   text = VMStoC(text_p);

   image_string(text, *x_p, *y_p, *xjust_p, *yjust_p);
   return;
}
#else
void image_string_c_(char *text_p, float *x_p, float *y_p,
	        int *xjust_p, int *yjust_p, int text_l)
{
   char *text;
   init_F77toC ;
   text = F77toC(text_p, text_l);

   image_string(text, *x_p, *y_p, *xjust_p, *yjust_p);
   return;
}
#endif
/* ************************************************************************** */

void ps_string(char *text, float siz, float x, float y, int xjust, int yjust)
{
   float xps, yps, compt=siz*72.0;

   if (ps_curpt != compt) {
      ps_curpt = compt;
      fprintf(pstrm, "%f resclfnt\n", ps_curpt);
   }
   pstrp(text);
   fprintf(pstrm, "%d %d %7.1f %7.1f jushow\n", xjust, yjust, x*72.0, y*72.0);
   return;
}
#ifdef VMS
void ps_string_c(VMSstrptr text_p, float *siz_p, float *x_p, float *y_p,
	      int *xjust_p, int *yjust_p)
{
   char *text;
   init_VMStoC ;
   text = VMStoC(text_p);

   ps_string(text, *siz_p, *x_p, *y_p, *xjust_p, *yjust_p);
   return;
}
#else
void ps_string_c_(char *text_p, float *siz_p, float *x_p, float *y_p,
	        int *xjust_p, int *yjust_p, int text_l)
{
   char *text;
   init_F77toC ;
   text = F77toC(text_p, text_l);

   ps_string(text, *siz_p, *x_p, *y_p, *xjust_p, *yjust_p);
   return;
}
#endif
/* ************************************************************************** */

#include <math.h>
#ifdef VMS
#define M_PI 3.14159265358979323846
#endif
#define RADS_PER_DEG (M_PI/180.0)

static void rotate(float *x, float *y, int npts, float theta_deg)
{
   int i;
   float costh, sinth, xtemp;
   double theta = theta_deg * RADS_PER_DEG;

   costh = cos(theta);
   sinth = sin(theta);

   for (i = 0; i < npts; i++) {
      xtemp = x[i];
      x[i] = xtemp*costh - y[i]*sinth;
      y[i] = xtemp*sinth + y[i]*costh;
   }
   return;
}
/* ************************************************************************** */

static void displace(float *x, float *y, int npts, float x0, float y0)
{
   int i;

   for (i = 0; i < npts; i++) {
      x[i] += x0;
      y[i] += y0;
   }
   return;
}
/* ************************************************************************** */
#define MAXPTS 100
#define MAXSEG MAXPTS/2
#define MAXREC MAXPTS/2
#define MAXARC MAXPTS/3
static float usrx[MAXPTS], usry[MAXPTS];
static float usrX[MAXPTS], usrY[MAXPTS];
static XPoint		xpnts[MAXPTS+1];
static XSegment		xseg[MAXSEG];
static XRectangle	xrec[MAXREC];
static XArc		xarc[MAXARC];

/* ************************************************************************** */

typedef union {
   short		coords;
   unsigned short	dims;
} xptdim, *X_pt_dim;

static float SMIN = (float) SHRT_MIN;
static float SMAX = (float) SHRT_MAX;
/*
static float UMAX = (float) USHRT_MAX;
*/
static float UMAX = (float) 65535;

void pixscl(float *x, float *y, int npts, xptdim *xrec, Boolean lpnts,
	    int start, int step)
{
   int i;
   register float fpixx, fpixy, fpixdim;
   xptdim *nxtrec = xrec + start;

   i = 0;
   if (lpnts) {
      while (i < npts) {
/*
	 nxtrec->coords = (x[i]-xcrnr) * wscalx;
	 (nxtrec+1)->coords = (wusrh+ycrnr-y[i++]) * wscaly;
*/
	 fpixx = (x[i]-xcrnr) * wscalx;
	 nxtrec->coords = MIN(MAX(fpixx, SMIN), SMAX);
	 fpixy = (wusrh+ycrnr-y[i++]) * wscaly;
	 (nxtrec+1)->coords = MIN(MAX(fpixy, SMIN), SMAX);
	 nxtrec += step;
      }
   }
   else {
      while (i < npts) {
/*
	 nxtrec->dims = x[i] * wscalx;
	 (nxtrec+1)->dims = y[i++] * wscaly;
*/
	 fpixdim = x[i] * wscalx;
	 nxtrec->dims = MIN(fpixdim, UMAX);
	 fpixdim = y[i++] * wscaly;
	 (nxtrec+1)->dims = MIN(fpixdim, UMAX);
	 nxtrec += step;
      }
   }
   return;
}
/* ************************************************************************** */

#define MAXSYM 11
static int sympntr[MAXSYM] = {0, 1, 12, 23, 33, 43, 51, 59, 71, 83, 95};
/*
 index symbol
   0	. (point)
   1	+ (vert crosshairs)
   2	X (45 degree crosshair)
   3	   open circle
   4	   filled circle
   5	   open square
   6	   filled square
   7	   open triangle
   8	   filled triangle
   9	   open inverted triangle
  10	   filled inverted triangle
*/
void points(float *x, float *y, int npts)
{
#ifdef VMS
   static char svec[113] = {
#else
   static signed char svec[113] = {
#endif
      0,
      3,  2, -50,   0,  50,   0,   0, -50,   0,  50,   0,   3,   2, -50,
     50, 50, -50, -50, -50,  50,  50,   0,   5,   0,   1,   0,   0, 100,
    100,  0, 120,   0,   5,   1,   1,   0,   0, 100, 100,   0, 120,   0,
      4,  0,   1,   0,   0, 100, 100,   0,   4,   1,   1,   0,   0, 100,
    100,  0,   2,   0,   4,   0,  50,  50, -50, -50, -50,   0,  50,   0,
      2,  2,   4,   0,  50,  50, -50, -50, -50,   0,  50,   0,   2,   0,
      4,  0, -50, -50,  50,  50,  50,   0, -50,   0,   2,   2,   4,   0,
    -50,-50,  50,  50,  50,   0, -50,   0,   0,   0,   0,   0,   0,   0
   };
   int symbol, sym, nxt, last, nxtptr;
   int func, nsym, nelems, i, j, k, filmod, shape;
   float symsiz, siz, xps, yps, sptsx[MAXPTS], sptsy[MAXPTS];
   enum subtypes { NONE, POINTS, LINES, SEGMENTS, RECTANGLES, ARCS};

   if (npts <= 0)
      return;

   nxtptr = sptr;
   if (nxtptr <= 0) {
      if (lps || no_x) {	/* lps && nxtptr<=0 */
	 nxt = npts-1;
	 i = j = 0;
	 do {
	    fprintf(pstrm, "%7.1f %7.1f ", (x[nxt]-xcrnr)*psclx,
					   (y[nxt]-ycrnr)*pscly);
	    if ((++i) >= 5) {
	       fprintf(pstrm, "\n");
	       if ((j += i) >= MAXPTS) {
		  fprintf(pstrm, "%d points\n", j);
		  j = 0;
	       }
	       i = 0;
	    }
	    --nxt;
	 } while (nxt >= 0);
	 if ((j += i) > 0)
	    fprintf(pstrm, "\n%d points\n", j);
      }
      else {		/* !lps && nxtptr<=0 */
	 nxt = 0;
	 do {
	    last = MIN(npts-nxt, MAXPTS);
	    pixscl(x+nxt, y+nxt, last, (X_pt_dim) xpnts, True, 0, 2);
	    XDrawPoints(pdisp, drwid, gc, xpnts, last, CoordModeOrigin);
	    nxt += last;
	 } while (nxt < npts);
      }
   }
   else {		/*  nxtptr>0 */
      while (func = svec[nxtptr]) {
	 switch (func) {
	 case POINTS:
	    nsym = svec[nxtptr+1];	/* # of x,y pairs to plot */
	    nxt = 0;
	    i = nxtptr + 2;
	    while (nxt < nsym) {
	       sptsx[nxt] = svec[i++] * xscal;
	       sptsy[nxt++] = svec[i++] * yscal;
	    }
	    if (lps || no_x) {
	       nxt = npts - 1;
	       i = j = k = 0;
	       while (nxt >= 0) {
		  xps = x[nxt] - xcrnr;
		  yps = y[nxt--] - ycrnr;
		  i = 0;
		  while (i < nsym) {
		     fprintf(pstrm, "%7.1f %7.1f ", (xps+sptsx[i])*psclx,
						    (yps+sptsy[i])*pscly);
		     ++i;
		     if ((++j) >= 5) {
			fprintf(pstrm, "\n");
			if ((k += j) >= MAXPTS) {
			   fprintf(pstrm, "%d points\n", k);
			   k = 0;
			}
			j = 0;
		     }
		  }
	       }
	       if ((k += j) > 0)
		  fprintf(pstrm, "\n%d points\n", k);
	    }
	    else {
	       nxt = 0;
	       j = 0;
	       do {
		  last = MIN(npts-nxt, MAXPTS/nsym);
		  xps = x[nxt];
		  yps = y[nxt];
		  i = 0;
		  while (i < nsym) {
		     usrx[j] = xps + sptsx[i];
		     usry[j++] = yps + sptsy[i++];
		  }
		  if (j >= MAXPTS) {
		     pixscl(usrx, usry, j, (X_pt_dim) xpnts, True, 0, 2);
		     XDrawPoints(pdisp, drwid, gc, xpnts, j, CoordModeOrigin);
		     j = 0;
		  }
		  nxt += last;
	       } while (nxt < npts);
	       if (j > 0) {
		  pixscl(usrx, usry, j, (X_pt_dim) xpnts, True, 0, 2);
		  XDrawPoints(pdisp, drwid, gc, xpnts, j, CoordModeOrigin);
	       }
	    }
	    nxtptr += 2*nsym + 2;
	    break;
	 case LINES:
	    nsym = svec[nxtptr+2];	/* # of x,y pairs to connect */
	    if (nsym < 2)
	       return;
	    filmod = svec[nxtptr+1];
	    if (filmod == 1)
	       shape = Complex;
	    else if (filmod == 2)
	       shape = Convex;
	    else if (filmod == 3)
	       shape = Nonconvex;
	    else
	       filmod = 0;
	    nxt = 0;
	    i = nxtptr + 3;
	    while (nxt < nsym) {
	       sptsx[nxt] = svec[i++] * xscal;
	       sptsy[nxt++] = svec[i++] * yscal;
	    }
	    if (lps || no_x) {
	       nxt = npts - 1;
	       j = 0;
	       while (nxt >= 0) {
		  xps = x[nxt] - xcrnr;
		  yps = y[nxt--] - ycrnr;
		  i = 0;
		  while (i < nsym) {
		     fprintf(pstrm, "%7.1f %7.1f ", (xps+sptsx[i])*psclx,
						    (yps+sptsy[i])*pscly);
		     ++i;
		     if ((++j) >= 5) {
			fprintf(pstrm, "\n");
			j = 0;
		     }
		  }
		  if (filmod>=1 && filmod<=3)
		     fprintf(pstrm, "%d polyfill\n", i);
		  else
		     fprintf(pstrm, "\n%d polyline\n", i);
	       }
	    }
	    else {
	       nxt = 0;
	       do {
		  xps = x[nxt];
		  yps = y[nxt++];
		  i = 0;
		  while (i < nsym) {
		     usrx[i] = xps + sptsx[i];
		     usry[i] = yps + sptsy[i];
		     ++i;
		  }
		  pixscl(usrx, usry, i, (X_pt_dim) xpnts, True, 0, 2);
		  if (!filmod)
		     XDrawLines(pdisp, drwid, gc, xpnts, i, CoordModeOrigin);
		  else
		     XFillPolygon(pdisp, drwid, gc, xpnts, i, shape,
				  CoordModeOrigin);
	       } while (nxt < npts);
	    }
	    nxtptr += 2*nsym + 3;
	    break;
	 case SEGMENTS:
	    nsym = svec[nxtptr+1];	/* # of line segments */
	    if (nsym < 1)
	       return;
	    nelems = 2*nsym;
	    nxt = 0;
	    i = nxtptr + 2;
	    while (nxt < nelems) {
	       sptsx[nxt] = svec[i++] * xscal;
	       sptsy[nxt++] = svec[i++] * yscal;
	       sptsx[nxt] = svec[i++] * xscal;
	       sptsy[nxt++] = svec[i++] * yscal;
	    }
	    if (lps || no_x) {
	       nxt = npts - 1;
	       j = k = 0;
	       while (nxt >= 0) {
		  xps = x[nxt] - xcrnr;
		  yps = y[nxt--] - ycrnr;
		  i = 0;
		  while (i < nelems) {
		     fprintf(pstrm, "%7.1f %7.1f ", (xps+sptsx[i])*psclx,
						    (yps+sptsy[i])*pscly);
		     ++i;
		     fprintf(pstrm, "%7.1f %7.1f ", (xps+sptsx[i])*psclx,
						    (yps+sptsy[i])*pscly);
		     ++i;
		     if ((++j) >= 2) {
			fprintf(pstrm, "\n");
			if ((k += j) >= MAXSEG) {
			   fprintf(pstrm, "%d segments\n", k);
			   k = 0;
			}
			j = 0;
		     }
		  }
	       }
	       if ((k += j) > 0)
		  fprintf(pstrm, "\n%d segments\n", k);
	    }
	    else {
	       nxt = j = 0;
	       do {
		  xps = x[nxt];
		  yps = y[nxt++];
		  i = 0;
		  while (i < nelems) {
		     usrx[j] = xps + sptsx[i];
		     usry[j++] = yps + sptsy[i++];
		     usrx[j] = xps + sptsx[i];
		     usry[j++] = yps + sptsy[i++];
		     if (j >= MAXPTS) {
			pixscl(usrx, usry, j, (X_pt_dim) xseg, True, 0, 2);
			XDrawSegments(pdisp, drwid, gc, xseg, j/2);
			j = 0;
		     }
		  }
	       } while (nxt < npts);
	       if (j > 0) {
		  pixscl(usrx, usry, j, (X_pt_dim) xseg, True, 0, 2);
		  XDrawSegments(pdisp, drwid, gc, xseg, j/2);
	       }
	    }
	    nxtptr += 4*nsym + 2;
	    break;
	 case RECTANGLES:
	    nsym = svec[nxtptr+2];	/* # of rectangles */
	    if (nsym < 1)
	       return;
	    nelems = 2*nsym;
	    filmod = svec[nxtptr+1];
	    nxt = 0;
	    i = nxtptr + 3;
	    while (nxt < nelems) {
	       sptsx[nxt] = svec[i++]*xscal ;	/* x,y of center */
	       sptsy[nxt++] = svec[i++]*yscal;
	       sptsx[nxt] = svec[i++]*xscal;	/* width */
	       sptsy[nxt] = svec[i++]*yscal;	/* height */
	       sptsx[nxt-1] -= sptsx[nxt]/2.0;	/* x,y of top left corner */
	       sptsy[nxt-1] += sptsy[nxt]/2.0;
	       ++nxt;
	    }
	    if (lps || no_x) {
	       nxt = npts - 1;
	       j = k = 0;
	       while (nxt >= 0) {
		  xps = x[nxt] - xcrnr;
		  yps = y[nxt--] - ycrnr;
		  i = 0;
		  while (i < nelems) {
		     ++i;
		     fprintf(pstrm, "%7.1f %7.1f ", sptsx[i]*psclx,
						    sptsy[i]*pscly);
		     --i;
		     fprintf(pstrm, "%7.1f %7.1f ", (xps+sptsx[i])*psclx,
						    (yps+sptsy[i])*pscly);
		     i += 2;
		     if ((++j) >= 2) {
			fprintf(pstrm, "\n");
			if ((k += j) >= MAXREC) {
			   if (filmod == 1)
			      fprintf(pstrm, "%d fillrectangles\n", k);
			   else
			      fprintf(pstrm, "%d rectangles\n", k);
			   k = 0;
			}
			j = 0;
		     }
		  }
	       }
	       if ((k += j) > 0) {
		  if (filmod == 1)
		     fprintf(pstrm, "\n%d fillrectangles\n", k);
		  else
		     fprintf(pstrm, "\n%d rectangles\n", k);
	       }
	    }
	    else {
	       nxt = j = 0;
	       do {
		  xps = x[nxt];
		  yps = y[nxt++];
		  i = 0;
		  while (i < nelems) {
		     usrx[j] = xps + sptsx[i];
		     usry[j] = yps + sptsy[i++];
		     usrx[j+MAXREC] = sptsx[i];
		     usry[j+MAXREC] = sptsy[i++];
		     if (++j >= MAXREC) {
			pixscl(usrx, usry, j, (X_pt_dim) xrec, True, 0, 4);
			pixscl(usrx+MAXREC, usry+MAXREC, j, (X_pt_dim) xrec,
			       False, 2, 4);
			if (filmod == 1)
			   XFillRectangles(pdisp, drwid, gc, xrec, j);
			else
			   XDrawRectangles(pdisp, drwid, gc, xrec, j);
			j = 0;
		     }
		  }
	       } while (nxt < npts);
	       if (j > 0) {
		  pixscl(usrx, usry, j, (X_pt_dim) xrec, True, 0, 4);
		  pixscl(usrx+MAXREC, usry+MAXREC, j, (X_pt_dim) xrec,
			 False, 2, 4);
		  if (filmod == 1)
		     XFillRectangles(pdisp, drwid, gc, xrec, j);
		  else
		     XDrawRectangles(pdisp, drwid, gc, xrec, j);
	       }
	    }
	    nxtptr += 4*nsym + 3;
	    break;
	 case ARCS:
	    nsym = svec[nxtptr+2];	/* # of arcs */
	    if (nsym < 1)
	       return;
	    nelems = 3*nsym;
	    filmod = svec[nxtptr+1];
	    if (lps || no_x) {
	       nxt = 0;
	       i = nxtptr + 3;
	       while (nxt < nelems) {
		  sptsx[nxt] = svec[i++]*xscal;		   /* x,y of center */
		  sptsy[nxt++] = svec[i++]*yscal;
		  sptsx[nxt] = svec[i++]*xscal;		   /* width */
		  sptsy[nxt++] = svec[i++]*yscal;	   /* height */
		  sptsx[nxt] = svec[i++]*3.0;		   /* begining angle */
		  sptsy[nxt] = sptsx[nxt]+svec[i++]*3.0;   /* ending angle */
		  ++nxt;
	       }
	       nxt = npts - 1;
	       j = k = 0;
	       while (nxt >= 0) {
		  xps = x[nxt] - xcrnr;
		  yps = y[nxt--] - ycrnr;
		  i = 0;
		  while (i < nelems) {
		     fprintf(pstrm, "%7.1f %7.1f ", (xps+sptsx[i])*psclx,
						    (yps+sptsy[i])*pscly);
		     fprintf(pstrm, "%7.1f %7.1f ", sptsx[i+2], sptsy[i+2]);
		     fprintf(pstrm, "%7.1f %7.1f\n", sptsx[i+1]*psclx,
						     sptsy[i+1]*pscly);
		     i += 3;
		     if (++k >= MAXARC) {
			if (filmod == 1)
			   fprintf(pstrm, "%d fillarcs\n", k);
			else
			   fprintf(pstrm, "%d arcs\n", k);
			k = 0;
		     }
		  }
	       }
	       if (k  > 0) {
		  if (filmod == 1)
		     fprintf(pstrm, "%d fillarcs\n", k);
		  else
		     fprintf(pstrm, "%d arcs\n", k);
	       }
	    }
	    else {
	       nxt = 0;
	       i = nxtptr + 3;
	       while (nxt < nelems) {
		  sptsx[nxt] = svec[i++]*xscal;	  /* x,y of center */
		  sptsy[nxt++] = svec[i++]*yscal;
		  sptsx[nxt] = svec[i++]*xscal;	  /* width */
		  sptsy[nxt] = svec[i++]*yscal;	  /* height */
		  sptsx[nxt-1] -= sptsx[nxt]/2.0; /* x,y of top left corner */
		  sptsy[nxt-1] += sptsy[nxt]/2.0;
		  sptsx[++nxt] = svec[i++]*192.0; /* begin angle */
		  sptsy[nxt++] = svec[i++]*192.0; /* ending angle */
	       }
	       nxt = j = 0;
	       do {
		  xps = x[nxt];
		  yps = y[nxt++];
		  i = 0;
		  while (i < nelems) {
		     usrx[j] = xps + sptsx[i];
		     usry[j] = yps + sptsy[i++];
		     usrx[j+MAXARC] = sptsx[i];
		     usry[j+MAXARC] = sptsy[i++];
		     xarc[j].angle1 = sptsx[i];
		     xarc[j].angle2 = sptsy[i++];
		     if (++j >= MAXARC) {
			pixscl(usrx, usry, j, (X_pt_dim) xarc, True, 0, 6);
			pixscl(usrx+MAXARC, usry+MAXARC, j, (X_pt_dim) xarc,
			       False, 2, 6);
			if (filmod == 1)
			   XFillArcs(pdisp, drwid, gc, xarc, j);
			else
			   XDrawArcs(pdisp, drwid, gc, xarc, j);
			j = 0;
		     }
		  }
	       } while (nxt < npts);
	       if (j > 0) {
		  pixscl(usrx, usry, j, (X_pt_dim) xarc, True, 0, 6);
		  pixscl(usrx+MAXARC, usry+MAXARC, j, (X_pt_dim) xarc,
			 False, 2, 6);
		  if (filmod == 1)
		     XFillArcs(pdisp, drwid, gc, xarc, j);
		  else
		     XDrawArcs(pdisp, drwid, gc, xarc, j);
	       }
	    }
	    nxtptr += 6*nsym + 3;
	    break;
	 default:
	    fprintf(stderr, "points: Illegal symbol sub-type = %d\n", func);
	    return;
	 }
      }
   }
   return;
}
#ifdef VMS
void points_c(float *x, float *y, int *npts_p)
{
   points(x, y, *npts_p);
   return;
}
#else
void points_c_(float *x, float *y, int *npts_p)
{
   points(x, y, *npts_p);
   return;
}
#endif
/* ************************************************************************** */

void set_symbol(int symbol, float symsiz)
{
   if (symbol > 0 && symbol < MAXSYM) {
      sptr = sympntr[symbol];
      xscal = symsiz*xpxmm/(100.0*wscalx);
      yscal = symsiz*ypxmm/(100.0*wscaly);
   }
   else
      sptr = 0;
   return;
}
#ifdef VMS
void set_symbol_c(int *symbol_p, float *symsiz_p)
{
   set_symbol(*symbol_p, *symsiz_p);
   return;
}
#else
void set_symbol_c_(int *symbol_p, float *symsiz_p)
{
   set_symbol(*symbol_p, *symsiz_p);
   return;
}
#endif
/* ************************************************************************** */

void lines(int mode, float *x, float *y, int npts)
{
   int i, j, nxt, todo, shape;

   if (npts < 2)
      return;

   if (lps || no_x) {
      nxt = npts;
      i = j = 0;
      while (--nxt >= 0) {
	 fprintf(pstrm, "%7.1f %7.1f ", (x[nxt] - xcrnr) * psclx,
					(y[nxt] - ycrnr) * pscly);
	 if ((++i) >= 5) {
	    fprintf(pstrm, "\n");
	    if ((j += i) >= MAXPTS) {
	       fprintf(pstrm, "%d polyline\n", j);
	       ++nxt;
	       j = 0;
	    }
	    i = 0;
	 }
      }
      if ((j += i) > 0) {
	 if (mode>=1 && mode<=3)
	    fprintf(pstrm, "%d polyfill\n", j);
	 else if (mode>=4 && mode<=6)
	    fprintf(pstrm, "%d polylinefill\n", j);
	 else
	    fprintf(pstrm, "\n%d polyline\n", j);
      }
   }
   else {
      nxt = i = 0;
      do {
	 if (i)
	    todo = MIN(npts-nxt, MAXPTS-1);
	 else
	    todo = MIN(npts-nxt, MAXPTS);
	 pixscl(x+nxt, y+nxt, todo, (X_pt_dim) xpnts, True, i, 2);
	 nxt += todo;
	 if (nxt < npts) {
	    xpnts[MAXPTS-1].x = (xpnts[MAXPTS-1].x + xpnts[MAXPTS-2].x)/2;
	    xpnts[MAXPTS-1].y = (xpnts[MAXPTS-1].y + xpnts[MAXPTS-2].y)/2;
	    XDrawLines(pdisp, drwid, gc, xpnts, MAXPTS, CoordModeOrigin);
	    xpnts[0].x = xpnts[MAXPTS-1].x;
	    xpnts[0].y = xpnts[MAXPTS-1].y;
	    i = 2;
	    --nxt;
	 }
	 else {
	    if (i)
	       ++todo;
	    if (!mode || mode>3)
	       XDrawLines(pdisp, drwid, gc, xpnts, todo, CoordModeOrigin);
	    if (mode>0 && mode<=6) {
	       if (mode==1 || mode==4)
		  shape = Complex;
	       else if (mode==2 || mode==5)
		  shape = Convex;
	       else
		  shape = Nonconvex;
	       XFillPolygon(pdisp, drwid, gc, xpnts, todo, shape,
			    CoordModeOrigin);
	    }
	    break;
	 }
      } while (True);
   }

   return;
}
#ifdef VMS
void lines_c(int *mode_p, float *x, float *y, int *npts_p)
{
   lines(*mode_p, x, y, *npts_p);
   return;
}
#else
void lines_c_(int *mode_p, float *x, float *y, int *npts_p)
{
   lines(*mode_p, x, y, *npts_p);
   return;
}
#endif
/* ************************************************************************** */

void lines_array(int mode, float *relx, float *rely, int nrel,
	         float *xloc, float *yloc, float *rloc, int nloc)
{
   int i, j, npts;
   float last_rloc;

   if ((npts = MIN(nrel, MAXPTS)) <= 1)
      return;
   last_rloc = rloc[0] + 1.0;
   i = 0;
   while (i < nloc) {
      if (rloc[i] != last_rloc) {
	 j = 0;
	 do {
	    usrx[j] = relx[j];
	    usry[j] = rely[j];
	 } while (++j < npts);
	 if (rloc[i] != 0.0)
	    rotate(usrx, usry, npts, rloc[i]);
	 last_rloc = rloc[i];
      }
      displace(usrx, usry, npts, xloc[i], yloc[i]);
      lines(mode, usrx, usry, npts);
      ++i;
   }
}
#ifdef VMS
void lines_array_c(int *mode_p, float *relx, float *rely, int *nrel_p,
		   float *xloc, float *yloc, float *rloc, int *nloc_p)
{
   lines_array(*mode_p, relx, rely, *nrel_p, xloc, yloc, rloc, *nloc_p);
   return;
}
#else
void lines_array_c_(int *mode_p, float *relx, float *rely, int *nrel_p,
		    float *xloc, float *yloc, float *rloc, int *nloc_p)
{
   lines_array(*mode_p, relx, rely, *nrel_p, xloc, yloc, rloc, *nloc_p);
   return;
}
#endif
/* ************************************************************************** */

void segments(float *usrx1, float *usry1, float *usrx2, float *usry2, int nsgs)
{
   int i, j, nxt, todo;

   if (lps || no_x) {
      nxt = nsgs;
      i = j = 0;
      while (--nxt >= 0) {
	 fprintf(pstrm, "%7.1f %7.1f %7.1f %7.1f ",
		 (usrx2[nxt] - xcrnr) * psclx,
	         (usry2[nxt] - ycrnr) * pscly,
		 (usrx1[nxt] - xcrnr) * psclx,
	         (usry1[nxt] - ycrnr) * pscly);
	 if ((++i) >= 2) {
	    fprintf(pstrm, "\n");
	    if ((j += i) >= MAXSEG) {
	       fprintf(pstrm, "%d segments\n", j);
	       j = 0;
	    }
	    i = 0;
	 }
      }
      if ((j += i) > 0)
	 fprintf(pstrm, "\n%d segments\n", j);
   }
   else {
      nxt = 0;
      while (nxt < nsgs) {
	 todo = MIN(nsgs-nxt, MAXSEG);
	 pixscl(usrx1+nxt, usry1+nxt, todo, (X_pt_dim) xseg, True, 0, 4);
	 pixscl(usrx2+nxt, usry2+nxt, todo, (X_pt_dim) xseg, True, 2, 4);
	 XDrawSegments(pdisp, drwid, gc, xseg, todo);
	 nxt += todo;
      }
   }

   return;
}
#ifdef VMS
void segments_c(float *usrx1, float *usry1, float *usrx2, float *usry2,
		int *nsgs_p)
{
   segments(usrx1, usry1, usrx2, usry2, *nsgs_p);
   return;
}
#else
void segments_c_(float *usrx1, float *usry1, float *usrx2, float *usry2,
		int *nsgs_p)
{
   segments(usrx1, usry1, usrx2, usry2, *nsgs_p);
   return;
}
#endif
/* ************************************************************************** */

void rectangles(int mode, float *width, float *hght, float *x, float *y,
		int xjus, int yjus, int nrecs)
{
/*
   mode = 0  Just draw rectangle outlines
   mode = 1  Just fill rectangles
   mode = 2  Draw and then fill rectangles
   xjus = -1,0 or +1 means that the given X coords are at the left edge, centre
           or right edge of the rectangles
   yjus = -1,0 or +1 means that the given Y coords are at the bottom edge,
           center or top edge of the rectangles
*/
   int i, j, k;
   float xofst, yofst;

   if (nrecs <=0)
      return;

   xofst = (xjus + 1.0)/2.0;
   yofst = (1.0 - yjus)/2.0;

   if (lps || no_x) {
      i = nrecs;
      j = k = 0;
      while (--i >= 0) {
	 fprintf(pstrm, "%7.1f %7.1f ", width[i]*psclx, hght[i]*pscly);
	 fprintf(pstrm, "%7.1f %7.1f ", (x[i]-xofst*width[i]-xcrnr)*psclx,
					(y[i]+yofst*hght[i]-ycrnr)*pscly);
	 if ((++j) >= 2) {
	    fprintf(pstrm, "\n");
	    if ((k += j) >= MAXREC) {
	       if (mode == 1)
		  fprintf(pstrm, "%d fillrectangles\n", k);
	       else if (mode == 2)
		  fprintf(pstrm, "%d drawfillrectangles\n", k);
	       else
		  fprintf(pstrm, "%d rectangles\n", k);
	       k = 0;
	    }
	    j = 0;
	 }
      }
      if ((k += j) > 0) {
	 if (mode == 1)
	    fprintf(pstrm, "\n%d fillrectangles\n", k);
	 else if (mode == 2)
	    fprintf(pstrm, "\n%d drawfillrectangles\n", k);
	 else
	    fprintf(pstrm, "\n%d rectangles\n", k);
      }
   }
   else {
      i = 0;
      while (i < nrecs) {
	 j = i;
	 for (k = 0; k < MIN(nrecs-i, MAXREC); j++, k++) {
	    usrx[k] = x[j] - xofst*width[j];
	    usry[k] = y[j] + yofst*hght[j];
	 }
	 pixscl(usrx, usry, k, (X_pt_dim) xrec, True, 0, 4);
	 pixscl(width+i, hght+i, k, (X_pt_dim) xrec, False, 2, 4);
	 if (mode == 1)
	    XFillRectangles(pdisp, drwid, gc, xrec, k);
	 else if (mode == 2) {
	    XDrawRectangles(pdisp, drwid, gc, xrec, k);
	    XFillRectangles(pdisp, drwid, gc, xrec, k);
	 }
	 else
	    XDrawRectangles(pdisp, drwid, gc, xrec, k);
	 i += k;
      }
   }

   return;
}
#ifdef VMS
void rectangles_c(int *mode_p, float *width, float *hght, float *x, float *y,
		  int *xjus_p, int *yjus_p, int *nrecs_p)
{
   rectangles(*mode_p, width, hght, x, y, *xjus_p, *yjus_p, *nrecs_p);
   return;
}
#else
void rectangles_c_(int *mode_p, float *width, float *hght, float *x, float *y,
		   int *xjus_p, int *yjus_p, int *nrecs_p)
{
   rectangles(*mode_p, width, hght, x, y, *xjus_p, *yjus_p, *nrecs_p);
   return;
}
#endif
/* ************************************************************************** */

void arcs(int mode, float *x, float *y, float *width, float *hght,
	  float *begang, float *arcang, int xjus, int yjus, int narcs)
{
   int i, j, k;
   float xofst, yofst, usrba, usrea;

   if (narcs <=0)
      return;

   if (lps || no_x) {
      xofst = xjus/2.0;
      yofst = yjus/2.0;
      i = narcs;
      j = 0;
      while (--i >= 0) {
	 fprintf(pstrm, "%7.1f %7.1f ", (x[i]-xofst*width[i]-xcrnr)*psclx,
					(y[i]+yofst*hght[i]-ycrnr)*pscly);
	 usrba = fmod((double) begang[i], 360.0);
	 if (fabs((double) arcang[i]) <= 360.0) {
	    if (arcang[i] >= 0.0)
	       usrea = usrba + arcang[i];
	    else {
	       usrea = usrba;
	       usrba += arcang[i];
	    }
	 }
	 else {
	    if (arcang[i] >= 0.0)
	       usrea = usrba + fmod((double) arcang[i], 360.0);
	    else {
	       usrea = usrba;
	       usrba += fmod((double) arcang[i], 360.0);
	    }
	 }
	 fprintf(pstrm, "%7.1f %7.1f ", usrba, usrea);
	 fprintf(pstrm, "%7.1f %7.1f\n", width[i]*psclx, hght[i]*pscly);
	 if ((++j) >= MAXARC) {
	    if (mode == 1)
	       fprintf(pstrm, "%d fillarcs\n", j);
	    else if (mode == 2)
	       fprintf(pstrm, "%d drawfillarcs\n", j);
	    else
	       fprintf(pstrm, "%d arcs\n", j);
	    j = 0;
	 }
      }
      if (j > 0) {
	 if (mode == 1)
	    fprintf(pstrm, "%d fillarcs\n", j);
	 else if (mode == 2)
	    fprintf(pstrm, "%d drawfillarcs\n", j);
	 else
	    fprintf(pstrm, "%d arcs\n", j);
      }
   }
   else {
      xofst = (xjus + 1.0)/2.0;
      yofst = (1.0 - yjus)/2.0;
      i = 0;
      while (i < narcs) {
	 j = i;
	 for (k = 0; k < MIN(narcs-i, MAXARC); j++, k++) {
	    usrx[k] = x[j] - xofst*width[j];
	    usry[k] = y[j] + yofst*hght[j];
	    xarc[k].angle1 = fmod((double) begang[j], 360.0) * 64;
	    if (fabs((double) arcang[j]) <= 360.0)
	       xarc[k].angle2 = arcang[j] * 64;
	    else
	       xarc[k].angle2 = fmod((double) arcang[j], 360.0) * 64;
	 }
	 pixscl(usrx, usry, k, (X_pt_dim) xarc, True, 0, 6);
	 pixscl(width+i, hght+i, k, (X_pt_dim) xarc, False, 2, 6);
	 if (mode == 1)
	    XFillArcs(pdisp, drwid, gc, xarc, k);
	 else if (mode == 2) {
	    XDrawArcs(pdisp, drwid, gc, xarc, k);
	    XFillArcs(pdisp, drwid, gc, xarc, k);
	 }
	 else
	    XDrawArcs(pdisp, drwid, gc, xarc, k);
	 i += k;
      }
   }

   return;
}
#ifdef VMS
void arcs_c(int *mode_p, float *x, float *y, float *width, float *hght,
	    float *begang, float *arcang,
	    int *xjus_p, int *yjus_p, int *narcs_p)
{
   arcs(*mode_p, x, y, width, hght, begang, arcang, *xjus_p, *yjus_p, *narcs_p);
   return;
}
#else
void arcs_c_(int *mode_p, float *x, float *y, float *width, float *hght,
	     float *begang, float *arcang,
	     int *xjus_p, int *yjus_p, int *narcs_p)
{
   arcs(*mode_p, x, y, width, hght, begang, arcang, *xjus_p, *yjus_p, *narcs_p);
   return;
}
#endif
/* ************************************************************************** */

void arcs_array(int mode, float *x, float *y, float width, float hght,
		float begang, float arcang, int xjus, int yjus, int nloc)
{
   int i, j, npts;

   if ((npts = MIN(nloc, MAXPTS/2)) < 1)
      return;

   j = 0;
   do {
      usrX[j] = width;
      usrY[j] = hght;
      usrX[j+MAXPTS/2] = begang;
      usrY[j+MAXPTS/2] = arcang;
   } while (++j < npts);

   i = nloc;
   j = 0;
   while (i > 0) {
      if (i >= npts)
	 arcs(mode, x+j, y+j, usrX, usrY, usrX+MAXPTS/2, usrY+MAXPTS/2,
	      xjus, yjus, npts);
      else
	 arcs(mode, x+j, y+j, usrX, usrY, usrX+MAXPTS/2, usrY+MAXPTS/2,
	      xjus, yjus, i);
      i -= npts;
      j += npts;
   }

   return;
}
#ifdef VMS
void arcs_array_c(int *mode_p, float *x, float *y,
		  float *width_p, float *hght_p,
		  float *begang_p, float *arcang_p,
		  int *xjus_p, int *yjus_p, int *narcs_p)
{
   arcs_array(*mode_p, x, y, *width_p, *hght_p, *begang_p, *arcang_p,
	      *xjus_p, *yjus_p, *narcs_p);
   return;
}
#else
void arcs_array_c_(int *mode_p, float *x, float *y,
		   float *width_p, float *hght_p,
		   float *begang_p, float *arcang_p,
		   int *xjus_p, int *yjus_p, int *narcs_p)
{
   arcs_array(*mode_p, x, y, *width_p, *hght_p, *begang_p, *arcang_p,
	      *xjus_p, *yjus_p, *narcs_p);
   return;
}
#endif
/* ************************************************************************** */

int set_vec_font(int ifont)
{
   int lastfnt;

   if ((ifont < 0) || (ifont > 23))
      return(-1);
   lastfnt = vec_font;
   vec_font = ifont;
   return(lastfnt);
}
#ifdef VMS
int set_vec_font_c(int *ifont_p)
{
   return set_vec_font(*ifont_p);
}
#else
int set_vec_font_c_(int *ifont_p)
{
   return set_vec_font(*ifont_p);
}
#endif
/* ************************************************************************** */

int get_vec_font_name(int ifont, char *fontname, int maxlen)
{
#include  "hnam.h"
   int lastfnt;

   if ((ifont < 0) || (ifont > 23) || (maxlen < 9))
      return(-1);
   strncpy(fontname, hnam[ifont], maxlen-1);
   return(strlen(fontname));
}
#ifdef VMS
int get_vec_font_name_c(int *ifont_p, VMSstrptr fontname_p)
{
   int maxlen;
   char *fontname;
   init_VMStoC ;
   fontname = adr_CtoVMS(fontname_p);
   maxlen = len_CtoVMS(fontname_p);

   return get_vec_font_name(*ifont_p, fontname, maxlen);
}
#else
int get_vec_font_name_c_(int *ifont_p, char *fontname_p, int maxlen_l)
{
   init_F77toC ;

   return get_vec_font_name(*ifont_p, fontname_p, maxlen_l);
}
#endif
/* ************************************************************************** */

#include  "hvec.h"
#include  "hmap.h"
#include  "hfnt.h"

static float hbsl[] = {0.0, -5.0, -6.0, -9.0};
static float hcpl[] = {0.0, 4.0, 7.0, 12.0};
static float hsiz[] = {0.0, 9.0, 13.0, 21.0};
static int alt_font[] = {
   10, 15, 19, 19, 19, 15, 16, 18, 15, 16,
   19, 18, 18, 18,  8,  5,  6,  7,  7,  5,
   15, 18, 18, 18
};
static float usrmmx, usrmmy, costheta, sintheta, cos21, sin21;

#define EXPFAC 0.5

int do_vec(float x, float y, int xjus, int yjus, float hght,
	   char *str, float theta_deg, float *retx, float *rety)
{
   unsigned char chr;
   char *mxstr;
   int indx, ih, pnt, k=0, n, maxk, nchr, lit, first, bsp, jx, jy;
   int x0, y0, x1, y1, yodd, yevn, font, fnt, lastfnt, sfont, mthfnt;
   float inw, maxinw, lb0, lb, rb, curwidth, nxtxofst, nxtyofst;
   float costh, sinth, hw, xofst, yofst, xshft, yshft, lastx, lasty, rx, ry;
   float xps, yps, compt=sizfact*hght*72.0*1.6527/25.4;
   double theta = theta_deg * RADS_PER_DEG;

   costh = cos21*hght;
   sinth = sin21*hght;
   *retx = x;
   *rety = y;

   nchr = 0;
   inw = maxinw = lb = rb = 0.0;
   bsp = lit = 0;
   curwidth = 1.0;
   first = 1;
   sfont = 22;
   mthfnt = 11;
   lastfnt = font = vec_font;
   while (chr = str[nchr++]) {
      if (chr == '\n')
	 break;
      else if (!lit) {
	 if (chr == '\\') {
	    lit = 1;
	    continue;
	 }
	 else if ((chr == '{') || (chr == '[')) {
	    curwidth *= EXPFAC;
	    continue;
	 }
	 else if ((chr == '}') || (chr == ']')) {
	    if (fabs(1.0 - ((double) curwidth)) > (1.0-EXPFAC)/2.0)
	       curwidth /= EXPFAC;
	    else
	       break;
	    continue;
	 }
	 else if (chr == '$') {
	    if (font==vec_font)
	       font = alt_font[vec_font];
	    else
	       font = vec_font;
	    continue;
	 }
	 else if (chr == '%') {
	    fnt = font;
	    font = mthfnt;
	    mthfnt = fnt;
	    continue;
	 }
	 else if (chr == '#') {
	    fnt = font;
	    font = sfont;
	    sfont = fnt;
	    continue;
	 }
	 else if (chr == '<') {
	    if (!bsp)
	       inw -= (rb-lb);
	    else if (bsp == 1)
	       inw -= (rb-lb)/2.0;
	    bsp = 2;
	    continue;
	 }
	 else if (chr == '|') {
	    if (!bsp) {
	       inw -= (rb-lb)/2.0;
	       bsp = 1;
	    }
	    else if (bsp == 1) {
	       inw -= (rb-lb)/2.0;
	       bsp = 2;
	    }
	    continue;
	 }
      }
      else if (isalpha((int) chr)) {
	 lastfnt = font;
	 if (isupper((int) chr))
	    font = chr - ((unsigned int) 'A');
	 else
	    font = chr - ((unsigned int) 'a');
	 lit = 0;
	 if (font > 23)
	    font = 0;
	 else if (font < 0)
	    font = 23;
	 continue;
      }
      else if (chr == ' ') {
	 font = lastfnt;
	 lit = 0;
	 continue;
      }
      k = hmap[hfnt[font][chr]]+1;
      lb = vec[k][0]*curwidth;
      if (first) {
	 lb0 = lb;
	 ih = vec[k-1][1];
	 first = 0;
      }
      rb = vec[k][1]*curwidth;
      inw += rb - vec[k][0]*curwidth;
      if (inw > maxinw)
	 maxinw = inw;
      lit = 0;
      bsp = 0;
   }
   if (!k || (nchr < 2)) {
      printf("Exiting do_vec - no characters in string!\n");
      return nchr;
   }

   inw = maxinw -= (rb - lb0);
   mxstr = str + nchr;

   if (xjus < 0)
      xshft = 0.0;
   else if (xjus > 0)
      xshft = lb0-inw-rb;
   else
      xshft = (lb0-inw-rb)/2.0;

   if (yjus < 0) {
      yodd = (-yjus-1) % 2;
      yevn = (-yjus-1) / 2;
      yshft = -hbsl[ih] + yodd*hcpl[ih] + yevn*hsiz[ih];
   }
   else if (yjus > 0) {
      yodd = (yjus-1) % 2;
      yevn = (yjus-1) / 2;
      yshft = -(hcpl[ih] - yodd*hbsl[ih] + yevn*hsiz[ih]);
   }
   else
      yshft = 0.0;

   nxtxofst = lastx = xofst = x + (costh*xshft - sinth*yshft)*str_w_by_h*usrmmx;
   nxtyofst = lasty = yofst = y + (sinth*xshft + costh*yshft)*str_w_by_h*usrmmy;

   nchr = 0;
   lit = 0;
   sfont = 22;
   mthfnt = 11;
   lastfnt = font = vec_font;
   indx = 0;
   bsp = 0;
   while (((str+nchr) < mxstr) && (chr = str[nchr++])) {
      if (chr == '\n') {
	 xofst = lastx + 1.4*hsiz[ih]*sinth*usrmmx;
	 yofst = lasty - 1.4*hsiz[ih]*costh*usrmmy;
	 lastx = xofst;
	 lasty = yofst;
	 lit = 0;
	 continue;
      }
      else if (!lit) {
	 if (chr == '\\') {
	    lit = 1;
	    continue;
	 }
	 else if (indx && ((chr == '{') || (chr == '[') ||
			   (chr == '|') || (chr == '<'))) {
	    x0 =  50;
	    x1 = -50;
	    y0 = -50;
	    y1 =  50;
	    for (k = indx+2; k < maxk; ++k) {
	       if (vec[k][0] != (signed char) -50) {
		  if (vec[k][0] < x0)
		     x0 = vec[k][0];
		  if (vec[k][0] > x1)
		     x1 = vec[k][0];
		  if (vec[k][1] > y0)
		     y0 = vec[k][1];
		  if (vec[k][1] < y1)
		     y1 = vec[k][1];
	       }
	    }
	    if (chr == '{') {
	       if (bsp == 1) {
		  jx = 0;
		  jy = -2;
	       }
	       else if (bsp == 2) {
		  jx = 1;
		  jy = 0;
	       }
	       else {
		  jx = -1;
		  jy = 0;
	       }
	       n = do_vec(xofst+y1*sinth*usrmmx, yofst-y1*costh*usrmmy, jx, jy,
			  hght*EXPFAC, str+nchr, theta_deg, &rx, &ry);
	       if (((rx-y1*sinth*usrmmx-nxtxofst)*costheta+
		    (ry+y1*costh*usrmmy-nxtyofst)*sintheta) > 0.0) {
		  nxtxofst = rx-y1*sinth*usrmmx;
		  nxtyofst = ry+y1*costh*usrmmy;
	       }
	       nchr += n;
	       if ((str[nchr] != '[') && (str[nchr] != '<')
				      && (str[nchr] != '|')) {
		  xofst = nxtxofst;
		  yofst = nxtyofst;
	       }
	    }
	    else if (chr == '[') {
	       if (bsp == 1) {
		  jx = 0;
		  jy = 2;
	       }
	       else if (bsp == 2) {
		  jx = 1;
		  jy = 0;
	       }
	       else {
		  jx = -1;
		  jy = 0;
	       }
	       n = do_vec(xofst+y0*sinth*usrmmx, yofst-y0*costh*usrmmy,
			  jx, jy, hght*EXPFAC, str+nchr, theta_deg, &rx, &ry);
	       if (((rx-y0*sinth*usrmmx-nxtxofst)*costheta+
		    (ry+y0*costh*usrmmy-nxtyofst)*sintheta) > 0.0) {
		  nxtxofst = rx-y0*sinth*usrmmx;
		  nxtyofst = ry+y0*costh*usrmmy;
	       }
	       nchr += n;
	       if ((str[nchr] != '{') && (str[nchr] != '<')
				      && (str[nchr] != '|')) {
		  xofst = nxtxofst;
		  yofst = nxtyofst;
	       }
	    }
	    else if (chr == '<') {
	       if (!bsp) {
		  xofst -= costh*(rb-lb)*str_w_by_h*usrmmx;
		  yofst -= sinth*(rb-lb)*str_w_by_h*usrmmy;
	       }
	       else if (bsp == 1) {
		  xofst -= costh*(rb-lb)*str_w_by_h*usrmmx/2.0;
		  yofst -= sinth*(rb-lb)*str_w_by_h*usrmmy/2.0;
	       }
	       bsp = 2;
	       if (str[nchr] == '~') {
		  xofst -= costh*lb*str_w_by_h*usrmmx;
		  yofst -= sinth*lb*str_w_by_h*usrmmy;
		  usrx[0] = (x0*costh*str_w_by_h + (y1-4)*sinth)*usrmmx + xofst;
		  usry[0] = (x0*sinth*str_w_by_h - (y1-4)*costh)*usrmmy + yofst;
		  usrx[1] = (x1*costh*str_w_by_h + (y1-4)*sinth)*usrmmx + xofst;
		  usry[1] = (x1*sinth*str_w_by_h - (y1-4)*costh)*usrmmy + yofst;
		  lines(0, usrx, usry, 2);
		  xofst += costh*rb*str_w_by_h*usrmmx;
		  yofst += sinth*rb*str_w_by_h*usrmmy;
		  nchr++;
		  bsp =0;
	       }
	    }
	    else if (chr == '|') {
	       if (!bsp) {
		  xofst -= costh*(rb-lb)*str_w_by_h*usrmmx/2.0;
		  yofst -= sinth*(rb-lb)*str_w_by_h*usrmmy/2.0;
		  bsp = 1;
	       }
	       else if (bsp == 1) {
		  xofst -= costh*(rb-lb)*str_w_by_h*usrmmx/2.0;
		  yofst -= sinth*(rb-lb)*str_w_by_h*usrmmy/2.0;
		  bsp = 2;
	       }
	       else {
		  xofst += costh*(rb-lb)*str_w_by_h*usrmmx/2.0;
		  yofst += sinth*(rb-lb)*str_w_by_h*usrmmy/2.0;
		  bsp = 1;
	       }
	    }
	    continue;
	 }
	 else if ((chr == '}') || (chr == ']')) {
	    *retx = xofst;
	    *rety = yofst;
	    return nchr;
	 }
	 else if (chr == '$') {
	    if (font==vec_font)
	       font = alt_font[vec_font];
	    else
	       font = vec_font;
	    continue;
	 }
	 else if (chr == '%') {
	    fnt = font;
	    font = mthfnt;
	    mthfnt = fnt;
	    continue;
	 }
	 else if (chr == '#') {
	    fnt = font;
	    font = sfont;
	    sfont = fnt;
	    continue;
	 }
      }
      else if (isalpha((int) chr)) {
	 lastfnt = font;
	 if (isupper((int) chr))
	    font = chr - ((unsigned int) 'A');
	 else
	    font = chr - ((unsigned int) 'a');
	 lit = 0;
	 if (font > 23)
	    font = 0;
	 else if (font < 0)
	    font = 23;
	 continue;
      }
      else if (chr == ' ') {
	 font = lastfnt;
	 lit = 0;
	 continue;
      }
      indx = hmap[hfnt[font][chr]];
      maxk = indx + vec[indx][0] + 1;
      ih = vec[indx][1];
      lb = vec[indx+1][0];
      rb = vec[indx+1][1];
      xofst -= costh*lb*str_w_by_h*usrmmx;
      yofst -= sinth*lb*str_w_by_h*usrmmy;
      pnt = 0;
      for (k = indx+2; k < maxk; ++k) {
	 if (vec[k][0] != (signed char) -50) {
	    usrx[pnt] = (vec[k][0]*costh*str_w_by_h +
			 vec[k][1]*sinth)*usrmmx + xofst;
	    usry[pnt] = (vec[k][0]*sinth*str_w_by_h -
			 vec[k][1]*costh)*usrmmy + yofst;
	    ++pnt;
	 }
	 else if (pnt > 1) {
	    lines(0, usrx, usry, pnt);
	    pnt = 0;
	 }
      }
      if (pnt > 1)
	 lines(0, usrx, usry, pnt);

      xofst += costh*rb*str_w_by_h*usrmmx;
      yofst += sinth*rb*str_w_by_h*usrmmy;
      if (bsp) {
	 if ((xofst-nxtxofst)*costheta+(yofst-nxtyofst)*sintheta > 0.0) {
	    nxtxofst = xofst;
	    nxtyofst = yofst;
	 }
	 if ((str[nchr] != '<') && (str[nchr] != '|')) {
	    xofst = nxtxofst;
	    yofst = nxtyofst;
	 }
      }
      else {
	 nxtxofst = xofst;
	 nxtyofst = yofst;
      }
      lit = 0;
      bsp = 0;
   }
   *retx = xofst;
   *rety = yofst;
   return --nchr;
}
void vec_string(float x, float y, int xjus, int yjus, float hght,
		    char *str, float theta_deg)
{
   unsigned char chr;
   int indx, ih, pnt, k=0, n, maxk, nchr, lit, first, bsp;
   int x0, y0, x1, y1, yodd, yevn, font, fnt, lastfnt, sfont, mthfnt;
   float inw, maxinw, lb0, lb, rb, curwidth;
   float costh, sinth, hw, xofst, yofst, xshft, yshft, lastx, lasty, rx, ry;
   float xps, yps, compt=sizfact*hght*72.0*1.6527/25.4;
   double theta = theta_deg * RADS_PER_DEG;

   costheta = cos(theta);
   sintheta = sin(theta);
   cos21 = costheta/21.0;
   sin21 = sintheta/21.0;
   costh = cos21*hght;
   sinth = sin21*hght;

   if (!(lps || no_x)) {
      usrmmx = xpxmm/wscalx;
      usrmmy = ypxmm/wscaly;
   }
   else if (ps_vec) {
      usrmmx = 72.0 / (25.4 * psclx);
      usrmmy = 72.0 / (25.4 * pscly);
   }
   else {
      if (ps_curpt != compt) {
      ps_curpt = compt;
      fprintf(pstrm, "%f resclfnt\n", ps_curpt);
      }
      usrmmx = 72.0 / (25.4 * psclx);
      usrmmy = 72.0 / (25.4 * pscly);
   }

   nchr = 0;
   inw = maxinw = lb = rb = 0.0;
   bsp = lit = 0;
   curwidth = 1.0;
   first = 1;
   sfont = 22;
   mthfnt = 11;
   lastfnt = font = vec_font;
   while (chr = str[nchr++]) {
      if (chr == '\n')
	 break;
      else if (!lit) {
	 if (chr == '\\') {
	    lit = 1;
	    continue;
	 }
	 else if ((chr == '{') || (chr == '[')) {
	    curwidth *= EXPFAC;
	    continue;
	 }
	 else if ((chr == '}') || (chr == ']')) {
	    curwidth /= EXPFAC;
	    continue;
	 }
	 else if (chr == '$') {
	    if (font==vec_font)
	       font = alt_font[vec_font];
	    else
	       font = vec_font;
	    continue;
	 }
	 else if (chr == '%') {
	    fnt = font;
	    font = mthfnt;
	    mthfnt = fnt;
	    continue;
	 }
	 else if (chr == '#') {
	    fnt = font;
	    font = sfont;
	    sfont = fnt;
	    continue;
	 }
	 else if (chr == '<') {
	    if (!bsp)
	       inw -= (rb-lb);
	    else if (bsp == 1)
	       inw -= (rb-lb)/2.0;
	    bsp = 2;
	    continue;
	 }
	 else if (chr == '|') {
	    if (!bsp) {
	       inw -= (rb-lb)/2.0;
	       bsp = 1;
	    }
	    else if (bsp == 1) {
	       inw -= (rb-lb)/2.0;
	       bsp = 2;
	    }
	    continue;
	 }
      }
      else if (isalpha((int) chr)) {
	 lastfnt = font;
	 if (isupper((int) chr))
	    font = chr - ((unsigned int) 'A');
	 else
	    font = chr - ((unsigned int) 'a');
	 lit = 0;
	 if (font > 23)
	    font = 0;
	 else if (font < 0)
	    font = 23;
	 continue;
      }
      else if (chr == ' ') {
	 font = lastfnt;
	 lit = 0;
	 continue;
      }
      k = hmap[hfnt[font][chr]]+1;
      lb = vec[k][0]*curwidth;
      if (first) {
	 lb0 = lb;
	 ih = vec[k-1][1];
	 first = 0;
      }
      rb = vec[k][1]*curwidth;
      inw += rb - vec[k][0]*curwidth;
      if (inw > maxinw)
	 maxinw = inw;
      lit = 0;
      bsp = 0;
   }
   if (!k || (nchr < 2))
      return;

   inw = maxinw -= (rb - lb0);

   if (xjus < 0)
      xshft = 0.0;
   else if (xjus > 0)
      xshft = lb0-inw-rb;
   else
      xshft = (lb0-inw-rb)/2.0;

   if (yjus < 0) {
      yodd = (-yjus-1) % 2;
      yevn = (-yjus-1) / 2;
      yshft = -hbsl[ih] + yodd*hcpl[ih] + yevn*hsiz[ih];
   }
   else if (yjus > 0) {
      yodd = (yjus-1) % 2;
      yevn = (yjus-1) / 2;
      yshft = -(hcpl[ih] - yodd*hbsl[ih] + yevn*hsiz[ih]);
   }
   else
      yshft = 0.0;

   xofst = x + (costh*xshft - sinth*yshft)*str_w_by_h*usrmmx;
   yofst = y + (sinth*xshft + costh*yshft)*str_w_by_h*usrmmy;

   nchr--;
   n = 0;
   while (n < nchr) {
      n += do_vec(xofst, yofst, -1, 0, hght, str+n, theta_deg, &rx, &ry);
      xofst = rx;
      yofst = ry;
   }
/*   printf("n = %d\n", n); */

   return;
}
#ifdef VMS
void vec_string_c(float *x_p, float *y_p, int *xjus_p, int *yjus_p,
		  float *hght_p, VMSstrptr str_p, float *theta_deg_p)
{
   char *str;
   init_VMStoC ;
   str = VMStoC(str_p);

   vec_string(*x_p, *y_p, *xjus_p, *yjus_p, *hght_p, str, *theta_deg_p);
   return;
}
#else
void vec_string_c_(float *x_p, float *y_p, int *xjus_p, int *yjus_p,
		   float *hght_p, char *str_p, float *theta_deg_p, int str_l)
{
   char *str;
   init_F77toC ;
   str = F77toC(str_p, str_l);

   vec_string(*x_p, *y_p, *xjus_p, *yjus_p, *hght_p, str, *theta_deg_p);
   return;
}
#endif
/* ************************************************************************** */

void curve(float (*func)(float *), float x1, float x2,
	   float xorg, float yorg, float theta)
{
   int nxt;
   float xstp, ystp, x0, y0, x, y, m, ylo, yhi, xend, yend, ylst;

   xstp = 1.0/wscalx;
   ystp = 1.0/wscaly;
   usrx[0] = x0 = x1;
   usry[0] = y0 = (*func)(&x0);
   nxt = 1;
   x = x0 + xstp;
   y = ylst = (*func)(&x);
   m = (y-y0)/xstp;
   while ((x += xstp) < x2) {
      y = (*func)(&x);
      ylo = m*(x-x0) + y0 - ystp*0.5;
      yhi = ylo + ystp;
      if (y<ylo || y>yhi) {
	 usrx[nxt] = x0 = x-xstp;
	 usry[nxt] = y0 = ylst;
	 m = (y-y0)/xstp;
	 if (nxt >= MAXPTS-1) {
	    usrx[nxt] = xend = (x0+usrx[nxt-1])/2.0;
	    usry[nxt] = yend = (*func)(&xend);
	    if (theta != 0.0) {
	       displace(usrx, usry, nxt+1, -x1, 0.0);
	       rotate(usrx, usry, nxt+1, theta);
	       if (xorg!=0.0 || yorg!=0.0)
		  displace(usrx, usry, nxt+1, xorg, yorg);
	    }
	    else if (xorg!=0.0 || yorg!=0.0) {
	       displace(usrx, usry, nxt+1, xorg, yorg);
	    }
	    lines(0, usrx, usry, nxt+1);
	    usrx[0] = xend;
	    usry[0] = yend;
	    usrx[1] = x0;
	    usry[1] = y0;
	    nxt = 1;
	 }
	 ++nxt;
      }
      ylst = y;
   }
   usrx[nxt] = x2;
   usry[nxt] = (*func)(&x2);
   if (theta != 0.0) {
      displace(usrx, usry, nxt+1, -x1, 0.0);
      rotate(usrx, usry, nxt+1, theta);
      if (xorg!=0.0 || yorg!=0.0)
	 displace(usrx, usry, nxt+1, xorg, yorg);
   }
   else if (xorg!=0.0 || yorg!=0.0) {
      displace(usrx, usry, nxt+1, xorg, yorg);
   }
   lines(0, usrx, usry, nxt+1);

   return;
}
#ifdef VMS
void curve_c(float (*func)(float *), float *x1_p, float *x2_p,
	     float *xorg_p, float *yorg_p, float *theta_p)
{
   curve(func, *x1_p, *x2_p, *xorg_p, *yorg_p, *theta_p);
   return;
}
#else
void curve_c_(float (*func)(float *), float *x1_p, float *x2_p,
	      float *xorg_p, float *yorg_p, float *theta_p)
{
   curve(func, *x1_p, *x2_p, *xorg_p, *yorg_p, *theta_p);
   return;
}
#endif
/* ************************************************************************** */

#define IABS(A) ((A) >= 0 ? (A) : -(A))
#define ISIGN(A, B) ((B) >= 0 ? IABS(A) : -IABS(A))
#define FABS(A) ((A) >= 0.0 ? (A) : -(A))
#define FSIGN(A, B) ((B) >= 0.0 ? FABS(A) : -FABS(A))

void axis(int xax,
	  float xorg, float yorg,
	  float majdlt, int majint, int minint,
	  int ndec, char *label, float size)
{
   int yax, isiz, txtlen, xjus, yjus, do_as_vec, smlint, i, j, maxlen, imajt;
   float x[3], y[3], ticsiz, hlftic, xofst, yofst, xusrmm, yusrmm, mindlt,
	 labx, laby, chrsiz;
   char text[30];

   if (!majint || majdlt==0.0)
      return;

   yax = 1 - xax;
   xusrmm = xpxmm/wscalx;
   yusrmm = ypxmm/wscaly;
   chrsiz = 1.2*FABS(size);
   ticsiz = (xax*yusrmm + yax*xusrmm)*chrsiz/1.5;
   ticsiz = ISIGN(ticsiz, majint);
   hlftic = ticsiz/2.0;
   if (ndec) {
      xjus = ISIGN(yax, ndec*majint);
      yjus = ISIGN(xax, ndec*majint);
      xofst = hlftic*(1 - ISIGN(1, ndec))*yax*1.2;
      yofst = hlftic*(1 - ISIGN(1, ndec))*xax*1.2;
   }
   else if (size != 0.0) {
      isiz = FSIGN(1, size);
      xjus = ISIGN(yax, isiz*majint);
      yjus = ISIGN(xax, isiz*majint);
      xofst = hlftic*(1 - ISIGN(1, isiz))*yax*1.2;
      yofst = hlftic*(1 - ISIGN(1, isiz))*xax*1.2;
   }
   maxlen = 0;
   x[0] = xorg - yax*ticsiz;
   y[0] = yorg - xax*ticsiz;
   x[1] = xorg;
   y[1] = yorg;
   imajt = abs(majint);
   x[2] = xorg + xax*majdlt*imajt;
   y[2] = yorg + yax*majdlt*imajt;

   lines(0, x+1, y+1, 2);

   if (label && *label) {
      if (xax) {
	 if (*label == '\t') {
	    label++;
	    do_as_vec = 0;
	 }
	 else
	    do_as_vec = 1;
      }
      else {
	 if (*label == '\t') {
	    label++;
	    if (*label == '\t') {
	       label++;
	       do_as_vec = 2;
	    }
	    else
	       do_as_vec = 0;
	 }
	 else
	    do_as_vec = 1;
      }
   }
   else
      do_as_vec = -1;

   for (i = 0; i < imajt+1; i++) {
      if (ndec && (size != 0.0)) {
	 if (abs(ndec) == 1)
	    sprintf(text, "%d", (int) (xax*x[0] + yax*y[0]));
	 else
	    sprintf(text, "%.*G", IABS(ndec)-1, xax*x[0] + yax*y[0]);
	 if (lps || no_x || do_as_vec)
	    vec_string(x[0]+xofst,y[0]+yofst,xjus,yjus,chrsiz/1.2,text,0.0);
	 else
	    string(text,x[0]+xofst,y[0]+yofst,xjus,yjus);
	 if ((txtlen = strlen(text)) > maxlen)
	    maxlen = txtlen;
      }
      lines(0, x, y, 2);
      x[0] += xax*majdlt;
      y[0] += yax*majdlt;
      x[1] += xax*majdlt;
      y[1] += yax*majdlt;
   }
   smlint = abs(minint);
   if (smlint > 1) {
      mindlt = majdlt/smlint;
      x[0] = xorg - yax*hlftic;
      y[0] = yorg - xax*hlftic;
      x[1] = xorg;
      y[1] = yorg;
      for (i = 0; i < imajt; i++) {
	 for (j = 1; j < smlint; j++) {
	    x[0] += xax*mindlt;
	    y[0] += yax*mindlt;
	    x[1] += xax*mindlt;
	    y[1] += yax*mindlt;
	    lines(0, x, y, 2);
	 }
	 x[0] += xax*mindlt;
	 y[0] += yax*mindlt;
	 x[1] += xax*mindlt;
	 y[1] += yax*mindlt;
      }
   }
   if ((do_as_vec >= 0) && *label && strcmp(label," ") && (size != 0.0)) {
      if (ndec) {
	 labx = (xorg+x[2])/2.0 + xofst -
		yax * (ticsiz + ISIGN(chrsiz*maxlen*str_w_by_h*xusrmm,xjus));
	 laby = (yorg+y[2])/2.0 + yofst -
		xax * (ticsiz + ISIGN(chrsiz*1.4*yusrmm,yjus));
      }
      else {
	 labx = (xorg+x[2])/2.0 + xofst - yax * ticsiz;
	 laby = (yorg+y[2])/2.0 + yofst - xax * ticsiz;
      }
      if (xax) {
	 if (lps || no_x || do_as_vec)
	    vec_string(labx, laby, 0, yjus, 1.5*chrsiz/1.2, label, 0.0);
	 else
	    string(label, labx, laby, 0, yjus);
      }
      else {
	 if (!do_as_vec)
	    if (lps || no_x)
	       vec_string(labx, laby, xjus, 0, 1.5*chrsiz/1.2, label, 0.0);
	    else
	       string(label, labx, laby, xjus, 0);
	 else if (do_as_vec == 1)
	    vec_string(labx, laby, 0, -1, 1.5*chrsiz/1.2, label, xjus*90.0);
	 else if (do_as_vec == 2)
	    vec_string(labx, laby, xjus, 0, 1.5*chrsiz/1.2, label, 0.0);
      }
   }

   return;
}
#ifdef VMS
void axis_c(int *xax_p, float *xorg_p, float *yorg_p,
	    float *majdlt_p, int *majint_p, int *minint_p,
	    int *ndec_p, VMSstrptr label_p, float *chrsiz_p)
{
   char *label;
   init_VMStoC ;
   label = VMStoC(label_p);

   axis(*xax_p, *xorg_p, *yorg_p, *majdlt_p, *majint_p, *minint_p, *ndec_p,
        label, *chrsiz_p);
   return;
}
#else
void axis_c_(int *xax_p, float *xorg_p, float *yorg_p,
	     float *majdlt_p, int *majint_p, int *minint_p,
	     int *ndec_p, char *label_p, float *chrsiz_p, int label_l)
{
   char *label;
   init_F77toC ;
   label = F77toC(label_p, label_l);

   axis(*xax_p, *xorg_p, *yorg_p, *majdlt_p, *majint_p, *minint_p, *ndec_p,
        label, *chrsiz_p);
   return;
}
#endif
/* ************************************************************************** */

void psclose(void)
{
   if (pstrm) {
      fprintf(pstrm, "%s\n%s\n%s\n%s%d\n",
		     "%%Trailer",
		     "end",
		     "savobj restore",
		     "%%Pages: ", npage);
      fclose(pstrm);
      pstrm = NULL;
   }
   return;
}
#ifdef VMS
void psclose_c(void)
{
   psclose();
   return;
}
#else
void psclose_c_(void)
{
   psclose();
   return;
}
#endif
/* ************************************************************************** */

#include <time.h>

int psopen(char *facility, char *filename)
{
   time_t   now, tstat;

   if (pstrm)
      psclose();
   pstrm = fopen(filename, "w");
   if (!pstrm)
      return NULL;

   fprintf(pstrm, "%s\n%s%s\n%s%s\n",
		  "%!PS-Adobe-2.0",
		  "%%Creator: ", facility,
		  "%%Title: ", filename);
   tstat = time(&now);
   if (tstat && (tstat != -1)) {
      fprintf(pstrm, "%s%s",
		     "%%CreationDate: ", ctime(&now));
   }
   fprintf(pstrm, "%s\n%s\n",
		  "%%Pages: (atend)",
		  "%%BoundingBox: 000 000  612 792");
   fprintf(pstrm, "%s\n",
		  "%%EndComments ");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/savobj save def",
		  "50 dict begin",
		  "/forgrd 0.0 def",
		  "/bckgrd 1.0 def",
		  "/vecrat 1.0 def",
		  "/asprat 1.0 def",
		  "/fntscl 14.0 def",
		  "/Symbol findfont /altfont exch def",
		  "/Courier findfont /curfont exch def",
		  "curfont fntscl scalefont setfont");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n",
		  "/setforeground",
		  "{   /forgrd exch def",
		  "    forgrd setgray",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n",
		  "/setbackground",
		  "{   /bckgrd exch def} bind def");
   fprintf(pstrm, "%s\n%s\n",
		  "/setasprat",
		  "{   /asprat exch def} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n",
		  "/resclfnt",
		  "{   dup fntscl div",
		  "    currentfont exch scalefont setfont",
		  "    /fntscl exch def",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n",
		  "/xchfnt",
		  "{   altfont fntscl scalefont setfont",
		  "    curfont altfont /curfont exch def /altfont exch def",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n",
		  "/loadfnt",
		  "{   findfont /curfont exch def",
		  "    curfont fntscl scalefont setfont",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/boxfill",
		  "{ 2 copy 5 2 roll",
		  "  newpath moveto",
		  "  sub dup 4 1 roll 0 exch",
		  "  neg rlineto sub 0",
		  "  rlineto 0 exch rlineto",
		  "  closepath bckgrd setgray fill forgrd setgray",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/bndbox",
		  "{ gsave currentpoint translate",
		  "  newpath 0 0 moveto dup",
		  "  false charpath flattenpath pathbbox",
		  "  boxfill",
		  "  grestore",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/jushow",
		  "{   moveto",
		  "    0.5 add fntscl mul -2 div",
		  "    3 1 roll 1 add exch dup 3 1 roll",
		  "    stringwidth pop vecrat mul -2 div mul 3 -1 roll",
		  "    rmoveto gsave vecrat 1.0 scale show grestore",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/juishow",
		  "{   moveto",
		  "    0.5 add fntscl mul -2 div",
		  "    3 1 roll 1 add exch dup 3 1 roll",
		  "    stringwidth pop -2 div mul 3 -1 roll",
		  "    rmoveto bndbox show",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/jurshow",
		  "{   gsave",
		  "    /vecrat asprat def",
		  "    translate rotate 0 0 jushow",
		  "    /vecrat 1.0 def",
		  "    grestore",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/points",
		  "{   gsave",
		  "    1 setlinewidth 1 setlinecap newpath",
		  "    { 2 copy moveto lineto } repeat",
		  "    stroke",
		  "    grestore",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/polyline",
		  "{ gsave",
		  "  newpath",
		  "  3 1 roll moveto 1 sub { lineto } repeat stroke",
		  "  grestore",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/polyfill",
		  "{ gsave",
		  "  newpath",
		  "  3 1 roll moveto 1 sub { lineto } repeat closepath eofill",
		  "  grestore",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/polylinefill",
		  "{ gsave",
		  "  newpath",
		  "  3 1 roll moveto 1 sub { lineto } repeat",
		  "  closepath gsave eofill grestore stroke",
		  "  grestore",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n",
		  "/segments",
		  "{ gsave",
		  "  newpath { moveto lineto } repeat stroke",
		  "  grestore",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n",
		  "/recpath",
		  "{ newpath",
		  "  moveto dup 0 exch neg rlineto exch 0",
		  "  rlineto 0 exch rlineto closepath",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/rectangles",
		  "{ gsave { recpath stroke } repeat grestore } bind def",
		  "/fillrectangles",
		  "{ gsave { recpath fill } repeat grestore } bind def",
		  "/drawfillrectangles",
		  "{ gsave",
		  "  { recpath gsave fill grestore stroke } repeat grestore",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/arcpath",
		  "{ newpath 2 copy eq",
		  "  { pop 2 div 3 1 roll arc false }",
		  "  { dup 2 div 0 0 3 -1 roll 7 3 roll div 1 9 -2 roll",
		  "    gsave translate scale arc true",
		  "  } ifelse",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n",
		  "/arcs",
		  "{ gsave",
		  "  { arcpath stroke { grestore } if } repeat",
		  "  grestore",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n",
		  "/fillarcs",
		  "{ gsave",
		  "  { arcpath fill { grestore } if } repeat",
		  "  grestore",
		  "} bind def");
   fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n%s\n",
		  "/drawfillarcs",
		  "{ gsave",
		  "  { arcpath gsave fill grestore stroke",
		  "    { grestore } if } repeat",
		  "  grestore",
		  "} bind def");
   fprintf(pstrm, "%s\n",
		  "%%EndProlog");

   return 1;
}
#ifdef VMS
int psopen_c(VMSstrptr facility_p, VMSstrptr filename_p)
{
   char *facility, *filename;
   init_VMStoC ;
   facility = VMStoC(facility_p);
   filename = VMStoC(filename_p);

   return psopen(facility, filename);
}
#else
int psopen_c_(char *facility_p, char *filename_p,
	      int facility_l, int filename_l)
{
   char *facility, *filename;
   init_F77toC ;
   facility = F77toC(facility_p, facility_l);
   filename = F77toC(filename_p, filename_l);

   return psopen(facility, filename);
}
#endif
/* ************************************************************************** */

void next_page(void)
{
   if (pstrm) {
      ++npage;
      fprintf(pstrm, "showpage\n");
   }
   return;
}
#ifdef VMS
void next_page_c(void)
{
   next_page();
   return;
}
#else
void next_page_c_(void)
{
   next_page();
   return;
}
#endif
/* ************************************************************************** */

void start_clip(void)
{
   if (pstrm) {
      fprintf(pstrm, "%s\n%s\n%s\n%s\n%s\n",
		     "20.0 110.0 translate",
		     "0.0 setlinewidth",
		     "14.0 resclfnt",
		     "newpath 0 0 moveto 572 0 rlineto",
		     "0 572 rlineto -572 0 rlineto closepath clip newpath");
      ps_curpt = 14.0;
   }
   return;
}
#ifdef VMS
void start_clip_c(void)
{
   start_clip();
   return;
}
#else
void start_clip_c_(void)
{
   start_clip();
   return;
}
#endif
/* ************************************************************************** */

int set_ps(int lval)
{
   int old_lps;

   old_lps = lps;
   lps = lval;
   return old_lps;
}
#ifdef VMS
int set_ps_c(int *lval_p)
{
   return set_ps(*lval_p);
}
#else
int set_ps_c_(int *lval_p)
{
   return set_ps(*lval_p);
}
#endif
/* ************************************************************************** */

int set_x(int lval)
{
   int old_x;

   old_x = !no_x;
   if (!lval && old_x)
      pltcls();
   lps = no_x = !lval;
   return old_x;
}
#ifdef VMS
int set_x_c(int *lval_p)
{
   return set_x(*lval_p);
}
#else
int set_x_c_(int *lval_p)
{
   return set_x(*lval_p);
}
#endif
/* ************************************************************************** */
