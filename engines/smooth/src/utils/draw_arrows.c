/******************************************************************************/
/* draw_arrows.c - Engine Agnostic Arrow Part Drawing                         */
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
/******************************************************************************/
#include "smooth_parts.h"

/* This function is based on XFCE's & CleanIce draw arrow routines, both which  were based on ThinIce's */
static void
SmoothDrawCleanArrow(SmoothCanvas *Canvas,
			SmoothArrow Arrow,
			SmoothRectangle Target,
			SmoothColor Color,
			SmoothBool OutsideEdge)
{
	SmoothInt i;

	SmoothInt steps, extra;
	SmoothInt start, increment;
	SmoothInt x, y, width, height, aw, ah;
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);
	aw = width;
	ah = height;
	
	SmoothCanvasSetPenColor(Canvas, Color);

	SmoothCanvasStore(Canvas);

	if ((Arrow.Type == SMOOTH_ARROW_TYPE_UP) || (Arrow.Type == SMOOTH_ARROW_TYPE_DOWN))
	{
		SmoothDouble tmp=((aw+1)/2) - ((height%2)?1:0);
      
		if (tmp > ah) 
		{
			aw = 2*ah - 1 - ((height%2)?1:0);
			ah = (aw+1)/2;
		} 
		else 
		{
			ah = (SmoothInt) tmp;
			aw = 2*ah - 1;
		}  

		if ((aw < 5) || (ah < 3)) 
		{
			aw = 5;
			ah = 3;
		}

		ah += Arrow.Tail;

		x += (width - aw) / 2 ;
		y += (height - ah) / 2;
		width = aw;
		height = ah;
		
		/* W/H Ratio Only Works If Forced Odd 
		 */
		width += width % 2 - 1;

		/* The Arrow W/H is always the same ratio, so any extra
		 * space is drawn as an extension to the base.
		 */
		steps = 1 + width / 2;
		extra = height - steps;

		/* Drawing is in the direction of the arrow, that is,
		 * Starts From the Base, and increments, or decrements, 
		 * to the Point.
		 */
		if (Arrow.Type == SMOOTH_ARROW_TYPE_DOWN)
		{
			start = y;
			increment = 1;
		}
		else
		{
			start = y + height - 1;
			increment = -1;
		}

		/* Draw Any Extra Space As an extension off the Base
		 * i.e, as a tail.
		 */
		for (i = 0; i < extra; i++)
		{
			if (OutsideEdge)
			{
				if (i == 0)
				{
					SmoothCanvasDrawLine(Canvas, x, start + i * increment, x + width - 1, start + i * increment);
				}
				else
				{
					SmoothCanvasDrawLine(Canvas, x, start + i * increment, x, start + i * increment);
					SmoothCanvasDrawLine(Canvas, x + width - 1, start + i * increment, x + width - 1, 
								start + i * increment);
				}
			}	
			else
				SmoothCanvasDrawLine(Canvas, x, start + i * increment, x + width - 1, start + i * increment);
		}

		/* Finally Draw Actual Arrow
		 */
		for (; i < height; i++)
		{
			if (OutsideEdge)
			{
				if (i == 0)
				{
					SmoothCanvasDrawLine(Canvas, x + (i - extra), start + i * increment, x + width - (i - extra) - 1,
								start + i * increment);
				}
				else
				{
					SmoothCanvasDrawLine(Canvas, x + (i - extra), start + i * increment, x + (i - extra), 
								start + i * increment);
					SmoothCanvasDrawLine(Canvas, x + width - (i - extra) - 1, start + i * increment, 
								x + width - (i - extra) - 1, start + i * increment);
				}
			}	
			else
				SmoothCanvasDrawLine(Canvas, x + (i - extra), start + i * increment, x + width - (i - extra) - 1, 
							start + i * increment);
		}
	}
	else
	{
		SmoothDouble tmp=((ah+1)/2) - ((width%2)?1:0);

		if (tmp > aw) 
		{
			ah = 2*aw - 1 - ((width%2)?1:0);
			aw = (ah+1)/2;
		} 
		else 
		{
			aw = (SmoothInt) tmp;
			ah = 2*aw - 1;
		}  

		if ((ah < 5) || (aw < 3)) 
		{
			ah = 5;
			aw = 3;
		}

		aw += Arrow.Tail;

		x += (width - aw) / 2 ;
		y += (height - ah) / 2;
		width = aw;
		height = ah;

		/* W/H Ratio Only Works If Forced Odd 
		 */
		height += height % 2 - 1;

		/* The Arrow W/H is always the same ratio, so any extra
		 * space is drawn as an extension to the base.
		 */
		steps = 1 + height / 2;
		extra = width - steps;

		/* Drawing is in the direction of the arrow, that is,
		 * Starts From the Base, and increments, or decrements, 
		 * to the Point.
		 */
		if (Arrow.Type == SMOOTH_ARROW_TYPE_RIGHT)
		{
			start = x;
			increment = 1;
		}
		else
		{
			start = x + width - 1;
			increment = -1;
		}

		/* Draw Any Extra Space As an extension off the Base
		 * i.e, as a tail.
		 */
		for (i = 0; i < extra; i++)
		{
			if (OutsideEdge)
			{
				if (i == 0)
				{
					SmoothCanvasDrawLine(Canvas, start + i * increment , y, start + i * increment, y + height - 1);
				}
				else
				{
					SmoothCanvasDrawLine(Canvas, start + i * increment , y, start + i * increment, y);
					SmoothCanvasDrawLine(Canvas, start + i * increment , y + height - 1, start + i * increment, 
								y + height - 1);
				}
			}	
			else
				SmoothCanvasDrawLine(Canvas, start + i * increment , y, start + i * increment, y + height - 1);
		}

		/* Finally Draw Actual Arrow
		 */
		for (; i < width; i++)
		{
			if (OutsideEdge)
			{
				if (i == 0)
				{
					SmoothCanvasDrawLine(Canvas,  start + i * increment, y + (i - extra), start + i * increment, 
								y + height - (i - extra) - 1);
				}
				else
				{
					SmoothCanvasDrawLine(Canvas,  start + i * increment, y + (i - extra), start + i * increment, 
							y + (i - extra));	
					SmoothCanvasDrawLine(Canvas,  start + i * increment, y + height - (i - extra) - 1, 
								start + i * increment, y + height - (i - extra) - 1);
				}
			}	
			else
				SmoothCanvasDrawLine(Canvas,  start + i * increment, y + (i - extra), start + i * increment, 
							y + height - (i - extra) - 1);
		}
	}

	SmoothCanvasRender(Canvas, SmoothFalse);
}

/* This function is based on EnGradient/IceGradient's draw arrow routines */
static void
SmoothDrawDirtyArrow(SmoothCanvas *Canvas,
			SmoothArrowType ArrowType,
			SmoothRectangle Target,
			SmoothColor Color,
			SmoothBool OutsideEdge)
{
	SmoothInt x, y, width, height;
	SmoothInt size, half_size, tail=0;
	
	SmoothPoint points[4];

	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

	size = MIN(width + (width % 2 - 1), height + (height % 2 - 1));  
	half_size = size / 2;

	x += (width - size)/2;
	y += (height - size)/2;
	
	switch (ArrowType) {
		case SMOOTH_ARROW_TYPE_UP:
			if (size - height > 0)
			{
				tail = size - height;
				/* y += (height - size)/2; */
			}

			SmoothPointSetValues(&points[0], x + half_size, y);
			SmoothPointSetValues(&points[1], x, y + size - 1);

			SmoothPointSetValues(&points[2], x + size - 1, y + size - 1);
			SmoothPointSetValues(&points[3], x + half_size, y);
		break;

		case SMOOTH_ARROW_TYPE_DOWN:			
			if (size - height > 0)
			{
				tail = size - height;
				/* y += (height - size)/2; */
			}

			SmoothPointSetValues(&points[0], x + half_size, y + size - 1);
			SmoothPointSetValues(&points[1], x + size - 1, y );

			SmoothPointSetValues(&points[2], x, y);
			SmoothPointSetValues(&points[3], x + half_size, y + size - 1);
		break;

		case SMOOTH_ARROW_TYPE_LEFT:
			if (size - height > 0)
			{
				tail = size - width;
				/* x += (width - size)/2; */
			}

			SmoothPointSetValues(&points[0], x, y + half_size);
			SmoothPointSetValues(&points[1], x + size - 1, y  + size - 1);

			SmoothPointSetValues(&points[2], x + size - 1, y);
			SmoothPointSetValues(&points[3], x, y + half_size);
		break;

		case SMOOTH_ARROW_TYPE_RIGHT:
			if (size - height > 0)
			{
				tail = size - width;
				/* x += (width - size)/2; */
			}

			SmoothPointSetValues(&points[0], x + size - 1, y + half_size);
			SmoothPointSetValues(&points[1], x, y);

			SmoothPointSetValues(&points[2], x, y + size - 1);
			SmoothPointSetValues(&points[3], x + size - 1, y + half_size);
		break;
		
		default:
		break;
	}    

	if (!OutsideEdge)
	{
		SmoothCanvasSetBrushColor(Canvas, Color);

		SmoothCanvasFillPolygon(Canvas, points, 4);
	}	

	SmoothCanvasSetPenColor(Canvas, Color);
	
	SmoothCanvasDrawLines(Canvas, points, 4);
}

/* This function is based on the Wonderland theme engine, 
 * it is essentially calculate_arrow_geometry and draw_arrow,
 * concatted into one composite whole...
 */
static void
SmoothDrawSlickArrow(SmoothCanvas *Canvas,
			SmoothArrowType ArrowType,
			SmoothRectangle Target,
			SmoothColor Color,
			SmoothBool OutsideEdge)
{
	SmoothInt i;

	SmoothInt x, y, width, height;
	SmoothInt w, h, offset1, offset2;

	SmoothBool base_line;	
	SmoothInt base, increment;
	SmoothInt x1, x2, y1, y2;
		
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

	w = width;
	h = height;

	SmoothCanvasSetPenColor(Canvas, Color);
  
	SmoothCanvasStore(Canvas);

	if ((ArrowType == SMOOTH_ARROW_TYPE_UP) || (ArrowType == SMOOTH_ARROW_TYPE_DOWN))
	{
		w += (w % 2) - 1;
		h = (w / 2 + 1) + 1;

		if (h > height)
		{
			h = height;
			w = 2 * (h - 1) - 1;
		}
      
		
		/* Drawing is in the direction of the arrow, that is,
		 * Starts From the Base, and increments, or decrements, 
		 * to the Point.
		 */
		if (ArrowType == SMOOTH_ARROW_TYPE_DOWN)
		{
			if (height % 2 == 1 || h % 2 == 0)
			{
				height += 1;
			}
			
			base = 0;
			increment = 1;
		}
		else
		{
			if (height % 2 == 0 || h % 2 == 0)
			{
				height -= 1;
			}	

			base = h - 1;
			increment = -1;
		}

		x += (width - w) / 2;
		y += (height - h) / 2;

		/* Finally Draw Actual Arrow
		 */
		for (i = 0; i < h; i++)
		{
			x1 = x + i - 1;
			x2 = x + w - i;
			
			y1 = y2 = y + base + i * increment;
			base_line = (y1 - y) == base;
			
			if ((ABS(x2 - x1) < 7) && (!base_line))
			{
				SmoothCanvasDrawLine(Canvas, x1, y1, x2, y2);
			}		
			else
			{
				offset1 = ((ABS(x2 - x1) > 7) || (!base_line))?2:1;
				offset2 = (base_line)?offset1:0;

				SmoothCanvasDrawLine(Canvas, x1 + offset2, y1, x1 + offset1, y1);

				SmoothCanvasDrawLine(Canvas, x2 - offset1, y2, x2 - offset2, y2);
			}
		}

	}
	else
	{
		h += (h % 2) - 1;
		w = (h / 2 + 1) + 1; 
      
		if (w > width)
		{
			w = width;
			h = 2 * (w - 1) - 1;
		}
      
		/* Drawing is in the direction of the arrow, that is,
		 * Starts From the Base, and increments, or decrements, 
		 * to the Point.
		 */
		if (ArrowType == SMOOTH_ARROW_TYPE_RIGHT)
		{
			if ((width % 2 == 1) || (w % 2 == 0))
			{
				width += 1;
			}
			
			base = 0;
			increment = 1;
		}
		else
		{
			if ((width % 2 == 0) || (w % 2 == 0))
			{
				width -= 1;
			}	
			
			base = w;
			increment = -1;
			x -= 1;
		}

		x += (width - w) / 2;
		y += (height - h) / 2;

		/* Finally Draw Actual Arrow
		 */
		for (i = 0; i < w; i++)
		{
			y1 = y + i - 1;
			y2 = y + h - i;

			x1 = x2 = x + base + i * increment;
			base_line = (x1 - x) == base;

			if ((ABS(y2 - y1) < 7) && (!base_line))
			{
				SmoothCanvasDrawLine(Canvas, x1, y1, x2, y2);
			}		
			else
			{
				offset1 = (base_line)?2:0;
	
				SmoothCanvasDrawLine(Canvas, x1, y1 + offset1, x1, y1 + 2);

				SmoothCanvasDrawLine(Canvas, x2, y2 - 2, x2, y2 - offset1);
			}	
		}
	}

	SmoothCanvasRender(Canvas, SmoothFalse);
}

/* This function is another home grown routine simply for the
 * purpose of abstracting the multiple draw_arrow routines, so
 * that modifications such as new variants could easily be added 
 * later on with virtually no modifications anywhere else.
 */
static SmoothDouble default_shades_table[] = 
{
	0.666667,	/* DARK		*/
	1.2,		/* LIGHT	*/
	0.665,		/* ICED		*/
	0.4,		/* COLD		*/
	0.112		/* REDMOND	*/
};

void
SmoothDrawArrow(SmoothCanvas *Canvas,
		SmoothArrow Arrow,
		SmoothInt X,
		SmoothInt Y,
		SmoothInt Width,
		SmoothInt Height,
		SmoothColor Background,
		SmoothColor Foreground)
{
	SmoothRectangle ta;
	SmoothColor border, fill;

	if (Arrow.Etched) 
	{
		SmoothCanvasCacheShadedColor(Canvas, Background, default_shades_table[0], &fill);
		SmoothCanvasCacheShadedColor(Canvas, Background, default_shades_table[1], &border);
	} 
	else
	{
		fill = Background;
		border = Foreground;
		SmoothCanvasCacheColor(Canvas, &fill);
		SmoothCanvasCacheColor(Canvas, &border);	
	}

	switch (Arrow.Style) {
		case SMOOTH_ARROW_STYLE_CLEAN :       
			if (Arrow.Solid || Arrow.Etched)
			{
				if (Arrow.Etched)
				{
					SmoothRectangleSetValues(&ta, X + 1, Y + 1, Width, Height);
					SmoothDrawCleanArrow(Canvas, Arrow, ta, border, FALSE);
				}

				SmoothRectangleSetValues(&ta, X, Y, Width, Height);
				SmoothDrawCleanArrow(Canvas, Arrow, ta, fill, FALSE);
			}	
			else
			{
				SmoothRectangleSetValues(&ta, X, Y, Width, Height);
				SmoothDrawCleanArrow(Canvas, Arrow, ta, fill, FALSE);

				SmoothDrawCleanArrow(Canvas, Arrow, ta, border, TRUE);
			}	

		break;

		case SMOOTH_ARROW_STYLE_DIRTY : 
			{
				SmoothColor MidAA;
					
				SmoothCanvasCacheMidPointColor(Canvas, Background, Foreground, &MidAA);

				if (Arrow.Etched)
				{					
					Width -= 1;
					Height -= 1;

					SmoothRectangleSetValues(&ta, X + 1, Y + 1, Width, Height);
					SmoothDrawDirtyArrow(Canvas, Arrow.Type, ta, border, FALSE);

					SmoothRectangleSetValues(&ta, X - 1, Y - 1, Width + 1, Height + 2);
				}
				else
				{
					SmoothRectangleSetValues(&ta, X - 1, Y - 1, Width + 1, Height + 1);
				}
	
				if (Arrow.Solid || Arrow.Etched)
				{
					SmoothDrawDirtyArrow(Canvas, Arrow.Type, ta, MidAA, FALSE);
				}
				
				SmoothRectangleSetValues(&ta, X, Y, Width, Height);
				SmoothDrawDirtyArrow(Canvas, Arrow.Type, ta, fill, FALSE);
			
				if (!Arrow.Etched && !Arrow.Solid)
				{
					SmoothDrawDirtyArrow(Canvas, Arrow.Type, ta, border, TRUE);
				}

				SmoothCanvasUnCacheMidPointColor(Canvas, Background, Foreground, &MidAA);
			}	
		break;

		case SMOOTH_ARROW_STYLE_SLICK : 
			if (Arrow.Etched)
			{
				SmoothRectangleSetValues(&ta, X + 1, Y + 1, Width, Height);
				SmoothDrawSlickArrow(Canvas, Arrow.Type, ta, border, TRUE);
			}

			if (Arrow.Solid && (!Arrow.Etched))
			{
				SmoothRectangleSetValues(&ta, X, Y, Width, Height);
				SmoothDrawSlickArrow(Canvas, Arrow.Type, ta, fill, TRUE);
			} 
			else
			{
				SmoothRectangleSetValues(&ta, X, Y, Width, Height);
				SmoothDrawSlickArrow(Canvas, Arrow.Type, ta, border, TRUE);
			}
		break;

		case SMOOTH_ARROW_STYLE_XPM : 
		default :
			Arrow.Style = SMOOTH_ARROW_STYLE_CLEAN;
			Arrow.Tail = 0;
			SmoothDrawArrow(Canvas, Arrow, X, Y, Width, Height, Background, Foreground);
		break;
	}

	if (Arrow.Etched) 
	{
		SmoothCanvasUnCacheShadedColor(Canvas, Background, default_shades_table[0], &fill);
		SmoothCanvasUnCacheShadedColor(Canvas, Background, default_shades_table[1], &border);
	} 
	else
	{
		SmoothCanvasUnCacheColor(Canvas, &fill);
		SmoothCanvasUnCacheColor(Canvas, &border);	
	}
}


void SmoothFreeArrowStyles(SmoothArrowPart *arrow)
{
	if (arrow)
	{
		gint i;

		if (arrow->DefaultStyle)
		{
			g_free(arrow->DefaultStyle);
			arrow->DefaultStyle = NULL;
		}

		if (arrow->DefaultStateStyles)
		{			
			g_free(arrow->DefaultStateStyles);
			arrow->DefaultStateStyles = NULL;
		}

		if (arrow->DefaultTypeStyles)
		{
			g_free(arrow->DefaultTypeStyles);
			arrow->DefaultTypeStyles = NULL;
		}

		for (i=0; i < 5; i++) 
		{
			if (arrow->Styles[i])
			{
				g_free(arrow->Styles[i]);
				arrow->Styles[i] = NULL;
			}
		}
	}
}
