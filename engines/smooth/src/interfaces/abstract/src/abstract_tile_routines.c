/******************************************************************************/
/* abstract_tile_routines.c - Default Tile Functions                          */
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
/* Portions Based on Nautilus/libnautilus-private                             */
/* nautilus-gdk-pixbuf-extensions.c -                                         */
/*     Darin Adler <darin@eazel.com>                                          */
/*     Ramiro Estrugo <ramiro@eazel.com>                                      */
/******************************************************************************/
/* Canvas Tile Image/ImageBuffer */
static SmoothBool
AbstractCanvasTileImageBuffer(SmoothCanvas Canvas,
						SmoothImageBuffer ImageBuffer,
						SmoothInt XOffset, 
						SmoothInt YOffset,
						SmoothInt X,
						SmoothInt Y,
						SmoothInt Width,
						SmoothInt Height)
{
	SmoothRectangle target, clip_area;
	
	SmoothInt target_x;
	SmoothInt target_y;

	SmoothInt minPoint_x;
	SmoothInt minPoint_y;
	SmoothInt maxPoint_x;
	SmoothInt maxPoint_y;
	
	SmoothInt num_left;
	SmoothInt num_above;

	SmoothInt tile_x;
	SmoothInt tile_y;
	SmoothInt tile_width;
	SmoothInt tile_height;

	if ((!(SmoothImageBufferGetWidth(&ImageBuffer, &tile_width))) || (!(SmoothImageBufferGetHeight(&ImageBuffer, &tile_height))))
	  return SmoothFalse;

	if (SmoothCanvasGetClipRectangle(Canvas, &clip_area))
	{
		SmoothRectangle tmp;       
		SmoothInt W, H;
      
		SmoothRectangleSetValues(&tmp, X+XOffset, Y+YOffset, Width, Height);
		SmoothRectangleFindIntersection(clip_area, tmp, &target);
      
		SmoothRectangleGetWidthValue(&target, &W);
		SmoothRectangleGetHeightValue(&target, &H);

		if ((W <= 0) && (H <= 0))
			return SmoothFalse;
	} 
	else 
	{
		SmoothRectangleSetValues(&target, X+XOffset, Y+YOffset, Width, Height);
  	}

	SmoothRectangleGetXValue(&target, &target_x);
	SmoothRectangleGetYValue(&target, &target_y);

	/* The number of tiles left and above the target area */
	num_left = target_x / tile_width;
	num_above = target_y / tile_height;
  
	minPoint_x = target_x + (num_left * tile_width) - tile_width;
	minPoint_y = target_y + (num_above * tile_height) - tile_height;
              	
	SmoothRectangleGetWidthValue(&target, &maxPoint_x);
	maxPoint_x = (target_x + maxPoint_x) + 2 * tile_width;

	SmoothRectangleGetHeightValue(&target, &maxPoint_y);
	maxPoint_y = (target_y + maxPoint_y) + 2 * tile_height;

	for (tile_y = minPoint_y; tile_y <= maxPoint_y; tile_y += tile_height) {
		for (tile_x = minPoint_x; tile_x <= maxPoint_x; tile_x += tile_width) {
			SmoothRectangle current;
			SmoothRectangle tmp;
			SmoothInt tmpx, tmpy;

			SmoothRectangleSetValues(&current, tile_x, tile_y, tile_width, tile_height);
			
			SmoothRectangleFindIntersection(target, current, &tmp);
        
			SmoothRectangleGetWidthValue(&tmp, &tmpx);
			SmoothRectangleGetHeightValue(&tmp, &tmpy);

			if ((tmpx > 0) && (tmpy > 0))
			{
				SmoothRectangleGetXValue(&tmp, &tmpx);
				SmoothRectangleGetYValue(&tmp, &tmpy);

				SmoothCanvasRenderImageBuffer(Canvas, ImageBuffer, 
									tmpx, tmpy,
									tile_width, tile_height);

			} 
		}
	}
	
	return SmoothTrue;	
}

static SmoothBool
AbstractCanvasTileImage(SmoothCanvas Canvas,
					SmoothImage Image,
					SmoothInt XOffset, 
					SmoothInt YOffset,
					SmoothInt X,
					SmoothInt Y,
					SmoothInt Width,
					SmoothInt Height)
{				
	SmoothRectangle target, clip_area;
	
	SmoothInt target_x;
	SmoothInt target_y;

	SmoothInt minPoint_x;
	SmoothInt minPoint_y;
	SmoothInt maxPoint_x;
	SmoothInt maxPoint_y;
	
	SmoothInt num_left;
	SmoothInt num_above;

	SmoothInt tile_x;
	SmoothInt tile_y;
	SmoothInt tile_width;
	SmoothInt tile_height;
	
	if ((!(SmoothImageGetWidth(&Image, &tile_width))) || (!(SmoothImageGetHeight(&Image, &tile_height))))
	  return SmoothFalse;

	if (SmoothCanvasGetClipRectangle(Canvas, &clip_area))
	{
		SmoothRectangle tmp;       
		SmoothInt W, H;
      
		SmoothRectangleSetValues(&tmp, X+XOffset, Y+YOffset, Width, Height);
		SmoothRectangleFindIntersection(clip_area, tmp, &target);
      
		SmoothRectangleGetWidthValue(&target, &W);
		SmoothRectangleGetHeightValue(&target, &H);

		if ((W <= 0) && (H <= 0))
			return SmoothFalse;
	} 
	else 
	{
		SmoothRectangleSetValues(&target, X+XOffset, Y+YOffset, Width, Height);
  	}

	SmoothRectangleGetXValue(&target, &target_x);
	SmoothRectangleGetYValue(&target, &target_y);

	/* The number of tiles left and above the target area */
	num_left = target_x / tile_width;
	num_above = target_y / tile_height;
  
	minPoint_x = target_x + (num_left * tile_width) - tile_width;
	minPoint_y = target_y + (num_above * tile_height) - tile_height;
              	
	SmoothRectangleGetWidthValue(&target, &maxPoint_x);
	maxPoint_x = (target_x + maxPoint_x) + 2 * tile_width;

	SmoothRectangleGetHeightValue(&target, &maxPoint_y);
	maxPoint_y = (target_y + maxPoint_y) + 2 * tile_height;

	for (tile_y = minPoint_y; tile_y <= maxPoint_y; tile_y += tile_height) {
		for (tile_x = minPoint_x; tile_x <= maxPoint_x; tile_x += tile_width) {
			SmoothRectangle current;
			SmoothRectangle tmp;
			SmoothInt tmpx, tmpy;

			SmoothRectangleSetValues(&current, tile_x, tile_y, tile_width, tile_height);
			
			SmoothRectangleFindIntersection(target, current, &tmp);

			SmoothRectangleGetWidthValue(&tmp, &tmpx);
			SmoothRectangleGetHeightValue(&tmp, &tmpy);

			if ((tmpx > 0) && (tmpy > 0))
			{
				SmoothRectangleGetXValue(&tmp, &tmpx);
				SmoothRectangleGetYValue(&tmp, &tmpy);
				
				SmoothCanvasRenderImage(Canvas, Image, 
								tmpx, tmpy,
								tile_width, tile_height);

			} 
		}
	}

	return SmoothTrue;
}

static void
AbstractCanvasRenderTile(SmoothCanvas *Canvas,
				SmoothTile Tile,
				SmoothInt X,
				SmoothInt Y,
				SmoothInt Width,
				SmoothInt Height)
{
	SmoothRectangle clip;
	SmoothImageBuffer ImageBuffer;
	SmoothImage Image;

	if (SmoothImageBufferLoadByFile(Tile.ImageFile, &ImageBuffer))
	{
		if (SmoothRectangleSetValues(&clip, X+Tile.XOffset, Y+Tile.YOffset, Width, Height) 
			&& SmoothCanvasClipUseIntersectingRectangle(Canvas, clip))
		{	
		  	if (Tile.UseBuffer)
	  		{
	  			if (SmoothCanvasCreateImageFromBuffer(Canvas, ImageBuffer, &Image))
	  			{
					AbstractCanvasTileImage(Canvas, Image, 
								Tile.XOffset, Tile.YOffset, 
								X, Y, Width, Height);
					SmoothCanvasDestroyImage(Canvas, &Image);
				}	
			}
			else 
			{
				AbstractCanvasTileImageBuffer(Canvas, ImageBuffer, 
								Tile.XOffset, Tile.YOffset, 
								X, Y, Width, Height);
			}
  
			SmoothCanvasClearClipRectangle(Canvas);
		}		
		
		SmoothImageBufferUnLoadByFile(Tile.ImageFile);
	}
}
