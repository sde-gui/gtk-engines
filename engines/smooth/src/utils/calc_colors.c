/******************************************************************************/
/* calc_colors.c - Engine/Backend Agnostic Color Shading Functions            */
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

/******************************************************************************/
/* Description and Glossary of Terms                                          */
/******************************************************************************/
/* The following are color routines - they are GUI agnostic,	              */ 
/*                                                                            */
/* They are basically just converting RGB values into HSB, or the reverse     */ 
/* i.e. HSB to RGB. Values are passed and converted as a Double               */
/* (floating point) where each value can be 0.0-1.0, representing 	      */
/* a percentage range of none to full.	                                      */
/*                                                                            */
/* Explanation of terms:					              */
/* 	RGB: Red Green and Blue, the color scheme where each	              */
/*		color is broken up into the amount of red 	              */
/*		green and blue values which can be combined to	              */
/*		create it. The primary history for this scheme	              */
/*		is CRT(cathode ray tube) monitors, where each 	              */
/*		pixel on screens color is created by using	              */
/*		3 electron guns, one for each R, G, & B value.	              */
/*								              */		
/* 	HSB: Hue Saturation and Brightness(or Brilliance)	              */
/*		an alternate way of expressing values in the	              */
/*		RGB color scheme, a representation of the	              */
/*		different aspects of the scheme in terms of	              */
/*		a light source. Also known as HSV for Hue	              */
/*		Saturation and Value.				              */
/*								              */
/*	Hue: wavelength in the visible light spectrum at which	              */
/*		the energy output from a source is greatest.	              */
/*		This cooresponds to the more traditional method	              */
/*		of determining color, i.e. every wavelength 	              */
/*		represents a specific type of color "red"	              */
/*		"orange" "violet" etc, on the spectrum, the	              */
/*		traditional rainbow as it were, as expressed	              */
/*		on a color wheel.				              */
/*								              */
/*	Saturation: relative bandwidth of the visible output	              */
/*		from a light source. This is essentially 	              */
/*		relative to the total amount of "white" light	              */
/*		or purity of the color, the less saturation	              */
/*		the more white, or "washed out" the color, 	              */
/*		eg. - a "baby" or "powder" blue has a lower 	              */
/*		saturation then say a "navy" blue.		              */
/*								              */
/*	Brightness: intensity of the energy output of the light               */
/*		source.	Or in other words the total amount of	              */
/*		light in the color. So 100% brightness is 	              */
/*		white and 0% is black, and varying levels in	              */
/*		between mean the color is lighter or darker.	              */
/*								              */
/*	Achromatic: when a color as represented in HSB only  	              */
/*		has a Brightness value, i.e. Hue and saturation	              */
/*		are 0. This corresponds to the RGB values where               */
/*		Red == Green == Blue. Also known as Grayscale.	              */
/******************************************************************************/ 
#include "smooth_parts.h"

/****************************************************************/ 
/* color_get_hsb_from_rgb originated in GTK+(rgb_to_hls)	*/
/*								*/
/* Modifications -						*/
/*	1.)Spacing/coding style has been changed.		*/
/*	2.)Comments/Explanations added				*/
/*	3.)Names changed to normal standards			*/
/*	4.)Code modified to use MAX/MIN macros for readability.	*/
/****************************************************************/ 
static void
color_get_hsb_from_rgb (SmoothDouble Red, 
				SmoothDouble Green, 
				SmoothDouble Blue,
				
				SmoothDouble *Hue, 
				SmoothDouble *Saturation,
				SmoothDouble *Brightness) 
{
	SmoothDouble min, max, delta;
    
	/****************************************************************/
	/* Find the min and max color shades.				*/
	/****************************************************************/
	if (Red > Green)
	{
		max = MAX(Red, Blue);
		min = MIN(Green, Blue);      
	}
	else
	{
		max = MAX(Green, Blue);
		min = MIN(Red, Blue);      
	}
  
	/****************************************************************/
	/* The Brightness value of a color is always equal to the	*/
	/* average difference of the color values Red, Green, and Blue.	*/
	/* This can be determined by (R + G + B)/3 which can also be	*/
	/* equated with (min(r,g,b) + max(r,g,b))/2, which we use since */
	/* we need to know the min and max for saturation anyway.	*/
	/* 								*/
	/* eg. :							*/
	/* 								*/
	/* if (Red == 0.5) && (Green == 0.2) && (Blue == 0.8), the max	*/
	/* value is 0.8(blue), and the min value is 0.2(green),	thus	*/
	/* Brightness is the average (0.8 + 0.2)/2 = 0.5		*/
	/****************************************************************/
	*Brightness = (max + min) / 2;
	
	if (max == min)
	{
		/****************************************************************/
		/* The Hue and Saturation of the color are set to 0, because 	*/
		/* if the max and min values are equal, the color is Achromatic.*/
		/****************************************************************/
		*Hue = 0;
		*Saturation = 0;
	}	
  	else
	{
		if (*Brightness <= 0.5)
		{
			*Saturation = (max - min) / (max + min);
		}	
		else
		{
			*Saturation = (max - min) / (2 - max - min);
		}	
      
		delta = max -min;
		if (Red == max)
		{
			*Hue = (Green - Blue) / delta;
		}	
		else 
		{
			if (Green == max)
			{
				*Hue = 2 + (Blue - Red) / delta;
			}	
			else
			{ 
				if (Blue == max)
				{
					*Hue = 4 + (Red - Green) / delta;
				}	
			}		
		}
		*Hue *= 60;
		if (*Hue < 0.0)
		{
			*Hue += 360;
		}	
	}
}

/****************************************************************/ 
/* color_get_rgb_from_hls originated in GTK+(hls_to_rgb)	*/
/*								*/
/* Modifications -						*/
/*	1.)Spacing/coding style has been changed.		*/
/*	2.)Comments/Explanations added				*/
/*	3.)Names changed to normal standards			*/
/*	4.)Reworked code to use MOD, tables and for loop, both	*/
/*		for readability purposes and to remove the	*/
/*		insane and potentially deadly 'while', which	*/
/* 		should prevent lockups with uninitialized	*/
/*		values, etc.					*/
/****************************************************************/ 
#define MODULA(number, divisor) (((SmoothInt)number % divisor) + (number - (SmoothInt)number));
static void
color_get_rgb_from_hsb (SmoothDouble Hue, 
				SmoothDouble Saturation,
				SmoothDouble Brightness, 
			
				SmoothDouble *Red, 
				SmoothDouble *Green, 
				SmoothDouble *Blue)
{
	SmoothInt i;
	SmoothDouble hue_shift[3], color_shift[3];
	SmoothDouble m1, m2, m3;
	  
	if (Brightness <= 0.5)
	{
		m2 = Brightness * (1 + Saturation);
	}	
	else
	{
		m2 = Brightness + Saturation - Brightness * Saturation;
	}	
	m1 = 2 * Brightness - m2;

	/****************************************************************/
	/* Initialize Hue Shift values					*/
	/****************************************************************/
	hue_shift[0] = Hue + 120;
	hue_shift[1] = Hue;
	hue_shift[2] = Hue - 120;

	/****************************************************************/
	/* Initialize Color Shift Values to Achromatic Defaults.	*/
	/****************************************************************/
	color_shift[0] = Brightness;
	color_shift[1] = Brightness;
	color_shift[2] = Brightness;	

	/****************************************************************/
	/* If Saturation is 0 then Color is Achromatic, so skip out	*/
	/****************************************************************/
	i = (Saturation == 0)?3:0;

	/****************************************************************/
	/* Loop hue_shift's to find the corresponing color_shift	*/
	/****************************************************************/
	for (; i < 3; i++)
	{
		/*************************************************************/
		/* Envisioning the hue range as a wheel, we see the Hue as   */
		/* an angle, in degrees, where any Hue value may be expressed*/
		/* as a value between 0 and 360. So here we ensure this range*/
		/* when Hue > 360,  or Hue < 0				     */
		/*************************************************************/
		m3 = hue_shift[i];
	
		if (m3 > 360)
		{
			m3 = MODULA(m3, 360);
		}
		else
		{
			if (m3 < 0)
			{
				m3 = 360 - MODULA(ABS(m3), 360);
			}
		}

		/****************************************************************/
		/* Determine corresponding color to Hue Angle on the wheel.	*/
		/****************************************************************/
		if (m3 < 60)
		{
			color_shift[i] = m1 + (m2 - m1) * m3 / 60;
		}
		else 
		{
			if (m3 < 180)
			{
				color_shift[i] = m2;
			}	
			else 
			{
				if (m3 < 240)
				{
					color_shift[i] = m1 + (m2 - m1) * (240 - m3) / 60;
				}	
				else
				{
					color_shift[i] = m1;
				}
			}
		}
	}
	
	/****************************************************************/
	/* Set Red, Green, and Blue to final color_shift values 	*/
	/****************************************************************/
	*Red = color_shift[0];
	*Green = color_shift[1];
	*Blue = color_shift[2];	
}

void
SmoothCompositeColorShade(SmoothColor *Original, 
			  SmoothDouble ShadeRatio,
			  SmoothColor *Composite)
{
	SmoothDouble red=0;
	SmoothDouble green=0;
	SmoothDouble blue=0;

	SmoothDouble hue = 0;
	SmoothDouble saturation = 0;
	SmoothDouble brightness = 0;

	SmoothColorGetRedValue(Original, &red);
	SmoothColorGetGreenValue(Original, &green);
	SmoothColorGetBlueValue(Original, &blue);

	color_get_hsb_from_rgb (red, green, blue, &hue, &saturation, &brightness);

	brightness = MIN(brightness*ShadeRatio, 1.0);
	brightness = MAX(brightness, 0.0);
  
	saturation = MIN(saturation*ShadeRatio, 1.0);
	saturation = MAX(saturation, 0.0);
  
	color_get_rgb_from_hsb (hue, saturation, brightness, &red, &green, &blue);
 
	SmoothColorSetRedValue(Composite, red);
	SmoothColorSetGreenValue(Composite, green);
	SmoothColorSetBlueValue(Composite, blue);
}
