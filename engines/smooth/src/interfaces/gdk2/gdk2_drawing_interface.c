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

GdkGC *internal_drawing_area_use_pen_gc(SmoothGDKPrivateCanvas *Canvas, SmoothBool RequireValidColor);
void internal_drawing_area_unuse_pen_gc(SmoothGDKPrivateCanvas *Canvas, GdkGC * GC);

GdkGC * internal_drawing_area_use_brush_gc(SmoothGDKPrivateCanvas *Canvas, SmoothBool RequireValidColor);
void internal_drawing_area_unuse_brush_gc(SmoothGDKPrivateCanvas *Canvas, GdkGC * GC);

/* some basic math defines */
#define SQR(x) x*x
#ifdef USE_CAIRO
#define cairo_cache_count 8
static SmoothBool cairo_cache_used[cairo_cache_count];
static cairo_t* cairo_cache[cairo_cache_count];

static void
CairoDrawArc(cairo_t *CairoHandle,
			SmoothInt X, 
			SmoothInt Y, 
			SmoothInt Width, 
			SmoothInt Height,
			SmoothDouble AnglePosition,
			SmoothDouble AngleLength);
#define USE_BEZIER
#endif

#ifdef USE_BEZIER
typedef struct
{
  SmoothDouble x;
  SmoothDouble y;
} SmoothFloatPoint;

typedef struct
{
  SmoothFloatPoint p0;
  SmoothFloatPoint p1;
  SmoothFloatPoint p2;
  SmoothFloatPoint p3;
} SmoothBezier;

static void 
smooth_bezier_scale(SmoothBezier *Bezier, 
			SmoothDouble ScaleX, 
			SmoothDouble ScaleY);

static void 
smooth_bezier_rotate(SmoothBezier *Bezier,  
			SmoothDouble Theta);

static void 
smooth_bezier_translate(SmoothBezier *Bezier, 
			SmoothDouble Centerx, 
			SmoothDouble Centery);

static void 
smooth_arc_bezier(SmoothDouble Size, 
			SmoothDouble AnglePosition,
			SmoothDouble AngleLength,
			SmoothBezier * Bezier);

static void
SmoothDrawArc(SmoothPrivateCanvas *Canvas,
			SmoothInt X, 
			SmoothInt Y, 
			SmoothInt Width, 
			SmoothInt Height,
			SmoothDouble AnglePosition,
			SmoothDouble AngleLength);
#endif

static SmoothBool _DrawingInterfaceInitialize(DrawingInterfaceClass *_DrawingInterface);
static SmoothBool _DrawingInterfaceFinalize(DrawingInterfaceClass *_DrawingInterface);

/* Image/Buffer By File name (For caching purposes) */
static void cleanup_gdk_pixbuf_cache(SmoothBool force);/* not part of class */

static SmoothBool GDK2ImageBufferLoadByFile(SmoothString ImageFile, SmoothImageBuffer *ImageBuffer);
static SmoothBool GDK2ImageBufferUnLoadByFile(SmoothString ImageFile);

/* Canvas Get ImageBuffer Size */
static SmoothBool GDK2ImageBufferGetWidth(SmoothImageBuffer *ImageBuffer,
						SmoothInt *Width);
static SmoothBool GDK2ImageBufferGetHeight(SmoothImageBuffer *ImageBuffer,
						SmoothInt *Height);

/* Canvas Get Image Size */
static SmoothBool GDK2ImageGetWidth(SmoothImage *Image,
						SmoothInt *Width);
static SmoothBool GDK2ImageGetHeight(SmoothImage *Image,
						SmoothInt *Height);

/* Canvas Store/Render */
static SmoothBool GDK2CanvasStore(SmoothPrivateCanvas *Canvas);
static SmoothBool GDK2CanvasRender(SmoothPrivateCanvas *Canvas,	
					SmoothBool ClosePathAndFill);

/* Canvas Create Image */
static SmoothBool GDK2CanvasCreateImage(SmoothPrivateCanvas *Canvas,
						SmoothInt Width,
						SmoothInt Height,
						SmoothImage *Image);
						
static SmoothBool GDK2CanvasCreateImageFromBuffer(SmoothPrivateCanvas *Canvas,
							SmoothImageBuffer ImageBuffer,
							SmoothImage *Image);
							
static SmoothBool GDK2CanvasDestroyImage(SmoothPrivateCanvas *Canvas,
						SmoothImage *Image);

/* Canvas Tile Image/ImageBuffer */
static SmoothBool GDK2CanvasRenderImage(SmoothPrivateCanvas *Canvas,
						SmoothImage Image,
						SmoothInt X,
						SmoothInt Y,
						SmoothInt Width,
						SmoothInt Height);
						
static SmoothBool GDK2CanvasRenderImageBuffer(SmoothPrivateCanvas *Canvas,
							SmoothImageBuffer ImageBuffer,
							SmoothInt X,
							SmoothInt Y,
							SmoothInt Width,
							SmoothInt Height);

/* Canvas Draw _Point(s) */
static SmoothBool GDK2CanvasDrawPoint(SmoothPrivateCanvas *Canvas,
						SmoothInt X,
						SmoothInt Y);
						
static SmoothBool GDK2CanvasDrawPoints(SmoothPrivateCanvas *Canvas,
						SmoothPoint *Points, 
						SmoothInt NumberOfPoints);

/* Canvas Draw Line(s) */
static SmoothBool GDK2CanvasDrawLine(SmoothPrivateCanvas *Canvas,
						SmoothInt X1,
						SmoothInt Y1,
						SmoothInt X2,
						SmoothInt Y2);
						
static SmoothBool GDK2CanvasDrawSegment(SmoothPrivateCanvas *Canvas,
						SmoothPoint _Point1, 
						SmoothPoint _Point2);

static SmoothBool GDK2CanvasDrawLines(SmoothPrivateCanvas *Canvas,
						SmoothPoint *Points, 
						SmoothInt NumberOfPoints);

/* Canvas Draw Arc(s)/Curve(s) */
static SmoothBool GDK2CanvasDrawArc(SmoothPrivateCanvas *Canvas,
					SmoothInt X, 
					SmoothInt Y, 
					SmoothInt Width, 
					SmoothInt Height,
					SmoothDouble AnglePosition,
					SmoothDouble AngleLength);

/* Canvas Frame Shaped Areas */
static SmoothBool GDK2CanvasFrameRectangle(SmoothPrivateCanvas *Canvas,
						SmoothInt X, 
						SmoothInt Y, 
						SmoothInt Width, 
						SmoothInt Height);
						
static SmoothBool GDK2CanvasFramePolygon(SmoothPrivateCanvas *Canvas,
						SmoothPoint *Points, 
						SmoothInt NumberOfPoints);

static SmoothBool GDK2CanvasFrameChord(SmoothPrivateCanvas *Canvas,
						SmoothInt X, 
						SmoothInt Y, 
						SmoothInt Width, 
						SmoothInt Height,
						SmoothDouble AnglePosition,
						SmoothDouble AngleLength);

/* Canvas Fill Shaped Areas */
static SmoothBool GDK2CanvasFillRectangle(SmoothPrivateCanvas *Canvas,
						SmoothInt X, 
						SmoothInt Y, 
						SmoothInt Width, 
						SmoothInt Height);
						
static SmoothBool GDK2CanvasFillPolygon(SmoothPrivateCanvas *Canvas,
						SmoothPoint *Points, 
						SmoothInt NumberOfPoints);

static SmoothBool GDK2CanvasFillChord(SmoothPrivateCanvas *Canvas,
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

static void *  GCSetClipArea(SmoothPrivateCanvas *Canvas, GdkGC *GC, GDK2ClipType *clip_type);
static void GCUnsetClipArea(SmoothPrivateCanvas *Canvas, GdkGC *GC, void *ClipArea, GDK2ClipType clip_type);

/* Internal Image Buffers */
static void
internel_image_buffer_free_pixels (guchar *pixels, gpointer data)
{
	g_free (pixels);
}

static SmoothImageBuffer
internal_image_buffer_new (SmoothInt width, SmoothInt height)
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
                                   internel_image_buffer_free_pixels, NULL);
}
/*
static SmoothImageBuffer
internal_canvas_get_image_buffer (SmoothPrivateCanvas *Canvas, SmoothInt X, SmoothInt Y, SmoothInt Width, SmoothInt Height)
{
	SmoothImageBuffer buffer = NULL;
			
	buffer = gdk_pixbuf_get_from_drawable(NULL, pixmap, Canvas->GDKPrivateCanvas.Colormap, 
						0, 0, 
						0, 0,
						Width, Height);
	
	return buffer;
}	
*/
#ifdef USE_CAIRO
	#ifndef USE_XLIB
		static SmoothImageBuffer
		internal_create_cairo_image_buffer (SmoothPrivateCanvas *Canvas, SmoothInt Width, SmoothInt Height)
		{
/*			SmoothImageBuffer buffer; */
			SmoothUChar *pixels;
			SmoothUChar *ptr;
			int rowstride;

			rowstride = Width * 4;
			
/*			buffer = internal_image_buffer_new (Width, Height);*/
			pixels = g_try_malloc (Height * rowstride);
			if (pixels == NULL)
				return NULL;

    			memset(pixels, 0, Height * rowstride);			
    
/*			gdk_pixbuf_get_from_drawable(buffer, Canvas->GDKPrivateCanvas.Window, Canvas->GDKPrivateCanvas.Colormap, 
							Canvas->CairoHandle.XOffset, Canvas->CairoHandle.YOffset, 
							0, 0, 
							Width, Height);

			pixels = gdk_pixbuf_get_pixels (buffer);
			ptr = pixels;
			rowstride = gdk_pixbuf_get_rowstride (buffer);*/

			cairo_set_target_image (Canvas->CairoHandle.CairoObject, pixels, CAIRO_FORMAT_ARGB32, Width, Height, rowstride);
			cairo_translate(Canvas->CairoHandle.CairoObject,Width/2,Height/2);
			
			Canvas->CairoHandle.Width = Width;
			Canvas->CairoHandle.Height = Height;

			Canvas->CairoHandle.Buffer = pixels;/* buffer */;
		}	

		static void
		internal_destroy_cairo_image_buffer (SmoothPrivateCanvas *Canvas)
		{
			SmoothInt x,y;
			SmoothImageBuffer buffer;
			SmoothUChar *pixels;
			SmoothUChar *ptr;
			int rowstride;
			
			buffer = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, Canvas->CairoHandle.Width, Canvas->CairoHandle.Height);

			pixels = gdk_pixbuf_get_pixels (buffer);
			rowstride = Canvas->CairoHandle.Width*4;
			
			for (y = 0; y < Canvas->CairoHandle.Height; y++)
			{
				for (x = 0; x < rowstride; x+=4)
				{
					pixels[(y*rowstride) + x + 0] = Canvas->CairoHandle.Buffer[(y*rowstride) + x + 2];
					pixels[(y*rowstride) + x + 1] = Canvas->CairoHandle.Buffer[(y*rowstride) + x + 1];
					pixels[(y*rowstride) + x + 2] = Canvas->CairoHandle.Buffer[(y*rowstride) + x + 0];
					pixels[(y*rowstride) + x + 3] = Canvas->CairoHandle.Buffer[(y*rowstride) + x + 3];
				}
			}	
			SmoothCanvasRenderImageBuffer(Canvas, buffer, 
							Canvas->CairoHandle.XOffset, 
							Canvas->CairoHandle.YOffset, 

							gdk_pixbuf_get_width(buffer), 
							gdk_pixbuf_get_height(buffer));

			g_object_unref(buffer);

			g_free(Canvas->CairoHandle.Buffer);
			Canvas->CairoHandle.Buffer = NULL;
		}	
	#endif
#endif

/* the following are routines for caching images by file name */
static GHashTable* pixbuf_cache = NULL;

typedef struct
{
	GString 	*FileName;
	GdkPixbuf	*ImageBuffer;
	SmoothInt		ref_count;
} GdkCachedImageBuffer;

static void
free_pixbuf_cache(GdkCachedImageBuffer *cache)
{
	g_object_unref(cache->ImageBuffer);
	g_string_free(cache->FileName, SmoothTrue);
	g_free(cache);
}

static GdkCachedImageBuffer *
new_pixbuf_cache(SmoothString FileName)
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
force_cleanup_pixbuf_cache(gpointer key, gpointer value, gpointer user_data)
{
	if (value) 
	{
		free_pixbuf_cache((GdkCachedImageBuffer *)value);
	}	
	return SmoothTrue;
}

static void
cleanup_gdk_pixbuf_cache(SmoothBool force)
{
	if (pixbuf_cache) 
	{
		if (force)
		{
			g_hash_table_foreach_remove(pixbuf_cache, force_cleanup_pixbuf_cache, NULL);
		}
		
		if (g_hash_table_size(pixbuf_cache)<=0) 
		{
			g_hash_table_destroy(pixbuf_cache);
			pixbuf_cache = NULL;
		}  
	}   
}

static GdkPixbuf *
internal_gdk_pixbuf_get_by_name(SmoothString file_name)
{
	GdkCachedImageBuffer *cache=NULL;
	GdkPixbuf *result=NULL;
   
	if (!pixbuf_cache)
	{
		pixbuf_cache=g_hash_table_new(g_str_hash, g_str_equal);
	}
	
	cache = g_hash_table_lookup(pixbuf_cache, file_name->str);
   
	if (!cache) 
	{
		cache = new_pixbuf_cache(file_name);

		if (cache) 
			g_hash_table_insert(pixbuf_cache, cache->FileName->str, cache);
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

	if (pixbuf_cache) {
		cache = g_hash_table_lookup(pixbuf_cache, file_name->str);

		if (cache) {
			cache->ref_count--;
   
			if (cache->ref_count == 0) {
				g_hash_table_remove(pixbuf_cache, file_name->str);
				free_pixbuf_cache(cache);
			}   
		}
		cleanup_gdk_pixbuf_cache(SmoothFalse);  
	}   
}

static SmoothBool 
GDK2ImageBufferLoadByFile(SmoothString ImageFile,
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
GDK2ImageBufferUnLoadByFile(SmoothString ImageFile)
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
GDK2ImageBufferGetWidth(SmoothImageBuffer *ImageBuffer,
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
GDK2ImageBufferGetHeight(SmoothImageBuffer *ImageBuffer,
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
GDK2ImageGetWidth(SmoothImage *Image,
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
GDK2ImageGetHeight(SmoothImage *Image,
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
GDK2CanvasStore(SmoothPrivateCanvas *Canvas)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		#ifdef USE_CAIRO
			if (!Canvas->CairoHandle.InStore)
			{
				cairo_save(Canvas->CairoHandle.CairoObject);
				cairo_new_path(Canvas->CairoHandle.CairoObject);

				cairo_set_line_width(Canvas->CairoHandle.CairoObject, 1.0);
				cairo_set_line_join (Canvas->CairoHandle.CairoObject, CAIRO_LINE_JOIN_BEVEL);
				cairo_set_line_cap (Canvas->CairoHandle.CairoObject, CAIRO_LINE_CAP_SQUARE);

				Canvas->CairoHandle.InStore = SmoothTrue;
				result = SmoothTrue;
			}				
		#else
			result = SmoothTrue;
		#endif
	}
	return result;
}

static SmoothBool 
GDK2CanvasRender(SmoothPrivateCanvas *Canvas,	
			SmoothBool ClosePathAndFill)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		#ifdef USE_CAIRO
			if (Canvas->CairoHandle.InStore)
			{
				SmoothDouble r,g,b, a;	

				if (ClosePathAndFill)
				{
					SmoothColorGetAlphaValue(&Canvas->GDKPrivateCanvas.Brush.Color, &a);

					cairo_set_alpha(Canvas->CairoHandle.CairoObject, 1.0);	   

					SmoothColorGetRedValue(&Canvas->GDKPrivateCanvas.Brush.Color, &r);
					SmoothColorGetGreenValue(&Canvas->GDKPrivateCanvas.Brush.Color, &g);
					SmoothColorGetBlueValue(&Canvas->GDKPrivateCanvas.Brush.Color, &b);
						
					cairo_set_rgb_color(Canvas->CairoHandle.CairoObject, r, g, b);	   

					cairo_close_path(Canvas->CairoHandle.CairoObject);
					cairo_fill(Canvas->CairoHandle.CairoObject);
				}
				else	
				{
					SmoothColorGetAlphaValue(&Canvas->GDKPrivateCanvas.Pen.Color, &a);

					cairo_set_alpha(Canvas->CairoHandle.CairoObject, 1.0);	   

					SmoothColorGetRedValue(&Canvas->GDKPrivateCanvas.Pen.Color, &r);
					SmoothColorGetGreenValue(&Canvas->GDKPrivateCanvas.Pen.Color, &g);
					SmoothColorGetBlueValue(&Canvas->GDKPrivateCanvas.Pen.Color, &b);
						
					cairo_set_rgb_color(Canvas->CairoHandle.CairoObject, r, g, b);	   

					if (Canvas->GDKPrivateCanvas.Pen.UseThickness)
					{
						cairo_set_line_width(Canvas->CairoHandle.CairoObject, Canvas->GDKPrivateCanvas.Pen.Thickness);
					}
					else
					{
						cairo_set_line_width(Canvas->CairoHandle.CairoObject, 1);
					}

					cairo_stroke(Canvas->CairoHandle.CairoObject);
				}
				cairo_restore(Canvas->CairoHandle.CairoObject);

				Canvas->CairoHandle.InStore = SmoothFalse;

				result = SmoothTrue;
			}	
		#else
			result = SmoothTrue;
		#endif
	}
	return result;
}			

/* Canvas Create/Destroy Image */
static SmoothBool
GDK2CanvasCreateImage(SmoothPrivateCanvas *Canvas,
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
GDK2CanvasCreateImageFromBuffer(SmoothPrivateCanvas *Canvas,
					SmoothImageBuffer ImageBuffer,
					SmoothImage *Image)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && ImageBuffer && Image)
	{
		if (GDK2CanvasCreateImage(Canvas,  gdk_pixbuf_get_width(ImageBuffer), gdk_pixbuf_get_height(ImageBuffer), Image))
		{
			gdk_draw_pixbuf(Image->Pixmap, Image->GC, ImageBuffer, 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NONE, 0, 0);
			result = SmoothTrue;
		}	
	}
	return result;
}

static SmoothBool
GDK2CanvasDestroyImage(SmoothPrivateCanvas *Canvas,
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
GDK2CanvasRenderImage(SmoothPrivateCanvas *Canvas,
				SmoothImage Image,
				SmoothInt X,
				SmoothInt Y,
				SmoothInt Width,
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && Image.Pixmap)
	{
		GdkGC *brush = internal_drawing_area_use_brush_gc(&Canvas->GDKPrivateCanvas, SmoothFalse);
		
		if (brush)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, brush, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
	        		gdk_draw_drawable(Canvas->GDKPrivateCanvas.Window, brush, Image.Pixmap, 0, 0, X, Y, Width, Height);
				GCUnsetClipArea(Canvas, brush, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_brush_gc(&Canvas->GDKPrivateCanvas, brush);
		}

		result = SmoothTrue;
	}
	return result;
}													

static SmoothBool		
GDK2CanvasRenderImageBuffer(SmoothPrivateCanvas *Canvas,
					SmoothImageBuffer ImageBuffer,
					SmoothInt X,
					SmoothInt Y,
					SmoothInt Width,
					SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas && ImageBuffer)
	{
		GdkGC *brush = internal_drawing_area_use_brush_gc(&Canvas->GDKPrivateCanvas, SmoothFalse);
		
		if (brush)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, brush, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				GdkRgbDither dither = GDK_RGB_DITHER_NONE;

				if (Canvas->GDKPrivateCanvas.DitherDepth > 8)
					dither = GDK_RGB_DITHER_MAX;
				else if (Canvas->GDKPrivateCanvas.DitherDepth > 0)
					dither = GDK_RGB_DITHER_NORMAL;

				gdk_draw_pixbuf(Canvas->GDKPrivateCanvas.Window, brush, ImageBuffer, 0, 0, X, Y,
							 Width, Height, dither, 0, 0);
				GCUnsetClipArea(Canvas, brush, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_brush_gc(&Canvas->GDKPrivateCanvas, brush);
		}

		result = SmoothTrue;
	}
	return result;
}													

/* Canvas Draw _Point(s) */
static SmoothBool		
GDK2CanvasDrawPoint(SmoothPrivateCanvas *Canvas,
				SmoothInt X,
				SmoothInt Y)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_point(Canvas->GDKPrivateCanvas.Window, pen, X, Y);
				GCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
		
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool
GDK2CanvasDrawPoints(SmoothPrivateCanvas *Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{				
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_points(Canvas->GDKPrivateCanvas.Window, pen, Points, NumberOfPoints);
				GCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
		
		result = SmoothTrue;
	}
	return result;
}

/* Canvas Draw Line(s) */
static SmoothBool		
GDK2CanvasDrawLine(SmoothPrivateCanvas *Canvas,
				SmoothInt X1,
				SmoothInt Y1,
				SmoothInt X2,
				SmoothInt Y2)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				#ifdef BROKEN_CAIRO_FUNCTIONS
					SmoothBool do_render=!Canvas->CairoHandle.InStore;
						
					if (do_render)
					{
						GDK2CanvasStore(Canvas);
					}
					
					if (X1 == X2)
					{
						cairo_move_to(Canvas->CairoHandle.CairoObject, X1 + 0.5, Y1 + ((Y2 - Y1)>2)*0.5);
						cairo_line_to(Canvas->CairoHandle.CairoObject, X2 + 0.5, Y2 + 0.5);					
					}
					else if (Y1 == Y2)
					{
						cairo_move_to(Canvas->CairoHandle.CairoObject, X1 + ((X2 - X1)>2)*0.5, Y1 + 0.5);
						cairo_line_to(Canvas->CairoHandle.CairoObject, X2 + 0.5, Y2 + 0.5);					
					}
					else
					{
						cairo_move_to(Canvas->CairoHandle.CairoObject, X1, Y1);
						cairo_line_to(Canvas->CairoHandle.CairoObject, X2, Y2);					
					}
					
					if (do_render)
						GDK2CanvasRender(Canvas, False);
				#else
					gdk_draw_line(Canvas->GDKPrivateCanvas.Window, pen, X1, Y1, X2, Y2);
				#endif
				GCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
		
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool
GDK2CanvasDrawSegment(SmoothPrivateCanvas *Canvas,
				SmoothPoint _Point1, 
				SmoothPoint _Point2)
{				
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_line(Canvas->GDKPrivateCanvas.Window, pen, _Point1.x, _Point1.y, _Point2.x, _Point2.y);
				GCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
			
		result = SmoothTrue;
	}
	return result;
}

static SmoothBool
GDK2CanvasDrawLines(SmoothPrivateCanvas *Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{				
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_lines(Canvas->GDKPrivateCanvas.Window, pen, Points, NumberOfPoints);
				GCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
		
		result = SmoothTrue;
	}
	return result;
}
/*
SmoothBool 
SmoothCanvasDrawSegments(SmoothPrivateCanvas *Canvas,
					SmoothPoint Points, 
					SmoothPoint NumberOfPoints)
{				
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasDrawSegments)
	{
		result = drawingInterface.CanvasDrawSegments(Canvas, Points, NumberOfPoints);
	}
	return result;
}*/

/* Canvas Draw Arc(s)/Curve(s) */
static SmoothBool
GDK2CanvasDrawArc(SmoothPrivateCanvas *Canvas,
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
		GdkGC *pen = internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				#ifdef USE_CAIRO
  					SmoothBool do_render=!Canvas->CairoHandle.InStore;

					if (do_render)
						GDK2CanvasStore(Canvas);

					CairoDrawArc(Canvas->CairoHandle.CairoObject, X, Y, Width, Height, AnglePosition, AngleLength);
					
					if (do_render)
						GDK2CanvasRender(Canvas, False);

				#else
					#ifdef USE_BEZIER
					SmoothDrawArc(Canvas, X, Y, Width, Height, AnglePosition, AngleLength);
					#else
					gdk_draw_arc(Canvas->GDKPrivateCanvas.Window, pen, SmoothFalse, X, Y, Width, Height,
							AnglePosition*64, AngleLength*64);
					#endif
				#endif
				GCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}

		result = SmoothTrue;
	}
	return result;
}				

/* Canvas Frame Shaped Areas */
static SmoothBool
GDK2CanvasFrameRectangle(SmoothPrivateCanvas *Canvas,
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				#ifdef SLOW_CAIRO_FUNCTIONS
				{
					SmoothBool do_render=!Canvas->CairoHandle.InStore;

					if (do_render)
						GDK2CanvasStore(Canvas);

					cairo_rectangle(Canvas->CairoHandle.CairoObject, X, Y, Width, Height);

					if (do_render)
						GDK2CanvasRender(Canvas, SmoothFalse);
				}		
				#else
				if ((!Canvas->GDKPrivateCanvas.Pen.UsePattern) || (!Canvas->GDKPrivateCanvas.Pen.Pattern.Pattern[0]))
				{
					gdk_draw_rectangle(Canvas->GDKPrivateCanvas.Window, pen, SmoothFalse, X, Y, Width - 1, Height - 1);
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
				#endif
			
				GCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool
GDK2CanvasFramePolygon(SmoothPrivateCanvas *Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *pen = internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_polygon(Canvas->GDKPrivateCanvas.Window, pen, SmoothFalse, 
							Points, NumberOfPoints);
				GCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
			
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool
GDK2CanvasFrameChord(SmoothPrivateCanvas *Canvas,
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
		GdkGC *pen = internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				#ifdef USE_CAIRO
  					SmoothBool do_render=!Canvas->CairoHandle.InStore;

					if (do_render)
						GDK2CanvasStore(Canvas);

					CairoDrawArc(Canvas->CairoHandle.CairoObject, X, Y, Width, Height, AnglePosition, AngleLength);
					/* Connect the end points..... */
					
					if (do_render)
						GDK2CanvasRender(Canvas, False);

				#else
					gdk_draw_arc(Canvas->GDKPrivateCanvas.Window, pen, SmoothFalse, X, Y, Width, Height,
							AnglePosition*64, AngleLength*64);
					/* Connect the end points..... */
				#endif
				GCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}

		result = SmoothTrue;
	}
	return result;
}

/* Canvas Fill Shaped Areas */
static SmoothBool
GDK2CanvasFillRectangle(SmoothPrivateCanvas *Canvas,
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *brush = internal_drawing_area_use_brush_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (brush)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, brush, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				#ifdef USE_XRENDER
					XRenderColor rendercolor;
				
					rendercolor.red = Canvas->GDKPrivateCanvas.Brush.Color.RGB.red;
					rendercolor.green = Canvas->GDKPrivateCanvas.Brush.Color.RGB.green;
					rendercolor.blue = Canvas->GDKPrivateCanvas.Brush.Color.RGB.blue;
					rendercolor.alpha = Canvas->GDKPrivateCanvas.Brush.Color.Alpha;
					XRenderFillRectangle (Canvas->X11Handle.Display, PictOpSrc, Canvas->Picture, &rendercolor, 
								X - Canvas->X11Handle.XOffset, Y - Canvas->X11Handle.YOffset, Width, Height);
				#else
					#ifdef SLOW_CAIRO_FUNCTIONS
					{
						SmoothBool do_render=!Canvas->CairoHandle.InStore;

						if (do_render)
							GDK2CanvasStore(Canvas);

						cairo_rectangle(Canvas->CairoHandle.CairoObject, X, Y, Width, Height);

						if (do_render)
							GDK2CanvasRender(Canvas, SmoothTrue);
					}		
					#else
						gdk_draw_rectangle(Canvas->GDKPrivateCanvas.Window, brush, SmoothTrue, X, Y, Width, Height);
					#endif
				#endif
				
				GCUnsetClipArea(Canvas, brush, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_brush_gc(&Canvas->GDKPrivateCanvas, brush);
		}
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool
GDK2CanvasFillPolygon(SmoothPrivateCanvas *Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{
	SmoothBool result = SmoothFalse;
	
	if (Canvas)
	{
		GdkGC *brush = internal_drawing_area_use_brush_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (brush)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, brush, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_draw_polygon(Canvas->GDKPrivateCanvas.Window, brush, SmoothTrue, 
							Points, NumberOfPoints);
				GCUnsetClipArea(Canvas, brush, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_brush_gc(&Canvas->GDKPrivateCanvas, brush);
		}
			
		result = SmoothTrue;
	}
	return result;
}				

static SmoothBool
GDK2CanvasFillChord(SmoothPrivateCanvas *Canvas,
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
		GdkGC *brush = internal_drawing_area_use_brush_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (brush)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, brush, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				#ifdef USE_CAIRO
  					SmoothBool do_render=!Canvas->CairoHandle.InStore;

					if (do_render)
						GDK2CanvasStore(Canvas);

					CairoDrawArc(Canvas->CairoHandle.CairoObject, X, Y, Width, Height, AnglePosition, AngleLength);
					
					if (do_render)
						GDK2CanvasRender(Canvas, TRUE);

				#else
					gdk_draw_arc(Canvas->GDKPrivateCanvas.Window, brush, SmoothTrue, X, Y, Width, Height,
							AnglePosition*64, AngleLength*64);
					gdk_draw_arc(Canvas->GDKPrivateCanvas.Window, brush, SmoothFalse, X, Y, Width, Height,
							AnglePosition*64, AngleLength*64);
				#endif
				GCUnsetClipArea(Canvas, brush, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_brush_gc(&Canvas->GDKPrivateCanvas, brush);
		}

		result = SmoothTrue;
	}
	return result;
}				

/* EVILNESS EVILNESS EVILNESS EVILNESS EVILNESS EVILNESS  */
/* Interface Internal code to set and unset Clipping mask */
/* EVILNESS EVILNESS EVILNESS EVILNESS EVILNESS EVILNESS  */
static void * 
GCSetClipArea(SmoothPrivateCanvas *Canvas, GdkGC *GC, GDK2ClipType *ClipType)
{
	void * result = NULL;
 	GdkBitmap *bmp = NULL;
	GdkRegion *rgn = NULL, *tmp_rgn = NULL;

	if (Canvas && GC && ClipType)
	{
		*ClipType = GDK2_CLIP_NONE;
		
		GCUnsetClipArea(Canvas, GC, NULL, GDK2_CLIP_NONE);

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
GCUnsetClipArea(SmoothPrivateCanvas *Canvas, GdkGC *GC, void *ClipArea, GDK2ClipType ClipType)
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
GDKInitializeCanvas(SmoothCanvas *Canvas,
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

	#ifdef USE_XLIB
		X11Display *dpy;
		Window drawable;
	#endif

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

	#ifdef USE_XLIB
		/* Unabstract X from GTK+ */
		if(!GDK_IS_WINDOW(_Window))
			real_drawable = _Window;
			
		dpy = gdk_x11_drawable_get_xdisplay (real_drawable);
		drawable = gdk_x11_drawable_get_xid (real_drawable);

		PrivateCanvas->X11Handle.Window = drawable;
		PrivateCanvas->X11Handle.Display = dpy;
		PrivateCanvas->X11Handle.XOffset = x_off;
		PrivateCanvas->X11Handle.YOffset = y_off;
		PrivateCanvas->X11Handle.Colormap = gdk_x11_colormap_get_xcolormap(PrivateCanvas->GDKPrivateCanvas.Colormap);
		PrivateCanvas->X11Handle.Visual = gdk_x11_visual_get_xvisual(gdk_colormap_get_visual(PrivateCanvas->GDKPrivateCanvas.Colormap));
	#endif
	
	#ifdef USE_XRENDER
		/* Create an XRender Handle */
		PrivateCanvas->Picture = XRenderCreatePicture(dpy, drawable, 
								XRenderFindVisualFormat (dpy, PrivateCanvas->X11Handle.Visual),
								0, NULL);		
	#endif

	#ifdef USE_CAIRO
		PrivateCanvas->CairoHandle.CacheIndex = -1;
		
		#if 0
		/* Create a Cairo Handle */
		for (i=0; i<cairo_cache_count, PrivateCanvas->CairoHandle.CacheIndex == -1; i++)
		{
			if (!(cairo_cache_used[i]))
			{
				cairo_cache_used[i] = TRUE;

				PrivateCanvas->CairoHandle.CacheIndex = i;

				if (!cairo_cache[i])
				{
					cairo_cache[i] = cairo_create();
				} 
				/*ifdef USE_XLIB*/
				else
				{
					if (_Window != real_drawable)
					{					
						cairo_destroy(cairo_cache[i]);
						cairo_cache[i] = cairo_create();
					}	
				}
				/*endif*/
				
				PrivateCanvas->CairoHandle.CairoObject = cairo_cache[i];
			}	
		}
		#endif
		
		if (PrivateCanvas->CairoHandle.CacheIndex == -1)
			PrivateCanvas->CairoHandle.CairoObject = cairo_create();
			
		#ifdef USE_XLIB
			cairo_set_target_drawable (PrivateCanvas->CairoHandle.CairoObject, dpy, drawable);

			/*#ifndef BROKEN_CAIRO_FUNCTIONS*/
				if(GDK_IS_WINDOW(_Window))
					cairo_translate(PrivateCanvas->CairoHandle.CairoObject,-(double)x_off,-(double)y_off);
			/*#endif*/		
		#else
			PrivateCanvas->CairoHandle.XOffset = x_off;
			PrivateCanvas->CairoHandle.YOffset = y_off;
			
			internal_create_cairo_image_buffer (PrivateCanvas, Width, Height);
		#endif		
	#endif

      	*Canvas = PrivateCanvas;

	GDKInitializeColorCube(*Canvas, Style, ColorCube);
}

void 
GDKFinalizeCanvas(SmoothCanvas *Canvas)
{
	SmoothPrivateCanvas *PrivateCanvas = *Canvas;
	
	#ifdef USE_XRENDER
		if (PrivateCanvas->Picture)
		      	XRenderFreePicture(PrivateCanvas->X11Handle.Display, PrivateCanvas->Picture);
	#endif

	#ifdef USE_CAIRO
	{
		#ifdef USE_XLIB
			if(GDK_IS_WINDOW(PrivateCanvas->GDKPrivateCanvas.Window))
				cairo_translate(PrivateCanvas->CairoHandle.CairoObject, PrivateCanvas->X11Handle.XOffset, PrivateCanvas->X11Handle.YOffset);
		#else
			internal_destroy_cairo_image_buffer(PrivateCanvas);
		#endif		

		if (PrivateCanvas->CairoHandle.CacheIndex == -1)
			cairo_destroy(PrivateCanvas->CairoHandle.CairoObject);
		else	
			cairo_cache_used[PrivateCanvas->CairoHandle.CacheIndex] = SmoothFalse;
	}		
	#endif
	
	g_free(PrivateCanvas);
	*Canvas = NULL;
}

#ifdef USE_BEZIER
/* scale a bezier */
static void 
smooth_bezier_scale(SmoothBezier *Bezier, 
			SmoothDouble ScaleX, 
			SmoothDouble ScaleY) 
{
	Bezier->p0.x *= ScaleX;	Bezier->p0.y *= ScaleY;
	Bezier->p1.x *= ScaleX;	Bezier->p1.y *= ScaleY;
	Bezier->p2.x *= ScaleX;	Bezier->p2.y *= ScaleY;
	Bezier->p3.x *= ScaleX;	Bezier->p3.y *= ScaleY;
}


/* rotate a bezier by theta degrees */
static void 
smooth_bezier_rotate(SmoothBezier *Bezier,  
			SmoothDouble Theta) 
{
	SmoothDouble sin_a, cos_a, tmp;

	Theta = -(Theta)*M_PI/180;
  
	cos_a = cos(Theta);
	sin_a = sin(Theta);

	tmp = Bezier->p0.x;
	Bezier->p0.x = tmp*cos_a + Bezier->p0.y*sin_a;
	Bezier->p0.y = tmp*sin_a - Bezier->p0.y*cos_a;

	tmp = Bezier->p1.x;
	Bezier->p1.x = tmp*cos_a + Bezier->p1.y*sin_a;
	Bezier->p1.y = tmp*sin_a - Bezier->p1.y*cos_a;

	tmp = Bezier->p2.x;
	Bezier->p2.x = tmp*cos_a + Bezier->p2.y*sin_a;
	Bezier->p2.y = tmp*sin_a - Bezier->p2.y*cos_a;

	tmp = Bezier->p3.x;
	Bezier->p3.x = tmp*cos_a + Bezier->p3.y*sin_a;
	Bezier->p3.y = tmp*sin_a - Bezier->p3.y*cos_a;
}

/* translate bezier to center of rectangle(x,y,x+width,y+height) */
static void 
smooth_bezier_translate(SmoothBezier *Bezier, 
			SmoothDouble CenterX, 
			SmoothDouble CenterY) 
{
	Bezier->p0.x += CenterX;	Bezier->p0.y += CenterY;
	Bezier->p1.x += CenterX;	Bezier->p1.y += CenterY;
	Bezier->p2.x += CenterX;	Bezier->p2.y += CenterY;
	Bezier->p3.x += CenterX;	Bezier->p3.y += CenterY;
}


/* aproximate an Arc of < 90 degrees as a Bezier */
static void 
smooth_arc_bezier(SmoothDouble Size, 
			SmoothDouble AnglePosition,
			SmoothDouble AngleLength,
			SmoothBezier * Bezier)
{
	SmoothDouble beta, s;
	SmoothDouble sin_a, cos_a;

	if ((abs(AngleLength) > 90) || (AngleLength == 0))
		return;

	s = Size / 2;

	AnglePosition *= M_PI/180;
	AngleLength *= M_PI/180;

	beta = (4/3)*(1 - cos(AngleLength/2))/(sin(AngleLength/2));

	cos_a = cos(AnglePosition);
	sin_a = sin(AnglePosition);

	Bezier->p0.x = s *cos_a;
	Bezier->p0.y = s *sin_a;
	Bezier->p1.x = Bezier->p0.x - beta * s * sin_a;
	Bezier->p1.y = Bezier->p0.y + beta * s * cos_a;

	cos_a = cos(AnglePosition + AngleLength);
	sin_a = sin(AnglePosition + AngleLength);

	Bezier->p3.x = s *cos_a;
	Bezier->p3.y = s *sin_a;
	Bezier->p2.x = Bezier->p3.x + beta * s * sin_a;
	Bezier->p2.y = Bezier->p3.y - beta * s * cos_a;
}

void
smooth_bezier_split(SmoothBezier * bezier, 
			SmoothBezier * left, 
			SmoothBezier * right)
{
  SmoothDouble centerX, centerY;
  
  centerX = (bezier->p1.x+bezier->p2.x)/2;
  centerY = (bezier->p1.y+bezier->p2.y)/2;


  left->p0.x = bezier->p0.x;
  left->p0.y = bezier->p0.y;

  right->p3.x = bezier->p3.x;
  right->p3.y = bezier->p3.y;

  left->p1.x = (bezier->p0.x + bezier->p1.x) / 2;
  left->p1.y = (bezier->p0.y + bezier->p1.y) / 2;

  right->p2.x = (bezier->p2.x + bezier->p3.x) / 2;
  right->p2.y = (bezier->p2.y + bezier->p3.y) / 2;

  left->p2.x = (left->p1.x + centerX) / 2;
  left->p2.y = (left->p1.y + centerY) / 2;

  right->p1.x = (right->p2.x + centerX) / 2;
  right->p1.y = (right->p2.y + centerY) / 2;

  right->p0.x = left->p3.x = (left->p2.x + right->p1.x) / 2;
  right->p0.y = left->p3.y = (left->p2.y + right->p1.y) / 2;
}

/* recursively split a bezier untill until colinear, then add to points */

SmoothBool 
smooth_bezier_colinear2(SmoothBezier *bezier, 
			SmoothDouble tolerance)
{
	SmoothDouble ux = pow(3.0*bezier->p1.x - 2.0*bezier->p0.x - bezier->p3.x, 2);
	SmoothDouble uy = pow(3.0*bezier->p1.y - 2.0*bezier->p0.y - bezier->p3.y, 2);
	SmoothDouble vx = pow(3.0*bezier->p2.x - 2.0*bezier->p3.x - bezier->p0.x, 2);
	SmoothDouble vy = pow(3.0*bezier->p2.y - 2.0*bezier->p3.y - bezier->p0.y, 2);

	if (ux < vx) ux = vx;
	if (uy < vy) uy = vy;

	return (ux+uy <= tolerance); /* tolerance is 16*tol^2 */
}

void 
smooth_bezier_split_recursive_draw(SmoothPrivateCanvas *Canvas,
					SmoothBezier *bezier, 
					SmoothDouble tolerance)
{
  SmoothBezier left, right;

  if (smooth_bezier_colinear2(bezier, tolerance)) 
  {
	if (Canvas)
	{
		GdkGC *pen = internal_drawing_area_use_pen_gc(&Canvas->GDKPrivateCanvas, SmoothTrue);
		
		if (pen)
		{
			GDK2ClipType clip_type;
			void *clip_area=NULL;
		
			clip_area = GCSetClipArea(Canvas, pen, &clip_type);

			if (clip_type != GDK2_CLIP_EMPTY)
			{
				gdk_gc_set_line_attributes(pen, 0, GDK_LINE_SOLID, GDK_CAP_ROUND, GDK_JOIN_BEVEL);
				gdk_draw_line(Canvas->GDKPrivateCanvas.Window, pen, bezier->p0.x, bezier->p0.y, bezier->p3.x, bezier->p3.y);
				GCUnsetClipArea(Canvas, pen, clip_area, clip_type);
			}	
			internal_drawing_area_unuse_pen_gc(&Canvas->GDKPrivateCanvas, pen);
		}
	}
  } 
  else 
  {
    smooth_bezier_split(bezier, &left, &right);
    smooth_bezier_split_recursive_draw(Canvas, &left, tolerance);
    smooth_bezier_split_recursive_draw(Canvas, &right, tolerance);
  }
}

static void
SmoothDrawArc(SmoothPrivateCanvas *Canvas,
			SmoothInt X, 
			SmoothInt Y, 
			SmoothInt Width, 
			SmoothInt Height,
			SmoothDouble AnglePosition,
			SmoothDouble AngleLength)
{
	SmoothDouble size = MIN(Width, Height);
	SmoothDouble scalex = 1, scaley = 1;

	SmoothInt i,k=0;
	SmoothDouble segment=22.5, full_angle=0,real_angle=AnglePosition,real_length=AngleLength;
									
	scalex = Width/Height;
	scaley = Height/Width;

	if (abs(AngleLength) > 360) 
	{
		AngleLength = 360;
		AnglePosition = 0;
	}

	full_angle = AnglePosition+AngleLength;
	k = ceil(abs(AngleLength) / segment);

	if (AngleLength > segment)
		AngleLength = segment;

	if (AngleLength < -segment)
		AngleLength = -segment;
		
	for (i = 0; i<k; i++) 
	{
		SmoothBezier bez;

		smooth_arc_bezier(size, AnglePosition, AngleLength, &bez);

		smooth_bezier_scale(&bez, scalex, scaley);
		smooth_bezier_rotate(&bez, 0);    	
		smooth_bezier_translate(&bez, X + Width/2 + 0.5, Y + Height/2 + 0.5);
 
		smooth_bezier_split_recursive_draw(Canvas, &bez, 0.1);

		AnglePosition += AngleLength;
    
		if ((AnglePosition > 0)?((full_angle - AnglePosition) > segment):(abs(full_angle - AnglePosition) > segment))
			AngleLength = (AngleLength > 0)?(segment):(-segment);
		else
			AngleLength = (AngleLength > 0)?(full_angle - AnglePosition):-(full_angle - AnglePosition);
	}
}
#endif

#ifdef USE_CAIRO
static void
CairoDrawArc(cairo_t *CairoHandle,
			SmoothInt X, 
			SmoothInt Y, 
			SmoothInt Width, 
			SmoothInt Height,
			SmoothDouble AnglePosition,
			SmoothDouble AngleLength)
{
	SmoothDouble size = MIN(Width + 0.25, Height + 0.25);
	SmoothDouble scalex = 1, scaley = 1;

	SmoothInt i,k=0;
	SmoothDouble segment=45, full_angle=0,real_angle=AnglePosition,real_length=AngleLength;
									
	scalex = Width/Height;
	scaley = Height/Width;

	if (abs(AngleLength) > 360) 
	{
		AngleLength = 360;
		AnglePosition = 0;
	}

	full_angle = AnglePosition+AngleLength;
	k = ceil(abs(AngleLength) / segment);

	if (AngleLength > segment)
		AngleLength = segment;

	if (AngleLength < -segment)
		AngleLength = -segment;
		
	for (i = 0; i<k; i++) 
	{
		SmoothBezier bez;

		smooth_arc_bezier(size, AnglePosition, AngleLength, &bez);

		smooth_bezier_scale(&bez, scalex, scaley);
		smooth_bezier_rotate(&bez, 0);    	
		smooth_bezier_translate(&bez, X + Width/2 + 0.5, Y + Height/2 + 0.5);
 
		if (i==0)
			cairo_move_to(CairoHandle, bez.p0.x, bez.p0.y);
    
		cairo_curve_to(CairoHandle, bez.p1.x, bez.p1.y,bez.p2.x, bez.p2.y,bez.p3.x, bez.p3.y);

		AnglePosition += AngleLength;
    
		if ((AnglePosition > 0)?((full_angle - AnglePosition) > segment):(abs(full_angle - AnglePosition) > segment))
			AngleLength = (AngleLength > 0)?(segment):(-segment);
		else
			AngleLength = (AngleLength > 0)?(full_angle - AnglePosition):-(full_angle - AnglePosition);
	}
}
#endif

/* Initialize drawingInterface */
static SmoothBool 
_DrawingInterfaceInitialize(DrawingInterfaceClass *_DrawingInterface)
{
	#ifdef USE_CAIRO
	SmoothInt i;

	for (i=0; i<cairo_cache_count; i++)
	{
		cairo_cache_used[i] = SmoothFalse;
		cairo_cache[i] = NULL;
	}
	#endif	

	GDKDrawingInterfaceInitialize(_DrawingInterface);

	/* Image/Buffer By File name (For caching purposes) */
	_DrawingInterface->ImageBufferLoadByFile = (void *) GDK2ImageBufferLoadByFile;
	_DrawingInterface->ImageBufferUnLoadByFile = (void *) GDK2ImageBufferUnLoadByFile;

	/* Canvas Get ImageBuffer Size */
	_DrawingInterface->ImageBufferGetWidth = (void *) GDK2ImageBufferGetWidth;
	_DrawingInterface->ImageBufferGetHeight = (void *) GDK2ImageBufferGetHeight;

	/* Canvas Get Image Size */
	_DrawingInterface->ImageGetWidth = (void *) GDK2ImageGetWidth;
	_DrawingInterface->ImageGetHeight = (void *) GDK2ImageGetHeight;

	/* Canvas Store/Render */
	_DrawingInterface->CanvasStore =  (void *) GDK2CanvasStore;
	_DrawingInterface->CanvasRender =  (void *) GDK2CanvasRender;

	/* Canvas Create Image */
	_DrawingInterface->CanvasCreateImage = (void *) GDK2CanvasCreateImage;
	_DrawingInterface->CanvasCreateImageFromBuffer = (void *) GDK2CanvasCreateImageFromBuffer;
	_DrawingInterface->CanvasDestroyImage = (void *) GDK2CanvasDestroyImage;

	/* Canvas Tile Image/ImageBuffer */
	_DrawingInterface->CanvasRenderImage = (void *) GDK2CanvasRenderImage;						
	_DrawingInterface->CanvasRenderImageBuffer = (void *) GDK2CanvasRenderImageBuffer;						


	/* Canvas Render Gradient */
	#ifdef SMOOTHGRADIENT
	_DrawingInterface->CanvasRenderGradient = (void *)GDK2CanvasRenderGradient;
	#endif
	
	/* Canvas Draw _Point(s) */
	_DrawingInterface->CanvasDrawPoint = (void *) GDK2CanvasDrawPoint;
	_DrawingInterface->CanvasDrawPoints = (void *) GDK2CanvasDrawPoints;

	/* Canvas Draw Line(s) */
	_DrawingInterface->CanvasDrawLine = (void *) GDK2CanvasDrawLine;
	_DrawingInterface->CanvasDrawSegment = (void *) (void *) GDK2CanvasDrawSegment;
	_DrawingInterface->CanvasDrawLines = (void *) GDK2CanvasDrawLines;
/*	_DrawingInterface->CanvasDrawSegments = (void *) NULL;*/
	
	/* Canvas Draw Arc(s)/Curve(s) */
	_DrawingInterface->CanvasDrawArc = (void *) GDK2CanvasDrawArc;

	/* Canvas Frame Shaped Areas */
	_DrawingInterface->CanvasFrameRectangle = (void *) GDK2CanvasFrameRectangle;
	_DrawingInterface->CanvasFramePolygon = (void *) GDK2CanvasFramePolygon;
	_DrawingInterface->CanvasFrameChord = (void *) GDK2CanvasFrameChord;

	/* Canvas Fill Shaped Areas */
	_DrawingInterface->CanvasFillRectangle = (void *) GDK2CanvasFillRectangle;
	_DrawingInterface->CanvasFillPolygon = (void *) GDK2CanvasFillPolygon;
	_DrawingInterface->CanvasFillChord = (void *) GDK2CanvasFillChord;
	
	return SmoothTrue;
}

static SmoothBool _DrawingInterfaceFinalize(DrawingInterfaceClass *_DrawingInterface)
{
	#ifdef USE_CAIRO
	SmoothInt i;

	for (i=0; i<cairo_cache_count; i++)
	{
		cairo_cache_used[i] = SmoothFalse;
		if (cairo_cache[i])
			cairo_destroy(cairo_cache[i]);
	}
	#endif	

	cleanup_gdk_pixbuf_cache(SmoothTrue);

	GDKDrawingInterfaceFinalize(_DrawingInterface);
	
	return SmoothTrue;
}

/* Main Functions for Engine to Setup and Finalize - Each interface must provide these */
void SmoothDrawingInterfaceInitialize()
{
	SmoothDrawingInterfaceSetup(_DrawingInterfaceInitialize, _DrawingInterfaceFinalize);
}

void SmoothDrawingInterfaceFinalize()
{
	SmoothDrawingInterfaceCleanup();
}

#ifdef SMOOTHGRADIENT
#include "src/gdk2_gradient_routines.c"
#endif

/* define stackblur */ 

#ifdef stackblur
/* Stack Blur Algorithm by Mario Klingemann <mario@quasimondo.com> */

SmoothBool
GDK2ImageBufferApplyBlur(SmoothImageBuffer ImageBuffer,
				SmoothInt Radius)
{
	/* Image Buffer Values */
	SmoothUChar *pixels;
	SmoothInt rowstride, width, height;

	/* A Blur with no radius is nice and fast ;) */
	if (Radius < 1)
	{
		return SmoothTrue; 
	}

	/* Get Pixels and Rowstride Of Image Buffer */
	if (ImageBuffer)
	{
		pixels = gdk_pixbuf_get_pixels (ImageBuffer);
		rowstride = gdk_pixbuf_get_rowstride (ImageBuffer);
		width = gdk_pixbuf_get_width(ImageBuffer);
		height = gdk_pixbuf_get_height(ImageBuffer);
	}
	else
	{
		return SmoothFalse;
	}

	/* Some Important Consts */
	SmoothInt bytes = rowstride/width;
	SmoothBool alpha = (bytes == 4);

	SmoothInt div = Radius+Radius+1;
	SmoothInt divsum = ((div+1)>>1)*((div+1)>>1);
	SmoothInt dv[256*divsum]; /* Table of Const RGBA Values */

	/* Some Important Variables */
	SmoothUChar stack[div][bytes];
	SmoothInt stackpointer;
	SmoothInt stackstart;
	SmoothInt vmin[MAX(width,height)];
	SmoothUChar *sir;
	SmoothInt rbs;

	SmoothInt current = 0;

	/* RGBA Sums
		0 - Sum of Incoming pixels(the Radius pixels above the Center/to left of Center)
		1 - Sum of Outgoing pixels(the Center and the Radius pixels below the Center/to right of Center)
		2 - Sum of All pixels within Radius
	*/
	SmoothInt rgba_sums[3][bytes];
		
	/* RGBA Values */
	SmoothUChar rgba[width*height][bytes];

	/* Temp Indexes/Counters */
	SmoothInt x, y, i, p, yp, yi=0, yw=0;

	for (i=0; i<256*divsum; i++)
	{
		dv[i] = (i/divsum);
	}

	for (y=0; y < height; y++)
	{
		/* initialize sums to zero */
		memset(rgba_sums, 0, sizeof(int)*bytes*3);

		/* Calculate Initial Sums For Radius */
		for(i=-Radius; i<=Radius; i++)
		{
			current = (yi + MIN(width - 1, MAX(i,0)))*bytes;

			sir = stack[i+Radius];

			memcpy(&sir[0], &pixels[current + 0], bytes);

			rbs = (Radius + 1) - abs(i);

			rgba_sums[2][0] += sir[0]*rbs;
			rgba_sums[2][1] += sir[1]*rbs;
			rgba_sums[2][2] += sir[2]*rbs;
			if (alpha)
			{
				rgba_sums[2][3] += sir[3]*rbs;
			}

			if (i>0)
			{
				rgba_sums[0][0] += sir[0];
				rgba_sums[0][1] += sir[1];
				rgba_sums[0][2] += sir[2];
				if (alpha)
				{
					rgba_sums[0][3] += sir[3];
				}
			} 
			else 
			{
				rgba_sums[1][0] += sir[0];
				rgba_sums[1][1] += sir[1];
				rgba_sums[1][2] += sir[2];
				if (alpha)
				{
					rgba_sums[1][3] += sir[3];
				}
			}
		}
		
		stackpointer = Radius;

		for (x=0; x<width; x++)
		{
			rgba[yi][0] = dv[rgba_sums[2][0]];
			rgba[yi][1] = dv[rgba_sums[2][1]];
			rgba[yi][2] = dv[rgba_sums[2][2]];
			if (alpha)
			{
				rgba[3][yi] = dv[rgba_sums[2][3]];
			}
      
			rgba_sums[2][0] -= rgba_sums[1][0];
			rgba_sums[2][1] -= rgba_sums[1][1];
			rgba_sums[2][2] -= rgba_sums[1][2];
			if (alpha)
			{
				rgba_sums[2][3] -= rgba_sums[1][3];
			}
			
			stackstart = stackpointer - Radius + div;
			sir = stack[stackstart%div];
      
			rgba_sums[1][0] -= sir[0];
			rgba_sums[1][1] -= sir[1];
			rgba_sums[1][2] -= sir[2];
			if (alpha)
			{
				rgba_sums[1][3] -= sir[3];
			}
      
			if(y==0)
			{
				vmin[x] = MIN(x + Radius + 1, width - 1);
			}

			current = (yw + vmin[x])*bytes;

			memcpy(&sir[0], &pixels[current + 0], bytes);

			rgba_sums[0][0] += sir[0];
			rgba_sums[0][1] += sir[1];
			rgba_sums[0][2] += sir[2];
			if (alpha)
			{
				rgba_sums[0][3] += sir[3];
			}
			
			rgba_sums[2][0] += rgba_sums[0][0];
			rgba_sums[2][1] += rgba_sums[0][1];
			rgba_sums[2][2] += rgba_sums[0][2];
			if (alpha)
			{
				rgba_sums[2][3] += rgba_sums[0][3];
			}
			
			stackpointer=(stackpointer+1)%div;
			sir=stack[(stackpointer)%div];

			rgba_sums[1][0] += sir[0];
			rgba_sums[1][1] += sir[1];
			rgba_sums[1][2] += sir[2];
			if (alpha)
			{
				rgba_sums[1][3] += sir[3];
			}
     			
			rgba_sums[0][0] -= sir[0];
			rgba_sums[0][1] -= sir[1];
			rgba_sums[0][2] -= sir[2];
			if (alpha)
			{
				rgba_sums[0][3] -= sir[3];
			}
     			
			yi++;
		}

		yw += width;
	}

	for (x=0; x<width; x++)
	{
		yp=-Radius*width;

		/* initialize sums to zero */
		memset(rgba_sums, 0, sizeof(int)*bytes*3);

		/* Calculate Initial Sums For Radius */
		for(i=-Radius; i<=Radius; i++)
		{
			yi = MAX(0,yp) + x;
     
			sir = stack[i+Radius];
      
			memcpy(&sir[0], &rgba[yi][0], bytes);

			rbs = (Radius + 1) - abs(i);
      
			rgba_sums[2][0] += rgba[yi][0]*rbs;
			rgba_sums[2][1] += rgba[yi][1]*rbs;
			rgba_sums[2][2] += rgba[yi][2]*rbs;
			if (alpha)
			{
				rgba_sums[2][3] += rgba[3][yi]*rbs;
			}     

			if (i>0)
			{
				rgba_sums[0][0] += sir[0];
				rgba_sums[0][1] += sir[1];
				rgba_sums[0][2] += sir[2];
				if (alpha)
				{
					rgba_sums[0][3] += sir[3];
				} 
			}
			else 
			{
				rgba_sums[1][0] += sir[0];
				rgba_sums[1][1] += sir[1];
				rgba_sums[1][2] += sir[2];
				if (alpha)
				{
					rgba_sums[1][3] += sir[3];
				}
			}
      
			if(i < height - 1)
			{
				yp += width;
			}
		}

		yi = x;
		stackpointer = Radius;

		for (y=0; y<height; y++)
		{
			current = (yi)*bytes;

			pixels[current + 0] = dv[rgba_sums[2][0]];
			pixels[current + 1] = dv[rgba_sums[2][1]];
			pixels[current + 2] = dv[rgba_sums[2][2]];
			if (alpha)
			{
				pixels[current + 3] = dv[rgba_sums[2][3]];
			}

			rgba_sums[2][0] -= rgba_sums[1][0];
			rgba_sums[2][1] -= rgba_sums[1][1];
			rgba_sums[2][2] -= rgba_sums[1][2];
			if (alpha)
			{
				rgba_sums[2][3] -= rgba_sums[1][3];
			}

			stackstart = stackpointer - Radius + div;
			sir = stack[stackstart%div];
     
			rgba_sums[1][0] -= sir[0];
			rgba_sums[1][1] -= sir[1];
			rgba_sums[1][2] -= sir[2];
			if (alpha)
			{
				rgba_sums[1][3] -= sir[3];
     			}

			if (x == 0)
			{
				vmin[y] = MIN(y + (Radius + 1), height - 1)*width;
			}

			p = x + vmin[y];
      
			memcpy(&sir[0], &rgba[p][0], bytes);

			rgba_sums[0][0] += sir[0];
			rgba_sums[0][1] += sir[1];
			rgba_sums[0][2] += sir[2];
			if (alpha)
			{
				rgba_sums[0][3] += sir[3];
			}

			rgba_sums[2][0] += rgba_sums[0][0];
			rgba_sums[2][1] += rgba_sums[0][1];
			rgba_sums[2][2] += rgba_sums[0][2];
			if (alpha)
			{
				rgba_sums[2][3] += rgba_sums[0][3];
			}

			stackpointer = (stackpointer+1)%div;
			sir = stack[stackpointer];
     
			rgba_sums[1][0] += sir[0];
			rgba_sums[1][1] += sir[1];
			rgba_sums[1][2] += sir[2];
			if (alpha)
			{
				rgba_sums[1][3] += sir[3];
			}      

			rgba_sums[0][0] -= sir[0];
			rgba_sums[0][1] -= sir[1];
			rgba_sums[0][2] -= sir[2];
			if (alpha)
			{
				rgba_sums[0][3] -= sir[3];
			}

			yi += width;
		}
	}

	return SmoothTrue;
}
#endif
