/******************************************************************************/
/* draw_grips.h - Engine Agnostic Grip Part Defines                           */
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
/* Portions Based on the CleanIce Engine                                      */
/*   Bastien Nocera <hadess@hadess.net>                                       */
/*   Rodney Dawes                                                             */
/*                                                                            */
/* Portions Based on the XFCE Engine                                          */
/*   Olivier Fourdan <fourdan@xfce.org>                                       */
/*                                                                            */
/* Portions Based on the Xenophilia Engine                                    */
/*   Johan Hanson <misagon@bahnhof.se>                                        */
/******************************************************************************/
#ifndef DRAW_GRIPS_H
#define DRAW_GRIPS_H

/* Currently the Grip Style is big mess we have the bunch of defines */
#define NO_GRIP				1
#define BARS_IN_GRIP			2
#define BARS_OUT_GRIP			3
#define LINES_IN_GRIP			4
#define LINES_OUT_GRIP			5
#define FIXEDLINES_IN_GRIP		6
#define FIXEDLINES_OUT_GRIP		7
#define SLASHES_GRIP			8
#define MAC_BUDS_IN_GRIP		9
#define MAC_BUDS_OUT_GRIP		10
#define NS_BUDS_IN_GRIP			11
#define NS_BUDS_OUT_GRIP		12
#define DOTS_IN_GRIP			13
#define DOTS_OUT_GRIP			14
#define SMALLDOTS_IN_GRIP		15
#define SMALLDOTS_OUT_GRIP		16
#define AUTODOTS_IN_GRIP		17
#define AUTODOTS_OUT_GRIP		18
#define XPM_GRIP			19

typedef struct
{
	SmoothInt Size;
	SmoothInt Count;
	SmoothInt Spacing;

	SmoothFloat Angle;
	SmoothFloat Roundness;

	SmoothInt Alternating;
	SmoothInt CutOff;
} SmoothGripObjects;

GE_INTERNAL void 
do_smooth_draw_broken_bars(SmoothCanvas *Canvas,

			SmoothColor Highlight,
			SmoothColor Shadow,
			SmoothColor MidPoint,

			SmoothInt X, 
			SmoothInt Y, 
			SmoothInt Width, 
			SmoothInt Height, 

			SmoothBool Horizontal);

GE_INTERNAL void
do_smooth_draw_lines(SmoothGripObjects *GripObjects,
		SmoothCanvas *Canvas,
		SmoothColor Highlight,
		SmoothColor Shadow,
		SmoothInt X, 
		SmoothInt Y, 
		SmoothInt Width, 
		SmoothInt Height, 
		SmoothBool Horizontal);

GE_INTERNAL void
do_smooth_draw_dots(SmoothGripObjects *GripObjects,
		SmoothCanvas *Canvas,
		SmoothColor Highlight,
		SmoothColor Shadow,
		SmoothColor MidPoint,
		SmoothInt X, 
		SmoothInt Y, 
		SmoothInt Width, 
		SmoothInt Height, 
		SmoothBool Horizontal);

GE_INTERNAL void 
do_smooth_draw_buds(SmoothCanvas *Canvas,
		SmoothColor Highlight,
		SmoothColor Shadow,
		SmoothColor MidPoint,
		SmoothInt X, 
		SmoothInt Y, 
		SmoothInt Width, 
		SmoothInt Height, 
		SmoothInt ClipOffset,
		SmoothBool Horizontal,
		SmoothBool Alternating);
		
#endif /* DRAW_GRIPS_H */
