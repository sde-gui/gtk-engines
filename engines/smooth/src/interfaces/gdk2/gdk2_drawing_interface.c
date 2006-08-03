/******************************************************************************/
/* gdk2_drawing_interface.c - GDK2 Drawing Interface                          */
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
#include "gdk2_private_drawing_interface.h"

GdkGC *smooth_internal_drawing_area_use_pen_gc(SmoothGDKPrivateCanvas *Canvas, SmoothBool RequireValidColor);
void smooth_internal_drawing_area_unuse_pen_gc(SmoothGDKPrivateCanvas *Canvas, GdkGC * GC);

GdkGC * smooth_internal_drawing_area_use_brush_gc(SmoothGDKPrivateCanvas *Canvas, SmoothBool RequireValidColor);
void smooth_internal_drawing_area_unuse_brush_gc(SmoothGDKPrivateCanvas *Canvas, GdkGC * GC);

/* some basic math defines */
#define SQR(x) x*x

static SmoothBool _SmoothDrawingInterfaceInitialize(DrawingInterfaceClass *_SmoothDrawingInterface);
static SmoothBool _SmoothDrawingInterfaceFinalize(DrawingInterfaceClass *_SmoothDrawingInterface);

/* Image/Buffer By File name (For caching purposes) */
static void smooth_cleanup_gdk_pixbuf_cache(SmoothBool force);/* not part of class */

static SmoothBool SmoothGDK2ImageBufferLoadByFile(SmoothString ImageFile, SmoothImageBuffer *ImageBuffer);
static SmoothBool SmoothGDK2ImageBufferUnLoadByFile(SmoothString ImageFile);

/* Canvas Get ImageBuffer Size */
static SmoothBool SmoothGDK2ImageBufferGetWidth(SmoothImageBuffer *ImageBuffer,
						SmoothInt *Width);
static SmoothBool SmoothGDK2ImageBufferGetHeight(SmoothImageBuffer *ImageBuffer,
						SmoothInt *Height);

/* Canvas Get Image Size */
static SmoothBool SmoothGDK2ImageGetWidth(SmoothImage *Image,
						SmoothInt *Width);
static SmoothBool SmoothGDK2ImageGetHeight(SmoothImage *Image,
						SmoothInt *Height);

/* Canvas Store/Render */
static SmoothBool SmoothGDK2CanvasStore(SmoothPrivateCanvas *Canvas);
static SmoothBool SmoothGDK2CanvasRender(SmoothPrivateCanvas *Canvas,	
					SmoothBool ClosePathAndFill);

/* Canvas Create Image */
static SmoothBool SmoothGDK2CanvasCreateImage(SmoothPrivateCanvas *Canvas,
						SmoothInt Width,
						SmoothInt Height,
						SmoothImage *Image);
						
static SmoothBool SmoothGDK2CanvasCreateImageFromBuffer(SmoothPrivateCanvas *Canvas,
							SmoothImageBuffer ImageBuffer,
							SmoothImage *Image);
							
static SmoothBool SmoothGDK2CanvasDestroyImage(SmoothPrivateCanvas *Canvas,
						SmoothImage *Image);

/* Canvas Tile Image/ImageBuffer */
static SmoothBool SmoothGDK2CanvasRenderImage(SmoothPrivateCanvas *Canvas,
						SmoothImage Image,
						SmoothInt X,
						SmoothInt Y,
						SmoothInt Width,
						SmoothInt Height);
						
static SmoothBool SmoothGDK2CanvasRenderImageBuffer(SmoothPrivateCanvas *Canvas,
							SmoothImageBuffer ImageBuffer,
							SmoothInt X,
							SmoothInt Y,
							SmoothInt Width,
							SmoothInt Height);

/* Canvas Draw _Point(s) */
static SmoothBool SmoothGDK2CanvasDrawPoint(SmoothPrivateCanvas *Canvas,
						SmoothInt X,
						SmoothInt Y);
						
static SmoothBool SmoothGDK2CanvasDrawPoints(SmoothPrivateCanvas *Canvas,
						SmoothPoint *Points, 
						SmoothInt NumberOfPoints);

/* Canvas Draw Line(s) */
static SmoothBool SmoothGDK2CanvasDrawLine(SmoothPrivateCanvas *Canvas,
						SmoothInt X1,
						SmoothInt Y1,
						SmoothInt X2,
						SmoothInt Y2);
						
static SmoothBool SmoothGDK2CanvasDrawSegment(SmoothPrivateCanvas *Canvas,
						SmoothPoint _Point1, 
						SmoothPoint _Point2);

static SmoothBool SmoothGDK2CanvasDrawLines(SmoothPrivateCanvas *Canvas,
						SmoothPoint *Points, 
						SmoothInt NumberOfPoints);

/* Canvas Draw Arc(s)/Curve(s) */
static SmoothBool SmoothGDK2CanvasDrawArc(SmoothPrivateCanvas *Canvas,
					SmoothInt X, 
					SmoothInt Y, 
					SmoothInt Width, 
					SmoothInt Height,
					SmoothDouble AnglePosition,
					SmoothDouble AngleLength);

/* Canvas Frame Shaped Areas */
static SmoothBool SmoothGDK2CanvasFrameRectangle(SmoothPrivateCanvas *Canvas,
						SmoothInt X, 
						SmoothInt Y, 
						SmoothInt Width, 
						SmoothInt Height);
						
static SmoothBool SmoothGDK2CanvasFramePolygon(SmoothPrivateCanvas *Canvas,
						SmoothPoint *Points, 
						SmoothInt NumberOfPoints);

static SmoothBool SmoothGDK2CanvasFrameChord(SmoothPrivateCanvas *Canvas,
						SmoothInt X, 
						SmoothInt Y, 
						SmoothInt Width, 
						SmoothInt Height,
						SmoothDouble AnglePosition,
						SmoothDouble AngleLength);

/* Canvas Fill Shaped Areas */
static SmoothBool SmoothGDK2CanvasFillRectangle(SmoothPrivateCanvas *Canvas,
						SmoothInt X, 
						SmoothInt Y, 
						SmoothInt Width, 
						SmoothInt Height);
						
static SmoothBool SmoothGDK2CanvasFillPolygon(SmoothPrivateCanvas *Canvas,
						SmoothPoint *Points, 
						SmoothInt NumberOfPoints);

static SmoothBool SmoothGDK2CanvasFillChord(SmoothPrivateCanvas *Canvas,
						SmoothInt X, 
						SmoothInt Y, 
						SmoothInt Width, 
						SmoothInt Height,
						SmoothDouble AnglePosition,
						SmoothDouble AngleLength);

#ifdef SMOOTHGRADIENT
#include "src/gdk2_gradient_routines.h"
#endif

/* Internal Clip EVILNESS */
typedef enum {GDK2_CLIP_NONE, GDK2_CLIP_EMPTY, GDK2_CLIP_RECTANGLE, GDK2_CLIP_REGION, GDK2_CLIP_BITMAP} GDK2ClipType;

static void *  SmoothGDKGCSetClipArea(SmoothPrivateCanvas *Canvas, GdkGC *GC, GDK2ClipType *clip_type);
static void SmoothGDKGCUnsetClipArea(SmoothPrivateCanvas *Canvas, GdkGC *GC, void *ClipArea, GDK2ClipType clip_type);

/* Internal Image Buffers */
static void
smooth_internal_image_buffer_free_pixels (guchar *pixels, gpointer data)
{
	g_free (pixels);
}

static SmoothImageBuffer
smooth_internal_image_buffer_new (SmoothInt width, SmoothInt height)
{
  SmoothUChar *buf;
  int rowstride;

  g_return_val_if_fail (width > 0, NULL);
  g_return_val_if_fail (height > 0, NULL);

  rowstride = width * 3;

  buf = g_try_malloc (height * rowstride);

  if (!buf)
    return NULL;

  return gdk_pixbuf_new_from_data (buf, GDK_COLORSPACE_RGB,
                                   FALSE, 8,
                                   width, height, rowstride,
                                   smooth_internal_image_buffer_free_pixels, NULL);
}

/* the following are routines for caching images by file name */
static GHashTable* smooth_pixbuf_cache = NULL;

typedef struct
{
	GString 	*FileName;
	GdkPixbuf	*ImageBuffer;
	SmoothInt		ref_count;
} GdkCachedImageBuffer;

static void
smooth_free_pixbuf_cache(GdkCachedImageBuffer *cache)
{
	g_object_unref(cache->ImageBuffer);
	g_string_free(cache->FileName, SmoothTrue);
	g_free(cache);
}

static GdkCachedImageBuffer *
smooth_new_pixbuf_cache(SmoothString FileName)
{
	GdkCachedImageBuffer *result=NULL;

	result = g_new0(GdkCachedImageBuffer, 1);
	result->ref_count = 1;
	result->FileName =  g_string_sized_new(FileName->len);
   	g_string_assign(result->FileName, FileName->str);
	result->ImageBuffer = gdk_pixbuf_new_from_file(FileName->str, NULL);

	return result;
}

static SmoothBool 
smooth_force_cleanup_pixbuf_cache(gpointer key, gpointer value, gpointer user_data)
{
	if (value) 
	{
		smooth_free_pixbuf_cache((GdkCachedImageBuffer *)value);
	}	
	return SmoothTrue;
}

static void
smooth_cleanup_gdk_pixbuf_cache(SmoothBool force)
{
	if (smooth_pixbuf_cache) 
	{
		if (force)
		{
			g_hash_table_foreach_remove(smooth_pixbuf_cache, smooth_force_cleanup_pixbuf_cache, NULL);
		}
		
		if (g_hash_table_size(smooth_pixbuf_cache)<=0) 
		{
			g_hash_table_destroy(smooth_pixbuf_cache);
			smooth_pixbuf_cache = NULL;
		}  
	}   
}

static GdkPixbuf *
internal_gdk_pixbuf_get_by_name(SmoothString file_name)
{
	GdkCachedImageBuffer *cache=NULL;
	GdkPixbuf *result=NULL;
   
	if (!smooth_pixbuf_cache)
	{
		smooth_pixbuf_cache=g_hash_table_new(g_str_hash, g_str_equal);
	}
	
	cache = g_hash_table_lookup(smooth_pixbuf_cache, file_name->str);
   
	if (!cache) 
	{
		cache = smooth_new_pixbuf_cache(file_name);

		if (cache) 
			g_hash_table_insert(smooth_pixbuf_cache, cache->FileName->str, cache);
	}
  
	if (cache) 
	{
		result = cache->ImageBuffer;
		cache->ref_count++;
	}
   
	return result;
}

static void
internal_gdk_pixbuf_unref(SmoothString file_name)
{  
	GdkCachedImageBuffer *cache = NULL;

	if (smooth_pixbuf_cache) {
		cache = g_hash_table_lookup(smooth_pixbuf_cache, file_name->str);

		if (cache) {
			cache->ref_count--;
   
			if (cache->ref_count == 0) {
				g_hash_table_remove(smooth_pixbuf_cache, file_name->str);
				smooth_free_pixbuf_cache(cache);
			}   
		}
		smooth_cleanup_gdk_pixbuf_cache(SmoothFalse);  
	}   
}

static SmoothBool 
SmoothGDK2ImageBufferLoadByFile(SmoothString ImageFile,
				SmoothImageBuffer *ImageBuffer)
{
	SmoothBool result = SmoothFalse;
	if (ImageFile)
	{
		*ImageBuffer = internal_gdk_pixbuf_get_by_name(ImageFile);
		result = SmoothTrue;
	}

	return result;
}

static SmoothBool
SmoothGDK2ImageBufferUnLoadByFile(SmoothString ImageFile)
{
	SmoothBool result = SmoothFalse;
	
	if (ImageFile)
	{
		internal_gdk_pixbuf_unref(ImageFile);
		result = SmoothTrue;
	}

	return result;
}

/* Canvas Get ImageBuffer Size */
static SmoothBool
SmoothGDK2ImageBufferGetWidth(SmoothImageBuffer *ImageBuffer,
				SmoothInt *Width)
{
	SmoothBool result = SmoothFalse;
	
	if (ImageBuffer && Width)
	{
		*Width = gdk_pixbuf_get_width(*ImageBuffer);
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
SmoothGDK2ImageBufferGetHeight(SmoothImageBuffer *ImageBuffer,
				SmoothInt *Height)
{
	SmoothBool result = SmoothFalse;

	if (ImageBuffer && Height)
	{
		*Height = gdk_pixbuf_get_height(*ImageBuffer);
		result = SmoothTrue;
	}
	return result;
}

/* Canvas Get Image Size */
static SmoothBool
SmoothGDK2ImageGetWidth(SmoothImage *Image,
			SmoothInt *Width)
{
	SmoothBool result = SmoothFalse;
	
	if (Image && Image->Pixmap && Width)
	{
		gdk_drawable_get_size(Image->Pixmap, Width, NULL);
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
SmoothGDK2ImageGetHeight(SmoothImage *Image,
				SmoothInt *Height)
{
	SmoothBool result = SmoothFalse;
	
	if (Image && Image->Pixmap && Height)
	{
		gdk_drawable_get_size(Image->Pixmap, NULL, Height);
		result = SmoothTrue;
	}
	return result;
}

/* Canvas Store/Render */
static SmoothBool 
SmoothGDK2CanvasStore(SmoothPrivateCanvas *Canvas)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool 
SmoothGDK2CanvasRender(SmoothPrivateCanvas *Canvas,	
			SmoothBool ClosePathAndFill)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		result = SmoothTrue;
	}
	return result;
}			

/* Canvas Create/Destroy Image */
static SmoothBool
SmoothGDK2CanvasCreateImage(SmoothPrivateCanvas *Canvas,
				SmoothInt Width,
				SmoothInt Height,
				SmoothImage *Image)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && Image)
	{
		Image->Pixmap = gdk_pixmap_new(Canvas->GDKPrivateCanvas.Window, Width, Height, Canvas->GDKPrivateCanvas.Depth);
		Image->GC = gdk_gc_new(Image->Pixmap);
		
		if (Image->Pixmap && Image->GC)
			result = SmoothTrue;
	}
	return result;
}
										
static SmoothBool
SmoothGDK2CanvasCreateImageFromBuffer(SmoothPrivateCanvas *Canvas,
					SmoothImageBuffer ImageBuffer,
					SmoothImage *Image)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && ImageBuffer && Image)
	{
		if (SmoothGDK2CanvasCreateImage(Canvas,  gdk_pixbuf_get_width(ImageBuffer), gdk_pixbuf_get_height(ImageBuffer), Image))
		{
			gdk_draw_pixbuf(Image->Pixmap, Image->GC, ImageBuffer, 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NONE, 0, 0);
			result = SmoothTrue;
		}	
	}
	return result;
}

static SmoothBool
SmoothGDK2CanvasDestroyImage(SmoothPrivateCanvas *Canvas,
				SmoothImage *Image)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && Image && Image->Pixmap && Image->GC)
	{
		g_object_unref(Image->GC);
		g_object_unref(Image->Pixmap);
		result = SmoothTrue;
	}
	return result;

}

/* Canvas Tile Image/ImageBuffer */
static SmoothBool		
SmoothGDK2CanvasRenderImage(SmoothPrivateCanvas *Canvas,
				SmoothImage Image,
				SmoothInt X,
				SmoothInt Y,
				SmoothInt Width,
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && Image.Pixmap)
	{
		GdkGC *brush = smooth_internal_drawing_area_use_brush_gc(&Canvas->GDKPrivateCanvas, SmoothFalse);
		
		if (brush)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, brush, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
	        		gdk_draw_drawable(Canvas->GDKPrivateCanvas.Window, brush, Image.Pixmap, 0, 0, X, Y, Width, Height);
				SmoothGDKGCUnsetClipArea(Canvas, brush, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_brush_gc(&Canvas->GDKPrivateCanvas, brush);
		}

		result = SmoothTrue;
	}
	return result;
}													

static SmoothBool		
SmoothGDK2CanvasRenderImageBuffer(SmoothPrivateCanvas *Canvas,
					SmoothImageBuffer ImageBuffer,
					SmoothInt X,
					SmoothInt Y,
					SmoothInt Width,
					SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && ImageBuffer)
	{
		GdkGC *brush = smooth_internal_drawing_area_use_brush_gc(&Canvas->GDKPrivateCanvas, SmoothFalse);
		
		if (brush)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, brush, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				GdkRgbDither dither = GDK_RGB_DITHER_NONE;

				if (Canvas->GDKPrivateCanvas.DitherDepth > 8)
					dither = GDK_RGB_DITHER_MAX;
				else if (Canvas->GDKPrivateCanvas.DitherDepth > 0)
					dither = GDK_RGB_DITHER_NORMAL;

				gdk_draw_pixbuf(Canvas->GDKPrivateCanvas.Window, brush, ImageBuffer, 0, 0, X, Y,
							 Width, Height, dither, 0, 0);
				SmoothGDKGCUnsetClipArea(Canvas, brush, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_brush_gc(&Canvas->GDKPrivateCanvas, brush);
		}

		result = SmoothTrue;
	}
	return result;
}													

/* Canvas Draw _Point(s) */
static SmoothBool		
SmoothGDK2CanvasDrawPoint(SmoothPrivateCanvas *Canvas,
				SmoothInt X,
				SmoothInt Y)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = smooth_internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_point(Canvas->GDKPrivateCanvas.Window, pen, X, Y);
				SmoothGDKGCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
		
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool
SmoothGDK2CanvasDrawPoints(SmoothPrivateCanvas *Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{				
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = smooth_internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_points(Canvas->GDKPrivateCanvas.Window, pen, Points, NumberOfPoints);
				SmoothGDKGCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
		
		result = SmoothTrue;
	}
	return result;
}

/* Canvas Draw Line(s) */
static SmoothBool		
SmoothGDK2CanvasDrawLine(SmoothPrivateCanvas *Canvas,
				SmoothInt X1,
				SmoothInt Y1,
				SmoothInt X2,
				SmoothInt Y2)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = smooth_internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_line(Canvas->GDKPrivateCanvas.Window, pen, X1, Y1, X2, Y2);
				SmoothGDKGCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
		
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool
SmoothGDK2CanvasDrawSegment(SmoothPrivateCanvas *Canvas,
				SmoothPoint _Point1, 
				SmoothPoint _Point2)
{				
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = smooth_internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_line(Canvas->GDKPrivateCanvas.Window, pen, _Point1.x, _Point1.y, _Point2.x, _Point2.y);
				SmoothGDKGCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
			
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
SmoothGDK2CanvasDrawLines(SmoothPrivateCanvas *Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{				
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = smooth_internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_lines(Canvas->GDKPrivateCanvas.Window, pen, Points, NumberOfPoints);
				SmoothGDKGCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
		
		result = SmoothTrue;
	}
	return result;
}

/* Canvas Draw Arc(s)/Curve(s) */
static SmoothBool
SmoothGDK2CanvasDrawArc(SmoothPrivateCanvas *Canvas,
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height,
				SmoothDouble AnglePosition,
				SmoothDouble AngleLength)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = smooth_internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_arc(Canvas->GDKPrivateCanvas.Window, pen, SmoothFalse, X, Y, Width, Height,
						AnglePosition*64, AngleLength*64);
				SmoothGDKGCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}

		result = SmoothTrue;
	}
	return result;
}				

/* Canvas Frame Shaped Areas */
static SmoothBool
SmoothGDK2CanvasFrameRectangle(SmoothPrivateCanvas *Canvas,
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = smooth_internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				if ((!Canvas->GDKPrivateCanvas.Pen.UsePattern) || (!Canvas->GDKPrivateCanvas.Pen.Pattern.Pattern[0]))
				{
					SmoothInt line = 0;

					if (Canvas->GDKPrivateCanvas.Pen.UseThickness)
					{
						line = Canvas->GDKPrivateCanvas.Pen.Thickness;
					}

					line /= 2;

					gdk_draw_rectangle(Canvas->GDKPrivateCanvas.Window, pen, SmoothFalse, X + line, Y + line, Width - line - 1, Height - line - 1);
				}
				else
				{
					/* We go through all the pain below because the X rasterization
					 * rules don't really work right for dashed lines if you
					 * want continuity in segments that go between top/right
					 * and left/bottom. For instance, a top left corner
					 * with a 1-1 dash is drawn as:
					 *
					 *  X X X 
					 *  X
					 *
					 *  X
					 *
					 * This is because pixels on the top and left boundaries
					 * of polygons are drawn, but not on the bottom and right.
					 * So, if you have a line going up that turns the corner
					 * and goes right, there is a one pixel shift in the pattern.
					 *
					 * So, to fix this, we drawn the top and right in one call,
					 * then the left and bottom in another call, fixing up
					 * the dash offset for the second call ourselves to get
					 * continuity at the upper left.
					 *
					 * It's not perfect since we really should have a join at
					 * the upper left and lower right instead of two intersecting
					 * lines but that's only really apparent for no-dashes,
					 * which (for this reason) are done as one polygon and
					 * don't to through this code path.
					 */
      
					SmoothPoint points[5];
					SmoothInt line_width = 0;
					SmoothInt offset = Canvas->GDKPrivateCanvas.Pen.Pattern.Offset;
		
					if (Canvas->GDKPrivateCanvas.Pen.UseThickness)
					{
						line_width = Canvas->GDKPrivateCanvas.Pen.Thickness;
					}

					SmoothPointSetValues(&points[0], X + line_width / 2, Y + line_width / 2);
					SmoothPointSetValues(&points[1], X + Width - line_width + line_width / 2, Y + line_width / 2);
					SmoothPointSetValues(&points[2], X + Width - line_width + line_width / 2,  
									Y + Height - line_width + line_width / 2);
					SmoothPointSetValues(&points[3], X + line_width / 2, Y + Height - line_width + line_width / 2);
					SmoothPointSetValues(&points[4], X + line_width / 2, Y + line_width / 2);

					Canvas->GDKPrivateCanvas.Pen.Pattern.Offset = offset;
					Canvas->GDKPrivateCanvas.Pen.Pattern.Length = strlen(Canvas->GDKPrivateCanvas.Pen.Pattern.Pattern);
           
					SmoothCanvasSetPenPattern(Canvas, Canvas->GDKPrivateCanvas.Pen.Pattern);
					SmoothCanvasDrawLines(Canvas, points, 3);

					/* We draw this line one farther over than it is "supposed" to
					 * because of another rasterization problem ... if two 1 pixel
					 * unjoined lines meet at the lower right, there will be a missing
					 * pixel.
					 */
					{
						SmoothInt tmp;
			
						SmoothPointGetXValue(&points[2], &tmp);
						SmoothPointSetXValue(&points[2], tmp + 1);
			      		}
      		
					if (Canvas->GDKPrivateCanvas.Pen.Pattern.Pattern[0])
					{
						SmoothInt dash_pixels = 0;
						SmoothInt i;
	  
						/* Adjust the dash offset for the bottom and left so we
						 * match up at the upper left.
						 */

						for (i = 0; i < Canvas->GDKPrivateCanvas.Pen.Pattern.Length; i++)
							dash_pixels += Canvas->GDKPrivateCanvas.Pen.Pattern.Pattern[i];
     
						if (Canvas->GDKPrivateCanvas.Pen.Pattern.Length % 2 == 1)
							dash_pixels *= 2;
	  
						Canvas->GDKPrivateCanvas.Pen.Pattern.Offset = offset + dash_pixels - (Width + Height - 2 * line_width) % dash_pixels;
			
						SmoothCanvasSetPenPattern(Canvas, Canvas->GDKPrivateCanvas.Pen.Pattern);
					}
      
					SmoothCanvasDrawLines(Canvas, points + 2, 3);

					Canvas->GDKPrivateCanvas.Pen.Pattern.Offset = offset;
			
					SmoothCanvasSetPenPattern(Canvas, Canvas->GDKPrivateCanvas.Pen.Pattern);
				}
			
				SmoothGDKGCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool
SmoothGDK2CanvasFramePolygon(SmoothPrivateCanvas *Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = smooth_internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_polygon(Canvas->GDKPrivateCanvas.Window, pen, SmoothFalse, 
							Points, NumberOfPoints);
				SmoothGDKGCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
			
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool
SmoothGDK2CanvasFrameChord(SmoothPrivateCanvas *Canvas,
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height,
				SmoothDouble AnglePosition,
				SmoothDouble AngleLength)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = smooth_internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_arc(Canvas->GDKPrivateCanvas.Window, pen, SmoothFalse, X, Y, Width, Height,
						AnglePosition*64, AngleLength*64);
				/* Connect the end points..... */
				SmoothGDKGCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}

		result = SmoothTrue;
	}
	return result;
}

/* Canvas Fill Shaped Areas */
static SmoothBool
SmoothGDK2CanvasFillRectangle(SmoothPrivateCanvas *Canvas,
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *brush = smooth_internal_drawing_area_use_brush_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (brush)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, brush, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_rectangle(Canvas->GDKPrivateCanvas.Window, brush, SmoothTrue, X, Y, Width, Height);
				
				SmoothGDKGCUnsetClipArea(Canvas, brush, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_brush_gc(&Canvas->GDKPrivateCanvas, brush);
		}
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool
SmoothGDK2CanvasFillPolygon(SmoothPrivateCanvas *Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *brush = smooth_internal_drawing_area_use_brush_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (brush)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, brush, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_polygon(Canvas->GDKPrivateCanvas.Window, brush, SmoothTrue, 
							Points, NumberOfPoints);
				SmoothGDKGCUnsetClipArea(Canvas, brush, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_brush_gc(&Canvas->GDKPrivateCanvas, brush);
		}
			
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool
SmoothGDK2CanvasFillChord(SmoothPrivateCanvas *Canvas,
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height,
				SmoothDouble AnglePosition,
				SmoothDouble AngleLength)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *brush = smooth_internal_drawing_area_use_brush_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (brush)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = SmoothGDKGCSetClipArea(Canvas, brush, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_arc(Canvas->GDKPrivateCanvas.Window, brush, SmoothTrue, X, Y, Width, Height,
						AnglePosition*64, AngleLength*64);
				gdk_draw_arc(Canvas->GDKPrivateCanvas.Window, brush, SmoothFalse, X, Y, Width, Height,
						AnglePosition*64, AngleLength*64);

				SmoothGDKGCUnsetClipArea(Canvas, brush, clip_area, clip_type);
			}	
			smooth_internal_drawing_area_unuse_brush_gc(&Canvas->GDKPrivateCanvas, brush);
		}

		result = SmoothTrue;
	}
	return result;
}				

/* EVILNESS EVILNESS EVILNESS EVILNESS EVILNESS EVILNESS  */
/* Interface Internal code to set and unset Clipping mask */
/* EVILNESS EVILNESS EVILNESS EVILNESS EVILNESS EVILNESS  */
static void * 
SmoothGDKGCSetClipArea(SmoothPrivateCanvas *Canvas, GdkGC *GC, GDK2ClipType *ClipType)
{
	void * result = NULL;
 	GdkBitmap *bmp = NULL;
	GdkRegion *rgn = NULL, *tmp_rgn = NULL;

	if (Canvas && GC && ClipType)
	{
		*ClipType = GDK2_CLIP_NONE;
		
		SmoothGDKGCUnsetClipArea(Canvas, GC, NULL, GDK2_CLIP_NONE);

		if (Canvas->GDKPrivateCanvas.EngineClip.Region)
		{
			rgn = gdk_region_copy (Canvas->GDKPrivateCanvas.EngineClip.Region);
			if (Canvas->GDKPrivateCanvas.EngineClip.Rectangle)	
			{
				tmp_rgn = gdk_region_rectangle(Canvas->GDKPrivateCanvas.EngineClip.Rectangle);				
				gdk_region_intersect(rgn, tmp_rgn);
				gdk_region_destroy(tmp_rgn);
			}

			if (!Canvas->GDKPrivateCanvas.NoClip)	
			{
				tmp_rgn = gdk_region_rectangle(&Canvas->GDKPrivateCanvas.Clip);				
				gdk_region_intersect(rgn, tmp_rgn);
				gdk_region_destroy(tmp_rgn);
			}	
			
			result = rgn;
			*ClipType = GDK2_CLIP_REGION;
		}
		else
		{
			if ((Canvas->GDKPrivateCanvas.EngineClip.Rectangle || (!Canvas->GDKPrivateCanvas.NoClip)) && Canvas->GDKPrivateCanvas.EngineClip.BitmapMask)	
			{
				if (Canvas->GDKPrivateCanvas.EngineClip.Rectangle)
				{
					rgn = gdk_region_rectangle(Canvas->GDKPrivateCanvas.EngineClip.Rectangle);				

					if (!Canvas->GDKPrivateCanvas.NoClip)	
					{
						tmp_rgn = gdk_region_rectangle(&Canvas->GDKPrivateCanvas.Clip);				
						gdk_region_intersect(rgn, tmp_rgn);
						gdk_region_destroy(tmp_rgn);
					}	
				}
				else	
				{
					rgn = gdk_region_rectangle(&Canvas->GDKPrivateCanvas.Clip);				
				}
			}
			else
			{
				if (Canvas->GDKPrivateCanvas.EngineClip.Rectangle)
				{
					if (!Canvas->GDKPrivateCanvas.NoClip)	
					{
						SmoothRectangle clip;
				
						gdk_rectangle_intersect(Canvas->GDKPrivateCanvas.EngineClip.Rectangle, &Canvas->GDKPrivateCanvas.Clip, &clip);			
				
						gdk_gc_set_clip_rectangle(GC, &clip);
					}		
					else
					{
						gdk_gc_set_clip_rectangle(GC, Canvas->GDKPrivateCanvas.EngineClip.Rectangle);
					}
				}
				else
				{
					if (!Canvas->GDKPrivateCanvas.NoClip)	
					{
						gdk_gc_set_clip_rectangle(GC, &Canvas->GDKPrivateCanvas.Clip);
					}
					else
					{
						gdk_gc_set_clip_rectangle(GC, NULL);
					}
				}	
				*ClipType = GDK2_CLIP_RECTANGLE;
			}	
		}
		
		if (rgn && gdk_region_empty(rgn))
		{
			gdk_region_destroy(rgn);
			*ClipType = GDK2_CLIP_EMPTY;
		}
		else
		{
			if (Canvas->GDKPrivateCanvas.EngineClip.BitmapMask)	
			{
				if  (rgn)
				{
					GdkGC *tmp_gc = NULL;
					GdkColor color;
					SmoothInt w, h;
				
					gdk_drawable_get_size(Canvas->GDKPrivateCanvas.EngineClip.BitmapMask, &w, &h);
					
					bmp = gdk_pixmap_new(NULL, w, h, 1);		
					tmp_gc = gdk_gc_new(bmp);
	
					color.pixel = 0;
					gdk_gc_set_background(tmp_gc, &color);
					gdk_draw_rectangle(bmp, tmp_gc, SmoothTrue, 0, 0, w, h);
					
					gdk_region_offset(rgn, -Canvas->GDKPrivateCanvas.EngineClip.XOffset, -Canvas->GDKPrivateCanvas.EngineClip.YOffset);
					gdk_gc_set_clip_region (tmp_gc, rgn);
	
					color.pixel = 1;
					gdk_gc_set_background(tmp_gc, &color);
				
					gdk_draw_drawable(bmp, tmp_gc, Canvas->GDKPrivateCanvas.EngineClip.BitmapMask, 0, 0, 0, 0, w, h);
				
					g_object_unref(tmp_gc);
					gdk_region_destroy(rgn);
					rgn = NULL;
				} 
				else
				{
					bmp = Canvas->GDKPrivateCanvas.EngineClip.BitmapMask;
					g_object_ref(bmp);
				}

				*ClipType = GDK2_CLIP_BITMAP;
				gdk_gc_set_clip_mask(GC, bmp);
				gdk_gc_set_clip_origin(GC, Canvas->GDKPrivateCanvas.EngineClip.XOffset, Canvas->GDKPrivateCanvas.EngineClip.YOffset);
				
				result = bmp;
			}	
		}	

		if (rgn)
		{
			gdk_gc_set_clip_region(GC, rgn);
		}		
		
	}	
	
	return result;
}

static void
SmoothGDKGCUnsetClipArea(SmoothPrivateCanvas *Canvas, GdkGC *GC, void *ClipArea, GDK2ClipType ClipType)
{
	if (Canvas && GC)
	{
		switch (ClipType)
		{
		
			case GDK2_CLIP_REGION:
				gdk_region_destroy((GdkRegion*)(ClipArea));

			break;		

			case GDK2_CLIP_BITMAP:
				g_object_unref(GDK_DRAWABLE(ClipArea));
			break;		

			case GDK2_CLIP_RECTANGLE:
			case GDK2_CLIP_EMPTY:
			case GDK2_CLIP_NONE:
			default:

			break;		
		}	

		gdk_gc_set_clip_region(GC, NULL);
		gdk_gc_set_clip_mask(GC, NULL);
		gdk_gc_set_clip_rectangle(GC, NULL);
	}	
}

void 
SmoothGDKInitializeCanvas(SmoothCanvas *Canvas,
				GtkStyle * Style,
				GdkWindow * _Window,
				GdkRectangle * Area,
				GdkRegion * _Region,
				GdkBitmap *ClipMask,
				SmoothInt Width,
				SmoothInt Height,				
				SmoothInt XOffset,
				SmoothInt YOffset,
				SmoothColorCube *ColorCube)
{				
	SmoothPrivateCanvas *PrivateCanvas;
	
	GdkDrawable *real_drawable;	
	SmoothInt x_off=0, y_off=0;	

	PrivateCanvas = g_new0(SmoothPrivateCanvas, 1);
      	        
	if(GDK_IS_WINDOW(_Window))
		gdk_window_get_internal_paint_info (_Window, &real_drawable, &x_off, &y_off);
	else
		real_drawable = _Window;

	PrivateCanvas->GDKPrivateCanvas.Window = _Window;
	PrivateCanvas->GDKPrivateCanvas.Drawable.Window = real_drawable;
	PrivateCanvas->GDKPrivateCanvas.Drawable.XOffset = x_off;
	PrivateCanvas->GDKPrivateCanvas.Drawable.YOffset = y_off;

	PrivateCanvas->GDKPrivateCanvas.Colormap = Style->colormap;
	PrivateCanvas->GDKPrivateCanvas.Depth = Style->depth;
	PrivateCanvas->GDKPrivateCanvas.DitherDepth = 8;

	PrivateCanvas->GDKPrivateCanvas.EngineClip.BitmapMask = ClipMask;
	PrivateCanvas->GDKPrivateCanvas.EngineClip.Region = _Region;
	PrivateCanvas->GDKPrivateCanvas.EngineClip.Rectangle = Area;
	PrivateCanvas->GDKPrivateCanvas.EngineClip.XOffset = XOffset;
	PrivateCanvas->GDKPrivateCanvas.EngineClip.YOffset = YOffset;
   
	PrivateCanvas->GDKPrivateCanvas.NoClip = TRUE;
	
      	*Canvas = PrivateCanvas;

	SmoothGDKInitializeColorCube(*Canvas, Style, ColorCube);
}

void 
SmoothGDKFinalizeCanvas(SmoothCanvas *Canvas)
{
	SmoothPrivateCanvas *PrivateCanvas = *Canvas;
	
	g_free(PrivateCanvas);
	*Canvas = NULL;
}

/* Initialize smoothDrawingInterface */
static SmoothBool 
_SmoothDrawingInterfaceInitialize(DrawingInterfaceClass *_SmoothDrawingInterface)
{
	SmoothGDKDrawingInterfaceInitialize(_SmoothDrawingInterface);

	/* Image/Buffer By File name (For caching purposes) */
	_SmoothDrawingInterface->ImageBufferLoadByFile = (void *) SmoothGDK2ImageBufferLoadByFile;
	_SmoothDrawingInterface->ImageBufferUnLoadByFile = (void *) SmoothGDK2ImageBufferUnLoadByFile;

	/* Canvas Get ImageBuffer Size */
	_SmoothDrawingInterface->ImageBufferGetWidth = (void *) SmoothGDK2ImageBufferGetWidth;
	_SmoothDrawingInterface->ImageBufferGetHeight = (void *) SmoothGDK2ImageBufferGetHeight;

	/* Canvas Get Image Size */
	_SmoothDrawingInterface->ImageGetWidth = (void *) SmoothGDK2ImageGetWidth;
	_SmoothDrawingInterface->ImageGetHeight = (void *) SmoothGDK2ImageGetHeight;

	/* Canvas Store/Render */
	_SmoothDrawingInterface->CanvasStore =  (void *) SmoothGDK2CanvasStore;
	_SmoothDrawingInterface->CanvasRender =  (void *) SmoothGDK2CanvasRender;

	/* Canvas Create Image */
	_SmoothDrawingInterface->CanvasCreateImage = (void *) SmoothGDK2CanvasCreateImage;
	_SmoothDrawingInterface->CanvasCreateImageFromBuffer = (void *) SmoothGDK2CanvasCreateImageFromBuffer;
	_SmoothDrawingInterface->CanvasDestroyImage = (void *) SmoothGDK2CanvasDestroyImage;

	/* Canvas Tile Image/ImageBuffer */
	_SmoothDrawingInterface->CanvasRenderImage = (void *) SmoothGDK2CanvasRenderImage;						
	_SmoothDrawingInterface->CanvasRenderImageBuffer = (void *) SmoothGDK2CanvasRenderImageBuffer;						


	/* Canvas Render Gradient */
	#ifdef SMOOTHGRADIENT
	_SmoothDrawingInterface->CanvasRenderGradient = (void *)SmoothGDK2CanvasRenderGradient;
	#endif
	
	/* Canvas Draw _Point(s) */
	_SmoothDrawingInterface->CanvasDrawPoint = (void *) SmoothGDK2CanvasDrawPoint;
	_SmoothDrawingInterface->CanvasDrawPoints = (void *) SmoothGDK2CanvasDrawPoints;

	/* Canvas Draw Line(s) */
	_SmoothDrawingInterface->CanvasDrawLine = (void *) SmoothGDK2CanvasDrawLine;
	_SmoothDrawingInterface->CanvasDrawSegment = (void *) (void *) SmoothGDK2CanvasDrawSegment;
	_SmoothDrawingInterface->CanvasDrawLines = (void *) SmoothGDK2CanvasDrawLines;
/*	_SmoothDrawingInterface->CanvasDrawSegments = (void *) NULL;*/
	
	/* Canvas Draw Arc(s)/Curve(s) */
	_SmoothDrawingInterface->CanvasDrawArc = (void *) SmoothGDK2CanvasDrawArc;

	/* Canvas Frame Shaped Areas */
	_SmoothDrawingInterface->CanvasFrameRectangle = (void *) SmoothGDK2CanvasFrameRectangle;
	_SmoothDrawingInterface->CanvasFramePolygon = (void *) SmoothGDK2CanvasFramePolygon;
	_SmoothDrawingInterface->CanvasFrameChord = (void *) SmoothGDK2CanvasFrameChord;

	/* Canvas Fill Shaped Areas */
	_SmoothDrawingInterface->CanvasFillRectangle = (void *) SmoothGDK2CanvasFillRectangle;
	_SmoothDrawingInterface->CanvasFillPolygon = (void *) SmoothGDK2CanvasFillPolygon;
	_SmoothDrawingInterface->CanvasFillChord = (void *) SmoothGDK2CanvasFillChord;
	
	return SmoothTrue;
}

static SmoothBool _SmoothDrawingInterfaceFinalize(DrawingInterfaceClass *_SmoothDrawingInterface)
{
	smooth_cleanup_gdk_pixbuf_cache(SmoothTrue);

	SmoothGDKDrawingInterfaceFinalize(_SmoothDrawingInterface);
	
	return SmoothTrue;
}

/* Main Functions for Engine to Setup and Finalize - Each interface must provide these */
void SmoothDrawingInterfaceInitialize()
{
	SmoothDrawingInterfaceSetup(_SmoothDrawingInterfaceInitialize, _SmoothDrawingInterfaceFinalize);
}

void SmoothDrawingInterfaceFinalize()
{
	SmoothDrawingInterfaceCleanup();
}

#ifdef SMOOTHGRADIENT
#include "src/gdk2_gradient_routines.c"
#endif
