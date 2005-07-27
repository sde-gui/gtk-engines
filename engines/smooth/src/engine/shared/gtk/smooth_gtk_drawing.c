/******************************************************************************/
/* smooth_gtk_drawing.c - Shared drawing functions for all GTK+ versions      */
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
#include ENGINE_HEADER
#include ENGINE_RC_HEADER
#include ENGINE_MISC_HEADER

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
#define FLAT_FILL_BACKGROUND(canvas, style, state_type, part, x, y, width, height) (smooth_fill_background(canvas, style, state_type, GTK_SHADOW_NONE, part, x, y, width, height, FALSE, FALSE, FALSE,FALSE, FALSE))		
#define gradient_fill_background(canvas, style, state_type, part, x, y, width, height, invert, horizontal) (smooth_fill_background(canvas, style, state_type, GTK_SHADOW_NONE, part, x, y, width, height, TRUE, invert, horizontal,FALSE, FALSE))
void 
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

void
smooth_parent_fill_background(SmoothCanvas Canvas,
				GtkStyle * style,
				GtkStateType state_type,
				GtkShadowType shadow_type,
				GtkWidget *widget,
				void * part,
				gint x,
				gint y,
				gint width,
				gint height,
				gboolean use_gradient,  
				gboolean invert,
				gboolean horizontal)
{
	/*try and paint fill with parent style */
	GtkStyle *parent_style = style;
	GtkStateType parent_state = GTK_STATE_NORMAL;
	gint parent_x = x, parent_y = y, parent_width = width, parent_height = height;
	SmoothRectangle area;
			
	if ((widget) && (widget->parent)) {
		parent_style = widget->parent->style;
		parent_state = widget->parent->state;
		
		parent_x = -widget->allocation.x;
		parent_y = -widget->allocation.y;
		parent_width = widget->parent->allocation.width;
		parent_height = widget->parent->allocation.width;
	}

	if (!part) part = THEME_PART(BACKGROUND_PART(style));
	
	area.x = x;
	area.y = y;
	area.width = width;
	area.height = height;
	
	SmoothCanvasSetClipRectangle(Canvas, area);
	
	smooth_fill_background(Canvas, parent_style, parent_state, GTK_SHADOW_NONE, part, parent_x, parent_y, parent_width, parent_height, use_gradient, invert, horizontal,FALSE, FALSE);						
	
	SmoothCanvasClearClipRectangle(Canvas);
}

void 
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

void
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
                ((is_in_combo_box (widget) || IS_SPIN_BUTTON (widget)) && 
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
		
#		ifdef GTK2
		if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
#		endif
		{
			SmoothDrawBorderWithGap(&border, Canvas, base, x, y, width+thick, height, GTK_POS_RIGHT, 0, height);
		}
#		ifdef GTK2
		else
		{
			SmoothDrawBorderWithGap(&border, Canvas, base, x-thick, y, width+thick, height, GTK_POS_LEFT, 0, height);
		}
#		endif
	}
	else if (CHECK_DETAIL(detail, "toolbar") && GRIP_OVERLAP_TOOLBAR(style) && (is_toolbar_item(widget)))
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

void
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
/********************************************************************************************************/
/* THE ABOVE IS WHATS LEFT OF smooth_style.c CLEAN IT UP/GET RID OF IT SOON                             */
/********************************************************************************************************/
