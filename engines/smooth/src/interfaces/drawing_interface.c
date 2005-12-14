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

/* drawingInterface */
static DrawingInterfaceClass drawingInterface;
static SmoothBool drawingInterfaceInitialized = SmoothFalse;

/* Setup drawingInterface, aka reset and then callback to initialize specific implementation */
void SmoothDrawingInterfaceSetup	(
						SmoothBool(* InitializeInterface)(DrawingInterfaceClass *SmoothDrawingInterface),
						SmoothBool(* FinalizeInterface)(DrawingInterfaceClass *SmoothDrawingInterface)
					)
{
	/* Make sure Implementation specific Finalize is NULL here before Finalizing */
	drawingInterface.FinalizeInterface = NULL;

	/* Reset all values to defaults */
	SmoothDrawingInterfaceFinalize();
	AbstractDrawingInterfaceInitialize(&drawingInterface);

	/* Assign Implementation Specific Initialize if any */
	if (InitializeInterface)
		drawingInterface.InitializeInterface = InitializeInterface;
		
	/* Assign Implementation Specific Finalize if any */
	if (FinalizeInterface)
		drawingInterface.FinalizeInterface = FinalizeInterface;

	/* Run Implementation Specific Initialize if any */
	if (drawingInterface.InitializeInterface)
		drawingInterface.InitializeInterface(&drawingInterface);

	drawingInterfaceInitialized = SmoothTrue;
}

/* Cleanup drawingInterface, aka reset to initial Values */
void SmoothDrawingInterfaceCleanup()
{
	/* Call to Implementation specific finalize if one exists */
	if (drawingInterface.FinalizeInterface)
		drawingInterface.FinalizeInterface(&drawingInterface);

	/* Always First in Struct */
	drawingInterface.InitializeInterface = NULL;

	AbstractDrawingInterfaceFinalize(&drawingInterface);
	
	/* Always Last in Struct */
	drawingInterface.FinalizeInterface = NULL;
}

/* Begin Interface Wrapper Functions Implementation */


/* Color Value Getions */
SmoothBool
SmoothColorGetRedValue(SmoothColor *Color,
				SmoothDouble *RedValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ColorGetRedValue)
	{
		result = drawingInterface.ColorGetRedValue(Color, RedValue);
	}
	return result;
}

SmoothBool
SmoothColorGetGreenValue(SmoothColor *Color,
				SmoothDouble *GreenValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ColorGetGreenValue)
	{
		result = drawingInterface.ColorGetGreenValue(Color, GreenValue);
	}
	return result;
}

SmoothBool
SmoothColorGetBlueValue(SmoothColor *Color, 
				SmoothDouble *BlueValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ColorGetBlueValue)
	{
		result = drawingInterface.ColorGetBlueValue(Color, BlueValue);
	}
	return result;
}

SmoothBool
SmoothColorGetAlphaValue(SmoothColor *Color, 
				SmoothDouble *AlphaValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ColorGetAlphaValue)
	{
		result = drawingInterface.ColorGetAlphaValue(Color, AlphaValue);
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
	
	if (drawingInterface.ColorGetValues)
	{
		result = drawingInterface.ColorGetValues(Color, RedValue, GreenValue, BlueValue, AlphaValue);
	}
	return result;
}		

/* Color Value Modifications */
SmoothBool 
SmoothColorSetRedValue(SmoothColor *Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ColorSetRedValue)
	{
		result = drawingInterface.ColorSetRedValue(Color, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothColorSetGreenValue(SmoothColor *Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ColorSetGreenValue)
	{
		result = drawingInterface.ColorSetGreenValue(Color, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothColorSetBlueValue(SmoothColor *Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ColorSetBlueValue)
	{
		result = drawingInterface.ColorSetBlueValue(Color, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothColorSetAlphaValue(SmoothColor *Color, 
				SmoothDouble NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ColorSetAlphaValue)
	{
		result = drawingInterface.ColorSetAlphaValue(Color, NewValue);
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
	
	if (drawingInterface.ColorSetValues)
	{
		result = drawingInterface.ColorSetValues(Color, RedValue, GreenValue, BlueValue, AlphaValue);
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
	
	if (drawingInterface.RectangleCreate)
	{
		result = drawingInterface.RectangleCreate(Rectangle, X, Y, Width, Height);
	}
	
	return result;
}

SmoothBool 
SmoothRectangleDestroy(SmoothRectangle *Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (Rectangle && drawingInterface.RectangleDestroy)
	{
		result = drawingInterface.RectangleDestroy(Rectangle);
	}
	
	return result;
}

SmoothBool 
SmoothRectangleIsValid(SmoothRectangle *Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (Rectangle && drawingInterface.RectangleIsValid)
	{
		result = drawingInterface.RectangleIsValid(Rectangle);
	}
	
	return result;
}

/* Rectangle Value Getions */
SmoothBool
SmoothRectangleGetXValue(SmoothRectangle *Rectangle,
					SmoothInt *XValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.RectangleGetXValue)
	{
		result = drawingInterface.RectangleGetXValue(Rectangle, XValue);
	}
	return result;
}

SmoothBool
SmoothRectangleGetYValue(SmoothRectangle *Rectangle,
					SmoothInt *YValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.RectangleGetYValue)
	{
		result = drawingInterface.RectangleGetYValue(Rectangle, YValue);
	}
	return result;
}

SmoothBool
SmoothRectangleGetWidthValue(SmoothRectangle *Rectangle,
					SmoothInt *WidthValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.RectangleGetWidthValue)
	{
		result = drawingInterface.RectangleGetWidthValue(Rectangle, WidthValue);
	}
	return result;
}

SmoothBool
SmoothRectangleGetHeightValue(SmoothRectangle *Rectangle,
					SmoothInt *HeightValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.RectangleGetHeightValue)
	{
		result = drawingInterface.RectangleGetHeightValue(Rectangle, HeightValue);
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
	
	if (drawingInterface.RectangleGetValues)
	{
		result = drawingInterface.RectangleGetValues(Rectangle, XValue, YValue, WidthValue, HeightValue);
	}
	return result;
}		

/* Rectangle Value Modifications */
SmoothBool 
SmoothRectangleSetXValue(SmoothRectangle *Rectangle, 
				SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.RectangleSetXValue)
	{
		result = drawingInterface.RectangleSetXValue(Rectangle, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothRectangleSetYValue(SmoothRectangle *Rectangle, 
				SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.RectangleSetYValue)
	{
		result = drawingInterface.RectangleSetYValue(Rectangle, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothRectangleSetWidthValue(SmoothRectangle *Rectangle, 
				SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.RectangleSetWidthValue)
	{
		result = drawingInterface.RectangleSetWidthValue(Rectangle, NewValue);
	}
	
	return result;
}

SmoothBool 
SmoothRectangleSetHeightValue(SmoothRectangle *Rectangle, 
				SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.RectangleSetHeightValue)
	{
		result = drawingInterface.RectangleSetHeightValue(Rectangle, NewValue);
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
	
	if (drawingInterface.RectangleSetValues)
	{
		result = drawingInterface.RectangleSetValues(Rectangle, XValue, YValue, WidthValue, HeightValue);
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
	
	if (IntersectingRectangle && drawingInterface.RectangleFindIntersection)
	{
		if (drawingInterface.RectangleIsValid)
		{
			result = drawingInterface.RectangleIsValid(&Rectangle1) && 
					drawingInterface.RectangleIsValid(&Rectangle2) && 
					drawingInterface.RectangleIsValid(IntersectingRectangle);
		}
		else
		{
			result = SmoothTrue;
		}
	
		if (result)
		{
			result = drawingInterface.RectangleFindIntersection(Rectangle1, Rectangle2, IntersectingRectangle);
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
	
	if (drawingInterface.PointGetXValue)
	{
		result = drawingInterface.PointGetXValue(Point, XValue);
	}
	return result;
}
		
SmoothBool
SmoothPointGetYValue(SmoothPoint *Point,
				SmoothInt *YValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.PointGetYValue)
	{
		result = drawingInterface.PointGetYValue(Point, YValue);
	}
	return result;
}
		
SmoothInt
SmoothPointGetValues(SmoothPoint *Point, 
				SmoothInt *XValue, 
				SmoothInt *YValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.PointGetValues)
	{
		result = drawingInterface.PointGetValues(Point, XValue, YValue);
	}
	return result;
}
		

/* Point Value Modifications */
SmoothBool
SmoothPointSetXValue(SmoothPoint *Point, 
			SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.PointSetXValue)
	{
		result = drawingInterface.PointSetXValue(Point, NewValue);
	}
	return result;
}
		

SmoothBool
SmoothPointSetYValue(SmoothPoint *Point, 
			SmoothInt NewValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.PointSetYValue)
	{
		result = drawingInterface.PointSetYValue(Point, NewValue);
	}
	return result;
}
		

SmoothBool
SmoothPointSetValues(SmoothPoint *Point, 
			SmoothInt XValue, 
			SmoothInt YValue)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.PointSetValues)
	{
		result = drawingInterface.PointSetValues(Point, XValue, YValue);
	}
	return result;
}
		

/* Image/Buffer By File name (For caching purposes) */
SmoothBool
SmoothImageBufferLoadByFile(SmoothString ImageFile,
				SmoothImageBuffer *ImageBuffer)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ImageBufferLoadByFile)
	{
		result = drawingInterface.ImageBufferLoadByFile(ImageFile, ImageBuffer);
	}
	
	return result;
}

SmoothBool
SmoothImageBufferUnLoadByFile(SmoothString ImageFile)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ImageBufferUnLoadByFile)
	{
		result = drawingInterface.ImageBufferUnLoadByFile(ImageFile);
	}
	
	return result;
}

/* Canvas Get Image/ImageBuffer Size */
SmoothBool
SmoothImageBufferGetWidth(SmoothImageBuffer *ImageBuffer,
				SmoothInt *Width)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ImageBufferGetWidth && ImageBuffer && Width)
	{
		result = drawingInterface.ImageBufferGetWidth(ImageBuffer, Width);
	}
	return result;
}

SmoothBool
SmoothImageBufferGetHeight(SmoothImageBuffer *ImageBuffer,
				SmoothInt *Height)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ImageBufferGetHeight && ImageBuffer && Height)
	{
		result = drawingInterface.ImageBufferGetHeight(ImageBuffer, Height);
	}
	return result;
}

SmoothBool
SmoothImageGetWidth(SmoothImage *Image,
			SmoothInt *Width)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ImageGetWidth && Image && Width)
	{
		result = drawingInterface.ImageGetWidth(Image, Width);
	}
	return result;
}

SmoothBool
SmoothImageGetHeight(SmoothImage *Image,
				SmoothInt *Height)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.ImageGetHeight && Image && Height)
	{
		result = drawingInterface.ImageGetHeight(Image, Height);
	}
	return result;
}

/* Canvas Map/Free A Color From The Colormap*/
SmoothBool
SmoothCanvasAllocateColor(SmoothCanvas Canvas, 
					SmoothColor *Color)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasAllocateColor)
	{
		result = drawingInterface.CanvasAllocateColor(Canvas, Color);
	}
	
	return result;
}

SmoothBool 
SmoothCanvasDeallocateColor(SmoothCanvas Canvas, 
					SmoothColor *Color)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasAllocateColor)
	{
		result = drawingInterface.CanvasDeallocateColor(Canvas, Color);
	}
	
	return result;
}

/* Canvas cache/uncache a Color */
SmoothBool
SmoothCanvasCacheColor(SmoothCanvas Canvas, 
				SmoothColor *Color)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasCacheColor)
	{
		result = drawingInterface.CanvasCacheColor(Canvas, Color);
	}
	
	return result;
}

SmoothBool
SmoothCanvasUnCacheColor(SmoothCanvas Canvas, 
				SmoothColor *Color)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasUnCacheColor)
	{
		result = drawingInterface.CanvasUnCacheColor(Canvas, Color);
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
	
	if (drawingInterface.CanvasCacheShadedColor)
	{
		result = drawingInterface.CanvasCacheShadedColor(Canvas, Base, Shade, Color);
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
	
	if (drawingInterface.CanvasUnCacheShadedColor)
	{
		result = drawingInterface.CanvasUnCacheShadedColor(Canvas, Base, Shade, Color);
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
	
	if (drawingInterface.CanvasCacheMidPointColor)
	{
		result = drawingInterface.CanvasCacheMidPointColor(Canvas, Point1, Point2, Color);
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
	
	if (drawingInterface.CanvasUnCacheMidPointColor)
	{
		result = drawingInterface.CanvasUnCacheMidPointColor(Canvas, Point1, Point2, Color);
	}
	
	return result;
}

/* Canvas Store/Render */
SmoothBool
SmoothCanvasStore(SmoothCanvas Canvas)
{
	SmoothBool result = SmoothTrue;
	
	if (drawingInterface.CanvasStore)
	{
		result = drawingInterface.CanvasStore(Canvas);
	}
	return result;
}

SmoothBool
SmoothCanvasRender(SmoothCanvas Canvas,	
			SmoothBool ClosePathAndFill)
{
	SmoothBool result = SmoothTrue;
	
	if (drawingInterface.CanvasRender)
	{
		result = drawingInterface.CanvasRender(Canvas, ClosePathAndFill);
	}
	return result;
}

/* Canvas Get/Set/Clear Clip Rectangle */
SmoothBool
SmoothCanvasGetClipRectangle(SmoothCanvas Canvas,
					SmoothRectangle *Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasGetClipRectangle)
	{
		result = drawingInterface.CanvasGetClipRectangle(Canvas, Rectangle);
	}
	return result;
}

SmoothBool 
SmoothCanvasSetClipRectangle(SmoothCanvas Canvas,
						SmoothRectangle	Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasSetClipRectangle)
	{
		if (drawingInterface.RectangleIsValid)
		{
			if (drawingInterface.RectangleIsValid(&Rectangle))
				result = drawingInterface.CanvasSetClipRectangle(Canvas, Rectangle);
		}
		else
		{
			result = drawingInterface.CanvasSetClipRectangle(Canvas, Rectangle);
		}
	}
	return result;
}

SmoothBool 
SmoothCanvasClearClipRectangle(SmoothCanvas Canvas)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasClearClipRectangle)
	{
		result = drawingInterface.CanvasClearClipRectangle(Canvas);
	}
	return result;
}

SmoothBool 
SmoothCanvasClipUseIntersectingRectangle(SmoothCanvas Canvas,
						SmoothRectangle	Rectangle)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasClipUseIntersectingRectangle)
	{
		result = drawingInterface.CanvasClipUseIntersectingRectangle(Canvas, Rectangle);
	}
	return result;
}

/* Canvas Get/Set Pen Properties */
SmoothBool
SmoothCanvasGetPenColor(SmoothCanvas Canvas,
					SmoothColor *Color)
{
	SmoothBool result = FALSE;
	
	if (drawingInterface.CanvasGetPenColor && Color)
	{
		result = drawingInterface.CanvasGetPenColor(Canvas, Color);
	}
	return result;
}

SmoothBool 
SmoothCanvasSetPenColor(SmoothCanvas Canvas, 
					SmoothColor Color)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasSetPenColor)
	{
		result = drawingInterface.CanvasSetPenColor(Canvas, Color);
	}
	return result;
}

SmoothBool
SmoothCanvasGetPenThickness(SmoothCanvas Canvas,
					SmoothInt *Thickness)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasGetPenThickness)
	{
		result = drawingInterface.CanvasGetPenThickness(Canvas, Thickness);
	}
	return result;
}
	
SmoothBool
SmoothCanvasSetPenThickness(SmoothCanvas Canvas, 
					SmoothInt Thickness)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasSetPenThickness)
	{
		result = drawingInterface.CanvasSetPenThickness(Canvas, Thickness);
	}
	return result;
}

SmoothBool
SmoothCanvasGetPenStyle(SmoothCanvas Canvas, 
				SmoothLineStyle *Style)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasGetPenStyle)
	{
		result = drawingInterface.CanvasGetPenStyle(Canvas, Style);
	}
	return result;
}				

SmoothBool
SmoothCanvasSetPenStyle(SmoothCanvas Canvas, 
				SmoothLineStyle Style)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasSetPenStyle)
	{
		result = drawingInterface.CanvasSetPenStyle(Canvas, Style);
	}
	return result;
}				

SmoothBool
SmoothCanvasGetPenJoin(SmoothCanvas Canvas, 
				SmoothLineJoin *Join)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasGetPenJoin)
	{
		result = drawingInterface.CanvasGetPenJoin(Canvas, Join);
	}
	return result;
}				

SmoothBool
SmoothCanvasSetPenJoin(SmoothCanvas Canvas, 
				SmoothLineJoin Join)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasSetPenJoin)
	{
		result = drawingInterface.CanvasSetPenJoin(Canvas, Join);
	}
	return result;
}				

SmoothBool
SmoothCanvasGetPenCap(SmoothCanvas Canvas, 
				SmoothLineCap *Cap)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasGetPenCap)
	{
		result = drawingInterface.CanvasGetPenCap(Canvas, Cap);
	}
	return result;
}				

SmoothBool
SmoothCanvasSetPenCap(SmoothCanvas Canvas, 
				SmoothLineCap Cap)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasSetPenCap)
	{
		result = drawingInterface.CanvasSetPenCap(Canvas, Cap);
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
	
	if (drawingInterface.CanvasGetPenValues)
	{
		result = drawingInterface.CanvasGetPenValues(Canvas, Color, Thickness, Style, Cap, Join);
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
	
	if (drawingInterface.CanvasSetPenValues)
	{
		result = drawingInterface.CanvasSetPenValues(Canvas, Color, Thickness, Style, Cap, Join);
	}
	return result;
}				

SmoothBool
SmoothCanvasGetPenPattern(SmoothCanvas Canvas, 
				SmoothLinePattern *Pattern)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasGetPenPattern)
	{
		result = drawingInterface.CanvasGetPenPattern(Canvas, Pattern);
	}
	return result;
}				

SmoothBool
SmoothCanvasSetPenPattern(SmoothCanvas Canvas, 
				SmoothLinePattern Pattern)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasSetPenPattern)
	{
		result = drawingInterface.CanvasSetPenPattern(Canvas, Pattern);
	}
	return result;
}				


/* Canvas Get/Set Brush Properties */
SmoothBool
SmoothCanvasGetBrushColor(SmoothCanvas Canvas,
					SmoothColor *Color)
{
	SmoothBool result = FALSE;
	
	if (drawingInterface.CanvasGetBrushColor && Color)
	{
		result = drawingInterface.CanvasGetBrushColor(Canvas, Color);
	}
	return result;
}

SmoothBool 
SmoothCanvasSetBrushColor(SmoothCanvas Canvas, 
					SmoothColor Color)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasSetBrushColor)
	{
		result = drawingInterface.CanvasSetBrushColor(Canvas, Color);
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
	
	if (drawingInterface.CanvasCreateImage)
	{
		result = drawingInterface.CanvasCreateImage(Canvas, Width, Height, Image);
	}
	return result;
}
										
SmoothBool
SmoothCanvasCreateImageFromBuffer(SmoothCanvas Canvas,
					SmoothImageBuffer ImageBuffer,
					SmoothImage *Image)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasCreateImageFromBuffer)
	{
		result = drawingInterface.CanvasCreateImageFromBuffer(Canvas, ImageBuffer, Image);
	}
	return result;
}

SmoothBool
SmoothCanvasDestroyImage(SmoothCanvas Canvas,
				SmoothImage *Image)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasDestroyImage)
	{
		result = drawingInterface.CanvasDestroyImage(Canvas, Image);
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
	
	if (drawingInterface.CanvasRenderImage && Canvas)
	{
		result = drawingInterface.CanvasRenderImage(Canvas, Image, 
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
	
	if (drawingInterface.CanvasRenderImageBuffer && Canvas)
	{
		result = drawingInterface.CanvasRenderImageBuffer(Canvas, ImageBuffer, 
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
	
	if (drawingInterface.CanvasRenderTile && Canvas)
	{
		result = drawingInterface.CanvasRenderTile(Canvas, Tile, 
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
	
	if (drawingInterface.CanvasRenderGradient && Canvas)
	{
		result = drawingInterface.CanvasRenderGradient(Canvas, Gradient, 
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
	
	if (drawingInterface.CanvasDrawPoint)
	{
		result = drawingInterface.CanvasDrawPoint(Canvas, X, Y);
	}
	return result;
}				

SmoothBool 
SmoothCanvasDrawPoints(SmoothCanvas Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{				
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasDrawPoints)
	{
		result = drawingInterface.CanvasDrawPoints(Canvas, Points, NumberOfPoints);
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
	
	if (drawingInterface.CanvasDrawLine)
	{
		result = drawingInterface.CanvasDrawLine(Canvas, X1, Y1, X2, Y2);
	}
	return result;
}				

SmoothBool 
SmoothCanvasDrawSegment(SmoothCanvas Canvas,
				SmoothPoint Point1, 
				SmoothPoint Point2)
{				
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasDrawSegment)
	{
		result = drawingInterface.CanvasDrawSegment(Canvas, Point1, Point2);
	}
	return result;
}

SmoothBool 
SmoothCanvasDrawLines(SmoothCanvas Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{				
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasDrawLines)
	{
		result = drawingInterface.CanvasDrawLines(Canvas, Points, NumberOfPoints);
	}
	return result;
}

SmoothBool 
SmoothCanvasDrawSegments(SmoothCanvas Canvas,
					SmoothPoint *Points, 
					SmoothInt NumberOfPoints)
{				
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasDrawSegments)
	{
		result = drawingInterface.CanvasDrawSegments(Canvas, Points, NumberOfPoints);
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
	
	if (drawingInterface.CanvasDrawArc)
	{
		result = drawingInterface.CanvasDrawArc(Canvas, X, Y, Width, Height, 
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
	
	if (drawingInterface.CanvasFrameRectangle)
	{
		result = drawingInterface.CanvasFrameRectangle(Canvas, X, Y, Width, Height);
	}
	return result;
}				

SmoothBool
SmoothCanvasFramePolygon(SmoothCanvas Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasFramePolygon)
	{
		result = drawingInterface.CanvasFramePolygon(Canvas, Points, NumberOfPoints);
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
	
	if (drawingInterface.CanvasFrameChord)
	{
		result = drawingInterface.CanvasFrameChord(Canvas, X, Y, Width, Height, 
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
	
	if (drawingInterface.CanvasFillRectangle)
	{
		result = drawingInterface.CanvasFillRectangle(Canvas, X, Y, Width, Height);
	}
	return result;
}				

SmoothBool
SmoothCanvasFillPolygon(SmoothCanvas Canvas,
				SmoothPoint *Points, 
				SmoothInt NumberOfPoints)
{
	SmoothBool result = SmoothFalse;
	
	if (drawingInterface.CanvasFillPolygon)
	{
		result = drawingInterface.CanvasFillPolygon(Canvas, Points, NumberOfPoints);
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
	
	if (drawingInterface.CanvasFillChord)
	{
		result = drawingInterface.CanvasFillChord(Canvas, X, Y, Width, Height, 
								AnglePosition, AngleLength);
	}
	return result;
}				
