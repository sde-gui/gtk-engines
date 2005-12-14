/******************************************************************************/
/* draw_border.c - Engine Agnostic Border Part Drawing                        */
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
/* Portions Based on the EnGradient Engine                                    */
/*   Andrew Cattau                                                            */
/*                                                                            */
/* Portions Based on the LightHouseBlue Engine                                */
/*   Jan Rosczak <jan.rosczak@web.de>                                         */
/*                                                                            */
/*   Design mockup, ideas, feedback:                                          */
/*     Eugenia Loli-Queru <eloli@hotmail.com>                                 */
/*                                                                            */
/* Portions Based on the XFCE Engine                                          */
/*   Olivier Fourdan <fourdan@xfce.org>                                       */
/*                                                                            */
/* Portions Based on the Xenophilia Engine                                    */
/*   Johan Hanson <misagon@bahnhof.se>                                        */
/******************************************************************************/
#include "smooth_parts.h"

/* This function is a homegrown function based upon the
 * draw_shadow & draw_shadow_gap routines of XFCE, lighthouseblue, 
 * EnGradient,  & Xenophilia,  and partly inspired by  the industrial
 * industrial_draw_rounded_rect_two_pixel, industrial_kaleidoscope_line,
 * & industrial_kaleidoscope_point functions
 */
void
SmoothDrawShadowWithGap(SmoothCanvas *Canvas,
			SmoothColor TopLeftColor,
			SmoothColor BottomRightColor,
			SmoothRectangle Target,
			SmoothPositionSide GapSide, 
			SmoothInt GapPos,
			SmoothInt GapSize,
			SmoothBool TopLeftOverlap)
{
	SmoothInt x, y, width, height;
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

	switch (GapSide) {
		case SMOOTH_POSITION_TOP:
			if (TopLeftOverlap) 
			{
				SmoothCanvasSetPenColor(Canvas, BottomRightColor);
				
				SmoothCanvasDrawLine(Canvas, 
							x,
							y + height - 1, 
							x + width - 1,
							y + height - 1);
							
				SmoothCanvasDrawLine(Canvas, 
							x + width - 1, 
							y,
							x + width - 1, 
							y + height - 1);
			} 

			SmoothCanvasSetPenColor(Canvas, TopLeftColor);
				
			SmoothCanvasDrawLine(Canvas, 
						x, 
						y,
						x, 
						y + height - 1);

			if (GapSize <= 0)
				SmoothCanvasDrawLine(Canvas, 
							x, 
							y, 
							x + width - 1, 
							y);
			else 
			{
				if (GapPos > 0)
					SmoothCanvasDrawLine(Canvas, 
								x, 
								y, 
								x + GapPos, 
								y);

				if ((width - (GapPos + GapSize)) > 0)
					SmoothCanvasDrawLine(Canvas, 
								x + GapPos + GapSize - 1,
								y, 
								x + width - 1, 
								y);
			}

			if (!TopLeftOverlap) 
			{
				SmoothCanvasSetPenColor(Canvas, BottomRightColor);

				SmoothCanvasDrawLine(Canvas, 
							x, 
							y + height - 1, 
							x + width - 1, 
							y + height - 1);
							
				SmoothCanvasDrawLine(Canvas, 
							x + width - 1, 
							y, 
							x + width - 1, 
							y + height - 1);
			} 
		break;

		case SMOOTH_POSITION_BOTTOM:
			if (!TopLeftOverlap) 
			{
				SmoothCanvasSetPenColor(Canvas, TopLeftColor);
				
				SmoothCanvasDrawLine(Canvas, 
							x, 
							y, 
							x, 
							y + height - 1);
							
				SmoothCanvasDrawLine(Canvas, 
							x, 
							y, 
							x + width - 1, 
							y);
			} 

			SmoothCanvasSetPenColor(Canvas, BottomRightColor);

			if (GapSize <= 0)
				SmoothCanvasDrawLine(Canvas, 
							x, 
							y + height - 1, 
							x + width - 1, 
							y + height - 1);
			else 
			{
				if (GapPos > 0)
					SmoothCanvasDrawLine(Canvas, 
								x, 
								y + height - 1, 
								x + GapPos, 
								y + height - 1);

				if ((width - (GapPos + GapSize)) > 0)
					SmoothCanvasDrawLine(Canvas, 
								x + GapPos + GapSize - 1, 
								y + height - 1, 
								x + width - 1, 
								y + height - 1);
			}
			
			SmoothCanvasDrawLine(Canvas, 
						x + width - 1, 
						y, 
						x + width - 1, 
						y + height - 1);

			if (TopLeftOverlap) 
			{
				SmoothCanvasSetPenColor(Canvas, TopLeftColor);

				SmoothCanvasDrawLine(Canvas, 
							x, 
							y, 
							x, 
							y + height - 1);
							
				SmoothCanvasDrawLine(Canvas, 
							x, 
							y, 
							x + width - 1, 
							y);
			} 
		break;
      
		case SMOOTH_POSITION_LEFT:
			if (TopLeftOverlap) 
			{
				SmoothCanvasSetPenColor(Canvas, BottomRightColor);

				SmoothCanvasDrawLine(Canvas, 
							x, 
							y + height - 1, 
							x + width - 1, 
							y + height-1);
							
				SmoothCanvasDrawLine(Canvas, 
							x + width - 1, 
							y, 
							x + width - 1, 
							y + height - 1);
			} 

			SmoothCanvasSetPenColor(Canvas, TopLeftColor);

			SmoothCanvasDrawLine(Canvas, 
						x, 
						y, 
						x + width - 1, 
						y);
    
			if (GapSize <= 0)
				SmoothCanvasDrawLine(Canvas, 
							x, 
							y, 
							x, 
							y + height - 1);
			else 
			{
				if (GapPos > 0)
					SmoothCanvasDrawLine(Canvas, 
								x, 
								y , 
								x, 
								y + GapPos);
          
				if ((height - (GapPos + GapSize)) > 0)
					SmoothCanvasDrawLine(Canvas, 
								x, 
								y + GapPos + GapSize - 1, 
								x, 
								y + height - 1);
			}

			if (!TopLeftOverlap) 
			{
				SmoothCanvasSetPenColor(Canvas, BottomRightColor);
	
				SmoothCanvasDrawLine(Canvas, 
							x, 
							y + height - 1, 
							x + width - 1, 
							y + height - 1);
							
				SmoothCanvasDrawLine(Canvas, 
							x + width - 1, 
							y, 
							x + width - 1, 
							y + height - 1);
			} 
		break;
      
		case SMOOTH_POSITION_RIGHT:
			if (!TopLeftOverlap) 
			{
				SmoothCanvasSetPenColor(Canvas, TopLeftColor);

				SmoothCanvasDrawLine(Canvas, 
							x, 
							y, 
							x, 
							y + height - 1);
							
				SmoothCanvasDrawLine(Canvas, 
							x, 
							y, 
							x + width - 1,
							y);
			} 

			SmoothCanvasSetPenColor(Canvas, BottomRightColor);

			SmoothCanvasDrawLine(Canvas, 
						x, 
						y + height - 1, 
						x + width - 1, 
						y + height - 1);
							
			if (GapSize <= 0)
				SmoothCanvasDrawLine(Canvas, 
							x + width - 1, 
							y, 
							x + width - 1, 
							y + height - 1);
			else 
			{
				if (GapPos > 0)
					SmoothCanvasDrawLine(Canvas, 
							x + width - 1, 
							y, 
							x + width - 1, 
							y + GapPos);
          
				if ((height - (GapPos + GapSize)) > 0)
					SmoothCanvasDrawLine(Canvas, 
							x + width - 1, 
							y + GapPos + GapSize - 1, 
							x + width - 1, 
							y + height - 1);
			}

			if (TopLeftOverlap) 
			{
				SmoothCanvasSetPenColor(Canvas, TopLeftColor);

				SmoothCanvasDrawLine(Canvas, 
							x, 
							y, 
							x, 
							y + height - 1);
							
				SmoothCanvasDrawLine(Canvas, 
							x, 
							y, 
							x + width - 1, 
							y);
			} 
		break;

		default :
		break;
	}
}

/* This function is a simple homegrown function wrapper around
 * do_draw_shadow_with_gap.
 */
void
SmoothDrawBevelWithGap(SmoothCanvas *Canvas,
			SmoothColor TopLeftColor,
			SmoothColor BottomRightColor,
			SmoothRectangle Target,
			SmoothInt BevelThickness,
			SmoothPositionSide GapSide, 
			SmoothInt GapPos,
			SmoothInt GapSize)
{
	SmoothInt i;
	SmoothInt x, y, width, height;
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

	for (i=0;i<BevelThickness;i++) 
	{
		SmoothInt gap_thick = BevelThickness - i - 1;

		SmoothRectangleSetValues(&Target, x+i, y+i, width-i*2, height-i*2);
		
		SmoothDrawShadowWithGap(Canvas, TopLeftColor, BottomRightColor, Target,
					GapSide, GapPos+gap_thick, GapSize-gap_thick, 
					SmoothFalse);
	}  
}

static void
SmoothDrawCircularShadow(SmoothCanvas *Canvas,
				SmoothColor TopLeftColor,
				SmoothColor BottomRightColor,
				SmoothRectangle Target)
{
	SmoothInt x, y, width, height;
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

	SmoothCanvasSetPenColor(Canvas, TopLeftColor);
	SmoothCanvasDrawArc(Canvas, x, y, width, height, 45.0, 225.0);

	SmoothCanvasSetPenColor(Canvas, BottomRightColor);
	SmoothCanvasDrawArc(Canvas, x, y, width, height, 225.0, 180.0);
}

static void
SmoothDrawCircularBevel(SmoothCanvas *Canvas,
				SmoothColor TopLeftColor,
				SmoothColor BottomRightColor,
				SmoothRectangle Target,
				SmoothInt BevelThickness)
{
	SmoothInt i;
	SmoothInt x, y, width, height;
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

	for (i=0;i<BevelThickness;i++) 
	{
		SmoothRectangleSetValues(&Target, x+i, y+i, width-i*2, height-i*2);
		SmoothDrawCircularShadow(Canvas, TopLeftColor, BottomRightColor, Target);
	}  
}

static SmoothDouble default_shades_table[] = 
{
	0.666667,	/* DARK		*/
	1.2,		/* LIGHT	*/
	0.665,		/* ICED		*/
	0.4,		/* COLD		*/
	0.112		/* REDMOND	*/
};

SmoothBool
SmoothDrawBorderWithGap(SmoothBorder *Border,
				SmoothCanvas *Canvas,
				SmoothColor BaseColor,
				SmoothInt X,
				SmoothInt Y,
				SmoothInt Width,
				SmoothInt Height,
				SmoothPositionSide GapSide, 
				SmoothInt GapPos,
				SmoothInt GapSize)

{
	SmoothRectangle ta, ta2;
	SmoothColor color1, color2, color3, color4;

	SmoothColor darktone, lighttone, icetone, coldtone, redmondtone;
	SmoothColor midtone, midlighttone, middarktone, black;

	if ((Border->Type == SMOOTH_BORDER_TYPE_NONE) 
		|| (Border->Style == SMOOTH_BEVEL_STYLE_NONE)) 
	{
		return SmoothTrue;
  	}
	
	SmoothRectangleSetValues(&ta, X, Y, Width, Height);
	SmoothRectangleSetValues(&ta2, X+1, Y+1, Width-2, Height-2);
	
	SmoothCanvasCacheShadedColor(Canvas, BaseColor, default_shades_table[0], &darktone);

	if (Border->Style == SMOOTH_BEVEL_STYLE_FLAT) 
	{
		SmoothCanvasStore(Canvas);


		if ((Border->CornerRadius >= 1.0) && (Width == Height))
			SmoothDrawCircularShadow(Canvas, darktone, darktone, ta);
		else
		{
			SmoothDrawShadowWithGap(Canvas, darktone, darktone, ta, GapSide, GapPos, GapSize, TRUE);
		}
		
		SmoothCanvasRender(Canvas, SmoothFalse);

		SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[0], &darktone);

		return SmoothTrue;
	}   
    
	SmoothCanvasCacheShadedColor(Canvas, BaseColor, default_shades_table[1], &lighttone);
	
	switch (Border->Type)
	{
		case SMOOTH_BORDER_TYPE_ETCHED:
		case SMOOTH_BORDER_TYPE_ENGRAVED:
		{
			SmoothCanvasCacheMidPointColor(Canvas, darktone, lighttone, &midtone);

			color1 = lighttone;

			if (Border->Style == SMOOTH_BEVEL_STYLE_SOFT)      
				color2 = midtone;
			else  
				color2 = darktone;
	
			if (Border->Type == SMOOTH_BORDER_TYPE_ENGRAVED)
			{
				SmoothColor tmp;

				tmp = color2;
				color2 = color1;
				color1 = tmp;							
			}
			
			if ((Border->CornerRadius >= 1.0) && (Width == Height))
			{
			        SmoothDrawCircularShadow(Canvas, color2, color1, ta);
			        SmoothDrawCircularShadow(Canvas, color1, color2, ta2);
			} 
			else 
			{
				SmoothDrawShadowWithGap(Canvas, color2, color1, ta, GapSide, GapPos+1, GapSize-1, TRUE);
				SmoothDrawShadowWithGap(Canvas, color1, color2, ta2, GapSide, GapPos, GapSize-2, TRUE);
			}

			SmoothCanvasUnCacheMidPointColor(Canvas, darktone, lighttone, &midtone);
			SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[1], &lighttone);
			SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[0], &darktone);
		}	
		break;
      
		case SMOOTH_BORDER_TYPE_IN:
		case SMOOTH_BORDER_TYPE_OUT:
		{
			SmoothBool line_overlap = SmoothFalse, invert_in = SmoothTrue;

			if (Border->Style == SMOOTH_BEVEL_STYLE_BEVELED) 
			{
				color1 = darktone;				
				color2 = lighttone;

				if (Border->Type == SMOOTH_BORDER_TYPE_IN)
				{
					SmoothColor tmp;

					tmp = color2;
					color2 = color1;
					color1 = tmp;							
				}
				
				if ((Border->CornerRadius >= 1.0) && (Width == Height))
					SmoothDrawCircularBevel(Canvas, color2, color1, ta, Border->Thickness);
				else
				{
					SmoothDrawBevelWithGap(Canvas, color2, color1, ta, Border->Thickness, GapSide, GapPos, GapSize);					
				}
				SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[1], &lighttone);
				SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[0], &darktone);
			} 
			else if ((Border->Style == SMOOTH_BEVEL_STYLE_THIN) || (Border->Style == SMOOTH_BEVEL_STYLE_SOFT)) 
			{
				SmoothCanvasCacheMidPointColor(Canvas, darktone, lighttone, &midtone);

				color1 = (Border->Style == SMOOTH_BEVEL_STYLE_THIN)?darktone:midtone;
				color2 = lighttone;
				
				if (Border->Type == SMOOTH_BORDER_TYPE_IN)
				{
					SmoothColor tmp;

					tmp = color2;
					color2 = color1;
					color1 = tmp;							
				}

				if ((Border->CornerRadius >= 1.0) && (Width == Height))
					SmoothDrawCircularShadow(Canvas, color2, color1, ta);
				else
				{
					SmoothDrawShadowWithGap(Canvas, color2, color1, ta, GapSide, GapPos, GapSize, TRUE);
				}

				SmoothCanvasUnCacheMidPointColor(Canvas, darktone, lighttone, &midtone);
				SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[1], &lighttone);
				SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[0], &darktone);
			} 
			else 
			{
				SmoothCanvasCacheShadedColor(Canvas, BaseColor, default_shades_table[2], &icetone);
				SmoothCanvasCacheShadedColor(Canvas, BaseColor, default_shades_table[3], &coldtone);
				SmoothCanvasCacheShadedColor(Canvas, BaseColor, default_shades_table[4], &redmondtone);

				SmoothCanvasCacheMidPointColor(Canvas, darktone, lighttone, &midtone);
				SmoothCanvasCacheMidPointColor(Canvas, midtone, lighttone, &midlighttone);
				SmoothCanvasCacheMidPointColor(Canvas, midtone, darktone, &middarktone);

				SmoothColorSetValues(&black, 0, 0, 0, 0);				
				SmoothCanvasCacheColor(Canvas, &black);
				
				switch (Border->Style)
				{
					case SMOOTH_BEVEL_STYLE_SMOOTHED :
						if (Border->Type == SMOOTH_BORDER_TYPE_IN)
						{
							color1 = midtone;
							color2 = darktone;
							color3 = lighttone;
							color4 = midtone;
						}
						else
						{						
							color1 = midtone;
							color2 = lighttone;
							color3 = darktone;
							color4 = midtone;
						}
						
						line_overlap = SmoothFalse;
						invert_in = SmoothFalse;
					break;

					case SMOOTH_BEVEL_STYLE_SMOOTHEDBEVEL :
						color1 = lighttone;	
						color2 = midlighttone;
						color3 = middarktone;
						color4 = darktone;
						
						line_overlap = SmoothFalse;
						invert_in = SmoothTrue;
					break;

					case SMOOTH_BEVEL_STYLE_COLD:
						color1 = coldtone;
						color2 = lighttone;
						color3 = coldtone;
						color4 = midtone;
						
						line_overlap = SmoothFalse;
						invert_in = SmoothTrue;
					break;
            
					case SMOOTH_BEVEL_STYLE_ICED :
						color1 = icetone;
						color2 = BaseColor;
						color3 = icetone;
						color4 = BaseColor;
						
						line_overlap = SmoothFalse;
						invert_in = SmoothFalse;
					break;
            
					case SMOOTH_BEVEL_STYLE_REDMOND :
						if (Border->Type == SMOOTH_BORDER_TYPE_IN)
						{
							color1 = darktone;
							color2 = redmondtone;
							color3 = lighttone;
							color4 = BaseColor;
						} 
						else
						{
							color1 = lighttone;
							SmoothColorSetAlphaValue(&color2, 0);
							color3 = redmondtone;
							color4 = darktone;
						}
						
						line_overlap = SmoothTrue;
						invert_in = SmoothFalse;
					break;

					case SMOOTH_BEVEL_STYLE_STANDARD :
					default :
						if (Border->Type == SMOOTH_BORDER_TYPE_IN)
						{
							color1 = darktone;
							color2 = black;
							color3 = lighttone;
							color4 = midtone;
						}
						else
						{
							color1 = midtone;
							color2 = lighttone;
							color3 = black;
							color4 = darktone;
						}
						
						line_overlap = SmoothFalse;
						invert_in = SmoothFalse;
					break;
				} 

				if ((invert_in) && (Border->Type == SMOOTH_BORDER_TYPE_IN))
				{
					SmoothColor tmp;

					tmp = color3;
					color3 = color1;
					color1 = tmp;							

					tmp = color4;
					color4 = color2;
					color2 = tmp;							
				}

				if ((Border->CornerRadius >= 1.0) && (Width == Height))
				{
					SmoothDrawCircularShadow(Canvas, color1, color3, ta);
					SmoothDrawCircularShadow(Canvas, color2, color4, ta2);
				} 
				else 
				{											
					SmoothDrawShadowWithGap(Canvas, color1, color3, ta, GapSide, GapPos+1, GapSize-1, line_overlap);						
					SmoothDrawShadowWithGap(Canvas, color2, color4, ta2, GapSide, GapPos, GapSize, line_overlap);
				}  
				SmoothCanvasUnCacheColor(Canvas, &black);
				SmoothCanvasUnCacheMidPointColor(Canvas, midtone, darktone, &middarktone);
				SmoothCanvasUnCacheMidPointColor(Canvas, midtone, lighttone, &midlighttone);
				SmoothCanvasUnCacheMidPointColor(Canvas, darktone, lighttone, &midtone);


				SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[4], &redmondtone);
				SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[3], &coldtone);
				SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[2], &icetone);
				SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[1], &lighttone);
				SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[0], &darktone);
			} 	
		}	
		break;

		default:
		break;
	}
	
	return SmoothTrue;
}

SmoothBool
SmoothDrawPolygonBorder(SmoothBorder *Border,
				SmoothCanvas *Canvas,
				SmoothColor BaseColor,
				SmoothPoint *Points,
				SmoothInt Count)
{
	SmoothColor darktone, lighttone, icetone, coldtone, redmondtone;
	SmoothColor midtone, midlighttone, middarktone, black;
	SmoothColor color[4], use_color, mid_color;

	SmoothDouble		angle;
	SmoothInt		j,i, x1,y1, x2,y2, xt, yt, mx=0,my=0, sign, thickness;
	SmoothBool		invert_in = SmoothTrue;
	
	SmoothCanvasCacheShadedColor(Canvas, BaseColor, default_shades_table[0], &darktone);
	SmoothCanvasCacheShadedColor(Canvas, BaseColor, default_shades_table[1], &lighttone);
	SmoothCanvasCacheShadedColor(Canvas, BaseColor, default_shades_table[2], &icetone);
	SmoothCanvasCacheShadedColor(Canvas, BaseColor, default_shades_table[3], &coldtone);
	SmoothCanvasCacheShadedColor(Canvas, BaseColor, default_shades_table[4], &redmondtone);

	SmoothCanvasCacheMidPointColor(Canvas, darktone, lighttone, &midtone);
	SmoothCanvasCacheMidPointColor(Canvas, midtone, lighttone, &midlighttone);
	SmoothCanvasCacheMidPointColor(Canvas, midtone, darktone, &middarktone);

	SmoothColorSetValues(&black, 0, 0, 0, 0);				
	SmoothCanvasCacheColor(Canvas, &black);

	switch (Border->Style)
	{
		case SMOOTH_BEVEL_STYLE_BEVELED :
			color[0] = darktone;
			color[1] = lighttone;
			color[2] = darktone;
			color[3] = lighttone;
			
			invert_in = SmoothTrue;
			thickness = Border->Thickness - 1;
		break;
		
		case SMOOTH_BEVEL_STYLE_SMOOTHED :
			if (Border->Type == SMOOTH_BORDER_TYPE_IN)
			{
				color[0] = midtone;
				color[1] = darktone;
				color[2] = lighttone;
				color[3] = midtone;
			}
			else
			{						
				color[0] = midtone;
				color[1] = lighttone;
				color[2] = darktone;
				color[3] = midtone;
			}
					
			invert_in = SmoothFalse;
			thickness = 1;
		break;

		case SMOOTH_BEVEL_STYLE_SMOOTHEDBEVEL :
			color[0] = lighttone;	
			color[1] = midlighttone;
			color[2] = middarktone;
			color[3] = darktone;
					
			invert_in = SmoothTrue;
			thickness = 1;
		break;

		case SMOOTH_BEVEL_STYLE_COLD:
			color[0] = coldtone;
			color[1] = lighttone;
			color[2] = coldtone;
			color[3] = midtone;
			
			invert_in = SmoothTrue;
			thickness = 1;
		break;
            
		case SMOOTH_BEVEL_STYLE_ICED :
			color[0] = icetone;
			color[1] = BaseColor;
			color[2] = icetone;
			color[3] = BaseColor;
				
			invert_in = SmoothFalse;
			thickness = 1;
		break;
            
            
		case SMOOTH_BEVEL_STYLE_THIN :
		case SMOOTH_BEVEL_STYLE_SOFT :
			color[0] = lighttone;
			color[2] = (Border->Style == SMOOTH_BEVEL_STYLE_THIN)?darktone:midtone;

			invert_in = SmoothTrue;
			thickness = 0;
		break;


		case SMOOTH_BEVEL_STYLE_FLAT :
			color[0] = darktone;
			color[2] = darktone;

			invert_in = SmoothTrue;
			thickness = 0;
		break;

		case SMOOTH_BEVEL_STYLE_REDMOND :
			if (Border->Type == SMOOTH_BORDER_TYPE_IN)
			{
				color[0] = darktone;
				color[1] = redmondtone;
				color[2] = lighttone;
				color[3] = BaseColor;
			} 
			else
			{
				color[0] = lighttone;
				SmoothColorSetAlphaValue(&color[1], 0.0);
				color[2] = redmondtone;
				color[3] = darktone;
			}
					
			invert_in = SmoothFalse;
			thickness = 1;
		break;

		case SMOOTH_BEVEL_STYLE_STANDARD :
		default :
			if (Border->Type == SMOOTH_BORDER_TYPE_IN)
			{
				color[0] = darktone;
				color[1] = black;
				color[2] = lighttone;
				color[3] = midtone;
			}
			else
			{
				color[0] = midtone;
				color[1] = lighttone;
				color[2] = black;
				color[3] = darktone;
			}
						
			invert_in = SmoothFalse;
			thickness = 1;
		break;
	} 

	if ((invert_in) && (Border->Type == SMOOTH_BORDER_TYPE_IN))
	{
		SmoothColor tmp;

		tmp = color[2];
		color[2] = color[0];
		color[0] = tmp;							

		tmp = color[3];
		color[3] = color[1];
		color[1] = tmp;							
	}

	sign = 0;
	for (i = thickness; i >= 0; --i) 
	{
		for (j = 0; j < Count - 1; ++j) 
		{
			x1	= Points[j].x;
			y1	= Points[j].y;
			x2	= Points[j+1].x;
			y2	= Points[j+1].y;
		
			if ((x1 == x2) && (y1 == y2)) 
			{
				angle = 0;
			} 
			else 
			{
				angle = atan2 (y2 - y1, x2 - x1);
			}
	
			if ((angle > - (M_PI_4 * 3) - 0.0625) && (angle < M_PI_4 - 0.0625)) 
			{
				if (i!=1) 
				{
					use_color = color[2];
				} 
				else 
				{
					use_color = color[3];
				} 
					
				SmoothColorSetAlphaValue(&mid_color, 0.0);

				if (angle > -M_PI_4) 
				{
					y1  -= i;
					y2 -= i;
				} 	
				else
				{
					x1  -= i;
					x2 -= i;
				}
					
				if (sign != 0) 
				{
					sign = 0;
					mx = x1 + i;

					if (i!=1) 
					{
						mid_color = color[2];
					} 
					else 
					{
						mid_color = color[3];
					} 
				}
			} 
			else 
			{
				if (i!=1) 
				{
					use_color = color[0];
				} 
				else 
				{
					use_color = color[1];
				}  

				SmoothColorSetAlphaValue(&mid_color, 0.0);
					
				if ((angle < -(M_PI_4 * 3)) || (angle > (M_PI_4 * 3))) 
				{
					y1  += i;
					y2 += i;
				} 
				else 
				{
					x1  += i;
					x2 += i;
				}
			
				if (sign != 1) 
				{
					sign = 1;
					mx = x1 - i;

					if (i!=1) 
					{
						mid_color = color[0];
					} 
					else 
					{
						mid_color = color[1];
					} 
				}
			}
			
			my = y1;
	
			if (y2 < y1) 
			{
				xt = x1; x1 = x2; x2 = xt;
				yt = y1; y1 = y2; y2 = yt;
			}

			SmoothCanvasSetPenColor(Canvas, use_color);
			SmoothCanvasDrawLine(Canvas, x1, y1, x2, y2);

			if ((j > 0))
			{
				SmoothCanvasSetPenColor(Canvas, mid_color);
				SmoothCanvasDrawPoint(Canvas, mx, my);
			}	
		}
	}	

	SmoothCanvasUnCacheColor(Canvas, &black);
	SmoothCanvasUnCacheMidPointColor(Canvas, midtone, darktone, &middarktone);
	SmoothCanvasUnCacheMidPointColor(Canvas, midtone, lighttone, &midlighttone);
	SmoothCanvasUnCacheMidPointColor(Canvas, darktone, lighttone, &midtone);


	SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[4], &redmondtone);
	SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[3], &coldtone);
	SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[2], &icetone);
	SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[1], &lighttone);
	SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[0], &darktone);
	
	return SmoothTrue;
}  

/* single beveled line */
void
SmoothDrawLineBevel(SmoothLineBevel LineBevel,
			SmoothCanvas Canvas,
			SmoothColor BaseColor,

			SmoothInt Start,
			SmoothInt End,
			SmoothInt Base,
			SmoothBool Horizontal)
{
	SmoothColor darktone, lighttone, midtone;
	SmoothColor color1, color2;


	SmoothInt                thickness_light;
	SmoothInt                thickness_dark;
	SmoothInt                i;

	thickness_light = LineBevel.Thickness / 2;
	thickness_dark = LineBevel.Thickness - thickness_light;
  
	if (LineBevel.Style == SMOOTH_BEVEL_STYLE_NONE) return;
	
	SmoothCanvasCacheShadedColor(Canvas, BaseColor, default_shades_table[0], &darktone);
	SmoothCanvasCacheShadedColor(Canvas, BaseColor, default_shades_table[1], &lighttone);		
	SmoothCanvasCacheMidPointColor(Canvas, darktone, lighttone, &midtone);		

	if (LineBevel.Style == SMOOTH_BEVEL_STYLE_FLAT){
			color1=darktone;

			color2=darktone;
			SmoothColorSetAlphaValue(&color2, 0);
	} 
	else 
	{
		color1 = lighttone;

		if (LineBevel.Style == SMOOTH_BEVEL_STYLE_SOFT)
		{
			color2 = midtone;
		}
		else
		{
			color2 = darktone;
		}
	} 

	if (LineBevel.Style == SMOOTH_BEVEL_STYLE_FLAT) 
	{
		if (Horizontal) 
		{
			SmoothCanvasSetPenColor(Canvas, color1);
			SmoothCanvasDrawLine(Canvas, Start, Base, End, Base);      
		}
		else
		{
			SmoothCanvasSetPenColor(Canvas, color1);
			SmoothCanvasDrawLine(Canvas, Base, Start, Base, End);      
		}  
	}
	else
	{
		for (i = 0; i < thickness_dark; i++)
		{
			if (Horizontal) 
			{
				SmoothCanvasSetPenColor(Canvas, color1);
				SmoothCanvasDrawLine(Canvas, End - i, Base + i, End, Base + i);
	
				SmoothCanvasSetPenColor(Canvas, color2);
				SmoothCanvasDrawLine(Canvas, Start, Base + i, End - i, Base + i);
			} 
			else 
			{
				SmoothCanvasSetPenColor(Canvas, color1);
				SmoothCanvasDrawLine(Canvas, Base + i, End - i, Base + i, End);
	
				SmoothCanvasSetPenColor(Canvas, color2);
				SmoothCanvasDrawLine(Canvas, Base + i, Start, Base + i, End - i);
			}   
		}

		Base += thickness_dark;
	
		for (i = 0; i < thickness_light; i++)
		{
			if (Horizontal) 
			{
				SmoothCanvasSetPenColor(Canvas, color2);
				SmoothCanvasDrawLine(Canvas, Start, Base + i, Start + thickness_light - i - 1, Base + i);
		
				SmoothCanvasSetPenColor(Canvas, color1);
				SmoothCanvasDrawLine(Canvas, Start + thickness_light - i - 1, Base + i, End, Base + i);
			} 
			else 
			{
				SmoothCanvasSetPenColor(Canvas, color2);
				SmoothCanvasDrawLine(Canvas, Base + i, Start, Base + i, Start + thickness_light - i - 1);

				SmoothCanvasSetPenColor(Canvas, color1);
				SmoothCanvasDrawLine(Canvas, Base + i, Start + thickness_light - i - 1, Base + i, End);
			}   
		}
	}
	SmoothCanvasUnCacheMidPointColor(Canvas, darktone, lighttone, &midtone);		
	SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[1], &lighttone);		
	SmoothCanvasUnCacheShadedColor(Canvas, BaseColor, default_shades_table[0], &darktone);
}
