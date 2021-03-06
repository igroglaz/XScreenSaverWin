/* xscreensaver, Copyright (c) 1992, 1995, 1996, 1998, 2000
 *  Jamie Zawinski <jwz@jwz.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 *
 * This code derived from TI Explorer Lisp code by Joe Keane, Fritz Mueller,
 * and Jamie Zawinski.
 */

#include "screenhack.h"
#include <math.h>

#define __STDC__

int observer_z_ = 3;
int delay_ = 20000;
float xy_ = 3;
float xz_ = 5;
float yw_ = 10;
float yz_ = 0;
float xw_ = 0;
float zw_ = 0;
char *background = "black";
char *foreground = "white";

static argtype vars[] = 
{
    {&observer_z_, "observer_z", NULL, "3", t_Int},
    {&delay_, "delay", NULL, "20000", t_Int},
    {&xy_, "xy", NULL, "3", t_Float},
    {&xz_, "xz", NULL, "5", t_Float},
    {&yw_, "yw", NULL, "10", t_Float},
    {&yz_, "yz", NULL, "0", t_Float},
    {&xw_, "xw", NULL, "0", t_Float},
    {&zw_, "zw", NULL, "0", t_Float},
    {&background, "background", NULL, "black", t_String},
    {&foreground, "foreground", NULL, "white", t_String},
};

char *colors[][8] = {
	{
		"#FF66BE", /* color00 */
		"#FFA300", /* color10 */
		"#BEDC00", /* color20 */
		"#12FB00", /* color30 */
		"#00F9BE", /* color40 */
		"#12D5FF", /* color50 */
		"#BE9AFF", /* color60 */
		"#FF5FFF", /* color70 */
	}, {
		"#FF5BAA", /* color01 */
		"#F09200", /* color11 */
		"#AAC500", /* color21 */
		"#10E100", /* color31 */
		"#00DFAA", /* color41 */
		"#10BFFF", /* color51 */
		"#AA8AFF", /* color61 */
		"#F055FF", /* color71 */
	}, {
		"#EE529A", /* color02 */
		"#D98400", /* color12 */
		"#9AB200", /* color22 */
		"#0ECB00", /* color32 */
		"#00C99A", /* color42 */
		"#0EADE7", /* color52 */
		"#9A7DFF", /* color62 */
		"#D94DE7", /* color72 */
	}, {
		"#DA4B8C", /* color03 */
		"#C67900", /* color13 */
		"#8CA300", /* color23 */
		"#0DBA00", /* color33 */
		"#00B88C", /* color43 */
		"#0D9ED3", /* color53 */
		"#8C72EA", /* color63 */
		"#C646D3", /* color73 */
	}, {
		"#C84581", /* color04 */
		"#B66F00", /* color14 */
		"#819600", /* color24 */
		"#0CAB00", /* color34 */
		"#00A981", /* color44 */
		"#0C91C2", /* color54 */
		"#8169D7", /* color64 */
		"#B641C2", /* color74 */
	}, {
		"#B94078", /* color05 */
		"#A96700", /* color15 */
		"#788B00", /* color25 */
		"#0B9E00", /* color35 */
		"#009D78", /* color45 */
		"#0B86B3", /* color55 */
		"#7861C7", /* color65 */
		"#A93CB3", /* color75 */
	}, {
		"#AC3C6F", /* color06 */
		"#9D6000", /* color16 */
		"#6F8100", /* color26 */
		"#0A9300", /* color36 */
		"#00926F", /* color46 */
		"#0A7DA7", /* color56 */
		"#6F5AB9", /* color66 */
		"#9D38A7", /* color76 */
	}, {
		"#A13868", /* color07 */
		"#935900", /* color17 */
		"#687900", /* color27 */
		"#0A8A00", /* color37 */
		"#008868", /* color47 */
		"#0A759C", /* color57 */
		"#6854AD", /* color67 */
		"#93349C", /* color77 */
	}
};

#define POINT_COUNT 600
#define LINE_COUNT 1200

#define ANGLE_SCALE 0.001

struct point_info
{
  float pg_a;
  float pg_b;
  float pg_c;
  float pg_d;
};

struct line_info
{
  short li_ip;
  short li_iq;
  char li_color;
  char li_pad[3];
};

struct point_state
{
  short old_x, old_y;
  short new_x, new_y;
  unsigned char old_dep, new_dep;
};

struct hyper_state
{
  char hs_stop;
  char hs_resize;
  char hs_redraw;
  char hs_icon;
  Display *hs_display;
  Window hs_window;
  float hs_observer_z;
  float hs_unit_scale;
  float hs_offset_x;
  float hs_offset_y;
  int hs_delay;
  double hs_angle_xy;
  double hs_angle_xz;
  double hs_angle_yz;
  double hs_angle_xw;
  double hs_angle_yw;
  double hs_angle_zw;
  double hs_cos_xy, hs_sin_xy;
  double hs_cos_xz, hs_sin_xz;
  double hs_cos_yz, hs_sin_yz;
  double hs_cos_xw, hs_sin_xw;
  double hs_cos_yw, hs_sin_yw;
  double hs_cos_zw, hs_sin_zw;
  double hs_ref_ax, hs_ref_ay, hs_ref_az, hs_ref_aw;
  double hs_ref_bx, hs_ref_by, hs_ref_bz, hs_ref_bw;
  double hs_ref_cx, hs_ref_cy, hs_ref_cz, hs_ref_cw;
  double hs_ref_dx, hs_ref_dy, hs_ref_dz, hs_ref_dw;
  GC hs_color_gcs[8][8];
  GC black_gc;
  char hs_moved[POINT_COUNT];
  struct point_state hs_points[POINT_COUNT];
  int roted;
};

static const struct point_info point_table[POINT_COUNT];
static const struct line_info line_table[LINE_COUNT];

static void set_sizes (struct hyper_state *hs, int width, int height);

static void *
hyperball_init (Display *dpy, Window win)
{
  struct hyper_state *hs = (struct hyper_state *) calloc (1, sizeof(*hs));

  XGCValues gcv;
  Colormap cmap;
  /* double xy, xz, yz, xw, yw, zw; */
  unsigned long bg_pixel;
  float observer_z;
  int delay;

  hs->hs_display = dpy;
  hs->hs_window = win;

  //observer_z = get_float_resource (dpy, "observer-z", "Float");
  observer_z = observer_z_;
  if (observer_z < 1.125)
    observer_z = 1.125;
  hs->hs_observer_z = observer_z;

  {
    XWindowAttributes wa;
    XGetWindowAttributes (dpy, win, &wa);
    cmap = wa.colormap;
    set_sizes (hs, wa.width, wa.height);
  }

#if 1
  hs->hs_angle_xy = xy_ * ANGLE_SCALE;
  hs->hs_angle_xz = xz_ * ANGLE_SCALE;
  hs->hs_angle_yz = yz_ * ANGLE_SCALE;
  hs->hs_angle_xw = xw_ * ANGLE_SCALE;
  hs->hs_angle_yw = yw_ * ANGLE_SCALE;
  hs->hs_angle_zw = zw_ * ANGLE_SCALE;
#else
  hs->hs_angle_xy = get_float_resource (dpy, "xy", "Float") * ANGLE_SCALE;
  hs->hs_angle_xz = get_float_resource (dpy, "xz", "Float") * ANGLE_SCALE;
  hs->hs_angle_yz = get_float_resource (dpy, "yz", "Float") * ANGLE_SCALE;
  hs->hs_angle_xw = get_float_resource (dpy, "xw", "Float") * ANGLE_SCALE;
  hs->hs_angle_yw = get_float_resource (dpy, "yw", "Float") * ANGLE_SCALE;
  hs->hs_angle_zw = get_float_resource (dpy, "zw", "Float") * ANGLE_SCALE;
#endif

  //delay = get_integer_resource (dpy, "delay", "Integer");
  delay = delay_;
  hs->hs_delay = delay;

  //bg_pixel = get_pixel_resource (dpy, cmap, "background", "Background");
  bg_pixel = load_color(dpy, cmap, background);

  if (mono_p)
    {
      GC black_gc;
      unsigned long fg_pixel;
      GC white_gc;

      gcv.function = GXcopy;
      gcv.foreground = bg_pixel;
      black_gc = XCreateGC (dpy, win, GCForeground|GCFunction, &gcv);
      //fg_pixel = get_pixel_resource (dpy, cmap, "foreground", "Foreground");
      fg_pixel = load_color(dpy, cmap, foreground);
      gcv.foreground = fg_pixel;
      white_gc = XCreateGC (dpy, win, GCForeground|GCFunction, &gcv);
      hs->hs_color_gcs[0][0] = black_gc;
      hs->hs_color_gcs[0][1] = white_gc;
    }
  else
    {
      int col;
      int dep;

      gcv.function = GXcopy;

      //gcv.foreground = get_pixel_resource (dpy, cmap,
      //                                     "background", "Background");
      gcv.foreground = load_color(dpy, cmap, background);
      hs->black_gc = XCreateGC (dpy, win, GCForeground|GCFunction, &gcv);

      for (col = 0; col < 8; col++)
	for (dep = 0; dep < 8; dep++)
	  {
	    char buffer[16];
	    unsigned long fg_pixel;
	    GC color_gc;

	    sprintf (buffer, "color%d%d", col, dep);
	    //fg_pixel = get_pixel_resource (dpy, cmap, buffer, "Foreground");
	    fg_pixel = load_color(dpy, cmap, colors[dep][col]);
	    gcv.foreground = fg_pixel /*^ bg_pixel*/;
	    color_gc = XCreateGC (dpy, win, GCForeground|GCFunction, &gcv);
	    hs->hs_color_gcs[col][dep] = color_gc;
	  }
    }

  {
  double xy;
  double xz;
  double yz;
  double xw;
  double yw;
  double zw;
#if 1
  double cos_xy, sin_xy;
  double cos_xz, sin_xz;
  double cos_yz, sin_yz;
  double cos_xw, sin_xw;
  double cos_yw, sin_yw;
  double cos_zw, sin_zw;
#endif

  hs->hs_ref_ax = 1.0, hs->hs_ref_ay = 0.0, hs->hs_ref_az = 0.0, hs->hs_ref_aw = 0.0;
  hs->hs_ref_bx = 0.0, hs->hs_ref_by = 1.0, hs->hs_ref_bz = 0.0, hs->hs_ref_bw = 0.0;
  hs->hs_ref_cx = 0.0, hs->hs_ref_cy = 0.0, hs->hs_ref_cz = 1.0, hs->hs_ref_cw = 0.0;
  hs->hs_ref_dx = 0.0, hs->hs_ref_dy = 0.0, hs->hs_ref_dz = 0.0, hs->hs_ref_dw = 1.0;

  xy = hs->hs_angle_xy;
  xz = hs->hs_angle_xz;
  yz = hs->hs_angle_yz;
  xw = hs->hs_angle_xw;
  yw = hs->hs_angle_yw;
  zw = hs->hs_angle_zw;

  cos_xy = cos (xy), sin_xy = sin (xy);
  hs->hs_cos_xy = cos_xy, hs->hs_sin_xy = sin_xy;
  cos_xz = cos (xz), sin_xz = sin (xz);
  hs->hs_cos_xz = cos_xz, hs->hs_sin_xz = sin_xz;
  cos_yz = cos (yz), sin_yz = sin (yz);
  hs->hs_cos_yz = cos_yz, hs->hs_sin_yz = sin_yz;
  cos_xw = cos (xw), sin_xw = sin (xw);
  hs->hs_cos_xw = cos_xw, hs->hs_sin_xw = sin_xw;
  cos_yw = cos (yw), sin_yw = sin (yw);
  hs->hs_cos_yw = cos_yw, hs->hs_sin_yw = sin_yw;
  cos_zw = cos (zw), sin_zw = sin (zw);
  hs->hs_cos_zw = cos_zw, hs->hs_sin_zw = sin_zw;
  }

  return hs;
}


static unsigned long
hyperball_draw (Display *dpy, Window window, void *closure)
{
  struct hyper_state *hs = (struct hyper_state *) closure;

  int icon;
  int resize;
  int redraw;
  int stop;
  int delay;

  icon = hs->hs_icon;
  resize = hs->hs_resize;
  if (icon || !(hs->roted | resize))
    goto skip1;

#ifdef HAVE_COCOA	/* Don't second-guess Quartz's double-buffering */
  XClearWindow (dpy, window);
#endif

  {
    int pc;
    const struct point_info *point_ptr;
    struct point_state *point_state;
    float observer_z;
    float unit_scale;
    float offset_x;
    float offset_y;
    char *mark_ptr;

    pc = POINT_COUNT;
    point_ptr = &point_table[0];
    point_state = &hs->hs_points[0];
    mark_ptr = &hs->hs_moved[0];

    while (--pc >= 0)
      {
        double pos_a;
        double pos_b;
        double pos_c;
        double pos_d;
        double az, bz, cz, dz;
        double sum_z;
        double ax, bx, cx, dx;
        double sum_x;
        double ay, by, cy, dy;
        double sum_y;
        double mul;
        int old_x;
        int old_y;
        int old_dep;
        double xf;
        double yf;
        int new_x;
        int new_y;
        int new_dep;
        int mov;

        pos_a = point_ptr->pg_a;
        pos_b = point_ptr->pg_b;
        pos_c = point_ptr->pg_c;
        pos_d = point_ptr->pg_d;
        point_ptr++;
        az = hs->hs_ref_az; bz = hs->hs_ref_bz; cz = hs->hs_ref_cz; dz = hs->hs_ref_dz;
        ax = hs->hs_ref_ax; bx = hs->hs_ref_bx; cx = hs->hs_ref_cx; dx = hs->hs_ref_dx;
        ay = hs->hs_ref_ay; by = hs->hs_ref_by; cy = hs->hs_ref_cy; dy = hs->hs_ref_dy;
        sum_z = pos_a * az + pos_b * bz + pos_c * cz + pos_d * dz;
        observer_z = hs->hs_observer_z;
        unit_scale = hs->hs_unit_scale;
        sum_x = pos_a * ax + pos_b * bx + pos_c * cx + pos_d * dx;
        sum_y = pos_a * ay + pos_b * by + pos_c * cy + pos_d * dy;
        mul = unit_scale / (observer_z - sum_z);
        offset_x = hs->hs_offset_x;
        offset_y = hs->hs_offset_y;
        old_x = point_state->new_x;
        old_y = point_state->new_y;
        old_dep = point_state->new_dep;
        xf = sum_x * mul + offset_x;
        yf = sum_y * mul + offset_y;
        new_x = (int)rint(xf);
        new_y = (int)rint(yf);
        new_dep = (int)floor(sum_z * -128.0) + 128;
        point_state->old_x = old_x;
        point_state->old_y = old_y;
        point_state->old_dep = old_dep;
        point_state->new_x = new_x;
        point_state->new_y = new_y;
        point_state->new_dep = new_dep;
        point_state++;
        mov = new_x != old_x || new_y != old_y || new_dep != old_dep;
        *mark_ptr = mov;
        mark_ptr++;
      }
  }

 skip1:
  icon = hs->hs_icon;
  redraw = hs->hs_redraw;
  if (icon || !(hs->roted | redraw))
    goto skip2;

  {
    int lc;
    const struct line_info *li_ptr;
    int mono;
    Window win = hs->hs_window;

    lc = LINE_COUNT;
    li_ptr = &line_table[0];
    mono = mono_p;

    while (--lc >= 0)
      {
        int ip;
        int iq;
        int col;
        int mov_p;
        int mov_q;
        struct point_state *sp;
        struct point_state *sq;
        int p_x;
        int p_y;
        int q_x;
        int q_y;
        GC erase_gc;
        GC draw_gc;
        int old_sum;
        int new_sum;
        int old_dep;
        int new_dep;

        ip = li_ptr->li_ip;
        iq = li_ptr->li_iq;
        col = li_ptr->li_color;
        li_ptr++;
        mov_p = hs->hs_moved[ip];
        mov_q = hs->hs_moved[iq];
        if (!(redraw | mov_p | mov_q))
          continue;

        sp = &hs->hs_points[ip];
        sq = &hs->hs_points[iq];

        if (mono)
          {
            erase_gc = hs->hs_color_gcs[0][0];
            draw_gc = hs->hs_color_gcs[0][1];
          }
        else
          {
            GC *row;

            old_sum = sp->old_dep + sq->old_dep;
            new_sum = sp->new_dep + sq->new_dep;
            row = &hs->hs_color_gcs[col][0];
            old_dep = old_sum >> 6;
            new_dep = new_sum >> 6;
            erase_gc = hs->black_gc;
            draw_gc = row[new_dep];
          }

        if (!redraw && erase_gc)
          {
            p_x = sp->old_x;
            p_y = sp->old_y;
            q_x = sq->old_x;
            q_y = sq->old_y;
            XDrawLine (dpy, win, erase_gc, p_x, p_y, q_x, q_y);
          }

        p_x = sp->new_x;
        p_y = sp->new_y;
        q_x = sq->new_x;
        q_y = sq->new_y;
        XDrawLine (dpy, win, draw_gc, p_x, p_y, q_x, q_y);
      }
  }

 skip2:
  stop = hs->hs_stop;
  hs->roted = 0;
  if (stop)
    goto skip3;

  hs->roted = 1;

  {
    double cos_a;
    double sin_a;
    double old_u;
    double old_v;
    double new_u;
    double new_v;

    /* If you get error messages about the following forms, and you think you're
       using an ANSI C conforming compiler, then you're mistaken.  Possibly you're
       mixing an ANSI compiler with a non-ANSI preprocessor, or vice versa.
       Regardless, your system is broken; it's not a bug in this program.
    */
#if defined(__STDC__) || defined(__ANSI_CPP__)

#define rotate(name,dim0,dim1)                  \
    old_u = hs->hs_ref_##name##dim0;            \
    old_v = hs->hs_ref_##name##dim1;            \
    new_u = old_u * cos_a + old_v * sin_a;      \
    new_v = old_v * cos_a - old_u * sin_a;      \
    hs->hs_ref_##name##dim0 = new_u;            \
    hs->hs_ref_##name##dim1 = new_v;

#define rotates(dim0,dim1)                      \
    if (hs->hs_sin_##dim0##dim1 != 0) {         \
      cos_a = hs->hs_cos_##dim0##dim1;          \
      sin_a = hs->hs_sin_##dim0##dim1;          \
      rotate(a, dim0, dim1);                    \
      rotate(b, dim0, dim1);                    \
      rotate(c, dim0, dim1);                    \
      rotate(d, dim0, dim1);                    \
    }

#else /* !__STDC__, courtesy of Andreas Luik <luik@isa.de> */

#define rotate(name,dim0,dim1,cos,sin)          \
    old_u = hs->hs_ref_/**/name/**/dim0;        \
    old_v = hs->hs_ref_/**/name/**/dim1;        \
    new_u = old_u * cos_a + old_v * sin_a;      \
    new_v = old_v * cos_a - old_u * sin_a;      \
    hs->hs_ref_/**/name/**/dim0 = new_u;        \
    hs->hs_ref_/**/name/**/dim1 = new_v;

#define rotates(dim0,dim1)                      \
    if (hs->hs_sin_/**/dim0/**/dim1 != 0) {     \
      cos_a = hs->hs_cos_/**/dim0/**/dim1;      \
      sin_a = hs->hs_sin_/**/dim0/**/dim1;      \
      rotate(a, dim0, dim1);                    \
      rotate(b, dim0, dim1);                    \
      rotate(c, dim0, dim1);                    \
      rotate(d, dim0, dim1);                    \
    }

#endif

    rotates (x, y);
    rotates (x, z);
    rotates (y, z);
    rotates (x, w);
    rotates (y, w);
    rotates (z, w);
  }

 skip3:
  /* stop = hs->hs_stop; */
  delay = hs->hs_delay;
  if (stop && delay < 10000)
    delay = 10000;

  hs->hs_redraw = 0;
  hs->hs_resize = 0;

  return delay;
}


#if 0
	static Bool
	hyperball_event (Display *dpy, Window win, void *closure, XEvent *event)
	{
	  struct hyper_state *hs = (struct hyper_state *) closure;

	  hs->hs_redraw = 0;

	  switch (event->type)
	    {
	    case Expose:
	      hs->hs_icon = 0;
	      hs->hs_redraw = 1;
	      break;

	    case ButtonPress:
	      switch (event->xbutton.button)
	        {
	        case 2:
	          hs->hs_stop = !hs->hs_stop;
	          break;
	        default:
	          break;
	        }
	      break;

	#ifndef HAVE_COCOA
	    case UnmapNotify:
	      hs->hs_icon = 1;
	      hs->hs_redraw = 0;
	      break;
	#endif

	    default:
	      break;
	    }

	  if (hs->hs_redraw)
	    XClearWindow (dpy, win);

	  return False;
	}
#endif

static void
hyperball_reshape (Display *dpy, Window window, void *closure, 
                 unsigned int w, unsigned int h)
{
  struct hyper_state *hs = (struct hyper_state *) closure;
  hs->hs_icon = 0;
  hs->hs_resize = 1;
  hs->hs_redraw = 1;
  set_sizes (hs, w, h);
  XClearWindow (dpy, window);
}


static void
set_sizes (struct hyper_state *hs, int width, int height)
{
  double observer_z;
  int min_dim;
  double var;
  double offset_x;
  double offset_y;
  double unit_scale;

  observer_z = hs->hs_observer_z;
  min_dim = width < height ? width : height;
  var = sqrt(observer_z * observer_z - 1.0);
  offset_x = 0.5 * (width - 1);
  offset_y = 0.5 * (height - 1);
  unit_scale = 0.4 * min_dim * var;
  hs->hs_offset_x = (float)offset_x;
  hs->hs_offset_y = (float)offset_y;
  hs->hs_unit_scale = (float)unit_scale;
}

static void
hyperball_free (Display *dpy, Window window, void *closure)
{
  struct hyper_state *hs = (struct hyper_state *) closure;
  free (hs);
}

/* data */

static const struct point_info point_table[POINT_COUNT] =
{
    { 0.93, 0.30, 0.09, 0.03, },
    { 0.84, 0.49, -0.08, 0.05, },
    { 0.72, 0.65, 0.02, -0.09, },
    { 0.57, 0.79, 0.05, 0.07, },
    { 0.40, 0.89, -0.09, -0.01, },
    { 0.20, 0.95, 0.07, -0.06, },
    { 0.00, 0.97, 0.00, 0.09, },
    { -0.20, 0.95, -0.07, -0.06, },
    { -0.40, 0.89, 0.09, -0.01, },
    { -0.57, 0.79, -0.05, 0.07, },
    { -0.72, 0.65, -0.02, -0.09, },
    { -0.84, 0.49, 0.08, 0.05, },
    { -0.93, 0.30, -0.09, 0.03, },
    { -0.97, 0.10, 0.04, -0.08, },
    { -0.97, -0.10, 0.04, 0.08, },
    { -0.93, -0.30, -0.09, -0.03, },
    { -0.84, -0.49, 0.08, -0.05, },
    { -0.72, -0.65, -0.02, 0.09, },
    { -0.57, -0.79, -0.05, -0.07, },
    { -0.40, -0.89, 0.09, 0.01, },
    { -0.20, -0.95, -0.07, 0.06, },
    { 0.00, -0.97, 0.00, -0.09, },
    { 0.20, -0.95, 0.07, 0.06, },
    { 0.40, -0.89, -0.09, 0.01, },
    { 0.57, -0.79, 0.05, -0.07, },
    { 0.72, -0.65, 0.02, 0.09, },
    { 0.84, -0.49, -0.08, -0.05, },
    { 0.93, -0.30, 0.09, -0.03, },
    { 0.97, -0.10, -0.04, 0.08, },
    { 0.97, 0.10, -0.04, -0.08, },
    { 0.27, 0.83, -0.13, -0.41, },
    { 0.09, 0.87, 0.40, 0.18, },
    { -0.09, 0.87, -0.40, 0.18, },
    { -0.27, 0.83, 0.13, -0.41, },
    { -0.44, 0.76, 0.22, 0.38, },
    { -0.59, 0.65, -0.42, -0.09, },
    { -0.71, 0.51, 0.35, -0.25, },
    { -0.80, 0.36, -0.05, 0.43, },
    { -0.86, 0.18, -0.29, -0.32, },
    { -0.88, 0.00, 0.43, 0.00, },
    { -0.86, -0.18, -0.29, 0.32, },
    { -0.80, -0.36, -0.05, -0.43, },
    { -0.71, -0.51, 0.35, 0.25, },
    { -0.59, -0.65, -0.42, 0.09, },
    { -0.44, -0.76, 0.22, -0.38, },
    { -0.27, -0.83, 0.13, 0.41, },
    { -0.09, -0.87, -0.40, -0.18, },
    { 0.09, -0.87, 0.40, -0.18, },
    { 0.27, -0.83, -0.13, 0.41, },
    { 0.44, -0.76, -0.22, -0.38, },
    { 0.59, -0.65, 0.42, 0.09, },
    { 0.71, -0.51, -0.35, 0.25, },
    { 0.80, -0.36, 0.05, -0.43, },
    { 0.86, -0.18, 0.29, 0.32, },
    { 0.88, 0.00, -0.43, 0.00, },
    { 0.86, 0.18, 0.29, -0.32, },
    { 0.80, 0.36, 0.05, 0.43, },
    { 0.71, 0.51, -0.35, -0.25, },
    { 0.59, 0.65, 0.42, -0.09, },
    { 0.44, 0.76, -0.22, 0.38, },
    { -0.13, 0.41, -0.27, 0.83, },
    { -0.22, 0.38, -0.44, -0.76, },
    { -0.29, 0.32, 0.86, 0.18, },
    { -0.35, 0.25, -0.71, 0.51, },
    { -0.40, 0.18, 0.09, -0.87, },
    { -0.42, 0.09, 0.59, 0.65, },
    { -0.43, 0.00, -0.88, 0.00, },
    { -0.42, -0.09, 0.59, -0.65, },
    { -0.40, -0.18, 0.09, 0.87, },
    { -0.35, -0.25, -0.71, -0.51, },
    { -0.29, -0.32, 0.86, -0.18, },
    { -0.22, -0.38, -0.44, 0.76, },
    { -0.13, -0.41, -0.27, -0.83, },
    { -0.05, -0.43, 0.80, 0.36, },
    { 0.05, -0.43, -0.80, 0.36, },
    { 0.13, -0.41, 0.27, -0.83, },
    { 0.22, -0.38, 0.44, 0.76, },
    { 0.29, -0.32, -0.86, -0.18, },
    { 0.35, -0.25, 0.71, -0.51, },
    { 0.40, -0.18, -0.09, 0.87, },
    { 0.42, -0.09, -0.59, -0.65, },
    { 0.43, 0.00, 0.88, 0.00, },
    { 0.42, 0.09, -0.59, 0.65, },
    { 0.40, 0.18, -0.09, -0.87, },
    { 0.35, 0.25, 0.71, 0.51, },
    { 0.29, 0.32, -0.86, 0.18, },
    { 0.22, 0.38, 0.44, -0.76, },
    { 0.13, 0.41, 0.27, 0.83, },
    { 0.05, 0.43, -0.80, -0.36, },
    { -0.05, 0.43, 0.80, -0.36, },
    { 0.09, -0.03, -0.93, 0.30, },
    { 0.09, -0.01, 0.40, -0.89, },
    { 0.09, 0.01, 0.40, 0.89, },
    { 0.09, 0.03, -0.93, -0.30, },
    { 0.08, 0.05, 0.84, -0.49, },
    { 0.07, 0.06, -0.20, 0.95, },
    { 0.05, 0.07, -0.57, -0.79, },
    { 0.04, 0.08, 0.97, 0.10, },
    { 0.02, 0.09, -0.72, 0.65, },
    { 0.00, 0.09, 0.00, -0.97, },
    { -0.02, 0.09, 0.72, 0.65, },
    { -0.04, 0.08, -0.97, 0.10, },
    { -0.05, 0.07, 0.57, -0.79, },
    { -0.07, 0.06, 0.20, 0.95, },
    { -0.08, 0.05, -0.84, -0.49, },
    { -0.09, 0.03, 0.93, -0.30, },
    { -0.09, 0.01, -0.40, 0.89, },
    { -0.09, -0.01, -0.40, -0.89, },
    { -0.09, -0.03, 0.93, 0.30, },
    { -0.08, -0.05, -0.84, 0.49, },
    { -0.07, -0.06, 0.20, -0.95, },
    { -0.05, -0.07, 0.57, 0.79, },
    { -0.04, -0.08, -0.97, -0.10, },
    { -0.02, -0.09, 0.72, -0.65, },
    { 0.00, -0.09, 0.00, 0.97, },
    { 0.02, -0.09, -0.72, -0.65, },
    { 0.04, -0.08, 0.97, -0.10, },
    { 0.05, -0.07, -0.57, 0.79, },
    { 0.07, -0.06, -0.20, -0.95, },
    { 0.08, -0.05, 0.84, 0.49, },
    { 0.64, 0.67, -0.15, -0.28, },
    { 0.49, 0.79, 0.30, 0.07, },
    { 0.31, 0.87, -0.26, 0.18, },
    { 0.13, 0.92, 0.04, -0.31, },
    { -0.07, 0.92, 0.20, 0.24, },
    { -0.26, 0.89, -0.31, -0.01, },
    { -0.44, 0.82, 0.22, -0.23, },
    { -0.60, 0.71, 0.02, 0.31, },
    { -0.73, 0.57, -0.25, -0.19, },
    { -0.83, 0.40, 0.31, -0.06, },
    { -0.90, 0.22, -0.16, 0.27, },
    { -0.93, 0.03, -0.09, -0.30, },
    { -0.91, -0.16, 0.28, 0.14, },
    { -0.86, -0.35, -0.29, 0.12, },
    { -0.77, -0.52, 0.11, -0.29, },
    { -0.64, -0.67, 0.15, 0.28, },
    { -0.49, -0.79, -0.30, -0.07, },
    { -0.31, -0.87, 0.26, -0.18, },
    { -0.13, -0.92, -0.04, 0.31, },
    { 0.07, -0.92, -0.20, -0.24, },
    { 0.26, -0.89, 0.31, 0.01, },
    { 0.44, -0.82, -0.22, 0.23, },
    { 0.60, -0.71, -0.02, -0.31, },
    { 0.73, -0.57, 0.25, 0.19, },
    { 0.83, -0.40, -0.31, 0.06, },
    { 0.90, -0.22, 0.16, -0.27, },
    { 0.93, -0.03, 0.09, 0.30, },
    { 0.91, 0.16, -0.28, -0.14, },
    { 0.86, 0.35, 0.29, -0.12, },
    { 0.77, 0.52, -0.11, 0.29, },
    { 0.44, 0.82, -0.22, -0.23, },
    { 0.26, 0.89, 0.31, -0.01, },
    { 0.07, 0.92, -0.20, 0.24, },
    { -0.12, 0.92, -0.04, -0.31, },
    { -0.31, 0.87, 0.26, 0.18, },
    { -0.49, 0.79, -0.30, 0.07, },
    { -0.64, 0.67, 0.15, -0.28, },
    { -0.77, 0.52, 0.11, 0.29, },
    { -0.86, 0.35, -0.29, -0.12, },
    { -0.91, 0.16, 0.28, -0.14, },
    { -0.93, -0.03, -0.09, 0.30, },
    { -0.90, -0.22, -0.16, -0.27, },
    { -0.83, -0.40, 0.31, 0.06, },
    { -0.73, -0.57, -0.25, 0.19, },
    { -0.60, -0.71, 0.02, -0.31, },
    { -0.44, -0.82, 0.22, 0.23, },
    { -0.26, -0.89, -0.31, 0.01, },
    { -0.07, -0.92, 0.20, -0.24, },
    { 0.12, -0.92, 0.04, 0.31, },
    { 0.31, -0.87, -0.26, -0.18, },
    { 0.49, -0.79, 0.30, -0.07, },
    { 0.64, -0.67, -0.15, 0.28, },
    { 0.77, -0.52, -0.11, -0.29, },
    { 0.86, -0.35, 0.29, 0.12, },
    { 0.91, -0.16, -0.28, 0.14, },
    { 0.93, 0.03, 0.09, -0.30, },
    { 0.90, 0.22, 0.16, 0.27, },
    { 0.83, 0.40, -0.31, -0.06, },
    { 0.73, 0.57, 0.25, -0.19, },
    { 0.60, 0.71, -0.02, 0.31, },
    { -0.13, 0.83, -0.27, -0.41, },
    { -0.30, 0.79, 0.49, 0.07, },
    { -0.46, 0.71, -0.38, 0.31, },
    { -0.60, 0.60, 0.02, -0.49, },
    { -0.71, 0.46, 0.35, 0.35, },
    { -0.79, 0.30, -0.49, 0.03, },
    { -0.83, 0.13, 0.31, -0.38, },
    { -0.84, -0.05, 0.08, 0.49, },
    { -0.81, -0.22, -0.41, -0.27, },
    { -0.75, -0.38, 0.48, -0.13, },
    { -0.65, -0.53, -0.22, 0.44, },
    { -0.53, -0.66, -0.18, -0.46, },
    { -0.38, -0.75, 0.46, 0.17, },
    { -0.22, -0.82, -0.44, 0.23, },
    { -0.04, -0.84, 0.12, -0.48, },
    { 0.13, -0.83, 0.27, 0.41, },
    { 0.30, -0.79, -0.49, -0.07, },
    { 0.46, -0.71, 0.38, -0.31, },
    { 0.60, -0.60, -0.02, 0.49, },
    { 0.71, -0.46, -0.35, -0.35, },
    { 0.79, -0.30, 0.49, -0.03, },
    { 0.83, -0.13, -0.31, 0.38, },
    { 0.84, 0.05, -0.08, -0.49, },
    { 0.81, 0.22, 0.41, 0.27, },
    { 0.75, 0.38, -0.48, 0.13, },
    { 0.65, 0.53, 0.22, -0.44, },
    { 0.53, 0.66, 0.18, 0.46, },
    { 0.38, 0.75, -0.46, -0.17, },
    { 0.22, 0.82, 0.44, -0.23, },
    { 0.04, 0.84, -0.12, 0.48, },
    { -0.65, 0.53, -0.22, -0.44, },
    { -0.75, 0.38, 0.48, 0.13, },
    { -0.81, 0.22, -0.41, 0.27, },
    { -0.84, 0.05, 0.08, -0.49, },
    { -0.83, -0.13, 0.31, 0.38, },
    { -0.79, -0.30, -0.49, -0.03, },
    { -0.71, -0.46, 0.35, -0.35, },
    { -0.60, -0.60, 0.02, 0.49, },
    { -0.46, -0.71, -0.38, -0.31, },
    { -0.30, -0.79, 0.49, -0.07, },
    { -0.13, -0.83, -0.27, 0.41, },
    { 0.04, -0.84, -0.13, -0.48, },
    { 0.22, -0.82, 0.44, 0.23, },
    { 0.38, -0.75, -0.46, 0.17, },
    { 0.53, -0.66, 0.18, -0.46, },
    { 0.65, -0.53, 0.22, 0.44, },
    { 0.75, -0.38, -0.48, -0.13, },
    { 0.81, -0.22, 0.41, -0.27, },
    { 0.84, -0.05, -0.08, 0.49, },
    { 0.83, 0.13, -0.31, -0.38, },
    { 0.79, 0.30, 0.49, 0.03, },
    { 0.71, 0.46, -0.35, 0.35, },
    { 0.60, 0.60, -0.02, -0.49, },
    { 0.46, 0.71, 0.38, 0.31, },
    { 0.30, 0.79, -0.49, 0.07, },
    { 0.13, 0.83, 0.27, -0.41, },
    { -0.04, 0.84, 0.13, 0.48, },
    { -0.22, 0.82, -0.44, -0.23, },
    { -0.38, 0.75, 0.46, -0.17, },
    { -0.53, 0.66, -0.18, 0.46, },
    { 0.11, 0.78, -0.33, -0.47, },
    { -0.05, 0.79, 0.57, 0.07, },
    { -0.22, 0.76, -0.44, 0.38, },
    { -0.37, 0.70, 0.01, -0.58, },
    { -0.51, 0.60, 0.42, 0.40, },
    { -0.62, 0.49, -0.57, 0.05, },
    { -0.71, 0.35, 0.35, -0.46, },
    { -0.77, 0.19, 0.11, 0.57, },
    { -0.79, 0.03, -0.49, -0.30, },
    { -0.78, -0.14, 0.55, -0.16, },
    { -0.73, -0.29, -0.25, 0.52, },
    { -0.65, -0.44, -0.22, -0.53, },
    { -0.55, -0.57, 0.54, 0.19, },
    { -0.42, -0.67, -0.51, 0.28, },
    { -0.27, -0.74, 0.13, -0.56, },
    { -0.11, -0.78, 0.33, 0.47, },
    { 0.05, -0.79, -0.57, -0.07, },
    { 0.22, -0.76, 0.44, -0.38, },
    { 0.37, -0.70, -0.01, 0.58, },
    { 0.51, -0.60, -0.42, -0.40, },
    { 0.62, -0.49, 0.57, -0.05, },
    { 0.71, -0.35, -0.35, 0.46, },
    { 0.77, -0.19, -0.11, -0.57, },
    { 0.79, -0.03, 0.49, 0.30, },
    { 0.78, 0.14, -0.55, 0.16, },
    { 0.73, 0.29, 0.25, -0.52, },
    { 0.65, 0.44, 0.22, 0.53, },
    { 0.55, 0.57, -0.54, -0.19, },
    { 0.42, 0.67, 0.51, -0.28, },
    { 0.27, 0.74, -0.13, 0.56, },
    { -0.73, 0.29, -0.25, -0.52, },
    { -0.78, 0.14, 0.55, 0.16, },
    { -0.79, -0.03, -0.49, 0.30, },
    { -0.77, -0.19, 0.11, -0.57, },
    { -0.71, -0.35, 0.35, 0.46, },
    { -0.62, -0.49, -0.57, -0.05, },
    { -0.51, -0.60, 0.42, -0.40, },
    { -0.37, -0.70, 0.01, 0.58, },
    { -0.22, -0.76, -0.44, -0.38, },
    { -0.05, -0.79, 0.57, -0.07, },
    { 0.11, -0.78, -0.33, 0.47, },
    { 0.27, -0.74, -0.13, -0.56, },
    { 0.42, -0.67, 0.51, 0.28, },
    { 0.55, -0.57, -0.54, 0.19, },
    { 0.65, -0.44, 0.22, -0.53, },
    { 0.73, -0.29, 0.25, 0.52, },
    { 0.78, -0.14, -0.55, -0.16, },
    { 0.79, 0.03, 0.49, -0.30, },
    { 0.77, 0.19, -0.11, 0.57, },
    { 0.71, 0.35, -0.35, -0.46, },
    { 0.62, 0.49, 0.57, 0.05, },
    { 0.51, 0.60, -0.42, 0.40, },
    { 0.37, 0.70, -0.01, -0.58, },
    { 0.22, 0.76, 0.44, 0.38, },
    { 0.05, 0.79, -0.57, 0.07, },
    { -0.11, 0.78, 0.33, -0.47, },
    { -0.27, 0.74, 0.13, 0.56, },
    { -0.42, 0.67, -0.51, -0.28, },
    { -0.55, 0.57, 0.54, -0.19, },
    { -0.65, 0.44, -0.22, 0.53, },
    { -0.28, 0.70, -0.24, -0.58, },
    { -0.42, 0.62, 0.59, 0.21, },
    { -0.54, 0.52, -0.55, 0.29, },
    { -0.64, 0.40, 0.15, -0.60, },
    { -0.71, 0.25, 0.35, 0.51, },
    { -0.75, 0.10, -0.62, -0.08, },
    { -0.75, -0.06, 0.48, -0.40, },
    { -0.72, -0.21, -0.02, 0.62, },
    { -0.66, -0.36, -0.45, -0.43, },
    { -0.57, -0.49, 0.62, -0.05, },
    { -0.46, -0.60, -0.38, 0.49, },
    { -0.33, -0.68, -0.11, -0.61, },
    { -0.18, -0.73, 0.53, 0.33, },
    { -0.02, -0.75, -0.60, 0.17, },
    { 0.13, -0.74, 0.27, -0.56, },
    { 0.28, -0.70, 0.24, 0.58, },
    { 0.42, -0.62, -0.59, -0.21, },
    { 0.54, -0.52, 0.55, -0.29, },
    { 0.64, -0.40, -0.15, 0.60, },
    { 0.71, -0.25, -0.35, -0.51, },
    { 0.75, -0.10, 0.62, 0.08, },
    { 0.75, 0.06, -0.48, 0.40, },
    { 0.72, 0.21, 0.02, -0.62, },
    { 0.66, 0.36, 0.45, 0.43, },
    { 0.57, 0.49, -0.62, 0.05, },
    { 0.46, 0.60, 0.38, -0.49, },
    { 0.33, 0.68, 0.11, 0.61, },
    { 0.18, 0.73, -0.53, -0.33, },
    { 0.02, 0.75, 0.60, -0.17, },
    { -0.13, 0.74, -0.27, 0.56, },
    { -0.46, 0.60, -0.38, -0.49, },
    { -0.57, 0.49, 0.62, 0.05, },
    { -0.66, 0.36, -0.45, 0.43, },
    { -0.72, 0.21, -0.02, -0.62, },
    { -0.75, 0.06, 0.48, 0.40, },
    { -0.75, -0.10, -0.62, 0.08, },
    { -0.71, -0.25, 0.35, -0.51, },
    { -0.64, -0.40, 0.15, 0.60, },
    { -0.54, -0.52, -0.55, -0.29, },
    { -0.42, -0.62, 0.59, -0.21, },
    { -0.28, -0.70, -0.24, 0.58, },
    { -0.13, -0.74, -0.27, -0.56, },
    { 0.02, -0.75, 0.60, 0.17, },
    { 0.18, -0.73, -0.53, 0.33, },
    { 0.33, -0.68, 0.11, -0.61, },
    { 0.46, -0.60, 0.38, 0.49, },
    { 0.57, -0.49, -0.62, -0.05, },
    { 0.66, -0.36, 0.45, -0.43, },
    { 0.72, -0.21, 0.02, 0.62, },
    { 0.75, -0.06, -0.48, -0.40, },
    { 0.75, 0.10, 0.62, -0.08, },
    { 0.71, 0.25, -0.35, 0.51, },
    { 0.64, 0.40, -0.15, -0.60, },
    { 0.54, 0.52, 0.55, 0.29, },
    { 0.42, 0.62, -0.59, 0.21, },
    { 0.28, 0.70, 0.24, -0.58, },
    { 0.13, 0.74, 0.27, 0.56, },
    { -0.02, 0.75, -0.60, -0.17, },
    { -0.18, 0.73, 0.53, -0.33, },
    { -0.33, 0.68, -0.11, 0.61, },
    { -0.24, 0.58, 0.28, 0.70, },
    { -0.35, 0.51, -0.71, -0.25, },
    { -0.45, 0.43, 0.66, -0.36, },
    { -0.53, 0.33, -0.18, 0.73, },
    { -0.59, 0.21, -0.42, -0.62, },
    { -0.62, 0.08, 0.75, 0.10, },
    { -0.62, -0.05, -0.57, 0.49, },
    { -0.60, -0.17, 0.02, -0.75, },
    { -0.55, -0.29, 0.54, 0.52, },
    { -0.48, -0.40, -0.75, 0.06, },
    { -0.38, -0.49, 0.46, -0.60, },
    { -0.27, -0.56, 0.13, 0.74, },
    { -0.15, -0.60, -0.64, -0.40, },
    { -0.02, -0.62, 0.72, -0.21, },
    { 0.11, -0.61, -0.33, 0.68, },
    { 0.24, -0.58, -0.29, -0.70, },
    { 0.35, -0.51, 0.71, 0.25, },
    { 0.45, -0.43, -0.66, 0.36, },
    { 0.53, -0.33, 0.18, -0.73, },
    { 0.59, -0.21, 0.42, 0.62, },
    { 0.62, -0.08, -0.75, -0.10, },
    { 0.62, 0.05, 0.57, -0.49, },
    { 0.60, 0.17, -0.02, 0.75, },
    { 0.55, 0.29, -0.54, -0.52, },
    { 0.48, 0.40, 0.75, -0.06, },
    { 0.38, 0.49, -0.46, 0.60, },
    { 0.27, 0.56, -0.13, -0.74, },
    { 0.15, 0.60, 0.64, 0.40, },
    { 0.02, 0.62, -0.72, 0.21, },
    { -0.11, 0.61, 0.33, -0.68, },
    { -0.38, 0.49, 0.46, 0.60, },
    { -0.48, 0.40, -0.75, -0.06, },
    { -0.55, 0.29, 0.54, -0.52, },
    { -0.60, 0.17, 0.02, 0.75, },
    { -0.62, 0.05, -0.57, -0.49, },
    { -0.62, -0.08, 0.75, -0.10, },
    { -0.59, -0.21, -0.42, 0.62, },
    { -0.53, -0.33, -0.18, -0.73, },
    { -0.45, -0.43, 0.66, 0.36, },
    { -0.35, -0.51, -0.71, 0.25, },
    { -0.24, -0.58, 0.28, -0.70, },
    { -0.11, -0.61, 0.33, 0.68, },
    { 0.02, -0.62, -0.72, -0.21, },
    { 0.15, -0.60, 0.64, -0.40, },
    { 0.27, -0.56, -0.13, 0.74, },
    { 0.38, -0.49, -0.46, -0.60, },
    { 0.48, -0.40, 0.75, 0.06, },
    { 0.55, -0.29, -0.54, 0.52, },
    { 0.60, -0.17, -0.02, -0.75, },
    { 0.62, -0.05, 0.57, 0.49, },
    { 0.62, 0.08, -0.75, 0.10, },
    { 0.59, 0.21, 0.42, -0.62, },
    { 0.53, 0.33, 0.18, 0.73, },
    { 0.45, 0.43, -0.66, -0.36, },
    { 0.35, 0.51, 0.71, -0.25, },
    { 0.24, 0.58, -0.28, 0.70, },
    { 0.11, 0.61, -0.33, -0.68, },
    { -0.02, 0.62, 0.72, 0.21, },
    { -0.15, 0.60, -0.64, 0.40, },
    { -0.27, 0.56, 0.13, -0.74, },
    { -0.33, 0.47, -0.11, 0.78, },
    { -0.42, 0.40, -0.51, -0.60, },
    { -0.49, 0.30, 0.79, 0.03, },
    { -0.54, 0.19, -0.55, 0.57, },
    { -0.57, 0.07, -0.05, -0.79, },
    { -0.57, -0.05, 0.62, 0.49, },
    { -0.55, -0.16, -0.78, 0.14, },
    { -0.51, -0.28, 0.42, -0.67, },
    { -0.44, -0.38, 0.22, 0.76, },
    { -0.35, -0.46, -0.71, -0.35, },
    { -0.25, -0.52, 0.73, -0.29, },
    { -0.13, -0.56, -0.27, 0.74, },
    { -0.01, -0.58, -0.37, -0.70, },
    { 0.11, -0.57, 0.77, 0.19, },
    { 0.22, -0.53, -0.65, 0.44, },
    { 0.33, -0.47, 0.11, -0.78, },
    { 0.42, -0.40, 0.51, 0.60, },
    { 0.49, -0.30, -0.79, -0.03, },
    { 0.54, -0.19, 0.55, -0.57, },
    { 0.57, -0.07, 0.05, 0.79, },
    { 0.57, 0.05, -0.62, -0.49, },
    { 0.55, 0.16, 0.78, -0.14, },
    { 0.51, 0.28, -0.42, 0.67, },
    { 0.44, 0.38, -0.22, -0.76, },
    { 0.35, 0.46, 0.71, 0.35, },
    { 0.25, 0.52, -0.73, 0.29, },
    { 0.13, 0.56, 0.27, -0.74, },
    { 0.01, 0.58, 0.37, 0.70, },
    { -0.11, 0.57, -0.77, -0.19, },
    { -0.22, 0.53, 0.65, -0.44, },
    { -0.25, 0.52, 0.73, 0.29, },
    { -0.35, 0.46, -0.71, 0.35, },
    { -0.44, 0.38, 0.22, -0.76, },
    { -0.51, 0.28, 0.42, 0.67, },
    { -0.55, 0.16, -0.78, -0.14, },
    { -0.57, 0.05, 0.62, -0.49, },
    { -0.57, -0.07, -0.05, 0.79, },
    { -0.54, -0.19, -0.55, -0.57, },
    { -0.49, -0.30, 0.79, -0.03, },
    { -0.42, -0.40, -0.51, 0.60, },
    { -0.33, -0.47, -0.11, -0.78, },
    { -0.22, -0.53, 0.65, 0.44, },
    { -0.11, -0.57, -0.77, 0.19, },
    { 0.01, -0.58, 0.37, -0.70, },
    { 0.13, -0.56, 0.27, 0.74, },
    { 0.25, -0.52, -0.73, -0.29, },
    { 0.35, -0.46, 0.71, -0.35, },
    { 0.44, -0.38, -0.22, 0.76, },
    { 0.51, -0.28, -0.42, -0.67, },
    { 0.55, -0.16, 0.78, 0.14, },
    { 0.57, -0.05, -0.62, 0.49, },
    { 0.57, 0.07, 0.05, -0.79, },
    { 0.54, 0.19, 0.55, 0.57, },
    { 0.49, 0.30, -0.79, 0.03, },
    { 0.42, 0.40, 0.51, -0.60, },
    { 0.33, 0.47, 0.11, 0.78, },
    { 0.22, 0.53, -0.65, -0.44, },
    { 0.11, 0.57, 0.77, -0.19, },
    { -0.01, 0.58, -0.37, 0.70, },
    { -0.13, 0.56, -0.27, -0.74, },
    { -0.27, 0.41, 0.13, 0.83, },
    { -0.35, 0.35, -0.71, -0.46, },
    { -0.41, 0.27, 0.81, -0.22, },
    { -0.46, 0.17, -0.38, 0.75, },
    { -0.49, 0.07, -0.30, -0.79, },
    { -0.49, -0.03, 0.79, 0.30, },
    { -0.48, -0.13, -0.75, 0.38, },
    { -0.44, -0.23, 0.22, -0.82, },
    { -0.38, -0.31, 0.46, 0.71, },
    { -0.31, -0.38, -0.83, -0.13, },
    { -0.22, -0.44, 0.65, -0.53, },
    { -0.13, -0.48, -0.04, 0.84, },
    { -0.02, -0.49, -0.60, -0.60, },
    { 0.08, -0.49, 0.84, -0.05, },
    { 0.18, -0.46, -0.53, 0.66, },
    { 0.27, -0.41, -0.13, -0.83, },
    { 0.35, -0.35, 0.71, 0.46, },
    { 0.41, -0.27, -0.81, 0.22, },
    { 0.46, -0.17, 0.38, -0.75, },
    { 0.49, -0.07, 0.30, 0.79, },
    { 0.49, 0.03, -0.79, -0.30, },
    { 0.48, 0.13, 0.75, -0.38, },
    { 0.44, 0.23, -0.22, 0.82, },
    { 0.38, 0.31, -0.46, -0.71, },
    { 0.31, 0.38, 0.83, 0.13, },
    { 0.22, 0.44, -0.65, 0.53, },
    { 0.12, 0.48, 0.04, -0.84, },
    { 0.02, 0.49, 0.60, 0.60, },
    { -0.08, 0.49, -0.84, 0.05, },
    { -0.18, 0.46, 0.53, -0.66, },
    { -0.22, 0.44, 0.65, 0.53, },
    { -0.31, 0.38, -0.83, 0.13, },
    { -0.38, 0.31, 0.46, -0.71, },
    { -0.44, 0.23, 0.22, 0.82, },
    { -0.48, 0.13, -0.75, -0.38, },
    { -0.49, 0.03, 0.79, -0.30, },
    { -0.49, -0.07, -0.30, 0.79, },
    { -0.46, -0.17, -0.38, -0.75, },
    { -0.41, -0.27, 0.81, 0.22, },
    { -0.35, -0.35, -0.71, 0.46, },
    { -0.27, -0.41, 0.13, -0.83, },
    { -0.18, -0.46, 0.53, 0.66, },
    { -0.08, -0.49, -0.84, -0.05, },
    { 0.02, -0.49, 0.60, -0.60, },
    { 0.12, -0.48, 0.04, 0.84, },
    { 0.22, -0.44, -0.65, -0.53, },
    { 0.31, -0.38, 0.83, -0.13, },
    { 0.38, -0.31, -0.46, 0.71, },
    { 0.44, -0.23, -0.22, -0.82, },
    { 0.48, -0.13, 0.75, 0.38, },
    { 0.49, -0.03, -0.79, 0.30, },
    { 0.49, 0.07, 0.30, -0.79, },
    { 0.46, 0.17, 0.38, 0.75, },
    { 0.41, 0.27, -0.81, -0.22, },
    { 0.35, 0.35, 0.71, -0.46, },
    { 0.27, 0.41, -0.13, 0.83, },
    { 0.18, 0.46, -0.53, -0.66, },
    { 0.08, 0.49, 0.84, 0.05, },
    { -0.02, 0.49, -0.60, 0.60, },
    { -0.12, 0.48, -0.04, -0.84, },
    { -0.15, 0.28, -0.64, 0.67, },
    { -0.20, 0.24, -0.07, -0.92, },
    { -0.25, 0.19, 0.73, 0.57, },
    { -0.28, 0.14, -0.91, 0.16, },
    { -0.30, 0.07, 0.49, -0.79, },
    { -0.31, 0.01, 0.26, 0.89, },
    { -0.31, -0.06, -0.83, -0.40, },
    { -0.29, -0.12, 0.86, -0.35, },
    { -0.26, -0.18, -0.31, 0.87, },
    { -0.22, -0.23, -0.44, -0.82, },
    { -0.16, -0.27, 0.90, 0.22, },
    { -0.11, -0.29, -0.77, 0.52, },
    { -0.04, -0.31, 0.12, -0.92, },
    { 0.02, -0.31, 0.60, 0.71, },
    { 0.09, -0.30, -0.93, -0.03, },
    { 0.15, -0.28, 0.64, -0.67, },
    { 0.20, -0.24, 0.07, 0.92, },
    { 0.25, -0.19, -0.73, -0.57, },
    { 0.28, -0.14, 0.91, -0.16, },
    { 0.30, -0.07, -0.49, 0.79, },
    { 0.31, -0.01, -0.26, -0.89, },
    { 0.31, 0.06, 0.83, 0.40, },
    { 0.29, 0.12, -0.86, 0.35, },
    { 0.26, 0.18, 0.31, -0.87, },
    { 0.22, 0.23, 0.44, 0.82, },
    { 0.16, 0.27, -0.90, -0.22, },
    { 0.11, 0.29, 0.77, -0.52, },
    { 0.04, 0.31, -0.12, 0.92, },
    { -0.02, 0.31, -0.60, -0.71, },
    { -0.09, 0.30, 0.93, 0.03, },
    { -0.22, 0.23, -0.44, 0.82, },
    { -0.26, 0.18, -0.31, -0.87, },
    { -0.29, 0.12, 0.86, 0.35, },
    { -0.31, 0.06, -0.83, 0.40, },
    { -0.31, -0.01, 0.26, -0.89, },
    { -0.30, -0.07, 0.49, 0.79, },
    { -0.28, -0.14, -0.91, -0.16, },
    { -0.25, -0.19, 0.73, -0.57, },
    { -0.20, -0.24, -0.07, 0.92, },
    { -0.15, -0.28, -0.64, -0.67, },
    { -0.09, -0.30, 0.93, -0.03, },
    { -0.02, -0.31, -0.60, 0.71, },
    { 0.04, -0.31, -0.13, -0.92, },
    { 0.11, -0.29, 0.77, 0.52, },
    { 0.16, -0.27, -0.90, 0.22, },
    { 0.22, -0.23, 0.44, -0.82, },
    { 0.26, -0.18, 0.31, 0.87, },
    { 0.29, -0.12, -0.86, -0.35, },
    { 0.31, -0.06, 0.83, -0.40, },
    { 0.31, 0.01, -0.26, 0.89, },
    { 0.30, 0.07, -0.49, -0.79, },
    { 0.28, 0.14, 0.91, 0.16, },
    { 0.25, 0.19, -0.73, 0.57, },
    { 0.20, 0.24, 0.07, -0.92, },
    { 0.15, 0.28, 0.64, 0.67, },
    { 0.09, 0.30, -0.93, 0.03, },
    { 0.02, 0.31, 0.60, -0.71, },
    { -0.04, 0.31, 0.13, 0.92, },
    { -0.11, 0.29, -0.77, -0.52, },
    { -0.16, 0.27, 0.90, -0.22, },
};

static const struct line_info line_table[LINE_COUNT] =
{
    { 0, 1, 0, },
    { 0, 29, 0, },
    { 0, 148, 0, },
    { 0, 176, 0, },
    { 1, 2, 0, },
    { 1, 149, 0, },
    { 1, 177, 0, },
    { 2, 3, 0, },
    { 2, 120, 0, },
    { 2, 178, 0, },
    { 3, 4, 0, },
    { 3, 121, 0, },
    { 3, 179, 0, },
    { 4, 5, 0, },
    { 4, 122, 0, },
    { 4, 150, 0, },
    { 5, 6, 0, },
    { 5, 123, 0, },
    { 5, 151, 0, },
    { 6, 7, 0, },
    { 6, 124, 0, },
    { 6, 152, 0, },
    { 7, 8, 0, },
    { 7, 125, 0, },
    { 7, 153, 0, },
    { 8, 9, 0, },
    { 8, 126, 0, },
    { 8, 154, 0, },
    { 9, 10, 0, },
    { 9, 127, 0, },
    { 9, 155, 0, },
    { 10, 11, 0, },
    { 10, 128, 0, },
    { 10, 156, 0, },
    { 11, 12, 0, },
    { 11, 129, 0, },
    { 11, 157, 0, },
    { 12, 13, 0, },
    { 12, 130, 0, },
    { 12, 158, 0, },
    { 13, 14, 0, },
    { 13, 131, 0, },
    { 13, 159, 0, },
    { 14, 15, 0, },
    { 14, 132, 0, },
    { 14, 160, 0, },
    { 15, 16, 0, },
    { 15, 133, 0, },
    { 15, 161, 0, },
    { 16, 17, 0, },
    { 16, 134, 0, },
    { 16, 162, 0, },
    { 17, 18, 0, },
    { 17, 135, 0, },
    { 17, 163, 0, },
    { 18, 19, 0, },
    { 18, 136, 0, },
    { 18, 164, 0, },
    { 19, 20, 0, },
    { 19, 137, 0, },
    { 19, 165, 0, },
    { 20, 21, 0, },
    { 20, 138, 0, },
    { 20, 166, 0, },
    { 21, 22, 0, },
    { 21, 139, 0, },
    { 21, 167, 0, },
    { 22, 23, 0, },
    { 22, 140, 0, },
    { 22, 168, 0, },
    { 23, 24, 0, },
    { 23, 141, 0, },
    { 23, 169, 0, },
    { 24, 25, 0, },
    { 24, 142, 0, },
    { 24, 170, 0, },
    { 25, 26, 0, },
    { 25, 143, 0, },
    { 25, 171, 0, },
    { 26, 27, 0, },
    { 26, 144, 0, },
    { 26, 172, 0, },
    { 27, 28, 0, },
    { 27, 145, 0, },
    { 27, 173, 0, },
    { 28, 29, 0, },
    { 28, 146, 0, },
    { 28, 174, 0, },
    { 29, 147, 0, },
    { 29, 175, 0, },
    { 30, 123, 0, },
    { 30, 150, 0, },
    { 30, 240, 0, },
    { 30, 292, 0, },
    { 31, 124, 0, },
    { 31, 151, 0, },
    { 31, 241, 0, },
    { 31, 293, 0, },
    { 32, 125, 0, },
    { 32, 152, 0, },
    { 32, 242, 0, },
    { 32, 294, 0, },
    { 33, 126, 0, },
    { 33, 153, 0, },
    { 33, 243, 0, },
    { 33, 295, 0, },
    { 34, 127, 0, },
    { 34, 154, 0, },
    { 34, 244, 0, },
    { 34, 296, 0, },
    { 35, 128, 0, },
    { 35, 155, 0, },
    { 35, 245, 0, },
    { 35, 297, 0, },
    { 36, 129, 0, },
    { 36, 156, 0, },
    { 36, 246, 0, },
    { 36, 298, 0, },
    { 37, 130, 0, },
    { 37, 157, 0, },
    { 37, 247, 0, },
    { 37, 299, 0, },
    { 38, 131, 0, },
    { 38, 158, 0, },
    { 38, 248, 0, },
    { 38, 270, 0, },
    { 39, 132, 0, },
    { 39, 159, 0, },
    { 39, 249, 0, },
    { 39, 271, 0, },
    { 40, 133, 0, },
    { 40, 160, 0, },
    { 40, 250, 0, },
    { 40, 272, 0, },
    { 41, 134, 0, },
    { 41, 161, 0, },
    { 41, 251, 0, },
    { 41, 273, 0, },
    { 42, 135, 0, },
    { 42, 162, 0, },
    { 42, 252, 0, },
    { 42, 274, 0, },
    { 43, 136, 0, },
    { 43, 163, 0, },
    { 43, 253, 0, },
    { 43, 275, 0, },
    { 44, 137, 0, },
    { 44, 164, 0, },
    { 44, 254, 0, },
    { 44, 276, 0, },
    { 45, 138, 1, },
    { 45, 165, 1, },
    { 45, 255, 1, },
    { 45, 277, 1, },
    { 46, 139, 1, },
    { 46, 166, 1, },
    { 46, 256, 1, },
    { 46, 278, 1, },
    { 47, 140, 1, },
    { 47, 167, 1, },
    { 47, 257, 1, },
    { 47, 279, 1, },
    { 48, 141, 1, },
    { 48, 168, 1, },
    { 48, 258, 1, },
    { 48, 280, 1, },
    { 49, 142, 1, },
    { 49, 169, 1, },
    { 49, 259, 1, },
    { 49, 281, 1, },
    { 50, 143, 1, },
    { 50, 170, 1, },
    { 50, 260, 1, },
    { 50, 282, 1, },
    { 51, 144, 1, },
    { 51, 171, 1, },
    { 51, 261, 1, },
    { 51, 283, 1, },
    { 52, 145, 1, },
    { 52, 172, 1, },
    { 52, 262, 1, },
    { 52, 284, 1, },
    { 53, 146, 1, },
    { 53, 173, 1, },
    { 53, 263, 1, },
    { 53, 285, 1, },
    { 54, 147, 1, },
    { 54, 174, 1, },
    { 54, 264, 1, },
    { 54, 286, 1, },
    { 55, 148, 1, },
    { 55, 175, 1, },
    { 55, 265, 1, },
    { 55, 287, 1, },
    { 56, 149, 1, },
    { 56, 176, 1, },
    { 56, 266, 1, },
    { 56, 288, 1, },
    { 57, 120, 1, },
    { 57, 177, 1, },
    { 57, 267, 1, },
    { 57, 289, 1, },
    { 58, 121, 1, },
    { 58, 178, 1, },
    { 58, 268, 1, },
    { 58, 290, 1, },
    { 59, 122, 1, },
    { 59, 179, 1, },
    { 59, 269, 1, },
    { 59, 291, 1, },
    { 60, 420, 1, },
    { 60, 478, 1, },
    { 60, 567, 1, },
    { 60, 570, 1, },
    { 61, 421, 1, },
    { 61, 479, 1, },
    { 61, 568, 1, },
    { 61, 571, 1, },
    { 62, 422, 1, },
    { 62, 450, 1, },
    { 62, 569, 1, },
    { 62, 572, 1, },
    { 63, 423, 1, },
    { 63, 451, 1, },
    { 63, 540, 1, },
    { 63, 573, 1, },
    { 64, 424, 1, },
    { 64, 452, 1, },
    { 64, 541, 1, },
    { 64, 574, 1, },
    { 65, 425, 1, },
    { 65, 453, 1, },
    { 65, 542, 1, },
    { 65, 575, 1, },
    { 66, 426, 1, },
    { 66, 454, 1, },
    { 66, 543, 1, },
    { 66, 576, 1, },
    { 67, 427, 1, },
    { 67, 455, 1, },
    { 67, 544, 1, },
    { 67, 577, 1, },
    { 68, 428, 1, },
    { 68, 456, 1, },
    { 68, 545, 1, },
    { 68, 578, 1, },
    { 69, 429, 1, },
    { 69, 457, 1, },
    { 69, 546, 1, },
    { 69, 579, 1, },
    { 70, 430, 1, },
    { 70, 458, 1, },
    { 70, 547, 1, },
    { 70, 580, 1, },
    { 71, 431, 1, },
    { 71, 459, 1, },
    { 71, 548, 1, },
    { 71, 581, 1, },
    { 72, 432, 1, },
    { 72, 460, 1, },
    { 72, 549, 1, },
    { 72, 582, 1, },
    { 73, 433, 1, },
    { 73, 461, 1, },
    { 73, 550, 1, },
    { 73, 583, 1, },
    { 74, 434, 1, },
    { 74, 462, 1, },
    { 74, 551, 1, },
    { 74, 584, 1, },
    { 75, 435, 1, },
    { 75, 463, 1, },
    { 75, 552, 1, },
    { 75, 585, 1, },
    { 76, 436, 1, },
    { 76, 464, 1, },
    { 76, 553, 1, },
    { 76, 586, 1, },
    { 77, 437, 1, },
    { 77, 465, 1, },
    { 77, 554, 1, },
    { 77, 587, 1, },
    { 78, 438, 1, },
    { 78, 466, 1, },
    { 78, 555, 1, },
    { 78, 588, 1, },
    { 79, 439, 1, },
    { 79, 467, 1, },
    { 79, 556, 1, },
    { 79, 589, 1, },
    { 80, 440, 1, },
    { 80, 468, 1, },
    { 80, 557, 1, },
    { 80, 590, 1, },
    { 81, 441, 1, },
    { 81, 469, 1, },
    { 81, 558, 1, },
    { 81, 591, 1, },
    { 82, 442, 1, },
    { 82, 470, 1, },
    { 82, 559, 2, },
    { 82, 592, 2, },
    { 83, 443, 2, },
    { 83, 471, 2, },
    { 83, 560, 2, },
    { 83, 593, 2, },
    { 84, 444, 2, },
    { 84, 472, 2, },
    { 84, 561, 2, },
    { 84, 594, 2, },
    { 85, 445, 2, },
    { 85, 473, 2, },
    { 85, 562, 2, },
    { 85, 595, 2, },
    { 86, 446, 2, },
    { 86, 474, 2, },
    { 86, 563, 2, },
    { 86, 596, 2, },
    { 87, 447, 2, },
    { 87, 475, 2, },
    { 87, 564, 2, },
    { 87, 597, 2, },
    { 88, 448, 2, },
    { 88, 476, 2, },
    { 88, 565, 2, },
    { 88, 598, 2, },
    { 89, 449, 2, },
    { 89, 477, 2, },
    { 89, 566, 2, },
    { 89, 599, 2, },
    { 90, 101, 2, },
    { 90, 109, 2, },
    { 90, 562, 2, },
    { 90, 584, 2, },
    { 91, 102, 2, },
    { 91, 110, 2, },
    { 91, 563, 2, },
    { 91, 585, 2, },
    { 92, 103, 2, },
    { 92, 111, 2, },
    { 92, 564, 2, },
    { 92, 586, 2, },
    { 93, 104, 2, },
    { 93, 112, 2, },
    { 93, 565, 2, },
    { 93, 587, 2, },
    { 94, 105, 2, },
    { 94, 113, 2, },
    { 94, 566, 2, },
    { 94, 588, 2, },
    { 95, 106, 2, },
    { 95, 114, 2, },
    { 95, 567, 2, },
    { 95, 589, 2, },
    { 96, 107, 2, },
    { 96, 115, 2, },
    { 96, 568, 2, },
    { 96, 590, 2, },
    { 97, 108, 2, },
    { 97, 116, 2, },
    { 97, 569, 2, },
    { 97, 591, 2, },
    { 98, 109, 2, },
    { 98, 117, 2, },
    { 98, 540, 2, },
    { 98, 592, 2, },
    { 99, 110, 2, },
    { 99, 118, 2, },
    { 99, 541, 2, },
    { 99, 593, 2, },
    { 100, 111, 2, },
    { 100, 119, 2, },
    { 100, 542, 2, },
    { 100, 594, 2, },
    { 101, 112, 2, },
    { 101, 543, 2, },
    { 101, 595, 2, },
    { 102, 113, 2, },
    { 102, 544, 2, },
    { 102, 596, 2, },
    { 103, 114, 2, },
    { 103, 545, 2, },
    { 103, 597, 2, },
    { 104, 115, 2, },
    { 104, 546, 2, },
    { 104, 598, 2, },
    { 105, 116, 2, },
    { 105, 547, 2, },
    { 105, 599, 2, },
    { 106, 117, 2, },
    { 106, 548, 2, },
    { 106, 570, 2, },
    { 107, 118, 2, },
    { 107, 549, 2, },
    { 107, 571, 2, },
    { 108, 119, 2, },
    { 108, 550, 2, },
    { 108, 572, 2, },
    { 109, 551, 2, },
    { 109, 573, 2, },
    { 110, 552, 2, },
    { 110, 574, 2, },
    { 111, 553, 2, },
    { 111, 575, 2, },
    { 112, 554, 2, },
    { 112, 576, 2, },
    { 113, 555, 2, },
    { 113, 577, 2, },
    { 114, 556, 2, },
    { 114, 578, 2, },
    { 115, 557, 2, },
    { 115, 579, 2, },
    { 116, 558, 2, },
    { 116, 580, 2, },
    { 117, 559, 2, },
    { 117, 581, 2, },
    { 118, 560, 2, },
    { 118, 582, 2, },
    { 119, 561, 2, },
    { 119, 583, 2, },
    { 120, 150, 2, },
    { 120, 232, 2, },
    { 121, 151, 2, },
    { 121, 233, 2, },
    { 122, 152, 2, },
    { 122, 234, 2, },
    { 123, 153, 2, },
    { 123, 235, 2, },
    { 124, 154, 2, },
    { 124, 236, 2, },
    { 125, 155, 2, },
    { 125, 237, 2, },
    { 126, 156, 2, },
    { 126, 238, 2, },
    { 127, 157, 2, },
    { 127, 239, 2, },
    { 128, 158, 2, },
    { 128, 210, 2, },
    { 129, 159, 2, },
    { 129, 211, 2, },
    { 130, 160, 2, },
    { 130, 212, 2, },
    { 131, 161, 2, },
    { 131, 213, 2, },
    { 132, 162, 2, },
    { 132, 214, 2, },
    { 133, 163, 2, },
    { 133, 215, 2, },
    { 134, 164, 2, },
    { 134, 216, 2, },
    { 135, 165, 3, },
    { 135, 217, 3, },
    { 136, 166, 3, },
    { 136, 218, 3, },
    { 137, 167, 3, },
    { 137, 219, 3, },
    { 138, 168, 3, },
    { 138, 220, 3, },
    { 139, 169, 3, },
    { 139, 221, 3, },
    { 140, 170, 3, },
    { 140, 222, 3, },
    { 141, 171, 3, },
    { 141, 223, 3, },
    { 142, 172, 3, },
    { 142, 224, 3, },
    { 143, 173, 3, },
    { 143, 225, 3, },
    { 144, 174, 3, },
    { 144, 226, 3, },
    { 145, 175, 3, },
    { 145, 227, 3, },
    { 146, 176, 3, },
    { 146, 228, 3, },
    { 147, 177, 3, },
    { 147, 229, 3, },
    { 148, 178, 3, },
    { 148, 230, 3, },
    { 149, 179, 3, },
    { 149, 231, 3, },
    { 150, 207, 3, },
    { 151, 208, 3, },
    { 152, 209, 3, },
    { 153, 180, 3, },
    { 154, 181, 3, },
    { 155, 182, 3, },
    { 156, 183, 3, },
    { 157, 184, 3, },
    { 158, 185, 3, },
    { 159, 186, 3, },
    { 160, 187, 3, },
    { 161, 188, 3, },
    { 162, 189, 3, },
    { 163, 190, 3, },
    { 164, 191, 3, },
    { 165, 192, 3, },
    { 166, 193, 3, },
    { 167, 194, 3, },
    { 168, 195, 3, },
    { 169, 196, 3, },
    { 170, 197, 3, },
    { 171, 198, 3, },
    { 172, 199, 3, },
    { 173, 200, 3, },
    { 174, 201, 3, },
    { 175, 202, 3, },
    { 176, 203, 3, },
    { 177, 204, 3, },
    { 178, 205, 3, },
    { 179, 206, 3, },
    { 180, 237, 3, },
    { 180, 240, 3, },
    { 180, 300, 3, },
    { 181, 238, 3, },
    { 181, 241, 3, },
    { 181, 301, 3, },
    { 182, 239, 3, },
    { 182, 242, 3, },
    { 182, 302, 3, },
    { 183, 210, 3, },
    { 183, 243, 3, },
    { 183, 303, 3, },
    { 184, 211, 3, },
    { 184, 244, 3, },
    { 184, 304, 3, },
    { 185, 212, 3, },
    { 185, 245, 3, },
    { 185, 305, 3, },
    { 186, 213, 3, },
    { 186, 246, 3, },
    { 186, 306, 3, },
    { 187, 214, 3, },
    { 187, 247, 3, },
    { 187, 307, 3, },
    { 188, 215, 3, },
    { 188, 248, 3, },
    { 188, 308, 3, },
    { 189, 216, 3, },
    { 189, 249, 3, },
    { 189, 309, 3, },
    { 190, 217, 3, },
    { 190, 250, 3, },
    { 190, 310, 3, },
    { 191, 218, 3, },
    { 191, 251, 3, },
    { 191, 311, 3, },
    { 192, 219, 3, },
    { 192, 252, 3, },
    { 192, 312, 3, },
    { 193, 220, 3, },
    { 193, 253, 3, },
    { 193, 313, 3, },
    { 194, 221, 3, },
    { 194, 254, 3, },
    { 194, 314, 3, },
    { 195, 222, 3, },
    { 195, 255, 3, },
    { 195, 315, 3, },
    { 196, 223, 3, },
    { 196, 256, 3, },
    { 196, 316, 3, },
    { 197, 224, 3, },
    { 197, 257, 3, },
    { 197, 317, 3, },
    { 198, 225, 3, },
    { 198, 258, 3, },
    { 198, 318, 3, },
    { 199, 226, 3, },
    { 199, 259, 3, },
    { 199, 319, 3, },
    { 200, 227, 3, },
    { 200, 260, 3, },
    { 200, 320, 3, },
    { 201, 228, 3, },
    { 201, 261, 3, },
    { 201, 321, 3, },
    { 202, 229, 3, },
    { 202, 262, 3, },
    { 202, 322, 3, },
    { 203, 230, 3, },
    { 203, 263, 3, },
    { 203, 323, 3, },
    { 204, 231, 3, },
    { 204, 264, 3, },
    { 204, 324, 3, },
    { 205, 232, 3, },
    { 205, 265, 3, },
    { 205, 325, 3, },
    { 206, 233, 3, },
    { 206, 266, 3, },
    { 206, 326, 3, },
    { 207, 234, 3, },
    { 207, 267, 3, },
    { 207, 327, 3, },
    { 208, 235, 3, },
    { 208, 268, 3, },
    { 208, 328, 3, },
    { 209, 236, 3, },
    { 209, 269, 3, },
    { 209, 329, 3, },
    { 210, 270, 4, },
    { 210, 330, 4, },
    { 211, 271, 4, },
    { 211, 331, 4, },
    { 212, 272, 4, },
    { 212, 332, 4, },
    { 213, 273, 4, },
    { 213, 333, 4, },
    { 214, 274, 4, },
    { 214, 334, 4, },
    { 215, 275, 4, },
    { 215, 335, 4, },
    { 216, 276, 4, },
    { 216, 336, 4, },
    { 217, 277, 4, },
    { 217, 337, 4, },
    { 218, 278, 4, },
    { 218, 338, 4, },
    { 219, 279, 4, },
    { 219, 339, 4, },
    { 220, 280, 4, },
    { 220, 340, 4, },
    { 221, 281, 4, },
    { 221, 341, 4, },
    { 222, 282, 4, },
    { 222, 342, 4, },
    { 223, 283, 4, },
    { 223, 343, 4, },
    { 224, 284, 4, },
    { 224, 344, 4, },
    { 225, 285, 4, },
    { 225, 345, 4, },
    { 226, 286, 4, },
    { 226, 346, 4, },
    { 227, 287, 4, },
    { 227, 347, 4, },
    { 228, 288, 4, },
    { 228, 348, 4, },
    { 229, 289, 4, },
    { 229, 349, 4, },
    { 230, 290, 4, },
    { 230, 350, 4, },
    { 231, 291, 4, },
    { 231, 351, 4, },
    { 232, 292, 4, },
    { 232, 352, 4, },
    { 233, 293, 4, },
    { 233, 353, 4, },
    { 234, 294, 4, },
    { 234, 354, 4, },
    { 235, 295, 4, },
    { 235, 355, 4, },
    { 236, 296, 4, },
    { 236, 356, 4, },
    { 237, 297, 4, },
    { 237, 357, 4, },
    { 238, 298, 4, },
    { 238, 358, 4, },
    { 239, 299, 4, },
    { 239, 359, 4, },
    { 240, 327, 4, },
    { 240, 416, 4, },
    { 241, 328, 4, },
    { 241, 417, 4, },
    { 242, 329, 4, },
    { 242, 418, 4, },
    { 243, 300, 4, },
    { 243, 419, 4, },
    { 244, 301, 4, },
    { 244, 390, 4, },
    { 245, 302, 4, },
    { 245, 391, 4, },
    { 246, 303, 4, },
    { 246, 392, 4, },
    { 247, 304, 4, },
    { 247, 393, 4, },
    { 248, 305, 4, },
    { 248, 394, 4, },
    { 249, 306, 4, },
    { 249, 395, 4, },
    { 250, 307, 4, },
    { 250, 396, 4, },
    { 251, 308, 4, },
    { 251, 397, 4, },
    { 252, 309, 4, },
    { 252, 398, 4, },
    { 253, 310, 4, },
    { 253, 399, 4, },
    { 254, 311, 4, },
    { 254, 400, 4, },
    { 255, 312, 4, },
    { 255, 401, 4, },
    { 256, 313, 4, },
    { 256, 402, 4, },
    { 257, 314, 4, },
    { 257, 403, 4, },
    { 258, 315, 4, },
    { 258, 404, 4, },
    { 259, 316, 4, },
    { 259, 405, 4, },
    { 260, 317, 4, },
    { 260, 406, 4, },
    { 261, 318, 4, },
    { 261, 407, 4, },
    { 262, 319, 4, },
    { 262, 408, 4, },
    { 263, 320, 4, },
    { 263, 409, 4, },
    { 264, 321, 4, },
    { 264, 410, 4, },
    { 265, 322, 4, },
    { 265, 411, 4, },
    { 266, 323, 4, },
    { 266, 412, 4, },
    { 267, 324, 4, },
    { 267, 413, 4, },
    { 268, 325, 4, },
    { 268, 414, 4, },
    { 269, 326, 4, },
    { 269, 415, 4, },
    { 270, 333, 4, },
    { 270, 364, 4, },
    { 271, 334, 4, },
    { 271, 365, 4, },
    { 272, 335, 4, },
    { 272, 366, 4, },
    { 273, 336, 4, },
    { 273, 367, 4, },
    { 274, 337, 4, },
    { 274, 368, 4, },
    { 275, 338, 4, },
    { 275, 369, 4, },
    { 276, 339, 4, },
    { 276, 370, 4, },
    { 277, 340, 4, },
    { 277, 371, 4, },
    { 278, 341, 4, },
    { 278, 372, 4, },
    { 279, 342, 4, },
    { 279, 373, 4, },
    { 280, 343, 4, },
    { 280, 374, 4, },
    { 281, 344, 4, },
    { 281, 375, 4, },
    { 282, 345, 4, },
    { 282, 376, 4, },
    { 283, 346, 4, },
    { 283, 377, 4, },
    { 284, 347, 4, },
    { 284, 378, 4, },
    { 285, 348, 5, },
    { 285, 379, 5, },
    { 286, 349, 5, },
    { 286, 380, 5, },
    { 287, 350, 5, },
    { 287, 381, 5, },
    { 288, 351, 5, },
    { 288, 382, 5, },
    { 289, 352, 5, },
    { 289, 383, 5, },
    { 290, 353, 5, },
    { 290, 384, 5, },
    { 291, 354, 5, },
    { 291, 385, 5, },
    { 292, 355, 5, },
    { 292, 386, 5, },
    { 293, 356, 5, },
    { 293, 387, 5, },
    { 294, 357, 5, },
    { 294, 388, 5, },
    { 295, 358, 5, },
    { 295, 389, 5, },
    { 296, 359, 5, },
    { 296, 360, 5, },
    { 297, 330, 5, },
    { 297, 361, 5, },
    { 298, 331, 5, },
    { 298, 362, 5, },
    { 299, 332, 5, },
    { 299, 363, 5, },
    { 300, 330, 5, },
    { 300, 479, 5, },
    { 301, 331, 5, },
    { 301, 450, 5, },
    { 302, 332, 5, },
    { 302, 451, 5, },
    { 303, 333, 5, },
    { 303, 452, 5, },
    { 304, 334, 5, },
    { 304, 453, 5, },
    { 305, 335, 5, },
    { 305, 454, 5, },
    { 306, 336, 5, },
    { 306, 455, 5, },
    { 307, 337, 5, },
    { 307, 456, 5, },
    { 308, 338, 5, },
    { 308, 457, 5, },
    { 309, 339, 5, },
    { 309, 458, 5, },
    { 310, 340, 5, },
    { 310, 459, 5, },
    { 311, 341, 5, },
    { 311, 460, 5, },
    { 312, 342, 5, },
    { 312, 461, 5, },
    { 313, 343, 5, },
    { 313, 462, 5, },
    { 314, 344, 5, },
    { 314, 463, 5, },
    { 315, 345, 5, },
    { 315, 464, 5, },
    { 316, 346, 5, },
    { 316, 465, 5, },
    { 317, 347, 5, },
    { 317, 466, 5, },
    { 318, 348, 5, },
    { 318, 467, 5, },
    { 319, 349, 5, },
    { 319, 468, 5, },
    { 320, 350, 5, },
    { 320, 469, 5, },
    { 321, 351, 5, },
    { 321, 470, 5, },
    { 322, 352, 5, },
    { 322, 471, 5, },
    { 323, 353, 5, },
    { 323, 472, 5, },
    { 324, 354, 5, },
    { 324, 473, 5, },
    { 325, 355, 5, },
    { 325, 474, 5, },
    { 326, 356, 5, },
    { 326, 475, 5, },
    { 327, 357, 5, },
    { 327, 476, 5, },
    { 328, 358, 5, },
    { 328, 477, 5, },
    { 329, 359, 5, },
    { 329, 478, 5, },
    { 330, 421, 5, },
    { 331, 422, 5, },
    { 332, 423, 5, },
    { 333, 424, 5, },
    { 334, 425, 5, },
    { 335, 426, 5, },
    { 336, 427, 5, },
    { 337, 428, 5, },
    { 338, 429, 5, },
    { 339, 430, 5, },
    { 340, 431, 5, },
    { 341, 432, 5, },
    { 342, 433, 5, },
    { 343, 434, 5, },
    { 344, 435, 5, },
    { 345, 436, 5, },
    { 346, 437, 5, },
    { 347, 438, 5, },
    { 348, 439, 5, },
    { 349, 440, 5, },
    { 350, 441, 5, },
    { 351, 442, 5, },
    { 352, 443, 5, },
    { 353, 444, 5, },
    { 354, 445, 5, },
    { 355, 446, 5, },
    { 356, 447, 5, },
    { 357, 448, 5, },
    { 358, 449, 5, },
    { 359, 420, 5, },
    { 360, 390, 5, },
    { 360, 447, 5, },
    { 360, 480, 5, },
    { 361, 391, 5, },
    { 361, 448, 5, },
    { 361, 481, 5, },
    { 362, 392, 5, },
    { 362, 449, 5, },
    { 362, 482, 5, },
    { 363, 393, 5, },
    { 363, 420, 5, },
    { 363, 483, 5, },
    { 364, 394, 5, },
    { 364, 421, 5, },
    { 364, 484, 5, },
    { 365, 395, 5, },
    { 365, 422, 5, },
    { 365, 485, 5, },
    { 366, 396, 5, },
    { 366, 423, 5, },
    { 366, 486, 5, },
    { 367, 397, 5, },
    { 367, 424, 5, },
    { 367, 487, 5, },
    { 368, 398, 5, },
    { 368, 425, 5, },
    { 368, 488, 5, },
    { 369, 399, 5, },
    { 369, 426, 5, },
    { 369, 489, 5, },
    { 370, 400, 6, },
    { 370, 427, 6, },
    { 370, 490, 6, },
    { 371, 401, 6, },
    { 371, 428, 6, },
    { 371, 491, 6, },
    { 372, 402, 6, },
    { 372, 429, 6, },
    { 372, 492, 6, },
    { 373, 403, 6, },
    { 373, 430, 6, },
    { 373, 493, 6, },
    { 374, 404, 6, },
    { 374, 431, 6, },
    { 374, 494, 6, },
    { 375, 405, 6, },
    { 375, 432, 6, },
    { 375, 495, 6, },
    { 376, 406, 6, },
    { 376, 433, 6, },
    { 376, 496, 6, },
    { 377, 407, 6, },
    { 377, 434, 6, },
    { 377, 497, 6, },
    { 378, 408, 6, },
    { 378, 435, 6, },
    { 378, 498, 6, },
    { 379, 409, 6, },
    { 379, 436, 6, },
    { 379, 499, 6, },
    { 380, 410, 6, },
    { 380, 437, 6, },
    { 380, 500, 6, },
    { 381, 411, 6, },
    { 381, 438, 6, },
    { 381, 501, 6, },
    { 382, 412, 6, },
    { 382, 439, 6, },
    { 382, 502, 6, },
    { 383, 413, 6, },
    { 383, 440, 6, },
    { 383, 503, 6, },
    { 384, 414, 6, },
    { 384, 441, 6, },
    { 384, 504, 6, },
    { 385, 415, 6, },
    { 385, 442, 6, },
    { 385, 505, 6, },
    { 386, 416, 6, },
    { 386, 443, 6, },
    { 386, 506, 6, },
    { 387, 417, 6, },
    { 387, 444, 6, },
    { 387, 507, 6, },
    { 388, 418, 6, },
    { 388, 445, 6, },
    { 388, 508, 6, },
    { 389, 419, 6, },
    { 389, 446, 6, },
    { 389, 509, 6, },
    { 390, 453, 6, },
    { 390, 510, 6, },
    { 391, 454, 6, },
    { 391, 511, 6, },
    { 392, 455, 6, },
    { 392, 512, 6, },
    { 393, 456, 6, },
    { 393, 513, 6, },
    { 394, 457, 6, },
    { 394, 514, 6, },
    { 395, 458, 6, },
    { 395, 515, 6, },
    { 396, 459, 6, },
    { 396, 516, 6, },
    { 397, 460, 6, },
    { 397, 517, 6, },
    { 398, 461, 6, },
    { 398, 518, 6, },
    { 399, 462, 6, },
    { 399, 519, 6, },
    { 400, 463, 6, },
    { 400, 520, 6, },
    { 401, 464, 6, },
    { 401, 521, 6, },
    { 402, 465, 6, },
    { 402, 522, 6, },
    { 403, 466, 6, },
    { 403, 523, 6, },
    { 404, 467, 6, },
    { 404, 524, 6, },
    { 405, 468, 6, },
    { 405, 525, 6, },
    { 406, 469, 6, },
    { 406, 526, 6, },
    { 407, 470, 6, },
    { 407, 527, 6, },
    { 408, 471, 6, },
    { 408, 528, 6, },
    { 409, 472, 6, },
    { 409, 529, 6, },
    { 410, 473, 6, },
    { 410, 530, 6, },
    { 411, 474, 6, },
    { 411, 531, 6, },
    { 412, 475, 6, },
    { 412, 532, 6, },
    { 413, 476, 6, },
    { 413, 533, 6, },
    { 414, 477, 6, },
    { 414, 534, 6, },
    { 415, 478, 6, },
    { 415, 535, 6, },
    { 416, 479, 6, },
    { 416, 536, 6, },
    { 417, 450, 6, },
    { 417, 537, 6, },
    { 418, 451, 6, },
    { 418, 538, 6, },
    { 419, 452, 6, },
    { 419, 539, 6, },
    { 420, 480, 6, },
    { 421, 481, 6, },
    { 422, 482, 6, },
    { 423, 483, 6, },
    { 424, 484, 6, },
    { 425, 485, 6, },
    { 426, 486, 6, },
    { 427, 487, 6, },
    { 428, 488, 6, },
    { 429, 489, 6, },
    { 430, 490, 6, },
    { 431, 491, 6, },
    { 432, 492, 6, },
    { 433, 493, 6, },
    { 434, 494, 6, },
    { 435, 495, 6, },
    { 436, 496, 6, },
    { 437, 497, 6, },
    { 438, 498, 6, },
    { 439, 499, 6, },
    { 440, 500, 6, },
    { 441, 501, 6, },
    { 442, 502, 6, },
    { 443, 503, 6, },
    { 444, 504, 6, },
    { 445, 505, 6, },
    { 446, 506, 6, },
    { 447, 507, 6, },
    { 448, 508, 6, },
    { 449, 509, 6, },
    { 450, 510, 7, },
    { 451, 511, 7, },
    { 452, 512, 7, },
    { 453, 513, 7, },
    { 454, 514, 7, },
    { 455, 515, 7, },
    { 456, 516, 7, },
    { 457, 517, 7, },
    { 458, 518, 7, },
    { 459, 519, 7, },
    { 460, 520, 7, },
    { 461, 521, 7, },
    { 462, 522, 7, },
    { 463, 523, 7, },
    { 464, 524, 7, },
    { 465, 525, 7, },
    { 466, 526, 7, },
    { 467, 527, 7, },
    { 468, 528, 7, },
    { 469, 529, 7, },
    { 470, 530, 7, },
    { 471, 531, 7, },
    { 472, 532, 7, },
    { 473, 533, 7, },
    { 474, 534, 7, },
    { 475, 535, 7, },
    { 476, 536, 7, },
    { 477, 537, 7, },
    { 478, 538, 7, },
    { 479, 539, 7, },
    { 480, 513, 7, },
    { 480, 597, 7, },
    { 481, 514, 7, },
    { 481, 598, 7, },
    { 482, 515, 7, },
    { 482, 599, 7, },
    { 483, 516, 7, },
    { 483, 570, 7, },
    { 484, 517, 7, },
    { 484, 571, 7, },
    { 485, 518, 7, },
    { 485, 572, 7, },
    { 486, 519, 7, },
    { 486, 573, 7, },
    { 487, 520, 7, },
    { 487, 574, 7, },
    { 488, 521, 7, },
    { 488, 575, 7, },
    { 489, 522, 7, },
    { 489, 576, 7, },
    { 490, 523, 7, },
    { 490, 577, 7, },
    { 491, 524, 7, },
    { 491, 578, 7, },
    { 492, 525, 7, },
    { 492, 579, 7, },
    { 493, 526, 7, },
    { 493, 580, 7, },
    { 494, 527, 7, },
    { 494, 581, 7, },
    { 495, 528, 7, },
    { 495, 582, 7, },
    { 496, 529, 7, },
    { 496, 583, 7, },
    { 497, 530, 7, },
    { 497, 584, 7, },
    { 498, 531, 7, },
    { 498, 585, 7, },
    { 499, 532, 7, },
    { 499, 586, 7, },
    { 500, 533, 7, },
    { 500, 587, 7, },
    { 501, 534, 7, },
    { 501, 588, 7, },
    { 502, 535, 7, },
    { 502, 589, 7, },
    { 503, 536, 7, },
    { 503, 590, 7, },
    { 504, 537, 7, },
    { 504, 591, 7, },
    { 505, 538, 7, },
    { 505, 592, 7, },
    { 506, 539, 7, },
    { 506, 593, 7, },
    { 507, 510, 7, },
    { 507, 594, 7, },
    { 508, 511, 7, },
    { 508, 595, 7, },
    { 509, 512, 7, },
    { 509, 596, 7, },
    { 510, 542, 7, },
    { 511, 543, 7, },
    { 512, 544, 7, },
    { 513, 545, 7, },
    { 514, 546, 7, },
    { 515, 547, 7, },
    { 516, 548, 7, },
    { 517, 549, 7, },
    { 518, 550, 7, },
    { 519, 551, 7, },
    { 520, 552, 7, },
    { 521, 553, 7, },
    { 522, 554, 7, },
    { 523, 555, 7, },
    { 524, 556, 7, },
    { 525, 557, 7, },
    { 526, 558, 7, },
    { 527, 559, 7, },
    { 528, 560, 7, },
    { 529, 561, 7, },
    { 530, 562, 7, },
    { 531, 563, 7, },
    { 532, 564, 7, },
    { 533, 565, 7, },
    { 534, 566, 7, },
    { 535, 567, 7, },
    { 536, 568, 7, },
    { 537, 569, 7, },
    { 538, 540, 7, },
    { 539, 541, 7, },
    { 540, 570, 7, },
    { 541, 571, 7, },
    { 542, 572, 7, },
    { 543, 573, 7, },
    { 544, 574, 7, },
    { 545, 575, 7, },
    { 546, 576, 7, },
    { 547, 577, 7, },
    { 548, 578, 7, },
    { 549, 579, 7, },
    { 550, 580, 7, },
    { 551, 581, 7, },
    { 552, 582, 7, },
    { 553, 583, 7, },
    { 554, 584, 7, },
    { 555, 585, 7, },
    { 556, 586, 7, },
    { 557, 587, 7, },
    { 558, 588, 7, },
    { 559, 589, 7, },
    { 560, 590, 7, },
    { 561, 591, 7, },
    { 562, 592, 7, },
    { 563, 593, 7, },
    { 564, 594, 7, },
    { 565, 595, 7, },
    { 566, 596, 7, },
    { 567, 597, 7, },
    { 568, 598, 7, },
    { 569, 599, 7, },
};


static const char *hyperball_defaults[] =
{
  "*observer-z: 3",
  "*delay: 20000",
  "*xy: 3",
  "*xz: 5",
  "*yw: 10",
  "*yz: 0",
  "*xw: 0",
  "*zw: 0",
  ".background:	black",
  ".foreground:	white",
  "*fpsSolid:	true",
  "*color00:#FF66BE",
  "*color10:#FFA300",
  "*color20:#BEDC00",
  "*color30:#12FB00",
  "*color40:#00F9BE",
  "*color50:#12D5FF",
  "*color60:#BE9AFF",
  "*color70:#FF5FFF",
  "*color01:#FF5BAA",
  "*color11:#F09200",
  "*color21:#AAC500",
  "*color31:#10E100",
  "*color41:#00DFAA",
  "*color51:#10BFFF",
  "*color61:#AA8AFF",
  "*color71:#F055FF",
  "*color02:#EE529A",
  "*color12:#D98400",
  "*color22:#9AB200",
  "*color32:#0ECB00",
  "*color42:#00C99A",
  "*color52:#0EADE7",
  "*color62:#9A7DFF",
  "*color72:#D94DE7",
  "*color03:#DA4B8C",
  "*color13:#C67900",
  "*color23:#8CA300",
  "*color33:#0DBA00",
  "*color43:#00B88C",
  "*color53:#0D9ED3",
  "*color63:#8C72EA",
  "*color73:#C646D3",
  "*color04:#C84581",
  "*color14:#B66F00",
  "*color24:#819600",
  "*color34:#0CAB00",
  "*color44:#00A981",
  "*color54:#0C91C2",
  "*color64:#8169D7",
  "*color74:#B641C2",
  "*color05:#B94078",
  "*color15:#A96700",
  "*color25:#788B00",
  "*color35:#0B9E00",
  "*color45:#009D78",
  "*color55:#0B86B3",
  "*color65:#7861C7",
  "*color75:#A93CB3",
  "*color06:#AC3C6F",
  "*color16:#9D6000",
  "*color26:#6F8100",
  "*color36:#0A9300",
  "*color46:#00926F",
  "*color56:#0A7DA7",
  "*color66:#6F5AB9",
  "*color76:#9D38A7",
  "*color07:#A13868",
  "*color17:#935900",
  "*color27:#687900",
  "*color37:#0A8A00",
  "*color47:#008868",
  "*color57:#0A759C",
  "*color67:#6854AD",
  "*color77:#93349C",
  0,
};

static XrmOptionDescRec hyperball_options [] =
{
  { "-observer-z",	".observer-z",	XrmoptionSepArg, 0 },
  { "-delay",		".delay",	XrmoptionSepArg, 0 },
  { "-xw",		".xw",		XrmoptionSepArg, 0 },
  { "-xy",		".xy",		XrmoptionSepArg, 0 },
  { "-xz",		".xz",		XrmoptionSepArg, 0 },
  { "-yw",		".yw",		XrmoptionSepArg, 0 },
  { "-yz",		".yz",		XrmoptionSepArg, 0 },
  { "-zw",		".zw",		XrmoptionSepArg, 0 },
  { 0, 0, 0, 0 },
};

XSCREENSAVER_MODULE ("HyperBall", hyperball)
