/******************************************************************************/
/* draw_fill.c - Engine Agnostic Fill Part Drawing                            */
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
#include "smooth_parts.h"

void	
SmoothDrawFill(SmoothFill *Fill,
			SmoothCanvas *Canvas,
			SmoothInt X,
			SmoothInt Y,
			SmoothInt Width,
			SmoothInt Height)
{
	if (Fill->Style == SMOOTH_FILL_STYLE_TILE) 
	{
		if (!Fill->Tile.ImageFile) 
			Fill->Style = SMOOTH_FILL_STYLE_SOLID;
	}
	
	switch (Fill->Style) {
		case SMOOTH_FILL_STYLE_TILE : 
			SmoothCanvasRenderTile(Canvas, Fill->Tile, X, Y, Width, Height);
		break;
		
		case SMOOTH_FILL_STYLE_GRADIENT : 
		{		
			SmoothCanvasCacheColor(Canvas, &Fill->Gradient.From);
			SmoothCanvasCacheColor(Canvas, &Fill->Gradient.To);

			SmoothCanvasRenderGradient(Canvas, Fill->Gradient, X, Y, Width - 1, Height - 1);
			
			SmoothCanvasUnCacheColor(Canvas, &Fill->Gradient.From);
			SmoothCanvasUnCacheColor(Canvas, &Fill->Gradient.To);
		}
		break;
		
		case SMOOTH_FILL_STYLE_SHADE_GRADIENT : 
		{		
			SmoothCanvasCacheColor(Canvas, &Fill->BaseColor);
			SmoothCanvasCacheShadedColor(Canvas, Fill->BaseColor, Fill->ColorShade.From, &Fill->Gradient.From);
			SmoothCanvasCacheShadedColor(Canvas, Fill->BaseColor, Fill->ColorShade.To, &Fill->Gradient.To);

			SmoothCanvasRenderGradient(Canvas, Fill->Gradient, X, Y, Width - 1, Height - 1);
			
			SmoothCanvasUnCacheShadedColor(Canvas, Fill->BaseColor, Fill->ColorShade.From, &Fill->Gradient.From);
			SmoothCanvasUnCacheShadedColor(Canvas, Fill->BaseColor, Fill->ColorShade.To, &Fill->Gradient.To);
			SmoothCanvasUnCacheColor(Canvas, &Fill->BaseColor);
		}
		break;

		case SMOOTH_FILL_STYLE_SOLID : 
		default :
		{
			SmoothColor color;
			color = Fill->BaseColor;
			
			SmoothCanvasCacheColor(Canvas, &color);
			SmoothCanvasSetBrushColor(Canvas, color);

			if (Fill->Roundness >= 1.0) {
				SmoothCanvasFillChord(Canvas, X, Y, Width, Height, 0, 360 * 64);
			} 
			else 
			{
				SmoothCanvasFillRectangle(Canvas, X, Y, Width, Height); 
			}

			SmoothCanvasUnCacheColor(Canvas, &color);
		}	
	}
}
