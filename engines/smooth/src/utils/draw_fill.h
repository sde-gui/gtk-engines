/******************************************************************************/
/* draw_fill.h - Engine Agnostic Fill Part Defines                            */
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
#ifndef DRAW_FILL_H
#define DRAW_FILL_H

typedef enum {
  SMOOTH_FILL_STYLE_SOLID,
  SMOOTH_FILL_STYLE_TILE,
  SMOOTH_FILL_STYLE_GRADIENT,
  SMOOTH_FILL_STYLE_SHADE_GRADIENT,

  SMOOTH_FILL_STYLE_DEFAULT = SMOOTH_FILL_STYLE_SOLID
} SmoothFillStyle;

typedef struct {
	SmoothFillStyle Style;
	
	SmoothTile Tile;
	SmoothGradient Gradient;
	
	SmoothDouble Roundness;
	
	SmoothColor BaseColor;

	struct {
		SmoothDouble From;
		SmoothDouble To;
	} ColorShade;
} SmoothFill;

#define GRADIENT(horizontal) (horizontal ? SMOOTH_GRADIENT_HORIZONTAL : SMOOTH_GRADIENT_VERTICAL)
#define DIAGONALGRADIENT(northern) (northern ? SMOOTH_GRADIENT_NORTHERN_DIAGONAL : SMOOTH_GRADIENT_SOUTHERN_DIAGONAL)

void	SmoothDrawFill(SmoothFill *Fill,
			SmoothCanvas *Canvas,
			SmoothInt X,
			SmoothInt Y,
			SmoothInt Width,
			SmoothInt Height);
		      
#endif /* DRAW_FILL_H */
