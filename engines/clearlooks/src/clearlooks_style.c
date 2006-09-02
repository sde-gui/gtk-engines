#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#include <ge-support.h>
#include "clearlooks_style.h"
#include "clearlooks_rc_style.h"
#include "clearlooks_draw.h"
#include "support.h"

/* #define DEBUG 1 */

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
#include "animation.h"
#endif

static GtkStyleClass *clearlooks_parent_class;

static void
clearlooks_set_widget_parameters (const GtkWidget      *widget,
                                  const GtkStyle       *style,
                                  GtkStateType          state_type,
                                  WidgetParameters     *params)
{
	if (widget && IS_ENTRY (widget))
		state_type = GTK_WIDGET_STATE (widget);

	params->active     = (state_type == GTK_STATE_ACTIVE);
	params->prelight   = (state_type == GTK_STATE_PRELIGHT);
	params->disabled   = (state_type == GTK_STATE_INSENSITIVE);			
	params->state_type = (ClearlooksStateType)state_type;
	params->corners    = CL_CORNER_ALL;
	params->ltr        = ge_widget_is_ltr ((GtkWidget*)widget);
	params->focus      = widget && GTK_WIDGET_HAS_FOCUS (widget);
	params->is_default = widget && GTK_WIDGET_HAS_DEFAULT (widget);
		
	if (!params->active && widget && IS_TOGGLE_BUTTON (widget))
		params->active = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget));
		
	params->xthickness = style->xthickness;
	params->ythickness = style->ythickness;
		
	/* I want to avoid to have to do this. I need it for GtkEntry, unless I
	   find out why it doesn't behave the way I expect it to. */
	if (widget)
		clearlooks_get_parent_bg (widget, &params->parentbg);
}

static void
clearlooks_style_draw_flat_box (DRAW_ARGS)
{
	if (detail && 	
	    state_type == GTK_STATE_SELECTED && (
	    !strncmp ("cell_even", detail, 9) ||
	    !strncmp ("cell_odd", detail, 8)))
	{
		ClearlooksStyle  *clearlooks_style;
		ClearlooksColors *colors;
		cairo_t          *cr;

		cairo_pattern_t *pattern;
		CairoColor       lower_color;
		CairoColor       upper_color;

		CHECK_ARGS
		SANITIZE_SIZE

		clearlooks_style = CLEARLOOKS_STYLE (style);
		colors = &clearlooks_style->colors;
		cr = ge_gdk_drawable_to_cairo (window, area);

		cairo_translate (cr, x, y);

		if (GTK_WIDGET_HAS_FOCUS (widget))
		{
			upper_color = colors->base[state_type];
		}
		else
		{
			upper_color = colors->base[GTK_STATE_ACTIVE];
		}

		ge_shade_color(&upper_color, 0.8, &lower_color);

		pattern = cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.0, upper_color.r,
		                                                upper_color.g,
		                                                upper_color.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.0, lower_color.r,
		                                                lower_color.g,
		                                                lower_color.b);

		cairo_set_source (cr, pattern);
		cairo_rectangle  (cr, 0, 0, width, height);
		cairo_fill       (cr);

		cairo_pattern_destroy (pattern);
		cairo_destroy (cr);
	}
	else
	{
		clearlooks_parent_class->draw_flat_box (style, window, state_type,
		                             shadow_type,
		                             area, widget, detail,
		                             x, y, width, height);
	}
}

static void
clearlooks_style_draw_shadow (DRAW_ARGS)
{
	ClearlooksStyle  *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;
	cairo_t          *cr     = ge_gdk_drawable_to_cairo (window, area);

	CHECK_ARGS
	SANITIZE_SIZE

	if (DETAIL ("entry") && !(widget && widget->parent && IS_TREE_VIEW (widget->parent)))
	{
		WidgetParameters params;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
		if (widget && (ge_is_in_combo_box(widget) || IS_SPIN_BUTTON (widget)))
		{
			width += style->xthickness;
			if (!params.ltr)
				x -= style->xthickness;
			
			if (params.ltr)
				params.corners = CL_CORNER_TOPLEFT | CL_CORNER_BOTTOMLEFT;
			else
				params.corners = CL_CORNER_TOPRIGHT | CL_CORNER_BOTTOMRIGHT;
		}
		
		clearlooks_draw_entry (cr, &clearlooks_style->colors, &params,
		                       x, y, width, height);
	}
	else if (DETAIL ("frame") && widget && IS_STATUS_BAR (widget->parent))
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
	
		if (widget && !g_str_equal ("XfcePanelWindow", gtk_widget_get_name (gtk_widget_get_toplevel (widget))))
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
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CL_CORNER_NONE;
		
		clearlooks_draw_frame (cr, colors, &params, &frame, x, y, width, height);
	}
	
	cairo_destroy (cr);
}

static void 
clearlooks_style_draw_box_gap (DRAW_ARGS,
	          GtkPositionType gap_side,
	          gint            gap_x,
	          gint            gap_width)
{
	ClearlooksStyle  *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;
	cairo_t          *cr;
	
	cr = ge_gdk_drawable_to_cairo (window, area);

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
		/* if (widget)
			printf("box_gap: %s %s\n", detail, G_OBJECT_TYPE_NAME (widget)); */
		clearlooks_parent_class->draw_box_gap (style, window, state_type, shadow_type,
									   area, widget, detail,
									   x, y, width, height,
									   gap_side, gap_x, gap_width);
	}
	
	cairo_destroy (cr);	
}

static void
clearlooks_style_draw_extension (DRAW_ARGS, GtkPositionType gap_side)
{
	ClearlooksStyle  *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;
	cairo_t          *cr;

	cr = ge_gdk_drawable_to_cairo (window, area);
	
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
		/* printf("draw_extension: %s\n", detail); */
		clearlooks_parent_class->draw_extension (style, window, state_type, shadow_type, area,
		                              widget, detail, x, y, width, height,
		                              gap_side);

	}
	
	cairo_destroy (cr);
}

static void 
clearlooks_style_draw_handle (DRAW_ARGS, GtkOrientation orientation)
{
	ClearlooksStyle  *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;
	cairo_t          *cr;
	gboolean         is_horizontal;
	
	CHECK_ARGS
	SANITIZE_SIZE
	
	cr = ge_gdk_drawable_to_cairo (window, area);
	
	/* Evil hack to work around broken orientation for toolbars */
	is_horizontal = (width > height);
	
	if (DETAIL ("handlebox"))
	{
		WidgetParameters params;
		HandleParameters handle;

		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		handle.type = CL_HANDLE_TOOLBAR;
		handle.horizontal = is_horizontal;
		
		/* Is this ever true? -Daniel */
		if (IS_TOOLBAR (widget) && shadow_type != GTK_SHADOW_NONE)
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
		WidgetParameters params;
		HandleParameters handle;

		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		handle.type = CL_HANDLE_TOOLBAR;
		handle.horizontal = is_horizontal;
		
		/* Is this ever true? -Daniel */
		if (IS_TOOLBAR (widget) && shadow_type != GTK_SHADOW_NONE)
		{
			cairo_save (cr);
			clearlooks_draw_toolbar (cr, colors, &params, x, y, width, height);
			cairo_restore (cr);
		}
		
		clearlooks_draw_handle (cr, colors, &params, &handle,
		                        x, y, width, height);
/*
		clearlooks_parent_class->draw_handle (style, window, state_type, shadow_type, area,
		                           widget, detail, x, y, width, height,
		                           orientation);*/
	}

	cairo_destroy (cr);
}

static void
clearlooks_style_draw_box (DRAW_ARGS)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	const ClearlooksColors *colors;
	cairo_t *cr;

	cr     = ge_gdk_drawable_to_cairo (window, area);
	colors = &clearlooks_style->colors;

	if ((width == -1) && (height == -1))
		gdk_window_get_size (window, &width, &height);
	else if (width == -1)   
		gdk_window_get_size (window, &width, NULL);
	else if (height == -1)
		gdk_window_get_size (window, NULL, &height);

	if (DETAIL ("menubar") && !ge_is_panel_widget_item(widget))
	{
		WidgetParameters params;
		MenuBarParameters menubar;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);

		menubar.style = clearlooks_style->menubarstyle;

		clearlooks_draw_menubar (cr, colors, &params, &menubar,
		                         x, y, width, height);
	}
	else if (DETAIL ("button") && widget && widget->parent &&
                 (IS_TREE_VIEW(widget->parent) ||
                  IS_CLIST (widget->parent)))
	{
		WidgetParameters params;
		ListViewHeaderParameters header;
		
		gint columns, column_index;
		gboolean resizable = TRUE;
		
		/* XXX: make unknown treeview header CL_ORDER_MIDDLE */
		columns = 3;
		column_index = 1;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
		params.corners = CL_CORNER_NONE;
		
		if (IS_TREE_VIEW (widget->parent))
		{
			clearlooks_treeview_get_header_index (GTK_TREE_VIEW(widget->parent),
										   widget, &column_index, &columns,
										   &resizable);
		}
		else if (IS_CLIST (widget->parent))
		{
			clearlooks_clist_get_header_index (GTK_CLIST(widget->parent),
										widget, &column_index, &columns);
		}
		
		header.resizable = resizable;
		
		if (column_index == 0)
			header.order = params.ltr ? CL_ORDER_FIRST : CL_ORDER_LAST;
		else if (column_index == columns-1)
			header.order = params.ltr ? CL_ORDER_LAST : CL_ORDER_FIRST;
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

		if (ge_is_in_combo_box(widget))
		{
			if (params.ltr)
				params.corners = CL_CORNER_TOPRIGHT | CL_CORNER_BOTTOMRIGHT;
			else
				params.corners = CL_CORNER_TOPLEFT | CL_CORNER_BOTTOMLEFT;
			
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
				if (params.ltr)
					x--;
				width++;
			}			
		}
		else
			params.corners    = CL_CORNER_ALL;		
	
		if (IS_TOGGLE_BUTTON (widget) &&
		    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
			params.active = TRUE;
		
		/* Fix some firefox crap. */
		if (IS_BUTTON (widget) && IS_FIXED (widget->parent) && widget->allocation.x == -1 &&  widget->allocation.y == -1)
		{
			gtk_style_apply_default_background (widget->parent->style, window, TRUE, GTK_STATE_NORMAL,
			                                    area, x, y, width, height);
		}
		
		clearlooks_draw_button (cr, &clearlooks_style->colors, &params,
		                        x, y, width, height);
	}
	else if (DETAIL ("spinbutton_up") || DETAIL ("spinbutton_down"))
	{
		if (state_type == GTK_STATE_ACTIVE)
		{
			WidgetParameters params;
			clearlooks_set_widget_parameters (widget, style, state_type, &params);
			
			if (style->xthickness == 3)
			{
				width++;
				if (params.ltr)
					x--;
			}
			
			if (DETAIL ("spinbutton_up"))
			{
				height+=2;
				if (params.ltr)
					params.corners = CL_CORNER_TOPRIGHT;
				else
					params.corners = CL_CORNER_TOPLEFT;
			}
			else
			{
				if (params.ltr)
					params.corners = CL_CORNER_BOTTOMRIGHT;
				else
					params.corners = CL_CORNER_BOTTOMLEFT;
			}
				
			clearlooks_draw_spinbutton_down (cr, &clearlooks_style->colors, &params, x, y, width, height);
		}
	}
	else if (DETAIL ("spinbutton"))
	{
		WidgetParameters params;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
		if (params.ltr)
			params.corners = CL_CORNER_TOPRIGHT | CL_CORNER_BOTTOMRIGHT;
		else
			params.corners = CL_CORNER_TOPLEFT | CL_CORNER_BOTTOMLEFT;
		
		if (style->xthickness == 3)
		{
			if (params.ltr)
				x--;
			width++;
		}
		
		clearlooks_draw_spinbutton (cr, &clearlooks_style->colors, &params,
		                            x, y, width, height);
	}
	else if (DETAIL ("trough") && widget && IS_SCALE (widget))
	{
		GtkAdjustment *adjustment = gtk_range_get_adjustment (GTK_RANGE (widget));	
		WidgetParameters params;
		SliderParameters slider;
		gint slider_length;
		
		gtk_widget_style_get (widget, "slider-length", &slider_length, NULL);
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		params.corners    = CL_CORNER_NONE;
		
		slider.inverted   = gtk_range_get_inverted (GTK_RANGE (widget));
		slider.horizontal = (GTK_RANGE (widget)->orientation == GTK_ORIENTATION_HORIZONTAL);
		slider.fill_size  = ((slider.horizontal ? width : height) - slider_length) * (1 / ((adjustment->upper - adjustment->lower) / (adjustment->value - adjustment->lower))) + slider_length / 2;
		if (slider.horizontal && !params.ltr)
			slider.inverted = !slider.inverted;
		clearlooks_draw_scale_trough (cr, &clearlooks_style->colors,
		                              &params, &slider,
		                              x, y, width, height);
	}
	else if (DETAIL ("trough") && widget && IS_PROGRESS_BAR (widget))
	{
		WidgetParameters params;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);		
		
		clearlooks_draw_progressbar_trough (cr, colors, &params, 
		                                    x, y, width, height);
	}
	else if (DETAIL ("trough") && widget && (IS_VSCROLLBAR (widget) || IS_HSCROLLBAR (widget)))
	{
		WidgetParameters params;
		ScrollBarParameters scrollbar;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CL_CORNER_NONE;
		
		scrollbar.horizontal = TRUE;
		scrollbar.junction   = clearlooks_scrollbar_get_junction (widget);
		scrollbar.steppers   = clearlooks_scrollbar_visible_steppers (widget);
		
		if (IS_RANGE (widget))
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
		gdouble               elapsed = 0.0;

#ifdef HAVE_ANIMATION
		if(clearlooks_style->animation && CL_IS_PROGRESS_BAR (widget))
		{	
			gboolean activity_mode = GTK_PROGRESS (widget)->activity_mode;
			
		 	if (!activity_mode)
				clearlooks_animation_progressbar_add ((gpointer)widget);
		}

		elapsed = clearlooks_animation_elapsed (widget);
#endif
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);

		if (widget && IS_PROGRESS_BAR (widget))
		{
			progressbar.orientation = gtk_progress_bar_get_orientation (GTK_PROGRESS_BAR (widget));
			progressbar.value = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(widget));
			progressbar.pulsing = GTK_PROGRESS (widget)->activity_mode;
		}
		else
		{
			progressbar.orientation = CL_ORIENTATION_LEFT_TO_RIGHT;
			progressbar.value = 0;
			progressbar.pulsing = FALSE;
		}
		
		if (!params.ltr)
		{
			if (progressbar.orientation == GTK_PROGRESS_LEFT_TO_RIGHT)
				progressbar.orientation = GTK_PROGRESS_RIGHT_TO_LEFT;
			else if (progressbar.orientation == GTK_PROGRESS_RIGHT_TO_LEFT)
				progressbar.orientation = GTK_PROGRESS_LEFT_TO_RIGHT;
		}
		
		clearlooks_draw_progressbar_fill (cr, colors, &params, &progressbar,
		                                  x, y, width, height,
		                                  10 - (int)(elapsed * 10.0) % 10);
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

		ge_option_menu_get_props (widget, &indicator_size, &indicator_spacing);
		
		if (ge_widget_is_ltr (widget))
			optionmenu.linepos = width - (indicator_size.width + indicator_spacing.left + indicator_spacing.right) - style->xthickness;
		else
			optionmenu.linepos = (indicator_size.width + indicator_spacing.left + indicator_spacing.right) + style->xthickness;
		
		clearlooks_draw_optionmenu (cr, colors, &params, &optionmenu,
		                            x, y, width, height);
		                            
		
	}
	else if (DETAIL ("menuitem"))
	{
		WidgetParameters params;
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
		if (widget && IS_MENU_BAR (widget->parent))
		{
			/*params.corners = CL_CORNER_NONE;
			params.active = TRUE;
			params.state_type = CL_STATE_ACTIVE;
			params.xthickness = 2;
			params.ythickness = 2;
			
			clearlooks_draw_button (cr, colors, &params, x, y, width, height+1);*/
			
			params.corners = CL_CORNER_TOPLEFT | CL_CORNER_TOPRIGHT;
			height += 1;
		}
		else
		{	
			params.corners = CL_CORNER_ALL;
		}
		
		clearlooks_draw_menuitem (cr, colors, &params, x, y, width, height);
	}
	else if (DETAIL ("hscrollbar") || DETAIL ("vscrollbar") || DETAIL ("slider") || DETAIL ("stepper"))
	{
		WidgetParameters    params;
		ScrollBarParameters scrollbar;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CL_CORNER_NONE;
		
		scrollbar.has_color  = FALSE;
		scrollbar.horizontal = TRUE;
		scrollbar.junction   = clearlooks_scrollbar_get_junction (widget);
		scrollbar.steppers   = clearlooks_scrollbar_visible_steppers (widget);
		
		if (IS_RANGE (widget))
			scrollbar.horizontal = GTK_RANGE (widget)->orientation == GTK_ORIENTATION_HORIZONTAL;
		
		if (clearlooks_style->has_scrollbar_color)
		{
			ge_gdk_color_to_cairo (&clearlooks_style->scrollbar_color, &scrollbar.color);
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

			stepper.stepper = clearlooks_scrollbar_get_stepper (widget, &this_rectangle);

			clearlooks_draw_scrollbar_stepper (cr, colors, &params, &scrollbar, &stepper,
			                                   x, y, width, height);
		}
	}
	else if (DETAIL ("toolbar") || DETAIL ("handlebox_bin") || DETAIL ("dockitem_bin"))
	{
		/* Only draw the shadows on horizontal toolbars */
		if (shadow_type != GTK_SHADOW_NONE && height < 2*width )
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
		/* printf("draw_box: %s\n", detail); */
		clearlooks_parent_class->draw_box (style, window, state_type, shadow_type, area,
		                        widget, detail, x, y, width, height);
	}
	
	cairo_destroy (cr);
}

static void
clearlooks_style_draw_slider (DRAW_ARGS, GtkOrientation orientation)
{
	if (DETAIL ("hscale") || DETAIL ("vscale"))
	{
		clearlooks_style_draw_box (style, window, state_type, shadow_type, area,
		          widget, detail, x, y, width, height);
	}
	else
		clearlooks_parent_class->draw_slider (style, window, state_type, shadow_type, area,
		                           widget, detail, x, y, width, height, orientation);
}

static void
clearlooks_style_draw_option (DRAW_ARGS)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	CairoColor *border;
	CairoColor *dot;
	double trans = 1.0;
	gboolean inconsistent; 
	gboolean draw_bullet = (shadow_type == GTK_SHADOW_IN);

	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);
	cairo_pattern_t *pt;

	inconsistent = (IS_TOGGLE_BUTTON(widget) && gtk_toggle_button_get_inconsistent(TOGGLE_BUTTON(widget)));
	inconsistent |= (GTK_IS_CELL_RENDERER_TOGGLE(widget) && ge_cell_renderer_toggle_get_inconsistent(widget));
	inconsistent |= (CHECK_DETAIL(detail, "cellradio") && (shadow_type == GTK_SHADOW_ETCHED_IN));

	draw_bullet |= inconsistent;

#ifdef HAVE_ANIMATION
	if (clearlooks_style->animation)
		clearlooks_animation_connect_checkbox (widget);
#endif

	if (state_type == GTK_STATE_INSENSITIVE)
	{
		border = &clearlooks_style->colors.shade[5];
		dot    = &clearlooks_style->colors.shade[6];
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
	if (clearlooks_style->animation && IS_CHECK_BUTTON (widget) &&
        clearlooks_animation_is_animated (widget) &&
        !gtk_toggle_button_get_inconsistent (GTK_TOGGLE_BUTTON (widget)))
	{
		gfloat elapsed = clearlooks_animation_elapsed (widget);
	
		if (draw_bullet)
			trans = sqrt (sqrt (MIN(elapsed / CHECK_ANIMATION_TIME, 1.0)));	
		else
			trans = 1.0 - sqrt (sqrt (MIN(elapsed / CHECK_ANIMATION_TIME, 1.0)));
		
		draw_bullet = TRUE;
	}
#endif

	/* inconsistent state is missing? */
	if (draw_bullet)
	{
		if (inconsistent)
		{
			cairo_pattern_t *pt;

			cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
			cairo_set_line_width (cr, 4);

			cairo_move_to(cr, 5, 7);
			cairo_line_to(cr, 9, 7);

			cairo_set_source_rgba (cr, dot->r, dot->g, dot->b, trans);
			cairo_stroke (cr);

		}
		else
		{
			cairo_arc (cr, 7, 7, 3, 0, M_PI*2);
			/* cairo_set_source_rgb (cr, dot->r, dot->g, dot->b); */
			cairo_set_source_rgba (cr, dot->r, dot->g, dot->b,trans);
			cairo_fill (cr);
		
			cairo_arc (cr, 6, 6, 1, 0, M_PI*2);
			/* cairo_set_source_rgba (cr, 1,1,1, 0.5); */
			cairo_set_source_rgba (cr, 1,1,1, 0.5+trans);		
			cairo_fill (cr);
		}
	}

	cairo_destroy (cr);
}

static void
clearlooks_style_draw_check (DRAW_ARGS)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	CairoColor *border;
	CairoColor *dot;
	double trans = 1.0;
	gboolean inconsistent = FALSE;
	gboolean draw_bullet = (shadow_type == GTK_SHADOW_IN);
	cairo_t *cr;
	cairo_pattern_t *pt;

	inconsistent = (IS_TOGGLE_BUTTON(widget) && gtk_toggle_button_get_inconsistent(TOGGLE_BUTTON(widget)));
	inconsistent |= (GTK_IS_CELL_RENDERER_TOGGLE(widget) && ge_cell_renderer_toggle_get_inconsistent(widget));
	inconsistent |= (CHECK_DETAIL(detail, "cellcheck") && (shadow_type == GTK_SHADOW_ETCHED_IN));

	draw_bullet |= inconsistent;

	cr = ge_gdk_drawable_to_cairo (window, area);


#ifdef HAVE_ANIMATION
	if (clearlooks_style->animation)
		clearlooks_animation_connect_checkbox (widget);
#endif
	
	if (state_type == GTK_STATE_INSENSITIVE)
	{
		border = &clearlooks_style->colors.shade[5];
		dot    = &clearlooks_style->colors.shade[6];
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
	if (clearlooks_style->animation && IS_CHECK_BUTTON (widget) &&
	    clearlooks_animation_is_animated(widget) &&
	    !gtk_toggle_button_get_inconsistent (GTK_TOGGLE_BUTTON (widget)))
	{
		gfloat elapsed = clearlooks_animation_elapsed (widget);
	
		if (draw_bullet)
			trans = sqrt (sqrt (MIN(elapsed / CHECK_ANIMATION_TIME, 1.0)));
		else
			trans = 1.0 - sqrt (sqrt (MIN(elapsed / CHECK_ANIMATION_TIME, 1.0)));
		
		draw_bullet = TRUE;
	}
#endif

	if (draw_bullet)
	{
		if (inconsistent) /* Inconsistent */
		{
			cairo_set_line_width (cr, 2.0);
			cairo_move_to (cr, 3, height*0.5);
			cairo_line_to (cr, width-3, height*0.5);
		}
		else
		{
			cairo_set_line_width (cr, 1.7);
			cairo_move_to (cr, 0.5 + (width*0.2), (height*0.5));
			cairo_line_to (cr, 0.5 + (width*0.4), (height*0.7));
		
			cairo_curve_to (cr, 0.5 + (width*0.4), (height*0.7),
			                    0.5 + (width*0.5), (height*0.4),
			                    0.5 + (width*0.70), (height*0.25));
		}
		
		cairo_set_source_rgba (cr, dot->r, dot->g, dot->b, trans);
		cairo_stroke (cr);
	}
	cairo_destroy (cr);
}

static void
clearlooks_style_draw_tab (DRAW_ARGS)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;	
	WidgetParameters params;
	ArrowParameters  arrow;
	
	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);
	
	clearlooks_set_widget_parameters (widget, style, state_type, &params);
	arrow.type      = CL_ARROW_COMBO;
	arrow.direction = CL_DIRECTION_DOWN;
	
	clearlooks_draw_arrow (cr, colors, &params, &arrow, x, y, width, height);

	cairo_destroy (cr);
}

static void
clearlooks_style_draw_vline                      (GtkStyle               *style,
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

	cr = ge_gdk_drawable_to_cairo (window, area);
	
	clearlooks_draw_separator (cr, NULL, NULL, &separator,
	                           x, y1, 2, y2-y1);
	
	cairo_destroy (cr);
}

static void
clearlooks_style_draw_hline                      (GtkStyle               *style,
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

	cr = ge_gdk_drawable_to_cairo (window, area);
	
	SeparatorParameters separator;
	
	separator.horizontal = TRUE;
	
	clearlooks_draw_separator (cr, NULL, NULL, &separator,
	                           x1, y, x2-x1, 2);
	
	cairo_destroy (cr);
}

static void 
clearlooks_style_draw_shadow_gap (DRAW_ARGS,
                 GtkPositionType gap_side,
                 gint            gap_x,
                 gint            gap_width)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	const ClearlooksColors *colors;
	cairo_t *cr;

	cr     = ge_gdk_drawable_to_cairo (window, area);
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
		clearlooks_parent_class->draw_shadow_gap (style, window, state_type, shadow_type, area,
									   widget, detail, x, y, width, height,
									   gap_side, gap_x, gap_width);
	}
	
	cairo_destroy (cr);
}

static void
clearlooks_style_draw_resize_grip (GtkStyle       *style,
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
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;

	cairo_t *cr;

	WidgetParameters params;
	ResizeGripParameters grip;
	grip.edge = (ClearlooksWindowEdge)edge;

	g_return_if_fail (window != NULL);

	if (edge != GDK_WINDOW_EDGE_SOUTH_EAST)
		return; /* sorry... need to work on this :P */
	
	cr = ge_gdk_drawable_to_cairo (window, area);

	clearlooks_set_widget_parameters (widget, style, state_type, &params);	

	clearlooks_draw_resize_grip (cr, colors, &params, &grip,
	                             x, y, width, height);

	cairo_destroy (cr);

	return;
}

static void
clearlooks_style_draw_arrow (GtkStyle  *style,
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
	ClearlooksColors *colors;
	cairo_t *cr;
		
	CHECK_ARGS
	SANITIZE_SIZE

	if (arrow_type == (GtkArrowType)4)/* NONE - new in GTK 2.10 */
		return;

	colors = &clearlooks_style->colors;

	cr = ge_gdk_drawable_to_cairo (window, area);

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
		if (ge_is_combo_box (widget, FALSE) && !ge_is_combo_box_entry (widget))
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
		/* printf("draw_arrow: %s %s\n", detail, widget ? G_OBJECT_TYPE_NAME (widget) : "null"); */

		clearlooks_parent_class->draw_arrow (style, window, state_type, shadow, area,
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
	
	clearlooks_parent_class->init_from_rc (style, rc_style);

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
	ge_gdk_color_to_cairo (&style->bg[GTK_STATE_NORMAL], &bg_normal);

	for (i = 0; i < 9; i++)
	{
		ge_shade_color(&bg_normal, (shades[i]-0.7) * contrast + 0.7, &clearlooks_style->colors.shade[i]);
	}
		
	ge_gdk_color_to_cairo (&style->bg[GTK_STATE_SELECTED], &spot_color);
	
	ge_shade_color(&spot_color, 1.42, &clearlooks_style->colors.spot[0]);
	ge_shade_color(&spot_color, 1.05, &clearlooks_style->colors.spot[1]);
	ge_shade_color(&spot_color, 0.65, &clearlooks_style->colors.spot[2]);
	
	for (i=0; i<5; i++)
	{
		ge_gdk_color_to_cairo (&style->bg[i], &clearlooks_style->colors.bg[i]);

		ge_gdk_color_to_cairo (&style->base[i],&clearlooks_style->colors.base[i]);
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
clearlooks_style_draw_focus (GtkStyle *style, GdkWindow *window, GtkStateType state_type,
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

	CHECK_ARGS
	SANITIZE_SIZE

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
clearlooks_style_copy (GtkStyle * style, GtkStyle * src)
{
	ClearlooksStyle * cl_style = CLEARLOOKS_STYLE (style);
	ClearlooksStyle * cl_src = CLEARLOOKS_STYLE (src);
	
	cl_style->colors              = cl_src->colors;
	cl_style->progressbarstyle    = cl_src->progressbarstyle;
	cl_style->menubarstyle        = cl_src->menubarstyle;
	cl_style->menuitemstyle       = cl_src->menuitemstyle;
	cl_style->listviewitemstyle   = cl_src->listviewitemstyle;
	cl_style->scrollbar_color     = cl_src->scrollbar_color;
	cl_style->has_scrollbar_color = cl_src->has_scrollbar_color;
	cl_style->animation           = cl_src->animation;
	
	clearlooks_parent_class->copy (style, src);
}

static void
clearlooks_style_unrealize (GtkStyle * style)
{
	clearlooks_parent_class->unrealize (style);
}

static GdkPixbuf *
set_transparency (const GdkPixbuf *pixbuf, gdouble alpha_percent)
{
	GdkPixbuf *target;
	guchar *data, *current;
	guint x, y, rowstride, height, width;

	g_return_val_if_fail (pixbuf != NULL, NULL);
	g_return_val_if_fail (GDK_IS_PIXBUF (pixbuf), NULL);

	/* Returns a copy of pixbuf with it's non-completely-transparent pixels to
	   have an alpha level "alpha_percent" of their original value. */

	target = gdk_pixbuf_add_alpha (pixbuf, FALSE, 0, 0, 0);

	if (alpha_percent == 1.0)
		return target;
	width = gdk_pixbuf_get_width (target);
	height = gdk_pixbuf_get_height (target);
	rowstride = gdk_pixbuf_get_rowstride (target);
	data = gdk_pixbuf_get_pixels (target);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			/* The "4" is the number of chars per pixel, in this case, RGBA,
			   the 3 means "skip to the alpha" */
			current = data + (y * rowstride) + (x * 4) + 3; 
			*(current) = (guchar) (*(current) * alpha_percent);
		}
	}

	return target;
}

static GdkPixbuf*
scale_or_ref (GdkPixbuf *src,
              int width,
              int height)
{
	if (width == gdk_pixbuf_get_width (src) &&
	    height == gdk_pixbuf_get_height (src)) {
		return g_object_ref (src);
	} else {
		return gdk_pixbuf_scale_simple (src,
                                        width, height,
                                        GDK_INTERP_BILINEAR);
	}
}

static GdkPixbuf *
clearlooks_style_draw_render_icon (GtkStyle            *style,
             const GtkIconSource *source,
             GtkTextDirection     direction,
             GtkStateType         state,
             GtkIconSize          size,
             GtkWidget           *widget,
             const char          *detail)
{
	int width = 1;
	int height = 1;
	GdkPixbuf *scaled;
	GdkPixbuf *stated;
	GdkPixbuf *base_pixbuf;
	GdkScreen *screen;
	GtkSettings *settings;
	
	/* Oddly, style can be NULL in this function, because
	 * GtkIconSet can be used without a style and if so
	 * it uses this function.
	 */
	
	base_pixbuf = gtk_icon_source_get_pixbuf (source);
	
	g_return_val_if_fail (base_pixbuf != NULL, NULL);
	
	if (widget && gtk_widget_has_screen (widget)) {
		screen = gtk_widget_get_screen (widget);
		settings = gtk_settings_get_for_screen (screen);
	} else if (style->colormap) {
		screen = gdk_colormap_get_screen (style->colormap);
		settings = gtk_settings_get_for_screen (screen);
	} else {
		settings = gtk_settings_get_default ();
		GTK_NOTE (MULTIHEAD,
			  g_warning ("Using the default screen for gtk_default_render_icon()"));
	}
	
  
	if (size != (GtkIconSize) -1 && !gtk_icon_size_lookup_for_settings (settings, size, &width, &height)) {
		g_warning (G_STRLOC ": invalid icon size '%d'", size);
		return NULL;
	}

	/* If the size was wildcarded, and we're allowed to scale, then scale; otherwise,
	 * leave it alone.
	 */
	if (size != (GtkIconSize)-1 && gtk_icon_source_get_size_wildcarded (source))
		scaled = scale_or_ref (base_pixbuf, width, height);
	else
		scaled = g_object_ref (base_pixbuf);
	
	/* If the state was wildcarded, then generate a state. */
	if (gtk_icon_source_get_state_wildcarded (source)) {
		if (state == GTK_STATE_INSENSITIVE) {
			stated = set_transparency (scaled, 0.3);
#if 0
			stated =
				gdk_pixbuf_composite_color_simple (scaled,
								   gdk_pixbuf_get_width (scaled),
								   gdk_pixbuf_get_height (scaled),
								   GDK_INTERP_BILINEAR, 128,
								   gdk_pixbuf_get_width (scaled),
								   style->bg[state].pixel,
								   style->bg[state].pixel);
#endif
			gdk_pixbuf_saturate_and_pixelate (stated, stated,
							  0.1, FALSE);
			
			g_object_unref (scaled);
		} else if (state == GTK_STATE_PRELIGHT) {
			stated = gdk_pixbuf_copy (scaled);      
			
			gdk_pixbuf_saturate_and_pixelate (scaled, stated,
							  1.2, FALSE);
			
			g_object_unref (scaled);
		} else {
			stated = scaled;
		}
	}
	else
		stated = scaled;
  
  return stated;
}

static void
clearlooks_style_init (ClearlooksStyle * style)
{
}

static void
clearlooks_style_class_init (ClearlooksStyleClass * klass)
{
	GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);
	
	clearlooks_parent_class = g_type_class_peek_parent (klass);

	style_class->copy             = clearlooks_style_copy;
	style_class->unrealize        = clearlooks_style_unrealize;
	style_class->init_from_rc     = clearlooks_style_init_from_rc;
	style_class->draw_handle      = clearlooks_style_draw_handle;
	style_class->draw_slider      = clearlooks_style_draw_slider;
	style_class->draw_shadow_gap  = clearlooks_style_draw_shadow_gap;
	style_class->draw_focus       = clearlooks_style_draw_focus;
	style_class->draw_box         = clearlooks_style_draw_box;
	style_class->draw_shadow      = clearlooks_style_draw_shadow;
	style_class->draw_box_gap     = clearlooks_style_draw_box_gap;
	style_class->draw_extension   = clearlooks_style_draw_extension;
	style_class->draw_option      = clearlooks_style_draw_option;
	style_class->draw_check       = clearlooks_style_draw_check;
	style_class->draw_flat_box    = clearlooks_style_draw_flat_box;
	style_class->draw_tab         = clearlooks_style_draw_tab;
	style_class->draw_vline       = clearlooks_style_draw_vline;
	style_class->draw_hline       = clearlooks_style_draw_hline;
	style_class->draw_resize_grip = clearlooks_style_draw_resize_grip;
	style_class->draw_arrow       = clearlooks_style_draw_arrow;
	style_class->render_icon      = clearlooks_style_draw_render_icon;
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
