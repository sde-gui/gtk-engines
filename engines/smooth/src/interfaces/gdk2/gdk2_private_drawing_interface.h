/******************************************************************************/
/* gdk2_private_drawing_interface.h - GDK2 specific Private canvas etc        */
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
#ifndef GDK2_PRIVATE_DRAWING_INTERFACE_H
#define GDK2_PRIVATE_DRAWING_INTERFACE_H

#include "gdk2_type_defines.h"
#include <interfaces/shared/gdk/gdk_private_drawing_interface.h>

#ifdef USE_XRENDER
  #ifndef USE_XLIB
    #define USE_XLIB
  #endif
#endif

#ifdef USE_CAIRO
  #ifndef USE_XLIB
    #define USE_XLIB
  #endif

/*#define BROKEN_CAIRO_FUNCTIONS*/
/*#define SLOW_CAIRO_FUNCTIONS*/
#undef USE_XRENDER
#endif

#ifdef USE_XLIB
#include "gdk/gdkx.h"
#include <X11/Xlib.h>

/* Declare these to avoid variable name conflicts */
typedef Display X11Display;

typedef Visual X11Visual;
typedef Colormap X11Colormap;

typedef Window X11Window;

#include <X11/Xutil.h>
#endif

#ifdef USE_XRENDER
#include <X11/extensions/Xrender.h>

typedef Picture XRenderPicture;
#endif

#ifdef USE_CAIRO
#include <cairo.h>
#endif

typedef struct
{
	SmoothGDKPrivateCanvas GDKPrivateCanvas;
#ifdef USE_XLIB
        struct 
	{
		/************************************************/
		/* Display:					*/
		/*						*/
		/* 	This is the X Display for the canvas.	*/
		/************************************************/
		X11Display	*Display;
	
		/************************************************/
		/* Window:					*/
		/*						*/
		/* 	This is the X handle of the canvas.	*/
		/************************************************/
		X11Window	Window;

		/************************************************/
		/* Colormap, Visual:				*/
		/*						*/
		/* 	These are the X Visual and Colormap,	*/
		/* 	extracted from the GDKColormap.		*/
		/************************************************/
		X11Colormap	Colormap;
		X11Visual	*Visual;

		/************************************************/
		/* Offsets:					*/
		/*						*/
		/* 	The X and Y Offset of the Window Handle.*/
		/************************************************/
		SmoothInt	XOffset;
		SmoothInt	YOffset;
	} X11Handle;
#endif
	
#ifdef USE_XRENDER
	/************************************************/
	/* SmoothGDKCanvas.Picture:			*/
	/*						*/
	/* 	This is the Xrender handle.		*/
	/************************************************/
	XRenderPicture			Picture;
#endif

#ifdef USE_CAIRO
	/************************************************/
	/* SmoothGDKCanvas.Picture:			*/
	/*						*/
	/* 	This is the Cairo handle.		*/
	/************************************************/
	struct
	{
		cairo_t			*CairoObject;
#ifdef USE_XLIB
		SmoothImage		Buffer;
#else
		SmoothUChar 		*Buffer;

		SmoothInt		XOffset;
		SmoothInt		YOffset;
		
		SmoothInt		Width;
		SmoothInt		Height;
#endif
		SmoothInt		CacheIndex;
		SmoothBool		InStore;
	}  CairoHandle;
#endif
} SmoothPrivateCanvas;

/*SmoothBool GDK2CanvasApplyBlur(SmoothPrivateCanvas *Canvas, SmoothInt Radius, SmoothInt X, SmoothInt Y, SmoothInt Width, SmoothInt Height);*/

SmoothBool GDK2ImageBufferApplyBlur(SmoothImageBuffer ImageBuffer, SmoothInt Radius);

#endif /* GDK2_PRIVATE_DRAWING_INTERFACE_H */
