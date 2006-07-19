/*
 * HighContrast GTK+ rendering engine for Gnome-Themes.
 *
 * Copyright 2003 Sun Microsystems Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "hc_gtk2_engine.h"
#include "hc_gtk2_support.h"
#include "hc_gtk2_drawing.h"

void 
hc_simple_border_gap_clip(cairo_t *canvas,
				gint border_thickness,

				gint x,
				gint y,
				gint width,
				gint height,

				GtkPositionType gap_side,
				gint gap_pos,
				gint gap_size)
{
	cairo_set_line_width(canvas, 1.0);

	switch (gap_side)
	{
		default:
		case GTK_POS_TOP:
			cairo_move_to(canvas, x, y);
			cairo_line_to(canvas, x, y + height);
			cairo_line_to(canvas, x + width, y + height);
			cairo_line_to(canvas, x + width, y);
			cairo_line_to(canvas, x + gap_pos + gap_size, y);
			cairo_line_to(canvas, x + gap_pos + gap_size, y + border_thickness + 1);
			cairo_line_to(canvas, x + gap_pos, y + border_thickness + 1);
			cairo_line_to(canvas, x + gap_pos, y);
			cairo_line_to(canvas, x, y);
		break;

		case GTK_POS_LEFT:
			cairo_move_to(canvas, x, y);
			cairo_line_to(canvas, x + width, y);
			cairo_line_to(canvas, x + width, y + height);
			cairo_line_to(canvas, x, y + height);
			cairo_line_to(canvas, x, y + gap_pos + gap_size);
			cairo_line_to(canvas, x + border_thickness + 1, y + gap_pos + gap_size);
			cairo_line_to(canvas, x + border_thickness + 1, y + gap_pos);
			cairo_line_to(canvas, x, y + gap_pos);
			cairo_line_to(canvas, x, y);
		break;

		case GTK_POS_BOTTOM:
			cairo_move_to(canvas, x + width, y + height);
			cairo_line_to(canvas, x + width, y);
			cairo_line_to(canvas, x, y);
			cairo_line_to(canvas, x, y + height);
			cairo_line_to(canvas, x + gap_pos, y + height);
			cairo_line_to(canvas, x + gap_pos, y + height - border_thickness - 1);
			cairo_line_to(canvas, x + gap_pos + gap_size, y + height - border_thickness - 1);
			cairo_line_to(canvas, x + gap_pos + gap_size, y + height);
			cairo_line_to(canvas, x + width, y + height);
		break;

		case GTK_POS_RIGHT:
			cairo_line_to(canvas, x + width, y);
			cairo_line_to(canvas, x, y);
			cairo_line_to(canvas, x, y + height);
			cairo_line_to(canvas, x + width, y + height);
			cairo_line_to(canvas, x + width, y + gap_pos + gap_size);
			cairo_line_to(canvas, x + width - border_thickness - 1, y + gap_pos + gap_size);
			cairo_line_to(canvas, x + width - border_thickness - 1, y + gap_pos);
			cairo_line_to(canvas, x + width, y + gap_pos);
			cairo_line_to(canvas, x + width, y);
		break;
	}

	cairo_clip(canvas);
}

/***********************************************
 * do_hc_draw_arrow -
 *  
 *   A simple routine to draw a redmond style
 *   arrow using the passed Color.
 *  
 *   Taken in part from smooth, it was based on 
 *   XFCE's & CleanIce draw arrow routines, 
 *   both which  were based on ThinIce's.
 ***********************************************/
void
do_hc_draw_arrow (cairo_t *canvas,
               CairoColor * color,
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
 
      cairo_save(canvas);

      ge_cairo_set_color(canvas, color);	
      cairo_set_line_width (canvas, 0.5);

      cairo_move_to(canvas, x + 0.5, start + 0.5);
      cairo_line_to(canvas, x + width - 0.5, start + 0.5);
      cairo_line_to(canvas, x + ((height - 1) - extra) + 0.5, start + (height - 1)*increment + 0.5);
      cairo_line_to(canvas, x + 0.5, start + 0.5);

      cairo_stroke(canvas);


      cairo_move_to(canvas, x + 0.5, start + 0.5);
      cairo_line_to(canvas, x + width - 0.5, start + 0.5);
      cairo_line_to(canvas, x + ((height - 1) - extra) + 0.5, start + (height - 1)*increment + 0.5);
      cairo_line_to(canvas, x + 0.5, start + 0.5);

      cairo_fill(canvas);

      cairo_restore(canvas);
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
 
      cairo_save(canvas);

      ge_cairo_set_color(canvas, color);	
      cairo_set_line_width (canvas, 0.5);

      cairo_move_to(canvas, start + 0.5, y + 0.5);
      cairo_line_to(canvas, start + 0.5, y + height - 0.5);
      cairo_line_to(canvas, start + (width - 1)*increment + 0.5, y + ((width - 1) - extra) + 0.5);
      cairo_line_to(canvas, start + 0.5, y + 0.5);

      cairo_stroke(canvas);


      cairo_move_to(canvas, start + 0.5, y + 0.5);
      cairo_line_to(canvas, start + 0.5, y + height - 0.5);
      cairo_line_to(canvas, start + (width - 1)*increment + 0.5, y + ((width - 1) - extra) + 0.5);
      cairo_line_to(canvas, start + 0.5, y + 0.5);

      cairo_fill(canvas);

      cairo_restore(canvas);
    }
}
 
void do_hc_draw_line (cairo_t *cr,
			CairoColor *color,
			gdouble thickness,
			gdouble x1,
			gdouble y1,
			gdouble x2,
			gdouble y2)
{ 
	cairo_save(cr);

	ge_cairo_set_color(cr, color);	
	cairo_set_line_width (cr, thickness);
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);

	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);

	cairo_stroke(cr);

	cairo_restore(cr);
}
 
void
do_hc_draw_dot (cairo_t *canvas,
			CairoColor * light,
			CairoColor * dark,
			gint x,
			gint y)
{
	cairo_save(canvas);

	cairo_set_line_width (canvas, 0.5);
	cairo_set_antialias(canvas, CAIRO_ANTIALIAS_NONE);

	ge_cairo_set_color(canvas, dark);	
	cairo_rectangle (canvas, x - 1, y, 0.5, 0.5);
	cairo_rectangle (canvas, x - 1, y - 1, 0.5, 0.5);
	cairo_rectangle (canvas, x, y - 1, 0.5, 0.5);
	cairo_stroke(canvas);

	ge_cairo_set_color(canvas, light);	
	cairo_rectangle (canvas, x + 1, y, 0.5, 0.5);
	cairo_rectangle (canvas, x + 1, y + 1, 0.5, 0.5);
	cairo_rectangle (canvas, x, y + 1, 0.5, 0.5);
	cairo_stroke(canvas);

	CairoColor mid;
}
