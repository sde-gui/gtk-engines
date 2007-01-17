/******************************************************************************/
/* draw_arrows.h - Engine Agnostic Arrow Part Defines                         */
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
#ifndef DRAW_ARROWS_H
#define DRAW_ARROWS_H

#define  SMOOTH_ARROW_STYLE_COUNT SMOOTH_ARROW_STYLE_LAST - SMOOTH_ARROW_STYLE_FIRST + 1
#define  SMOOTH_ARROW_TYPE_COUNT SMOOTH_ARROW_TYPE_LAST - SMOOTH_ARROW_TYPE_FIRST + 1

typedef enum {
	SMOOTH_ARROW_STYLE_CLEAN = 1, 
	SMOOTH_ARROW_STYLE_DIRTY, 
	SMOOTH_ARROW_STYLE_SLICK, 
	SMOOTH_ARROW_STYLE_XPM, 
	SMOOTH_ARROW_STYLE_NONE,

	/* DEPRECATED STYLES */
	SMOOTH_ARROW_STYLE_DEPRECATED_CLEANICE,
	SMOOTH_ARROW_STYLE_DEPRECATED_XFCE,
	SMOOTH_ARROW_STYLE_DEPRECATED_THINICE,

	SMOOTH_ARROW_STYLE_DEPRECATED_ICEGRADIENT = SMOOTH_ARROW_STYLE_DIRTY,
	SMOOTH_ARROW_STYLE_DEPRECATED_WONDERLAND = SMOOTH_ARROW_STYLE_SLICK,

	/* First & Last */
	SMOOTH_ARROW_STYLE_FIRST = SMOOTH_ARROW_STYLE_CLEAN,
	SMOOTH_ARROW_STYLE_LAST = SMOOTH_ARROW_STYLE_NONE,

	/* Set By Default */
	SMOOTH_ARROW_STYLE_DEFAULT = SMOOTH_ARROW_STYLE_CLEAN
} SmoothArrowStyle;

typedef enum {
	SMOOTH_ARROW_TYPE_UP, 
	SMOOTH_ARROW_TYPE_DOWN, 
	SMOOTH_ARROW_TYPE_LEFT, 
	SMOOTH_ARROW_TYPE_RIGHT, 
	SMOOTH_ARROW_TYPE_NONE,

	/* First & Last */
	SMOOTH_ARROW_TYPE_FIRST = SMOOTH_ARROW_TYPE_UP,
	SMOOTH_ARROW_TYPE_LAST = SMOOTH_ARROW_TYPE_NONE
} SmoothArrowType;

typedef struct {
	SmoothArrowStyle Style;/*must be first till theme_part macro crap is killed*/
	SmoothArrowType Type;

	SmoothBool Etched;
	SmoothBool Solid;
	SmoothInt Tail;

	SmoothInt XPadding;
  	SmoothInt YPadding;

	SmoothBool	HasStyle;
	SmoothBool	HasType;
	SmoothBool	HasEtched;
	SmoothBool	HasSolid;
	SmoothBool	HasTail;

	SmoothBool	HasXPadding;
	SmoothBool	HasYPadding;
} SmoothArrow;

typedef struct SmoothArrowPartProto SmoothArrowPart;
struct SmoothArrowPartProto
{
	SmoothArrowPart *Inherited;
		
	SmoothArrow *DefaultStyle;
	SmoothArrow *DefaultStateStyles;/*[5];*/
	SmoothArrow *DefaultTypeStyles;/*[SMOOTH_ARROW_TYPE_COUNT]*/
	SmoothArrow *Styles[5];/*[5][SMOOTH_ARROW_TYPE_COUNT]*/

	SmoothBool StylesFreed;

	/* the first time the get part is called its set here so 
	   we don't waste time compositing it again */
	SmoothArrow CompositeArrows[5][SMOOTH_ARROW_TYPE_COUNT];
	SmoothBool CompositeArrowsSet[5][SMOOTH_ARROW_TYPE_COUNT];
};

GE_INTERNAL void
SmoothDrawArrow(SmoothCanvas *Canvas,
			SmoothArrow Arrow,
			SmoothInt X,
			SmoothInt Y,
			SmoothInt Width,
			SmoothInt Height,
			SmoothColor Background,
			SmoothColor Foreground);

GE_INTERNAL void 
SmoothFreeArrowStyles(SmoothArrowPart *arrow);
GE_INTERNAL void
SmoothCopyArrowPart(SmoothArrowPart *dst, SmoothArrowPart *src);

#endif /* DRAW_ARROWS_H */
