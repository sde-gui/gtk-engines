/******************************************************************************/
/* drawing_interface.c - Base Drawing Interface Functions                     */
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
#include <interfaces/abstract/abstract_drawing_interface.h>

/* smoothDrawingInterface */
static DrawingInterfaceClass smoothDrawingInterface;
static SmoothBool smoothDrawingInterfaceInitialized = SmoothFalse;

/* Setup smoothDrawingInterface, aka reset and then callback to initialize specific implementation */
void SmoothDrawingInterfaceSetup	(
						SmoothBool(* InitializeInterface)(DrawingInterfaceClass *SmoothDrawingInterface),
						SmoothBool(* FinalizeInterface)(DrawingInterfaceClass *SmoothDrawingInterface)
					)
{
	/* Make sure Implementation specific Finalize is NULL here before Finalizing */
	smoothDrawingInterface.FinalizeInterface = NULL;

	/* Reset all values to defaults */
	SmoothDrawingInterfaceFinalize();
	SmoothAbstractDrawingInterfaceInitialize(&smoothDrawingInterface);

	/* Assign Implementation Specific Initialize if any */
	if (InitializeInterface)
		smoothDrawingInterface.InitializeInterface = InitializeInterface;
		
	/* Assign Implementation Specific Finalize if any */
	if (FinalizeInterface)
		smoothDrawingInterface.FinalizeInterface = FinalizeInterface;

	/* Run Implementation Specific Initialize if any */
	if (smoothDrawingInterface.InitializeInterface)
		smoothDrawingInterface.InitializeInterface(&smoothDrawingInterface);

	smoothDrawingInterfaceInitialized = SmoothTrue;
}

/* Cleanup smoothDrawingInterface, aka reset to initial Values */
void SmoothDrawingInterfaceCleanup()
{
	/* Call to Implementation specific finalize if one exists */
	if (smoothDrawingInterface.FinalizeInterface)
		smoothDrawingInterface.FinalizeInterface(&smoothDrawingInterface);

	/* Always First in Struct */
	smoothDrawingInterface.InitializeInterface = NULL;

	SmoothAbstractDrawingInterfaceFinalize(&smoothDrawingInterface);
	
	/* Always Last in Struct */
	smoothDrawingInterface.FinalizeInterface = NULL;
}

/* Begin Interface Wrapper Functions Implementation */


/* Color Value Getions */
SmoothBool
SmoothColorGetRedValue(SmoothColor *Color,
				SmoothDouble *RedValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ColorGetRedValue)
	{
		result = smoothDrawingInterface.ColorGetRedValue(Color, RedValue);
	}
	return result;
}

SmoothBool
SmoothColorGetGreenValue(SmoothColor *Color,
				SmoothDouble *GreenValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ColorGetGreenValue)
	{
		result = smoothDrawingInterface.ColorGetGreenValue(Color, GreenValue);
	}
	return result;
}

SmoothBool
SmoothColorGetBlueValue(SmoothColor *Color, 
				SmoothDouble *BlueValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ColorGetBlueValue)
	{
		result = smoothDrawingInterface.ColorGetBlueValue(Color, BlueValue);
	}
	return result;
}

SmoothBool
SmoothColorGetAlphaValue(SmoothColor *Color, 
				SmoothDouble *AlphaValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ColorGetAlphaValue)
	{
		result = smoothDrawingInterface.ColorGetAlphaValue(Color, AlphaValue);
	}
	return result;
}

SmoothBool 
SmoothColorGetValues(SmoothColor *Color, 
				SmoothDouble *RedValue, 
				SmoothDouble *GreenValue, 
				SmoothDouble *BlueValue, 
				SmoothDouble *AlphaValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ColorGetValues)
	{
		result = smoothDrawingInterface.ColorGetValues(Color, RedValue, GreenValue, BlueValue, AlphaValue);
	}
	return result;
}		

/* Color Value Modifications */
SmoothBool 
SmoothColorSetRedValue(SmoothColor *Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ColorSetRedValue)
	{
		result = smoothDrawingInterface.ColorSetRedValue(Color, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothColorSetGreenValue(SmoothColor *Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ColorSetGreenValue)
	{
		result = smoothDrawingInterface.ColorSetGreenValue(Color, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothColorSetBlueValue(SmoothColor *Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ColorSetBlueValue)
	{
		result = smoothDrawingInterface.ColorSetBlueValue(Color, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothColorSetAlphaValue(SmoothColor *Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ColorSetAlphaValue)
	{
		result = smoothDrawingInterface.ColorSetAlphaValue(Color, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothColorSetValues(SmoothColor *Color, 
				SmoothDouble RedValue, 
				SmoothDouble GreenValue, 
				SmoothDouble BlueValue, 
				SmoothDouble AlphaValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ColorSetValues)
	{
		result = smoothDrawingInterface.ColorSetValues(Color, RedValue, GreenValue, BlueValue, AlphaValue);
	}
	return result;
}		


/* Rectangle Status Functions */
SmoothBool 
SmoothRectangleCreate(SmoothRectangle *Rectangle, 
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.RectangleCreate)
	{
		result = smoothDrawingInterface.RectangleCreate(Rectangle, X, Y, Width, Height);
	}
	
	return result;
}

SmoothBool 
SmoothRectangleDestroy(SmoothRectangle *Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (Rectangle && smoothDrawingInterface.RectangleDestroy)
	{
		result = smoothDrawingInterface.RectangleDestroy(Rectangle);
	}
	
	return result;
}

SmoothBool 
SmoothRectangleIsValid(SmoothRectangle *Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (Rectangle && smoothDrawingInterface.RectangleIsValid)
	{
		result = smoothDrawingInterface.RectangleIsValid(Rectangle);
	}
	
	return result;
}

/* Rectangle Value Getions */
SmoothBool
SmoothRectangleGetXValue(SmoothRectangle *Rectangle,
					SmoothInt *XValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.RectangleGetXValue)
	{
		result = smoothDrawingInterface.RectangleGetXValue(Rectangle, XValue);
	}
	return result;
}

SmoothBool
SmoothRectangleGetYValue(SmoothRectangle *Rectangle,
					SmoothInt *YValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.RectangleGetYValue)
	{
		result = smoothDrawingInterface.RectangleGetYValue(Rectangle, YValue);
	}
	return result;
}

SmoothBool
SmoothRectangleGetWidthValue(SmoothRectangle *Rectangle,
					SmoothInt *WidthValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.RectangleGetWidthValue)
	{
		result = smoothDrawingInterface.RectangleGetWidthValue(Rectangle, WidthValue);
	}
	return result;
}

SmoothBool
SmoothRectangleGetHeightValue(SmoothRectangle *Rectangle,
					SmoothInt *HeightValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.RectangleGetHeightValue)
	{
		result = smoothDrawingInterface.RectangleGetHeightValue(Rectangle, HeightValue);
	}
	return result;
}

SmoothBool 
SmoothRectangleGetValues(SmoothRectangle *Rectangle,
				SmoothInt *XValue, 
				SmoothInt *YValue, 
				SmoothInt *WidthValue, 
				SmoothInt *HeightValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.RectangleGetValues)
	{
		result = smoothDrawingInterface.RectangleGetValues(Rectangle, XValue, YValue, WidthValue, HeightValue);
	}
	return result;
}		

/* Rectangle Value Modifications */
SmoothBool 
SmoothRectangleSetXValue(SmoothRectangle *Rectangle, 
				SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.RectangleSetXValue)
	{
		result = smoothDrawingInterface.RectangleSetXValue(Rectangle, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothRectangleSetYValue(SmoothRectangle *Rectangle, 
				SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.RectangleSetYValue)
	{
		result = smoothDrawingInterface.RectangleSetYValue(Rectangle, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothRectangleSetWidthValue(SmoothRectangle *Rectangle, 
				SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.RectangleSetWidthValue)
	{
		result = smoothDrawingInterface.RectangleSetWidthValue(Rectangle, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothRectangleSetHeightValue(SmoothRectangle *Rectangle, 
				SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.RectangleSetHeightValue)
	{
		result = smoothDrawingInterface.RectangleSetHeightValue(Rectangle, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothRectangleSetValues(SmoothRectangle *Rectangle,
				SmoothInt XValue, 
				SmoothInt YValue, 
				SmoothInt WidthValue, 
				SmoothInt HeightValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.RectangleSetValues)
	{
		result = smoothDrawingInterface.RectangleSetValues(Rectangle, XValue, YValue, WidthValue, HeightValue);
	}
	return result;
}		

/* Rectangle Misc Functions */
SmoothBool 
SmoothRectangleFindIntersection(SmoothRectangle Rectangle1, 
					SmoothRectangle Rectangle2, 
					SmoothRectangle *IntersectingRectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (IntersectingRectangle && smoothDrawingInterface.RectangleFindIntersection)
	{
		if (smoothDrawingInterface.RectangleIsValid)
		{
			result = smoothDrawingInterface.RectangleIsValid(&Rectangle1) && 
					smoothDrawingInterface.RectangleIsValid(&Rectangle2) && 
					smoothDrawingInterface.RectangleIsValid(IntersectingRectangle);
		}
		else
		{
			result = SmoothTrue;
		}
	
		if (result)
		{
			result = smoothDrawingInterface.RectangleFindIntersection(Rectangle1, Rectangle2, IntersectingRectangle);
		}		
	}
	
	return result;
}

/* Point Value Getions */
SmoothBool
SmoothPointGetXValue(SmoothPoint *Point,
				SmoothInt *XValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.PointGetXValue)
	{
		result = smoothDrawingInterface.PointGetXValue(Point, XValue);
	}
	return result;
}
		
SmoothBool
SmoothPointGetYValue(SmoothPoint *Point,
				SmoothInt *YValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.PointGetYValue)
	{
		result = smoothDrawingInterface.PointGetYValue(Point, YValue);
	}
	return result;
}
		
SmoothInt
SmoothPointGetValues(SmoothPoint *Point, 
				SmoothInt *XValue, 
				SmoothInt *YValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.PointGetValues)
	{
		result = smoothDrawingInterface.PointGetValues(Point, XValue, YValue);
	}
	return result;
}
		

/* Point Value Modifications */
SmoothBool
SmoothPointSetXValue(SmoothPoint *Point, 
			SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.PointSetXValue)
	{
		result = smoothDrawingInterface.PointSetXValue(Point, NewValue);
	}
	return result;
}
		

SmoothBool
SmoothPointSetYValue(SmoothPoint *Point, 
			SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.PointSetYValue)
	{
		result = smoothDrawingInterface.PointSetYValue(Point, NewValue);
	}
	return result;
}
		

SmoothBool
SmoothPointSetValues(SmoothPoint *Point, 
			SmoothInt XValue, 
			SmoothInt YValue)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.PointSetValues)
	{
		result = smoothDrawingInterface.PointSetValues(Point, XValue, YValue);
	}
	return result;
}
		

/* Image/Buffer By File name (For caching purposes) */
SmoothBool
SmoothImageBufferLoadByFile(SmoothString ImageFile,
				SmoothImageBuffer *ImageBuffer)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ImageBufferLoadByFile)
	{
		result = smoothDrawingInterface.ImageBufferLoadByFile(ImageFile, ImageBuffer);
	}
	
	return result;
}

SmoothBool
SmoothImageBufferUnLoadByFile(SmoothString ImageFile)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ImageBufferUnLoadByFile)
	{
		result = smoothDrawingInterface.ImageBufferUnLoadByFile(ImageFile);
	}
	
	return result;
}

/* Canvas Get Image/ImageBuffer Size */
SmoothBool
SmoothImageBufferGetWidth(SmoothImageBuffer *ImageBuffer,
				SmoothInt *Width)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ImageBufferGetWidth && ImageBuffer && Width)
	{
		result = smoothDrawingInterface.ImageBufferGetWidth(ImageBuffer, Width);
	}
	return result;
}

SmoothBool
SmoothImageBufferGetHeight(SmoothImageBuffer *ImageBuffer,
				SmoothInt *Height)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ImageBufferGetHeight && ImageBuffer && Height)
	{
		result = smoothDrawingInterface.ImageBufferGetHeight(ImageBuffer, Height);
	}
	return result;
}

SmoothBool
SmoothImageGetWidth(SmoothImage *Image,
			SmoothInt *Width)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ImageGetWidth && Image && Width)
	{
		result = smoothDrawingInterface.ImageGetWidth(Image, Width);
	}
	return result;
}

SmoothBool
SmoothImageGetHeight(SmoothImage *Image,
				SmoothInt *Height)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.ImageGetHeight && Image && Height)
	{
		result = smoothDrawingInterface.ImageGetHeight(Image, Height);
	}
	return result;
}

/* Canvas Map/Free A Color From The Colormap*/
SmoothBool
SmoothCanvasAllocateColor(SmoothCanvas Canvas, 
					SmoothColor *Color)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasAllocateColor)
	{
		result = smoothDrawingInterface.CanvasAllocateColor(Canvas, Color);
	}
	
	return result;
}

SmoothBool 
SmoothCanvasDeallocateColor(SmoothCanvas Canvas, 
					SmoothColor *Color)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasAllocateColor)
	{
		result = smoothDrawingInterface.CanvasDeallocateColor(Canvas, Color);
	}
	
	return result;
}

/* Canvas cache/uncache a Color */
SmoothBool
SmoothCanvasCacheColor(SmoothCanvas Canvas, 
				SmoothColor *Color)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasCacheColor)
	{
		result = smoothDrawingInterface.CanvasCacheColor(Canvas, Color);
	}
	
	return result;
}

SmoothBool
SmoothCanvasUnCacheColor(SmoothCanvas Canvas, 
				SmoothColor *Color)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasUnCacheColor)
	{
		result = smoothDrawingInterface.CanvasUnCacheColor(Canvas, Color);
	}
	
	return result;
}									

/* Canvas cache/uncache a shaded Color */
SmoothBool
SmoothCanvasCacheShadedColor(SmoothCanvas Canvas, 
					SmoothColor Base,
					SmoothDouble Shade,
					SmoothColor *Color)
{					
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasCacheShadedColor)
	{
		result = smoothDrawingInterface.CanvasCacheShadedColor(Canvas, Base, Shade, Color);
	}
	
	return result;
}

SmoothBool
SmoothCanvasUnCacheShadedColor(SmoothCanvas Canvas, 
					SmoothColor Base,
					SmoothDouble Shade,
					SmoothColor *Color)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasUnCacheShadedColor)
	{
		result = smoothDrawingInterface.CanvasUnCacheShadedColor(Canvas, Base, Shade, Color);
	}
	
	return result;
}

/* Canvas cache/uncache a midpoint Color */
SmoothBool
SmoothCanvasCacheMidPointColor(SmoothCanvas Canvas, 
					SmoothColor Point1,
					SmoothColor Point2,
					SmoothColor *Color)
{					
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasCacheMidPointColor)
	{
		result = smoothDrawingInterface.CanvasCacheMidPointColor(Canvas, Point1, Point2, Color);
	}
	
	return result;
}

SmoothBool
SmoothCanvasUnCacheMidPointColor(SmoothCanvas Canvas, 
					SmoothColor Point1,
					SmoothColor Point2,
					SmoothColor *Color)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasUnCacheMidPointColor)
	{
		result = smoothDrawingInterface.CanvasUnCacheMidPointColor(Canvas, Point1, Point2, Color);
	}
	
	return result;
}

/* Canvas Store/Render */
SmoothBool
SmoothCanvasStore(SmoothCanvas Canvas)
{
	SmoothBool result = SmoothTrue;
	
	if (smoothDrawingInterface.CanvasStore)
	{
		result = smoothDrawingInterface.CanvasStore(Canvas);
	}
	return result;
}

SmoothBool
SmoothCanvasRender(SmoothCanvas Canvas,	
			SmoothBool ClosePathAndFill)
{
	SmoothBool result = SmoothTrue;
	
	if (smoothDrawingInterface.CanvasRender)
	{
		result = smoothDrawingInterface.CanvasRender(Canvas, ClosePathAndFill);
	}
	return result;
}

/* Canvas Get/Set/Clear Clip Rectangle */
SmoothBool
SmoothCanvasGetClipRectangle(SmoothCanvas Canvas,
					SmoothRectangle *Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasGetClipRectangle)
	{
		result = smoothDrawingInterface.CanvasGetClipRectangle(Canvas, Rectangle);
	}
	return result;
}

SmoothBool 
SmoothCanvasSetClipRectangle(SmoothCanvas Canvas,
						SmoothRectangle	Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasSetClipRectangle)
	{
		if (smoothDrawingInterface.RectangleIsValid)
		{
			if (smoothDrawingInterface.RectangleIsValid(&Rectangle))
				result = smoothDrawingInterface.CanvasSetClipRectangle(Canvas, Rectangle);
		}
		else
		{
			result = smoothDrawingInterface.CanvasSetClipRectangle(Canvas, Rectangle);
		}
	}
	return result;
}

SmoothBool 
SmoothCanvasClearClipRectangle(SmoothCanvas Canvas)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasClearClipRectangle)
	{
		result = smoothDrawingInterface.CanvasClearClipRectangle(Canvas);
	}
	return result;
}

SmoothBool 
SmoothCanvasClipUseIntersectingRectangle(SmoothCanvas Canvas,
						SmoothRectangle	Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasClipUseIntersectingRectangle)
	{
		result = smoothDrawingInterface.CanvasClipUseIntersectingRectangle(Canvas, Rectangle);
	}
	return result;
}

/* Canvas Get/Set Pen Properties */
SmoothBool
SmoothCanvasGetPenColor(SmoothCanvas Canvas,
					SmoothColor *Color)
{
	SmoothBool result = FALSE;
	
	if (smoothDrawingInterface.CanvasGetPenColor && Color)
	{
		result = smoothDrawingInterface.CanvasGetPenColor(Canvas, Color);
	}
	return result;
}

SmoothBool 
SmoothCanvasSetPenColor(SmoothCanvas Canvas, 
					SmoothColor Color)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasSetPenColor)
	{
		result = smoothDrawingInterface.CanvasSetPenColor(Canvas, Color);
	}
	return result;
}

SmoothBool
SmoothCanvasGetPenThickness(SmoothCanvas Canvas,
					SmoothInt *Thickness)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasGetPenThickness)
	{
		result = smoothDrawingInterface.CanvasGetPenThickness(Canvas, Thickness);
	}
	return result;
}
	
SmoothBool
SmoothCanvasSetPenThickness(SmoothCanvas Canvas, 
					SmoothInt Thickness)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasSetPenThickness)
	{
		result = smoothDrawingInterface.CanvasSetPenThickness(Canvas, Thickness);
	}
	return result;
}

SmoothBool
SmoothCanvasGetPenStyle(SmoothCanvas Canvas, 
				SmoothLineStyle *Style)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasGetPenStyle)
	{
		result = smoothDrawingInterface.CanvasGetPenStyle(Canvas, Style);
	}
	return result;
}				

SmoothBool
SmoothCanvasSetPenStyle(SmoothCanvas Canvas, 
				SmoothLineStyle Style)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasSetPenStyle)
	{
		result = smoothDrawingInterface.CanvasSetPenStyle(Canvas, Style);
	}
	return result;
}				

SmoothBool
SmoothCanvasGetPenJoin(SmoothCanvas Canvas, 
				SmoothLineJoin *Join)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasGetPenJoin)
	{
		result = smoothDrawingInterface.CanvasGetPenJoin(Canvas, Join);
	}
	return result;
}				

SmoothBool
SmoothCanvasSetPenJoin(SmoothCanvas Canvas, 
				SmoothLineJoin Join)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasSetPenJoin)
	{
		result = smoothDrawingInterface.CanvasSetPenJoin(Canvas, Join);
	}
	return result;
}				

SmoothBool
SmoothCanvasGetPenCap(SmoothCanvas Canvas, 
				SmoothLineCap *Cap)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasGetPenCap)
	{
		result = smoothDrawingInterface.CanvasGetPenCap(Canvas, Cap);
	}
	return result;
}				

SmoothBool
SmoothCanvasSetPenCap(SmoothCanvas Canvas, 
				SmoothLineCap Cap)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasSetPenCap)
	{
		result = smoothDrawingInterface.CanvasSetPenCap(Canvas, Cap);
	}
	return result;
}				

SmoothBool
SmoothCanvasGetPenValues(SmoothCanvas Canvas, 
				SmoothColor *Color,
				SmoothInt *Thickness,
				SmoothLineStyle *Style,
				SmoothLineCap *Cap,
				SmoothLineJoin *Join)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasGetPenValues)
	{
		result = smoothDrawingInterface.CanvasGetPenValues(Canvas, Color, Thickness, Style, Cap, Join);
	}
	return result;
}				

SmoothBool
SmoothCanvasSetPenValues(SmoothCanvas Canvas, 
				SmoothColor Color,
				SmoothInt Thickness,
				SmoothLineStyle Style,
				SmoothLineCap Cap,
				SmoothLineJoin Join)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasSetPenValues)
	{
		result = smoothDrawingInterface.CanvasSetPenValues(Canvas, Color, Thickness, Style, Cap, Join);
	}
	return result;
}				

SmoothBool
SmoothCanvasGetPenPattern(SmoothCanvas Canvas, 
				SmoothLinePattern *Pattern)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasGetPenPattern)
	{
		result = smoothDrawingInterface.CanvasGetPenPattern(Canvas, Pattern);
	}
	return result;
}				

SmoothBool
SmoothCanvasSetPenPattern(SmoothCanvas Canvas, 
				SmoothLinePattern Pattern)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasSetPenPattern)
	{
		result = smoothDrawingInterface.CanvasSetPenPattern(Canvas, Pattern);
	}
	return result;
}				


/* Canvas Get/Set Brush Properties */
SmoothBool
SmoothCanvasGetBrushColor(SmoothCanvas Canvas,
					SmoothColor *Color)
{
	SmoothBool result = FALSE;
	
	if (smoothDrawingInterface.CanvasGetBrushColor && Color)
	{
		result = smoothDrawingInterface.CanvasGetBrushColor(Canvas, Color);
	}
	return result;
}

SmoothBool 
SmoothCanvasSetBrushColor(SmoothCanvas Canvas, 
					SmoothColor Color)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasSetBrushColor)
	{
		result = smoothDrawingInterface.CanvasSetBrushColor(Canvas, Color);
	}
	return result;
}


/* Canvas Create/Destroy Image */
SmoothBool
SmoothCanvasCreateImage(SmoothCanvas Canvas,
				SmoothInt Width,
				SmoothInt Height,
				SmoothImage *Image)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasCreateImage)
	{
		result = smoothDrawingInterface.CanvasCreateImage(Canvas, Width, Height, Image);
	}
	return result;
}
										
SmoothBool
SmoothCanvasCreateImageFromBuffer(SmoothCanvas Canvas,
					SmoothImageBuffer ImageBuffer,
					SmoothImage *Image)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasCreateImageFromBuffer)
	{
		result = smoothDrawingInterface.CanvasCreateImageFromBuffer(Canvas, ImageBuffer, Image);
	}
	return result;
}

SmoothBool
SmoothCanvasDestroyImage(SmoothCanvas Canvas,
				SmoothImage *Image)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasDestroyImage)
	{
		result = smoothDrawingInterface.CanvasDestroyImage(Canvas, Image);
	}
	return result;

}				
										
/* Canvas Render Image/ImageBuffer */
SmoothBool		
SmoothCanvasRenderImage(SmoothCanvas Canvas,
				SmoothImage Image,
				SmoothInt X,
				SmoothInt Y,
				SmoothInt Width,
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasRenderImage && Canvas)
	{
		result = smoothDrawingInterface.CanvasRenderImage(Canvas, Image, 
									X, Y, Width, Height);
	}
	return result;
}													

SmoothBool		
SmoothCanvasRenderImageBuffer(SmoothCanvas Canvas,
					SmoothImageBuffer ImageBuffer,
					SmoothInt X,
					SmoothInt Y,
					SmoothInt Width,
					SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasRenderImageBuffer && Canvas)
	{
		result = smoothDrawingInterface.CanvasRenderImageBuffer(Canvas, ImageBuffer, 
									X, Y, Width, Height);
	}
	return result;
}													

/* Canvas Render Tile */
SmoothBool
SmoothCanvasRenderTile(SmoothCanvas Canvas,
				SmoothTile Tile,
				SmoothInt X,
				SmoothInt Y,
				SmoothInt Width,
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasRenderTile && Canvas)
	{
		result = smoothDrawingInterface.CanvasRenderTile(Canvas, Tile, 
								X, Y, Width, Height);
	}
	return result;
}

/* Canvas Render Gradient */
SmoothBool		
SmoothCanvasRenderGradient(SmoothCanvas Canvas,
				SmoothGradient Gradient,
				SmoothInt X,
				SmoothInt Y,
				SmoothInt Width,
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasRenderGradient && Canvas)
	{
		result = smoothDrawingInterface.CanvasRenderGradient(Canvas, Gradient, 
								X, Y, Width, Height);
	}
	return result;
}

/* Canvas Draw Point(s) */
SmoothBool 
SmoothCanvasDrawPoint(SmoothCanvas Canvas,
				SmoothInt X, 
				SmoothInt Y)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasDrawPoint)
	{
		result = smoothDrawingInterface.CanvasDrawPoint(Canvas, X, Y);
	}
	return result;
}				

SmoothBool 
SmoothCanvasDrawPoints(SmoothCanvas Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{				
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasDrawPoints)
	{
		result = smoothDrawingInterface.CanvasDrawPoints(Canvas, Points, NumberOfPoints);
	}
	return result;
}


/* Canvas Draw Line(s) */
SmoothBool 
SmoothCanvasDrawLine(SmoothCanvas Canvas,
				SmoothInt X1, 
				SmoothInt Y1, 
				SmoothInt X2, 
				SmoothInt Y2)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasDrawLine)
	{
		result = smoothDrawingInterface.CanvasDrawLine(Canvas, X1, Y1, X2, Y2);
	}
	return result;
}				

SmoothBool 
SmoothCanvasDrawSegment(SmoothCanvas Canvas,
				SmoothPoint Point1, 
				SmoothPoint Point2)
{				
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasDrawSegment)
	{
		result = smoothDrawingInterface.CanvasDrawSegment(Canvas, Point1, Point2);
	}
	return result;
}

SmoothBool 
SmoothCanvasDrawLines(SmoothCanvas Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{				
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasDrawLines)
	{
		result = smoothDrawingInterface.CanvasDrawLines(Canvas, Points, NumberOfPoints);
	}
	return result;
}

SmoothBool 
SmoothCanvasDrawSegments(SmoothCanvas Canvas,
					SmoothPoint *Points, 
					SmoothInt NumberOfPoints)
{				
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasDrawSegments)
	{
		result = smoothDrawingInterface.CanvasDrawSegments(Canvas, Points, NumberOfPoints);
	}
	return result;
}

/* Canvas Draw Arc(s)/Curve(s) */
SmoothBool
SmoothCanvasDrawArc(SmoothCanvas Canvas,
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height,
				SmoothDouble AnglePosition,
				SmoothDouble AngleLength)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasDrawArc)
	{
		result = smoothDrawingInterface.CanvasDrawArc(Canvas, X, Y, Width, Height, 
								AnglePosition, AngleLength);
	}
	return result;
}				

/* Canvas Frame Shaped Areas */
SmoothBool
SmoothCanvasFrameRectangle(SmoothCanvas Canvas,
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasFrameRectangle)
	{
		result = smoothDrawingInterface.CanvasFrameRectangle(Canvas, X, Y, Width, Height);
	}
	return result;
}				

SmoothBool
SmoothCanvasFramePolygon(SmoothCanvas Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasFramePolygon)
	{
		result = smoothDrawingInterface.CanvasFramePolygon(Canvas, Points, NumberOfPoints);
	}
	return result;
}				

SmoothBool
SmoothCanvasFrameChord(SmoothCanvas Canvas,
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height,
				SmoothDouble AnglePosition,
				SmoothDouble AngleLength)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasFrameChord)
	{
		result = smoothDrawingInterface.CanvasFrameChord(Canvas, X, Y, Width, Height, 
								AnglePosition, AngleLength);
	}
	return result;
}				

/* Canvas Fill Shaped Areas */
SmoothBool
SmoothCanvasFillRectangle(SmoothCanvas Canvas,
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasFillRectangle)
	{
		result = smoothDrawingInterface.CanvasFillRectangle(Canvas, X, Y, Width, Height);
	}
	return result;
}				

SmoothBool
SmoothCanvasFillPolygon(SmoothCanvas Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasFillPolygon)
	{
		result = smoothDrawingInterface.CanvasFillPolygon(Canvas, Points, NumberOfPoints);
	}
	return result;
}				

SmoothBool
SmoothCanvasFillChord(SmoothCanvas Canvas,
				SmoothInt X, 
				SmoothInt Y, 
				SmoothInt Width, 
				SmoothInt Height,
				SmoothDouble AnglePosition,
				SmoothDouble AngleLength)
{
	SmoothBool result = SmoothFalse;
	
	if (smoothDrawingInterface.CanvasFillChord)
	{
		result = smoothDrawingInterface.CanvasFillChord(Canvas, X, Y, Width, Height, 
								AnglePosition, AngleLength);
	}
	return result;
}				
