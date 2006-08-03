/******************************************************************************/
/* gdk_drawing_interface.c - Shared drawing interface for all GDK versions    */
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
/* Portions Based on GTK+                                                     */
/*   Peter Mattis <petm@xcf.berkeley.edu>                                     */
/*   Spencer Kimball <spencer@xcf.berkeley.edu>                               */
/*   Josh MacDonald <jmacd@xcf.berkeley.edu>                                  */
/******************************************************************************/
#include ENGINE_HEADER
#include ENGINE_RC_HEADER

#include <interfaces/drawing_interface.h>
#include INTERFACE_DEFINES
#include "gdk_private_drawing_interface.h"

/* _Color Value Getions */
static SmoothBool
SmoothGDKColorGetRedValue(SmoothColor *_Color, 
				SmoothDouble *RedValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Color && RedValue)
	{
		*RedValue = (SmoothDouble)_Color->RGB.red / 65535.0;
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool
SmoothGDKColorGetGreenValue(SmoothColor *_Color, 
				SmoothDouble *GreenValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Color && GreenValue)
	{
		*GreenValue = (SmoothDouble)_Color->RGB.green / 65535.0;
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool
SmoothGDKColorGetBlueValue(SmoothColor *_Color, 
				SmoothDouble *BlueValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Color && BlueValue)
	{
		*BlueValue = (SmoothDouble)_Color->RGB.blue / 65535.0;
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool
SmoothGDKColorGetAlphaValue(SmoothColor *_Color, 
				SmoothDouble *AlphaValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Color && AlphaValue)
	{
		*AlphaValue = (SmoothDouble)_Color->Alpha /  65535.0;
		result = SmoothTrue;
	}	
	return result;
}

/* _Color Value Modifications */
static SmoothBool
SmoothGDKColorSetRedValue(SmoothColor *_Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if ((NewValue <= 1.0) && (NewValue >= 0.0))
	{
		(*_Color).RGB.red = NewValue * 65535.0;
		(*_Color).CacheIndex = -1;/* .Cached = SmoothFalse; */
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool
SmoothGDKColorSetGreenValue(SmoothColor *_Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	if ((NewValue <= 1.0) && (NewValue >= 0.0))
	{
		(*_Color).RGB.green = NewValue * 65535.0;
		(*_Color).CacheIndex = -1;/* .Cached = SmoothFalse; */
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool
SmoothGDKColorSetBlueValue(SmoothColor *_Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	if ((NewValue <= 1.0) && (NewValue >= 0.0))
	{
		(*_Color).RGB.blue = NewValue * 65535.0;
		(*_Color).CacheIndex = -1;/* .Cached = SmoothFalse; */
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool
SmoothGDKColorSetAlphaValue(SmoothColor *_Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	if ((NewValue <= 1.0) && (NewValue >= 0.0))
	{
		(*_Color).Alpha = NewValue * 65535.0;
		(*_Color).CacheIndex = -1;/* Cached = SmoothFalse; */
		result = SmoothTrue;
	}	
	return result;
}

/* _Rectangle Status Functions */
static SmoothBool 
SmoothGDKRectangleCreate(SmoothRectangle *_Rectangle, 
			SmoothInt X, 
			SmoothInt Y, 
			SmoothInt Width, 
			SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (_Rectangle)
	{
		_Rectangle->x = X;
		_Rectangle->y = Y;
		_Rectangle->width = Width;
		_Rectangle->height = Height;

		result = SmoothTrue;
	}	
	
	return result;
}

static SmoothBool 
SmoothGDKRectangleDestroy(SmoothRectangle *_Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (_Rectangle)
	{
		result = SmoothTrue;
	}
	
	return result;
}

static SmoothBool 
SmoothGDKRectangleIsValid(SmoothRectangle *_Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (_Rectangle)
	{
		result = SmoothTrue;
	}
	
	return result;
}

/* _Rectangle Value Getions */
static SmoothBool
SmoothGDKRectangleGetXValue(SmoothRectangle *_Rectangle, SmoothInt *_XValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Rectangle && _XValue)
	{
		*_XValue = _Rectangle->x;
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
SmoothGDKRectangleGetYValue(SmoothRectangle *_Rectangle, SmoothInt *_YValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Rectangle && _YValue)
	{
		*_YValue = _Rectangle->y;
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
SmoothGDKRectangleGetWidthValue(SmoothRectangle *_Rectangle, SmoothInt *_WidthValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Rectangle && _WidthValue)
	{
		*_WidthValue = _Rectangle->width;
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
SmoothGDKRectangleGetHeightValue(SmoothRectangle *_Rectangle, SmoothInt *_HeightValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Rectangle && _HeightValue)
	{
		*_HeightValue = _Rectangle->height;
		result = SmoothTrue;
	}
	return result;
}

/* _Rectangle Value Modifications */
static SmoothBool
SmoothGDKRectangleSetXValue(SmoothRectangle *_Rectangle, 
				SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Rectangle)
	{
		_Rectangle->x = NewValue;
		result = SmoothTrue;
	}
	
	return result;
}

static SmoothBool
SmoothGDKRectangleSetYValue(SmoothRectangle *_Rectangle, 
				SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Rectangle)
	{
		_Rectangle->y = NewValue;
		result = SmoothTrue;
	}
	
	return result;
}

static SmoothBool
SmoothGDKRectangleSetWidthValue(SmoothRectangle *_Rectangle, 
				SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Rectangle)
	{
		_Rectangle->width = NewValue;
		result = SmoothTrue;
	}
	
	return result;
}

static SmoothBool
SmoothGDKRectangleSetHeightValue(SmoothRectangle *_Rectangle, 
				SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Rectangle)
	{
		_Rectangle->height = NewValue;
		result = SmoothTrue;
	}
	
	return result;
}

/* _Rectangle Misc Functions */
static SmoothBool 
SmoothGDKRectangleFindIntersection(SmoothRectangle _Rectangle1, 
					SmoothRectangle _Rectangle2, 
					SmoothRectangle *IntersectingRectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (IntersectingRectangle)
	{
		result = gdk_rectangle_intersect(&_Rectangle1, &_Rectangle2, IntersectingRectangle);
	}
	
	return result;
}

/* _Point Value Getions */
static SmoothBool
SmoothGDKPointGetXValue(SmoothPoint *_Point,
			SmoothInt *_XValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Point && _XValue)
	{
		*_XValue = _Point[0].x;
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
SmoothGDKPointGetYValue(SmoothPoint *_Point, 
			SmoothInt *_YValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Point && _YValue)
	{
		*_YValue = _Point[0].y;
		result = SmoothTrue;
	}
	return result;
}


/* _Point Value Modifications */
static SmoothBool
SmoothGDKPointSetXValue(SmoothPoint *_Point,
			SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Point)
	{
		_Point[0].x = NewValue;
		result = SmoothTrue;
	}
	
	return result;
}

static SmoothBool
SmoothGDKPointSetYValue(SmoothPoint *_Point,
			SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (_Point)
	{
		_Point[0].y = NewValue;
		result = SmoothTrue;
	}
	
	return result;
}

/* Canvas Map/Free A _Color From The Colormap*/
static SmoothBool
SmoothGDKCanvasAllocateColor(SmoothGDKPrivateCanvas *Canvas, 
					SmoothColor *_Color)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && _Color)
	{	
		gdk_colormap_alloc_color(Canvas->Colormap, &_Color->RGB, SmoothFalse, SmoothTrue);
		result = SmoothTrue;
	}
	
	return result;
}

static SmoothBool 
SmoothGDKCanvasDeallocateColor(SmoothGDKPrivateCanvas *Canvas, 
					SmoothColor *_Color)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && _Color)
	{	
		gdk_colormap_free_colors(Canvas->Colormap, &_Color->RGB, 1);
		result = SmoothTrue;
	}
	
	return result;
}

/* Canvas cache/uncache a _Color */
static SmoothInt
cached_color_hash(SmoothColor *_Color, SmoothDouble Shade)
{
	SmoothInt hash = 0;

	hash = (SmoothInt)(Shade*255/3);
	hash = ((SmoothInt)(_Color->RGB.red) >> 8) + (hash << 6) + (hash << 16) - hash; 
	hash = ((SmoothInt)(_Color->RGB.green) >> 8) + (hash << 6) + (hash << 16) - hash; 
	hash = ((SmoothInt)(_Color->RGB.blue) >> 8) + (hash << 6) + (hash << 16) - hash; 
	hash = ((SmoothInt)(_Color->Alpha) >> 8) + (hash << 6) + (hash << 16) - hash; 

	return (hash & 0x7FFFFFFF);
}

static GHashTable* smooth_color_cache = NULL;

typedef struct
{
	SmoothColor 	_Color;
	GdkColormap	*Colormap;

	SmoothInt		ref_count;
} GdkCachedColor;

static void
smooth_free_color_cache(GdkCachedColor *Cache)
{
	gdk_colormap_free_colors(Cache->Colormap, &Cache->_Color.RGB, 1);
	g_free(Cache);
}

static GdkCachedColor *
new_color_cache(GdkColormap *Colormap, SmoothColor _Color, SmoothDouble Shade)
{
	GdkCachedColor *result=NULL;

	result = g_new0(GdkCachedColor, 1);
	result->ref_count = 1;
	
	result->Colormap = Colormap;
	if (!(result->Colormap)) 
	{
		result->Colormap = gdk_colormap_get_system();
	}
	
	result->_Color = _Color;

	if (Shade != 1.0)
	{
		SmoothCompositeColorShade(&result->_Color, Shade, &result->_Color);	
	}
	
	gdk_colormap_alloc_color(result->Colormap, &result->_Color.RGB, SmoothFalse, SmoothTrue);

	return result;
}

static SmoothBool 
smooth_force_smooth_cleanup_color_cache(gpointer key, gpointer value, gpointer user_data)
{
	if (value) 
	{
		smooth_free_color_cache((GdkCachedColor *)value);
	}	
	return SmoothTrue;
}

static void
smooth_cleanup_color_cache(SmoothBool Force)
{
	if (smooth_color_cache) 
	{
		if (Force)
		{
			g_hash_table_foreach_remove(smooth_color_cache, smooth_force_smooth_cleanup_color_cache, NULL);
		}
		
		if (g_hash_table_size(smooth_color_cache)<=0) 
		{
			g_hash_table_destroy(smooth_color_cache);
			smooth_color_cache = NULL;
		}  
	}   
}

static SmoothColor *
smooth_internal_color_get_color(GdkColormap *Colormap, SmoothColor *_Color, SmoothDouble Shade,  SmoothInt index)
{
	GdkCachedColor *cache=NULL;
	SmoothColor *result=NULL;
	
	if ((index < 0) && (_Color))	
	{
		index = cached_color_hash(_Color, Shade);
	}	

	if (!(smooth_color_cache))
	{
		smooth_color_cache = g_hash_table_new(g_int_hash, g_int_equal);
	}
	
	cache = g_hash_table_lookup(smooth_color_cache, &index);
   
	if (cache) 
	{
		result = &cache->_Color;
		cache->ref_count++;
	}
	else
	{
		cache = new_color_cache(Colormap, *_Color, Shade);
	
		cache->_Color.CacheIndex = index;

		if (cache) 
			g_hash_table_insert(smooth_color_cache, &cache->_Color.CacheIndex, cache);
			
		result = &cache->_Color;
	}
	
	return result;
}

static void
smooth_internal_color_unref(SmoothInt index)
{  
	GdkCachedColor *cache = NULL;

	if (smooth_color_cache) 
	{
		cache = g_hash_table_lookup(smooth_color_cache, &index);

		if (cache) 
		{
			cache->ref_count--;
   
			if (cache->ref_count == 0) 
			{
				g_hash_table_remove(smooth_color_cache, &index);
				smooth_free_color_cache(cache);
			}
		}
		smooth_cleanup_color_cache(SmoothFalse);  
	}   
}

GdkGC *
smooth_internal_drawing_area_use_pen_gc(SmoothGDKPrivateCanvas *Canvas, SmoothBool RequireValidColor)
{
	GdkGC *result = NULL;

	if (Canvas)
	{
		GdkGCValues gc_values;
		SmoothInt value = Canvas->Pen.Color.CacheIndex, flags=0;


		if (!(Canvas->Colormap)) {
			Canvas->Colormap = gdk_colormap_get_system();
			Canvas->Depth = gdk_colormap_get_visual(Canvas->Colormap)->depth;
		}


		if ((Canvas->Pen.Color.Alpha > 0))
		{
			SmoothColor *fg = NULL;

			fg = smooth_internal_color_get_color(Canvas->Colormap, &Canvas->Pen.Color, 1.0, value);
			
			gc_values.foreground = fg->RGB;
			gc_values.background = fg->RGB;

			flags = GDK_GC_FOREGROUND | GDK_GC_BACKGROUND;

			if (Canvas->Pen.UseThickness)
			{
				gc_values.line_width = Canvas->Pen.Thickness;

				flags |=  GDK_GC_LINE_WIDTH;
			}
			
			if (Canvas->Pen.UseStyle)
			{
				gc_values.line_style = Canvas->Pen.Style;

				flags |=  GDK_GC_LINE_STYLE;
			}

			if (Canvas->Pen.UseJoin)
			{
				gc_values.join_style = Canvas->Pen.Join;

				flags |=  GDK_GC_JOIN_STYLE;
			}


			if (Canvas->Pen.UseCap)
			{
				gc_values.cap_style = Canvas->Pen.Cap;

				flags |=  GDK_GC_CAP_STYLE;
			}
		}
		
		if (!RequireValidColor || flags)
		{
			result = gtk_gc_get (Canvas->Depth, Canvas->Colormap, &gc_values, flags);

			if (result && Canvas->Pen.UsePattern)
			{
				 gdk_gc_set_dashes(result, Canvas->Pen.Pattern.Offset, Canvas->Pen.Pattern.Pattern, Canvas->Pen.Pattern.Length); 
			}
		}	
	}	
	
	return result;
	
}

void
smooth_internal_drawing_area_unuse_pen_gc(SmoothGDKPrivateCanvas *Canvas, GdkGC * GC)
{
	if (Canvas)
	{
		SmoothInt value = Canvas->Pen.Color.CacheIndex;

		if (GC)
		{
			gtk_gc_release(GC);
		}	
		
		if ((value >= 0) && (Canvas->Pen.Color.Alpha > 0))
		{
			smooth_internal_color_unref(value);
		}
	}	
}

GdkGC *
smooth_internal_drawing_area_use_brush_gc(SmoothGDKPrivateCanvas *Canvas, SmoothBool RequireValidColor)
{
	GdkGC *result = NULL;

	if (Canvas)
	{
		GdkGCValues gc_values;
		SmoothInt value = Canvas->Brush.Color.CacheIndex, flags=0;

		if (!(Canvas->Colormap)) {
			Canvas->Colormap = gdk_colormap_get_system();
			Canvas->Depth = gdk_colormap_get_visual(Canvas->Colormap)->depth;
		}

		if ((Canvas->Brush.Color.Alpha > 0))
		{
			SmoothColor *fg = NULL;

			fg = smooth_internal_color_get_color(Canvas->Colormap, &Canvas->Brush.Color, 1.0, value);
			
			gc_values.foreground = fg->RGB;
			gc_values.background = fg->RGB;
			
			flags = GDK_GC_FOREGROUND | GDK_GC_BACKGROUND;
		}
		
		if (!RequireValidColor || flags)
			result = gtk_gc_get (Canvas->Depth, Canvas->Colormap, &gc_values, flags);
	}	
	
	return result;
	
}

void
smooth_internal_drawing_area_unuse_brush_gc(SmoothGDKPrivateCanvas *Canvas, GdkGC * GC)
{
	if (Canvas)
	{
		SmoothInt value = Canvas->Brush.Color.CacheIndex;
		
		if (GC)
		{
			gtk_gc_release(GC);
		}
	
		if ((value >= 0) && (Canvas->Brush.Color.Alpha > 0))
		{
			smooth_internal_color_unref(value);
		}
	}	
}

/* Canvas cache/uncache a shaded _Color */
static SmoothBool
SmoothGDKCanvasCacheShadedColor(SmoothGDKPrivateCanvas *Canvas, 
					SmoothColor Base,
					SmoothDouble Shade,
					SmoothColor *_Color)
{					
	SmoothBool result = SmoothFalse;
	
	if (Canvas && _Color)
	{
		if (Base.Alpha > 0)
		{		
			SmoothColor *tmp = NULL;

			tmp = smooth_internal_color_get_color(Canvas->Colormap, &Base, Shade, -1);
		
			if (tmp)
			{
				*_Color = *tmp;
			}
		}
		else		
		{
			_Color->Alpha = 0;
		}
		result = SmoothTrue;
	}
	
	return result;
}

static SmoothBool
SmoothGDKCanvasUnCacheShadedColor(SmoothGDKPrivateCanvas *Canvas, 
					SmoothColor Base,
					SmoothDouble Shade,
					SmoothColor *_Color)
{
	SmoothBool result = SmoothFalse;
	
	if (_Color)
	{
		if (Base.Alpha > 0)
		{		
			smooth_internal_color_unref(_Color->CacheIndex);
			_Color->CacheIndex = -1;
		}	

		result = SmoothTrue;
	}
	
	return result;
}

static SmoothBool
SmoothGDKCanvasCacheColor(SmoothGDKPrivateCanvas *Canvas, 
				SmoothColor *_Color)
{
	SmoothColor tmp = *_Color;
	
	return SmoothGDKCanvasCacheShadedColor(Canvas, tmp, 1.0, _Color);
}

static SmoothBool
SmoothGDKCanvasUnCacheColor(SmoothGDKPrivateCanvas *Canvas, 
				SmoothColor *_Color)
{
	SmoothBool result = SmoothFalse;
	
	if (_Color)
	{
		if (_Color->Alpha > 0)
		{						
		
			smooth_internal_color_unref(_Color->CacheIndex);
			_Color->CacheIndex = -1;
		}
		result = SmoothTrue;
	}
	
	return result;
}									


/* Canvas cache/uncache a midpoint _Color */
static SmoothBool
SmoothGDKCanvasCacheMidPointColor(SmoothGDKPrivateCanvas *Canvas, 
					SmoothColor _Point1,
					SmoothColor _Point2,
					SmoothColor *_Color)
{					
	SmoothBool result = SmoothFalse;
	
	if (Canvas && _Color)
	{
		_Color->RGB.red = (_Point1.RGB.red + _Point2.RGB.red) / 2;
		_Color->RGB.green = (_Point1.RGB.green + _Point2.RGB.green) / 2;
		_Color->RGB.blue = (_Point1.RGB.blue + _Point2.RGB.blue) / 2;
		_Color->Alpha = (_Point1.Alpha + _Point2.Alpha) / 2;
		_Color->CacheIndex = -1;

		result = SmoothGDKCanvasCacheShadedColor(Canvas, *_Color, 1.0, _Color);
	}
	
	return result;
}

static SmoothBool
SmoothGDKCanvasUnCacheMidPointColor(SmoothGDKPrivateCanvas *Canvas, 
					SmoothColor _Point1,
					SmoothColor _Point2,
					SmoothColor *_Color)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && _Color)
	{
		result = SmoothGDKCanvasUnCacheColor(Canvas, _Color);
	}
	
	return result;
}

/* Canvas Get/Set/Clear Clip _Rectangle */
static SmoothBool
SmoothGDKCanvasGetClipRectangle(SmoothGDKPrivateCanvas *Canvas,
					SmoothRectangle *_Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && !Canvas->NoClip)
	{
		*_Rectangle = Canvas->Clip;
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
SmoothGDKCanvasSetClipRectangle(SmoothGDKPrivateCanvas *Canvas,
					SmoothRectangle _Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		Canvas->Clip = _Rectangle;
		Canvas->NoClip = SmoothFalse;
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
SmoothGDKCanvasClearClipRectangle(SmoothGDKPrivateCanvas *Canvas)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		Canvas->NoClip = SmoothTrue;
		result = SmoothTrue;
	}
	return result;
}

/* Canvas Get/Set Pen Properties */
static SmoothBool
SmoothGDKCanvasGetPenColor(SmoothGDKPrivateCanvas *Canvas, 
					SmoothColor *_Color)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && _Color)
	{
		*_Color = Canvas->Pen.Color;
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
SmoothGDKCanvasSetPenColor(SmoothGDKPrivateCanvas *Canvas, 
					SmoothColor _Color)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		Canvas->Pen.Color = _Color;
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
SmoothGDKCanvasGetPenThickness(SmoothGDKPrivateCanvas *Canvas,
					SmoothInt *Thickness)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && Thickness)
	{
		*Thickness = Canvas->Pen.Thickness;
		result = SmoothTrue;
	}
	return result;
}
	
static SmoothBool
SmoothGDKCanvasSetPenThickness(SmoothGDKPrivateCanvas *Canvas, 
					SmoothInt Thickness)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		Canvas->Pen.Thickness = Thickness;
		Canvas->Pen.UseThickness = SmoothTrue;
		
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool 
SmoothGDKCanvasGetPenStyle(SmoothGDKPrivateCanvas *Canvas, 
				SmoothLineStyle *Style)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && Style)
	{
		*Style = Canvas->Pen.Style;
		result = SmoothTrue;
	}
	return result;
}
				
static SmoothBool 
SmoothGDKCanvasSetPenStyle(SmoothGDKPrivateCanvas *Canvas, 
				SmoothLineStyle Style)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		Canvas->Pen.Style = Style;
		Canvas->Pen.UseStyle = SmoothTrue;
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool 
SmoothGDKCanvasGetPenJoin(SmoothGDKPrivateCanvas *Canvas, 
				SmoothLineJoin *Join)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		*Join = Canvas->Pen.Join;
		result = SmoothTrue;
	}
	return result;
}
				
static SmoothBool 
SmoothGDKCanvasSetPenJoin(SmoothGDKPrivateCanvas *Canvas, 
				SmoothLineJoin Join)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		Canvas->Pen.Join = Join;
		Canvas->Pen.UseJoin = SmoothTrue;
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool 
SmoothGDKCanvasGetPenCap(SmoothGDKPrivateCanvas *Canvas, 
				SmoothLineCap *Cap)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && Cap)
	{
		*Cap = Canvas->Pen.Cap;
		result = SmoothTrue;
	}
	return result;
}
				
static SmoothBool 
SmoothGDKCanvasSetPenCap(SmoothGDKPrivateCanvas *Canvas, 
				SmoothLineCap Cap)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		Canvas->Pen.Cap = Cap;
		Canvas->Pen.UseCap = SmoothTrue;
		
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool 
SmoothGDKCanvasGetPenPattern(SmoothGDKPrivateCanvas *Canvas, 
				SmoothLinePattern *Pattern)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && Pattern)
	{
		*Pattern = Canvas->Pen.Pattern;
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool 
SmoothGDKCanvasSetPenPattern(SmoothGDKPrivateCanvas *Canvas, 
				SmoothLinePattern Pattern)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		Canvas->Pen.Pattern = Pattern;
		Canvas->Pen.UsePattern = SmoothTrue;

		result = SmoothTrue;
	}
	return result;
}				

/* Canvas Get/Set Brush Properties */
static SmoothBool
SmoothGDKCanvasGetBrushColor(SmoothGDKPrivateCanvas *Canvas, 
					SmoothColor *_Color)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && _Color)
	{
		*_Color = Canvas->Brush.Color;
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
SmoothGDKCanvasSetBrushColor(SmoothGDKPrivateCanvas *Canvas, 
					SmoothColor _Color)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		Canvas->Brush.Color = _Color;
		result = SmoothTrue;
	}
	return result;
}

/* Initialize smoothDrawingInterface */
SmoothBool 
SmoothGDKDrawingInterfaceInitialize(DrawingInterfaceClass *_SmoothDrawingInterface)
{
	/* _Color Value Getions */
	_SmoothDrawingInterface->ColorGetRedValue = (void *) SmoothGDKColorGetRedValue;
	_SmoothDrawingInterface->ColorGetGreenValue = (void *) SmoothGDKColorGetGreenValue;
	_SmoothDrawingInterface->ColorGetBlueValue = (void *) SmoothGDKColorGetBlueValue;
	_SmoothDrawingInterface->ColorGetAlphaValue = (void *) SmoothGDKColorGetAlphaValue;

	/* _Color Value Modifications */
	_SmoothDrawingInterface->ColorSetRedValue = (void *) SmoothGDKColorSetRedValue;
	_SmoothDrawingInterface->ColorSetGreenValue = (void *) SmoothGDKColorSetGreenValue;
	_SmoothDrawingInterface->ColorSetBlueValue = (void *) SmoothGDKColorSetBlueValue;
	_SmoothDrawingInterface->ColorSetAlphaValue = (void *) SmoothGDKColorSetAlphaValue;

	/* _Rectangle Status Functions */
	_SmoothDrawingInterface->RectangleCreate = (void *) SmoothGDKRectangleCreate;
	_SmoothDrawingInterface->RectangleDestroy = (void *) SmoothGDKRectangleDestroy;
	_SmoothDrawingInterface->RectangleIsValid = (void *) SmoothGDKRectangleIsValid;

	/* _Rectangle Value Getions */
	_SmoothDrawingInterface->RectangleGetXValue = (void *) SmoothGDKRectangleGetXValue;
	_SmoothDrawingInterface->RectangleGetYValue = (void *) SmoothGDKRectangleGetYValue;
	_SmoothDrawingInterface->RectangleGetWidthValue = (void *) SmoothGDKRectangleGetWidthValue;
	_SmoothDrawingInterface->RectangleGetHeightValue = (void *) SmoothGDKRectangleGetHeightValue;
	
	/* _Rectangle Value Modifications */
	_SmoothDrawingInterface->RectangleSetXValue = (void *) SmoothGDKRectangleSetXValue;
	_SmoothDrawingInterface->RectangleSetYValue = (void *) SmoothGDKRectangleSetYValue;
	_SmoothDrawingInterface->RectangleSetWidthValue = (void *) SmoothGDKRectangleSetWidthValue;
	_SmoothDrawingInterface->RectangleSetHeightValue = (void *) SmoothGDKRectangleSetHeightValue;

	/* _Rectangle Misc Functions */
	_SmoothDrawingInterface->RectangleFindIntersection = (void *) SmoothGDKRectangleFindIntersection;

	/* _Point Value Getions */
	_SmoothDrawingInterface->PointGetXValue = (void *) SmoothGDKPointGetXValue;
	_SmoothDrawingInterface->PointGetYValue = (void *) SmoothGDKPointGetYValue;

	/* _Point Value Modifications */
	_SmoothDrawingInterface->PointSetXValue = (void *) SmoothGDKPointSetXValue;
	_SmoothDrawingInterface->PointSetYValue = (void *) SmoothGDKPointSetYValue;

	/* Canvas Map/Free A _Color From The Colormap*/
	_SmoothDrawingInterface->CanvasAllocateColor = (void *) SmoothGDKCanvasAllocateColor;
	_SmoothDrawingInterface->CanvasDeallocateColor = (void *) SmoothGDKCanvasDeallocateColor;

	/* Canvas cache/uncache a _Color */
	_SmoothDrawingInterface->CanvasCacheColor = (void *) SmoothGDKCanvasCacheColor;
	_SmoothDrawingInterface->CanvasUnCacheColor = (void *) SmoothGDKCanvasUnCacheColor;
			
	/* Canvas cache/uncache a shaded _Color */
	_SmoothDrawingInterface->CanvasCacheShadedColor = (void *) SmoothGDKCanvasCacheShadedColor;
	_SmoothDrawingInterface->CanvasUnCacheShadedColor = (void *) SmoothGDKCanvasUnCacheShadedColor;
	
	/* Canvas cache/uncache a midpoint _Color */
	_SmoothDrawingInterface->CanvasCacheMidPointColor = (void *) SmoothGDKCanvasCacheMidPointColor;
	_SmoothDrawingInterface->CanvasUnCacheMidPointColor = (void *) SmoothGDKCanvasUnCacheMidPointColor;

	/* Canvas Get/Set Clip _Rectangle */
	_SmoothDrawingInterface->CanvasGetClipRectangle = (void *) SmoothGDKCanvasGetClipRectangle;
	_SmoothDrawingInterface->CanvasSetClipRectangle = (void *) SmoothGDKCanvasSetClipRectangle;
	_SmoothDrawingInterface->CanvasClearClipRectangle = (void *) SmoothGDKCanvasClearClipRectangle;

	/* Canvas Get/Set Pen Properties */
	_SmoothDrawingInterface->CanvasGetPenColor = (void *) SmoothGDKCanvasGetPenColor;
	_SmoothDrawingInterface->CanvasSetPenColor = (void *) SmoothGDKCanvasSetPenColor;	
	_SmoothDrawingInterface->CanvasGetPenThickness = (void *) SmoothGDKCanvasGetPenThickness;
	_SmoothDrawingInterface->CanvasSetPenThickness = (void *) SmoothGDKCanvasSetPenThickness;
	_SmoothDrawingInterface->CanvasGetPenStyle = (void *) SmoothGDKCanvasGetPenStyle;
	_SmoothDrawingInterface->CanvasSetPenStyle = (void *) SmoothGDKCanvasSetPenStyle;
	_SmoothDrawingInterface->CanvasGetPenCap = (void *) SmoothGDKCanvasGetPenCap;
	_SmoothDrawingInterface->CanvasSetPenCap = (void *) SmoothGDKCanvasSetPenCap;
	_SmoothDrawingInterface->CanvasGetPenJoin = (void *) SmoothGDKCanvasGetPenJoin;
	_SmoothDrawingInterface->CanvasSetPenJoin = (void *) SmoothGDKCanvasSetPenJoin;
	_SmoothDrawingInterface->CanvasGetPenPattern = (void *) SmoothGDKCanvasGetPenPattern;
	_SmoothDrawingInterface->CanvasSetPenPattern = (void *) SmoothGDKCanvasSetPenPattern;

	/* Canvas Get/Set Brush Properties */
	_SmoothDrawingInterface->CanvasGetBrushColor = (void *) SmoothGDKCanvasGetBrushColor;
	_SmoothDrawingInterface->CanvasSetBrushColor = (void *) SmoothGDKCanvasSetBrushColor;	


	return SmoothTrue;
}

SmoothBool 
SmoothGDKDrawingInterfaceFinalize(DrawingInterfaceClass *_SmoothDrawingInterface)
{
	smooth_cleanup_color_cache(SmoothTrue);
	
	return SmoothTrue;
}

void SmoothGDKModifyCanvasClipArea(SmoothCanvas *Canvas,
				GdkRectangle * _Rectangle)
{
	SmoothGDKPrivateCanvas *PrivateCanvas;

	PrivateCanvas = *Canvas;

	PrivateCanvas->EngineClip.Rectangle = _Rectangle;
}

void SmoothGDKModifyCanvasClipRegion(SmoothCanvas *Canvas,
				GdkRegion * _Region)
{
	SmoothGDKPrivateCanvas *PrivateCanvas;

	PrivateCanvas = *Canvas;

	PrivateCanvas->EngineClip.Region = _Region;
}

void SmoothGDKModifyCanvasClipMask(SmoothCanvas *Canvas,
				GdkBitmap *ClipMask)
{
	SmoothGDKPrivateCanvas *PrivateCanvas;

	PrivateCanvas = *Canvas;

	PrivateCanvas->EngineClip.BitmapMask = ClipMask;
}

void SmoothGDKModifyCanvasClipOffset(SmoothCanvas *Canvas,
				SmoothInt XOffset,
				SmoothInt YOffset)
{
	SmoothGDKPrivateCanvas *PrivateCanvas;

	PrivateCanvas = *Canvas;

	PrivateCanvas->EngineClip.XOffset = XOffset;
	PrivateCanvas->EngineClip.YOffset = YOffset;
}

void SmoothGDKModifyCanvasDitherDepth(SmoothCanvas *Canvas,
				SmoothInt DitherDepth)
{
	SmoothGDKPrivateCanvas *PrivateCanvas = *Canvas;

	PrivateCanvas->DitherDepth = DitherDepth;
}

void SmoothColorAssignGdkColor(SmoothColor *DestinationColor, GdkColor SourceColor, SmoothInt SourceAlpha)
{
 	DestinationColor->RGB = SourceColor;
	DestinationColor->Alpha = SourceAlpha;
	DestinationColor->CacheIndex = -1;
}

SmoothWidgetState
SmoothGtkWidgetState(GtkStateType State)
{
	SmoothWidgetState result = SMOOTH_STATE_DEFAULT;
	
	switch (State)
	{
		case GTK_STATE_NORMAL : 
			result = SMOOTH_STATE_DEFAULT;
		break;
		
		case GTK_STATE_PRELIGHT : 
			result = SMOOTH_STATE_PRELIGHT;
		break;
		
		case GTK_STATE_SELECTED : 
			result = SMOOTH_STATE_SELECTED;
		break;
		
		case GTK_STATE_ACTIVE : 
			result = SMOOTH_STATE_ACTIVE;
		break;
		
		case GTK_STATE_INSENSITIVE : 
			result = SMOOTH_STATE_DISABLED;
		break;
	}
	
	return result;
}

void 
SmoothGDKInitializeColorCube(SmoothCanvas Canvas, 
			GtkStyle * style,
			SmoothColorCube *ColorCube)
{
	if (ColorCube->ReferenceCount == 0)
	{
		SmoothInt i;
		for (i = 0; i< 5; i++)
		{
			SmoothWidgetState state = SmoothGtkWidgetState(i);
			
			/* Interaction Pairs */
			SmoothColorAssignGdkColor(&ColorCube->Interaction[state].Background, style->bg[i], 1.0);
			SmoothGDKCanvasCacheColor(Canvas, &ColorCube->Interaction[state].Background);
			
			SmoothColorAssignGdkColor(&ColorCube->Interaction[state].Foreground, style->fg[i], 1.0);
			SmoothGDKCanvasCacheColor(Canvas, &ColorCube->Interaction[state].Foreground);

			/* Input Pairs */
			SmoothColorAssignGdkColor(&ColorCube->Input[state].Background, style->base[i], 1.0);
			SmoothGDKCanvasCacheColor(Canvas, &ColorCube->Input[state].Background);

			SmoothColorAssignGdkColor(&ColorCube->Input[state].Foreground, style->text[i], 1.0);
			SmoothGDKCanvasCacheColor(Canvas, &ColorCube->Input[state].Foreground);
		}
		ColorCube->ReferenceCount++;
	}
}

void 
SmoothGDKFinalizeColorCube(SmoothColorCube *ColorCube)
{
	ColorCube->ReferenceCount--;
	if (ColorCube->ReferenceCount <= 0)
	{
		SmoothInt i;
		for (i = 0; i< 5; i++)
		{
			SmoothWidgetState state = SmoothGtkWidgetState(i);
			
			/* Interaction Pairs */
			SmoothGDKCanvasUnCacheColor(NULL, &ColorCube->Interaction[state].Background);
			
			SmoothGDKCanvasUnCacheColor(NULL, &ColorCube->Interaction[state].Foreground);

			/* Input Pairs */
			SmoothGDKCanvasUnCacheColor(NULL, &ColorCube->Input[state].Background);

			SmoothGDKCanvasUnCacheColor(NULL, &ColorCube->Input[state].Foreground);
		}
	}
}
