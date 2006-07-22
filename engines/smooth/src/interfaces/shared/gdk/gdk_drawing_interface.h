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

SmoothBool GDKDrawingInterfaceInitialize(DrawingInterfaceClass *_DrawingInterface) G_GNUC_INTERNAL;
SmoothBool GDKDrawingInterfaceFinalize(DrawingInterfaceClass *_DrawingInterface) G_GNUC_INTERNAL;

void GDKModifyCanvasClipArea(SmoothCanvas *Canvas,
				GdkRectangle * _Rectangle) G_GNUC_INTERNAL;
				
void GDKModifyCanvasClipRegion(SmoothCanvas *Canvas,
				GdkRegion * _Region) G_GNUC_INTERNAL;
				
void GDKModifyCanvasClipMask(SmoothCanvas *Canvas,
				GdkBitmap *ClipMask) G_GNUC_INTERNAL;
				
void GDKModifyCanvasClipOffset(SmoothCanvas *Canvas,
				SmoothInt XOffset,
				SmoothInt YOffset) G_GNUC_INTERNAL;
				
void GDKModifyCanvasDitherDepth(SmoothCanvas *Canvas,
				SmoothInt DitherDepth) G_GNUC_INTERNAL;

void GDKSmoothColorAssignGdkColor(SmoothColor *DestinationColor, GdkColor SourceColor, gint SourceAlpha) G_GNUC_INTERNAL;

void GDKInitializeColorCube(SmoothCanvas Canvas, GtkStyle * style, SmoothColorCube *ColorCube) G_GNUC_INTERNAL;
void  GDKFinalizeColorCube(SmoothColorCube *ColorCube) G_GNUC_INTERNAL;

SmoothWidgetState GDKSmoothWidgetState(GtkStateType State) G_GNUC_INTERNAL;

void GDKFinalizeColorCache(GHashTable **ColorCache) G_GNUC_INTERNAL;

#endif /* GDK_DRAWING_INTERFACE_H */
