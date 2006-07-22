/******************************************************************************/
/* smooth_gtk_misc.h - Misc. Shared definition for all GTK+ versions          */
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
/* Portions Based on the EnGradient Engine                                    */
/*   Andrew Cattau                                                            */
/*                                                                            */
/* Portions Based on the Xenophilia Engine                                    */
/*   Johan Hanson <misagon@bahnhof.se>                                        */
/******************************************************************************/
#ifndef SMOOTH_GTK_MISC_H
#define SMOOTH_GTK_MISC_H

/* Polygon points for Fixed widget shapes*/
gboolean rounded_extension_points(gint x, gint y, gint width,  gint height, gboolean selected, gboolean fill, GtkPositionType position, GdkPoint points[8]) G_GNUC_INTERNAL;
gboolean square_extension_points(gint x, gint y, gint width, gint height, gboolean selected, gboolean fill, GtkPositionType position, GdkPoint points[8]) G_GNUC_INTERNAL;
gboolean triangle_extension_points(gint x, gint y, gint width, gint height, gboolean selected, gboolean fill, GtkPositionType position, GdkPoint points[8]) G_GNUC_INTERNAL;
		
void rounded_box_points(gint x, gint y, gint width, gint height, GdkPoint points[13], gboolean large) G_GNUC_INTERNAL;

#endif /* SMOOTH_GTK_MISC_H */
