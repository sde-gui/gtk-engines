/******************************************************************************/
/* abstract_type_defines.h - Abstract type defines for Drawing Interface      */
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
#ifndef ABSTRACT_TYPE_DEFINES_H
#define ABSTRACT_TYPE_DEFINES_H

/* Declare Basic Types For Abstract Drawing Functions */
#ifndef SMOOTHSTRINGS
typedef char		SmoothChar;
typedef unsigned char	SmoothUChar;
typedef char*	 	SmoothString;
#endif

#ifndef SMOOTHINTS
typedef int		SmoothInt;
typedef float		SmoothFloat;
typedef double		SmoothDouble;
#endif

#ifndef SMOOTHBOOL
typedef int		SmoothBool;

#define SmoothFalse	0
#define SmoothTrue	1
#endif

/* Declare Point/Rectangle Structs */
#ifndef SMOOTHPOINT
typedef struct
{
	SmoothInt	X;
	SmoothInt	Y;
} SmoothPoint;
#endif

#ifndef SMOOTHRECTANGLE
typedef struct
{
	SmoothInt	X;
	SmoothInt	Y;

	SmoothInt	Width;
	SmoothInt	Height;
} SmoothRectangle;
#endif

/* Declare Shape/Image Structs */

#ifndef SMOOTHIMAGES
typedef char* SmoothImageBuffer;
typedef void* SmoothImage;
#endif

/* Declare/Define Color/Pen/Brush Structs/Properties */
#ifndef SMOOTHCOLOR
typedef struct
{
	SmoothDouble	 Red;
	SmoothDouble	 Green;
	SmoothDouble	 Blue;
	SmoothDouble	 Alpha;
} SmoothColor;
#endif

#ifndef SMOOTHBRUSHTYPE
typedef enum 
{
	SMOOTH_BRUSH_SOLID, 
	SMOOTH_BRUSH_PATTERN, 
	SMOOTH_BRUSH_OPAQUE_PATTERN, 
	SMOOTH_BRUSH_IMAGE
} SmoothBrushType;
#endif

#ifndef SMOOTHLINEPROPERTIES
typedef enum 
{
	SMOOTH_LINE_SOLID, 
	SMOOTH_LINE_ON_OFF_DASH, 
	SMOOTH_LINE_DOUBLE_DASH
} SmoothLineStyle;

typedef enum 
{
	SMOOTH_CAP_NOT_LAST, 
	SMOOTH_CAP_BUTT, 
	SMOOTH_CAP_ROUND,
	SMOOTH_CAP_PROJECTING
} SmoothLineCap;

typedef enum 
{
	SMOOTH_JOIN_MITER, 
	SMOOTH_JOIN_ROUND, 
	SMOOTH_JOIN_BEVEL
} SmoothLineJoin;
#endif

#ifndef SMOOTHLINEPATTERN
typedef struct
{
	SmoothChar	*Pattern;
	SmoothInt	Length;
	SmoothInt	Offset;
} SmoothLinePattern;
#endif

#ifndef SMOOTHGRADIENT
typedef enum {
  SMOOTH_GRADIENT_HORIZONTAL,
  SMOOTH_GRADIENT_VERTICAL,
  SMOOTH_GRADIENT_NORTHERN_DIAGONAL,
  SMOOTH_GRADIENT_SOUTHERN_DIAGONAL
} SmoothGradientType;

#define GRADIENT(horizontal) (horizontal ? SMOOTH_GRADIENT_HORIZONTAL : SMOOTH_GRADIENT_VERTICAL)
#define DIAGONALGRADIENT(northern) (northern ? SMOOTH_GRADIENT_NORTHERN_DIAGONAL : SMOOTH_GRADIENT_SOUTHERN_DIAGONAL)
#endif

#ifndef SMOOTHCANVAS
typedef void* SmoothCanvas;
#endif

#endif /* ABSTRACT_TYPE_DEFINES_H */
