#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#include "clearlooks_style.h"
#include "clearlooks_rc_style.h"
#include "clearlooks_draw.h"
#include "support.h"

/* #define DEBUG 1 */

#define HAVE_ANIMATION 1

#define SCALE_SIZE 5

#define DETAIL(xx)   ((detail) && (!strcmp(xx, detail)))
#define COMPARE_COLORS(a,b) (a.red == b.red && a.green == b.green && a.blue == b.blue)

#define DRAW_ARGS    GtkStyle       *style, \
                     GdkWindow      *window, \
                     GtkStateType    state_type, \
                     GtkShadowType   shadow_type, \
                     GdkRectangle   *area, \
                     GtkWidget      *widget, \
                     const gchar    *detail, \
                     gint            x, \
                     gint            y, \
                     gint            width, \
                     gint            height

#ifdef HAVE_ANIMATION
#include "animation.c"
#endif

static GtkStyleClass *parent_class;

static cairo_t *
clearlooks_begin_paint (GdkDrawable  *window, GdkRectangle *area)
{
    cairo_t *cr;

    cr = (cairo_t*)gdk_cairo_create (window);

    if (area) {
        cairo_rectangle (cr, area->x, area->y, area->width, area->height);
        cairo_clip (cr);
        cairo_new_path (cr);
    }

    return cr;
}

static void
clearlooks_set_widget_parameters (const GtkWidget      *widget,
                                  const GtkStyle       *style,
                                  GtkStateType          state_type,
                                  WidgetParameters     *params)
{
	if (widget && GTK_IS_ENTRY (widget))
		state_type = GTK_WIDGET_STATE (widget);

	params->active     = (state_type == GTK_STATE_ACTIVE);
	params->prelight   = (state_type == GTK_STATE_PRELIGHT);
	params->disabled   = (state_type == GTK_STATE_INSENSITIVE);			
	params->state_type = (ClearlooksStateType)state_type;
	params->corners    = CL_CORNER_ALL;
		
	params->focus      = widget && GTK_WIDGET_HAS_FOCUS (widget);
	params->is_default = widget && GTK_WIDGET_HAS_DEFAULT (widget);
		
	if (!params->active && widget && GTK_IS_TOGGLE_BUTTON (widget))
		params->active = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget));
		
	params->xthickness = style->xthickness;
	params->ythickness = style->ythickness;
		
	/* I want to avoid to have to do this. I need it for GtkEntry, unless I
	   find out why it doesn't behave the way I expect it to. */
	if (widget)
		clearlooks_get_parent_bg (widget, &params->parentbg);
}

static void
draw_flat_box (DRAW_ARGS)
{
	parent_class->draw_flat_box (style, window, state_type,
	                             shadow_type,
	                             area, widget, detail,
	                             x, y, width, height);
}

static void
draw_shadow (DRAW_ARGS)
{
	ClearlooksStyle  *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;
	cairo_t          *cr     = clearlooks_begin_paint (window, area);

	sanitize_size (window, &width, &height);

	if (DETAIL ("entry") && !(widget && widget->parent && GTK_IS_TREE_VIEW (widget->parent)))
	{
		WidgetParameters params;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
		if (widget && (GTK_IS_COMBO (widget->parent) ||
		               GTK_IS_COMBO_BOX_ENTRY(widget->parent) ||
		               GTK_IS_SPIN_BUTTON (widget)))
		{
			width += style->xthickness;
			params.corners = CL_CORNER_TOPLEFT | CL_CORNER_BOTTOMLEFT;
		}
		
		clearlooks_draw_entry (cr, &clearlooks_style->colors, &params,
		                       x, y, width, height);
	}
	else if (DETAIL ("frame") && widget && GTK_IS_STATUSBAR (widget->parent))
	{
		WidgetParameters params;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);

		gtk_style_apply_default_background (style, window, TRUE, state_type,
		                                    area, x, y, width, height);
		
		clearlooks_draw_statusbar (cr, colors, &params,
		                           x, y, width, height);
	}
	else if (DETAIL ("frame"))
	{
		WidgetParameters params;
		FrameParameters  frame;
		frame.shadow  = shadow_type;
		frame.gap_x   = -1;                 /* No gap will be drawn */
		frame.border  = &colors->shade[4];
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CL_CORNER_NONE;
	
		if (!g_str_equal ("XfcePanelWindow", gtk_widget_get_name (gtk_widget_get_toplevel (widget))))
			clearlooks_draw_frame (cr, colors, &params, &frame,
			                       x, y, width, height);
	}
	else if (DETAIL ("scrolled_window") || DETAIL ("viewport") || detail == NULL)
	{
		CairoColor *border = (CairoColor*)&colors->shade[5];
		cairo_rectangle (cr, x+0.5, y+0.5, width-1, height-1);
		cairo_set_source_rgb (cr, border->r, border->g, border->b);
		cairo_set_line_width (cr, 1);
		cairo_stroke (cr);
	}
	else
	{
		WidgetParameters params;
		FrameParameters frame;

		frame.shadow = CL_SHADOW_NONE;
		frame.gap_x  = -1;
		frame.border = &colors->shade[5];
		//printf("draw_shadow: %s %s\n", detail, widget? G_OBJECT_TYPE_NAME (widget) : "null");
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CL_CORNER_NONE;
		
		clearlooks_draw_frame (cr, colors, &params, &frame, x, y, width, height);
	}
	
	cairo_destroy (cr);
}

static void 
draw_box_gap (DRAW_ARGS,
	          GtkPositionType gap_side,
	          gint            gap_x,
	          gint            gap_width)
{
	ClearlooksStyle  *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;
	cairo_t          *cr;
	
	cr = clearlooks_begin_paint (window, area);

	if (DETAIL ("notebook"))
	{
		WidgetParameters params;
		FrameParameters  frame;
		
		frame.shadow    = shadow_type;
		frame.gap_side  = gap_side;
		frame.gap_x     = gap_x;
		frame.gap_width = gap_width;
		frame.border    = &colors->shade[6];
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);

		params.corners = CL_CORNER_NONE;
		
		clearlooks_draw_frame (cr, colors, &params, &frame,
		                       x, y, width, height);
	}
	else
	{
		if (widget)
			printf("box_gap: %s %s\n", detail, G_OBJECT_TYPE_NAME (widget));
		parent_class->draw_box_gap (style, window, state_type, shadow_type,
									   area, widget, detail,
									   x, y, width, height,
									   gap_side, gap_x, gap_width);
	}
	
	cairo_destroy (cr);	
}

static void
draw_extension (DRAW_ARGS, GtkPositionType gap_side)
{
	ClearlooksStyle  *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;
	cairo_t          *cr;

	cr = clearlooks_begin_paint (window, area);
	
	if (DETAIL ("tab"))
	{
		WidgetParameters params;
		TabParameters    tab;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
		tab.gap_side = (ClearlooksGapSide)gap_side;
		
		if (gap_side == GTK_POS_BOTTOM)
			params.corners = CL_CORNER_TOPLEFT | CL_CORNER_TOPRIGHT;
		else if (gap_side == GTK_POS_TOP)
			params.corners = CL_CORNER_BOTTOMLEFT | CL_CORNER_BOTTOMRIGHT;
		
		clearlooks_draw_tab (cr, colors, &params, &tab,
		                     x, y, width, height);
	}
	else
	{
		printf("draw_extension: %s\n", detail);
		parent_class->draw_extension (style, window, state_type, shadow_type, area,
		                              widget, detail, x, y, width, height,
		                              gap_side);

	}
	
	cairo_destroy (cr);
}

static void 
draw_handle (DRAW_ARGS, GtkOrientation orientation)
{
	ClearlooksStyle  *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;
	cairo_t          *cr;
	
	sanitize_size (window, &width, &height);
	
	cr = clearlooks_begin_paint (window, area);
	
	if (DETAIL ("handlebox"))
	{
		WidgetParameters params;
		HandleParameters handle;

		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		handle.type = CL_HANDLE_TOOLBAR;
		handle.horizontal = FALSE;
		
		if (GTK_IS_TOOLBAR (widget) && shadow_type != GTK_SHADOW_NONE)
		{
			cairo_save (cr);
			clearlooks_draw_toolbar (cr, colors, &params, x, y, width, height);
			cairo_restore (cr);
		}
		
		clearlooks_draw_handle (cr, colors, &params, &handle,
		                        x, y, width, height);
	}
	else if (DETAIL ("paned"))
	{
		WidgetParameters params;
		HandleParameters handle;

		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		handle.type = CL_HANDLE_SPLITTER;
		handle.horizontal = orientation == GTK_ORIENTATION_HORIZONTAL;
			
		clearlooks_draw_handle (cr, colors, &params, &handle,
		                        x, y, width, height);
	}
	else
	{
//		printf ("draw_handle: %s %s\n", detail, widget ? G_OBJECT_TYPE_NAME (widget) : "null");
		WidgetParameters params;
		HandleParameters handle;

		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		handle.type = CL_HANDLE_TOOLBAR;
		handle.horizontal = FALSE;
		
		if (GTK_IS_TOOLBAR (widget) && shadow_type != GTK_SHADOW_NONE)
		{
			cairo_save (cr);
			clearlooks_draw_toolbar (cr, colors, &params, x, y, width, height);
			cairo_restore (cr);
		}
		
		clearlooks_draw_handle (cr, colors, &params, &handle,
		                        x, y, width, height);
/*
		parent_class->draw_handle (style, window, state_type, shadow_type, area,
		                           widget, detail, x, y, width, height,
		                           orientation);*/
	}

	cairo_destroy (cr);
}

static int
combo_box_get_seperator_pos (GtkWidget *widget)
{
	GList *children;
	int pos = 0;
	
	if (widget && GTK_IS_COMBO_BOX (widget->parent))
	{
		children = gtk_container_get_children (GTK_CONTAINER (widget));
		
		if (children && GTK_IS_HBOX (children->data))
			children = gtk_container_get_children (GTK_CONTAINER (children->data));

		if (children)
		{
			do
			{
				if (GTK_IS_ARROW (children->data) || GTK_IS_VSEPARATOR (children->data))
					pos += GTK_WIDGET (children->data)->allocation.width;
				
			} while ((children = g_list_next (children)));
		}
	}
	
	pos += 2; /* don't ask me why.... widget->style->xthickness - 1 ? */
	
	return pos;
}

static void
draw_box (DRAW_ARGS)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	const ClearlooksColors *colors;
	cairo_t *cr;

	cr     = clearlooks_begin_paint (window, area);
	colors = &clearlooks_style->colors;

	if ((width == -1) && (height == -1))
		gdk_window_get_size (window, &width, &height);
	else if (width == -1)   
		gdk_window_get_size (window, &width, NULL);
	else if (height == -1)
		gdk_window_get_size (window, NULL, &height);

	if (DETAIL ("menubar") && !(widget && cl_is_gnome_panel (widget->parent)))
	{
		WidgetParameters params;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);

		clearlooks_draw_menubar (cr, colors, &params,
		                         x, y, width, height);
	}
	else if (DETAIL ("button") && widget && widget->parent &&
                 (GTK_IS_TREE_VIEW(widget->parent) ||
                  GTK_IS_CLIST (widget->parent)))
	{
		WidgetParameters params;
		ListViewHeaderParameters header;
		
		gint columns, column_index;
		gboolean resizable = TRUE;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
		params.corners = CL_CORNER_NONE;
		
		if (GTK_IS_TREE_VIEW (widget->parent))
		{
			gtk_treeview_get_header_index (GTK_TREE_VIEW(widget->parent),
										   widget, &column_index, &columns,
										   &resizable);
		}
		else if (GTK_IS_CLIST (widget->parent))
		{
			gtk_clist_get_header_index (GTK_CLIST(widget->parent),
										widget, &column_index, &columns);
		}
		
		header.resizable = resizable;
		
		if (column_index == 0)
			header.order = CL_ORDER_FIRST;
		else if (column_index == columns-1)
			header.order = CL_ORDER_LAST;
		else
			header.order = CL_ORDER_MIDDLE;
		
		gtk_style_apply_default_background (style, window, FALSE, state_type, area, x, y, width, height);
		
		clearlooks_draw_list_view_header (cr, colors, &params, &header,
		                                  x, y, width, height);
	}
	else if (DETAIL ("button") || DETAIL ("buttondefault"))
	{
		WidgetParameters params;
		clearlooks_set_widget_parameters (widget, style, state_type, &params);

		if ((widget && (GTK_IS_COMBO_BOX_ENTRY (widget->parent) || GTK_IS_COMBO (widget->parent))))
		{
			params.corners = CL_CORNER_TOPRIGHT | CL_CORNER_BOTTOMRIGHT;
			
			/* Seriously, why can't non-gtk-apps at least try to be decent citizens?
			   Take this fscking OpenOffice.org 1.9 for example. The morons responsible
			   for this utter piece of crap gave the clip size wrong values! :'(  */
/*			cairo_reset_clip (cr);
			cairo_rectangle (cr, x+ 0.5, y+ 0.5, 10, 10);
			cairo_clip (cr);
			cairo_new_path (cr);
*/
			if (params.xthickness > 2)
			{
				x--;
				width++;
			}			
		}
		else
			params.corners    = CL_CORNER_ALL;		
	
		if (GTK_IS_TOGGLE_BUTTON (widget) &&
		    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
			params.active = TRUE;
		
		// Fix some firefox crap.
		if (GTK_IS_BUTTON (widget) && GTK_IS_FIXED (widget->parent) && widget->allocation.x == -1 &&  widget->allocation.y == -1)
		{
			gtk_style_apply_default_background (widget->parent->style, window, TRUE, GTK_STATE_NORMAL,
			                                    area, x, y, width, height);
		}
		
		clearlooks_draw_button (cr, &clearlooks_style->colors, &params,
		                        x, y, width, height);
	}
	else if (DETAIL ("spinbutton_up"))
	{
		if (state_type == GTK_STATE_ACTIVE)
		{
			WidgetParameters params;
			
			if (style->xthickness ==3)
			{
				x--;
				width++;
			}
			height+=2;
			
			params.state_type = (ClearlooksStateType)state_type;
			
			params.corners = CL_CORNER_TOPRIGHT;
			clearlooks_draw_spinbutton_down (cr, &clearlooks_style->colors, &params,
											 x, y, width, height);
		}
	}
	else if (DETAIL ("spinbutton_down"))
	{
		if (state_type == GTK_STATE_ACTIVE)
		{
			WidgetParameters params;
			params.state_type = (ClearlooksStateType)state_type;
			
			params.corners = CL_CORNER_BOTTOMRIGHT;
			
			if (style->xthickness ==3)
			{
				x--;
				width++;
			}
			clearlooks_draw_spinbutton_down (cr, &clearlooks_style->colors, &params,
											 x, y, width, height);
		}
	}
	else if (DETAIL ("spinbutton"))
	{
		WidgetParameters params;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
		params.corners = CL_CORNER_TOPRIGHT | CL_CORNER_BOTTOMRIGHT;
		
		if (style->xthickness == 3)
		{
			x--;
			width++;
		}
		
		clearlooks_draw_spinbutton (cr, &clearlooks_style->colors, &params,
		                            x, y, width, height);
	}
	else if (DETAIL ("trough") && widget && GTK_IS_SCALE (widget))
	{
		GtkAdjustment *adjustment = gtk_range_get_adjustment (GTK_RANGE (widget));	
		WidgetParameters params;
		SliderParameters slider;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		params.corners    = CL_CORNER_NONE;
		
		slider.inverted   = gtk_range_get_inverted (GTK_RANGE (widget));
		slider.horizontal = (GTK_RANGE (widget)->orientation == GTK_ORIENTATION_HORIZONTAL);
		slider.fill_size  = (slider.horizontal ? width: height) * (1 / ((adjustment->upper - adjustment->lower) / (adjustment->value - adjustment->lower)));	

		clearlooks_draw_scale_trough (cr, &clearlooks_style->colors,
		                              &params, &slider,
		                              x, y, width, height);
	}
	else if (DETAIL ("trough") && widget && GTK_IS_PROGRESS_BAR (widget))
	{
		WidgetParameters params;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);		
		
		clearlooks_draw_progressbar_trough (cr, colors, &params, 
		                                    x, y, width, height);
	}
	else if (DETAIL ("trough") && widget && (GTK_IS_VSCROLLBAR (widget) || GTK_IS_HSCROLLBAR (widget)))
	{
		WidgetParameters params;
		ScrollBarParameters scrollbar;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CL_CORNER_NONE;
		
		scrollbar.horizontal = TRUE;
		scrollbar.junction   = scrollbar_get_junction (widget);
		scrollbar.steppers   = scrollbar_visible_steppers (widget);
		
		if (GTK_IS_RANGE (widget))
			scrollbar.horizontal = GTK_RANGE (widget)->orientation == GTK_ORIENTATION_HORIZONTAL;
		
		if (scrollbar.horizontal)
		{
			x += 2;
			width -= 4;
		}
		else
		{
			y += 2;
			height -= 4;
		}
		
		clearlooks_draw_scrollbar_trough (cr, colors, &params, &scrollbar,
		                                  x, y, width, height);
	}
	else if (DETAIL ("bar"))
	{
		WidgetParameters      params;
		ProgressBarParameters progressbar;
		gint8                 frame = 0;

#ifdef HAVE_ANIMATION
		if(clearlooks_style->animation && CL_IS_PROGRESS_BAR (widget))
		{	
			gboolean activity_mode = GTK_PROGRESS (widget)->activity_mode;
			
		 	if (!activity_mode &&
			    gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR (widget)) != 1.0 &&
			    !cl_async_animation_lookup((gconstpointer)widget))
			{
				cl_progressbar_add ((gpointer)widget);
			}
		}
#endif
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);

		if (widget && GTK_IS_PROGRESS_BAR (widget))
			progressbar.orientation = gtk_progress_bar_get_orientation (GTK_PROGRESS_BAR (widget));
		else
			progressbar.orientation = CL_ORIENTATION_LEFT_TO_RIGHT;
		
		cairo_reset_clip (cr);
		
#ifdef HAVE_ANIMATION
		frame = 10 - cl_async_animation_getdata((gpointer)widget).frame;
#endif
		
		clearlooks_draw_progressbar_fill (cr, colors, &params, &progressbar,
		                                  x, y, width, height, frame);
	}
	else if (DETAIL ("hscale") || DETAIL ("vscale"))
	{
		WidgetParameters params;
		SliderParameters slider;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
		slider.horizontal = DETAIL ("hscale");
		
		clearlooks_draw_slider_button (cr, &clearlooks_style->colors,
		                               &params, &slider,
		                               x, y, width, height);
	}
	else if (DETAIL ("optionmenu"))
	{
		WidgetParameters params;
		OptionMenuParameters optionmenu;
		
		GtkRequisition indicator_size;
		GtkBorder indicator_spacing;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);

		option_menu_get_props (widget, &indicator_size, &indicator_spacing);
		
		if (widget && get_direction (widget) == GTK_TEXT_DIR_RTL)
			optionmenu.linepos = (indicator_size.width + indicator_spacing.left + indicator_spacing.right) + style->xthickness;
		else
			optionmenu.linepos = width - (indicator_size.width + indicator_spacing.left + indicator_spacing.right) - style->xthickness;
		
		clearlooks_draw_optionmenu (cr, colors, &params, &optionmenu,
		                            x, y, width, height);
		                            
		
	}
	else if (DETAIL ("menuitem"))
	{
		WidgetParameters params;
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
	/*	if (widget && GTK_IS_MENU_BAR (widget->parent))
		{
			params.corners = CL_CORNER_NONE;
			params.active = TRUE;
			params.state_type = CL_STATE_ACTIVE;
			params.xthickness = 2;
			params.ythickness = 2;
			
			clearlooks_draw_button (cr, colors, &params, x, y, width, height+1);
		}
		else*/
		{	
			WidgetParameters params;
			clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
			clearlooks_draw_menuitem (cr, colors, &params,
			                          x, y, width, height);
		}
	}
	else if (DETAIL ("hscrollbar") || DETAIL ("vscrollbar") || DETAIL ("slider") || DETAIL ("stepper"))
	{
		WidgetParameters    params;
		ScrollBarParameters scrollbar;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CL_CORNER_NONE;
		
		scrollbar.has_color  = FALSE;
		scrollbar.horizontal = TRUE;
		scrollbar.junction   = scrollbar_get_junction (widget);
		scrollbar.steppers   = scrollbar_visible_steppers (widget);
		
		if (GTK_IS_RANGE (widget))
			scrollbar.horizontal = GTK_RANGE (widget)->orientation == GTK_ORIENTATION_HORIZONTAL;
		
		if (clearlooks_style->has_scrollbar_color)
		{
			clearlooks_gdk_color_to_rgb (&clearlooks_style->scrollbar_color, &scrollbar.color.r, &scrollbar.color.g, &scrollbar.color.b);
			scrollbar.has_color = TRUE;
		}
		
		if (DETAIL ("slider"))
		{
			clearlooks_draw_scrollbar_slider (cr, colors, &params, &scrollbar,
			                                  x, y, width, height);
		}
		else
		{
			ScrollBarStepperParameters stepper;
			GdkRectangle this_rectangle = { x, y, width, height };

			stepper.stepper = scrollbar_get_stepper (widget, &this_rectangle);

			clearlooks_draw_scrollbar_stepper (cr, colors, &params, &scrollbar, &stepper,
			                                   x, y, width, height);
		}
	}
	else if (DETAIL ("toolbar") || DETAIL ("handlebox_bin") || DETAIL ("dockitem_bin"))
	{
		if (shadow_type != GTK_SHADOW_NONE)
			clearlooks_draw_toolbar (cr, colors, NULL, x, y, width, height);
	}
	else if (DETAIL ("trough"))
	{
		
	}
	else if (DETAIL ("menu"))
	{
		WidgetParameters params;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
		clearlooks_draw_menu_frame (cr, colors, &params, x, y, width, height);
	}
	else
	{
		//printf("draw_box: %s\n", detail);
		parent_class->draw_box (style, window, state_type, shadow_type, area,
		                        widget, detail, x, y, width, height);
	}
	
	cairo_destroy (cr);
}

static void
draw_slider (DRAW_ARGS, GtkOrientation orientation)
{
	if (DETAIL ("hscale") || DETAIL ("vscale"))
	{
		draw_box (style, window, state_type, shadow_type, area,
		          widget, detail, x, y, width, height);
	}
	else
		parent_class->draw_slider (style, window, state_type, shadow_type, area,
		                           widget, detail, x, y, width, height, orientation);
}

static void
draw_option (DRAW_ARGS)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	CairoColor *border;
	CairoColor *dot;
	double trans = 1.0;
	gboolean draw_bullet = (shadow_type == GTK_SHADOW_IN);

	cairo_t *cr = clearlooks_begin_paint (window, area);
	cairo_pattern_t *pt;

#ifdef HAVE_ANIMATION
	if(clearlooks_style->animation && GTK_IS_CHECK_BUTTON (widget) && !cl_async_animation_lookup((gconstpointer)widget)  && !g_slist_find (signaled_widgets, (gconstpointer)widget))
	{
			signaled_widgets = g_slist_append (signaled_widgets, widget);
			g_signal_connect ((GObject*)widget, "toggled", G_CALLBACK (cl_checkbox_toggle), widget);
			//printf("signal connected %d\n",widget);
	}
#endif

	if (state_type == GTK_STATE_INSENSITIVE)
	{
		border = &clearlooks_style->colors.shade[2];
		dot    = &clearlooks_style->colors.shade[2];
	}
	else
	{
		border = &clearlooks_style->colors.shade[7];
		dot    = &clearlooks_style->colors.spot[1];
	}
	pt = cairo_pattern_create_linear (0, 0, 13, 13);
	cairo_pattern_add_color_stop_rgba (pt, 0.0, 0, 0, 0, 0.1);
	cairo_pattern_add_color_stop_rgba (pt, 0.5, 0, 0, 0, 0);
	cairo_pattern_add_color_stop_rgba (pt, 0.5, 1, 1, 1, 0);
	cairo_pattern_add_color_stop_rgba (pt, 1.0, 1, 1, 1, 0.5);
	
	cairo_translate (cr, x, y);
	
	cairo_set_line_width (cr, 2);
	cairo_arc       (cr, 7, 7, 6, 0, M_PI*2);	
	cairo_set_source (cr, pt);
	cairo_stroke (cr);
	cairo_pattern_destroy (pt);

	cairo_set_line_width (cr, 1);

	cairo_arc       (cr, 7, 7, 5.5, 0, M_PI*2);	
	
	if (state_type != GTK_STATE_INSENSITIVE)
	{
		CairoColor *bg = &clearlooks_style->colors.base[0];
		cairo_set_source_rgb (cr, bg->r, bg->g, bg->b);
		cairo_fill_preserve (cr);
	}
	
	cairo_set_source_rgb (cr, border->r, border->g, border->b);
	cairo_stroke (cr);
	
#ifdef HAVE_ANIMATION
	if (clearlooks_style->animation && cl_async_animation_lookup((gconstpointer)widget) && GTK_IS_CHECK_BUTTON (widget))
	{
		int value = cl_async_animation_getdata((gpointer)widget).frame;
		
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
			trans = (float)(5-value)/5;
		else
			trans = (float)value/5;
		
		draw_bullet = TRUE;
	}
#endif

	if (draw_bullet)
	{
		cairo_arc (cr, 7, 7, 3, 0, M_PI*2);
		//cairo_set_source_rgb (cr, dot->r, dot->g, dot->b);
		cairo_set_source_rgba (cr, dot->r, dot->g, dot->b,trans);
		cairo_fill (cr);
		
		cairo_arc (cr, 6, 6, 1, 0, M_PI*2);
		//cairo_set_source_rgba (cr, 1,1,1, 0.5);
		cairo_set_source_rgba (cr, 1,1,1, 0.5+trans);		
		cairo_fill (cr);
	}

	cairo_destroy (cr);
}

static void
draw_check (DRAW_ARGS)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	CairoColor *border;
	CairoColor *dot;
	double trans = 1.0;
	gboolean draw_bullet = (shadow_type == GTK_SHADOW_IN);

	cairo_t *cr = clearlooks_begin_paint (window, area);
	cairo_pattern_t *pt;

#ifdef HAVE_ANIMATION
	if(clearlooks_style->animation && GTK_IS_CHECK_BUTTON (widget) && !cl_async_animation_lookup((gconstpointer)widget)  && !g_slist_find (signaled_widgets, (gconstpointer)widget))
	{
			signaled_widgets = g_slist_append (signaled_widgets, widget);
			g_signal_connect ((GObject*)widget, "toggled", G_CALLBACK (cl_checkbox_toggle), widget);
	}
#endif
	
	if (state_type == GTK_STATE_INSENSITIVE)
	{
		border = &clearlooks_style->colors.shade[2];
		dot    = &clearlooks_style->colors.shade[2];
	}
	else
	{
		border = &clearlooks_style->colors.shade[7];
		dot    = &clearlooks_style->colors.spot[1];
	}

	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1);
	
	if (style->xthickness > 2 && style->ythickness > 2)
	{
		/* Draw a gradient around the box so it appears sunken. */
		pt = cairo_pattern_create_linear (0, 0, 0, 13);
		cairo_pattern_add_color_stop_rgba (pt, 0.0, 0, 0, 0, 0.04);
		cairo_pattern_add_color_stop_rgba (pt, 0.5, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pt, 0.5, 1, 1, 1, 0);
		cairo_pattern_add_color_stop_rgba (pt, 1.0, 1, 1, 1, 0.4);
		
		cairo_rectangle (cr, 0.5, 0.5, width-1, height-1);
		cairo_set_source (cr, pt);
		cairo_stroke (cr);
		cairo_pattern_destroy (pt);
		
		/* Draw the rectangle for the checkbox itself */
		cairo_rectangle (cr, 1.5, 1.5, width-3, height-3);
	}
	else
	{
		cairo_rectangle (cr, 0.5, 0.5, width-1, height-1);
	}
	
	if (state_type != GTK_STATE_INSENSITIVE)
	{
		CairoColor *bg = &clearlooks_style->colors.base[0];
		cairo_set_source_rgb (cr, bg->r, bg->g, bg->b);
		cairo_fill_preserve (cr);
	}
	
	cairo_set_source_rgb (cr, border->r, border->g, border->b);
	cairo_stroke (cr);
		
#ifdef HAVE_ANIMATION
	if (clearlooks_style->animation && cl_async_animation_lookup((gconstpointer)widget))
	{
		int value = cl_async_animation_getdata((gpointer)widget).frame;
		
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
			trans = (float)(5-value)/5;
		else
			trans = (float)value/5;
		
		draw_bullet = TRUE;
	}
#endif

	if (draw_bullet)
	{
		cairo_set_line_width (cr, 1.7);
		cairo_move_to (cr, 0.5 + (width*0.2), (height*0.5));
		cairo_line_to (cr, 0.5 + (width*0.4), (height*0.7));
		
		cairo_curve_to (cr, 0.5 + (width*0.4), (height*0.7),
		                    0.5 + (width*0.5), (height*0.4),
		                    0.5 + (width*0.70), (height*0.25));
		
		
		cairo_set_source_rgba (cr, dot->r, dot->g, dot->b, trans);
		cairo_stroke (cr);
	}
	cairo_destroy (cr);
}

static void
draw_tab (DRAW_ARGS)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;	
	WidgetParameters params;
	ArrowParameters  arrow;
	
	cairo_t *cr = clearlooks_begin_paint (window, area);
	
	clearlooks_set_widget_parameters (widget, style, state_type, &params);
	arrow.type      = CL_ARROW_COMBO;
	arrow.direction = CL_DIRECTION_DOWN;
	
	clearlooks_draw_arrow (cr, colors, &params, &arrow, x, y, width, height);

	cairo_destroy (cr);
}

static void
draw_vline                      (GtkStyle               *style,
                                 GdkWindow              *window,
                                 GtkStateType            state_type,
                                 GdkRectangle           *area,
                                 GtkWidget              *widget,
                                 const gchar            *detail,
                                 gint                    y1,
                                 gint                    y2,
                                 gint                    x)
{
	SeparatorParameters separator = { FALSE };
	cairo_t *cr;

	cr = clearlooks_begin_paint (window, area);
	
	clearlooks_draw_separator (cr, NULL, NULL, &separator,
	                           x, y1, 2, y2-y1);
	
	cairo_destroy (cr);
}

static void
draw_hline                      (GtkStyle               *style,
                                 GdkWindow              *window,
                                 GtkStateType            state_type,
                                 GdkRectangle           *area,
                                 GtkWidget              *widget,
                                 const gchar            *detail,
                                 gint                    x1,
                                 gint                    x2,
                                 gint                    y)
{
	cairo_t *cr;

	cr = clearlooks_begin_paint (window, area);
	
	if (DETAIL ("label")) /* wtf? */
	{
		printf("draw_vline: label. ermm....?\n");
	}
	else
	{
		SeparatorParameters separator;
		
		separator.horizontal = TRUE;
		
		clearlooks_draw_separator (cr, NULL, NULL, &separator,
		                           x1, y, x2-x1, 2);
	}
	
	cairo_destroy (cr);
}

static void 
draw_shadow_gap (DRAW_ARGS,
                 GtkPositionType gap_side,
                 gint            gap_x,
                 gint            gap_width)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	const ClearlooksColors *colors;
	cairo_t *cr;

	cr     = clearlooks_begin_paint (window, area);
	colors = &clearlooks_style->colors;
	
	if (DETAIL ("frame"))
	{
		WidgetParameters params;
		FrameParameters  frame;
		
		frame.shadow    = shadow_type;
		frame.gap_side  = gap_side;
		frame.gap_x     = gap_x;
		frame.gap_width = gap_width;
		frame.border    = (CairoColor*)&colors->shade[5];
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);

		params.corners = CL_CORNER_NONE;
		
		clearlooks_draw_frame (cr, colors, &params, &frame,
		                       x, y, width, height);
	}
	else
	{
		parent_class->draw_shadow_gap (style, window, state_type, shadow_type, area,
									   widget, detail, x, y, width, height,
									   gap_side, gap_x, gap_width);
	}
	
	cairo_destroy (cr);
}

static void
draw_resize_grip (GtkStyle       *style,
                  GdkWindow      *window,
                  GtkStateType    state_type,
                  GdkRectangle   *area,
                  GtkWidget      *widget,
                  const gchar    *detail,
                  GdkWindowEdge   edge,
                  gint            x,
                  gint            y,
                  gint            width,
                  gint            height)
{
	return;
}

static void
draw_arrow (GtkStyle  *style,
                       GdkWindow     *window,
                       GtkStateType   state_type,
                       GtkShadowType  shadow,
                       GdkRectangle  *area,
                       GtkWidget     *widget,
                       const gchar   *detail,
                       GtkArrowType   arrow_type,
                       gboolean       fill,
                       gint           x,
                       gint           y,
                       gint           width,
                       gint           height)
{
	ClearlooksStyle  *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;
	cairo_t *cr = clearlooks_begin_paint (window, area);
		
	sanitize_size (window, &width, &height);

	if (DETAIL ("arrow"))
	{
		WidgetParameters params;
		ArrowParameters  arrow;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		arrow.type = CL_ARROW_NORMAL;
		arrow.direction = (ClearlooksDirection)arrow_type;
		
/*		cairo_rectangle (cr, x, y, width, height);
		cairo_set_source_rgb (cr, 1, 0, 0);
		cairo_fill (cr);
	*/	
		if (is_combo_box (widget))
		{
			arrow.type = CL_ARROW_COMBO;
			y -= 2;
			height += 4;
			x += 1;
		}
		
		clearlooks_draw_arrow (cr, colors, &params, &arrow,
		                       x, y, width, height);
	}
	else
	{
		//printf("draw_arrow: %s %s\n", detail, widget ? G_OBJECT_TYPE_NAME (widget) : "null");

		parent_class->draw_arrow (style, window, state_type, shadow, area,
		                          widget, detail, arrow_type, fill,
		                          x, y, width, height);
	}
	
	cairo_destroy (cr);
}

static void
clearlooks_style_init_from_rc (GtkStyle * style,
			       GtkRcStyle * rc_style)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	double shades[] = {1.15, 0.95, 0.896, 0.82, 0.7, 0.665, 0.5, 0.45, 0.4};
	CairoColor spot_color;
	CairoColor bg_normal;
	double contrast;
	int i;
	
	parent_class->init_from_rc (style, rc_style);

	bg_normal.r = style->bg[0].red   / 65535.0;
	bg_normal.g = style->bg[0].green / 65535.0;
	bg_normal.b = style->bg[0].blue  / 65535.0;

	contrast = CLEARLOOKS_RC_STYLE (rc_style)->contrast;
	
	clearlooks_style->progressbarstyle  = CLEARLOOKS_RC_STYLE (rc_style)->progressbarstyle;
	clearlooks_style->menubarstyle      = CLEARLOOKS_RC_STYLE (rc_style)->menubarstyle;
	clearlooks_style->menuitemstyle     = CLEARLOOKS_RC_STYLE (rc_style)->menuitemstyle;
	clearlooks_style->listviewitemstyle = CLEARLOOKS_RC_STYLE (rc_style)->listviewitemstyle;
	clearlooks_style->has_scrollbar_color = CLEARLOOKS_RC_STYLE (rc_style)->has_scrollbar_color;
	clearlooks_style->animation         = CLEARLOOKS_RC_STYLE (rc_style)->animation;
	
	if (clearlooks_style->has_scrollbar_color)
		clearlooks_style->scrollbar_color = CLEARLOOKS_RC_STYLE (rc_style)->scrollbar_color;
	
	/* Lighter to darker */
	for (i = 0; i < 9; i++)
	{
		shade (&bg_normal, &clearlooks_style->colors.shade[i],
		       (shades[i]-0.7) * contrast + 0.7);
	}
		
	spot_color.r = style->bg[GTK_STATE_SELECTED].red   / 65535.0;
	spot_color.g = style->bg[GTK_STATE_SELECTED].green / 65535.0;
	spot_color.b = style->bg[GTK_STATE_SELECTED].blue  / 65535.0;
	
	shade (&spot_color, &clearlooks_style->colors.spot[0], 1.42);
	shade (&spot_color, &clearlooks_style->colors.spot[1], 1.05);
	shade (&spot_color, &clearlooks_style->colors.spot[2], 0.65);
	
	for (i=0; i<5; i++)
	{
		clearlooks_gdk_color_to_rgb (&style->bg[i],
		                             &clearlooks_style->colors.bg[i].r,
		                             &clearlooks_style->colors.bg[i].g,
		                             &clearlooks_style->colors.bg[i].b);

		clearlooks_gdk_color_to_rgb (&style->base[i],
		                             &clearlooks_style->colors.base[i].r,
		                             &clearlooks_style->colors.base[i].g,
		                             &clearlooks_style->colors.base[i].b);
	}
}

static void
gdk_cairo_set_source_color_alpha (cairo_t  *cr,
                GdkColor *color, float alpha)
{
  g_return_if_fail (cr != NULL);
  g_return_if_fail (color != NULL);

  cairo_set_source_rgba (cr,
            color->red / 65535.,
            color->green / 65535.,
            color->blue / 65535.,
	        alpha);
}

static void
draw_focus (GtkStyle *style, GdkWindow *window, GtkStateType state_type,
            GdkRectangle *area, GtkWidget *widget, const gchar *detail,
            gint x, gint y, gint width, gint height)
{
	cairo_t *cr;
	gboolean free_dash_list = FALSE;
	gint line_width = 1;
	gint8 *dash_list = "\1\1";

	if (widget)
	{
		gtk_widget_style_get (widget,
				      "focus-line-width", &line_width,
				      "focus-line-pattern",
				      (gchar *) & dash_list, NULL);

		free_dash_list = TRUE;
	}

	if (detail && !strcmp (detail, "add-mode"))
	{
		if (free_dash_list)
			g_free (dash_list);

		dash_list = "\4\4";
		free_dash_list = FALSE;
	}

	sanitize_size (window, &width, &height);

	cr = gdk_cairo_create (window);

	if (detail && !strcmp (detail, "colorwheel_light"))
		cairo_set_source_rgb (cr, 0., 0., 0.);
	else if (detail && !strcmp (detail, "colorwheel_dark"))
		cairo_set_source_rgb (cr, 1., 1., 1.);
	else
		gdk_cairo_set_source_color_alpha (cr, &style->fg[state_type],
						  0.7);

	cairo_set_line_width (cr, line_width);

	if (dash_list[0])
	{
		gint n_dashes = strlen (dash_list);
		gdouble *dashes = g_new (gdouble, n_dashes);
		gdouble total_length = 0;
		gdouble dash_offset;
		gint i;

		for (i = 0; i < n_dashes; i++)
		{
			dashes[i] = dash_list[i];
			total_length += dash_list[i];
		}

		/* The dash offset here aligns the pattern to integer pixels
		 * by starting the dash at the right side of the left border
		 * Negative dash offsets in cairo don't work
		 * (https://bugs.freedesktop.org/show_bug.cgi?id=2729)
		 */
		dash_offset = -line_width / 2.;
		while (dash_offset < 0)
			dash_offset += total_length;

		cairo_set_dash (cr, dashes, n_dashes, dash_offset);
		g_free (dashes);
	}

	if (area)
	{
		gdk_cairo_rectangle (cr, area);
		cairo_clip (cr);
	}

	cairo_rectangle (cr,
			 x + line_width / 2.,
			 y + line_width / 2.,
			 width - line_width, height - line_width);
	cairo_stroke (cr);
	cairo_destroy (cr);

	if (free_dash_list)
		g_free (dash_list);
}

static void
clearlooks_style_unrealize (GtkStyle * style)
{
#ifdef HAVE_ANIMATION
	 while (signaled_widgets)
	{
		if(GTK_IS_CHECK_BUTTON (signaled_widgets->data))
			g_object_disconnect (signaled_widgets->data,
			                     "any_signal::toggled",
			                     G_CALLBACK (cl_checkbox_toggle),
			                     signaled_widgets->data, NULL);

		cl_async_animation_remove(signaled_widgets->data);
		signaled_widgets = g_slist_next (signaled_widgets);
	}

	if(async_widgets != NULL)
	{
		g_hash_table_foreach_remove(async_widgets, cl_disconnect, NULL);
		//g_hash_table_destroy(async_widgets);
	}
	
	if(async_widget_timer_id != 0)
	{
		g_source_remove(async_widget_timer_id);
		async_widget_timer_id = 0;
	}
#endif /* HAVE_ANIMATION */	

	parent_class->unrealize (style);
}

static void
clearlooks_style_init (ClearlooksStyle * style)
{
}

static void
clearlooks_style_class_init (ClearlooksStyleClass * klass)
{
	GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);
	
	parent_class = g_type_class_peek_parent (klass);

	style_class->unrealize        = clearlooks_style_unrealize;
	style_class->init_from_rc     = clearlooks_style_init_from_rc;
	style_class->draw_handle      = draw_handle;
	style_class->draw_slider      = draw_slider;
	style_class->draw_shadow_gap  = draw_shadow_gap;
	style_class->draw_focus       = draw_focus;
	style_class->draw_box         = draw_box;
	style_class->draw_shadow      = draw_shadow;
	style_class->draw_box_gap     = draw_box_gap;
	style_class->draw_extension   = draw_extension;
	style_class->draw_option      = draw_option;
	style_class->draw_check       = draw_check;
	style_class->draw_flat_box    = draw_flat_box;
	style_class->draw_tab         = draw_tab;
	style_class->draw_vline       = draw_vline;
	style_class->draw_hline       = draw_hline;
	style_class->draw_resize_grip = draw_resize_grip;
	style_class->draw_arrow       = draw_arrow;
}

GType clearlooks_type_style = 0;

void
clearlooks_style_register_type (GTypeModule * module)
{
	static const GTypeInfo object_info =
	{
		sizeof (ClearlooksStyleClass),
		(GBaseInitFunc) NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc) clearlooks_style_class_init,
		NULL,         /* class_finalize */
		NULL,         /* class_data */
		sizeof (ClearlooksStyle),
		0,            /* n_preallocs */
		(GInstanceInitFunc) clearlooks_style_init,
		NULL
	};

	clearlooks_type_style = g_type_module_register_type (module,
	                                                     GTK_TYPE_STYLE,
	                                                     "ClearlooksStyle",
	                                                     &object_info, 0);
}
