/******************************************************************************/
/* smooth_gtk2_drawing.c - Drawing Functions for GTK2 Engine                  */
/******************************************************************************/
/* Smooth Theme Engine                                                        */
/* Copyright (C) 2002-2004 Andrew Johnson                                     */
/*                                                                            */
/* This library is free software; you can redistribute it and/or              */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 2.1 of the License, or (at your option) any later version.         */
/*                                                                            */
/* This library is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          */
/* Lesser General Public License for more details.                            */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public           */
/* License along with this library; if not, write to the Free Software        */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  */
/*                                                                            */
/* Author(s): Andrew Johnson <ajgenius@ajgenius.us>                           */
/******************************************************************************/
/* Portions Based on GTK+                                                     */
/*   Peter Mattis <petm@xcf.berkeley.edu>                                     */
/*   Spencer Kimball <spencer@xcf.berkeley.edu>                               */
/*   Josh MacDonald <jmacd@xcf.berkeley.edu>                                  */
/*                                                                            */
/* Portions Based on the EnGradient Engine                                    */
/*   Andrew Cattau                                                            */
/*                                                                            */
/* Portions Based on the IceGradient Engine                                   */
/*   Tim Gerla <timg@means.net>                                               */
/*   Tomas Ögren <stric@ing.umu.se>                                           */
/*   JM Perez <jose.perez@upcnet.es>                                          */
/*                                                                            */
/* Portions Based on the CleanIce Engine                                      */
/*   Bastien Nocera <hadess@hadess.net>                                       */
/*   Rodney Dawes                                                             */
/*                                                                            */
/* Portions Based on the ThinIce Engine                                       */
/*   Tim Gerla <timg@rrv.net>                                                 */
/*   Tomas Ögren <stric@ing.umu.se>                                           */
/*   Richard Hult <rhult@codefactory.se>                                      */
/*                                                                            */
/* Portions Based on the Wonderland Engine                                    */
/*   Garrett LeSage                                                           */
/*   Alexander Larsson                                                        */
/*   Owen Taylor <otaylor@redhat.com>                                         */
/*                                                                            */
/* Portions Based on the XFCE Engine                                          */
/*   Olivier Fourdan <fourdan@xfce.org>                                       */
/*                                                                            */
/* Portions Based on the Xenophilia Engine                                    */
/*   Johan Hanson <misagon@bahnhof.se>                                        */
/******************************************************************************/
#include "smooth_gtk2_engine.h"
#include "smooth_gtk2_drawing.h"
#include "smooth_gtk2_misc.h"
#include "smooth_gtk2_rc.h"

#define FLAT_FILL_BACKGROUND(canvas, style, state_type, part, x, y, width, height) (smooth_fill_background(canvas, style, state_type, GTK_SHADOW_NONE, part, x, y, width, height, FALSE, FALSE, FALSE,FALSE, FALSE))		
#define gradient_fill_background(canvas, style, state_type, part, x, y, width, height, invert, horizontal) (smooth_fill_background(canvas, style, state_type, GTK_SHADOW_NONE, part, x, y, width, height, TRUE, invert, horizontal,FALSE, FALSE))

static SmoothDouble default_shades_table[] = 
{
	0.666667,	/* DARK		*/
	1.2,		/* LIGHT	*/
	0.665,		/* ICED		*/
	0.4,		/* COLD		*/
	0.112		/* REDMOND	*/
};

/********************************************************************************************************/
/* THE BELOW IS WHATS LEFT OF smooth_style.c CLEAN IT UP/GET RID OF IT SOON                             */
/********************************************************************************************************/

/* This function is based on a portion of Xenophilia's xeno_draw_extension */
static gboolean
rounded_extension_points(gint x,
                         gint y, 
			 gint width, 
			 gint height,
			 gboolean selected,
			 gboolean fill,
			 GtkPositionType position,
			 GdkPoint points[8])
{
  gint x2 = x + width, y2 = y + height;
  switch (position) {
    case GTK_POS_BOTTOM:
      y2 += ((fill)?1:0) + ((selected)?0:-1);
      points[0].x = x2-1;	points[0].y = y2;
      points[1].x = x2-1;	points[1].y = y+5;
      points[2].x = x2-3;	points[2].y = y+2;
      points[3].x = x2-6;	points[3].y = y;
      points[4].x = x+5;	points[4].y = y;
      points[5].x = x+2;	points[5].y = y+2;
      points[6].x = x;		points[6].y = y+5;
      points[7].x = x;		points[7].y = y2;
      break;
	
    case GTK_POS_RIGHT:
      x2 += ((fill)?1:0) + ((selected)?0:-1);
      points[0].y = y;		points[0].x = x2;
      points[1].y = y;		points[1].x = x+5;
      points[2].y = y+2;	points[2].x = x+2;
      points[3].y = y+5;	points[3].x = x;
      points[4].y = y2-6;	points[4].x = x;
      points[5].y = y2-3;	points[5].x = x+2;
      points[6].y = y2-1;	points[6].x = x+5;
      points[7].y = y2-1;	points[7].x = x2;
      break;
			
    case GTK_POS_TOP:
      y -= ((fill)?1:0) + ((selected)?1:0);
      y2 += ((selected)?-1:0) - 1;
      points[0].x = x;		points[0].y = y;
      points[1].x = x;		points[1].y = y2-5;
      points[2].x = x+2;	points[2].y = y2-2;
      points[3].x = x+5;	points[3].y = y2;
      points[4].x = x2-6;	points[4].y = y2;
      points[5].x = x2-3;	points[5].y = y2-2;
      points[6].x = x2-1;	points[6].y = y2-5;
      points[7].x = x2-1;	points[7].y = y;
      break;
    
    case GTK_POS_LEFT:
      x -= ((fill)?1:0) + ((selected)?1:0);
      x2 += ((selected)?-1:0) - 1;
      points[0].y = y2-1;	points[0].x = x;
      points[1].y = y2-1;	points[1].x = x2-5;
      points[2].y = y2-3;	points[2].x = x2-2;
      points[3].y = y2-6;	points[3].x = x2;
      points[4].y = y+5;	points[4].x = x2;
      points[5].y = y+2;	points[5].x = x2-2;
      points[6].y = y;		points[6].x = x2-5;
      points[7].y = y;		points[7].x = x;
      break;

    default :
      return FALSE;     
  }  
  return TRUE;     
}

/* This function is based on a portion of Xenophilia's xeno_draw_extension */
static gboolean
square_extension_points(gint x,
                         gint y, 
			 gint width, 
			 gint height,
			 gboolean selected,
			 gboolean fill,
			 GtkPositionType position,
			 GdkPoint points[8])
{
  gint x2 = x + width, y2 = y + height;

  switch (position) {
    case GTK_POS_BOTTOM:
      y2 += ((fill)?1:0) + ((selected)?0:-1);
      points[0].x = x2-1;	points[0].y = y2;
      points[1].x = x2-1;	points[1].y = y+1;
      points[2].x = x2-1;	points[2].y = y;
      points[3].x = x2-2;	points[3].y = y;
      points[4].x = x+1;	points[4].y = y;
      points[5].x = x;		points[5].y = y;
      points[6].x = x;		points[6].y = y+1;
      points[7].x = x;		points[7].y = y2;
      break;
	
    case GTK_POS_RIGHT:
      x2 += ((fill)?1:0) + ((selected)?0:-1);
      points[0].y = y;		points[0].x = x2;
      points[1].y = y;		points[1].x = x+1;
      points[2].y = y;		points[2].x = x;
      points[3].y = y+1;	points[3].x = x;
      points[4].y = y2-2;	points[4].x = x;
      points[5].y = y2-1;	points[5].x = x;
      points[6].y = y2-1;	points[6].x = x+1;
      points[7].y = y2-1;	points[7].x = x2;
      break;
			
    case GTK_POS_TOP:
      y -= ((fill)?1:0) + ((selected)?1:0);
      y2 += ((selected)?-1:0) - 1;
      points[0].x = x;		points[0].y = y;
      points[1].x = x;		points[1].y = y2-1;
      points[2].x = x;		points[2].y = y2;
      points[3].x = x+1;	points[3].y = y2;
      points[4].x = x2-2;	points[4].y = y2;
      points[5].x = x2-1;	points[5].y = y2;
      points[6].x = x2-1;	points[6].y = y2-1;
      points[7].x = x2-1;	points[7].y = y;
      break;
    
    case GTK_POS_LEFT:
      x -= ((fill)?1:0) + ((selected)?1:0);
      x2 += ((selected)?-1:0) - 1;
      points[0].y = y2-1;	points[0].x = x;
      points[1].y = y2-1;	points[1].x = x2-1;
      points[2].y = y2-1;	points[2].x = x2;
      points[3].y = y2-2;	points[3].x = x2;
      points[4].y = y+1;	points[4].x = x2;
      points[5].y = y;		points[5].x = x2;
      points[6].y = y;		points[6].x = x2-1;
      points[7].y = y;		points[7].x = x;
      break;
         
    default :
      return FALSE;     
  }  
  return TRUE;     
}

/* This function is based on a portion of Xenophilia's xeno_draw_extension */
static gboolean
triangle_extension_points(gint x,
                         gint y, 
			 gint width, 
			 gint height,
			 gboolean selected,
			 gboolean fill,
			 GtkPositionType position,
			 GdkPoint points[8])
{
  gint x2 = x + width, y2 = y + height;
  gint i = 0;

  switch (position) {
    case GTK_POS_BOTTOM:
      i = (height - 5 + 2) / 3;

      if (!(i > 0)) return square_extension_points(x, y, width, height, selected, fill, position, points);

      y2 = y + i*3 + 5 + ((fill)?1:0) + ((selected)?0:-1);
         
      points[0].x = x2-1;	points[0].y = y2;
      points[1].x = x2-i-1;	points[1].y = y+4;
      points[2].x = x2-i-3;	points[2].y = y+2;
      points[3].x = x2-i-6;	points[3].y = y;
      points[4].x = x+i+5;	points[4].y = y;
      points[5].x = x+i+2;	points[5].y = y+2;
      points[6].x = x+i;	points[6].y = y+4;
      points[7].x = x;		points[7].y = y2;
      break;
	
    case GTK_POS_RIGHT:
      i = (width- 5 + 2) / 3;
      if (!(i > 0)) return square_extension_points(x, y, width, height, selected, fill, position, points);

      x2 = x + i*3 + 5 + ((fill)?1:0) + ((selected)?0:-1);
      points[0].y = y;		points[0].x = x2;
      points[1].y = y+i;	points[1].x = x+4;
      points[2].y = y+i+2;	points[2].x = x+2;
      points[3].y = y+i+5;	points[3].x = x;
      points[4].y = y2-i-6;	points[4].x = x;
      points[5].y = y2-i-3;	points[5].x = x+2;
      points[6].y = y2-i-1;	points[6].x = x+4;
      points[7].y = y2-1;	points[7].x = x2;
      break;

    case GTK_POS_TOP:
      i = (height - 5 + 2) / 3;
      if (!(i > 0)) return square_extension_points(x, y, width, height, selected, fill, position, points);

      y -= (i*3 + 5 - height) + ((fill)?1:0) + ((selected)?1:0);
      y2 += ((selected)?-1:0) - 1;
      points[0].x = x;		points[0].y = y;
      points[1].x = x+i;		points[1].y = y2-4;
      points[2].x = x+i+2;	points[2].y = y2-2;
      points[3].x = x+i+5;	points[3].y = y2;
      points[4].x = x2-i-6;	points[4].y = y2;
      points[5].x = x2-i-3;	points[5].y = y2-2;
      points[6].x = x2-i-1;	points[6].y = y2-4;
      points[7].x = x2-1;	points[7].y = y;
      break;
             
    case GTK_POS_LEFT:
      i = (width- 5 + 2) / 3;
      if (!(i > 0)) return square_extension_points(x, y, width, height, selected, fill, position, points);

      x -= (i*3 + 5 - width) + ((fill)?1:0) + ((selected)?1:0);
      x2 += ((selected)?-1:0) - 1;
      points[0].y = y2-1;	points[0].x = x;
      points[1].y = y2-i-1;	points[1].x = x2-4;
      points[2].y = y2-i-3;	points[2].x = x2-2;
      points[3].y = y2-i-6;	points[3].x = x2;
      points[4].y = y+i+5;	points[4].x = x2;
      points[5].y = y+i+2;	points[5].x = x2-2;
      points[6].y = y+i;	points[6].x = x2-4;
      points[7].y = y;		points[7].x = x;
      break;

    default :
      return square_extension_points(x, y, width, height, selected, fill, position, points);
  }  
  return TRUE;     
}

static void 
smooth_fill_background(SmoothCanvas Canvas,
			GtkStyle * style,
	               GtkStateType state_type,
		       GtkShadowType shadow_type,
		       void * part,
	               gint x,
	               gint y,
	               gint width,
	               gint height,
		       gboolean use_gradient,  
	               gboolean invert,
		       gboolean horizontal,
		       gboolean arc_fill,
		       gboolean input_widget)
{
        SmoothFill fill;	

	gint buffered=BUFFERED_FILL(style);
  
	GdkBitmap * clip_mask = NULL;
	gint fill_style;
  	
  	SmoothWidgetState widget_state = GDKSmoothWidgetState(state_type);
  	
	if (!part) part = THEME_PART(BACKGROUND_PART(style));
  
	fill_style = smooth_fill_style(style, part);  
	
	if (!use_gradient && ((fill_style == SMOOTH_FILL_STYLE_GRADIENT) 
		|| (fill_style == SMOOTH_FILL_STYLE_SHADE_GRADIENT)))
	{	
		fill_style = SMOOTH_FILL_STYLE_SOLID;
	}
	
	if (arc_fill)
	{
		clip_mask = arc_clip_mask(width+1, height+1);

		if ((fill_style == SMOOTH_FILL_STYLE_GRADIENT) || (fill_style == SMOOTH_FILL_STYLE_SHADE_GRADIENT))
		{
		        width += 3;
		        height += 3;
	        }

		GDKModifyCanvasClipMask(&Canvas, clip_mask);
		GDKModifyCanvasClipOffset(&Canvas, x, y);

		fill.Roundness = 1.0;
	}	
	else
	{
		fill.Roundness = 0.0;

		GDKModifyCanvasClipMask(&Canvas, NULL);
		GDKModifyCanvasClipOffset(&Canvas, 0, 0);
	}
	
	fill.Style = fill_style;
	switch (fill_style)
	{
		case SMOOTH_FILL_STYLE_TILE :		
			fill.Tile.ImageFile = smooth_fill_file_name(style, part, state_type);
			fill.Tile.XOffset = 0;
			fill.Tile.YOffset = 0;
			fill.Tile.UseBuffer = !buffered;
		break;

		case SMOOTH_FILL_STYLE_GRADIENT :
			fill.Gradient.Type = smooth_fill_gradient_direction(style, part, horizontal);
			fill.Gradient.QuadraticGradientRange = smooth_fill_quadratic_gradient(style, part);

		 	fill.Gradient.From = smooth_fill_color1(style, part, state_type);

		 	fill.Gradient.To = smooth_fill_color2(style, part, state_type);
		break;	

		case SMOOTH_FILL_STYLE_SHADE_GRADIENT :
			fill.Gradient.Type = smooth_fill_gradient_direction(style, part, horizontal);
			fill.Gradient.QuadraticGradientRange = smooth_fill_quadratic_gradient(style, part);

			fill.ColorShade.From = smooth_fill_shade1_value(style, part);
			fill.ColorShade.To = smooth_fill_shade2_value(style, part);

		 	fill.BaseColor = COLOR_CUBE(style).Interaction[widget_state].Background;
		break;	
		
		default:
		{
			fill.Style = SMOOTH_FILL_STYLE_SOLID;

			if (!input_widget)
			{
			 	fill.BaseColor = COLOR_CUBE(style).Interaction[widget_state].Background;
			}
			else
			{
			 	fill.BaseColor = COLOR_CUBE(style).Input[widget_state].Background;
			}
		}
	}
	
	{
		int dither_depth = THEME_DATA(style)->fill.default_dither_depth;

		if ((THEME_PART(part)->use_fill) && (THEME_PART(part)->fill.use_dither_depth[state_type]))
			dither_depth = THEME_PART(part)->fill.dither_depth[state_type];
		else
			if ((THEME_PART(part)->use_fill) && (THEME_PART(part)->fill.default_dither_depth_set))
				dither_depth = THEME_PART(part)->fill.default_dither_depth;
		else
			if (THEME_DATA(style)->fill.use_dither_depth[state_type])
				dither_depth = THEME_DATA(style)->fill.dither_depth[state_type];
		
		GDKModifyCanvasDitherDepth(&Canvas, dither_depth);
	}
	
	SmoothDrawFill(&fill, Canvas, x, y, width, height);
	
	GDKModifyCanvasClipMask(&Canvas, NULL);
	GDKModifyCanvasClipOffset(&Canvas, 0, 0);

	if (clip_mask)
		clip_mask_unref(clip_mask);
}

static void
smooth_draw_grip(SmoothCanvas Canvas,

                GtkStyle * style,
                 GtkStateType state_type,

                SmoothInt X,
                SmoothInt Y,
                SmoothInt Width,
                SmoothInt Height,
                SmoothBool Horizontal)
{
       SmoothColor base, white, darktone, lighttone, midtone;
       SmoothColor color1, color2, color3;

       smooth_grip_style * grip = GRIP_PART(style);

       if (PART_STYLE(grip) == NO_GRIP)
               return;

       base = COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;

       SmoothCanvasCacheColor(Canvas, &base);
       SmoothCanvasCacheShadedColor(Canvas, base, default_shades_table[0], &darktone);
       SmoothCanvasCacheShadedColor(Canvas, base, default_shades_table[1], &lighttone);
       SmoothCanvasCacheMidPointColor(Canvas, darktone, lighttone, &midtone);

       switch PART_STYLE(grip)
       {
               case BARS_IN_GRIP :
               case BARS_OUT_GRIP :
               {
                       if (LINE_STYLE(style, grip) == SMOOTH_BEVEL_STYLE_FLAT)
                       {
                               color1=darktone;

                               color2=darktone;
                               SmoothColorSetAlphaValue(&color2, 0);

                               color3=darktone;
                               SmoothColorSetAlphaValue(&color3, 0);
                       }
                       else
                       {
                               color2 = lighttone;

                               if (LINE_STYLE(style, grip) == SMOOTH_BEVEL_STYLE_SOFT)
                               {
                                       color1 = midtone;
                                       color3 = base;
                               }
                               else
                               {
                                       color1 = darktone;
                                       color3 = midtone;
                               }
                       }

                       if (PART_STYLE(grip)==BARS_OUT_GRIP)
                       {
                               SmoothColor tmp = color2;
                               color2 = color1;
                               color1 = tmp;
                       }

                       do_smooth_draw_broken_bars(Canvas, color1, color2, color3, X + 2, Y + 2, Width - 4, Height - 4, Horizontal);
               }
               break;

               case LINES_IN_GRIP :
               case LINES_OUT_GRIP :
               case FIXEDLINES_IN_GRIP :
               case FIXEDLINES_OUT_GRIP :
               case SLASHES_GRIP :
               {
                       SmoothGripObjects sgo;

                       sgo.Count = GRIP_COUNT(style);
                       sgo.Spacing = GRIP_SPACING(style);
                       sgo.Angle = (PART_STYLE(grip) == SLASHES_GRIP)?45:0;
                       sgo.Roundness = 0;

                       sgo.Alternating = 0;
                       sgo.CutOff = EDGE_LINE_THICKNESS(style, grip) + 1;

                       sgo.Size = (((Horizontal)?Height:Width) >> 1) - 1;

                       if ((PART_STYLE(grip)==LINES_OUT_GRIP) || (PART_STYLE(grip)==LINES_IN_GRIP))
                       {
                               if (Horizontal)
                               {
                                       sgo.Count = (Width - ((Width % (sgo.Spacing))))/(sgo.Spacing + 2) - 1;
                               }
                               else
                               {
                                       sgo.Count = (Height - ((Height % (sgo.Spacing))))/(sgo.Spacing + 2) - 1;
                               }
                       }

                       if (LINE_STYLE(style, grip) == SMOOTH_BEVEL_STYLE_FLAT)
                       {
                               color1=darktone;
                               color2=darktone;
                               SmoothColorSetAlphaValue(&color2, 0.0);
                       }
                       else
                       {
                               color1 = lighttone;

                               if (LINE_STYLE(style, grip) == SMOOTH_BEVEL_STYLE_SOFT)
                               {
                                       color2 = midtone;
                               }
                               else
                               {
                                       color2 = darktone;
                               }
                       }

                       if ((PART_STYLE(grip)==FIXEDLINES_OUT_GRIP) || (PART_STYLE(grip)==LINES_OUT_GRIP))
                       {
                               SmoothColor tmp = color2;
                               color2 = color1;
                               color1 = tmp;
                       }

                       do_smooth_draw_lines(&sgo, Canvas, color1, color2, X, Y, Width, Height, Horizontal);
               }
               break;

               case AUTODOTS_IN_GRIP :
               case AUTODOTS_OUT_GRIP :
               case DOTS_IN_GRIP :
               case DOTS_OUT_GRIP :
               case SMALLDOTS_IN_GRIP :
               case SMALLDOTS_OUT_GRIP :
               {
                       SmoothGripObjects sgo;

                       sgo.Count = GRIP_COUNT(style);
                       sgo.Spacing = GRIP_SPACING(style);

                       sgo.Angle = 0;
                       sgo.Roundness = 0;

                       sgo.Alternating = 0;
                       sgo.CutOff = EDGE_LINE_THICKNESS(style, grip) + 1;

                       if ((PART_STYLE(grip) == SMALLDOTS_IN_GRIP) || (PART_STYLE(grip) == SMALLDOTS_OUT_GRIP))
                       {
                               sgo.Size = 2;
                       }
                       else
                       {
                               sgo.Size = 3;
                       }

                       if ((PART_STYLE(grip)==AUTODOTS_OUT_GRIP) || (PART_STYLE(grip)==AUTODOTS_IN_GRIP))
                       {
                               if (Horizontal)
                               {
                                       sgo.Count = (Width - ((Width % (sgo.Spacing))))/(sgo.Spacing + 2) - 1;
                               }
                               else
                               {
                                       sgo.Count = (Height - ((Height % (sgo.Spacing))))/(sgo.Spacing + 2) - 1;
                               }
                       }

                       if (LINE_STYLE(style, grip) == SMOOTH_BEVEL_STYLE_FLAT)
                       {
                               color1=darktone;

                               color2=darktone;
                               SmoothColorSetAlphaValue(&color2, 0);

                               color3=darktone;
                               SmoothColorSetAlphaValue(&color3, 0);
                       }
                       else
                       {
                               color2 = lighttone;

                               if (LINE_STYLE(style, grip) == SMOOTH_BEVEL_STYLE_SOFT)
                               {
                                       color1 = midtone;
                                       color3 = base;
                               }
                               else
                               {
                                       color1 = darktone;
                                       color3 = midtone;
                               }
                       }

                       if ((PART_STYLE(grip)==DOTS_OUT_GRIP) || (PART_STYLE(grip)==SMALLDOTS_OUT_GRIP))
                       {
                               SmoothColor tmp = color2;

                               color2 = color1;
                               color1 = tmp;

                               if (Horizontal)
                                       Y -= 1;
                               else
                                       X -= 1;
                       }

                       do_smooth_draw_dots(&sgo, Canvas, color1, color2, color3, X, Y, Width, Height, Horizontal);
               }
               break;

               case MAC_BUDS_IN_GRIP :
               case MAC_BUDS_OUT_GRIP :
               case NS_BUDS_IN_GRIP :
               case NS_BUDS_OUT_GRIP :
               {
                       SmoothColorSetValues(&white, 1.0, 1.0, 1.0, 1.0);

                       SmoothCanvasCacheColor(Canvas, &white);

                       if (LINE_STYLE(style, grip) == SMOOTH_BEVEL_STYLE_FLAT)
                       {
                               color1=darktone;

                               color2=darktone;
                               SmoothColorSetAlphaValue(&color2, 0);

                               color3=darktone;
                               SmoothColorSetAlphaValue(&color3, 0);
                       }
                       else
                       {
                               if ((PART_STYLE(grip) == MAC_BUDS_OUT_GRIP) || (PART_STYLE(grip) == NS_BUDS_OUT_GRIP))
                                       color1 = white;
                               else
                                       color1 = lighttone;

                               if (LINE_STYLE(style, grip) == SMOOTH_BEVEL_STYLE_SOFT)
                               {
                                       color2 = midtone;
                                       color3 = base;
                                       SmoothColorSetAlphaValue(&color3, 0);
                               }
                               else
                               {
                                       color2 = darktone;
                                       color3 = midtone;
                               }
                       }

                       if ((PART_STYLE(grip) == MAC_BUDS_IN_GRIP) || (PART_STYLE(grip) == NS_BUDS_IN_GRIP))
                       {
                               SmoothColor tmp = color2;
                               color2 = color1;
                               color1 = tmp;
                       }
                       else
                       {
                               if (Horizontal)
                                       Y -= 1;
                               else
                                       X -= 1;
                       }


                       do_smooth_draw_buds(Canvas, color1, color2, color3,

                                       X, Y, Width, Height,

                                       EDGE_LINE_THICKNESS(style, grip), Horizontal,

                                       ((PART_STYLE(grip) == NS_BUDS_OUT_GRIP) || (PART_STYLE(grip) == NS_BUDS_IN_GRIP)));

                       SmoothCanvasUnCacheColor(Canvas, &white);
               }
               break;

               default:
               break;
       }

       SmoothCanvasUnCacheMidPointColor(Canvas, darktone, lighttone, &midtone);
       SmoothCanvasUnCacheShadedColor(Canvas, base, default_shades_table[1], &lighttone);
       SmoothCanvasUnCacheShadedColor(Canvas, base, default_shades_table[0], &darktone);
       SmoothCanvasUnCacheColor(Canvas, &base);
}

static void
smooth_draw_slider_grip(SmoothCanvas Canvas,

	 		   GtkStyle *style,
		           GtkStateType state_type,

		           SmoothInt X,
		           SmoothInt Y,
		           SmoothInt Width,
		           SmoothInt Height,
	
		           SmoothBool Horizontal)
{
	SmoothBool BAIL = FALSE;
	smooth_grip_style *grip = GRIP_PART(style);
 
	switch (PART_STYLE(grip))
	{
		case NO_GRIP:
		break;

		case BARS_IN_GRIP:
		case BARS_OUT_GRIP:
		{
			SmoothInt w = Width, h = Height;

			if (Horizontal) 
			{
				h = MIN(Height, Width + Height/7);
				Y = Y + (Height - h)/2;
	
				Horizontal = TRUE;
			} 
			else 
			{
				w = MIN(Width, Width/7 + Height);
				X = X + (Width - w)/2;
	
				Horizontal = FALSE;
			}
		
			Width = w;
			Height = h;

			/* too small no bars */
			if ((Width <= 13) && (Height <= 11))
				BAIL = TRUE;
		} 
		break;

		case LINES_IN_GRIP:
		case LINES_OUT_GRIP:
		{
			SmoothInt w = Width, h = Height;

			if (Horizontal) 
			{
				h = MIN(Height, Width + Height/7);
				Y = Y + (Height - h)/2;
			} 
			else 
			{
				w = MIN(Width, Width/7 + Height);
				X = X + (Width - w)/2;
			}
		
			Width = w;
			Height = h;

			/* too small no lines */
			if ((Width <= 13) && (Height <= 11))
				BAIL = TRUE;
		} 
		break;

		case FIXEDLINES_IN_GRIP:
		case FIXEDLINES_OUT_GRIP:
		{
			/* too small no midlines */
			if (((Width <= 13) && (Horizontal)) || ((Height <= 13) && (!Horizontal)))
				BAIL = TRUE;
		}
		break;

		case SLASHES_GRIP:
		{
			if (((Width <= 13) && (Horizontal)) || ((Height <= 13) && (!Horizontal)))
				BAIL = TRUE;
		}
		break;

		case DOTS_IN_GRIP:
		case SMALLDOTS_IN_GRIP :
		case AUTODOTS_IN_GRIP :
		{
			if ((Width <= 7) && (Height <= 7))
				BAIL = TRUE;
		}
		break;

		case DOTS_OUT_GRIP:
		case SMALLDOTS_OUT_GRIP :
		case AUTODOTS_OUT_GRIP :
		{
			if (Horizontal)
				Y += 1;
			else
				X += 1;

			if ((Width <= 7) && (Height <= 7))
				BAIL = TRUE;
		}
		break;

		case MAC_BUDS_IN_GRIP:
		case MAC_BUDS_OUT_GRIP:
		case NS_BUDS_IN_GRIP:
		case NS_BUDS_OUT_GRIP:
		{
			SmoothBool ns = ((PART_STYLE(grip) == NS_BUDS_OUT_GRIP) || (PART_STYLE(grip) == NS_BUDS_IN_GRIP));
			SmoothInt w = Width, h = Height;

			if (!Horizontal) 
			{
				h = MIN(Height, Width + Height/7);

				X += 0;
				Y += (Height - h)/2 + 3 + (gint)(ns);
				
				Width = w;
				Height = h - 6;
			} 
			else 
			{
				w = MIN(Width, Width/7 + Height);

				X += (Width - w)/2 + 2 + (gint)(ns);
				Y += 1;
				Width = w - 4;
				Height = h;
			}

			Horizontal = !Horizontal;
			
			/* too small no buds */
			if ((Width <= 13) && (Height <= 11))
				BAIL = TRUE;
		}
		break;

		default :
			BAIL = TRUE;
	}	  

	if (!BAIL)
		smooth_draw_grip(Canvas, style, state_type, X+PART_XPADDING(grip), Y+PART_YPADDING(grip), 
					Width-PART_XPADDING(grip)*2, Height-PART_YPADDING(grip)*2, Horizontal);      
}

static void
do_smooth_draw_shadow(SmoothCanvas Canvas,

			GtkStyle * style,
			GtkStateType state_type,
			GtkShadowType shadow_type,
			
			GtkWidget * widget,
			const gchar * detail,

			SmoothInt x,
			SmoothInt y,
			SmoothInt width,
			SmoothInt height)
{
	SmoothBorder border;
	SmoothColor base;
	smooth_part_style *part = NULL;
	SmoothDouble roundness = 0.0;

	if (shadow_type == GTK_SHADOW_NONE)
		return;

 	base = COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;
	
	if (CHECK_DETAIL(detail, "smooth_trough"))
		part = THEME_PART(TROUGH_PART(style));	
	else if (CHECK_DETAIL(detail, "smooth_slider"))
		part=THEME_PART(GRIP_PART(style));
        else if (CHECK_DETAIL(detail, "smooth_stepper"))
		part = THEME_PART(STEPPER_PART(style));
        else if (CHECK_DETAIL(detail, "smooth_button"))
		part = smooth_button_part(style, FALSE);

	smooth_style_get_border(style, state_type, shadow_type, detail, part, roundness, &border);

	if (CHECK_DETAIL(detail, "menubar") && GRIP_OVERLAP_TOOLBAR(style) && IS_BONOBO_DOCK_ITEM(widget))
	{
		gint thick = EDGE_LINE_THICKNESS(style, NULL);

		SmoothDrawBorderWithGap(&border, Canvas, base, x-thick, y, width+thick, height, GTK_POS_LEFT, 0, height);
	}
	else if (((CHECK_DETAIL (detail, "entry"))
		|| (CHECK_DETAIL (detail, "frame"))) &&
		(widget && (!GTK_IS_BUTTON(widget)) && 
                ((ge_is_in_combo_box (widget) || IS_SPIN_BUTTON (widget)) && 
                 (ENTRY_BUTTON_EMBED(style)))))
	{
		/* The Combo/ComboBoxEntry button and the SpingButton Steppers should apear
		 * to be inset into the entry, as opposed to next to it, so we fake it
		 * by drawing an extra 2 pixels here to ignore the right(or left)
		 * edge, which will be drawn by the buttons.
		 */
		smooth_part_style *part = smooth_button_part(style, FALSE);
		SmoothInt thick = 2;

		thick = EDGE_LINE_THICKNESS(style, part);
		
		if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
		{
			SmoothDrawBorderWithGap(&border, Canvas, base, x, y, width+thick, height, GTK_POS_RIGHT, 0, height);
		}
		else
		{
			SmoothDrawBorderWithGap(&border, Canvas, base, x-thick, y, width+thick, height, GTK_POS_LEFT, 0, height);
		}
	}
	else if (CHECK_DETAIL(detail, "toolbar") && GRIP_OVERLAP_TOOLBAR(style) && (ge_is_toolbar_item(widget)))
	{
		gboolean horiz=TRUE;/*(orientation==GTK_ORIENTATION_HORIZONTAL); FIXME*/

		gint thick = EDGE_LINE_THICKNESS(style, NULL);  
	
		if (horiz)
			SmoothDrawBorderWithGap(&border, Canvas, base, x-thick, y, width+thick, height, GTK_POS_LEFT, 0, height);
		else
			SmoothDrawBorderWithGap(&border, Canvas, base, x, y-thick, width, height+thick, GTK_POS_TOP, 0, width);
	}
	else 
		SmoothDrawBorderWithGap(&border, Canvas, base, x, y, width, height, 0, 0, 0);
}

static void
do_smooth_draw_focus(SmoothCanvas Canvas,
           GtkStyle *style,
  	   GtkStateType state_type,
           GtkWidget *widget,
           const gchar *detail,
           SmoothInt x,
           SmoothInt y,
           SmoothInt width,
           SmoothInt height)
{
	gboolean free_dash_list = FALSE;
	gchar *dash_list = "";

	SmoothColor color;

	SmoothLinePattern dash_pattern;  
	SmoothInt line_width = 1;
	SmoothBool interior_focus = TRUE;

	/* Setup Pen Color, Line Pattern, & Width */
	{
		/* Get Style's Focus Color */
		{
			if (FOCUS_USE_FOREGROUND(style, state_type)) 
			{
				color = FOCUS_FOREGROUND(style, GDKSmoothWidgetState(state_type));
			} 
			else
			{
				color = COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Foreground;
			}
	
			SmoothCanvasCacheColor(Canvas, &color);		
		}

		/* Check for a Smooth Focus Part Line Width and Pattern */
		if (FOCUS_USE_PATTERN(style, state_type))
		{
			dash_pattern.Pattern = FOCUS_PATTERN(style, state_type);
		}
		else 
		{
			/* Fallback on GTK's Line Pattern */
			if (widget)
			{
				gtk_widget_style_get (widget, "focus-line-pattern", &dash_list, NULL);
		
				free_dash_list = TRUE;
			}

			if (dash_list) 
			{
				dash_pattern.Pattern = dash_list;
			} 
    		}

		if (FOCUS_USE_LINE_WIDTH(style, state_type))
		{
			line_width = FOCUS_LINE_WIDTH(style, state_type);
		}
		else 
		{
			/* Fallback on GTK's Line Width and Pattern */
			if (widget)
			{
				gtk_widget_style_get (widget, "focus-line-width", &line_width, NULL);
			}
    		}

		if (CHECK_DETAIL(detail, "add-mode"))
		{
			dash_pattern.Pattern = "\4\4";
		}

		SmoothCanvasSetPenValues(Canvas, color, line_width, 
						dash_pattern.Pattern[0] ? GDK_LINE_ON_OFF_DASH : GDK_LINE_SOLID,
						GDK_JOIN_MITER, GDK_CAP_NOT_LAST);
	}

	dash_pattern.Length = strlen(dash_pattern.Pattern);

	if (dash_pattern.Pattern[0])
		SmoothCanvasSetPenPattern(Canvas, dash_pattern);
	
	if (widget)
	{
		gtk_widget_get_focus_props (widget, NULL, NULL, &interior_focus);
	}

 	if (CHECK_DETAIL(detail, "button") && (ge_is_in_combo_box (widget) && (ENTRY_BUTTON_EMBED(style))))
	{
		smooth_part_style *part = smooth_button_part(style, FALSE);
		SmoothInt thick = 2;

		if (!part) part = THEME_PART(BACKGROUND_PART(style));

		thick = EDGE_LINE_THICKNESS(style, part);

		if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
		{
			if (!interior_focus)
			{
				SmoothCanvasFrameRectangle(Canvas, x - thick + line_width*2, y + thick, width - line_width*2, height - thick*2);
			}
			else
			{
				SmoothCanvasFrameRectangle(Canvas, x - thick + line_width, y + thick, width, height - thick*2);
			}
		}
		else
		{
			if (!interior_focus)
			{
				SmoothCanvasFrameRectangle(Canvas, x + thick, y + thick, width - line_width*2, height - thick*2);
			}
			else
			{
				SmoothCanvasFrameRectangle(Canvas, x + thick, y + thick, width - line_width, height - thick*2);
			}
		}
	}
 	else if (CHECK_DETAIL(detail, "entry") && (IS_SPIN_BUTTON(widget) && (ENTRY_BUTTON_EMBED(style))) && (!interior_focus))
	{
		if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
		{
			SmoothCanvasFrameRectangle(Canvas, x, y, width + line_width, height);
		}
		else
		{
			SmoothCanvasFrameRectangle(Canvas, x - line_width, y, width + line_width, height);
		}
	}
 	else if (CHECK_DETAIL(detail, "entry") && (ge_is_in_combo_box (widget) && (ENTRY_BUTTON_EMBED(style))) && (!interior_focus))
	{
		GtkWidget *button = g_object_get_data(G_OBJECT(ge_find_combo_box_widget_parent (widget)), "button");

		if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
		{
			SmoothCanvasFrameRectangle(Canvas,x, y, width + line_width, height);
		}
		else
		{
			SmoothCanvasFrameRectangle(Canvas,x - line_width , y, width + line_width, height);
		}

		if (GTK_IS_WIDGET(button))
		{
			gtk_widget_queue_draw_area(button, button->allocation.x, button->allocation.y, 
							button->allocation.width, button->allocation.height);
		}
	}
        else if (CHECK_DETAIL(detail, "spinbutton_up"))
	{
		SmoothRectangle clip;
		if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
		{
			SmoothRectangleSetValues(&clip, x + line_width, y, width - line_width, height);
		}
		else
		{
			SmoothRectangleSetValues(&clip, x, y, width - line_width, height);
		}

		SmoothCanvasSetClipRectangle(Canvas, clip);
		SmoothCanvasFrameRectangle(Canvas, 
						x, y, 
						width, height + line_width);
		SmoothCanvasClearClipRectangle(Canvas);
	} 
        else if (CHECK_DETAIL(detail, "spinbutton_down"))
	{
		SmoothRectangle clip;
		if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
		{
			SmoothRectangleSetValues(&clip, x + line_width, y, width - line_width, height);
		}
		else
		{
			SmoothRectangleSetValues(&clip, x, y, width - line_width, height);
		}		

		SmoothCanvasSetClipRectangle(Canvas, clip);
		SmoothCanvasFrameRectangle(Canvas, 
						x, y - line_width, 
						width, height + line_width);
		SmoothCanvasClearClipRectangle(Canvas);
	} 
        else if (CHECK_DETAIL(detail, "combo_entry_button"))
	{
		SmoothRectangle clip;

		if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
		{
			SmoothRectangleSetValues(&clip, x + line_width, y, width - line_width, height + 1);
			SmoothCanvasSetClipRectangle(Canvas, clip);
			SmoothCanvasFrameRectangle(Canvas, x, y, width, height);
		}
		else
		{
			SmoothRectangleSetValues(&clip, x, y, width, height + 1);
			SmoothCanvasSetClipRectangle(Canvas, clip);
			SmoothCanvasFrameRectangle(Canvas, x, y, width + line_width, height);
		}

		SmoothCanvasClearClipRectangle(Canvas);
	} else
	{
  		SmoothCanvasFrameRectangle(Canvas, x, y, width, height);
	}

	SmoothCanvasUnCacheColor(Canvas, &color);		

	if (free_dash_list)
		g_free (dash_list);
}

static void
smooth_draw_combobox_button (SmoothCanvas Canvas,

		GtkStyle * style,
		GtkStateType state_type,
		GtkShadowType shadow_type,

		GtkWidget * widget,
		const gchar * detail,

		SmoothInt x,
		SmoothInt y,
		SmoothInt width,
		SmoothInt height)
{
  /* The Combo/ComboBoxEntry button should apear to be inset into the entry, 
   * as opposed to next to it, so we fake it by drawing an entry fill
   * then an entry border - but 2 pixels bigger so it overlaps real border
   * and so left( or right) edge doesn't actually get drawn on screen;
   * and THEN draw the button, but essentially 4 pixels smaller.
   */
  GtkStyle * parent_style = style;
  GtkStateType parent_state = state_type;
  smooth_part_style *part = smooth_button_part(style, FALSE);
  SmoothInt thick = 2;
  gboolean entry_focused = FALSE;

  SmoothInt focus_line_width = 0, focus_padding = 0, focus = 0;
  SmoothBool interior_focus = FALSE;

  if (!part) part = THEME_PART(BACKGROUND_PART(style));

  thick = EDGE_LINE_THICKNESS(style, part);

  if (EDGE_LINE_STYLE(style, part) == SMOOTH_BEVEL_STYLE_FLAT)
    thick = 0;
  else if (EDGE_LINE_STYLE(style, part) == SMOOTH_BEVEL_STYLE_THIN)
    thick = 1;
  else if (EDGE_LINE_STYLE(style, part) == SMOOTH_BEVEL_STYLE_ICED)
    thick = 1;

  g_object_set_data(G_OBJECT(ge_find_combo_box_widget_parent(widget)), "button", widget);

	if (GTK_WIDGET_HAS_FOCUS(widget))
	{
		gtk_widget_get_focus_props (widget, &focus_line_width, &focus_padding, &interior_focus);
		if (!interior_focus)
			focus = -(focus_line_width);
		else
			focus_padding = 0;
	}
	else
	{
		GList *child=NULL,*children = gtk_container_get_children (GTK_CONTAINER(widget->parent));

		for (child = children; child; child = child->next)
		{
			if (GTK_IS_ENTRY(child->data) && GTK_WIDGET_HAS_FOCUS(child->data))
			{
				gtk_widget_get_focus_props (GTK_WIDGET(child->data), &focus_line_width, &focus_padding, &interior_focus);
				
				if (!interior_focus)
				{
					focus = focus_line_width;
					entry_focused = TRUE;
				}
				focus_padding = 0;
			}
		}
		g_list_free(children);
	}

  if ((!(widget)) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
    {
      if (ge_is_combo_box_entry (widget)
          || ge_is_combo_box (widget, TRUE))
        {
          if (!ge_is_combo_box_entry (widget))
            {
              if ((widget->parent))
                {
                  gtk_widget_ensure_style(widget->parent);

                  parent_style = widget->parent->style;
                  parent_state = widget->parent->state;
                }

	      if (parent_state != GTK_STATE_INSENSITIVE)
                parent_state = GTK_STATE_NORMAL;

              SmoothCanvasSetPenColor(Canvas, (COLOR_CUBE(parent_style).Input[GDKSmoothWidgetState(parent_state)].Background));

              SmoothCanvasFillRectangle(Canvas, x - thick*2 - focus - focus_padding*2, y + focus - focus_padding, 
					width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
            }
          else
            {
	      if (parent_state != GTK_STATE_INSENSITIVE)
                parent_state = GTK_STATE_NORMAL;

              SmoothCanvasSetPenColor(Canvas, (COLOR_CUBE(style).Input[GDKSmoothWidgetState(parent_state)].Background));

              SmoothCanvasFillRectangle(Canvas, x - thick*2 - focus - focus_padding, y + focus - focus_padding, 
					width + thick*2 - focus + focus_padding*2, height - focus*2  + focus_padding*2);
            }
 
          do_smooth_draw_shadow(Canvas, parent_style, parent_state, GTK_SHADOW_IN, widget, "entry", 
				x - thick*2 - focus - focus_padding, y + focus - focus_padding, 
				width + thick*2 + focus_padding*2, height - focus*2 + focus_padding*2);
        }
      else if (ge_is_combo(widget))
        {
          GtkWidget *entry = widget;
          SmoothRectangle clip;

          if (GTK_IS_WIDGET(widget->parent))
            {
               entry = widget->parent;
               gtk_widget_ensure_style(entry);

               parent_style = entry->style;
               parent_state = entry->state;
            }
 
	  if (parent_state != GTK_STATE_INSENSITIVE)
            parent_state = GTK_STATE_NORMAL;

          SmoothRectangleSetValues(&clip, x - thick, y, width + thick, height);
          SmoothCanvasSetClipRectangle(Canvas, clip);

          SmoothCanvasSetPenColor(Canvas, (COLOR_CUBE(style).Input[GDKSmoothWidgetState(state_type)].Background));
	  SmoothCanvasFillRectangle(Canvas, x - thick*2 - focus - focus_padding, y + focus - focus_padding, 
				    width + thick*2 - focus + focus_padding*2, height - focus*2  + focus_padding*2);

          do_smooth_draw_shadow(Canvas, parent_style, parent_state, GTK_SHADOW_IN, widget, "entry", 
				x - thick*2 - focus - focus_padding, y + focus - focus_padding, 
				width + thick*2 + focus_padding*2, height - focus*2 + focus_padding*2);
        }
      else
        {
          GtkWidget *parent = widget;
      
          if (widget->parent)
            parent = widget->parent;
        
          if ((parent))
            {
              gtk_widget_ensure_style(parent);

              parent_style = parent->style;
              parent_state = parent->state;
            }

          if (parent_state != GTK_STATE_INSENSITIVE)
            parent_state = GTK_STATE_NORMAL;

          SmoothCanvasSetPenColor(Canvas, (COLOR_CUBE(style).Input[GDKSmoothWidgetState(state_type)].Background));
	  SmoothCanvasFillRectangle(Canvas, x - thick*2 - focus - focus_padding, y + focus - focus_padding, 
				    width + thick*2 - focus + focus_padding*2, height - focus*2  + focus_padding*2);

          do_smooth_draw_shadow(Canvas, parent_style, parent_state, GTK_SHADOW_IN, widget, "entry", 
				x - thick*2 - focus - focus_padding, y + focus - focus_padding, 
				width + thick*2 + focus_padding*2, height - focus*2 + focus_padding*2);
        }

      if (GTK_WIDGET_HAS_FOCUS(widget) && (!interior_focus))
      {
        focus = -focus_padding;
      }

      smooth_fill_background(Canvas, style, state_type, shadow_type, part, x + focus, y + thick + focus,
                             width - thick - focus*2, height - thick*2 - focus*2, 
                             TRUE, shadow_type == GTK_SHADOW_IN, TRUE, FALSE, FALSE);

      do_smooth_draw_shadow(Canvas, style, state_type, shadow_type, widget, detail, 
                            x + focus, y + thick + focus, width - thick - focus*2, height - 2*thick - focus*2);

      if (entry_focused)
      {
        do_smooth_draw_focus(Canvas, style, state_type, widget, "combo_entry_button", x - thick, y, width + thick, height);
      }
    } 
  else
    {
      if (ge_is_combo_box_entry (widget)
          || ge_is_combo_box (widget, TRUE))
        {
          if (!ge_is_combo_box_entry (widget))
            {
              if ((widget->parent))
                {
                  gtk_widget_ensure_style(widget->parent);

                  parent_style = widget->parent->style;
                  parent_state = widget->parent->state;
                }

              if (parent_state != GTK_STATE_INSENSITIVE)
                parent_state = GTK_STATE_NORMAL;

              SmoothCanvasSetPenColor(Canvas, (COLOR_CUBE(parent_style).Input[GDKSmoothWidgetState(parent_state)].Background));
              SmoothCanvasFillRectangle(Canvas, x + focus - focus_padding, y + focus - focus_padding, 
                                        width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
            }
          else
            {
              if (parent_state != GTK_STATE_INSENSITIVE)
                parent_state = GTK_STATE_NORMAL;

              SmoothCanvasSetPenColor(Canvas, (COLOR_CUBE(style).Input[GDKSmoothWidgetState(parent_state)].Background));
              SmoothCanvasFillRectangle(Canvas, x + focus - focus_padding, y + focus - focus_padding, 
                                        width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
            }

          do_smooth_draw_shadow(Canvas, parent_style, parent_state, GTK_SHADOW_IN, widget, "entry", 
                                x + focus - focus_padding, y + focus - focus_padding, 
                                width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
        }
      else if (ge_is_combo(widget))
        {
          GtkWidget *entry = widget;

          if (GTK_IS_WIDGET(widget->parent))
            {
               entry = widget->parent;
               gtk_widget_ensure_style(entry);

               parent_style = entry->style;
               parent_state = entry->state;
            }
  
           if (parent_state != GTK_STATE_INSENSITIVE)
             parent_state = GTK_STATE_NORMAL;

          SmoothCanvasSetPenColor(Canvas, (COLOR_CUBE(parent_style).Input[GDKSmoothWidgetState(parent_state)].Background));
          SmoothCanvasFillRectangle(Canvas, x + focus - focus_padding, y + focus - focus_padding, 
                                    width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);

          do_smooth_draw_shadow(Canvas, parent_style, parent_state, GTK_SHADOW_IN, widget, "entry", 
                                x + focus - focus_padding, y + focus - focus_padding, 
                                width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
        }
      else
        {
          GtkWidget *parent = widget;
      
          if (widget->parent)
            parent = widget->parent;
        
          if ((parent))
            {
              gtk_widget_ensure_style(parent);

              parent_style = parent->style;
              parent_state = parent->state;
            }

          if (parent_state != GTK_STATE_INSENSITIVE)
            parent_state = GTK_STATE_NORMAL;

          SmoothCanvasSetPenColor(Canvas, (COLOR_CUBE(parent_style).Input[GDKSmoothWidgetState(parent_state)].Background));
          SmoothCanvasFillRectangle(Canvas, x + focus - focus_padding, y + focus - focus_padding, 
                                    width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);

          do_smooth_draw_shadow(Canvas, parent_style, parent_state, GTK_SHADOW_IN, widget, "entry", 
                                x + focus - focus_padding, y + focus - focus_padding, 
                                width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
        }

      if (GTK_WIDGET_HAS_FOCUS(widget) && (!interior_focus))
      {
        focus = -focus_padding;
      }

      smooth_fill_background(Canvas, style, state_type, shadow_type, part, x + thick + focus, y + thick + focus,
                             width - thick - focus*2, height - 2*thick - focus*2 , 
                             TRUE, shadow_type == GTK_SHADOW_IN, TRUE, FALSE, FALSE);
 
      do_smooth_draw_shadow(Canvas, style, state_type, shadow_type, widget, detail, 
                            x + thick + focus, y + thick + focus, width - thick - focus*2, height - 2*thick - focus*2);

      if (entry_focused)
      {
        do_smooth_draw_focus(Canvas, style, state_type, widget, "combo_entry_button", x, y, width, height);
      }
    }
}

static void
smooth_draw_spinbutton_stepper (SmoothCanvas Canvas,

		GtkStyle * style,
		GtkStateType state_type,
		GtkShadowType shadow_type,

		GtkWidget * widget,
		const gchar * detail,

		SmoothInt x,
		SmoothInt y,
		SmoothInt width,
		SmoothInt height)
{
  /* The SpinButton Steppers should apear to be inset into the entry, 
   * as opposed to next to it, so we fake it by drawing en entry fill
   * and then entry border - but 2 pixels bigger so it overlaps real border
   * and so left( or right) edge doesn't actually get drawn on screen -
   * and THEN draw the Stepper. Since we have two steppers this is slightly
   * more complicated because we can only do half for each stepper.
   */

  SmoothWidgetState parent_smooth_state = SMOOTH_STATE_DEFAULT;
  GtkStateType parent_state = GTK_STATE_NORMAL;
  SmoothRectangle spin_area;
  smooth_part_style *part = NULL;
  SmoothInt thick = 2;
  gboolean entry_focused = FALSE;
  SmoothInt focus_line_width = 0, focus_padding = 0, focus = 0;
  SmoothBool interior_focus = TRUE;

  SmoothRectangleSetValues(&spin_area, x, y, width, height); 

  if (GTK_WIDGET_STATE(widget) == GTK_STATE_INSENSITIVE)
  {
	parent_smooth_state = SMOOTH_STATE_DISABLED;
	parent_state = GTK_STATE_INSENSITIVE;
  }

  part = THEME_PART(STEPPER_PART(style));

  thick = EDGE_LINE_THICKNESS(style, part);

  if (EDGE_LINE_STYLE(style, part) == SMOOTH_BEVEL_STYLE_FLAT)
    thick = 0;
  else if (EDGE_LINE_STYLE(style, part) == SMOOTH_BEVEL_STYLE_THIN)
    thick = 1;
  else if (EDGE_LINE_STYLE(style, part) == SMOOTH_BEVEL_STYLE_ICED)
    thick = 1;

	if (GTK_WIDGET_HAS_FOCUS(widget))
	{
		gtk_widget_get_focus_props (widget, &focus_line_width, &focus_padding, &interior_focus);
		if (!interior_focus)
		{
			focus = focus_line_width;
			entry_focused = TRUE;
		}
	}

  if ((!(widget)) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
    {
      if (CHECK_DETAIL (detail, "spinbutton_up"))
	{
          SmoothCanvasSetClipRectangle(Canvas, spin_area);

          SmoothCanvasSetPenColor(Canvas, (COLOR_CUBE(style).Input[parent_smooth_state].Background));
          SmoothCanvasFillRectangle(Canvas, x - thick, y + focus, width + thick - focus, height - focus*2);

          do_smooth_draw_shadow(Canvas, style, parent_state, GTK_SHADOW_IN, widget, "spinentry", 
                                x - thick, y + focus, width + thick - focus, height + thick - focus);

          SmoothCanvasClearClipRectangle(Canvas);

          smooth_fill_background(Canvas, style, state_type, shadow_type, part, 
                                 x, y + thick + focus, width - thick - focus, height - thick - focus,
                                 TRUE, shadow_type == GTK_SHADOW_IN, TRUE, FALSE, FALSE);
 
          if ((part->use_line) || (part->edge.use_line))
            detail = "smooth_stepper";

          do_smooth_draw_shadow(Canvas, style, state_type, shadow_type, widget, detail, 
                                x, y + thick + focus, width - thick - focus, height - thick - focus);

          if (entry_focused)
            {
              do_smooth_draw_focus(Canvas, style, parent_state, widget, detail, x - thick, y, width + thick, height + thick);
            }
	}
      else
	{
          SmoothCanvasSetClipRectangle(Canvas, spin_area);

          SmoothCanvasSetPenColor(Canvas, (COLOR_CUBE(style).Input[parent_smooth_state].Background));
          SmoothCanvasFillRectangle(Canvas, x - thick, y, width + thick - focus, height - focus);

          do_smooth_draw_shadow(Canvas, style, parent_state, GTK_SHADOW_IN, widget, "spinentry", 
                                x - thick, y - thick, width + thick - focus, height + thick - focus);

          SmoothCanvasClearClipRectangle(Canvas);

          smooth_fill_background(Canvas, style, state_type, shadow_type, part, 
                                 x, y + thick, width - thick - focus, height - thick - focus,
                                 TRUE, shadow_type == GTK_SHADOW_IN, TRUE, FALSE, FALSE);
 
          if ((part->use_line) || (part->edge.use_line))
            detail = "smooth_stepper";

          do_smooth_draw_shadow(Canvas, style, state_type, shadow_type, widget, detail, 
                                x, y , width - thick - focus, height - thick - focus);

          if (entry_focused)
            {
              do_smooth_draw_focus(Canvas, style, parent_state, widget, detail, x - thick, y, width + thick, height);
            }
	}
    }
  else
    {
      if (CHECK_DETAIL (detail, "spinbutton_up"))
	{
          SmoothCanvasSetClipRectangle(Canvas, spin_area);

          SmoothCanvasSetPenColor(Canvas, (COLOR_CUBE(style).Input[parent_smooth_state].Background));
          SmoothCanvasFillRectangle(Canvas, x + focus, y + focus, width + thick*2 - focus, height - focus);

          do_smooth_draw_shadow(Canvas, style, parent_state, GTK_SHADOW_IN, widget, "spinentry", 
                                x + focus, y + focus, width + thick*2 - focus, height + thick - focus);

          SmoothCanvasClearClipRectangle(Canvas);

          smooth_fill_background(Canvas, style, state_type, shadow_type, part, 
                                 x + thick + focus, y + thick + focus, width - thick - focus, height - thick - focus,
                                 TRUE, shadow_type == GTK_SHADOW_IN, TRUE, FALSE, FALSE);
 
          if ((part->use_line) || (part->edge.use_line))
            detail = "smooth_stepper";

          do_smooth_draw_shadow(Canvas, style, state_type, shadow_type, widget, detail, 
                                x + thick + focus, y + thick + focus , width - thick - focus, height - thick - focus);

          if (entry_focused)
            {
              do_smooth_draw_focus(Canvas, style, parent_state, widget, detail, x, y, width + thick*2, height + thick);
            }
	}
      else
	{
          SmoothCanvasSetClipRectangle(Canvas, spin_area);

          SmoothCanvasSetPenColor(Canvas, (COLOR_CUBE(style).Input[parent_smooth_state].Background));
          SmoothCanvasFillRectangle(Canvas, x + focus, y, width + thick*2 - focus, height - focus);

          do_smooth_draw_shadow(Canvas, style, parent_state, GTK_SHADOW_IN, widget, "spinentry", 
                                x + focus , y - thick, width + thick*2 - focus, height + thick - focus);

          SmoothCanvasClearClipRectangle(Canvas);

          smooth_fill_background(Canvas, style, state_type, shadow_type, part, 
                                 x + thick + focus, y, width - thick - focus, height - thick - focus,
                                 TRUE, shadow_type == GTK_SHADOW_IN, TRUE, FALSE, FALSE);
 
          if ((part->use_line) || (part->edge.use_line))
            detail = "smooth_stepper";

          do_smooth_draw_shadow(Canvas, style, state_type, shadow_type, widget, detail, 
                                x + thick + focus, y, width - thick - focus, height - thick - focus);

          if (entry_focused)
            {
              do_smooth_draw_focus(Canvas, style, parent_state, widget, detail, x, y - thick, width + thick*2, height + thick);
            }
	}
    }
}

static void
do_smooth_draw_box(SmoothCanvas Canvas,

		GtkStyle * style,
		GtkStateType state_type,
		GtkShadowType shadow_type,

		GtkWidget * widget,
		const gchar * detail,

		SmoothInt X,
		SmoothInt Y,
		SmoothInt Width,
		SmoothInt Height,

		SmoothBool Horizontal)
{
	SmoothColor base, darktone, lighttone;
	smooth_part_style *part = NULL;

	base =  COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;

	SmoothCanvasCacheShadedColor(Canvas, base, default_shades_table[0], &darktone);
	SmoothCanvasCacheShadedColor(Canvas, base, default_shades_table[1], &lighttone);		

   if (CHECK_DETAIL(detail, "handlebox_bin")) {
		if (!part) part = THEME_PART(BACKGROUND_PART(style));
		FLAT_FILL_BACKGROUND(Canvas, style, state_type, part, X, Y, Width, Height);
    } else if (CHECK_DETAIL(detail, "menubar")) {
        gint thick = 0;

	if (GRIP_OVERLAP_TOOLBAR(style) && IS_BONOBO_DOCK_ITEM(widget))
	  thick = EDGE_LINE_THICKNESS(style, NULL);
 
	if (!part) part = THEME_PART(BACKGROUND_PART(style));
        gradient_fill_background(Canvas, style, state_type, part, -thick, Y, X + Width + thick, Height, TRUE, Horizontal);
	do_smooth_draw_shadow(Canvas, style, state_type, shadow_type, widget, detail, X, Y, Width, Height);
    } else if (CHECK_DETAIL(detail, "toolbar")) {
	gboolean toolbar_overlap = (GRIP_OVERLAP_TOOLBAR(style) && (IS_BONOBO_DOCK_ITEM(widget)));
	gint hthick = 0, vthick = 0;
	
	if (toolbar_overlap) {
	  if (Horizontal)
	    hthick = EDGE_LINE_THICKNESS(style, NULL);
	  else
	    vthick = EDGE_LINE_THICKNESS(style, NULL);  
        }

	if (!part) part = THEME_PART(BACKGROUND_PART(style));
        FLAT_FILL_BACKGROUND(Canvas, style, state_type, part, X, Y, Width, Height);
	  
	gradient_fill_background(Canvas, style, state_type, part, X-hthick, Y-vthick, Width+hthick, Height+vthick, TRUE, Horizontal);
	do_smooth_draw_shadow(Canvas, style, state_type, shadow_type, widget, detail, X-hthick, Y-vthick, Width+hthick, Height+vthick);
    } else if (CHECK_DETAIL(detail, "trough")) {
        if (GTK_IS_SCALE (widget) && REAL_SLIDERS(style)) {
		SmoothLineBevel line_bevel;

	  if (!part) part = THEME_PART(BACKGROUND_PART(style));

          FLAT_FILL_BACKGROUND(Canvas, style, GTK_STATE_NORMAL, part, X, Y, Width, Height);

	  line_bevel.Style = LINE_STYLE(style, NULL);
	  line_bevel.Thickness = ythickness(style);

          if GTK_IS_HSCALE (widget) 
	    SmoothDrawLineBevel (line_bevel, Canvas, base, X, X + Width, Y + (Height / 2), TRUE);
          else
	    SmoothDrawLineBevel (line_bevel, Canvas, base, Y, Y + Height, X + (Width / 2), FALSE);
        } else {
  	  part = THEME_PART(TROUGH_PART(style));
			if (!part) part = THEME_PART(BACKGROUND_PART(style));	 
			gradient_fill_background(Canvas, style, state_type, part, 
	                           X+PART_XPADDING(part), Y+PART_YPADDING(part), 
	                           Width-PART_XPADDING(part)*2, Height-PART_YPADDING(part)*2, 
	                           shadow_type == GTK_SHADOW_IN, Horizontal);

          if (GTK_IS_SCALE(widget) && TROUGH_SHOW_VALUE(style)) {	    
	    GtkAdjustment * adjustment = gtk_range_get_adjustment(GTK_RANGE(widget));
	    gfloat value = 0;
  
            value = gtk_range_get_value(GTK_RANGE(widget));
	    
	    if (Horizontal) {
              gint w=0;           
              
              if (gtk_range_get_inverted(GTK_RANGE(widget)))
  	        w = (Width-PART_XPADDING(part)*2)*(1-(value- adjustment->lower) / (adjustment->upper - adjustment->lower));
	      else  
  	        w = (Width-PART_XPADDING(part)*2)*((value- adjustment->lower) / (adjustment->upper - adjustment->lower));
	      w = MAX (2, w);
	      w = MIN(w, Width-PART_XPADDING(part)*2);              
              
	      gradient_fill_background(Canvas, style, GTK_STATE_SELECTED, part, 
	                               X+PART_XPADDING(part), Y+PART_YPADDING(part), 
	                               w, Height-PART_YPADDING(part)*2, 
	                               shadow_type == GTK_SHADOW_IN, Horizontal);
            } else {
              gint h;           
              
              if (gtk_range_get_inverted(GTK_RANGE(widget)))
	        h = (Height-PART_YPADDING(part)*2)*((value - adjustment->lower) / (adjustment->upper - adjustment->lower));
	      else  
	        h = (Height-PART_YPADDING(part)*2)*(1-(value - adjustment->lower) / (adjustment->upper - adjustment->lower));

              h = MAX (2, h);
              h = MIN(h, Height-PART_YPADDING(part)*2);
	      
              gradient_fill_background(Canvas, style, GTK_STATE_SELECTED, part, 
	                               X + PART_XPADDING(part), Y + Height-PART_YPADDING(part)-h, 
	                               Width - PART_XPADDING(part)*2, h,
	                               shadow_type == GTK_SHADOW_IN, Horizontal);
            }
          }
	  
	  do_smooth_draw_shadow(Canvas, style, state_type, shadow_type, widget, "smooth_trough", X+PART_XPADDING(part), Y+PART_YPADDING(part), 
				Width - PART_XPADDING(part)*2, Height - PART_YPADDING(part)*2);
      }
    } else if ((CHECK_DETAIL (detail, "button")) && widget && ge_is_in_combo_box (widget) && (ENTRY_BUTTON_EMBED(style)))
    {
        smooth_draw_combobox_button (Canvas, style, state_type, shadow_type, widget, detail, X, Y, Width, Height);
    } else if ((CHECK_DETAIL(detail, "spinbutton_up") || CHECK_DETAIL(detail, "spinbutton_down")) && (ENTRY_BUTTON_EMBED(style)))
    {
        smooth_draw_spinbutton_stepper (Canvas, style, state_type, shadow_type, widget, detail, X, Y, Width, Height);
    } else if (CHECK_DETAIL(detail, "hruler") || CHECK_DETAIL(detail, "vruler") || CHECK_DETAIL(detail, "metacity") || CHECK_DETAIL(detail, "bar") || CHECK_DETAIL(detail, "spinbutton_up") || CHECK_DETAIL(detail, "spinbutton_down") || 
               CHECK_DETAIL(detail, "optionmenu") || CHECK_DETAIL(detail, "slider") || CHECK_DETAIL(detail, "menuitem") || 
               CHECK_DETAIL(detail, "togglebutton") || CHECK_DETAIL(detail, "button") || CHECK_DETAIL(detail, "hscrollbar") || CHECK_DETAIL(detail, "vscrollbar")) 
    {
 	part = NULL;

        /* per widget size/state/shadow adjustments */ 
        if (CHECK_DETAIL(detail, "slider"))
        {
		part = THEME_PART(GRIP_PART(style));

		if ((part->use_line || part->edge.use_line))
			detail = "smooth_slider";
        }
	else if ((EDGE_LINE_STYLE(style,NULL) == SMOOTH_BEVEL_STYLE_FLAT) && CHECK_DETAIL(detail, "spinbutton_down")) 
          {
            Y -= 1;
            Height += 1;
          } 
        else if ((shadow_type == GTK_SHADOW_NONE) && (EDGE_LINE_STYLE(style, NULL) != SMOOTH_BEVEL_STYLE_NONE) && CHECK_DETAIL(detail, "menuitem"))
	  shadow_type = GTK_SHADOW_OUT;
        else if (CHECK_DETAIL(detail, "bar") && GTK_IS_PROGRESS_BAR(widget))
          {
            part = PROGRESS_PART(style);

	    X += PART_XPADDING(part);
            Y += PART_YPADDING(part), 
	    Width -= PART_XPADDING(part)*2;
            Height -= PART_YPADDING(part)*2; 
          } 
        else if (CHECK_DETAIL(detail, "hscrollbar") || CHECK_DETAIL(detail, "vscrollbar"))
	  {
		part = THEME_PART(STEPPER_PART(style));

		if ((part->use_line) || (part->edge.use_line))
			detail = "smooth_stepper";
          }
        else if (CHECK_DETAIL(detail, "button"))
          {
            part = smooth_button_part(style, FALSE);

		if ((part->use_line || part->edge.use_line))
			detail = "smooth_button";
          }

        /*paint fill of orientation with a gradient, invert if GTK_SHADOW_IN */
	if (!part) part = THEME_PART(BACKGROUND_PART(style));
	smooth_fill_background(Canvas, style, state_type, shadow_type, part, X, Y, Width, Height, 
				TRUE, shadow_type == GTK_SHADOW_IN, Horizontal, FALSE, FALSE);

        /* per widget special drawing */ 
        if (CHECK_DETAIL(detail, "slider") || CHECK_DETAIL(detail, "smooth_slider"))
          smooth_draw_slider_grip(Canvas, style, state_type, X, Y, Width, Height, Horizontal);
        else if (CHECK_DETAIL(detail, "button") && (smooth_button_default_triangle(style)) & GTK_WIDGET_HAS_DEFAULT (widget)) 
	{
		/* Paint a triangle here instead of in "buttondefault"
		 * which is drawn _behind_ the current button 
		 */
		SmoothColor selected;
			
		SmoothPoint points1[4]; /* dark */
		SmoothPoint points2[4]; /* light */
	  
		SmoothPointSetValues(&points1[0], X + 2 ,  Y + 2 );
		SmoothPointSetValues(&points1[1], X + 10,  Y + 2 );
		SmoothPointSetValues(&points1[2], X + 2 ,  Y + 10);
		SmoothPointSetValues(&points1[3], X + 2 ,  Y + 2 );

		SmoothPointSetValues(&points2[0], X + 3 ,  Y + 3 );
		SmoothPointSetValues(&points2[1], X + 10,  Y + 3 );
		SmoothPointSetValues(&points2[2], X + 3 ,  Y + 10);
		SmoothPointSetValues(&points2[3], X + 3 ,  Y + 3 );

		SmoothCanvasSetPenColor(Canvas, darktone);
		SmoothCanvasDrawLines(Canvas, points1, 4);

		SmoothCanvasSetPenColor(Canvas, lighttone);
		SmoothCanvasDrawLines(Canvas, points2, 4);

		if (state_type == GTK_STATE_SELECTED)
			selected = base;
		else
		{
			selected =  COLOR_CUBE(style).Interaction[SMOOTH_STATE_SELECTED].Background;

			SmoothCanvasCacheColor(Canvas, &selected);		
		}	

		SmoothCanvasSetBrushColor(Canvas, selected);
		SmoothCanvasFillPolygon(Canvas, points2, 4);

		if (state_type != GTK_STATE_SELECTED)
		{
			SmoothCanvasUnCacheColor(Canvas, &selected);		
			SmoothCanvasSetPenColor(Canvas, base);
		}	
	}
          
        /* paint shadow */
	  do_smooth_draw_shadow(Canvas, style, state_type, shadow_type, widget, detail, X, Y, Width, Height);

    } else {
		if (!part) part = THEME_PART(BACKGROUND_PART(style));
		FLAT_FILL_BACKGROUND(Canvas, style, state_type, part, X, Y, Width, Height);
		do_smooth_draw_shadow(Canvas, style, state_type, shadow_type, widget, detail, X, Y, Width, Height);
    }

	SmoothCanvasUnCacheShadedColor(Canvas, base, default_shades_table[0], &darktone);
	SmoothCanvasUnCacheShadedColor(Canvas, base, default_shades_table[1], &lighttone);		
}

/********************************************************************************************************/
/* THE ABOVE IS WHATS LEFT OF smooth_style.c CLEAN IT UP/GET RID OF IT SOON                             */
/********************************************************************************************************/

void
smooth_draw_shadow(GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height)
{
	SmoothCanvas da;

	g_return_if_fail(sanitize_parameters(style, window, &width, &height));


	if ((CHECK_DETAIL (detail, "entry")) && (widget) && (ge_is_in_combo_box (widget)) && (ENTRY_BUTTON_EMBED(style)))
	{
		GtkWidget *button = g_object_get_data(G_OBJECT(ge_find_combo_box_widget_parent (widget)), "button");

		if (GTK_IS_WIDGET(button))
		{
			gtk_widget_queue_draw_area(button, button->allocation.x, button->allocation.y, 
							button->allocation.width, button->allocation.height);
		}
	}

        GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));
	if ((EDGE_LINE_STYLE(style,NULL) == SMOOTH_BEVEL_STYLE_FLAT)  && CHECK_DETAIL(detail, "entry") && widget && (GTK_IS_SPIN_BUTTON (widget) || (ge_is_combo_box_entry(widget)) || (ge_is_combo(widget)))) 	
	{
		gtk_paint_flat_box(style, window, widget->state, GTK_SHADOW_NONE, area, widget, "entry_bg", x, y, width, height);

		do_smooth_draw_shadow(da, style, state_type, shadow_type, widget, detail, x, y, width, height);
	} 
	else 
	{
		do_smooth_draw_shadow(da, style, state_type, shadow_type, widget, detail, x, y, width, height);
	}	

	GDKFinalizeCanvas(&da);
}

void
smooth_draw_hline(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   const gchar * detail,
	   gint x1,
	   gint x2,
	   gint y)
{
	SmoothCanvas da;
	SmoothColor base;
	SmoothLineBevel line_bevel;

	g_return_if_fail(sanitize_parameters(style, window, NULL, NULL));


        GDKInitializeCanvas(&da, style, window, area, NULL, NULL, x2-x1, x2-x1, 0, 0, &COLOR_CUBE(style));

	base =  COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;
	
	line_bevel.Style = LINE_STYLE(style, NULL);
	line_bevel.Thickness = ythickness(style);
	SmoothDrawLineBevel(line_bevel, da, base, x1, x2, y, TRUE);

	GDKFinalizeCanvas(&da);
}


void
smooth_draw_vline(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   const gchar * detail,
	   gint y1,
	   gint y2,
	   gint x)
{
	SmoothCanvas da;
	SmoothColor base;
	SmoothLineBevel line_bevel;

	g_return_if_fail(sanitize_parameters(style, window, NULL, NULL));

	if (ge_is_combo_box(widget, FALSE) && (!ge_is_combo_box_entry(widget)))
		return;


        GDKInitializeCanvas(&da, style, window, area, NULL, NULL, y2-y1, y2-y1, 0, 0, &COLOR_CUBE(style));

	base =  COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;

	line_bevel.Style = LINE_STYLE(style, NULL);
	line_bevel.Thickness = ythickness(style);
	SmoothDrawLineBevel(line_bevel, da, base, y1, y2, x, FALSE);

	GDKFinalizeCanvas(&da);
}

void
smooth_draw_polygon(GtkStyle * style,
	     GdkWindow * window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle * area,
	     GtkWidget * widget,
	     const gchar * detail,
	     GdkPoint * points,
	     gint npoints,
	     gint fill)
{
	SmoothCanvas da;
	SmoothBorder border;
	SmoothColor base;
	smooth_part_style *part = NULL;
	SmoothDouble roundness = 0.0;
	SmoothInt width, height;

	g_return_if_fail(sanitize_parameters(style, window, NULL, NULL));


	gdk_drawable_get_size(window, &width, &height);
        GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));

	base =  COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;
	
	if (fill)
	{
		SmoothCanvasSetBrushColor(da, base);
		SmoothCanvasFillPolygon(da, points, npoints);
	}
  
	smooth_style_get_border(style, state_type, shadow_type, detail, part, roundness, &border);
	SmoothDrawPolygonBorder(&border, da, base, points, npoints);

	GDKFinalizeCanvas(&da);
}

void
smooth_draw_arrow(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GtkShadowType shadow_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   const gchar * detail,
	   GtkArrowType arrow_type,
	   gint fill,
	   gint x,
	   gint y,
	   gint width,
	   gint height)
{
	SmoothCanvas da;
	SmoothArrowPart *part = NULL;
	SmoothArrow arrow;
	SmoothColor background, foreground;
	SmoothWidgetState widget_state = GDKSmoothWidgetState(state_type);
	
	g_return_if_fail(sanitize_parameters(style, window, &width, &height));

	if (ge_is_combo_box(widget, FALSE) && (!ge_is_combo_box_entry(widget)))
		return;
	

	if (IS_SCROLLBAR(widget) || IS_SPIN_BUTTON(widget))
		part = &(STEPPER_PART(style)->Arrow);
	else
		part = (SmoothArrowPart *)&ARROW_PART(style);

	smooth_style_get_arrow(part, state_type, arrow_type, &arrow);

	GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));
    
	background =  COLOR_CUBE(style).Interaction[widget_state].Background;

	if (arrow.Etched)
	{
		background =  COLOR_CUBE(style).Interaction[widget_state].Background;
	}	
	else
	{
		if (arrow.Solid)
		{
			background = COLOR_CUBE(style).Interaction[widget_state].Foreground;
		}	
	}	
	
	if (arrow.Solid)
	{
		foreground =  COLOR_CUBE(style).Interaction[widget_state].Background;
	}	
	else
	{
		foreground = COLOR_CUBE(style).Interaction[widget_state].Foreground;
	}
	
	reverse_engineer_arrow_box (widget, detail, arrow_type, &x, &y, &width, &height);

	x += arrow.XPadding; 
	y += arrow.YPadding; 
	width -= 2*arrow.XPadding; 
	height -= 2*arrow.YPadding;

	SmoothCanvasCacheColor(da, &background);
	SmoothCanvasCacheColor(da, &foreground);

	SmoothDrawArrow(da, arrow, x+1, y+1, width-2, height-2, background, foreground);

	SmoothCanvasUnCacheColor(da, &foreground);
	SmoothCanvasUnCacheColor(da, &background);

	GDKFinalizeCanvas(&da);
}

void
smooth_draw_focus(GtkStyle *style,
           GdkWindow *window,
           GtkStateType state_type,
           GdkRectangle *area,
           GtkWidget *widget,
           const gchar *detail,
           gint x,
           gint y,
           gint width,
           gint height)
{
	SmoothCanvas da;

	g_return_if_fail(sanitize_parameters(style, window, &width, &height));


        GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));

	do_smooth_draw_focus(da, style, state_type, widget, detail, x, y, width, height);

	GDKFinalizeCanvas(&da);
}

void
smooth_draw_slider(GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height,
	    GtkOrientation orientation)
{
	SmoothCanvas da;

	g_return_if_fail(sanitize_parameters(style, window, &width, &height));

        GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));

	do_smooth_draw_box(da, style, state_type, shadow_type, widget, "slider", x, y, width, height, orientation == GTK_ORIENTATION_HORIZONTAL);

	GDKFinalizeCanvas(&da);
}

void
smooth_draw_extension(GtkStyle * style,
	       GdkWindow * window,
	       GtkStateType state_type,
	       GtkShadowType shadow_type,
	       GdkRectangle * area,
	       GtkWidget * widget,
	       const gchar * detail,
	       gint x,
	       gint y,
	       gint width,
	       gint height,
	       GtkPositionType gap_side)
{
  	SmoothCanvas da;
	SmoothBorder border;

  GtkStateType parent_state;
  SmoothColor base_color;
  GdkPoint	 fill_points[8], edge_points[8];
  GdkRectangle tab_area, smooth_draw_area;
  GtkStyle	 *parent_style;
  gint 		 selected=0;
  GtkNotebook *notebook=NULL;
  smooth_part_style *part;
  smooth_tab_style  *tab = NULL;
  gint               tab_stripe_off = 0;
  GdkRegion         *cliprgn = NULL;

  g_return_if_fail(sanitize_parameters(style, window, &width, &height));
    
  if (GTK_IS_NOTEBOOK(widget)) {
    notebook = GTK_NOTEBOOK(widget);
    selected = (state_type == GTK_STATE_NORMAL);
  }  

  tab_area.x = x - 1;
  tab_area.y = y - 1;
  tab_area.width = width + 2;
  tab_area.height = height + 2;
	
  if (area) {
    gdk_rectangle_intersect (&tab_area, area, &smooth_draw_area);
    area = &smooth_draw_area;
  } else {
    area = &tab_area;
  }
	
  part = smooth_tab_part(style, selected);
  tab = &THEME_DATA(style)->tabs;


	GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));

	base_color =  COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;	
	smooth_style_get_border(style, state_type, shadow_type, detail, part, 0.0, &border);


  switch (smooth_tab_get_style(style,selected))
    {
    case SMOOTH_TAB_NORMAL:
      goto square;
    case SMOOTH_TAB_ROUND:
      tab_stripe_off = 1;
      if ((rounded_extension_points(x, y, width, height, selected, TRUE, gap_side, fill_points)) && 
          (rounded_extension_points(x, y, width, height, selected, FALSE, gap_side, edge_points))) 
        goto draw;	    
      else
        return;
    case SMOOTH_TAB_TRIANGLE:
        tab_stripe_off = 1;
	if ( CHECK_DETAIL(detail, "tab") && widget && GTK_IS_NOTEBOOK(widget)) {
          gint i, t;

          switch (gap_side) {
            case GTK_POS_BOTTOM:
            case GTK_POS_TOP:
	        t = height - 5 + 2;
                i = t / 3;
                if (!(i > 0)) 
                  goto square;
		
                if (notebook->tab_hborder == 2) 
                {
                  g_object_set (notebook, "tab-hborder", i+2,NULL);
                  goto square;
                }
                if (notebook->tab_vborder != 2) 
                {
                  g_object_set (notebook, "tab-vborder", 2,NULL);
                  goto square;
                }
                break;               
            case GTK_POS_LEFT:
            case GTK_POS_RIGHT:
	        t = width - 5 + 2;
                i = t / 3;
                if (!(i > 0)) 
                  goto square;
		
                if (notebook->tab_vborder == 2) {
                  g_object_set (notebook, "tab-vborder", 2,NULL);
                  goto square;
               }
                if (notebook->tab_hborder != 2) {
                  g_object_set (notebook, "tab-hborder", 2,NULL);
                  goto square;
               }
               break;
          }
          if ((triangle_extension_points(x, y, width, height, selected, TRUE, gap_side, fill_points)) && 
             (triangle_extension_points(x, y, width, height, selected, FALSE, gap_side, edge_points))) 
            goto draw;	    
          else
            return;
	} else goto square;
    }
    
    square:
      if ((square_extension_points(x, y, width, height, selected, TRUE, gap_side, fill_points)) && 
          (square_extension_points(x, y, width, height, selected, FALSE, gap_side, edge_points))) 
        goto draw;	    
      else
        return;
    
    draw :	

	parent_style = style;
	parent_state = GTK_STATE_NORMAL;
	if (widget) {
		if (widget->state == GTK_STATE_INSENSITIVE)
			state_type = GTK_STATE_INSENSITIVE;
		
		if (widget->parent) {
			parent_style = widget->parent->style;
			parent_state = widget->parent->state;
		}
	}

        smooth_fill_background(da, parent_style, parent_state, GTK_SHADOW_NONE, part, x, y, width, height, FALSE, FALSE,
				 GTK_ORIENTATION_VERTICAL,FALSE, FALSE);
	  
	cliprgn = gdk_region_polygon(fill_points, 8, GDK_EVEN_ODD_RULE);

	GDKModifyCanvasClipRegion(&da, cliprgn);
	smooth_fill_background(da, style, state_type, GTK_SHADOW_NONE, part, x, y, width, height, FALSE, FALSE,
				 GTK_ORIENTATION_VERTICAL,FALSE, FALSE);

	/* draw tab highlight */
	if (selected && 
	    tab->highlight &&
	    GTK_IS_NOTEBOOK (widget)) {

		if (GTK_POS_LEFT == gtk_notebook_get_tab_pos (GTK_NOTEBOOK (widget))) {

			x += tab_stripe_off;
			smooth_fill_background(da, style, GTK_STATE_SELECTED, 
					   GTK_SHADOW_NONE, part, x, y, 3, 
					   height, FALSE, FALSE, GTK_ORIENTATION_VERTICAL, 
					   FALSE, FALSE);
		}
		if (GTK_POS_TOP== gtk_notebook_get_tab_pos (GTK_NOTEBOOK (widget))) {

			y += tab_stripe_off;
			smooth_fill_background(da, style, GTK_STATE_SELECTED, 
					   GTK_SHADOW_NONE, part, x, y, width, 
					   3, FALSE, FALSE, GTK_ORIENTATION_VERTICAL, 
					   FALSE, FALSE);
		}
		if (GTK_POS_RIGHT == gtk_notebook_get_tab_pos (GTK_NOTEBOOK (widget))) {

			x -= (tab_stripe_off + 1);
			smooth_fill_background(da, style, GTK_STATE_SELECTED, 
					   GTK_SHADOW_NONE, part, x + width - 3, y, 3, 
					   height, FALSE, FALSE, GTK_ORIENTATION_VERTICAL, 
					   FALSE, FALSE);
		}
		if (GTK_POS_BOTTOM == gtk_notebook_get_tab_pos (GTK_NOTEBOOK (widget))) {

			y -= (tab_stripe_off + 1);
			smooth_fill_background(da, style, GTK_STATE_SELECTED, 
					   GTK_SHADOW_NONE, part, x, y + height - 3, width, 
					   3, FALSE, FALSE, GTK_ORIENTATION_VERTICAL, 
					   FALSE, FALSE);		
		}
	}
	GDKModifyCanvasClipRegion(&da, NULL);
	gdk_region_destroy(cliprgn);

	/* draw inner shadow line(s)  */	
	SmoothDrawPolygonBorder(&border, da, base_color, edge_points, 8);

	GDKFinalizeCanvas(&da);
}

static void
do_smooth_draw_tab (SmoothCanvas Canvas,
			GtkStyle      *style,
		      GtkStateType   state_type,
		      GtkShadowType  shadow_type,
			GtkWidget * widget,
		      gint           x,
		      gint           y,
		      gint           width,
		      gint           height)
{
#define ARROW_SPACE 0

	GtkRequisition indicator_size;
	GtkBorder indicator_spacing;
	gint arrow_height;
  
	SmoothLineBevel line_bevel;
	SmoothInt vline_x;

	SmoothArrowPart *part = NULL;
	SmoothArrow arrow;
	SmoothColor base, background, foreground;
	SmoothWidgetState widget_state;
	
	part = (SmoothArrowPart *)&ARROW_PART(style);
	smooth_style_get_arrow(part, state_type, GTK_ARROW_UP, &arrow);

	widget_state = GDKSmoothWidgetState(state_type);
	base =  COLOR_CUBE(style).Interaction[widget_state].Background;	
 	
	background =  COLOR_CUBE(style).Input[widget_state].Background;	

 	foreground =  COLOR_CUBE(style).Interaction[widget_state].Foreground;	
	
	if (arrow.Etched)
	{
		background = base;
	}	
	else
	{
		if (arrow.Solid)
		{
			background = foreground;	
		}	
	}	

	ge_option_menu_get_props (widget, &indicator_size, &indicator_spacing);

	indicator_size.width += 2;
	arrow_height = indicator_size.width;

	line_bevel.Style = LINE_STYLE(style, NULL);
	line_bevel.Thickness = ythickness(style);

	if (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL)
	{
		vline_x = x + xthickness(style) + 19;
	}
	else
	{
		vline_x = x + width - 19 - xthickness(style);
	}
			
	SmoothDrawLineBevel(line_bevel, Canvas, base, 
				  y + ythickness(style) + 1,
				  y + height - ythickness(style) - 2,
				  vline_x,
		                  FALSE);

	y += ythickness(style) + 1;
	
	if (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL)
	{
		x += xthickness(style) - 1;
	}
	else
	{
		x += width - 19 - xthickness(style) + 1;
	}
	
	width =  19;
	height = height - ythickness(style) - 2,

	x += (width - indicator_size.width) / 2;
	y += (height - (2 * arrow_height + ARROW_SPACE)) / 2;

	SmoothDrawArrow(Canvas, arrow, x+1, y+1, indicator_size.width-2, arrow_height-2, background, foreground);

	smooth_style_get_arrow(part, state_type, GTK_ARROW_DOWN, &arrow);
	SmoothDrawArrow(Canvas, arrow, x+1, y + arrow_height + ARROW_SPACE+1, indicator_size.width-2, arrow_height-2, background, foreground);
}

/* This function is based on a portion of EnGradient's draw_box */
static void
draw_default_triangle(GtkStyle * style,
	              GdkWindow * window,
	              GtkStateType state_type,
	              GtkShadowType shadow_type,
	              GdkRectangle * area,
	              GtkWidget * widget,
	              const gchar * detail,
		      gint x,
		      gint y,
		      gint width,
		      gint height)
{
  GdkPoint points1[3]; /* dark */
  GdkPoint points2[3]; /* light */
  
  points1[0].x = x+2;  points1[0].y = y+2;
  points1[1].x = x+10; points1[1].y = y+2;
  points1[2].x = x+2;  points1[2].y = y+10;
  points2[0].x = x+3;  points2[0].y = y+3;
  points2[1].x = x+10; points2[1].y = y+3;
  points2[2].x = x+3;  points2[2].y = y+10;

   if (area) {
     gdk_gc_set_clip_rectangle(style->bg_gc[state_type], area);
     gdk_gc_set_clip_rectangle(style->light_gc[state_type], area);
     gdk_gc_set_clip_rectangle(style->dark_gc[state_type], area);
   } 
	
   /* Paint a triangle here instead of in "buttondefault"
      which is drawn _behind_ the current button */
   if (GTK_WIDGET_HAS_DEFAULT (widget)) {
     gdk_draw_polygon(window, style->dark_gc[state_type], FALSE, points1, 3);
     gdk_draw_polygon(window, style->light_gc[state_type], FALSE, points2, 3);
     gdk_draw_polygon(window, style->bg_gc[GTK_STATE_SELECTED], TRUE, points2, 3);
   }

   if (area) {
     gdk_gc_set_clip_rectangle(style->bg_gc[state_type], NULL);
     gdk_gc_set_clip_rectangle(style->light_gc[state_type], NULL);
     gdk_gc_set_clip_rectangle(style->dark_gc[state_type], NULL);
   } 
}

static void
smooth_draw_button_default(SmoothCanvas Canvas,

				GtkStyle *style,
				GtkStateType state_type,

				GdkRectangle *button,
				GtkWidget *widget,

				gint x,
				gint y,
				gint width,
				gint height)
{
	GdkRectangle button_area;
	smooth_part_style *part=smooth_button_part(style, TRUE);
	
	if (button)
	{
		button_area.x = button->x;
		button_area.y = button->y;
		button_area.width = button->width;
		button_area.height = button->height;
	}
	else
	{
		/* if no button area was passed assume the button area is 1 pixel inside, eg. assume there is room for win32 focus */
		button_area.x = x+1;
		button_area.y = y+1;
		button_area.width = width+1;
		button_area.height = height+1;
	}
	
	switch (smooth_button_get_style(style, TRUE))
	{
		case SMOOTH_BUTTON_DEFAULT_STYLE_GTK:
		{
			SmoothBorder border;
			SmoothColor base;
			SmoothDouble roundness = 0.0;

			/*paint fill with a gradient, invert if GTK_SHADOW_IN */
			if (!part) part = THEME_PART(BACKGROUND_PART(style));
			gradient_fill_background(Canvas, style, GTK_SHADOW_IN, part, x, y, width, height, TRUE, FALSE);

			/* paint shadow */
		 	base = COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;

			part = smooth_button_part(style, TRUE);
			smooth_style_get_border(style, state_type, GTK_SHADOW_IN, NULL, part, roundness, &border);

			SmoothDrawBorderWithGap(&border, Canvas, base, x, y, width, height, 0, 0, 0);
		}	
		break;

		case SMOOTH_BUTTON_DEFAULT_STYLE_NONE:
		case SMOOTH_BUTTON_DEFAULT_STYLE_TRIANGLE:/* draw the triangle on button draw, not here */
		case SMOOTH_BUTTON_DEFAULT_STYLE_WIN32:/* draw background first */
		default:
		{
			/*try and paint fill with parent style */
			GtkStyle *parent_style = style;
			GtkStateType parent_state = GTK_STATE_NORMAL;
			
			if ((widget) && (widget->parent)) {
				parent_style = widget->parent->style;
				parent_state = widget->parent->state;
			}

			if (!part) part = THEME_PART(BACKGROUND_PART(style));
			smooth_fill_background(Canvas, parent_style, parent_state, GTK_SHADOW_NONE, part, x, y, width, height, FALSE, FALSE,
						FALSE,FALSE, FALSE);						
		}				
	}	

	if (smooth_button_get_style(style, TRUE) == SMOOTH_BUTTON_DEFAULT_STYLE_WIN32)
	{
		SmoothRectangle ta;
		SmoothColor base, shade;
		
	 	base = COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;

		SmoothRectangleSetValues(&ta, button_area.x+1, button_area.y+1,  button_area.width-2, button_area.height-2);

		SmoothCanvasCacheShadedColor(Canvas, base, default_shades_table[4], &shade);

		SmoothDrawShadowWithGap(Canvas, shade, shade, ta, 0, 0, 0, TRUE);

		SmoothCanvasUnCacheShadedColor(Canvas, base, default_shades_table[4], &shade);
	}
}

void
smooth_draw_box(GtkStyle * style,
	 GdkWindow * window,
	 GtkStateType state_type,
	 GtkShadowType shadow_type,
	 GdkRectangle * area,
	 GtkWidget * widget,
	 const gchar * detail,
	 gint x,
	 gint y,
	 gint width,
	 gint height)
{
	SmoothCanvas da;

	SmoothBool horizontal = SmoothTrue;
  
	g_return_if_fail(sanitize_parameters(style, window, &width, &height));


	if (CHECK_DETAIL(detail, "buttondefault"))
	{
		GdkRectangle button;

		gint border_width = GTK_CONTAINER (widget)->border_width;
		GtkBorder default_border;
		GtkBorder default_outside_border;
		gboolean interior_focus;
		gint focus_width;
		gint focus_pad;

		gtk_button_get_props (widget, &default_border, &default_outside_border, &interior_focus);
		gtk_widget_style_get (widget,
					"focus-line-width", &focus_width,
					"focus-padding", &focus_pad,
					NULL); 
	
		button.x = widget->allocation.x + border_width + default_outside_border.left;
		button.y = widget->allocation.y + border_width + default_outside_border.top;
		button.width = (widget->allocation.width - border_width * 2) - (default_outside_border.left + default_outside_border.right);
		button.height = (widget->allocation.height - border_width * 2) - (default_outside_border.top + default_outside_border.bottom);

		if (!interior_focus)
		{
			button.x += focus_width + focus_pad;
			button.y += focus_width + focus_pad;
			button.width -= 2 * (focus_width + focus_pad);
			button.height -= 2 * (focus_width + focus_pad);
		}
		
		GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));

		smooth_draw_button_default(da, style, state_type, &button, widget, x, y, width, height);

		GDKFinalizeCanvas(&da);
	} 
	else if ((EDGE_LINE_STYLE(style,NULL) == SMOOTH_BEVEL_STYLE_FLAT) && CHECK_DETAIL(detail, "entry") && widget 
				&& (GTK_IS_SPIN_BUTTON (widget) || (ge_is_combo_box_entry(widget)) || (ge_is_combo(widget)))) 
	{
		SmoothColor fill;
		SmoothBorder border;
		SmoothColor base;
		smooth_part_style *part = NULL;
		SmoothDouble roundness = 0.0;

		fill =  COLOR_CUBE(style).Input[GDKSmoothWidgetState(state_type)].Background;
		base =  COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;

		GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));

		smooth_style_get_border(style, state_type, shadow_type, detail, part, roundness, &border);

		FLAT_FILL_BACKGROUND(da, style, state_type, NULL, x, y, width, height);

		SmoothCanvasCacheColor(da, &fill);

		SmoothCanvasSetBrushColor(da, fill);
		SmoothCanvasFillRectangle(da, x, y, width, height);

		SmoothCanvasUnCacheColor(da, &fill);

		if (!(widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR)) 
			SmoothDrawBorderWithGap(&border, da, base, x, y, width, height, GTK_POS_RIGHT, 0, height);
		else
			SmoothDrawBorderWithGap(&border, da, base, x, y, width, height, GTK_POS_LEFT, 0, height);

		GDKFinalizeCanvas(&da);
	}
	else	
	{
		if (CHECK_DETAIL(detail, "togglebutton") || CHECK_DETAIL(detail, "button") || CHECK_DETAIL(detail, "spinbutton_up") || CHECK_DETAIL(detail, "spinbutton_down") || GTK_IS_BUTTON(widget))
			horizontal = TRUE;
		else if (CHECK_DETAIL(detail, "menubar") || CHECK_DETAIL(detail, "menuitem") || CHECK_DETAIL(detail, "optionmenu") || CHECK_DETAIL(detail, "optionmenutab") || CHECK_DETAIL(detail, "metacity"))
			horizontal = TRUE;
		else if (CHECK_DETAIL(detail, "hruler"))
			horizontal = TRUE;
		else if (CHECK_DETAIL(detail, "vruler"))
			horizontal = FALSE;
		else if ((CHECK_DETAIL(detail, "hscrollbar") || CHECK_DETAIL(detail, "vscrollbar")))
		{
			if (CHECK_DETAIL(detail, "vscrollbar")) 
			{
				if (EDGE_LINE_STYLE(style,NULL) == SMOOTH_BEVEL_STYLE_FLAT)
				{
					if (y < widget->allocation.y + widget->allocation.height/2) 
					{
						if ((area) && (area->height >= height))
							area->height = height + 1;
						height +=1;
					} 
					else 
					{
						if ((area) && (area->y <= y))
							area->y = y - 1;
						y -= 1; 
		
						if ((area) && (area->height >= height))
							area->height = height + 1;
						height +=1;
					}
				}	

				horizontal = FALSE;
			} 
			else 
			{
				if (EDGE_LINE_STYLE(style,NULL) == SMOOTH_BEVEL_STYLE_FLAT)
				{
					if (x < widget->allocation.x + widget->allocation.width/2) 
					{
						if ((area) && (area->width >= width))
							area->width = width + 1;
						width +=1;
					} 
					else 
					{
						if ((area) && (area->x <= x))
							area->x = x - 1;
						x -= 1; 
		
						if ((area) && (area->width >= width))
							area->width = width + 1;
						width +=1;
					}
				}				

				horizontal = TRUE;
			}        
		}
		else if (GTK_IS_PROGRESS_BAR(widget)) 
		{
			switch (GTK_PROGRESS_BAR(widget)->orientation) 
			{
				case GTK_PROGRESS_LEFT_TO_RIGHT:
				case GTK_PROGRESS_RIGHT_TO_LEFT:
					horizontal = TRUE;
				break;
	
				case GTK_PROGRESS_BOTTOM_TO_TOP:
				case GTK_PROGRESS_TOP_TO_BOTTOM:
					horizontal = FALSE;
				break;
			}   
		}  
		else if (GTK_IS_SCROLLBAR(widget)) 
			horizontal = GTK_IS_HSCROLLBAR(widget);
		else if (GTK_IS_SCALE(widget))
			horizontal = GTK_IS_HSCALE(widget);
		else if (height > width)
			horizontal = FALSE;
		else
			horizontal = TRUE;


		GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));

		do_smooth_draw_box(da, style, state_type, shadow_type, widget, detail, x, y, width, height, horizontal);

		if (CHECK_DETAIL(detail, "optionmenu") ||  (CHECK_DETAIL(detail, "button") && (ge_is_combo_box(widget, FALSE)) && !(ge_is_combo_box_entry(widget))))
		{		                  
			do_smooth_draw_tab (da, style, state_type, shadow_type, widget, x, y, width, height);
		} 
		else if (CHECK_DETAIL(detail, "button") && (smooth_button_default_triangle(style))) 
		{
			/* Paint a triangle here instead of in "buttondefault"
			   which is drawn _behind_ the current button */
			draw_default_triangle(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
		}
		
		GDKFinalizeCanvas(&da);
	}
}

void
smooth_draw_box_gap (GtkStyle * style, 
              GdkWindow * window, 
	      GtkStateType state_type, 
	      GtkShadowType shadow_type, 
	      GdkRectangle * area, 
	      GtkWidget * widget, 
	      const gchar * detail, 
	      gint x, 
	      gint y, 
	      gint width, 
	      gint height, 
	      GtkPositionType gap_side, 
	      gint gap_pos, 
	      gint gap_width)
{
	SmoothCanvas da;
	SmoothBorder border;
	SmoothColor base;
	smooth_part_style *part = NULL;

	g_return_if_fail(sanitize_parameters(style, window, &width, &height));


        GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));

	base =  COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;	

	FLAT_FILL_BACKGROUND(da, style, state_type, NULL, x, y, width, height);

	smooth_style_get_border(style, state_type, shadow_type, detail, part, 0.0, &border);
	SmoothDrawBorderWithGap(&border, da, base, x, y, width, height, gap_side, gap_pos - 1, gap_width + 1);

	GDKFinalizeCanvas(&da);
}

void
smooth_draw_shadow_gap (GtkStyle * style, 
                 GdkWindow * window, 
		 GtkStateType state_type, 
		 GtkShadowType shadow_type, 
		 GdkRectangle * area, 
		 GtkWidget * widget, 
		 const gchar * detail, 
		 gint x, 
		 gint y, 
		 gint width, 
		 gint height, 
		 GtkPositionType gap_side, 
		 gint gap_pos, 
		 gint gap_width)
{
	SmoothCanvas da;
	SmoothBorder border;
	SmoothColor base;
	smooth_part_style *part = NULL;

	g_return_if_fail(sanitize_parameters(style, window, &width, &height));


        GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));

	base =  COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;	

	FLAT_FILL_BACKGROUND(da, style, state_type,  NULL, x, y, width, height);

	smooth_style_get_border(style, state_type, shadow_type, detail, part, 0.0, &border);
	SmoothDrawBorderWithGap(&border, da, base, x, y, width, height, gap_side, gap_pos, gap_width);

	GDKFinalizeCanvas(&da);
}

void
smooth_draw_option(GtkStyle * style,
            GdkWindow * window,
            GtkStateType state_type,
            GtkShadowType shadow_type,
            GdkRectangle * area,
            GtkWidget * widget,
            const gchar *detail,
            gint X,
            gint Y,
            gint Width,
            gint Height)
{
	SmoothCanvas da;
	SmoothColor basecolor, color;
	SmoothBorder border;
	gboolean inconsistent = FALSE;

	smooth_check_style * option;
	SmoothLinePart * line_style;
	SmoothWidgetState widget_state;
	
	g_return_if_fail(sanitize_parameters(style, window, &Width, &Height));



	if (GTK_IS_CELL_RENDERER_TOGGLE(widget) || CHECK_DETAIL(detail, "cellradio"))
        {
		X -= 1;
		Y -= 1;
		Width += 2;
		Height += 2;
	}

        GDKInitializeCanvas(&da, style, window, area, NULL, NULL, Width, Height, 0, 0, &COLOR_CUBE(style));
  
  	widget_state = GDKSmoothWidgetState(state_type);
	basecolor =  COLOR_CUBE(style).Interaction[widget_state].Background;	

	color =  COLOR_CUBE(style).Input[widget_state].Foreground;	

	SmoothCanvasCacheColor(da, &color);		

	option = OPTION_PART(style);
	if (THEME_PART(option)->use_line) 
		line_style = &THEME_PART(option)->line;
	else if (EDGE_PART(style)->use_line) 
		line_style = &EDGE_PART(style)->line;
	else  
		line_style = &THEME_DATA(style)->line;

	if (((shadow_type == GTK_SHADOW_IN) || (!OPTION_MOTIF(style))) && (PART_STYLE(option) == SMOOTH_CHECKMARK_STYLE_NONE))
		state_type = GTK_STATE_ACTIVE;

	if ((line_style->style != SMOOTH_BEVEL_STYLE_BEVELED) && (line_style->style != SMOOTH_BEVEL_STYLE_THIN) && (line_style->style != SMOOTH_BEVEL_STYLE_SOFT))
		{X++; Y++; Width-=2; Height-=2;}     

	smooth_fill_background(da, style, state_type, shadow_type, THEME_PART(option), X, Y, Width, Height, 
				(OPTION_MOTIF(style)), TRUE, FALSE, TRUE, !((OPTION_MOTIF(style)) || (shadow_type == GTK_SHADOW_NONE)));
      
	if ((line_style->style != SMOOTH_BEVEL_STYLE_BEVELED) && (line_style->style != SMOOTH_BEVEL_STYLE_THIN) && (line_style->style != SMOOTH_BEVEL_STYLE_SOFT))
		{X--; Y--; Width+=2; Height+=2;}

	smooth_style_get_border(style, state_type, OPTION_MOTIF(style)?shadow_type:GTK_SHADOW_IN, detail, THEME_PART(option), 1.0, &border);

	SmoothDrawBorderWithGap(&border, da, basecolor, X, Y, Width, Height, 0, 0, 0);

	if ((line_style->style != SMOOTH_BEVEL_STYLE_BEVELED) && (line_style->style != SMOOTH_BEVEL_STYLE_THIN) && (line_style->style != SMOOTH_BEVEL_STYLE_SOFT))
		{X+=2; Y+=2; Width-=3; Height-=3;}
	else
		{X++; Y++; Width-=2; Height-=2;}   
	
	inconsistent = (IS_TOGGLE_BUTTON(widget) && gtk_toggle_button_get_inconsistent(TOGGLE_BUTTON(widget)));
	inconsistent |= (GTK_IS_CELL_RENDERER_TOGGLE(widget) && gtk_cell_renderer_toggle_get_inconsistent (widget));
        inconsistent |= (CHECK_DETAIL(detail, "cellradio") && (shadow_type == GTK_SHADOW_ETCHED_IN));

	if ((shadow_type == GTK_SHADOW_IN) || (inconsistent))
	{
		SmoothCheckMark cm;
		
		cm.Shadow = 0;
                
		if (IS_TOGGLE_BUTTON(widget) && gtk_toggle_button_get_inconsistent(TOGGLE_BUTTON(widget)))
			cm.Style = SMOOTH_CHECKMARK_STYLE_MINUS; 
		else
			cm.Style = PART_STYLE(option); 

		SmoothDrawCheckMark(da, cm, X, Y, Width, Height, color, color);
	}		      

	SmoothCanvasUnCacheColor(da, &color);		

	GDKFinalizeCanvas(&da);
}

static void
smooth_style_get_checkmark(GtkStyle * style,
				SmoothCheckMark *CheckMark)
{
	CheckMark->Shadow = 0;
	CheckMark->Style = PART_STYLE(CHECK_PART(style));
}

void
smooth_draw_check(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GtkShadowType shadow_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   const gchar * detail,
	   gint x,
	   gint y,
	   gint width,
	   gint height)
{  
	SmoothCanvas da;

	smooth_check_style * check;
	SmoothWidgetState widget_state;
	gboolean inconsistent = FALSE;

	g_return_if_fail(sanitize_parameters(style, window, &width, &height));



	check = CHECK_PART(style);

	if (GTK_IS_CHECK_MENU_ITEM(widget)) 
	{
		x -= 1;
		y -= 1;
		width += 2;
		height += 2;
	} else if (GTK_IS_CELL_RENDERER_TOGGLE(widget) || CHECK_DETAIL(detail, "cellcheck"))
        {
		x -= 1;
		y -= 1;
		width += 2;
		height += 2;
	}
  	
        GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));

	widget_state = GDKSmoothWidgetState(state_type);
  
	if (!((GTK_IS_CHECK_MENU_ITEM(widget) && (EDGE_LINE_STYLE(style, check) == SMOOTH_BEVEL_STYLE_NONE))))
	{
		if ((shadow_type == GTK_SHADOW_IN) && (EDGE_LINE_STYLE(style, check) == SMOOTH_BEVEL_STYLE_BEVELED) && (PART_STYLE(check) == SMOOTH_CHECKMARK_STYLE_NONE))
		{
			state_type = GTK_STATE_ACTIVE;
		}   

		smooth_fill_background(da, style, state_type, shadow_type, THEME_PART(check), x, y, width, height, 
				(CHECK_MOTIF(style)), TRUE, TRUE, FALSE, !((CHECK_MOTIF(style)) || (shadow_type == GTK_SHADOW_NONE)));
	}

	inconsistent = (IS_TOGGLE_BUTTON(widget) && gtk_toggle_button_get_inconsistent(TOGGLE_BUTTON(widget)));
	inconsistent |= (GTK_IS_CELL_RENDERER_TOGGLE(widget) && gtk_cell_renderer_toggle_get_inconsistent (widget));
        inconsistent |= (CHECK_DETAIL(detail, "cellcheck") && (shadow_type == GTK_SHADOW_ETCHED_IN));

	if ((shadow_type == GTK_SHADOW_IN) || (inconsistent))
	{
		SmoothCheckMark cm;
		SmoothInt thick = EDGE_LINE_THICKNESS(style, check);
		SmoothColor background, foreground;
		
		if (EDGE_LINE_STYLE(style, check) == SMOOTH_BEVEL_STYLE_COLD) 
			thick = 1;
		
	  	smooth_style_get_checkmark(style, &cm);

		if (IS_TOGGLE_BUTTON(widget) && gtk_toggle_button_get_inconsistent(TOGGLE_BUTTON(widget)))
			cm.Style = SMOOTH_CHECKMARK_STYLE_MINUS; 

		background =  COLOR_CUBE(style).Input[widget_state].Background;	
		foreground =  COLOR_CUBE(style).Input[widget_state].Foreground;	

		SmoothDrawCheckMark(da, cm, x+thick, y+thick, width-thick*2, height-thick*2, background, foreground);

	}

	if (!CHECK_MOTIF(style))
		shadow_type = GTK_SHADOW_IN;
  
	if (THEME_PART(check)->use_line || THEME_PART(check)->edge.use_line)
	{
		SmoothBorder border;
		SmoothColor base;
		SmoothDouble roundness = 0.0;

		base =  COLOR_CUBE(style).Interaction[widget_state].Background;	
	
		smooth_style_get_border(style, state_type, shadow_type, detail, THEME_PART(check), roundness, &border);
		SmoothDrawBorderWithGap(&border, da, base, x, y, width, height, 0, 0, 0);
	}
	else  
		do_smooth_draw_shadow(da, style, state_type, shadow_type, widget, detail, x, y, width, height);

	GDKFinalizeCanvas(&da);
}

/* Ancient crap almost never used. 
	for completeness sake this can probably reworked and moved
	to the gtk1.2 engine, and maybe gtk2, but overall its not pertinent
	to smooth so long as we have a polygon routine.
*/	
void
smooth_draw_diamond(GtkStyle * style,
	     GdkWindow * window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle * area,
	     GtkWidget * widget,
	     const gchar * detail,
	     gint x,
	     gint y,
	     gint width,
	     gint height)
{
	SmoothCanvas da;
	SmoothColor basecolor, darktone, lighttone;
	SmoothInt half_width, half_height;

	g_return_if_fail(sanitize_parameters(style, window, &width, &height));



	half_width = width / 2;
	half_height = height / 2;

	GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));
 
	basecolor =  COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;	
	
	SmoothCanvasCacheShadedColor(da, basecolor, default_shades_table[1], &lighttone);		
	SmoothCanvasCacheShadedColor(da, basecolor, default_shades_table[0], &darktone);

	switch (shadow_type)
	{
		case GTK_SHADOW_IN:
		{
			SmoothCanvasSetPenColor(da, lighttone);
			SmoothCanvasDrawLine(da, x + 2, y + half_height, x + half_width, y + height - 2);
			SmoothCanvasDrawLine(da, x + half_width, y + height - 2, x + width - 2, y + half_height);
			SmoothCanvasDrawLine(da, x + 1, y + half_height, x + half_width, y + height - 1);
			SmoothCanvasDrawLine(da, x + half_width, y + height - 1, x + width - 1, y + half_height);
			SmoothCanvasDrawLine(da, x, y + half_height, x + half_width, y + height);
			SmoothCanvasDrawLine(da, x + half_width, y + height, x + width, y + half_height);

			SmoothCanvasSetPenColor(da, darktone);
			SmoothCanvasDrawLine(da, x + 2, y + half_height, x + half_width, y + 2);
			SmoothCanvasDrawLine(da, x + half_width, y + 2, x + width - 2, y + half_height);
			SmoothCanvasDrawLine(da, x + 1, y + half_height, x + half_width, y + 1);
			SmoothCanvasDrawLine(da, x + half_width, y + 1, x + width - 1, y + half_height);
			SmoothCanvasDrawLine(da, x, y + half_height, x + half_width, y);
			SmoothCanvasDrawLine(da, x + half_width, y, x + width, y + half_height);
		}  
		break;

		case GTK_SHADOW_OUT:
		{
			SmoothCanvasSetPenColor(da, darktone);
			SmoothCanvasDrawLine(da, x + 2, y + half_height, x + half_width, y + height - 2);
			SmoothCanvasDrawLine(da, x + half_width, y + height - 2, x + width - 2, y + half_height);
			SmoothCanvasDrawLine(da, x + 1, y + half_height, x + half_width, y + height - 1);
			SmoothCanvasDrawLine(da, x + half_width, y + height - 1, x + width - 1, y + half_height);
			SmoothCanvasDrawLine(da, x, y + half_height, x + half_width, y + height);
			SmoothCanvasDrawLine(da, x + half_width, y + height, x + width, y + half_height);

			SmoothCanvasSetPenColor(da, lighttone);
			SmoothCanvasDrawLine(da, x + 2, y + half_height, x + half_width, y + 2);
			SmoothCanvasDrawLine(da, x + half_width, y + 2, x + width - 2, y + half_height);
			SmoothCanvasDrawLine(da, x + 1, y + half_height, x + half_width, y + 1);
			SmoothCanvasDrawLine(da, x + half_width, y + 1, x + width - 1, y + half_height);
			SmoothCanvasDrawLine(da, x, y + half_height, x + half_width, y);
			SmoothCanvasDrawLine(da, x + half_width, y, x + width, y + half_height);
		}
		break;

		default:
		break;
	}

	SmoothCanvasUnCacheShadedColor(da, basecolor, default_shades_table[1], &lighttone);		
	SmoothCanvasUnCacheShadedColor(da, basecolor, default_shades_table[0], &darktone);

	GDKFinalizeCanvas(&da);
}

void
smooth_draw_handle(GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height,
	    GtkOrientation orientation)
{
	SmoothCanvas da;
	SmoothBorder border;
	SmoothColor base;
	smooth_part_style *part = NULL;
	SmoothDouble roundness = 0.0;

    smooth_grip_style  *grip;
    gint ax=x, ay=y, aw=width, ah=height;
    gboolean toolbar_overlap = (GRIP_OVERLAP_TOOLBAR(style) && (CHECK_DETAIL(detail, "dockitem")));
    gboolean horiz=(CHECK_DETAIL(detail, "handlebox") || (CHECK_DETAIL(detail, "dockitem") && !IS_HANDLE_BOX_ITEM(widget)))?(orientation==GTK_ORIENTATION_HORIZONTAL):(orientation==GTK_ORIENTATION_VERTICAL);
    gint gap_size=(!horiz)?y+height:x+width, thick=0;
    gboolean vert=(!horiz);

	g_return_if_fail(sanitize_parameters(style, window, &width, &height));



        GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0, &COLOR_CUBE(style));

	base =  COLOR_CUBE(style).Interaction[GDKSmoothWidgetState(state_type)].Background;		
	
    aw=width; ah=height;
     
    grip = GRIP_PART(style);
    
    if (!GTK_IS_PANED(widget)) {
      if (toolbar_overlap)
	thick = EDGE_LINE_THICKNESS(style, NULL)*2;
      gradient_fill_background(da, style, state_type, THEME_PART(grip), x, y, width+thick*horiz, height+thick*vert, shadow_type == GTK_SHADOW_IN, orientation==GTK_ORIENTATION_HORIZONTAL);
    } else  {
      FLAT_FILL_BACKGROUND(da, style, state_type, THEME_PART(grip), x, y, width, height);
    }
    switch (PART_STYLE(grip)) {
      case NO_GRIP:
        break;
      case BARS_IN_GRIP :
      case BARS_OUT_GRIP :
        {
          gint w=width, h=height;
		
          if (vert) {
	     if (CHECK_DETAIL(detail, "handlebox")) {
              h+=1;
              gap_size=x+width;
	     }else
	     if (CHECK_DETAIL(detail, "dockitem")) {
	        w+=1;
	        h = MIN(height+1, width + height/7);
   	        y = y + (height - h)/2;
	        orientation = GTK_ORIENTATION_HORIZONTAL;
	        if (!toolbar_overlap) y -=1;
             } else 
	     {
               h+=1;
	       y-=1;
	       w = MIN(width+1, width/7 + height);
   	       x = x + (width - w)/2;
	     }
          } else {
	     if (CHECK_DETAIL(detail, "handlebox")) {
              w+=1;
              gap_size=y+height;
              orientation = GTK_ORIENTATION_VERTICAL;
	     }else
	     if (CHECK_DETAIL(detail, "dockitem")) {
               h+=1;
	       w = MIN(width+1, width/7 + height);
   	       x = x + (width - w)/2;
	       orientation = GTK_ORIENTATION_VERTICAL;
               if (!toolbar_overlap) x -=1;
	     } else {
               w+=1;
	       x-=1;
	       h = MIN(height+1, width + height/7);
   	       y = y + (height - h)/2;
	     }
          }
		
          width = w;
          height = h;
        }
	break;
      case LINES_IN_GRIP :
      case LINES_OUT_GRIP :
        {
          gint w=width, h=height;
		
          if (vert) {
	     w+=1;
	     h = MIN(height+1, width + height/7);
	     y = y + (height - h)/2;
             if ((GTK_IS_PANED(widget)) || (GTK_IS_RANGE(widget))) orientation = GTK_ORIENTATION_HORIZONTAL;
	     if (!GRIP_OVERLAP_TOOLBAR(style) && CHECK_DETAIL(detail, "dockitem")) y -=1;
          } else {
	     w = MIN(width+1, width/7 + height);
	     x = x + (width - w)/2;
	     if ((GTK_IS_PANED(widget)) || (GTK_IS_RANGE(widget))) orientation = GTK_ORIENTATION_VERTICAL;
	     if (!GRIP_OVERLAP_TOOLBAR(style) && CHECK_DETAIL(detail, "dockitem")) x -=1;
          }
		
          if (CHECK_DETAIL(detail, "dockitem") || CHECK_DETAIL(detail, "handlebox")) {
	    orientation = GTK_ORIENTATION_VERTICAL;
            break;
	  }

          width = w;
          height = h;
        }
	break;
      case DOTS_OUT_GRIP:
      case DOTS_IN_GRIP:
      case SMALLDOTS_IN_GRIP :
      case SMALLDOTS_OUT_GRIP :
        if (CHECK_DETAIL(detail, "dockitem") || CHECK_DETAIL(detail, "handlebox"))
	  orientation = GTK_ORIENTATION_VERTICAL;
        break;
      case FIXEDLINES_OUT_GRIP:
      case FIXEDLINES_IN_GRIP:
        if (CHECK_DETAIL(detail, "dockitem") || CHECK_DETAIL(detail, "handlebox"))
	  orientation = GTK_ORIENTATION_VERTICAL;
        break;
      case MAC_BUDS_IN_GRIP:
      case MAC_BUDS_OUT_GRIP:
        x += 2;
        y += 1;
      case NS_BUDS_IN_GRIP:
      case NS_BUDS_OUT_GRIP:
        orientation = GTK_ORIENTATION_VERTICAL;
        break;
     }

    smooth_draw_grip(da, style, state_type,x+PART_XPADDING(grip), y+PART_YPADDING(grip), width-PART_XPADDING(grip)*2, height-PART_YPADDING(grip)*2, orientation == GTK_ORIENTATION_HORIZONTAL);      

    switch (PART_STYLE(grip)) {
      case BARS_IN_GRIP :
      case BARS_OUT_GRIP :
      case LINES_IN_GRIP :
      case LINES_OUT_GRIP :
        x=ax; 
	y=ay; 
	width=aw; 
	height=ah;
	break;
	
      case MAC_BUDS_IN_GRIP:
      case MAC_BUDS_OUT_GRIP:
        x -= 2;
        y -= 1;
      case NS_BUDS_IN_GRIP:
      case NS_BUDS_OUT_GRIP:
	break;
    }

	if ((THEME_PART(grip)->use_line || THEME_PART(grip)->edge.use_line))
		part = THEME_PART(grip);


	smooth_style_get_border(style, state_type, shadow_type, detail, part, roundness, &border);

	if (toolbar_overlap)
		thick = EDGE_LINE_THICKNESS(style, part)*2;
	else
		gap_size = 0;    

	SmoothDrawBorderWithGap(&border, da, base, x, y, width+horiz*thick, height+vert*thick, vert?GTK_POS_BOTTOM:GTK_POS_RIGHT, 0, gap_size);

	GDKFinalizeCanvas(&da);
}

void
smooth_draw_resize_grip (GtkStyle       *style,
                              GdkWindow      *window,
                              GtkStateType    state_type,
                              GdkRectangle   *area,
                              GtkWidget      *widget,
                              const gchar    *detail,
                              GdkWindowEdge   edge,
                              gint            x,
                              gint            y,
                              gint            width,
                              gint            height)
{
	g_return_if_fail(sanitize_parameters(style, window, &width, &height));
  
	if (widget && GTK_IS_STATUSBAR(widget) && !RESIZE_GRIP(style)) 
	{
		//gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(widget), FALSE);
		return;
	}

	smooth_theme_parent_class->draw_resize_grip (style, window, state_type, area, widget, detail, edge, x, y, width, height);
}

void
smooth_draw_tab (GtkStyle      *style,
		      GdkWindow     *window,
		      GtkStateType   state_type,
		      GtkShadowType  shadow_type,
		      GdkRectangle  *area,
		      GtkWidget     *widget,
		      const gchar   *detail,
		      gint           x,
		      gint           y,
		      gint           width,
		      gint           height)
{
 /* handled in smooth_draw_box */
}

void 
smooth_draw_flat_box (GtkStyle * style,
               GdkWindow * window,
	       GtkStateType state_type,
	       GtkShadowType shadow_type,
	       GdkRectangle * area,
	       GtkWidget * widget,
	       const gchar * detail,
	       gint x, gint y, gint width, gint height)
{
	SmoothCanvas da;

   g_return_if_fail(sanitize_parameters(style, window, &width, &height));

   /* we always want call to the default for treeviews and such */  
 
   if ((CHECK_DETAIL(detail, "text")) || (CHECK_DETAIL(detail, "viewportbin")) || (CHECK_DETAIL(detail, "entry_bg")) || ((CHECK_DETAIL(detail, "cell_even")) || 
      (CHECK_DETAIL(detail, "cell_odd")) || (CHECK_DETAIL(detail, "cell_even_ruled")) || (CHECK_DETAIL(detail, "cell_odd_ruled")) || (CHECK_DETAIL(detail, "cell_even_sorted")) ||
      (CHECK_DETAIL(detail, "cell_odd_sorted")) || (CHECK_DETAIL(detail, "cell_even_ruled_sorted")) || (CHECK_DETAIL(detail, "cell_odd_ruled_sorted"))))
   {
     smooth_theme_parent_class->draw_flat_box (style, window, state_type, shadow_type,
                                               area, widget, detail, x, y, width, height);
   } 
   else {
        GDKInitializeCanvas(&da, style, window, area, NULL, NULL, width, height, 0, 0,&COLOR_CUBE(style));

	FLAT_FILL_BACKGROUND(da, style, state_type, NULL, x, y, width, height);

	if (CHECK_DETAIL(detail, "tooltip")) 
		gdk_draw_rectangle(window, style->dark_gc[state_type], FALSE, x, y, width - 1, height - 1);

	GDKFinalizeCanvas(&da);
   }
}                                             
