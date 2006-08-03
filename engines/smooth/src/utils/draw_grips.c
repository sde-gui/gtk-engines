/******************************************************************************/
/* draw_grips.c - Engine Agnostic Grip Part Drawing                           */
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
/* Portions Based on the CleanIce Engine                                      */
/*   Bastien Nocera <hadess@hadess.net>                                       */
/*   Rodney Dawes                                                             */
/*                                                                            */
/* Portions Based on the XFCE Engine                                          */
/*   Olivier Fourdan <fourdan@xfce.org>                                       */
/*                                                                            */
/* Portions Based on the Xenophilia Engine                                    */
/*   Johan Hanson <misagon@bahnhof.se>                                        */
/******************************************************************************/
#include "smooth_parts.h"

static SmoothInt smooth_modula(SmoothFloat number, SmoothInt divisor) 
{
  return (((SmoothInt)number % divisor) + (number - (SmoothInt)number));
}  

/* This function is taken for the most part from Xenophilia */
void 
do_smooth_draw_broken_bars(SmoothCanvas *Canvas,

			SmoothColor Highlight,
			SmoothColor Shadow,
			SmoothColor MidPoint,

			SmoothInt X, 
			SmoothInt Y, 
			SmoothInt Width, 
			SmoothInt Height, 

			SmoothBool Horizontal)
{
	SmoothInt i;

	if (Horizontal) 
	{
		for (i = Y + ((Height % 3) & 0x01); i < Y + Height; i += 3) 
		{
			SmoothCanvasSetPenColor(Canvas, Highlight);
			SmoothCanvasDrawLine(Canvas, X, i, X + Width - 2, i);
			
			SmoothCanvasSetPenColor(Canvas, Shadow);
			SmoothCanvasDrawLine(Canvas, X + 1, i + 1, X + Width - 1, i + 1);

			SmoothCanvasSetPenColor(Canvas, MidPoint);
			SmoothCanvasDrawPoint(Canvas, X, i + 1);
			SmoothCanvasDrawPoint(Canvas, X + Width - 1, i);
		}
	} 
	else 
	{
		for (i = X + ((Width % 3) & 0x01); i < X + Width; i += 3) 
		{
			SmoothCanvasSetPenColor(Canvas, Highlight);
			SmoothCanvasDrawLine(Canvas, i, Y, i, Y + Height - 2);

			SmoothCanvasSetPenColor(Canvas, Shadow);
			SmoothCanvasDrawLine(Canvas, i + 1, Y + 1, i + 1, Y + Height - 1);

			SmoothCanvasSetPenColor(Canvas, MidPoint);
			SmoothCanvasDrawPoint(Canvas, i + 1, Y);
			SmoothCanvasDrawPoint(Canvas, i, Y + Height - 1);
		}
	}
}

/* This function is based for the most part on XFCE (xfce_draw_handlers), and EnGradient*/
void
do_smooth_draw_lines(SmoothGripObjects *GripObjects,
		SmoothCanvas *Canvas,
		SmoothColor Highlight,
		SmoothColor Shadow,
		SmoothInt X, 
		SmoothInt Y, 
		SmoothInt Width, 
		SmoothInt Height, 
		SmoothBool Horizontal)
{
	SmoothInt i,offset;
	SmoothInt centerx=(Horizontal?(Width/2) + X:(Width/2) + X);
	SmoothInt centery=(Horizontal?(Height/2) + Y:(Height/2) + Y);
	SmoothInt xthick=GripObjects->Size, ythick=GripObjects->Size;
	SmoothDouble delta_offset = (GripObjects->Count < 2)?0:((GripObjects->Count - 1)*GripObjects->Spacing/2) + (GripObjects->Count) - 1 ;
	SmoothInt delta=((Horizontal?centerx-1:centery-1)) - delta_offset;

	if (GripObjects->Angle > 360)
	{
		GripObjects->Angle = smooth_modula(GripObjects->Angle, 360);
	}
	else
	{
		if (GripObjects->Angle < 0)
		{
			GripObjects->Angle = 360 - smooth_modula(ABS(GripObjects->Angle), 360);
		}
	}

	if  ((GripObjects->Angle == 0) || (GripObjects->Angle == 360))
	{
		if (Horizontal)
		{
			xthick = 0;
			ythick = GripObjects->Size;
		}	
		else	
		{
			xthick = GripObjects->Size;
			ythick = 0;
		}	
	}
	else
	{
		if (Horizontal)
		{
			xthick = GripObjects->Size*cos(GripObjects->Angle*M_PI/180);
			ythick = GripObjects->Size*sin(GripObjects->Angle*M_PI/180);
		}	
		else	
		{
			xthick = GripObjects->Size*sin(GripObjects->Angle*M_PI/180);
			ythick = GripObjects->Size*cos(GripObjects->Angle*M_PI/180);
		}			
	}
	
	for (i = 0, offset = delta; (i < GripObjects->Count); offset += (GripObjects->Spacing + 2), i++)
	{
		if (Horizontal && ((offset - xthick) >= (X + GripObjects->CutOff)) && ((offset + xthick + 1) <= (X + Width - GripObjects->CutOff)))
		{
			SmoothCanvasSetPenColor(Canvas, Shadow);
			SmoothCanvasDrawLine(Canvas, 
							offset + xthick, centery - ythick, 
							offset - xthick, centery + ythick);
			
			SmoothCanvasSetPenColor(Canvas, Highlight);
			SmoothCanvasDrawLine(Canvas, 
							offset + xthick + 1, centery - ythick, 
							offset - xthick + 1, centery + ythick);
		} 
		else if (!Horizontal && ((offset - ythick) >= (Y + GripObjects->CutOff)) && ((offset + ythick + 1) <= (Y + Height - GripObjects->CutOff)))
		{
			SmoothCanvasSetPenColor(Canvas, Shadow);
			SmoothCanvasDrawLine(Canvas, 
							centerx - xthick, offset + ythick, 
							centerx + xthick, offset - ythick);

			SmoothCanvasSetPenColor(Canvas, Highlight);
			SmoothCanvasDrawLine(Canvas, 
							centerx - xthick, offset + ythick + 1, 
							centerx + xthick, offset - ythick + 1);
		}
	}
}

/* This function is taken for the most part from CleanIce */
static void
smooth_internal_draw_dot(SmoothCanvas *Canvas,

			SmoothInt Size,
			
			SmoothColor Highlight,
			SmoothColor Shadow,
			SmoothColor MidPoint,

			SmoothInt DotX, 
			SmoothInt DotY, 

			SmoothInt X, 
			SmoothInt Y, 
			SmoothInt Width, 
			SmoothInt Height, 
			
			SmoothInt XCutoff,
			SmoothInt YCutoff,
			
			SmoothBool Horizontal)
{
	if (Size == 2) 
	{
		if (((Horizontal) && ((DotX) >= (X + XCutoff)) && ((DotX + 1) <= (X + Width - 2*XCutoff))) ||
			((!Horizontal) && ((DotY) >= (Y + YCutoff)) && ((DotY + 1) <= (Y + Height - 2*YCutoff))))
		{	
			SmoothCanvasSetPenColor(Canvas, Highlight);

			SmoothCanvasDrawPoint(Canvas, DotX - 0, DotY - 0);


			SmoothCanvasSetPenColor(Canvas, Shadow);

			SmoothCanvasDrawPoint(Canvas, DotX + 1, DotY + 1);

			SmoothCanvasSetPenColor(Canvas, MidPoint);

			SmoothCanvasDrawPoint(Canvas, DotX + 1, DotY - 0);
			SmoothCanvasDrawPoint(Canvas, DotX - 0, DotY + 1);
		}	
	} 
	else if (Size == 3) 
	{
		if (((Horizontal) && ((DotX - 1) >= (X + XCutoff + 2)) && ((DotX + 1) <= (X + Width - XCutoff*2 + 2))) ||
			((!Horizontal) && ((DotY - 1) >= (Y + YCutoff )) && ((DotY + 1) <= (Y + Height - YCutoff*2 + 2))))
		{
			SmoothCanvasSetPenColor(Canvas, Highlight);

			SmoothCanvasDrawPoint(Canvas, DotX - 1, DotY + 0);
			SmoothCanvasDrawPoint(Canvas, DotX - 1, DotY - 1);
			SmoothCanvasDrawPoint(Canvas, DotX + 0, DotY - 1);

			SmoothCanvasSetPenColor(Canvas, Shadow);

			SmoothCanvasDrawPoint(Canvas, DotX + 1, DotY - 0);
			SmoothCanvasDrawPoint(Canvas, DotX + 1, DotY + 1);
			SmoothCanvasDrawPoint(Canvas, DotX - 0, DotY + 1);

			SmoothCanvasSetPenColor(Canvas, MidPoint);
				
			SmoothCanvasDrawPoint(Canvas, DotX + 1, DotY - 1);
			SmoothCanvasDrawPoint(Canvas, DotX - 1, DotY + 1);
		}   
	}	
}

void
do_smooth_draw_dots(SmoothGripObjects *GripObjects,
		SmoothCanvas *Canvas,
		SmoothColor Highlight,
		SmoothColor Shadow,
		SmoothColor MidPoint,
		SmoothInt X, 
		SmoothInt Y, 
		SmoothInt Width, 
		SmoothInt Height, 
		SmoothBool Horizontal)
{
	SmoothInt i, dot_x, dot_y;
	SmoothInt centerx=(Horizontal?(Width/2) + X:(Width/2) + X);
	SmoothInt centery=(Horizontal?(Height/2) + Y:(Height/2) + Y);
	SmoothDouble delta_offset = (GripObjects->Count < 2)?0:((GripObjects->Count - 1)*GripObjects->Spacing/2) + (GripObjects->Count*GripObjects->Size/2) - (GripObjects->Size%2);
	SmoothInt delta=((Horizontal?centerx:centery)) - delta_offset;

	for (i=0; i < GripObjects->Count*(GripObjects->Spacing + GripObjects->Size); i += (GripObjects->Spacing + GripObjects->Size))
	{
		if (Horizontal) 
		{		
			dot_x = i + delta;
			dot_y = centery;
		} 
		else 
		{
			dot_x = centerx;
			dot_y = i + delta;
		}

		smooth_internal_draw_dot(Canvas, GripObjects->Size, Highlight, Shadow, MidPoint, dot_x, dot_y, X, Y, Width, Height, GripObjects->CutOff, GripObjects->CutOff, Horizontal);
	}
}

/* This function is taken for the most part from Xenophilia */

void 
do_smooth_draw_buds(SmoothCanvas *Canvas,
		SmoothColor Highlight,
		SmoothColor Shadow,
		SmoothColor MidPoint,
		SmoothInt X, 
		SmoothInt Y, 
		SmoothInt Width, 
		SmoothInt Height, 
		SmoothInt ClipOffset,
		SmoothBool Horizontal,
		SmoothBool Alternating)
{
	SmoothInt dot_x, dot_y;
   
	if (Alternating && !Horizontal) 
	{
		X -= (Width % 2) & 0x01;
		Y -= (Height% 2) & 0x01;
		
		/* netscape - style */
		for (dot_y = Y + 1; dot_y < Y + Height - 1; dot_y += 3) 
		{
			for (dot_x = X; dot_x < X + Width - 1; dot_x += 6) 
			{
				smooth_internal_draw_dot(Canvas, 2,
							Highlight, Shadow, MidPoint, 
							dot_x, dot_y, 
							X, Y, Width, Height, 
							ClipOffset, ClipOffset, 
							Horizontal);
			}
		}
		
		for (dot_y = Y; dot_y < Y + Height - 1; dot_y += 3) 
		{
			for (dot_x = X + 3; dot_x < X + Width-1; dot_x += 6) 
			{
				smooth_internal_draw_dot(Canvas, 2,
							Highlight, Shadow, MidPoint, 
							dot_x, dot_y, 
							X, Y, Width, Height, 
							ClipOffset, ClipOffset, 
							Horizontal);
			}
		}
	} 
	else 
	{
		/* mac - style */
		X += (Width % 3) & 0x01;
		Y += (Height% 3) & 0x01;
      
		for (dot_y = Y; dot_y < Y + Height - 1; dot_y += 3) 
		{
			for (dot_x = X; dot_x < X + Width - 1; dot_x += 3) 
			{
				smooth_internal_draw_dot(Canvas, 2,
							Highlight, Shadow, MidPoint, 
							dot_x, dot_y, 
							X, Y, Width, Height, 
							ClipOffset, ClipOffset, 
							Horizontal);
			}
		}
	}
}
