/******************************************************************************/
/* gdk2_drawing_interface.h - GDK2 Drawing Interface Header                   */
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
#ifndef GDK2_DRAWING_INTERFACE_H
#define GDK2_DRAWING_INTERFACE_H

#include <interfaces/drawing_interface.h>
#include <interfaces/shared/gdk/gdk_drawing_interface.h>

void GDKInitializeCanvas(SmoothCanvas *Canvas,
				GtkStyle * Style,
				GdkWindow * _Window,
				GdkRectangle * Area,
				GdkRegion * _Region,
				GdkBitmap *ClipMask,
				SmoothInt Width,
				SmoothInt Height,				
				SmoothInt XOffset,
				SmoothInt YOffset,
				SmoothColorCube *ColorCube);

void GDKFinalizeCanvas(SmoothCanvas *Canvas);

#endif /* GDK2_DRAWING_INTERFACE_H */
