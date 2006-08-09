/******************************************************************************/
/* draw_checkmark.h - Engine Agnostic Checkmark Part Defines                  */
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
#ifndef DRAW_CHECKMARK_H
#define DRAW_CHECKMARK_H

typedef enum {
	SMOOTH_CHECKMARK_STYLE_CLEAN, 
	SMOOTH_CHECKMARK_STYLE_FAST, 

	SMOOTH_CHECKMARK_STYLE_CROSS, 
	SMOOTH_CHECKMARK_STYLE_PLUS, 
	SMOOTH_CHECKMARK_STYLE_MINUS, 

	SMOOTH_CHECKMARK_STYLE_BLOCK, 
	SMOOTH_CHECKMARK_STYLE_CIRCLE, 
	SMOOTH_CHECKMARK_STYLE_DIAMOND, 

	SMOOTH_CHECKMARK_STYLE_SLOPPY, 
	SMOOTH_CHECKMARK_STYLE_PIXBUF, 

	SMOOTH_CHECKMARK_STYLE_NONE,
	
	SMOOTH_CHECKMARK_STYLE_DEFAULT = SMOOTH_CHECKMARK_STYLE_CLEAN,
	SMOOTH_CHECKMARK_STYLE_DEFAULT_OPTION = SMOOTH_CHECKMARK_STYLE_CIRCLE
} SmoothCheckMarkStyle;

typedef struct {
	SmoothCheckMarkStyle Style;

	SmoothBool Shadow;
} SmoothCheckMark;

GE_INTERNAL void
SmoothDrawCheckMark(SmoothCanvas Canvas,
			SmoothCheckMark CheckMark,
			SmoothInt X,
			SmoothInt Y,
			SmoothInt Width,
			SmoothInt Height,
			SmoothColor Background,
			SmoothColor Foreground);


#endif /* DRAW_CHECKMARK_H */
