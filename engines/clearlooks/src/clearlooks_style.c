#include <gtk/gtk.h>
#include <cairo.h>

#include "clearlooks_style.h"
#include "clearlooks_rc_style.h"
#include "clearlooks_draw.h"

#include <math.h>
#include <string.h>

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
		params->corners     = CL_CORNER_ALL;
		
		if (!params->active && widget && GTK_IS_TOGGLE_BUTTON (widget))
			params->active = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget));
		
		params->xthickness = style->xthickness;
		params->ythickness = style->ythickness;		
		
		/* I want to avoid to have to do this. I need it for GtkEntry, unless I
		   find out why it doesn't behave the way I expect it to. */
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

	if (DETAIL ("entry"))
	{
		WidgetParameters params;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		
		if (widget && (GTK_IS_COMBO (widget->parent) ||
		               GTK_IS_COMBO_BOX_ENTRY(widget->parent) ||
		               GTK_IS_SPIN_BUTTON (widget)))
		{
			width += style->xthickness;
		}
		

		
		clearlooks_draw_entry (cr, &clearlooks_style->colors, &params,
		                       x, y, width, height);
	}
	else if (DETAIL ("frame") && widget && GTK_IS_STATUSBAR (widget->parent))
	{
		SeparatorParameters separator;
		separator.horizontal = TRUE;
		
		clearlooks_draw_separator (cr, NULL, NULL, &separator,
		                           x, y, width, height);
	}
	else if (DETAIL ("frame"))
	{
		WidgetParameters params;
		FrameParameters  frame;
		
		frame.shadow  = shadow_type;
		frame.gap_x   = -1;                 /* No gap will be drawn */
		frame.border  = &colors->shade[5];
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CL_CORNER_NONE;
		
		clearlooks_draw_frame (cr, colors, &params, &frame,
		                       x, y, width, height);
	}
	else if (DETAIL ("scrolled_window"))
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
	else
	{
		WidgetParameters params;
		FrameParameters frame;

		printf("draw_shadow: %s %s\n", detail, widget ? G_OBJECT_TYPE_NAME (widget) : "(null");
			
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
draw_box_gap (DRAW_ARGS,
	          GtkPositionType gap_side,
	          gint            gap_x,
	          gint            gap_width)
{
	ClearlooksStyle  *clearlooks_style = CLEARLOOKS_STYLE (style);
	ClearlooksColors *colors = &clearlooks_style->colors;
	cairo_t          *cr;
	
	cr = clearlooks_begin_paint (window, NULL);

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
	printf("draw_handle: %s\n", detail);
	return;
	parent_class->draw_handle (style, window, state_type, shadow_type, area,
	                           widget, detail, x, y, width, height,
	                           orientation);
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
	
	pos += 2; /* don't ask me why.... */
	
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

	if (DETAIL ("menubar"))
	{
		WidgetParameters params;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);

		clearlooks_draw_menubar (cr, colors, &params,
		                         x, y, width, height);
	}
	else if (DETAIL ("button") && widget->parent &&
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
			if (params.xthickness > 2)
			{
				x--;
				width++;
			}
		}
		else
			params.corners    = CL_CORNER_ALL;
		
		if (DETAIL ("spinbutton"))
			params.disabled = GTK_WIDGET_STATE (widget) & GTK_STATE_INSENSITIVE;
		
		if (GTK_IS_TOGGLE_BUTTON (widget) &&
		    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)))
			params.active = TRUE;
		
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
	else if (DETAIL ("bar"))
	{
		WidgetParameters      params;
		ProgressBarParameters progressbar;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);

		if (widget && GTK_IS_PROGRESS_BAR (widget))
			progressbar.orientation = gtk_progress_bar_get_orientation (GTK_PROGRESS_BAR (widget));
		else
			progressbar.orientation = CL_ORIENTATION_LEFT_TO_RIGHT;
		
		cairo_reset_clip (cr);
				
		clearlooks_draw_progressbar_fill (cr, colors, &params, &progressbar,
		                                  x, y, width, height);
		
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
		
		clearlooks_draw_menuitem (cr, colors, &params,
		                          x, y, width, height);
	}
	else if (DETAIL ("hscrollbar") || DETAIL ("vscrollbar") || DETAIL ("slider"))
	{
		/* Temporary */
		WidgetParameters params;
		FrameParameters  frame;
		
		clearlooks_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CL_CORNER_NONE;
		
		frame.gap_x = -1;
		frame.border = (CairoColor*)&colors->shade[6];
		frame.shadow = CL_SHADOW_OUT;
		
		clearlooks_draw_frame (cr, colors, &params, &frame,
		                       x, y, width, height);
	}
	else if (DETAIL ("toolbar") || DETAIL ("handlebox_bin") || DETAIL ("dockitem_bin"))
	{
		clearlooks_draw_toolbar (cr, colors, NULL, x, y, width, height);
	}
	else if (DETAIL ("trough"))
	{
		
	}
	else
	{
		printf("draw_box: %s\n", detail);
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
	parent_class->draw_option (style, window, state_type, shadow_type, area,
	                           widget, detail, x, y, width, height);
}

static void
draw_tab (DRAW_ARGS)
{
	
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

/**************************************************************************/

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
draw_arrow (GtkStyle      *style,
                       GdkWindow     *window,
                       GtkStateType   state,
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
	parent_class->draw_arrow (style, window, state, shadow, area,
	                          widget, detail, arrow_type, fill,
	                          x, y, width, height);
}

static void
clearlooks_style_init_from_rc (GtkStyle * style,
			       GtkRcStyle * rc_style)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	double shades[] = {1.15, 0.93, 0.896, 0.85, 0.768, 0.665, 0.5, 0.4, 0.205};
	CairoColor spot_color;
	CairoColor bg_normal;
	double contrast;
	int i;
	
	bg_normal.r = style->bg[GTK_STATE_NORMAL].red   / 65535.0;
	bg_normal.g = style->bg[GTK_STATE_NORMAL].green / 65535.0;
	bg_normal.b = style->bg[GTK_STATE_NORMAL].blue  / 65535.0;

	parent_class->init_from_rc (style, rc_style);
	
	contrast = CLEARLOOKS_RC_STYLE (rc_style)->contrast;
	
	clearlooks_style->progressbarstyle  = CLEARLOOKS_RC_STYLE (rc_style)->progressbarstyle;
	clearlooks_style->menubarstyle      = CLEARLOOKS_RC_STYLE (rc_style)->menubarstyle;
	clearlooks_style->menuitemstyle     = CLEARLOOKS_RC_STYLE (rc_style)->menuitemstyle;
	clearlooks_style->listviewitemstyle = CLEARLOOKS_RC_STYLE (rc_style)->listviewitemstyle;
	
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
draw_focus (GtkStyle *style, GdkWindow *window, GtkStateType state_type,
            GdkRectangle *area, GtkWidget *widget, const gchar *detail,
            gint x, gint y, gint width, gint height)
{
	
}

static void
clearlooks_style_unrealize (GtkStyle * style)
{
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
