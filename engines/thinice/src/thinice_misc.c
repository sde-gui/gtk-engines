#include <math.h>
#include <string.h>
#include <gtk/gtknotebook.h>

#include "thinice_style.h"
#include "thinice_rc_style.h"
#include "thinice_misc.h"

GtkShadowType
thinice_shadow_type (GtkStyle *style, const char *detail, GtkShadowType requested)
{
    GtkShadowType retval = GTK_SHADOW_NONE;

    if (requested != GTK_SHADOW_NONE) {
        retval = GTK_SHADOW_ETCHED_IN;
    }

    if (CHECK_DETAIL (detail, "dockitem") || CHECK_DETAIL (detail, "handlebox_bin") || CHECK_DETAIL (detail, "spinbutton_up") || CHECK_DETAIL (detail, "spinbutton_down")) {
        retval = GTK_SHADOW_NONE;
    } else if (CHECK_DETAIL (detail, "button") || CHECK_DETAIL (detail, "togglebutton") || CHECK_DETAIL (detail, "notebook") || CHECK_DETAIL (detail, "optionmenu")) {
        retval = requested;
    } else if (CHECK_DETAIL (detail, "menu")) {
        retval = GTK_SHADOW_ETCHED_IN;
    }

    return retval;
}

/***********************************************
 * thinice_arrow-
 *  
 *    borrowed from redmond till shareable
 ***********************************************/
void thinice_arrow (cairo_t *cr,
			CairoColor *color,
			GtkArrowType arrow_type, 
			gint x, 
			gint y, 
			gint width, 
			gint height)
{
  gint i;
 
  gint steps, extra;
  gint start, increment;
  gint aw = width, ah = height;
 	
  if ((arrow_type == GTK_ARROW_UP) || (arrow_type == GTK_ARROW_DOWN))
    {
      gdouble tmp=((aw+1)/2) - ((height%2)?1:0);
       
      if (tmp > ah) 
        {
          aw = 2*ah - 1 - ((height%2)?1:0);
          ah = (aw+1)/2;
        } 
      else 
        {
          ah = (gint) tmp;
          aw = 2*ah - 1;
        }  
 
      if ((aw < 5) || (ah < 3)) 
        {
          aw = 5;
          ah = 3;
        }
 
      x += (width - aw) / 2 ;
      y += (height - ah) / 2;
      width = aw;
      height = ah;
 		
      width += width % 2 - 1;
 
      steps = 1 + width / 2;
      extra = height - steps;
 
      if (arrow_type == GTK_ARROW_DOWN)
        {
          start = y;
          increment = 1;
        }
      else
        {
          start = y + height - 1;
          increment = -1;
        }
 
      cairo_save(cr);

      ge_cairo_set_color(cr, color);	
      cairo_set_line_width (cr, 0.5);

      cairo_move_to(cr, x + 0.5, start + 0.5);
      cairo_line_to(cr, x + width - 0.5, start + 0.5);
      cairo_line_to(cr, x + ((height - 1) - extra) + 0.5, start + (height - 1)*increment + 0.5);
      cairo_line_to(cr, x + 0.5, start + 0.5);

      cairo_stroke(cr);


      cairo_move_to(cr, x + 0.5, start + 0.5);
      cairo_line_to(cr, x + width - 0.5, start + 0.5);
      cairo_line_to(cr, x + ((height - 1) - extra) + 0.5, start + (height - 1)*increment + 0.5);
      cairo_line_to(cr, x + 0.5, start + 0.5);

      cairo_fill(cr);

      cairo_restore(cr);
    }
  else
    {
      gdouble tmp=((ah+1)/2) - ((width%2)?1:0);
 
      if (tmp > aw) 
        {
          ah = 2*aw - 1 - ((width%2)?1:0);
          aw = (ah+1)/2;
        } 
      else 
        {
          aw = (gint) tmp;
          ah = 2*aw - 1;
        }  
 
      if ((ah < 5) || (aw < 3)) 
        {
          ah = 5;
          aw = 3;
        }
 
      x += (width - aw) / 2 ;
      y += (height - ah) / 2;
      width = aw;
      height = ah;
 
      height += height % 2 - 1;
 
      steps = 1 + height / 2;
      extra = width - steps;
 
      if (arrow_type == GTK_ARROW_RIGHT)
        {
          start = x;
          increment = 1;
        }
      else
        {
          start = x + width - 1;
          increment = -1;
        }
 
      cairo_save(cr);

      ge_cairo_set_color(cr, color);	
      cairo_set_line_width (cr, 0.5);

      cairo_move_to(cr, start + 0.5, y + 0.5);
      cairo_line_to(cr, start + 0.5, y + height - 0.5);
      cairo_line_to(cr, start + (width - 1)*increment + 0.5, y + ((width - 1) - extra) + 0.5);
      cairo_line_to(cr, start + 0.5, y + 0.5);

      cairo_stroke(cr);


      cairo_move_to(cr, start + 0.5, y + 0.5);
      cairo_line_to(cr, start + 0.5, y + height - 0.5);
      cairo_line_to(cr, start + (width - 1)*increment + 0.5, y + ((width - 1) - extra) + 0.5);
      cairo_line_to(cr, start + 0.5, y + 0.5);

      cairo_fill(cr);

      cairo_restore(cr);
    }
}

void
thinice_slash_two(cairo_t *cr,
                  CairoColor *color1,
                  CairoColor *color2,
                  gint x,
                  gint y,
                  gint width,
                  gint height)
{
  gdouble centerx, centery, thick;
  gint ax1=0,ax2=0,ay1=0,ay2=0;

  centerx = width/2 + x + 0.5;
  centery = height/2 + y + 0.5;
  if (width > height)
    {
      ax1 = -2; ax2 = 1;
    }
  else
    {
      ay1 = -2; ay2 = 1;
    }

  thick = ((width < height?width-1:height-1) >> 1) - 1.5;

	cairo_set_line_width (cr, 1);

	ge_cairo_set_color(cr, color2);	
  	cairo_move_to(cr, centerx - thick + ax1, centery + thick + ay1);
	cairo_line_to(cr, centerx + thick + ax1, centery - thick + ay1);
	cairo_stroke(cr);

	ge_cairo_set_color(cr, color1);	
	cairo_move_to(cr, centerx - thick + ax1 + ax2, centery + thick + ay1 + ay2);
	cairo_line_to(cr, centerx + thick + ax1 + ax2, centery - thick + ay1 + ay2);
	cairo_stroke(cr);

  if (width > height)
    {
      ax1 = 2; /* ax2 = 1; */
    }
  else
    {
      ay1 = 2; /* ay2 = 1; */
    }

	cairo_set_line_width (cr, 1);

	ge_cairo_set_color(cr, color2);	
  	cairo_move_to(cr, centerx - thick + ax1, centery + thick + ay1);
	cairo_line_to(cr, centerx + thick + ax1, centery - thick + ay1);
	cairo_stroke(cr);

	ge_cairo_set_color(cr, color1);	
	cairo_move_to(cr, centerx - thick + ax1 + ax2, centery + thick + ay1 + ay2);
	cairo_line_to(cr, centerx + thick + ax1 + ax2, centery - thick + ay1 + ay2);
	cairo_stroke(cr);
}


void
thinice_slash_one(cairo_t *cr,
                  CairoColor *color1,
                  CairoColor *color2,
                  gint x,
                  gint y,
                  gint width,
                  gint height)
{
  gint centerx, centery, thick;

  centerx = width/2 + x + 0.5;
  centery = height/2 + y + 0.5;

  thick = ((width < height?width:height) >> 1) - 1.5;

  cairo_set_line_width (cr, 1);

  ge_cairo_set_color(cr, color2);	
  cairo_move_to(cr, centerx - thick, centery + thick);
  cairo_line_to(cr, centerx + thick, centery - thick);
  cairo_stroke(cr);

  ge_cairo_set_color(cr, color2);	
  cairo_move_to(cr, centerx - thick, centery + thick - 1);
  cairo_line_to(cr, centerx + thick - 1, centery - thick);
  cairo_stroke(cr);
}

void
thinice_dot(cairo_t *cr,
            CairoColor *color1,
            CairoColor *color2,
            gint x,
            gint y)
{
	cairo_save(cr);

	cairo_set_line_width (cr, 0.5);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

	ge_cairo_set_color(cr, color2);	

	cairo_rectangle (cr, x - 1, y, 0.5, 0.5);
	cairo_rectangle (cr, x - 1, y - 1, 0.5, 0.5);
	cairo_rectangle (cr, x, y - 1, 0.5, 0.5);

	cairo_stroke(cr);
	
	ge_cairo_set_color(cr, color1);	

	cairo_rectangle (cr, x + 1, y, 0.5, 0.5);
	cairo_rectangle (cr, x + 1, y + 1, 0.5, 0.5);
	cairo_rectangle (cr, x, y + 1, 0.5, 0.5);

	cairo_stroke(cr);

	cairo_restore(cr);
}


void
thinice_draw_separator(cairo_t *cr, gboolean horizontal,
                           int x, int y, int width, int height)
{
	if (horizontal)
	{
		cairo_set_line_width  (cr, 1.0);
		cairo_translate       (cr, x, y+0.5);
		
		cairo_move_to         (cr, 0.0,     0.0);
		cairo_line_to         (cr, width+1, 0.0);
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.2);
		cairo_stroke          (cr);
		
		cairo_move_to         (cr, 0.0,   1.0);
		cairo_line_to         (cr, width, 1.0);
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.8);
		cairo_stroke          (cr);				
	}
	else
	{
		cairo_set_line_width  (cr, 1.0);
		cairo_translate       (cr, x+0.5, y);
		
		cairo_move_to         (cr, 0.0, 0.0);
		cairo_line_to         (cr, 0.0, height);
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.2);
		cairo_stroke          (cr);
		
		cairo_move_to         (cr, 1.0, 0.0);
		cairo_line_to         (cr, 1.0, height);
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.8);
		cairo_stroke          (cr);		
	}
}
