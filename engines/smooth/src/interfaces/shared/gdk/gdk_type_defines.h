/******************************************************************************/
/* gdk_type_defines.h - Shared Type Defines for all GDK versions              */
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
#ifndef GDK_TYPE_DEFINES_H
#define GDK_TYPE_DEFINES_H

#include <gdk/gdk.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif 

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif 

#ifndef M_PI_4 
#define M_PI_4 0.78539816339744830962
#endif 

/* Declare Basic Types For Abstract Drawing Functions */
typedef gchar		SmoothChar;
typedef guchar		SmoothUChar;
typedef GString* 	SmoothString;

typedef gint		SmoothInt;
typedef gfloat		SmoothFloat;
typedef gdouble		SmoothDouble;

typedef gboolean	SmoothBool;

#define SmoothFalse	FALSE
#define SmoothTrue	TRUE

/* Declare Point/Rectangle Structs */
typedef GdkPoint SmoothPoint;

typedef GdkRectangle SmoothRectangle;

typedef GdkRegion SmoothRegion;

/* Declare Shape/Image Structs */
typedef GdkBitmap SmoothBitmap;

typedef struct
{
	GdkPixmap	*Pixmap;
	GdkGC		*GC;
} SmoothGDKImage;

/* Declare/Define Color/Pen/Brush Structs/Properties */
typedef struct
{
	GdkColor RGB;

	gdouble	 Alpha;
	SmoothInt CacheIndex;
} SmoothSmoothGDKColor;

typedef enum 
{
	SMOOTH_BRUSH_SOLID = GDK_SOLID, 
	SMOOTH_BRUSH_IMAGE = GDK_TILED
} SmoothGDKBrushType;
  
typedef enum 
{
	SMOOTH_LINE_SOLID = GDK_LINE_SOLID, 
	SMOOTH_LINE_ON_OFF_DASH = GDK_LINE_ON_OFF_DASH, 
	SMOOTH_LINE_DOUBLE_DASH = GDK_LINE_DOUBLE_DASH
} SmoothGDKLineStyle;

typedef enum 
{
	SMOOTH_CAP_NOT_LAST = GDK_CAP_NOT_LAST, 
	SMOOTH_CAP_BUTT = GDK_CAP_BUTT, 
	SMOOTH_CAP_ROUND = GDK_CAP_ROUND,
	SMOOTH_CAP_PROJECTING = GDK_CAP_PROJECTING
} SmoothGDKLineCap;

typedef enum 
{
	SMOOTH_JOIN_MITER = GDK_JOIN_MITER, 
	SMOOTH_JOIN_ROUND = GDK_JOIN_ROUND, 
	SMOOTH_JOIN_BEVEL = GDK_JOIN_BEVEL
} SmoothGDKLineJoin;

#endif /* GDK_TYPE_DEFINES_H */
