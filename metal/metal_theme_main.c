#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gmodule.h>

/**************************************************************************
* GTK Metal Theme
*
* Version 0.9, Oct 2, 1998
*
* Copyright 1998: Randy Gordon, Integrand Systems
*                 http://www.integrand.com
*                 mailto://randy@integrand.com
*
* License: GPL (Gnu Public License)
*
*
**************************************************************************/

#define DEBUG 0

#define SCROLLBAR_WIDTH 17
#if 0
#define SCALE_WIDTH     15
#else
#define SCALE_WIDTH     31
#endif

/* Theme functions to export */
void                theme_init(GtkThemeEngine * engine);
void                theme_exit(void);

/* Exported vtable from th_draw */

extern GtkStyleClass metal_default_class;
extern GtkStyleClass metal_special_class;

/* internals */

/* external theme functions called */

static gint
gtk_range_expose_metal (GtkWidget      *widget,
                       GdkEventExpose *event);
static void
shade(GdkColor *oldcolor, GdkColor *newcolor, float mult);


/* Useful GCs for coloring (based on white_gc) */
GdkGC *metal_light_gray_gc;
GdkGC *metal_mid_gray_gc;
GdkGC *metal_dark_gray_gc;


/* external theme functions called */

static guint
theme_parse_rc_style(GScanner * scanner,
		     GtkRcStyle * rc_style)
{
  guint               token;

  token = g_scanner_peek_next_token(scanner);
  while (token != G_TOKEN_RIGHT_CURLY)
    {
      switch (token)
	{
	default:
	  g_scanner_get_next_token(scanner);
	  token = G_TOKEN_RIGHT_CURLY;
	  break;
	}

      if (token != G_TOKEN_NONE)
	return token;

      token = g_scanner_peek_next_token(scanner);
    }

  g_scanner_get_next_token(scanner);

  rc_style->engine_data = NULL;

  return G_TOKEN_NONE;
}

static void
theme_merge_rc_style(GtkRcStyle * dest,
		     GtkRcStyle * src)
{
}

static void
theme_rc_style_to_style(GtkStyle * style,
			GtkRcStyle * rc_style)
{
  style->klass = &metal_default_class;
}

static void
theme_duplicate_style(GtkStyle * dest,
		      GtkStyle * src)
{
}

static void
theme_realize_style(GtkStyle * style)
{
}

static void
theme_unrealize_style(GtkStyle * style)
{
}

static void
theme_destroy_rc_style(GtkRcStyle * rc_style)
{
}

static void
theme_destroy_style(GtkStyle * style)
{
}

/******************************************************************/
void
theme_init(GtkThemeEngine * engine)
{
   GtkRangeClass *rangeclass;
   GtkScaleClass *scaleclass;
   GtkButtonClass *buttonclass;
   GtkToggleButtonClass *togglebuttonclass;
   GtkWidgetClass *widget_class;
   GtkStyle *style;
   GtkVScrollbar *scrollbar;
   GdkColor white;
   GdkColor gray;
   GdkGCValues values;
   GdkColormap *colormap;
   gint x, y, width, height, depth;

#if DEBUG
  printf("Metal Theme Init\n");
#endif

   engine->parse_rc_style = theme_parse_rc_style;
   engine->merge_rc_style = theme_merge_rc_style;
   engine->rc_style_to_style = theme_rc_style_to_style;
   engine->duplicate_style = theme_duplicate_style;
   engine->realize_style = theme_realize_style;
   engine->unrealize_style = theme_unrealize_style;
   engine->destroy_rc_style = theme_destroy_rc_style;
   engine->destroy_style = theme_destroy_style;
   engine->set_background = NULL;

   /* Make scrollbars wider */
   rangeclass = (GtkRangeClass *)gtk_type_class(gtk_range_get_type());
   rangeclass->slider_width    = SCROLLBAR_WIDTH;
   rangeclass->min_slider_size = SCROLLBAR_WIDTH;
   rangeclass->stepper_size    = SCROLLBAR_WIDTH;
   rangeclass->stepper_slider_spacing = 0;

   /* Make scale slider smaller */
   scaleclass = (GtkScaleClass *)gtk_type_class(gtk_scale_get_type());
   scaleclass->slider_length   = SCALE_WIDTH;

   /* A bunch of hacks to workaround GTK problems */
   widget_class = (GtkWidgetClass *)rangeclass;
   widget_class->expose_event = gtk_range_expose_metal;

   widget_class = (GtkWidgetClass *)scaleclass;
   widget_class->expose_event = gtk_range_expose_metal;

   scrollbar = (GtkVScrollbar *)gtk_vscrollbar_new(NULL);
   GTK_WIDGET(scrollbar)->style->klass = &metal_special_class;


   /* Some useful GCs for coloring (based on shades of style->white) */
   style = GTK_WIDGET(scrollbar)->style;
   white = style->white;
   colormap = gdk_window_get_colormap(GDK_ROOT_PARENT());
   gdk_window_get_geometry(GDK_ROOT_PARENT(), &x, &y, &width, &height, &depth);

   /* Light Gray */
   shade(&white, &gray, 0.8);
   if (!gdk_color_alloc (colormap, &gray)) {
        g_warning ("unable to allocate color: ( %d %d %d )",
                   gray.red, gray.green, gray.blue);
   }
   values.foreground = gray;
   metal_light_gray_gc = gtk_gc_get(depth, colormap, 
                                    &values, GDK_GC_FOREGROUND);
   /* Mid Gray */
   shade(&white, &gray, 0.6);
   if (!gdk_color_alloc (colormap, &gray)) {
        g_warning ("unable to allocate color: ( %d %d %d )",
                   gray.red, gray.green, gray.blue);
   }
   values.foreground = gray;
   metal_mid_gray_gc = gtk_gc_get(depth, colormap, 
                                    &values, GDK_GC_FOREGROUND);
   /* Dark Gray */
   shade(&white, &gray, 0.5);
   if (!gdk_color_alloc (colormap, &gray)) {
        g_warning ("unable to allocate color: ( %d %d %d )",
                   gray.red, gray.green, gray.blue);
   }
   values.foreground = gray;
   metal_dark_gray_gc = gtk_gc_get(depth, colormap, 
                                    &values, GDK_GC_FOREGROUND);
}
/*****************************************************************/
void
theme_exit(void)
{
#if DEBUG
  printf("Metal Theme Exit\n* Need to add memory deallocation code here *\n");
#endif
}
/****************************************************************/
static gint
gtk_range_expose_metal (GtkWidget      *widget,
                       GdkEventExpose *event)
{
  GtkRange *range;

  /* The version of this method in gtkrange.c doesn't work
   * when the slider is as wide as the trough.
   */
  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_RANGE (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  range = GTK_RANGE (widget);

  if (event->window == range->trough)
    {
        gtk_range_draw_trough (range);
    }
  else if (event->window == widget->window)
    {
      gtk_range_draw_background (range);
    }
  else if (event->window == range->slider)
    {
      gtk_range_draw_slider (range);
    }
  else if (event->window == range->step_forw)
    {
      gtk_range_draw_step_forw (range);
    }
  else if (event->window == range->step_back)
    {
      gtk_range_draw_step_back (range);
    }
  return FALSE;
}
/******************************************************************/
static void
shade(GdkColor *oldcolor, GdkColor *newcolor, float mult)
{
   newcolor->red   = oldcolor->red   * mult;
   newcolor->green = oldcolor->green * mult;
   newcolor->blue  = oldcolor->blue  * mult;
} 

/* The following function will be called by GTK+ when the module
 * is loaded and checks to see if we are compatible with the
 * version of GTK+ that loads us.
 */
G_MODULE_EXPORT const gchar* g_module_check_init (GModule *module);
const gchar*
g_module_check_init (GModule *module)
{
  return gtk_check_version (GTK_MAJOR_VERSION,
			    GTK_MINOR_VERSION,
			    GTK_MICRO_VERSION - GTK_INTERFACE_AGE);
}
