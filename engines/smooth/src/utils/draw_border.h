/******************************************************************************/
/* draw_border.h - Engine Agnostic Border Part Defines                        */
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
#ifndef DRAW_BORDER_H
#define DRAW_BORDER_H

typedef enum {
	SMOOTH_POSITION_LEFT, 
	SMOOTH_POSITION_RIGHT, 
	SMOOTH_POSITION_TOP, 
	SMOOTH_POSITION_BOTTOM
} SmoothPositionSide;

typedef enum {
	SMOOTH_BEVEL_STYLE_SMOOTHED = 1,
	SMOOTH_BEVEL_STYLE_SMOOTHEDBEVEL,	
	SMOOTH_BEVEL_STYLE_BEVELED,	

	SMOOTH_BEVEL_STYLE_THIN,
	SMOOTH_BEVEL_STYLE_SOFT,

	SMOOTH_BEVEL_STYLE_STANDARD,
	SMOOTH_BEVEL_STYLE_REDMOND,

	SMOOTH_BEVEL_STYLE_COLD,
	SMOOTH_BEVEL_STYLE_ICED,

	SMOOTH_BEVEL_STYLE_FLAT,

	SMOOTH_BEVEL_STYLE_XPM,

	SMOOTH_BEVEL_STYLE_NONE,

	/* Deprecated */
	SMOOTH_BEVEL_STYLE_DEPRECATED_WONDERLAND,

	/* Default */
	SMOOTH_BEVEL_STYLE_DEFAULT = SMOOTH_BEVEL_STYLE_THIN
} SmoothBevelStyle;

typedef enum {
	SMOOTH_BORDER_TYPE_IN, 
	SMOOTH_BORDER_TYPE_OUT, 
	SMOOTH_BORDER_TYPE_ETCHED, 
	SMOOTH_BORDER_TYPE_ENGRAVED, 
	SMOOTH_BORDER_TYPE_NONE
} SmoothBorderType;

typedef struct
{
	SmoothBevelStyle Style;
	SmoothBorderType Type;
		
	SmoothFloat CornerRadius; /* Percentages of 1/2 cooresponding (square's) diagonal */
	SmoothFloat Thickness; /* Number of Pixels */
} SmoothBorder;

typedef struct
{
	SmoothBevelStyle Style;
	SmoothFloat Thickness;
} SmoothLineBevel;

void
SmoothDrawShadowWithGap(SmoothCanvas *Canvas,
			SmoothColor TopLeftColor,
			SmoothColor BottomRightColor,
			SmoothRectangle Target,
			SmoothPositionSide GapSide, 
			SmoothInt GapPos,
			SmoothInt GapSize,
			SmoothBool TopLeftOverlap);

void
SmoothDrawBevelWithGap(SmoothCanvas *Canvas,
			SmoothColor TopLeftColor,
			SmoothColor BottomRightColor,
			SmoothRectangle Target,
			SmoothInt BevelThickness,
			SmoothPositionSide GapSide, 
			SmoothInt GapPos,
			SmoothInt GapSize);

void
SmoothDrawCirclularShadow(SmoothCanvas *Canvas,
				SmoothColor TopLeftColor,
				SmoothColor BottomRightColor,
				SmoothRectangle Target);
	       
void
SmoothDrawCirclularBevel(SmoothCanvas *Canvas,
				SmoothColor TopLeftColor,
				SmoothColor BottomRightColor,
				SmoothRectangle Target,
				SmoothInt BevelThickness);


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
				SmoothInt GapSize);

SmoothBool
SmoothDrawPolygonBorder(SmoothBorder *Border,
				SmoothCanvas *Canvas,
				SmoothColor BaseColor,
				SmoothPoint *Points,
				SmoothInt Count);

void
SmoothDrawLineBevel(SmoothLineBevel LineBevel,
			SmoothCanvas Canvas,
			SmoothColor BaseColor,

			SmoothInt Start,
			SmoothInt End,
			SmoothInt Base,
			SmoothBool Horizontal);				

#endif /* DRAW_BORDER_H */
