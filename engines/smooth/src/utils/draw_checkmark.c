/******************************************************************************/
/* draw_checkmark.c - Engine Agnostic Checkmark Part Drawing                  */
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
/******************************************************************************/
#include "smooth_parts.h"

static void
SmoothDrawCleanCheckMark(SmoothCanvas *Canvas,
				SmoothRectangle Target,
				SmoothColor Color)
{
	SmoothInt x, y, width, height;
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

	SmoothCanvasSetPenColor(Canvas, Color);

	SmoothCanvasStore(Canvas);

        /* Short Diagonal */
	SmoothCanvasDrawLine(Canvas, x + 1, y + height - 6, x + 1, y + height - 4);/* Left Line */
        SmoothCanvasDrawLine(Canvas, x + 2, y + height - 5, x + 2, y + height - 3);/* Right Line */

        /* Long Diagonal */
        SmoothCanvasDrawLine(Canvas, x + 3, y + height - 4, x + width - 2, y + 1);/* Top Line */
        SmoothCanvasDrawLine(Canvas, x + 3, y + height - 3, x + width - 2, y + 2);/* Center Line */
        SmoothCanvasDrawLine(Canvas, x + 3, y + height - 2, x + width - 2, y + 3);/* Bottom Line */

	SmoothCanvasRender(Canvas, SmoothFalse);
}

static void
SmoothDrawFastCheckMark(SmoothCanvas *Canvas,
			SmoothRectangle Target,
			SmoothColor Color)
{
	SmoothInt x, y, width, height;
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

	SmoothCanvasSetPenColor(Canvas, Color);

	SmoothCanvasStore(Canvas);

	SmoothCanvasDrawLine(Canvas, x, y+height-1, x, y+(height/2)-1);
        SmoothCanvasDrawLine(Canvas, x, y+height-1, x, y+(height/2)-1);
        SmoothCanvasDrawLine(Canvas, x+1, y+height-1, x+1, y+(height/2)-1);
        SmoothCanvasDrawLine(Canvas, x+1, y+height-1, x+width, y - 1);
        SmoothCanvasDrawLine(Canvas, x, y+height-1, x+width, y);

	SmoothCanvasRender(Canvas, SmoothFalse);
}

static void
SmoothDrawCrossCheckMark(SmoothCanvas *Canvas,
				SmoothRectangle Target,
				SmoothColor Color)
{
	SmoothInt x, y, width, height;
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

	SmoothCanvasSetPenColor(Canvas, Color);

        if (width >= 16) 
	{
		SmoothInt off_1, off_2, off_3;
		SmoothInt line_width = ceil(((width + 1)/5));
	
		if (!(line_width % 2)) 
			line_width -= 1;

		SmoothCanvasSetPenThickness(Canvas, line_width);
		/* SmoothCanvasSetPenCap(Canvas, ROUND_CAP); */

		off_2 = line_width;
		off_1 = off_2 - 1;
		off_3 = off_2 + 1;


		SmoothCanvasStore(Canvas);

		/* Backward Diagonal */
		SmoothCanvasDrawLine(Canvas, x+off_1, y+off_1, x+width-off_2, y+height-off_2);/* Center Line */

		/* Forward Diagonal */
		SmoothCanvasDrawLine(Canvas, x+off_1, y+height - off_2, x+width-off_2, y+off_1);/* Center Line */
	} 
	else 
	{
		SmoothCanvasStore(Canvas);

		/* Backward Diagonal */
		if (width>=7) 
			SmoothCanvasDrawLine(Canvas, x+2, y+1, x+width-2, y+height-3);/* Top Line */

		SmoothCanvasDrawLine(Canvas, x+1, y+1, x+width-2, y+height-2);/* Center Line */

		if (width>=7) 
			SmoothCanvasDrawLine(Canvas, x+1, y+2, x+width-3, y+height-2);/* Bottom Line */

		/* Forward Diagonal */
		if (width>=7) 
			SmoothCanvasDrawLine(Canvas, x+1, y+height-3, x+width-3, y+1);/* Top Line */

		SmoothCanvasDrawLine(Canvas, x+1, y+height - 2, x+width-2, y+1);/* Center Line */

		if (width>=7) 
			SmoothCanvasDrawLine(Canvas, x+2, y+height - 2, x+width-2, y+2);/* Bottom Line */
	}		

	SmoothCanvasRender(Canvas, SmoothFalse);
}

static void
SmoothDrawMinusCheckMark(SmoothCanvas *Canvas,
				SmoothRectangle Target,
				SmoothColor Color)
{
	SmoothInt x, y, width, height;
	SmoothInt line_width = 0;
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

	SmoothCanvasSetBrushColor(Canvas, Color);

	/* Rough logic, just make  thickness 1/3 height */
	line_width = ceil((height + 1)/3);

	/* Force Thickness Even */
	line_width -= (line_width % 2);

        SmoothCanvasFillRectangle(Canvas, x + 1, y + ((height - line_width)/2), width - 2, line_width + ((height % 2)?1:0));
}

static void
SmoothDrawBlockCheckMark(SmoothCanvas *Canvas,
				SmoothRectangle Target,
				SmoothColor Color)
{
	SmoothInt x, y, width, height;
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

	x += 1;
	y += 1; 
	width -= 2;
        height -= 2; 

	SmoothCanvasSetBrushColor(Canvas, Color);

        SmoothCanvasFillRectangle(Canvas, x, y, width, height);
}

static void
SmoothDrawCircleCheckMark(SmoothCanvas *Canvas,
				SmoothRectangle Target,
				SmoothColor Color)
{
	SmoothInt x, y, width, height;
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

	x += width / 4;
	y += height / 4; 
	width = width / 2;
        height = height / 2; 

	SmoothCanvasSetBrushColor(Canvas, Color);

        SmoothCanvasFillChord(Canvas, x, y, width, height, 0, 360*64);
}

static void
SmoothDrawDiamondCheckMark(SmoothCanvas *Canvas,
				SmoothRectangle Target,
				SmoothColor Color)
{
	SmoothInt x, y, width, height;
	SmoothPoint points[5];
		
	SmoothRectangleGetValues(&Target, &x, &y, &width, &height);

          
	x += 1;
	y += 1; 
	width -= 2;
        height -= 2; 
	  
	SmoothPointSetValues(&points[0], x + width/2, y);
	SmoothPointSetValues(&points[1], x + width, y + height/2);
	SmoothPointSetValues(&points[2], x + width/2, y + height);
	SmoothPointSetValues(&points[3], x, y + height/2);
	SmoothPointSetValues(&points[4], x + width/2, y);
	  
	SmoothCanvasSetBrushColor(Canvas, Color);

	SmoothCanvasFillPolygon(Canvas, points, 5);
}

void
SmoothDrawCheckMark(SmoothCanvas Canvas,
			SmoothCheckMark CheckMark,
			SmoothInt X,
			SmoothInt Y,
			SmoothInt Width,
			SmoothInt Height,
			SmoothColor Background,
			SmoothColor Foreground)
{
	SmoothRectangle ta;

	SmoothRectangleSetValues(&ta, X, Y, Width, Height);

	SmoothCanvasCacheColor(Canvas, &Background);		
	SmoothCanvasCacheColor(Canvas, &Foreground);

	switch (CheckMark.Style) 
	{
		case SMOOTH_CHECKMARK_STYLE_CLEAN : /* classic redmond style */

			SmoothDrawCleanCheckMark(Canvas, ta, Foreground);
		break;

		case SMOOTH_CHECKMARK_STYLE_FAST : /* most common style */
			SmoothDrawFastCheckMark(Canvas, ta, Foreground);
		break;	

		case SMOOTH_CHECKMARK_STYLE_CROSS : /* common(default?) style in kde themes */
			SmoothDrawCrossCheckMark(Canvas, ta, Foreground);
		break;	

		case SMOOTH_CHECKMARK_STYLE_PLUS : /* a + mark */
			SmoothDrawCrossCheckMark(Canvas, ta, Foreground);
		break;	

		case SMOOTH_CHECKMARK_STYLE_MINUS : /* a - mark */
			SmoothDrawMinusCheckMark(Canvas, ta, Foreground);
		break;	

		case SMOOTH_CHECKMARK_STYLE_BLOCK : /* square check style, like mist */
			SmoothDrawBlockCheckMark(Canvas, ta, Foreground);
		break;	

		case SMOOTH_CHECKMARK_STYLE_CIRCLE : /* round check style, like mist, but with circles */
			SmoothDrawCircleCheckMark(Canvas, ta, Foreground);
		break;	

		case SMOOTH_CHECKMARK_STYLE_DIAMOND : /* diamond check style, like mist, but with diamonds - needs to follow motif option and do 3d edge to allow being gtk1.2 like*/
			SmoothDrawDiamondCheckMark(Canvas, ta, Foreground);
		break;	

		case SMOOTH_CHECKMARK_STYLE_SLOPPY :
		break;	

		case SMOOTH_CHECKMARK_STYLE_PIXBUF : /* custom check in xpm format */
		break;	

		case SMOOTH_CHECKMARK_STYLE_NONE :
		default :
			return;
	}   

	SmoothCanvasUnCacheColor(Canvas, &Background);		
	SmoothCanvasUnCacheColor(Canvas, &Foreground);
}
