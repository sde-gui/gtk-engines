/******************************************************************************/
/* gdk_private_drawing_interface.h - Base Private Canvas for all GDK versions */
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
#ifndef GDK_PRIVATE_DRAWING_INTERFACE_H
#define GDK_PRIVATE_DRAWING_INTERFACE_H

typedef struct
{

	/************************************************/
	/* SmoothGDKCanvas.Window:			*/
	/*						*/
	/* 	This is the GDK abstraction around the	*/
	/* 	X handle of the virtual canvas.		*/
	/*						*/
	/* 	All drawing primitives are ultimately 	*/
	/* 	performed on this.			*/
	/************************************************/
	GdkWindow	*Window;
	struct
	{
		GdkDrawable	*Window;
		SmoothInt	XOffset;
		SmoothInt	YOffset;
	} Drawable;

	/************************************************/
	/* SmoothGDKCanvas.Pen:				*/
	/*						*/
	/*	Pen Color/Thickness - seperate from GC	*/
	/*	to allow caching GC's by color hash.	*/
	/************************************************/
	struct 
	{
		SmoothColor Color;
		SmoothBool Cached;

		SmoothFloat Thickness;
		SmoothBool UseThickness;

		SmoothLineStyle Style;
		SmoothBool UseStyle;

		SmoothLineJoin Join;
		SmoothBool UseJoin;

		SmoothLineCap Cap;
		SmoothBool UseCap;
		
		SmoothLinePattern Pattern;
		SmoothBool UsePattern;
	} Pen;

	/************************************************/
	/* SmoothGDKCanvas.Brush:			*/
	/*						*/
	/*	Brush Color/Thickness - seperate from GC*/
	/*	to allow caching GC's by color hash.	*/
	/************************************************/
	struct 
	{
		SmoothBrushType Type;
		SmoothColor Color;
		SmoothImage Image;
	} Brush;

	/************************************************/
	/* SmoothGDKCanvas.Colormap :			*/
	/*						*/
	/* 	Every color used in GDK is allocated 	*/
	/* 	within a a colormap which is always	*/
	/* 	specific to the visual which represents	*/
	/* 	the actual server upon	which the 	*/
	/* 	drawing is performed.			*/
	/*						*/
	/*	We need this to be able to allocate,    */
	/*	and/or free new colors on the fly for	*/
	/* 	complex drawing such as of gradients.	*/
	/************************************************/
	GdkColormap	*Colormap;		
	SmoothInt	Depth;

	SmoothInt	DitherDepth;

	SmoothColorCube	*ColorCube;
	
	/************************************************/
	/* SmoothGDKCanvas.EngineClip :			*/
	/*						*/
	/* 	It may be necesary to always ensure a 	*/
	/* 	clip which has been preset by the engine*/
	/*	or by the toolkit itself. So we provide	*/
	/*	this which will be merged with any	*/
	/*	backend specific clips as needed on	*/
	/*	actual draw/fill/render.		*/
	/************************************************/
	struct {
 		SmoothBitmap *BitmapMask;
		SmoothRegion *Region;
		SmoothRectangle *Rectangle;
		
		SmoothInt XOffset;
		SmoothInt YOffset;
	} EngineClip;

	/************************************************/
	/* SmoothGDKCanvas.Clip :			*/
	/*						*/
	/* 	Many functions require the ability	*/
	/* 	to modify or ignore the clipping	*/
	/* 	area, so we hold it internally allowing	*/
	/* 	it to be modified, or its values read	*/
	/* 	without having to pass it directly.	*/
	/*	We also do it this way to hide the 	*/
	/*	differences between clip types, which 	*/
	/*	in GDK are mutually exclusive.		*/
	/************************************************/

	SmoothRectangle Clip;
	SmoothBool NoClip;

	/************************************************/
	/* About Clipping - 				*/
	/*						*/
	/* to determine used GC Clip we must do cruft -	*/
	/*						*/
	/* if there is an Engine Clip of Bitmap mask	*/
	/* and region or rectangle, then we first create*/
	/* a new bitmap of size:			*/
	/*						*/
	/* 	XOffset + BitmapMask.Width		*/
	/*	YOffet + BitmapMask.Height		*/
	/*						*/
	/* Then we render the bitmap mask to the new	*/
	/* mask at clip offsets, using the Region or	*/
	/* Rectangle as the clip to only include those	*/
	/* areas which are in both.			*/
	/*						*/
	/* In the event that we have both a Rectangle	*/
	/* and a region, we use a union between them for*/
	/* this clip instead.				*/
	/* 						*/
	/* This mask will be used if there is no other	*/
	/* clip.					*/
	/* 						*/
	/* If there is also a backend clip we do the	*/
	/* same, essentially finding the lowest common	*/
	/* way to merge supplied clips.			*/
	/*						*/
	/* We then merge EngineClip and Clip together,	*/
	/* using whatever is the simplest method, with	*/
	/* the most complex being of course create yet	*/
	/* another(potentially 3rd or 4th) mask and use	*/
	/* the engine clip to draw the backend clip.	*/
	/* Where the resultant mask as the used clip.	*/
	/************************************************/
} SmoothGDKPrivateCanvas;

GdkGC * smooth_internal_drawing_area_use_brush_gc(SmoothGDKPrivateCanvas *Canvas, SmoothBool RequireValidColor) G_GNUC_INTERNAL;
void smooth_internal_drawing_area_unuse_brush_gc(SmoothGDKPrivateCanvas *Canvas, GdkGC * GC) G_GNUC_INTERNAL;
void smooth_internal_drawing_area_unuse_pen_gc(SmoothGDKPrivateCanvas *Canvas, GdkGC * GC) G_GNUC_INTERNAL;
GdkGC * smooth_internal_drawing_area_use_pen_gc(SmoothGDKPrivateCanvas *Canvas, SmoothBool RequireValidColor) G_GNUC_INTERNAL;

#endif /* GDK_PRIVATE_DRAWING_INTERFACE_H */
