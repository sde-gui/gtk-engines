/******************************************************************************/
/* abstract_drawing_interface.c - Abstract Drawing Interface                  */
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
#include <interfaces/drawing_interface.h>
#include "abstract_drawing_interface.h"

/* Declare Abstract Interface Functions */

/* Color Get/Set Values */
#ifndef SMOOTHCOLOR
static SmoothBool SmoothAbstractColorGetRedValue(SmoothColor *_Color,
						SmoothDouble *RedValue);
static SmoothBool SmoothAbstractColorSetRedValue(SmoothColor *_Color, 
						SmoothDouble NewValue);

static SmoothBool SmoothAbstractColorGetGreenValue(SmoothColor *_Color,
						SmoothDouble *GreenValue);
static SmoothBool SmoothAbstractColorSetGreenValue(SmoothColor *_Color, 
						SmoothDouble NewValue);

static SmoothBool SmoothAbstractColorGetBlueValue(SmoothColor *_Color,
						SmoothDouble *BlueValue);
static SmoothBool SmoothAbstractColorSetBlueValue(SmoothColor *_Color,
						SmoothDouble NewValue);

static SmoothBool SmoothAbstractColorGetAlphaValue(SmoothColor *_Color,
						SmoothDouble *AlphaValue);
static SmoothBool SmoothAbstractColorSetAlphaValue(SmoothColor *_Color, 
						SmoothDouble NewValue);
#endif

static SmoothBool 
SmoothAbstractColorGetValues(SmoothColor *Color, 
				SmoothDouble *RedValue, 
				SmoothDouble *GreenValue, 
				SmoothDouble *BlueValue, 
				SmoothDouble *AlphaValue);

static SmoothBool 
SmoothAbstractColorSetValues(SmoothColor *Color, 
				SmoothDouble RedValue, 
				SmoothDouble GreenValue, 
				SmoothDouble BlueValue, 
				SmoothDouble AlphaValue);

/* Rectangle Get/Set Values */
static SmoothBool 
SmoothAbstractRectangleGetValues(SmoothRectangle *Rectangle, 
				SmoothInt *XValue, 
				SmoothInt *YValue, 
				SmoothInt *WidthValue, 
				SmoothInt *HeightValue);


static SmoothBool 
SmoothAbstractRectangleSetValues(SmoothRectangle *Rectangle, 
				SmoothInt XValue, 
				SmoothInt YValue, 
				SmoothInt WidthValue, 
				SmoothInt HeightValue);

/* Point Get/Set Values */
static SmoothBool 
SmoothAbstractPointGetValues(SmoothPoint *Point, 
				SmoothInt *XValue, 
				SmoothInt *YValue);

static SmoothBool 
SmoothAbstractPointSetValues(SmoothPoint *Point, 
				SmoothInt XValue, 
				SmoothInt YValue);

/* Canvas Get/Set Clip Rectangle */
static SmoothBool
SmoothAbstractCanvasClipUseIntersectingRectangle(SmoothCanvas Canvas,
						SmoothRectangle Rectangle);

/* Canvas Get/Set Pen Properties */
static SmoothBool 
SmoothAbstractCanvasGetPenValues(SmoothCanvas Canvas,
					SmoothColor *Color,
					SmoothInt *Thickness,
					SmoothLineStyle *Style,
					SmoothLineCap *Cap,
					SmoothLineJoin *Join);

static SmoothBool 
SmoothAbstractCanvasSetPenValues(SmoothCanvas Canvas,
					SmoothColor Color,
					SmoothInt Thickness,
					SmoothLineStyle Style,
					SmoothLineCap Cap,
					SmoothLineJoin Join);

/* Abstract Tile Routines */
#ifndef SMOOTHTILE
#include "src/abstract_tile_routines.h"
#endif

/* Abstract Gradient Routines */
#ifndef SMOOTHGRADIENT
#include "src/abstract_gradient_routines.h"
#endif

/* Initialize smoothDrawingInterface */
SmoothBool 
SmoothAbstractDrawingInterfaceInitialize(DrawingInterfaceClass *_SmoothDrawingInterface)
{
	/* Color Get/Set Values */
	_SmoothDrawingInterface->ColorGetRedValue = (void *) NULL;
	_SmoothDrawingInterface->ColorSetRedValue = (void *) NULL;

	_SmoothDrawingInterface->ColorGetGreenValue = (void *) NULL;
	_SmoothDrawingInterface->ColorSetGreenValue = (void *) NULL;

	_SmoothDrawingInterface->ColorGetBlueValue = (void *) NULL;
	_SmoothDrawingInterface->ColorSetBlueValue = (void *) NULL;

	_SmoothDrawingInterface->ColorGetAlphaValue = (void *) NULL;
	_SmoothDrawingInterface->ColorSetAlphaValue = (void *) NULL;

	_SmoothDrawingInterface->ColorGetValues = SmoothAbstractColorGetValues;
	_SmoothDrawingInterface->ColorSetValues = SmoothAbstractColorSetValues;

	/* Rectangle Status Functions */
	_SmoothDrawingInterface->RectangleCreate = (void *) NULL;
	_SmoothDrawingInterface->RectangleDestroy = (void *) NULL;
	_SmoothDrawingInterface->RectangleIsValid = (void *) NULL;

	/* Rectangle Get/Set Values */
	_SmoothDrawingInterface->RectangleGetXValue = (void *) NULL;
	_SmoothDrawingInterface->RectangleSetXValue = (void *) NULL;

	_SmoothDrawingInterface->RectangleGetYValue = (void *) NULL;
	_SmoothDrawingInterface->RectangleSetYValue = (void *) NULL;

	_SmoothDrawingInterface->RectangleGetWidthValue = (void *) NULL;
	_SmoothDrawingInterface->RectangleSetWidthValue = (void *) NULL;

	_SmoothDrawingInterface->RectangleGetHeightValue = (void *) NULL;
	_SmoothDrawingInterface->RectangleSetHeightValue = (void *) NULL;

	_SmoothDrawingInterface->RectangleGetValues = SmoothAbstractRectangleGetValues;	
	_SmoothDrawingInterface->RectangleSetValues = SmoothAbstractRectangleSetValues;

	/* Rectangle Misc Functions */
	_SmoothDrawingInterface->RectangleFindIntersection = (void *) NULL;

	/* Point Get/Set Values */
	_SmoothDrawingInterface->PointGetXValue = (void *) NULL;
	_SmoothDrawingInterface->PointSetXValue = (void *) NULL;

	_SmoothDrawingInterface->PointGetYValue = (void *) NULL;
	_SmoothDrawingInterface->PointSetYValue = (void *) NULL;

	_SmoothDrawingInterface->PointGetValues = SmoothAbstractPointGetValues;
	_SmoothDrawingInterface->PointSetValues = SmoothAbstractPointSetValues;

	/* Image/Buffer By File name (For caching purposes) */
	_SmoothDrawingInterface->ImageBufferLoadByFile = (void *) NULL;
	_SmoothDrawingInterface->ImageBufferUnLoadByFile = (void *) NULL;

	/* Canvas Get Image/ImageBuffer Size */
	_SmoothDrawingInterface->ImageBufferGetWidth = (void *) NULL;
	_SmoothDrawingInterface->ImageBufferGetHeight = (void *) NULL;
	_SmoothDrawingInterface->ImageGetWidth = (void *) NULL;
	_SmoothDrawingInterface->ImageGetHeight = (void *) NULL;

	/* Canvas Map/Free A Color From The Colormap*/
	_SmoothDrawingInterface->CanvasAllocateColor = (void *) NULL;
	_SmoothDrawingInterface->CanvasDeallocateColor = (void *) NULL;

	/* Canvas cache/uncache a Color */
	_SmoothDrawingInterface->CanvasCacheColor = (void *) NULL;
	_SmoothDrawingInterface->CanvasUnCacheColor = (void *) NULL;
			
	/* Canvas cache/uncache a shaded Color */
	_SmoothDrawingInterface->CanvasCacheShadedColor = (void *) NULL;
	_SmoothDrawingInterface->CanvasUnCacheShadedColor = (void *) NULL;
	
	/* Canvas cache/uncache a midpoint Color */
	_SmoothDrawingInterface->CanvasCacheMidPointColor = (void *) NULL;
	_SmoothDrawingInterface->CanvasUnCacheMidPointColor = (void *) NULL;

	/* Canvas Store/Render */
	_SmoothDrawingInterface->CanvasStore =  (void *) NULL;
	_SmoothDrawingInterface->CanvasRender =  (void *) NULL;

	/* Canvas Get/Set Clip Rectangle */
	_SmoothDrawingInterface->CanvasGetClipRectangle = (void *) NULL;
	_SmoothDrawingInterface->CanvasSetClipRectangle = (void *) NULL;
	_SmoothDrawingInterface->CanvasClearClipRectangle = (void *) NULL;
	_SmoothDrawingInterface->CanvasClipUseIntersectingRectangle = (void *)SmoothAbstractCanvasClipUseIntersectingRectangle;

	/* Canvas Get/Set Pen Properties */
	_SmoothDrawingInterface->CanvasGetPenColor = (void *) NULL;
	_SmoothDrawingInterface->CanvasSetPenColor = (void *) NULL;	
	
	_SmoothDrawingInterface->CanvasGetPenThickness = (void *) NULL;
	_SmoothDrawingInterface->CanvasSetPenThickness = (void *) NULL;
	
	_SmoothDrawingInterface->CanvasGetPenStyle = (void *) NULL;
	_SmoothDrawingInterface->CanvasSetPenStyle = (void *) NULL;
	
	_SmoothDrawingInterface->CanvasGetPenCap = (void *) NULL;
	_SmoothDrawingInterface->CanvasSetPenCap = (void *) NULL;
	
	_SmoothDrawingInterface->CanvasGetPenJoin = (void *) NULL;
	_SmoothDrawingInterface->CanvasSetPenJoin = (void *) NULL;
	
	_SmoothDrawingInterface->CanvasGetPenPattern = (void *) NULL;
	_SmoothDrawingInterface->CanvasSetPenPattern = (void *) NULL;

	_SmoothDrawingInterface->CanvasGetPenValues = (void *)SmoothAbstractCanvasGetPenValues;
	_SmoothDrawingInterface->CanvasSetPenValues = (void *)SmoothAbstractCanvasSetPenValues;

	/* Canvas Get/Set Brush Properties */
	_SmoothDrawingInterface->CanvasGetBrushColor = (void *) NULL;
	_SmoothDrawingInterface->CanvasSetBrushColor = (void *) NULL;	

	/* Canvas Create Image */
	_SmoothDrawingInterface->CanvasCreateImage = (void *) NULL;
	_SmoothDrawingInterface->CanvasCreateImageFromBuffer = (void *) NULL;
	_SmoothDrawingInterface->CanvasDestroyImage = (void *) NULL;

	/* Canvas Render Image/ImageBuffer */
	_SmoothDrawingInterface->CanvasRenderImage = (void *) NULL;						
	_SmoothDrawingInterface->CanvasRenderImageBuffer = (void *) NULL;						

	/* Canvas Render Tile */
	#ifndef SMOOTHTILE
	_SmoothDrawingInterface->CanvasRenderTile = (void *)SmoothAbstractCanvasRenderTile;
	#else
	_SmoothDrawingInterface->CanvasRenderTile = (void *)NULL;
	#endif

	/* Canvas Render Gradient */
	#ifndef SMOOTHGRADIENT
	_SmoothDrawingInterface->CanvasRenderGradient = (void *)SmoothAbstractCanvasRenderGradient;
	#else
	_SmoothDrawingInterface->CanvasRenderGradient = (void *)NULL;
	#endif

	/* Canvas Draw Point(s) */
	_SmoothDrawingInterface->CanvasDrawPoint = (void *) NULL;
	_SmoothDrawingInterface->CanvasDrawPoints = (void *) NULL;

	/* Canvas Draw Line(s) */
	_SmoothDrawingInterface->CanvasDrawLine = (void *) NULL;
	_SmoothDrawingInterface->CanvasDrawSegment = (void *) NULL;
	_SmoothDrawingInterface->CanvasDrawLines = (void *) NULL;
/*	_SmoothDrawingInterface->CanvasDrawSegments = (void *) NULL;*/
	
	/* Canvas Draw Arc(s)/Curve(s) */
	_SmoothDrawingInterface->CanvasDrawArc = (void *) NULL;

	/* Canvas Frame Shaped Areas */
	_SmoothDrawingInterface->CanvasFrameRectangle = (void *) NULL;
	_SmoothDrawingInterface->CanvasFramePolygon = (void *) NULL;
	_SmoothDrawingInterface->CanvasFrameChord = (void *) NULL;

	/* Canvas Fill Shaped Areas */
	_SmoothDrawingInterface->CanvasFillRectangle = (void *) NULL;
	_SmoothDrawingInterface->CanvasFillPolygon = (void *) NULL;
	_SmoothDrawingInterface->CanvasFillChord = (void *) NULL;
	
	return SmoothTrue;
}

SmoothBool 
SmoothAbstractDrawingInterfaceFinalize(DrawingInterfaceClass *_SmoothDrawingInterface)
{
	return SmoothTrue;
}

/* Implement Abstract Interface Functions */

/* Color Get/Set Values */
#ifndef SMOOTHCOLOR
static SmoothBool 
SmoothAbstractColorGetRedValue(SmoothColor *Color,
				SmoothDouble *RedValue)
{
	SmoothBool result = SmoothFalse;
	
	if (Color && RedValue)
	{
		*RedValue = (SmoothDouble)Color->Red;
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool 
SmoothAbstractColorSetRedValue(SmoothColor *Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if ((NewValue <= 1.0) && (NewValue >= 0.0))
	{
		(*Color).Red = NewValue;
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool 
SmoothAbstractColorGetGreenValue(SmoothColor *Color,
				SmoothDouble *GreenValue);
{
	SmoothBool result = SmoothFalse;
	
	if (Color && GreenValue)
	{
		*GreenValue = (SmoothDouble)Color->Green;
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool 
SmoothAbstractColorSetGreenValue(SmoothColor *Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if ((NewValue <= 1.0) && (NewValue >= 0.0))
	{
		(*Color).Green = NewValue;
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool 
SmoothAbstractColorGetBlueValue(SmoothColor *Color,
				SmoothDouble *BlueValue);
{
	SmoothBool result = SmoothFalse;
	
	if (Color && BlueValue)
	{
		*BlueValue = (SmoothDouble)Color->Blue;
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool 
SmoothAbstractColorSetBlueValue(SmoothColor *Color,
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if ((NewValue <= 1.0) && (NewValue >= 0.0))
	{
		(*Color).Blue = NewValue;
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool 
SmoothAbstractColorGetAlphaValue(SmoothColor *_Color,
				SmoothDouble *AlphaValue);
{
	SmoothBool result = SmoothFalse;
	
	if (Color && AlphaValue)
	{
		*AlphaValue = (SmoothDouble)Color->Alpha;
		result = SmoothTrue;
	}	
	return result;
}

static SmoothBool 
SmoothAbstractColorSetAlphaValue(SmoothColor *_Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if ((NewValue <= 1.0) && (NewValue >= 0.0))
	{
		(*Color).Alpha = NewValue;
		result = SmoothTrue;
	}	
	return result;
}

#endif

static SmoothBool 
SmoothAbstractColorGetValues(SmoothColor *Color, 
				SmoothDouble *RedValue, 
				SmoothDouble *GreenValue, 
				SmoothDouble *BlueValue, 
				SmoothDouble *AlphaValue)
{
	SmoothBool result = SmoothTrue;

	if (result && RedValue)
	{
		result = SmoothColorGetRedValue(Color, RedValue);
	}	

	if (result && GreenValue)
	{
		result = SmoothColorGetGreenValue(Color, GreenValue);
	}	

	if (result && BlueValue)
	{
		result = SmoothColorGetBlueValue(Color, BlueValue);
	}	

	if (result && AlphaValue)
	{
		result = SmoothColorGetAlphaValue(Color, AlphaValue);
	}	
	
	return result;
}

static SmoothBool 
SmoothAbstractColorSetValues(SmoothColor *Color, 
				SmoothDouble RedValue, 
				SmoothDouble GreenValue, 
				SmoothDouble BlueValue, 
				SmoothDouble AlphaValue)
{
	SmoothBool result = SmoothTrue;

	if (result)
	{
		result = SmoothColorSetRedValue(Color, RedValue);
	}	
	else
	{
		result = SmoothFalse;
	}

	if (result)
	{
		result = SmoothColorSetGreenValue(Color, GreenValue);
	}	
	else
	{
		result = SmoothFalse;
	}

	if (result)
	{
		result = SmoothColorSetBlueValue(Color, BlueValue);
	}	
	else
	{
		result = SmoothFalse;
	}

	if (result)
	{
		result = SmoothColorSetAlphaValue(Color, AlphaValue);
	}	
	else
	{
		result = SmoothFalse;
	}
	
	return result;
}

/* Rectangle Get/Set Values */
static SmoothBool 
SmoothAbstractRectangleGetValues(SmoothRectangle *Rectangle, 
				SmoothInt *XValue, 
				SmoothInt *YValue, 
				SmoothInt *WidthValue, 
				SmoothInt *HeightValue)
{
	SmoothBool result = SmoothTrue;

	if (result && XValue)
	{
		result = SmoothRectangleGetXValue(Rectangle, XValue);
	}	

	if (result && YValue)
	{
		result = SmoothRectangleGetYValue(Rectangle, YValue);
	}	

	if (result && WidthValue)
	{
		result = SmoothRectangleGetWidthValue(Rectangle, WidthValue);
	}	

	if (result && HeightValue)
	{
		result = SmoothRectangleGetHeightValue(Rectangle, HeightValue);
	}	
	
	return result;
}

static SmoothBool 
SmoothAbstractRectangleSetValues(SmoothRectangle *Rectangle, 
				SmoothInt XValue, 
				SmoothInt YValue, 
				SmoothInt WidthValue, 
				SmoothInt HeightValue)
{
	SmoothBool result = SmoothTrue;

	if (result)
	{
		result = SmoothRectangleSetXValue(Rectangle, XValue);
	}	
	else
	{
		result = SmoothFalse;
	}

	if (result)
	{
		result = SmoothRectangleSetYValue(Rectangle, YValue);
	}	
	else
	{
		result = SmoothFalse;
	}

	if (result)
	{
		result = SmoothRectangleSetWidthValue(Rectangle, WidthValue);
	}	
	else
	{
		result = SmoothFalse;
	}

	if (result)
	{
		result = SmoothRectangleSetHeightValue(Rectangle, HeightValue);
	}	
	else
	{
		result = SmoothFalse;
	}
	
	return result;
}

/* Point Get/Set Values */
static SmoothBool 
SmoothAbstractPointGetValues(SmoothPoint *Point, 
				SmoothInt *XValue, 
				SmoothInt *YValue)
{
	SmoothBool result = SmoothTrue;

	if (result && XValue)
	{
		result = SmoothPointGetXValue(Point, XValue);
	}	

	if (result && YValue)
	{
		result = SmoothPointGetYValue(Point, YValue);
	}	

	return result;
}

static SmoothBool 
SmoothAbstractPointSetValues(SmoothPoint *Point, 
				SmoothInt XValue, 
				SmoothInt YValue)
{
	SmoothBool result = SmoothTrue;

	if (result)
	{
		result = SmoothPointSetXValue(Point, XValue);
	}	
	else
	{
		result = SmoothFalse;
	}

	if (result)
	{
		result = SmoothPointSetYValue(Point, YValue);
	}	
	else
	{
		result = SmoothFalse;
	}
	
	return result;
}

/* Canvas Get/Set Clip Rectangle */
static SmoothBool
SmoothAbstractCanvasClipUseIntersectingRectangle(SmoothCanvas Canvas,
						SmoothRectangle Rectangle)
{
	SmoothBool result = SmoothTrue;
	SmoothRectangle area;
	
	if (SmoothCanvasGetClipRectangle(Canvas, &area)) 
	{
		SmoothRectangle tmp;       

		if (SmoothRectangleFindIntersection(area, Rectangle, &tmp))
		{
			SmoothCanvasSetClipRectangle(Canvas, tmp);
		}	
		else
		{
			result = SmoothFalse;
		}	
	} 
	else
	{
		SmoothCanvasSetClipRectangle(Canvas, Rectangle);
	}	
	
	return result;
}

/* Canvas Get/Set Pen Properties */
static SmoothBool 
SmoothAbstractCanvasGetPenValues(SmoothCanvas Canvas,
					SmoothColor *Color,
					SmoothInt *Thickness,
					SmoothLineStyle *Style,
					SmoothLineCap *Cap,
					SmoothLineJoin *Join)
{					
	SmoothBool result = SmoothTrue;

	if (result && Color)
	{
		result = SmoothCanvasGetPenColor(Canvas, Color);
	}	

	if (result && Thickness)
	{
		result = SmoothCanvasGetPenThickness(Canvas, Thickness);
	}	

	if (result && Style)
	{
		result = SmoothCanvasGetPenStyle(Canvas, Style);
	}	

	if (result && Cap)
	{
		result = SmoothCanvasGetPenCap(Canvas, Cap);
	}	

	if (result && Join)
	{
		result = SmoothCanvasGetPenJoin(Canvas, Join);
	}	

	return result;
}

static SmoothBool 
SmoothAbstractCanvasSetPenValues(SmoothCanvas Canvas,
					SmoothColor Color,
					SmoothInt Thickness,
					SmoothLineStyle Style,
					SmoothLineCap Cap,
					SmoothLineJoin Join)
{
	SmoothBool result = SmoothTrue;

	if (result)
	{
		result = SmoothCanvasSetPenColor(Canvas, Color);
	}	

	if (result)
	{
		result = SmoothCanvasSetPenThickness(Canvas, Thickness);
	}	

	if (result)
	{
		result = SmoothCanvasSetPenStyle(Canvas, Style);
	}	

	if (result)
	{
		result = SmoothCanvasSetPenCap(Canvas, Cap);
	}	

	if (result)
	{
		result = SmoothCanvasSetPenJoin(Canvas, Join);
	}	

	return result;
}

/* Abstract Image Routines */
#ifndef SMOOTHTILE
#include "src/abstract_tile_routines.c"
#endif

/* Abstract Gradient Routines */
#ifndef SMOOTHGRADIENT
#include "src/abstract_gradient_routines.c"
#endif
