/******************************************************************************/
/* smooth_gtk_misc.c - Misc. Shared functions for all GTK+ versions           */
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
/* Portions Based on the Xenophilia Engine                                    */
/*   Johan Hanson <misagon@bahnhof.se>                                        */
/******************************************************************************/
#include ENGINE_HEADER

/* This function is based on a portion of Xenophilia's xeno_draw_extension */
gboolean
smooth_rounded_extension_points(gint x,
                         gint y, 
			 gint width, 
			 gint height,
			 gboolean selected,
			 gboolean fill,
			 GtkPositionType position,
			 GdkPoint points[8])
{
  gint x2 = x + width, y2 = y + height;
  switch (position) {
    case GTK_POS_BOTTOM:
      y2 += ((fill)?1:0) + ((selected)?0:-1);
      points[0].x = x2-1;	points[0].y = y2;
      points[1].x = x2-1;	points[1].y = y+5;
      points[2].x = x2-3;	points[2].y = y+2;
      points[3].x = x2-6;	points[3].y = y;
      points[4].x = x+5;	points[4].y = y;
      points[5].x = x+2;	points[5].y = y+2;
      points[6].x = x;		points[6].y = y+5;
      points[7].x = x;		points[7].y = y2;
      break;
	
    case GTK_POS_RIGHT:
      x2 += ((fill)?1:0) + ((selected)?0:-1);
      points[0].y = y;		points[0].x = x2;
      points[1].y = y;		points[1].x = x+5;
      points[2].y = y+2;	points[2].x = x+2;
      points[3].y = y+5;	points[3].x = x;
      points[4].y = y2-6;	points[4].x = x;
      points[5].y = y2-3;	points[5].x = x+2;
      points[6].y = y2-1;	points[6].x = x+5;
      points[7].y = y2-1;	points[7].x = x2;
      break;
			
    case GTK_POS_TOP:
      y -= ((fill)?1:0) + ((selected)?1:0);
      y2 += ((selected)?-1:0) - 1;
      points[0].x = x;		points[0].y = y;
      points[1].x = x;		points[1].y = y2-5;
      points[2].x = x+2;	points[2].y = y2-2;
      points[3].x = x+5;	points[3].y = y2;
      points[4].x = x2-6;	points[4].y = y2;
      points[5].x = x2-3;	points[5].y = y2-2;
      points[6].x = x2-1;	points[6].y = y2-5;
      points[7].x = x2-1;	points[7].y = y;
      break;
    
    case GTK_POS_LEFT:
      x -= ((fill)?1:0) + ((selected)?1:0);
      x2 += ((selected)?-1:0) - 1;
      points[0].y = y2-1;	points[0].x = x;
      points[1].y = y2-1;	points[1].x = x2-5;
      points[2].y = y2-3;	points[2].x = x2-2;
      points[3].y = y2-6;	points[3].x = x2;
      points[4].y = y+5;	points[4].x = x2;
      points[5].y = y+2;	points[5].x = x2-2;
      points[6].y = y;		points[6].x = x2-5;
      points[7].y = y;		points[7].x = x;
      break;

    default :
      return FALSE;     
  }  
  return TRUE;     
}

/* This function is based on a portion of Xenophilia's xeno_draw_extension */
gboolean
smooth_square_extension_points(gint x,
                         gint y, 
			 gint width, 
			 gint height,
			 gboolean selected,
			 gboolean fill,
			 GtkPositionType position,
			 GdkPoint points[8])
{
  gint x2 = x + width, y2 = y + height;

  switch (position) {
    case GTK_POS_BOTTOM:
      y2 += ((fill)?1:0) + ((selected)?0:-1);
      points[0].x = x2-1;	points[0].y = y2;
      points[1].x = x2-1;	points[1].y = y+1;
      points[2].x = x2-1;	points[2].y = y;
      points[3].x = x2-2;	points[3].y = y;
      points[4].x = x+1;	points[4].y = y;
      points[5].x = x;		points[5].y = y;
      points[6].x = x;		points[6].y = y+1;
      points[7].x = x;		points[7].y = y2;
      break;
	
    case GTK_POS_RIGHT:
      x2 += ((fill)?1:0) + ((selected)?0:-1);
      points[0].y = y;		points[0].x = x2;
      points[1].y = y;		points[1].x = x+1;
      points[2].y = y;		points[2].x = x;
      points[3].y = y+1;	points[3].x = x;
      points[4].y = y2-2;	points[4].x = x;
      points[5].y = y2-1;	points[5].x = x;
      points[6].y = y2-1;	points[6].x = x+1;
      points[7].y = y2-1;	points[7].x = x2;
      break;
			
    case GTK_POS_TOP:
      y -= ((fill)?1:0) + ((selected)?1:0);
      y2 += ((selected)?-1:0) - 1;
      points[0].x = x;		points[0].y = y;
      points[1].x = x;		points[1].y = y2-1;
      points[2].x = x;		points[2].y = y2;
      points[3].x = x+1;	points[3].y = y2;
      points[4].x = x2-2;	points[4].y = y2;
      points[5].x = x2-1;	points[5].y = y2;
      points[6].x = x2-1;	points[6].y = y2-1;
      points[7].x = x2-1;	points[7].y = y;
      break;
    
    case GTK_POS_LEFT:
      x -= ((fill)?1:0) + ((selected)?1:0);
      x2 += ((selected)?-1:0) - 1;
      points[0].y = y2-1;	points[0].x = x;
      points[1].y = y2-1;	points[1].x = x2-1;
      points[2].y = y2-1;	points[2].x = x2;
      points[3].y = y2-2;	points[3].x = x2;
      points[4].y = y+1;	points[4].x = x2;
      points[5].y = y;		points[5].x = x2;
      points[6].y = y;		points[6].x = x2-1;
      points[7].y = y;		points[7].x = x;
      break;
         
    default :
      return FALSE;     
  }  
  return TRUE;     
}

/* This function is based on a portion of Xenophilia's xeno_draw_extension */
gboolean
smooth_triangle_extension_points(gint x,
                         gint y, 
			 gint width, 
			 gint height,
			 gboolean selected,
			 gboolean fill,
			 GtkPositionType position,
			 GdkPoint points[8])
{
  gint x2 = x + width, y2 = y + height;
  gint i = 0;

  switch (position) {
    case GTK_POS_BOTTOM:
      i = (height - 5 + 2) / 3;

      if (!(i > 0)) return smooth_square_extension_points(x, y, width, height, selected, fill, position, points);

      y2 = y + i*3 + 5 + ((fill)?1:0) + ((selected)?0:-1);
         
      points[0].x = x2-1;	points[0].y = y2;
      points[1].x = x2-i-1;	points[1].y = y+4;
      points[2].x = x2-i-3;	points[2].y = y+2;
      points[3].x = x2-i-6;	points[3].y = y;
      points[4].x = x+i+5;	points[4].y = y;
      points[5].x = x+i+2;	points[5].y = y+2;
      points[6].x = x+i;	points[6].y = y+4;
      points[7].x = x;		points[7].y = y2;
      break;
	
    case GTK_POS_RIGHT:
      i = (width- 5 + 2) / 3;
      if (!(i > 0)) return smooth_square_extension_points(x, y, width, height, selected, fill, position, points);

      x2 = x + i*3 + 5 + ((fill)?1:0) + ((selected)?0:-1);
      points[0].y = y;		points[0].x = x2;
      points[1].y = y+i;	points[1].x = x+4;
      points[2].y = y+i+2;	points[2].x = x+2;
      points[3].y = y+i+5;	points[3].x = x;
      points[4].y = y2-i-6;	points[4].x = x;
      points[5].y = y2-i-3;	points[5].x = x+2;
      points[6].y = y2-i-1;	points[6].x = x+4;
      points[7].y = y2-1;	points[7].x = x2;
      break;

    case GTK_POS_TOP:
      i = (height - 5 + 2) / 3;
      if (!(i > 0)) return smooth_square_extension_points(x, y, width, height, selected, fill, position, points);

      y -= (i*3 + 5 - height) + ((fill)?1:0) + ((selected)?1:0);
      y2 += ((selected)?-1:0) - 1;
      points[0].x = x;		points[0].y = y;
      points[1].x = x+i;		points[1].y = y2-4;
      points[2].x = x+i+2;	points[2].y = y2-2;
      points[3].x = x+i+5;	points[3].y = y2;
      points[4].x = x2-i-6;	points[4].y = y2;
      points[5].x = x2-i-3;	points[5].y = y2-2;
      points[6].x = x2-i-1;	points[6].y = y2-4;
      points[7].x = x2-1;	points[7].y = y;
      break;
             
    case GTK_POS_LEFT:
      i = (width- 5 + 2) / 3;
      if (!(i > 0)) return smooth_square_extension_points(x, y, width, height, selected, fill, position, points);

      x -= (i*3 + 5 - width) + ((fill)?1:0) + ((selected)?1:0);
      x2 += ((selected)?-1:0) - 1;
      points[0].y = y2-1;	points[0].x = x;
      points[1].y = y2-i-1;	points[1].x = x2-4;
      points[2].y = y2-i-3;	points[2].x = x2-2;
      points[3].y = y2-i-6;	points[3].x = x2;
      points[4].y = y+i+5;	points[4].x = x2;
      points[5].y = y+i+2;	points[5].x = x2-2;
      points[6].y = y+i;	points[6].x = x2-4;
      points[7].y = y;		points[7].x = x;
      break;

    default :
      return smooth_square_extension_points(x, y, width, height, selected, fill, position, points);
  }  
  return TRUE;     
}
		
void
rounded_box_points(gint x,
                   gint y, 
  	           gint width, 
		   gint height,
		   GdkPoint points[13],
		   gboolean large)
{
  gint x2 = x + width - 1, y2 = y + height - 1;

  if (large) {
  	points[0].x = x+5;	points[0].y = y2;
  	points[1].x = x+2;	points[1].y = y2-2;
  	points[2].x = x;	points[2].y = y2-5;
  	points[3].x = x;	points[3].y = y+5;
  	points[4].x = x+2;	points[4].y = y+2;
  	points[5].x = x+5;	points[5].y = y;
  	points[6].x = x2-5;	points[6].y = y;
  	points[7].x = x2-2;	points[7].y = y+2;
  	points[8].x = x2;	points[8].y = y+5;
  	points[9].x = x2;	points[9].y = y2-5;
  	points[10].x = x2-2;	points[10].y = y2-2;
  	points[11].x = x2-5;	points[11].y = y2;
  	points[12].x = x+5;	points[12].y = y2;
   } else {
  	points[0].x = x+2;	points[0].y = y2;
  	points[1].x = x+1;	points[1].y = y2-1;
  	points[2].x = x;	points[2].y = y2-2;
  	points[3].x = x;	points[3].y = y+2;
  	points[4].x = x+1;	points[4].y = y+1;
  	points[5].x = x+2;	points[5].y = y;
  	points[6].x = x2-2;	points[6].y = y;
  	points[7].x = x2-1;	points[7].y = y+1;
  	points[8].x = x2;	points[8].y = y+2;
  	points[9].x = x2;	points[9].y = y2-2;
  	points[10].x = x2-1;	points[10].y = y2-1;
  	points[11].x = x2-2;	points[11].y = y2;
  	points[12].x = x+2;	points[12].y = y2;
   }
}
