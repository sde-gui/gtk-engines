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
GDKColorGetRedValue(SmoothColor *_Color, 
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
GDKColorGetGreenValue(SmoothColor *_Color, 
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
GDKColorGetBlueValue(SmoothColor *_Color, 
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
GDKColorGetAlphaValue(SmoothColor *_Color, 
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
GDKColorSetRedValue(SmoothColor *_Color, 
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
GDKColorSetGreenValue(SmoothColor *_Color, 
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
GDKColorSetBlueValue(SmoothColor *_Color, 
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
GDKColorSetAlphaValue(SmoothColor *_Color, 
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
GDKRectangleCreate(SmoothRectangle *_Rectangle, 
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
GDKRectangleDestroy(SmoothRectangle *_Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (_Rectangle)
	{
		result = SmoothTrue;
	}
	
	return result;
}

static SmoothBool 
GDKRectangleIsValid(SmoothRectangle *_Rectangle)
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
GDKRectangleGetXValue(SmoothRectangle *_Rectangle, SmoothInt *_XValue)
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
GDKRectangleGetYValue(SmoothRectangle *_Rectangle, SmoothInt *_YValue)
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
GDKRectangleGetWidthValue(SmoothRectangle *_Rectangle, SmoothInt *_WidthValue)
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
GDKRectangleGetHeightValue(SmoothRectangle *_Rectangle, SmoothInt *_HeightValue)
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
GDKRectangleSetXValue(SmoothRectangle *_Rectangle, 
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
GDKRectangleSetYValue(SmoothRectangle *_Rectangle, 
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
GDKRectangleSetWidthValue(SmoothRectangle *_Rectangle, 
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
GDKRectangleSetHeightValue(SmoothRectangle *_Rectangle, 
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
GDKRectangleFindIntersection(SmoothRectangle _Rectangle1, 
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
GDKPointGetXValue(SmoothPoint *_Point,
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
GDKPointGetYValue(SmoothPoint *_Point, 
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
GDKPointSetXValue(SmoothPoint *_Point,
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
GDKPointSetYValue(SmoothPoint *_Point,
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
GDKCanvasAllocateColor(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasDeallocateColor(SmoothGDKPrivateCanvas *Canvas, 
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

static GHashTable* color_cache = NULL;

typedef struct
{
	SmoothColor 	_Color;
	GdkColormap	*Colormap;

	SmoothInt		ref_count;
} GdkCachedColor;

static void
free_color_cache(GdkCachedColor *Cache)
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
force_cleanup_color_cache(gpointer key, gpointer value, gpointer user_data)
{
	if (value) 
	{
		free_color_cache((GdkCachedColor *)value);
	}	
	return SmoothTrue;
}

static void
cleanup_color_cache(SmoothBool Force)
{
	if (color_cache) 
	{
		if (Force)
		{
			g_hash_table_foreach_remove(color_cache, force_cleanup_color_cache, NULL);
		}
		
		if (g_hash_table_size(color_cache)<=0) 
		{
			g_hash_table_destroy(color_cache);
			color_cache = NULL;
		}  
	}   
}

static SmoothColor *
internal_color_get_color(GdkColormap *Colormap, SmoothColor *_Color, SmoothDouble Shade,  SmoothInt index)
{
	GdkCachedColor *cache=NULL;
	SmoothColor *result=NULL;
	
	if ((index < 0) && (_Color))	
	{
		index = cached_color_hash(_Color, Shade);
	}	

	if (!(color_cache))
	{
		color_cache = g_hash_table_new(g_int_hash, g_int_equal);
	}
	
	cache = g_hash_table_lookup(color_cache, &index);
   
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
			g_hash_table_insert(color_cache, &cache->_Color.CacheIndex, cache);
			
		result = &cache->_Color;
	}
	
	return result;
}

static void
internal_color_unref(SmoothInt index)
{  
	GdkCachedColor *cache = NULL;

	if (color_cache) 
	{
		cache = g_hash_table_lookup(color_cache, &index);

		if (cache) 
		{
			cache->ref_count--;
   
			if (cache->ref_count == 0) 
			{
				g_hash_table_remove(color_cache, &index);
				free_color_cache(cache);
			}
		}
		cleanup_color_cache(SmoothFalse);  
	}   
}

GdkGC *
internal_drawing_area_use_pen_gc(SmoothGDKPrivateCanvas *Canvas, SmoothBool RequireValidColor)
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

			fg = internal_color_get_color(Canvas->Colormap, &Canvas->Pen.Color, 1.0, value);
			
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
				 gdk_gc_set_dashes(result, Canvas->Pen.Pattern.Offset, (gint8 *)Canvas->Pen.Pattern.Pattern, Canvas->Pen.Pattern.Length); 
			}
		}	
	}	
	
	return result;
	
}

/* Canvas cache/uncache a shaded _Color */
static SmoothBool
GDKCanvasCacheShadedColor(SmoothGDKPrivateCanvas *Canvas, 
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

			tmp = internal_color_get_color(Canvas->Colormap, &Base, Shade, -1);
		
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
GDKCanvasUnCacheShadedColor(SmoothGDKPrivateCanvas *Canvas, 
					SmoothColor Base,
					SmoothDouble Shade,
					SmoothColor *_Color)
{
	SmoothBool result = SmoothFalse;
	
	if (_Color)
	{
		if (Base.Alpha > 0)
		{		
			internal_color_unref(_Color->CacheIndex);
			_Color->CacheIndex = -1;
		}	

		result = SmoothTrue;
	}
	
	return result;
}

static SmoothBool
GDKCanvasCacheColor(SmoothGDKPrivateCanvas *Canvas, 
				SmoothColor *_Color)
{
	SmoothColor tmp = *_Color;
	
	return GDKCanvasCacheShadedColor(Canvas, tmp, 1.0, _Color);
}

static SmoothBool
GDKCanvasUnCacheColor(SmoothGDKPrivateCanvas *Canvas, 
				SmoothColor *_Color)
{
	SmoothBool result = SmoothFalse;
	
	if (_Color)
	{
		if (_Color->Alpha > 0)
		{						
		
			internal_color_unref(_Color->CacheIndex);
			_Color->CacheIndex = -1;
		}
		result = SmoothTrue;
	}
	
	return result;
}									


/* Canvas cache/uncache a midpoint _Color */
static SmoothBool
GDKCanvasCacheMidPointColor(SmoothGDKPrivateCanvas *Canvas, 
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

		result = GDKCanvasCacheShadedColor(Canvas, *_Color, 1.0, _Color);
	}
	
	return result;
}

static SmoothBool
GDKCanvasUnCacheMidPointColor(SmoothGDKPrivateCanvas *Canvas, 
					SmoothColor _Point1,
					SmoothColor _Point2,
					SmoothColor *_Color)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && _Color)
	{
		result = GDKCanvasUnCacheColor(Canvas, _Color);
	}
	
	return result;
}

/* Canvas Get/Set/Clear Clip _Rectangle */
static SmoothBool
GDKCanvasGetClipRectangle(SmoothGDKPrivateCanvas *Canvas,
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
GDKCanvasSetClipRectangle(SmoothGDKPrivateCanvas *Canvas,
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
GDKCanvasClearClipRectangle(SmoothGDKPrivateCanvas *Canvas)
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
GDKCanvasGetPenColor(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasSetPenColor(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasGetPenThickness(SmoothGDKPrivateCanvas *Canvas,
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
GDKCanvasSetPenThickness(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasGetPenStyle(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasSetPenStyle(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasGetPenJoin(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasSetPenJoin(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasGetPenCap(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasSetPenCap(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasGetPenPattern(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasSetPenPattern(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasGetBrushColor(SmoothGDKPrivateCanvas *Canvas, 
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
GDKCanvasSetBrushColor(SmoothGDKPrivateCanvas *Canvas, 
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

/* Initialize drawingInterface */
SmoothBool 
GDKDrawingInterfaceInitialize(DrawingInterfaceClass *_DrawingInterface)
{
	/* _Color Value Getions */
	_DrawingInterface->ColorGetRedValue = (void *) GDKColorGetRedValue;
	_DrawingInterface->ColorGetGreenValue = (void *) GDKColorGetGreenValue;
	_DrawingInterface->ColorGetBlueValue = (void *) GDKColorGetBlueValue;
	_DrawingInterface->ColorGetAlphaValue = (void *) GDKColorGetAlphaValue;

	/* _Color Value Modifications */
	_DrawingInterface->ColorSetRedValue = (void *) GDKColorSetRedValue;
	_DrawingInterface->ColorSetGreenValue = (void *) GDKColorSetGreenValue;
	_DrawingInterface->ColorSetBlueValue = (void *) GDKColorSetBlueValue;
	_DrawingInterface->ColorSetAlphaValue = (void *) GDKColorSetAlphaValue;

	/* _Rectangle Status Functions */
	_DrawingInterface->RectangleCreate = (void *) GDKRectangleCreate;
	_DrawingInterface->RectangleDestroy = (void *) GDKRectangleDestroy;
	_DrawingInterface->RectangleIsValid = (void *) GDKRectangleIsValid;

	/* _Rectangle Value Getions */
	_DrawingInterface->RectangleGetXValue = (void *) GDKRectangleGetXValue;
	_DrawingInterface->RectangleGetYValue = (void *) GDKRectangleGetYValue;
	_DrawingInterface->RectangleGetWidthValue = (void *) GDKRectangleGetWidthValue;
	_DrawingInterface->RectangleGetHeightValue = (void *) GDKRectangleGetHeightValue;
	
	/* _Rectangle Value Modifications */
	_DrawingInterface->RectangleSetXValue = (void *) GDKRectangleSetXValue;
	_DrawingInterface->RectangleSetYValue = (void *) GDKRectangleSetYValue;
	_DrawingInterface->RectangleSetWidthValue = (void *) GDKRectangleSetWidthValue;
	_DrawingInterface->RectangleSetHeightValue = (void *) GDKRectangleSetHeightValue;

	/* _Rectangle Misc Functions */
	_DrawingInterface->RectangleFindIntersection = (void *) GDKRectangleFindIntersection;

	/* _Point Value Getions */
	_DrawingInterface->PointGetXValue = (void *) GDKPointGetXValue;
	_DrawingInterface->PointGetYValue = (void *) GDKPointGetYValue;

	/* _Point Value Modifications */
	_DrawingInterface->PointSetXValue = (void *) GDKPointSetXValue;
	_DrawingInterface->PointSetYValue = (void *) GDKPointSetYValue;

	/* Canvas Map/Free A _Color From The Colormap*/
	_DrawingInterface->CanvasAllocateColor = (void *) GDKCanvasAllocateColor;
	_DrawingInterface->CanvasDeallocateColor = (void *) GDKCanvasDeallocateColor;

	/* Canvas cache/uncache a _Color */
	_DrawingInterface->CanvasCacheColor = (void *) GDKCanvasCacheColor;
	_DrawingInterface->CanvasUnCacheColor = (void *) GDKCanvasUnCacheColor;
			
	/* Canvas cache/uncache a shaded _Color */
	_DrawingInterface->CanvasCacheShadedColor = (void *) GDKCanvasCacheShadedColor;
	_DrawingInterface->CanvasUnCacheShadedColor = (void *) GDKCanvasUnCacheShadedColor;
	
	/* Canvas cache/uncache a midpoint _Color */
	_DrawingInterface->CanvasCacheMidPointColor = (void *) GDKCanvasCacheMidPointColor;
	_DrawingInterface->CanvasUnCacheMidPointColor = (void *) GDKCanvasUnCacheMidPointColor;

	/* Canvas Get/Set Clip _Rectangle */
	_DrawingInterface->CanvasGetClipRectangle = (void *) GDKCanvasGetClipRectangle;
	_DrawingInterface->CanvasSetClipRectangle = (void *) GDKCanvasSetClipRectangle;
	_DrawingInterface->CanvasClearClipRectangle = (void *) GDKCanvasClearClipRectangle;

	/* Canvas Get/Set Pen Properties */
	_DrawingInterface->CanvasGetPenColor = (void *) GDKCanvasGetPenColor;
	_DrawingInterface->CanvasSetPenColor = (void *) GDKCanvasSetPenColor;	
	_DrawingInterface->CanvasGetPenThickness = (void *) GDKCanvasGetPenThickness;
	_DrawingInterface->CanvasSetPenThickness = (void *) GDKCanvasSetPenThickness;
	_DrawingInterface->CanvasGetPenStyle = (void *) GDKCanvasGetPenStyle;
	_DrawingInterface->CanvasSetPenStyle = (void *) GDKCanvasSetPenStyle;
	_DrawingInterface->CanvasGetPenCap = (void *) GDKCanvasGetPenCap;
	_DrawingInterface->CanvasSetPenCap = (void *) GDKCanvasSetPenCap;
	_DrawingInterface->CanvasGetPenJoin = (void *) GDKCanvasGetPenJoin;
	_DrawingInterface->CanvasSetPenJoin = (void *) GDKCanvasSetPenJoin;
	_DrawingInterface->CanvasGetPenPattern = (void *) GDKCanvasGetPenPattern;
	_DrawingInterface->CanvasSetPenPattern = (void *) GDKCanvasSetPenPattern;

	/* Canvas Get/Set Brush Properties */
	_DrawingInterface->CanvasGetBrushColor = (void *) GDKCanvasGetBrushColor;
	_DrawingInterface->CanvasSetBrushColor = (void *) GDKCanvasSetBrushColor;	


	return SmoothTrue;
}

SmoothBool 
GDKDrawingInterfaceFinalize(DrawingInterfaceClass *_DrawingInterface)
{
	cleanup_color_cache(SmoothTrue);
	
	return SmoothTrue;
}

void GDKModifyCanvasClipArea(SmoothCanvas *Canvas,
				GdkRectangle * _Rectangle)
{
	SmoothGDKPrivateCanvas *PrivateCanvas;

	PrivateCanvas = *Canvas;

	PrivateCanvas->EngineClip.Rectangle = _Rectangle;
}

void GDKModifyCanvasClipRegion(SmoothCanvas *Canvas,
				GdkRegion * _Region)
{
	SmoothGDKPrivateCanvas *PrivateCanvas;

	PrivateCanvas = *Canvas;

	PrivateCanvas->EngineClip.Region = _Region;
}

void GDKModifyCanvasClipMask(SmoothCanvas *Canvas,
				GdkBitmap *ClipMask)
{
	SmoothGDKPrivateCanvas *PrivateCanvas;

	PrivateCanvas = *Canvas;

	PrivateCanvas->EngineClip.BitmapMask = ClipMask;
}

void GDKModifyCanvasClipOffset(SmoothCanvas *Canvas,
				SmoothInt XOffset,
				SmoothInt YOffset)
{
	SmoothGDKPrivateCanvas *PrivateCanvas;

	PrivateCanvas = *Canvas;

	PrivateCanvas->EngineClip.XOffset = XOffset;
	PrivateCanvas->EngineClip.YOffset = YOffset;
}

void GDKModifyCanvasDitherDepth(SmoothCanvas *Canvas,
				SmoothInt DitherDepth)
{
	SmoothGDKPrivateCanvas *PrivateCanvas = *Canvas;

	PrivateCanvas->DitherDepth = DitherDepth;
}

void GDKSmoothColorAssignGdkColor(SmoothColor *DestinationColor, GdkColor SourceColor, SmoothInt SourceAlpha)
{
 	DestinationColor->RGB = SourceColor;
	DestinationColor->Alpha = SourceAlpha;
	DestinationColor->CacheIndex = -1;
}

SmoothWidgetState
GDKSmoothWidgetState(GtkStateType State)
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
GDKInitializeColorCube(SmoothCanvas Canvas, 
			GtkStyle * style,
			SmoothColorCube *ColorCube)
{
	if (ColorCube->ReferenceCount == 0)
	{
		SmoothInt i;
		for (i = 0; i< 5; i++)
		{
			SmoothWidgetState state = GDKSmoothWidgetState(i);
			
			/* Interaction Pairs */
			GDKSmoothColorAssignGdkColor(&ColorCube->Interaction[state].Background, style->bg[i], 1.0);
			GDKCanvasCacheColor(Canvas, &ColorCube->Interaction[state].Background);
			
			GDKSmoothColorAssignGdkColor(&ColorCube->Interaction[state].Foreground, style->fg[i], 1.0);
			GDKCanvasCacheColor(Canvas, &ColorCube->Interaction[state].Foreground);

			/* Input Pairs */
			GDKSmoothColorAssignGdkColor(&ColorCube->Input[state].Background, style->base[i], 1.0);
			GDKCanvasCacheColor(Canvas, &ColorCube->Input[state].Background);

			GDKSmoothColorAssignGdkColor(&ColorCube->Input[state].Foreground, style->text[i], 1.0);
			GDKCanvasCacheColor(Canvas, &ColorCube->Input[state].Foreground);
		}
		ColorCube->ReferenceCount++;
	}
}

void 
GDKFinalizeColorCube(SmoothColorCube *ColorCube)
{
	ColorCube->ReferenceCount--;
	if (ColorCube->ReferenceCount <= 0)
	{
		SmoothInt i;
		for (i = 0; i< 5; i++)
		{
			SmoothWidgetState state = GDKSmoothWidgetState(i);
			
			/* Interaction Pairs */
			GDKCanvasUnCacheColor(NULL, &ColorCube->Interaction[state].Background);
			
			GDKCanvasUnCacheColor(NULL, &ColorCube->Interaction[state].Foreground);

			/* Input Pairs */
			GDKCanvasUnCacheColor(NULL, &ColorCube->Input[state].Background);

			GDKCanvasUnCacheColor(NULL, &ColorCube->Input[state].Foreground);
		}
	}
}
