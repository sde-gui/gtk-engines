/* Redmond theme engine
 * Copyright (C) 2001 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Written by Owen Taylor <otaylor@redhat.com>
 * modified by Andrew Johnson <ajgenius@ajgenius.us>
 */  
#define CHECK_SIZE 13 
#define PART_SIZE 13 
 
typedef enum
{
  RADIO_BASE,
  RADIO_BLACK,
  RADIO_DARK,
  RADIO_LIGHT,
  RADIO_MID,
  RADIO_TEXT,
  RADIO_NONE
} Part;
  
/* Misc Drawing/Colour Functions */ 
void redmond_draw_part (GdkDrawable * drawable, 
                        GdkGC * gc, 
                        GdkRectangle * area, 
                        gint x, 
                        gint y, 
                        Part part);
 
void do_redmond_draw_default_fill (GtkStyle *style, 
                                   GdkWindow *window, 
                                   GtkStateType state_type, 
                                   GdkRectangle *area, 
                                   gint x, 
                                   gint y, 
                                   gint width, 
                                   gint height);
                                   
void do_redmond_draw_cross_hatch_fill (GtkStyle * style, 
                                       GdkWindow * window, 
                                       GtkStateType state, 
                                       GdkRectangle * area, 
                                       Part part, 
                                       gint x, 
                                       gint y, 
                                       gint width, 
                                       gint height);
                                       
void do_redmond_draw_check (cairo_t *cr,
                            CairoColor *color, 
                            gint x, 
                            gint y, 
                            gint width, 
                            gint height);
                            
void do_redmond_draw_arrow (cairo_t *cr,
                            CairoColor *color,
                            GtkArrowType arrow_type, 
                            gint x, 
                            gint y, 
                            gint width, 
                            gint height);
                            
void do_redmond_draw_line(cairo_t *cr, 
                          CairoColor * dark, 
                          CairoColor * light, 
                          GdkRectangle * area, 
                          gint start, 
                          gint end, 
                          gint base, 
                          gboolean horizontal);
 
void gtk_menu_shell_setup_signals(GtkWidget *widget);
void gtk_menu_shell_cleanup_signals(GtkWidget *widget);
