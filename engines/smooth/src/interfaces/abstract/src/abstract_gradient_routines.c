/******************************************************************************/
/* abstract_gradient_routines.c - Default Gradient Functions                  */
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
/* Portions Based on the IceGradient Engine                                   */
/*   Tim Gerla <timg@means.net>                                               */
/*   Tomas Ögren <stric@ing.umu.se>                                           */
/*   JM Perez <jose.perez@upcnet.es>                                          */
/******************************************************************************/
#include <math.h>
/*
 * Explanation of the quadratic gradient formula:
 * y|
 *  |\        /        If you use linear gradients in buttons the text may be difficult to read because
 *  | \      /         the color range of the background is too wide. The idea is to make the range
 *  |  \    /          shorter in the center where the text will be. An easy way to do it would be
 *  !   -__-           to set the relative distances between steps corresponding to a something like
 * -+----------|-x                            y = (x - w/2)² for x in [0,w]
 *  |          w       where w is the number of steps in the gradient.
 *                     At step w you'll have the acumulated sum of all steps and thus
 *                                   int(y, 0, w) = w³/12
 * At each step you would get
 *                               int(y) = x³/3 - wx²/2 + w²x/4
 * If you want a coefficient in [0,1]
 *                      int(y) / int(y, 0, w) = 4x³/w³ - 6x²/w² + 3x/w =
 *                                            = ax³ + bx² + cx
*/
static void 
SmoothAllocateGradientStepColor(SmoothCanvas *Canvas,
				SmoothColor FromColor, 
				SmoothColor ToColor,
				SmoothInt StepPosition,
				SmoothInt NumberOfSteps,
				SmoothBool QuadraticGradientRange,
				SmoothColor *StepColor)
{
	SmoothDouble delta, tmp_from, tmp_to;

	if (QuadraticGradientRange && (NumberOfSteps > 3)) 
	{
		/* delta = ax³ + bx² + cx */ 
		delta = (4 * pow(StepPosition, 3) / pow(NumberOfSteps, 3)) - 
			(6 * pow(StepPosition, 2) / pow(NumberOfSteps, 2)) + 
			(3 * pow(StepPosition, 1) / pow(NumberOfSteps, 1));
	} 
	else 
	{
		/* delta = i/steps */     

		delta = ((SmoothFloat)StepPosition/(SmoothFloat)NumberOfSteps);
	}	

	SmoothColorGetRedValue(&FromColor, &tmp_from);
	SmoothColorGetRedValue(&ToColor, &tmp_to);
	SmoothColorSetRedValue(StepColor, tmp_from + (SmoothFloat)((tmp_to- tmp_from)*delta));

	SmoothColorGetGreenValue(&FromColor, &tmp_from);
	SmoothColorGetGreenValue(&ToColor, &tmp_to);
	SmoothColorSetGreenValue(StepColor, tmp_from + (SmoothFloat)((tmp_to - tmp_from)*delta));

	SmoothColorGetBlueValue(&FromColor, &tmp_from);
	SmoothColorGetBlueValue(&ToColor, &tmp_to);
	SmoothColorSetBlueValue(StepColor, tmp_from + (SmoothFloat)((tmp_to - tmp_from)*delta));

	SmoothColorGetAlphaValue(&FromColor, &tmp_from);
	SmoothColorGetAlphaValue(&ToColor, &tmp_to);
	SmoothColorSetAlphaValue(StepColor, tmp_from + (SmoothFloat)((tmp_to - tmp_from)*delta));

	SmoothCanvasCacheColor(Canvas, StepColor);  
}

/* Most of the gradient functions were taken originaly from the 
 * IceGradient & EnGradient engines, however, after taking a 
 * look at the sources more closely I realized that 600+ lines
 * were a waste of repetetive code in multiple routines, so I merged
 * the bulk of the code into one major routine: gdk_draw_gradient,
 * which was then broken out into alloc_gradient_color to take care of
 * several repetitions of that. I then took style specific code from
 * EnGradient and converted into style_draw_gradient, which simply
 * called to gdk_draw_gradient for the real work. With the Interfaces
 * rewrite it was again simplified to what you see here.
 *
 * It still needs more work, and better support for multiple gradients.
 */
static SmoothBool
SmoothAbstractCanvasRenderGradient(SmoothCanvas *Canvas,
				SmoothGradient Gradient,
				SmoothInt X,
				SmoothInt Y,
				SmoothInt Width,
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	SmoothBool horizontal = (Gradient.Type == SMOOTH_GRADIENT_HORIZONTAL);
	SmoothBool northern   = (Gradient.Type == SMOOTH_GRADIENT_NORTHERN_DIAGONAL);
	SmoothBool diagonal   = ((Gradient.Type == SMOOTH_GRADIENT_NORTHERN_DIAGONAL) || (Gradient.Type == SMOOTH_GRADIENT_SOUTHERN_DIAGONAL));

	SmoothRectangle clip;
	SmoothColor color;
	SmoothInt i, steps=0;
	
	if (diagonal)
	{
		steps = Width + Height - 1;
	}	
	else 
	{
		if (horizontal)
		{
			steps = Width; 
		}	
		else
		{
			steps = Height;
		}
	}	
   
	if (SmoothRectangleSetValues(&clip, X, Y, Width, Height)
		&& SmoothCanvasClipUseIntersectingRectangle(Canvas, clip))
	{		
		for(i=0; i < steps; i++) 
		{
			/* this whole thing needs smarter logic to eg determine if multiple steps use the same
			 * colour and if so just use fill rect for that area and skip ahead
			 */
			SmoothAllocateGradientStepColor(Canvas, Gradient.From, Gradient.To, i, steps, Gradient.QuadraticGradientRange, &color);
			if (SmoothCanvasSetPenColor(Canvas, color))
			{
				if (diagonal) 
				{
					if (northern)
					{
						SmoothCanvasDrawLine(Canvas, X + i, Y, X, Y + i);
					}	
					else
					{
						SmoothCanvasDrawLine(Canvas, X + Width - 1 - i, Y, X + Width - 1, Y + i);
					}	
				} 
				else 
				{
					if (horizontal)
					{
						SmoothCanvasDrawLine(Canvas, X+i, Y, X+i, Y+Height);
					}	
					else
					{
						SmoothCanvasDrawLine(Canvas, X, Y+i, X+Width, Y+i);
					}	
				}
			}
			SmoothCanvasUnCacheColor(Canvas, &color);  
		}

		SmoothCanvasClearClipRectangle(Canvas);
		result = SmoothTrue;
	}	
	
	return result;
}
