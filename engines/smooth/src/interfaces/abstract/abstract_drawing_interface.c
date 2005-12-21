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

/* Declare Abstract Interface Functions */

/* Color Get/Set Values */
#ifndef SMOOTHCOLOR
static SmoothBool AbstractColorGetRedValue(SmoothColor *_Color,
						SmoothDouble *RedValue);
static SmoothBool AbstractColorSetRedValue(SmoothColor *_Color, 
						SmoothDouble NewValue);

static SmoothBool AbstractColorGetGreenValue(SmoothColor *_Color,
						SmoothDouble *GreenValue);
static SmoothBool AbstractColorSetGreenValue(SmoothColor *_Color, 
						SmoothDouble NewValue);

static SmoothBool AbstractColorGetBlueValue(SmoothColor *_Color,
						SmoothDouble *BlueValue);
static SmoothBool AbstractColorSetBlueValue(SmoothColor *_Color,
						SmoothDouble NewValue);

static SmoothBool AbstractColorGetAlphaValue(SmoothColor *_Color,
						SmoothDouble *AlphaValue);
static SmoothBool AbstractColorSetAlphaValue(SmoothColor *_Color, 
						SmoothDouble NewValue);
#endif

static SmoothBool 
AbstractColorGetValues(SmoothColor *Color, 
				SmoothDouble *RedValue, 
				SmoothDouble *GreenValue, 
				SmoothDouble *BlueValue, 
				SmoothDouble *AlphaValue);

static SmoothBool 
AbstractColorSetValues(SmoothColor *Color, 
				SmoothDouble RedValue, 
				SmoothDouble GreenValue, 
				SmoothDouble BlueValue, 
				SmoothDouble AlphaValue);

/* Rectangle Get/Set Values */
static SmoothBool 
AbstractRectangleGetValues(SmoothRectangle *Rectangle, 
				SmoothInt *XValue, 
				SmoothInt *YValue, 
				SmoothInt *WidthValue, 
				SmoothInt *HeightValue);

static SmoothBool 
AbstractRectangleSetValues(SmoothRectangle *Rectangle, 
				SmoothInt XValue, 
				SmoothInt YValue, 
				SmoothInt WidthValue, 
				SmoothInt HeightValue);

/* Point Get/Set Values */
static SmoothBool 
AbstractPointGetValues(SmoothPoint *Point, 
				SmoothInt *XValue, 
				SmoothInt *YValue);

static SmoothBool 
AbstractPointSetValues(SmoothPoint *Point, 
				SmoothInt XValue, 
				SmoothInt YValue);

/* Canvas Get/Set Clip Rectangle */
static SmoothBool
AbstractCanvasClipUseIntersectingRectangle(SmoothCanvas Canvas,
						SmoothRectangle Rectangle);

/* Canvas Get/Set Pen Properties */
static SmoothBool 
AbstractCanvasGetPenValues(SmoothCanvas Canvas,
					SmoothColor *Color,
					SmoothInt *Thickness,
					SmoothLineStyle *Style,
					SmoothLineCap *Cap,
					SmoothLineJoin *Join);

static SmoothBool 
AbstractCanvasSetPenValues(SmoothCanvas Canvas,
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

/* Initialize drawingInterface */
SmoothBool 
AbstractDrawingInterfaceInitialize(DrawingInterfaceClass *_DrawingInterface)
{
	/* Color Get/Set Values */
	_DrawingInterface->ColorGetRedValue = (void *) NULL;
	_DrawingInterface->ColorSetRedValue = (void *) NULL;

	_DrawingInterface->ColorGetGreenValue = (void *) NULL;
	_DrawingInterface->ColorSetGreenValue = (void *) NULL;

	_DrawingInterface->ColorGetBlueValue = (void *) NULL;
	_DrawingInterface->ColorSetBlueValue = (void *) NULL;

	_DrawingInterface->ColorGetAlphaValue = (void *) NULL;
	_DrawingInterface->ColorSetAlphaValue = (void *) NULL;

	_DrawingInterface->ColorGetValues = AbstractColorGetValues;
	_DrawingInterface->ColorSetValues = AbstractColorSetValues;

	/* Rectangle Status Functions */
	_DrawingInterface->RectangleCreate = (void *) NULL;
	_DrawingInterface->RectangleDestroy = (void *) NULL;
	_DrawingInterface->RectangleIsValid = (void *) NULL;

	/* Rectangle Get/Set Values */
	_DrawingInterface->RectangleGetXValue = (void *) NULL;
	_DrawingInterface->RectangleSetXValue = (void *) NULL;

	_DrawingInterface->RectangleGetYValue = (void *) NULL;
	_DrawingInterface->RectangleSetYValue = (void *) NULL;

	_DrawingInterface->RectangleGetWidthValue = (void *) NULL;
	_DrawingInterface->RectangleSetWidthValue = (void *) NULL;

	_DrawingInterface->RectangleGetHeightValue = (void *) NULL;
	_DrawingInterface->RectangleSetHeightValue = (void *) NULL;

	_DrawingInterface->RectangleGetValues = AbstractRectangleGetValues;	
	_DrawingInterface->RectangleSetValues = AbstractRectangleSetValues;

	/* Rectangle Misc Functions */
	_DrawingInterface->RectangleFindIntersection = (void *) NULL;

	/* Point Get/Set Values */
	_DrawingInterface->PointGetXValue = (void *) NULL;
	_DrawingInterface->PointSetXValue = (void *) NULL;

	_DrawingInterface->PointGetYValue = (void *) NULL;
	_DrawingInterface->PointSetYValue = (void *) NULL;

	_DrawingInterface->PointGetValues = AbstractPointGetValues;
	_DrawingInterface->PointSetValues = AbstractPointSetValues;

	/* Image/Buffer By File name (For caching purposes) */
	_DrawingInterface->ImageBufferLoadByFile = (void *) NULL;
	_DrawingInterface->ImageBufferUnLoadByFile = (void *) NULL;

	/* Canvas Get Image/ImageBuffer Size */
	_DrawingInterface->ImageBufferGetWidth = (void *) NULL;
	_DrawingInterface->ImageBufferGetHeight = (void *) NULL;
	_DrawingInterface->ImageGetWidth = (void *) NULL;
	_DrawingInterface->ImageGetHeight = (void *) NULL;

	/* Canvas Map/Free A Color From The Colormap*/
	_DrawingInterface->CanvasAllocateColor = (void *) NULL;
	_DrawingInterface->CanvasDeallocateColor = (void *) NULL;

	/* Canvas cache/uncache a Color */
	_DrawingInterface->CanvasCacheColor = (void *) NULL;
	_DrawingInterface->CanvasUnCacheColor = (void *) NULL;
			
	/* Canvas cache/uncache a shaded Color */
	_DrawingInterface->CanvasCacheShadedColor = (void *) NULL;
	_DrawingInterface->CanvasUnCacheShadedColor = (void *) NULL;
	
	/* Canvas cache/uncache a midpoint Color */
	_DrawingInterface->CanvasCacheMidPointColor = (void *) NULL;
	_DrawingInterface->CanvasUnCacheMidPointColor = (void *) NULL;

	/* Canvas Store/Render */
	_DrawingInterface->CanvasStore =  (void *) NULL;
	_DrawingInterface->CanvasRender =  (void *) NULL;

	/* Canvas Get/Set Clip Rectangle */
	_DrawingInterface->CanvasGetClipRectangle = (void *) NULL;
	_DrawingInterface->CanvasSetClipRectangle = (void *) NULL;
	_DrawingInterface->CanvasClearClipRectangle = (void *) NULL;
	_DrawingInterface->CanvasClipUseIntersectingRectangle = (void *)AbstractCanvasClipUseIntersectingRectangle;

	/* Canvas Get/Set Pen Properties */
	_DrawingInterface->CanvasGetPenColor = (void *) NULL;
	_DrawingInterface->CanvasSetPenColor = (void *) NULL;	
	
	_DrawingInterface->CanvasGetPenThickness = (void *) NULL;
	_DrawingInterface->CanvasSetPenThickness = (void *) NULL;
	
	_DrawingInterface->CanvasGetPenStyle = (void *) NULL;
	_DrawingInterface->CanvasSetPenStyle = (void *) NULL;
	
	_DrawingInterface->CanvasGetPenCap = (void *) NULL;
	_DrawingInterface->CanvasSetPenCap = (void *) NULL;
	
	_DrawingInterface->CanvasGetPenJoin = (void *) NULL;
	_DrawingInterface->CanvasSetPenJoin = (void *) NULL;
	
	_DrawingInterface->CanvasGetPenPattern = (void *) NULL;
	_DrawingInterface->CanvasSetPenPattern = (void *) NULL;

	_DrawingInterface->CanvasGetPenValues = (void *)AbstractCanvasGetPenValues;
	_DrawingInterface->CanvasSetPenValues = (void *)AbstractCanvasSetPenValues;

	/* Canvas Get/Set Brush Properties */
	_DrawingInterface->CanvasGetBrushColor = (void *) NULL;
	_DrawingInterface->CanvasSetBrushColor = (void *) NULL;	

	/* Canvas Create Image */
	_DrawingInterface->CanvasCreateImage = (void *) NULL;
	_DrawingInterface->CanvasCreateImageFromBuffer = (void *) NULL;
	_DrawingInterface->CanvasDestroyImage = (void *) NULL;

	/* Canvas Render Image/ImageBuffer */
	_DrawingInterface->CanvasRenderImage = (void *) NULL;						
	_DrawingInterface->CanvasRenderImageBuffer = (void *) NULL;						

	/* Canvas Render Tile */
	#ifndef SMOOTHTILE
	_DrawingInterface->CanvasRenderTile = (void *)AbstractCanvasRenderTile;
	#else
	_DrawingInterface->CanvasRenderTile = (void *)NULL;
	#endif

	/* Canvas Render Gradient */
	#ifndef SMOOTHGRADIENT
	_DrawingInterface->CanvasRenderGradient = (void *)AbstractCanvasRenderGradient;
	#else
	_DrawingInterface->CanvasRenderGradient = (void *)NULL;
	#endif

	/* Canvas Draw Point(s) */
	_DrawingInterface->CanvasDrawPoint = (void *) NULL;
	_DrawingInterface->CanvasDrawPoints = (void *) NULL;

	/* Canvas Draw Line(s) */
	_DrawingInterface->CanvasDrawLine = (void *) NULL;
	_DrawingInterface->CanvasDrawSegment = (void *) NULL;
	_DrawingInterface->CanvasDrawLines = (void *) NULL;
/*	_DrawingInterface->CanvasDrawSegments = (void *) NULL;*/
	
	/* Canvas Draw Arc(s)/Curve(s) */
	_DrawingInterface->CanvasDrawArc = (void *) NULL;

	/* Canvas Frame Shaped Areas */
	_DrawingInterface->CanvasFrameRectangle = (void *) NULL;
	_DrawingInterface->CanvasFramePolygon = (void *) NULL;
	_DrawingInterface->CanvasFrameChord = (void *) NULL;

	/* Canvas Fill Shaped Areas */
	_DrawingInterface->CanvasFillRectangle = (void *) NULL;
	_DrawingInterface->CanvasFillPolygon = (void *) NULL;
	_DrawingInterface->CanvasFillChord = (void *) NULL;
	
	return SmoothTrue;
}

SmoothBool 
AbstractDrawingInterfaceFinalize(DrawingInterfaceClass *_DrawingInterface)
{
	return SmoothTrue;
}

/* Implement Abstract Interface Functions */

/* Color Get/Set Values */
#ifndef SMOOTHCOLOR
static SmoothBool 
AbstractColorGetRedValue(SmoothColor *Color,
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
AbstractColorSetRedValue(SmoothColor *Color, 
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
AbstractColorGetGreenValue(SmoothColor *Color,
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
AbstractColorSetGreenValue(SmoothColor *Color, 
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
AbstractColorGetBlueValue(SmoothColor *Color,
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
AbstractColorSetBlueValue(SmoothColor *Color,
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
AbstractColorGetAlphaValue(SmoothColor *_Color,
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
AbstractColorSetAlphaValue(SmoothColor *_Color, 
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
AbstractColorGetValues(SmoothColor *Color, 
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
AbstractColorSetValues(SmoothColor *Color, 
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
AbstractRectangleGetValues(SmoothRectangle *Rectangle, 
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
AbstractRectangleSetValues(SmoothRectangle *Rectangle, 
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
AbstractPointGetValues(SmoothPoint *Point, 
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
AbstractPointSetValues(SmoothPoint *Point, 
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
AbstractCanvasClipUseIntersectingRectangle(SmoothCanvas Canvas,
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
AbstractCanvasGetPenValues(SmoothCanvas Canvas,
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
AbstractCanvasSetPenValues(SmoothCanvas Canvas,
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
