/******************************************************************************/
/* drawing_interface.h - Base Drawing Interface Declaration                   */
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
#ifndef DRAWING_INTERFACE_H
#define DRAWING_INTERFACE_H

#include INTERFACE_DEFINES

#include <interfaces/abstract/abstract_type_defines.h>

/****************************/
/* Define Generic Utilities */
/****************************/
typedef struct {
	SmoothGradientType Type;
	SmoothBool QuadraticGradientRange;

	SmoothColor From;
	SmoothColor To;
} SmoothGradient;

typedef struct {
	SmoothString ImageFile;

	SmoothInt XOffset; 
	SmoothInt YOffset;

	SmoothBool UseBuffer;
} SmoothTile;

/****************************/
/* Define ColorCube         */
/****************************/
#define SMOOTHSTATECOUNT SMOOTH_STATE_LAST + 1

typedef enum 
{
	SMOOTH_STATE_DEFAULT = 0, 
	SMOOTH_STATE_PRELIGHT, 
	SMOOTH_STATE_SELECTED, 
	SMOOTH_STATE_ACTIVE, 
	SMOOTH_STATE_DISABLED,
	
	SMOOTH_STATE_LAST = SMOOTH_STATE_DISABLED
} SmoothWidgetState;

typedef struct
{
	SmoothColor Background;
	SmoothColor Foreground;
} SmoothColorPair;

typedef struct 
{

	SmoothColorPair	Input[SMOOTHSTATECOUNT];
	SmoothColorPair	Interaction[SMOOTHSTATECOUNT];

	SmoothInt	ReferenceCount;
} SmoothColorCube;

/********************************/
/* Define DrawingInterfaceClass */
/********************************/
typedef struct _SmoothDrawingInterfaceClass DrawingInterfaceClass;

struct _SmoothDrawingInterfaceClass
{
	/* Callback to Initialize Interface if Required - Always First in Struct*/
	SmoothBool (* InitializeInterface)			(DrawingInterfaceClass *SmoothDrawingInterface);

	/* Color Value Getions */
	SmoothBool (* ColorGetRedValue)				(SmoothColor *Color,
									SmoothDouble *RedValue);
	SmoothBool (* ColorGetGreenValue)			(SmoothColor *Color,
									SmoothDouble *GreenValue);
	SmoothBool (* ColorGetBlueValue)			(SmoothColor *Color,
									SmoothDouble *BlueValue);
	SmoothBool (* ColorGetAlphaValue)			(SmoothColor *Color,
									SmoothDouble *AlphaValue);
	SmoothBool (* ColorGetValues)				(SmoothColor *Color, 
									SmoothDouble *RedValue, 
									SmoothDouble *GreenValue, 
									SmoothDouble *BlueValue, 
									SmoothDouble *AlphaValue);
 
	/* Color Value Modifications */
	SmoothBool (* ColorSetRedValue)				(SmoothColor *Color, 
									SmoothDouble NewValue);
	SmoothBool (* ColorSetGreenValue)			(SmoothColor *Color, 
									SmoothDouble NewValue);
	SmoothBool (* ColorSetBlueValue)			(SmoothColor *Color, 
									SmoothDouble NewValue);
	SmoothBool (* ColorSetAlphaValue)			(SmoothColor *Color, 
									SmoothDouble NewValue);								
	SmoothBool (* ColorSetValues)				(SmoothColor *Color, 
									SmoothDouble RedValue, 
									SmoothDouble GreenValue, 
									SmoothDouble BlueValue, 
									SmoothDouble AlphaValue);

	/* Rectangle Status Functions */
	SmoothBool (* RectangleCreate)				(SmoothRectangle *Rectangle, 
									SmoothInt X, 
									SmoothInt Y, 
									SmoothInt Width, 
									SmoothInt Height);
	SmoothBool (* RectangleDestroy)				(SmoothRectangle *Rectangle);

	SmoothBool (* RectangleIsValid)				(SmoothRectangle *Rectangle);

	/* Rectangle Value Getions */
	SmoothBool (* RectangleGetXValue)			(SmoothRectangle *Rectangle,
									SmoothInt *XValue);
	SmoothBool (* RectangleGetYValue)			(SmoothRectangle *Rectangle,
									SmoothInt *YValue);
	SmoothBool (* RectangleGetWidthValue)			(SmoothRectangle *Rectangle,
									SmoothInt *WidthValue);
	SmoothBool (* RectangleGetHeightValue)			(SmoothRectangle *Rectangle,
									SmoothInt *HeightValue);
	SmoothBool (* RectangleGetValues)			(SmoothRectangle *Rectangle, 
									SmoothInt *XValue, 
									SmoothInt *YValue, 
									SmoothInt *WidthValue, 
									SmoothInt *HeightValue);

	/* Rectangle Value Modifications */
	SmoothBool (* RectangleSetXValue)			(SmoothRectangle *Rectangle, 
									SmoothInt NewValue);
	SmoothBool (* RectangleSetYValue)			(SmoothRectangle *Rectangle, 
									SmoothInt NewValue);
	SmoothBool (* RectangleSetWidthValue)			(SmoothRectangle *Rectangle, 
									SmoothInt NewValue);
	SmoothBool (* RectangleSetHeightValue)			(SmoothRectangle *Rectangle, 
									SmoothInt NewValue);								
	SmoothBool (* RectangleSetValues)			(SmoothRectangle *Rectangle, 
									SmoothInt XValue, 
									SmoothInt YValue, 
									SmoothInt WidthValue, 
									SmoothInt HeightValue);

	/* Rectangle Misc Functions */
	SmoothBool (* RectangleFindIntersection)		(SmoothRectangle Rectangle1, 
									SmoothRectangle Rectangle2, 
									SmoothRectangle *IntersectingRectangle);


	/* Rectangle Point Getions */
	SmoothBool (* PointGetXValue)				(SmoothPoint *Point,
									SmoothInt *XValue);
	SmoothBool (* PointGetYValue)				(SmoothPoint *Point,
									SmoothInt *YValue);
	SmoothBool (* PointGetValues)				(SmoothPoint *Point, 
									SmoothInt *XValue, 
									SmoothInt *YValue);

	/* Rectangle Value Modifications */
	SmoothBool (* PointSetXValue)				(SmoothPoint *Point, 
									SmoothInt NewValue);
	SmoothBool (* PointSetYValue)				(SmoothPoint *Point, 
									SmoothInt NewValue);
	SmoothBool (* PointSetValues)				(SmoothPoint *Point, 
									SmoothInt XValue, 
									SmoothInt YValue);


	/* Image/Buffer By File name (For caching purposes) */
	SmoothBool (* ImageBufferLoadByFile)			(SmoothString ImageFile,
									SmoothImageBuffer *ImageBuffer);
	SmoothBool (* ImageBufferUnLoadByFile)			(SmoothString ImageFile);

	/* Canvas Get Image/ImageBuffer Size */
	SmoothBool (* ImageBufferGetWidth)			(SmoothImageBuffer *ImageBuffer,
									SmoothInt *Width);
	SmoothBool (* ImageBufferGetHeight)			(SmoothImageBuffer *ImageBuffer,
									SmoothInt *Height);

	SmoothBool (* ImageGetWidth)				(SmoothImage *Image,
									SmoothInt *Width);
	SmoothBool (* ImageGetHeight)				(SmoothImage *Image,
									SmoothInt *Height);

	/* Canvas Map/Free A Color From The Colormap*/
	SmoothBool (* CanvasAllocateColor)			(SmoothCanvas Canvas, 
									SmoothColor *Color);
	SmoothBool (* CanvasDeallocateColor)			(SmoothCanvas Canvas, 
									SmoothColor *Color);

	/* Canvas cache/uncache a Color */
	SmoothBool (* CanvasCacheColor)				(SmoothCanvas Canvas, 
									SmoothColor *Color);

	SmoothBool (* CanvasUnCacheColor)			(SmoothCanvas Canvas, 
									SmoothColor *Color);
									
	/* Canvas cache/uncache a shaded Color */
	SmoothBool (* CanvasCacheShadedColor)			(SmoothCanvas Canvas, 
									SmoothColor Base,
									SmoothDouble Shade,
									SmoothColor *Color);

	SmoothBool (* CanvasUnCacheShadedColor)			(SmoothCanvas Canvas, 
									SmoothColor Base,
									SmoothDouble Shade,
									SmoothColor *Color);

	/* Canvas cache/uncache a midpoint Color */
	SmoothBool (* CanvasCacheMidPointColor)			(SmoothCanvas Canvas, 
									SmoothColor Point1,
									SmoothColor Point2,
									SmoothColor *Color);

	SmoothBool (* CanvasUnCacheMidPointColor)		(SmoothCanvas Canvas, 
									SmoothColor Point1,
									SmoothColor Point2,
									SmoothColor *Color);

	/* Canvas Store/Render */
	SmoothBool (* CanvasStore)				(SmoothCanvas Canvas);

	SmoothBool (* CanvasRender)				(SmoothCanvas Canvas,	
									SmoothBool ClosePathAndFill);

	/* Canvas Get/Set Clip Rectangle */
	SmoothBool (* CanvasGetClipRectangle)			(SmoothCanvas Canvas,
									SmoothRectangle *Rectangle);

	SmoothBool (* CanvasSetClipRectangle)			(SmoothCanvas Canvas,
									SmoothRectangle	Rectangle);
								
	SmoothBool (* CanvasClearClipRectangle)			(SmoothCanvas Canvas);

	/* Canvas Get/Set Pen Properties */
	SmoothBool (* CanvasGetPenColor)			(SmoothCanvas Canvas,
									SmoothColor *Color);
	SmoothBool (* CanvasSetPenColor)			(SmoothCanvas Canvas, 
									SmoothColor Color);

	SmoothBool (* CanvasGetPenThickness)			(SmoothCanvas Canvas,
									SmoothInt *Thickness);
	SmoothBool (* CanvasSetPenThickness)			(SmoothCanvas Canvas, 
									SmoothInt Thickness);

	SmoothBool (* CanvasGetPenStyle)			(SmoothCanvas Canvas, 
									SmoothLineStyle *Style);
	SmoothBool (* CanvasSetPenStyle)			(SmoothCanvas Canvas, 
									SmoothLineStyle Style);

	SmoothBool (* CanvasGetPenJoin)				(SmoothCanvas Canvas, 
									SmoothLineJoin *Join);
	SmoothBool (* CanvasSetPenJoin)				(SmoothCanvas Canvas, 
									SmoothLineJoin Join);

	SmoothBool (* CanvasGetPenCap)				(SmoothCanvas Canvas, 
									SmoothLineCap *Cap);
	SmoothBool (* CanvasSetPenCap)				(SmoothCanvas Canvas, 
									SmoothLineCap Cap);

	SmoothBool (* CanvasGetPenValues)			(SmoothCanvas Canvas, 
									SmoothColor *Color,
									SmoothInt *Thickness,
									SmoothLineStyle *Style,
									SmoothLineCap *Cap,
									SmoothLineJoin *Join);

	SmoothBool (* CanvasSetPenValues)			(SmoothCanvas Canvas, 
									SmoothColor Color,
									SmoothInt Thickness,
									SmoothLineStyle Style,
									SmoothLineCap Cap,
									SmoothLineJoin Join);
									
	SmoothBool (* CanvasGetPenPattern)			(SmoothCanvas Canvas, 
									SmoothLinePattern *Pattern);
	SmoothBool (* CanvasSetPenPattern)			(SmoothCanvas Canvas, 
									SmoothLinePattern Pattern);

	/* Canvas Get/Set Brush Properties */
	SmoothBool (* CanvasGetBrushColor)			(SmoothCanvas Canvas,
									SmoothColor *Color);
	SmoothBool (* CanvasSetBrushColor)			(SmoothCanvas Canvas, 
									SmoothColor Color);


	/* Canvas Create Image */
	SmoothBool (* CanvasCreateImage)			(SmoothCanvas Canvas,
									SmoothInt Width,
									SmoothInt Height,
									SmoothImage *Image);
											
	SmoothBool (* CanvasCreateImageFromBuffer)		(SmoothCanvas Canvas,
									SmoothImageBuffer ImageBuffer,
									SmoothImage *Image);

	SmoothBool (* CanvasDestroyImage)			(SmoothCanvas Canvas,
									SmoothImage *Image);
										

	/* Canvas Render/Tile Image/ImageBuffer */
	SmoothBool (* CanvasTileImageBuffer)			(SmoothCanvas Canvas,
									SmoothImageBuffer ImageBuffer,
									SmoothInt XOffset, 
									SmoothInt YOffset,
									SmoothInt X,
									SmoothInt Y,
									SmoothInt Width,
									SmoothInt Height);

	SmoothBool (* CanvasTileImage)				(SmoothCanvas Canvas,
									SmoothImage Image,
									SmoothInt XOffset, 
									SmoothInt YOffset,
									SmoothInt X,
									SmoothInt Y,
									SmoothInt Width,
									SmoothInt Height);


	SmoothBool (* CanvasRenderImage)			(SmoothCanvas Canvas,
									SmoothImage Image,
									SmoothInt X,
									SmoothInt Y,
									SmoothInt Width,
									SmoothInt Height);
										

	SmoothBool (* CanvasRenderImageBuffer)			(SmoothCanvas Canvas,
									SmoothImageBuffer ImageBuffer,
									SmoothInt X,
									SmoothInt Y,
									SmoothInt Width,
									SmoothInt Height);


	/* Canvas Render Tile */
	SmoothBool (* CanvasRenderTile)				(SmoothCanvas Canvas,
									SmoothTile Tile,
									SmoothInt X,
									SmoothInt Y,
									SmoothInt Width,
									SmoothInt Height);

	/* Canvas Render Gradient */
	SmoothBool (* CanvasRenderGradient)			(SmoothCanvas Canvas,
									SmoothGradient Gradient,
									SmoothInt X,
									SmoothInt Y,
									SmoothInt Width,
									SmoothInt Height);

	/* Canvas Draw Point(s) */
	SmoothBool (* CanvasDrawPoint)				(SmoothCanvas Canvas,
									SmoothInt X, 
									SmoothInt Y);

	SmoothBool (* CanvasDrawPoints)				(SmoothCanvas Canvas,
									SmoothPoint *Points, 
									SmoothInt NumberOfPoints);

	/* Canvas Draw Line(s) */
	SmoothBool (* CanvasDrawLine)				(SmoothCanvas Canvas,
									SmoothInt X1, 
									SmoothInt Y1, 
									SmoothInt X2, 
									SmoothInt Y2);
	
	SmoothBool (* CanvasDrawSegment)			(SmoothCanvas Canvas,
									SmoothPoint Point1, 
									SmoothPoint Point2);
	
	SmoothBool (* CanvasDrawLines)				(SmoothCanvas Canvas,
									SmoothPoint *Points, 
									SmoothInt NumberOfPoints);
	
	SmoothBool (* CanvasDrawSegments)			(SmoothCanvas Canvas,
									SmoothPoint *Points, 
									SmoothInt NumberOfPoints);
										
	/* Canvas Draw Arc(s)/Curve(s) */
	SmoothBool (* CanvasDrawArc)				(SmoothCanvas Canvas,
									SmoothInt X, 
									SmoothInt Y, 
									SmoothInt Width, 
									SmoothInt Height,
									SmoothDouble AnglePosition,
									SmoothDouble AngleLength);

	/* Canvas Frame Shaped Areas */
	SmoothBool (* CanvasFrameRectangle)			(SmoothCanvas Canvas,
									SmoothInt X, 
									SmoothInt Y, 
									SmoothInt Width, 
									SmoothInt Height);

	SmoothBool (* CanvasFramePolygon)			(SmoothCanvas Canvas,
									SmoothPoint *Points, 
									SmoothInt NumberOfPoints);
	
	SmoothBool (* CanvasFrameChord)				(SmoothCanvas Canvas,
									SmoothInt X, 
									SmoothInt Y, 
									SmoothInt Width, 
									SmoothInt Height,
									SmoothDouble AnglePosition,
									SmoothDouble AngleLength);

	/* Canvas Fill Shaped Areas */
	SmoothBool (* CanvasFillRectangle)			(SmoothCanvas Canvas,
									SmoothInt X, 
									SmoothInt Y, 
									SmoothInt Width, 
									SmoothInt Height);

	SmoothBool (* CanvasFillPolygon)			(SmoothCanvas Canvas,
									SmoothPoint *Points, 
									SmoothInt NumberOfPoints);
	
	SmoothBool (* CanvasFillChord)				(SmoothCanvas Canvas,
									SmoothInt X, 
									SmoothInt Y, 
									SmoothInt Width, 
									SmoothInt Height,
									SmoothDouble AnglePosition,
									SmoothDouble AngleLength);

	SmoothBool (*CanvasClipUseIntersectingRectangle)	(SmoothCanvas Canvas,
									SmoothRectangle Rectangle);

	/* Callback to Finalize Interface if Required - Always Last in Struct*/
	SmoothBool (* FinalizeInterface)			(DrawingInterfaceClass *SmoothDrawingInterface);
};

/************************************/
/* Initialize/Finalize Interface    */
/* ABSTRACT MUST BE DECLARED BY     */
/* Specific Drawing Interface       */
/************************************/
GE_INTERNAL void SmoothDrawingInterfaceInitialize();
GE_INTERNAL void SmoothDrawingInterfaceFinalize();


/**********************************************/
/* Hooks For Interface Specific Setup/Cleanup */
/**********************************************/
GE_INTERNAL void 			SmoothDrawingInterfaceSetup		(
										SmoothBool(* InitializeInterface)(DrawingInterfaceClass *SmoothDrawingInterface),
										SmoothBool(* FinalizeInterface)(DrawingInterfaceClass *SmoothDrawingInterface)
									);
									
GE_INTERNAL void 			SmoothDrawingInterfaceCleanup			();

/*************************/
/* Function Declarations */
/*************************/

/* Callback to Initialize Interface if Required - Always First in Struct*/
GE_INTERNAL SmoothBool 		SmoothInitializeInterface();

/* Color Value Getions */
GE_INTERNAL SmoothBool		SmoothColorGetRedValue				(SmoothColor *Color,
										SmoothDouble *RedValue);
GE_INTERNAL SmoothBool		SmoothColorGetGreenValue			(SmoothColor *Color,
										SmoothDouble *GreenValue);
GE_INTERNAL SmoothBool		SmoothColorGetBlueValue				(SmoothColor *Color,
										SmoothDouble *BlueValue);
GE_INTERNAL SmoothBool		SmoothColorGetAlphaValue			(SmoothColor *Color,
										SmoothDouble *AlphaValue);
GE_INTERNAL SmoothBool		SmoothColorGetValues				(SmoothColor *Color, 
										SmoothDouble *RedValue, 
										SmoothDouble *GreenValue, 
										SmoothDouble *BlueValue, 
										SmoothDouble *AlphaValue);

/* Color Value Modifications */
GE_INTERNAL SmoothBool		SmoothColorSetRedValue				(SmoothColor *Color, 
										SmoothDouble NewValue);
GE_INTERNAL SmoothBool		SmoothColorSetGreenValue			(SmoothColor *Color, 
										SmoothDouble NewValue);
GE_INTERNAL SmoothBool		SmoothColorSetBlueValue				(SmoothColor *Color, 
										SmoothDouble NewValue);
GE_INTERNAL SmoothBool		SmoothColorSetAlphaValue			(SmoothColor *Color, 
										SmoothDouble NewValue);
GE_INTERNAL SmoothBool		SmoothColorSetValues				(SmoothColor *Color, 
										SmoothDouble RedValue, 
										SmoothDouble GreenValue, 
										SmoothDouble BlueValue, 
										SmoothDouble AlphaValue);

/* Rectangle Status Functions */
GE_INTERNAL SmoothBool		SmoothRectangleCreate				(SmoothRectangle *Rectangle, 
										SmoothInt X, 
										SmoothInt Y, 
										SmoothInt Width, 
										SmoothInt Height);
GE_INTERNAL SmoothBool		SmoothRectangleDestroy				(SmoothRectangle *Rectangle);
	
GE_INTERNAL SmoothBool		SmoothRectangleIsValid				(SmoothRectangle *Rectangle);
	
/* Rectangle Value Getions */
GE_INTERNAL SmoothBool		SmoothRectangleGetXValue			(SmoothRectangle *Rectangle,
										SmoothInt *XValue);
GE_INTERNAL SmoothBool		SmoothRectangleGetYValue			(SmoothRectangle *Rectangle,
										SmoothInt *YValue);
GE_INTERNAL SmoothBool		SmoothRectangleGetWidthValue			(SmoothRectangle *Rectangle,
										SmoothInt *WidthValue);
GE_INTERNAL SmoothBool		SmoothRectangleGetHeightValue			(SmoothRectangle *Rectangle,
										SmoothInt *HeightValue);
GE_INTERNAL SmoothBool		SmoothRectangleGetValues			(SmoothRectangle *Rectangle, 
										SmoothInt *XValue, 
										SmoothInt *YValue, 
										SmoothInt *WidthValue, 
										SmoothInt *HeightValue);
	
/* Rectangle Value Modifications */
GE_INTERNAL SmoothBool		SmoothRectangleSetXValue			(SmoothRectangle *Rectangle, 
										SmoothInt NewValue);
GE_INTERNAL SmoothBool		SmoothRectangleSetYValue			(SmoothRectangle *Rectangle, 
										SmoothInt NewValue);
GE_INTERNAL SmoothBool		SmoothRectangleSetWidthValue			(SmoothRectangle *Rectangle, 
										SmoothInt NewValue);
GE_INTERNAL SmoothBool		SmoothRectangleSetHeightValue			(SmoothRectangle *Rectangle, 
										SmoothInt NewValue);							
GE_INTERNAL SmoothBool		SmoothRectangleSetValues			(SmoothRectangle *Rectangle, 
										SmoothInt XValue, 
										SmoothInt YValue, 
										SmoothInt WidthValue, 
										SmoothInt HeightValue);

/* Rectangle Misc Functions */
GE_INTERNAL SmoothBool		SmoothRectangleFindIntersection			(SmoothRectangle Rectangle1, 
										SmoothRectangle Rectangle2, 
										SmoothRectangle *IntersectingRectangle);

/* Point Value Getions */
GE_INTERNAL SmoothBool		SmoothPointGetXValue				(SmoothPoint *Point,
										SmoothInt *XValue);
GE_INTERNAL SmoothBool		SmoothPointGetYValue				(SmoothPoint *Point,
										SmoothInt *YValue);
GE_INTERNAL SmoothBool		SmoothPointGetValues				(SmoothPoint *Point,
										SmoothInt *XValue, 
										SmoothInt *YValue);

/* Point Value Modifications */
GE_INTERNAL SmoothBool		SmoothPointSetXValue				(SmoothPoint *Point,
										SmoothInt NewValue);
GE_INTERNAL SmoothBool		SmoothPointSetYValue				(SmoothPoint *Point,
										SmoothInt NewValue);
GE_INTERNAL SmoothBool		SmoothPointSetValues				(SmoothPoint *Point, 
										SmoothInt XValue, 
										SmoothInt YValue);

/* Image/Buffer By File name (For caching purposes) */
GE_INTERNAL SmoothBool		SmoothImageBufferLoadByFile			(SmoothString ImageFile,
										SmoothImageBuffer *ImageBuffer);
GE_INTERNAL SmoothBool		SmoothImageBufferUnLoadByFile			(SmoothString ImageFile);

/* Canvas Get Image/ImageBuffer Size */
GE_INTERNAL SmoothBool		SmoothImageBufferGetWidth			(SmoothImageBuffer *ImageBuffer,
										SmoothInt *Width);
GE_INTERNAL SmoothBool		SmoothImageBufferGetHeight			(SmoothImageBuffer *ImageBuffer,
										SmoothInt *Height);

GE_INTERNAL SmoothBool		SmoothImageGetWidth				(SmoothImage *Image,
										SmoothInt *Width);
GE_INTERNAL SmoothBool		SmoothImageGetHeight				(SmoothImage *Image,
										SmoothInt *Height);

/* Canvas Map/Free A Color From The Colormap*/
GE_INTERNAL SmoothBool		SmoothCanvasAllocateColor			(SmoothCanvas Canvas, 
										SmoothColor *Color);
GE_INTERNAL SmoothBool		SmoothCanvasDeallocateColor			(SmoothCanvas Canvas, 
										SmoothColor *Color);
/* Canvas cache/uncache a Color */
GE_INTERNAL SmoothBool		SmoothCanvasCacheColor				(SmoothCanvas Canvas, 
										SmoothColor *Color);

GE_INTERNAL SmoothBool		SmoothCanvasUnCacheColor			(SmoothCanvas Canvas, 
										SmoothColor *Color);
									
/* Canvas cache/uncache a shaded Color */
GE_INTERNAL SmoothBool		SmoothCanvasCacheShadedColor			(SmoothCanvas Canvas, 
										SmoothColor Base,
										SmoothDouble Shade,
										SmoothColor *Color);

GE_INTERNAL SmoothBool		SmoothCanvasUnCacheShadedColor			(SmoothCanvas Canvas, 
										SmoothColor Base,
										SmoothDouble Shade,
										SmoothColor *Color);

/* Canvas cache/uncache a MidPoint Color */
GE_INTERNAL SmoothBool		SmoothCanvasCacheMidPointColor			(SmoothCanvas Canvas, 
										SmoothColor Point1,
										SmoothColor Color2,
										SmoothColor *Color);

GE_INTERNAL SmoothBool		SmoothCanvasUnCacheMidPointColor		(SmoothCanvas Canvas, 
										SmoothColor Point1,
										SmoothColor Point2,
										SmoothColor *Color);

/* Canvas Store/Render */
GE_INTERNAL SmoothBool		SmoothCanvasStore				(SmoothCanvas Canvas);
SmoothBool		SmoothCanvasRender				(SmoothCanvas Canvas,	
										GE_INTERNAL SmoothBool ClosePathAndFill);

/* Canvas Get/Set/Clear Clip Rectangle */
GE_INTERNAL SmoothBool		SmoothCanvasGetClipRectangle			(SmoothCanvas Canvas,
										SmoothRectangle* Rectangle);

GE_INTERNAL SmoothBool		SmoothCanvasSetClipRectangle			(SmoothCanvas Canvas,
										SmoothRectangle	Rectangle);

GE_INTERNAL SmoothBool		SmoothCanvasClearClipRectangle			(SmoothCanvas Canvas);

GE_INTERNAL SmoothBool		SmoothCanvasClipUseIntersectingRectangle	(SmoothCanvas Canvas,
										SmoothRectangle	Rectangle);

/* Canvas Get/Set Pen Properties */
GE_INTERNAL SmoothBool		SmoothCanvasGetPenColor				(SmoothCanvas Canvas, 
										SmoothColor *Color);
GE_INTERNAL SmoothBool		SmoothCanvasSetPenColor				(SmoothCanvas Canvas, 
										SmoothColor Color);

GE_INTERNAL SmoothBool		SmoothCanvasGetPenThickness			(SmoothCanvas Canvas,
										SmoothInt *Thickness);
GE_INTERNAL SmoothBool		SmoothCanvasSetPenThickness			(SmoothCanvas Canvas, 
										SmoothInt Thickness);

GE_INTERNAL SmoothBool		SmoothCanvasGetPenStyle				(SmoothCanvas Canvas, 
										SmoothLineStyle *Style);
GE_INTERNAL SmoothBool		SmoothCanvasSetPenStyle				(SmoothCanvas Canvas, 
										SmoothLineStyle Style);

GE_INTERNAL SmoothBool		SmoothCanvasGetPenJoin				(SmoothCanvas Canvas, 
										SmoothLineJoin *Join);
GE_INTERNAL SmoothBool		SmoothCanvasSetPenJoin				(SmoothCanvas Canvas, 
										SmoothLineJoin Join);

GE_INTERNAL SmoothBool		SmoothCanvasGetPenCap				(SmoothCanvas Canvas, 
										SmoothLineCap *Cap);
GE_INTERNAL SmoothBool		SmoothCanvasSetPenCap				(SmoothCanvas Canvas, 
										SmoothLineCap Cap);

GE_INTERNAL SmoothBool		SmoothCanvasGetPenValues			(SmoothCanvas Canvas, 
										SmoothColor *Color,
										SmoothInt *Thickness,
										SmoothLineStyle *Style,
										SmoothLineCap *Cap,
										SmoothLineJoin *Join);

GE_INTERNAL SmoothBool		SmoothCanvasSetPenValues			(SmoothCanvas Canvas, 
										SmoothColor Color,
										SmoothInt Thickness,
										SmoothLineStyle Style,
										SmoothLineCap Cap,
										SmoothLineJoin Join);

GE_INTERNAL SmoothBool		SmoothCanvasGetPenPattern			(SmoothCanvas Canvas, 
										SmoothLinePattern *Pattern);
GE_INTERNAL SmoothBool		SmoothCanvasSetPenPattern			(SmoothCanvas Canvas, 
										SmoothLinePattern Pattern);


/* Canvas Get/Set Brush Properties */
GE_INTERNAL SmoothBool		SmoothCanvasGetBrushColor			(SmoothCanvas Canvas, 
										SmoothColor *Color);
GE_INTERNAL SmoothBool		SmoothCanvasSetBrushColor			(SmoothCanvas Canvas, 
										SmoothColor Color);

/* Canvas Create/Destroy Image */
GE_INTERNAL SmoothBool		SmoothCanvasCreateImage				(SmoothCanvas Canvas,
										SmoothInt Width,
										SmoothInt Height,
										SmoothImage *Image);
										
GE_INTERNAL SmoothBool 		SmoothCanvasCreateImageFromBuffer		(SmoothCanvas Canvas,
										SmoothImageBuffer ImageBuffer,
										SmoothImage *Image);

GE_INTERNAL SmoothBool 		SmoothCanvasDestroyImage			(SmoothCanvas Canvas,
										SmoothImage *Image);
																				
/* Canvas Render Image/ImageBuffer */
GE_INTERNAL SmoothBool		SmoothCanvasRenderImage				(SmoothCanvas Canvas,
										SmoothImage Image,
										SmoothInt X,
										SmoothInt Y,
										SmoothInt Width,
										SmoothInt Height);
										

GE_INTERNAL SmoothBool		SmoothCanvasRenderImageBuffer			(SmoothCanvas Canvas,
										SmoothImageBuffer ImageBuffer,
										SmoothInt X,
										SmoothInt Y,
										SmoothInt Width,
										SmoothInt Height);

/* Canvas Render Tile */
GE_INTERNAL SmoothBool		SmoothCanvasRenderTile				(SmoothCanvas Canvas,
										SmoothTile Tile,
										SmoothInt X,
										SmoothInt Y,
										SmoothInt Width,
										SmoothInt Height);

/* Canvas Render Gradient */
GE_INTERNAL SmoothBool		SmoothCanvasRenderGradient			(SmoothCanvas Canvas,
										SmoothGradient Gradient,
										SmoothInt X,
										SmoothInt Y,
										SmoothInt Width,
										SmoothInt Height);

/* Canvas Draw Point(s) */
GE_INTERNAL SmoothBool		SmoothCanvasDrawPoint				(SmoothCanvas Canvas,
										SmoothInt X, 
										SmoothInt Y);

GE_INTERNAL SmoothBool		SmoothCanvasDrawPoints				(SmoothCanvas Canvas,
										SmoothPoint *Points, 
										SmoothInt NumberOfPoints);

/* Canvas Draw Line(s) */
GE_INTERNAL SmoothBool		SmoothCanvasDrawLine				(SmoothCanvas Canvas,
										SmoothInt X1, 
										SmoothInt Y1, 
										SmoothInt X2, 
										SmoothInt Y2);
	
GE_INTERNAL SmoothBool		SmoothCanvasDrawSegment				(SmoothCanvas Canvas,
										SmoothPoint Point1, 
										SmoothPoint Point2);
	
GE_INTERNAL SmoothBool		SmoothCanvasDrawLines				(SmoothCanvas Canvas,
										SmoothPoint *Points, 
										SmoothInt NumberOfPoints);

GE_INTERNAL SmoothBool		SmoothCanvasDrawSegments			(SmoothCanvas Canvas,
										SmoothPoint *Points, 
										SmoothInt NumberOfPoints);

/* Canvas Draw Arc(s)/Curve(s) */
GE_INTERNAL SmoothBool		SmoothCanvasDrawArc				(SmoothCanvas Canvas,
										SmoothInt X, 
										SmoothInt Y, 
										SmoothInt Width, 
										SmoothInt Height,
										SmoothDouble AnglePosition,
										SmoothDouble AngleLength);

/* Canvas Frame Shaped Areas */
GE_INTERNAL SmoothBool		SmoothCanvasFrameRectangle			(SmoothCanvas Canvas,
										SmoothInt X, 
										SmoothInt Y, 
										SmoothInt Width, 
										SmoothInt Height);

GE_INTERNAL SmoothBool		SmoothCanvasFramePolygon			(SmoothCanvas Canvas,
										SmoothPoint *Points, 
										SmoothInt NumberOfPoints);

GE_INTERNAL SmoothBool		SmoothCanvasFrameChord				(SmoothCanvas Canvas,
										SmoothInt X, 
										SmoothInt Y, 
										SmoothInt Width, 
										SmoothInt Height,
										SmoothDouble AnglePosition,
										SmoothDouble AngleLength);
/* Canvas Fill Shaped Areas */
GE_INTERNAL SmoothBool		SmoothCanvasFillRectangle			(SmoothCanvas Canvas,
										SmoothInt X, 
										SmoothInt Y, 
										SmoothInt Width, 
										SmoothInt Height);

GE_INTERNAL SmoothBool		SmoothCanvasFillPolygon				(SmoothCanvas Canvas,
										SmoothPoint *Points, 
										SmoothInt NumberOfPoints);

GE_INTERNAL SmoothBool		SmoothCanvasFillChord				(SmoothCanvas Canvas,
										SmoothInt X, 
										SmoothInt Y, 
										SmoothInt Width, 
										SmoothInt Height,
										SmoothDouble AnglePosition,
										SmoothDouble AngleLength);

/* Callback to Finalize Interface if Required - Always Last in Struct*/
GE_INTERNAL SmoothBool		SmoothFinalizeInterface();

#endif /* DRAWING_INTERFACE_H */
