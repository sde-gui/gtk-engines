/******************************************************************************/
/* gdk2_type_defines.h - GDK2 specific type definitions                       */
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
#ifndef GDK2_TYPE_DEFINES_H
#define GDK2_TYPE_DEFINES_H

#include <interfaces/shared/gdk/gdk_type_defines.h>

#define SMOOTHSTRINGS	1
#define SMOOTHINTS	1
#define SMOOTHBOOL	1
#define SMOOTHPOINT	1
#define SMOOTHRECTANGLE	1

/* Declare Shape/Image Structs */
#define SMOOTHIMAGES	1
typedef GdkPixbuf* SmoothImageBuffer;
typedef SmoothGDKImage SmoothImage;

#define SMOOTHCOLOR	1
typedef SmoothGDKColor SmoothColor;

#define SMOOTHBRUSHTYPE	1
typedef SmoothGDKBrushType SmoothBrushType;
  
#define SMOOTHLINEPROPERTIES	1
typedef SmoothGDKLineStyle SmoothLineStyle;
typedef SmoothGDKLineCap SmoothLineCap;
typedef SmoothGDKLineJoin SmoothLineJoin;

#define SMOOTHGRADIENT	1

typedef enum {
	SMOOTH_GRADIENT_HORIZONTAL,
	SMOOTH_GRADIENT_VERTICAL,
	SMOOTH_GRADIENT_NORTHERN_DIAGONAL,
	SMOOTH_GRADIENT_SOUTHERN_DIAGONAL
} SmoothGradientType;

#define GRADIENT(horizontal) (horizontal ? SMOOTH_GRADIENT_HORIZONTAL : SMOOTH_GRADIENT_VERTICAL)
#define DIAGONALGRADIENT(northern) (northern ? SMOOTH_GRADIENT_NORTHERN_DIAGONAL : SMOOTH_GRADIENT_SOUTHERN_DIAGONAL)

#endif /* GDK2_TYPE_DEFINES_H */
