/******************************************************************************/
/* gdk_drawing_interface.h - Shared drawing interface for all GDK versions    */
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
#ifndef GDK_DRAWING_INTERFACE_H
#define GDK_DRAWING_INTERFACE_H

GE_INTERNAL SmoothBool SmoothGDKDrawingInterfaceInitialize(DrawingInterfaceClass *_SmoothDrawingInterface);
GE_INTERNAL SmoothBool SmoothGDKDrawingInterfaceFinalize(DrawingInterfaceClass *_SmoothDrawingInterface);

GE_INTERNAL void SmoothGDKModifyCanvasClipArea(SmoothCanvas *Canvas,
				GdkRectangle * _Rectangle);
				
GE_INTERNAL void SmoothGDKModifyCanvasClipRegion(SmoothCanvas *Canvas,
				GdkRegion * _Region);
				
GE_INTERNAL void SmoothGDKModifyCanvasClipMask(SmoothCanvas *Canvas,
				GdkBitmap *ClipMask);
				
GE_INTERNAL void SmoothGDKModifyCanvasClipOffset(SmoothCanvas *Canvas,
				SmoothInt XOffset,
				SmoothInt YOffset);
				
GE_INTERNAL void SmoothGDKModifyCanvasDitherDepth(SmoothCanvas *Canvas,
				SmoothInt DitherDepth);

GE_INTERNAL void SmoothColorAssignGdkColor(SmoothColor *DestinationColor, GdkColor SourceColor, gint SourceAlpha);

GE_INTERNAL void SmoothGDKInitializeColorCube(SmoothCanvas Canvas, GtkStyle * style, SmoothColorCube *ColorCube);
GE_INTERNAL void  SmoothGDKFinalizeColorCube(SmoothColorCube *ColorCube);

GE_INTERNAL SmoothWidgetState SmoothGtkWidgetState(GtkStateType State);

GE_INTERNAL void SmoothGDKFinalizeColorCache(GHashTable **ColorCache);

#endif /* GDK_DRAWING_INTERFACE_H */
