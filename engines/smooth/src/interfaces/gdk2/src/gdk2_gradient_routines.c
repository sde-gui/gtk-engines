/******************************************************************************/
/* gdk2_gradient_routines.c - GDK2 Gradient Functions                         */
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
/*                                                                            */
/* Portions Based on Metacity                                                 */
/* gradient.c -                                                               */
/*   Havoc Pennington <hp@redhat.com>                                         */
/*     99% copied from wrlib in WindowMaker,                                  */
/*       Copyright (C) 1997-2000 Dan Pascu and Alfredo Kojima                 */
/******************************************************************************/
static void 
internal_color_get_as_uchars(SmoothColor Color, 
				SmoothUChar *Red, 
				SmoothUChar *Green, 
				SmoothUChar *Blue, 
				SmoothUChar *Alpha)
{
	SmoothDouble r, g, b, a;
	
	SmoothColorGetValues(&Color, &r, &g, &b, &a);
	
	*Red = (SmoothUChar) (Color.RGB.red / 256.0);
	*Green = (SmoothUChar) (Color.RGB.green / 256.0);
	*Blue = (SmoothUChar) (Color.RGB.blue / 256.0);
	*Alpha = (SmoothUChar) (Color.Alpha / 256.0);
}				

static SmoothImageBuffer
internal_create_horizontal_gradient_image_buffer (SmoothInt Width, SmoothInt Height,
							SmoothColor From,
							SmoothColor To,
							SmoothBool QuadraticGradientRange)
{    
	int i;
	long r, g, b, a, dr, dg, db, da;
	SmoothImageBuffer buffer;
	SmoothUChar *ptr;
	SmoothUChar *pixels;
	SmoothUChar r0, g0, b0, a0;
	SmoothUChar rf, gf, bf, af;
	int rowstride;

	buffer = internal_image_buffer_new (Width, Height);
	if (buffer == NULL)
		return NULL;
    
	pixels = gdk_pixbuf_get_pixels (buffer);
	ptr = pixels;
	rowstride = gdk_pixbuf_get_rowstride (buffer);
  
	internal_color_get_as_uchars(From, &r0, &g0, &b0, &a0);
	internal_color_get_as_uchars(To, &rf, &gf, &bf, &af);
  
	r = r0 << 16;
	g = g0 << 16;
	b = b0 << 16;
	a = a0 << 16;
    
	dr = ((rf-r0)<<16)/(int)Width;
	dg = ((gf-g0)<<16)/(int)Width;
	db = ((bf-b0)<<16)/(int)Width;
	da = ((af-a0)<<16)/(int)Width;

	/* render the first line */
	for (i=0; i<Width; i++)
	{
		*(ptr++) = (SmoothUChar)(r>>16);
		*(ptr++) = (SmoothUChar)(g>>16);
		*(ptr++) = (SmoothUChar)(b>>16);
/*		*(ptr++) = (SmoothUChar)(a>>16);*/

		if (QuadraticGradientRange) 
		{
			SmoothDouble delta, tmp_a, tmp_b, tmp_c;

			tmp_a = 4.0 / pow(Width, 3);
			tmp_b = -6.0 / pow(Width, 2);
			tmp_c = 3.0 / Width;
   
			/* delta = ax³ + bx² + cx */ 
			delta = (tmp_a * pow(i, 3)) + (tmp_b * pow(i, 2)) + (tmp_c * i);

			r = (SmoothUChar)(r0 + ((rf - r0) * delta)) << 16;
			g = (SmoothUChar)(g0 + ((gf - g0) * delta)) << 16;
			b = (SmoothUChar)(b0 + ((bf - b0) * delta)) << 16;
			a = (SmoothUChar)(a0 + ((af - a0) * delta)) << 16;
		} 
		else
		{
			r += dr;
			g += dg;
			b += db;
			a += da;
		}
	}

	/* copy the first line to the other lines */
	for (i=1; i<Height; i++)
	{
		memcpy (&(pixels[i*rowstride]), pixels, rowstride);
	}
    
	return buffer;
}

static SmoothImageBuffer
internal_create_vertical_gradient_image_buffer (SmoothInt Width, SmoothInt Height,
						SmoothColor From,
						SmoothColor To,
						SmoothBool QuadraticGradientRange)
{
	SmoothInt i, j, max_block, last_block;
	long r, g, b, a, dr, dg, db, da;
	SmoothImageBuffer buffer;
	
	SmoothUChar *ptr;
	SmoothUChar point[4];

	SmoothUChar r0, g0, b0, a0;
	SmoothUChar rf, gf, bf, af;

	SmoothInt rowstride;
	SmoothUChar *pixels;
  
	buffer = internal_image_buffer_new (Width, Height);
	if (buffer == NULL)
		return NULL;
    
	pixels = gdk_pixbuf_get_pixels (buffer);
	rowstride = gdk_pixbuf_get_rowstride (buffer);
  
	internal_color_get_as_uchars(From, &r0, &g0, &b0, &a0);
	internal_color_get_as_uchars(To, &rf, &gf, &bf, &af);

	r = r0<<16;
	g = g0<<16;
	b = b0<<16;
	a = a0<<16;

	dr = ((rf-r0)<<16)/(int)Height;
	dg = ((gf-g0)<<16)/(int)Height;
	db = ((bf-b0)<<16)/(int)Height;
	da = ((af-a0)<<16)/(int)Height;

	max_block = Width/2;

	for (i=0; i<Height; i++)
	{
		ptr = pixels + i * rowstride;
      
		ptr[0] = r>>16;
		ptr[1] = g>>16;
		ptr[2] = b>>16;
		
		if (Width > 1)
		{
			last_block = 0;

			for (j=1; j <= max_block; j *= 2)
			{
				memcpy (&(ptr[j*3]), ptr, j*3);

				if ((j*2) >= max_block)
				{
					last_block = j*2;
				}
			}

			if ((last_block < Width) && (last_block > 0))
			{
				memcpy (&(ptr[last_block*3]), ptr, (Width - last_block)*3);
			}
		}

		if (QuadraticGradientRange) 
		{
			SmoothDouble delta, tmp_a, tmp_b, tmp_c;

			tmp_a = 4.0 / pow(Height, 3);
			tmp_b = -6.0 / pow(Height, 2);
			tmp_c = 3.0 / Height;
   
			/* delta = ax³ + bx² + cx */ 
			delta = (tmp_a * pow(i, 3)) + (tmp_b * pow(i, 2)) + (tmp_c * i);

			r = (SmoothUChar)(r0 + ((rf - r0) * delta)) << 16;
			g = (SmoothUChar)(g0 + ((gf - g0) * delta)) << 16;
			b = (SmoothUChar)(b0 + ((bf - b0) * delta)) << 16;
			a = (SmoothUChar)(a0 + ((af - a0) * delta)) << 16;
		} 
		else
		{
			r += dr;
			g += dg;
			b += db;
			a += da;
		}
	}
	
	return buffer;
}

static void
internal_destroy_image_buffer(SmoothImageBuffer *ImageBuffer)
{
	g_object_unref(*ImageBuffer);
	*ImageBuffer = NULL;
}

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

	if (QuadraticGradientRange) 
	{
		SmoothFloat a, b, c;

		a = 4.0 / pow(NumberOfSteps, 3);
		b = -6.0 / pow(NumberOfSteps, 2);
		c = 3.0 / NumberOfSteps;
   
		/* delta = ax³ + bx² + cx */ 
		delta = (a * pow(StepPosition, 3)) + (b * pow(StepPosition, 2)) + (c * StepPosition);
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

static SmoothBool
GDK2CanvasRenderGradient(SmoothCanvas Canvas,
				SmoothGradient Gradient,
				SmoothInt X,
				SmoothInt Y,
				SmoothInt Width,
				SmoothInt Height)

{	
	SmoothGDKPrivateCanvas *GDKPrivateCanvas = Canvas;

	SmoothBool result = SmoothFalse;
	SmoothRectangle clip;
	
	SmoothBool horizontal = (Gradient.Type == SMOOTH_GRADIENT_HORIZONTAL);
	SmoothBool northern   = (Gradient.Type == SMOOTH_GRADIENT_NORTHERN_DIAGONAL);
	SmoothBool diagonal   = ((Gradient.Type == SMOOTH_GRADIENT_NORTHERN_DIAGONAL) || (Gradient.Type == SMOOTH_GRADIENT_SOUTHERN_DIAGONAL));
	
	SmoothInt depth = GDKPrivateCanvas->Depth;
	SmoothInt ditherdepth = GDKPrivateCanvas->DitherDepth;
	
	#ifdef ALWAYSDITHER
	SmoothBool dither = (!diagonal);
	#else
	SmoothBool dither = (((depth > 0) && (depth <= ditherdepth)) && (!diagonal));
	#endif
	
	if (SmoothRectangleSetValues(&clip, X, Y, Width, Height)
		&& SmoothCanvasClipUseIntersectingRectangle(Canvas, clip))
	{		
		if (dither)
		{
			if (diagonal)
			{
			/* need to write a new internal_create_diagonal_gradient_image_buffer */
			}
			else
			{
				SmoothImageBuffer ImageBuffer = NULL;
				
				if (horizontal)
					ImageBuffer = internal_create_horizontal_gradient_image_buffer (Width, Height, Gradient.From, Gradient.To, Gradient.QuadraticGradientRange);
				else	
					ImageBuffer = internal_create_vertical_gradient_image_buffer (Width, Height, Gradient.From, Gradient.To, Gradient.QuadraticGradientRange);
	
				if (ImageBuffer)
				{
					SmoothCanvasRenderImageBuffer(Canvas, ImageBuffer, X, Y, Width, Height);
					internal_destroy_image_buffer(&ImageBuffer);
				}	
					
			}
		}
		else
		{
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
		}
		
		SmoothCanvasClearClipRectangle(Canvas);
		result = SmoothTrue;
	}	

	return result;	
}
