#include "support.h"

void clearlooks_treeview_get_header_index (GtkTreeView *tv, GtkWidget *header,
                                    gint *column_index, gint *columns,
                                    gboolean *resizable)
{
	GList *list, *list_start;
	*column_index = *columns = 0;
	list_start = list = gtk_tree_view_get_columns (tv);

	do
	{
		GtkTreeViewColumn *column = GTK_TREE_VIEW_COLUMN(list->data);
		if ( column->button == header )
		{
			*column_index = *columns;
			*resizable = column->resizable;
		}
		if ( column->visible )
			(*columns)++;
	} while ((list = g_list_next(list)));

	g_list_free (list_start);
}

void clearlooks_clist_get_header_index (GtkCList *clist, GtkWidget *button,
                                 gint *column_index, gint *columns)
{
	int i;
	*columns = clist->columns;
	
	for (i=0; i<*columns; i++)
	{
		if (clist->column[i].button == button)
		{
			*column_index = i;
			break;
		}
	}
}

void
clearlooks_get_parent_bg (const GtkWidget *widget, CairoColor *color)
{
	GtkStateType state_type;
	const GtkWidget *parent;
	GdkColor *gcolor;
	
	if (widget == NULL)
		return;
	
	parent = widget->parent;
	
	while (parent && GTK_WIDGET_NO_WINDOW (parent) && !((GTK_IS_NOTEBOOK (parent)) || (GTK_IS_TOOLBAR (parent))))
		parent = parent->parent;

	if (parent == NULL)
		return;
	
	state_type = GTK_WIDGET_STATE (parent);
	
	gcolor = &parent->style->bg[state_type];
	
	ge_gdk_color_to_cairo (gcolor, color);
}

ClearlooksStepper
clearlooks_scrollbar_get_stepper (GtkWidget    *widget,
                       GdkRectangle *stepper)
{
	ClearlooksStepper value = CL_STEPPER_UNKNOWN;
	GdkRectangle tmp;
	GdkRectangle check_rectangle;
	GtkOrientation orientation;

	if (!GE_IS_RANGE (widget))
		return CL_STEPPER_UNKNOWN;

	check_rectangle.x      = widget->allocation.x;
	check_rectangle.y      = widget->allocation.y;
	check_rectangle.width  = stepper->width;
	check_rectangle.height = stepper->height;
	
	orientation = GTK_RANGE (widget)->orientation;
	
	if (widget->allocation.x == -1 && widget->allocation.y == -1)
		return CL_STEPPER_UNKNOWN;
		
	if (gdk_rectangle_intersect (stepper, &check_rectangle, &tmp))
		value = CL_STEPPER_A;

	if (value == CL_STEPPER_UNKNOWN) /* Haven't found a match */
	{
		if (orientation == GTK_ORIENTATION_HORIZONTAL)
			check_rectangle.x = widget->allocation.x + stepper->width;
		else
			check_rectangle.y = widget->allocation.y + stepper->height;
		
		if (gdk_rectangle_intersect (stepper, &check_rectangle, &tmp))
			value = CL_STEPPER_B;
	}

	if (value == CL_STEPPER_UNKNOWN) /* Still haven't found a match */
	{
		if (orientation == GTK_ORIENTATION_HORIZONTAL)
			check_rectangle.x = widget->allocation.x + widget->allocation.width - (stepper->width * 2);
		else
			check_rectangle.y = widget->allocation.y + widget->allocation.height - (stepper->height * 2);
		
		if (gdk_rectangle_intersect (stepper, &check_rectangle, &tmp))
			value = CL_STEPPER_C;
	}

	if (value == CL_STEPPER_UNKNOWN) /* STILL haven't found a match */
	{
		if (orientation == GTK_ORIENTATION_HORIZONTAL)
			check_rectangle.x = widget->allocation.x + widget->allocation.width - stepper->width;
		else
			check_rectangle.y = widget->allocation.y + widget->allocation.height - stepper->height;
		
		if (gdk_rectangle_intersect (stepper, &check_rectangle, &tmp))
			value = CL_STEPPER_D;
	}
	
	return value;
}

ClearlooksStepper
clearlooks_scrollbar_visible_steppers (GtkWidget *widget)
{
	ClearlooksStepper steppers = 0;
	
	if (!GE_IS_RANGE (widget))
		return 0;
	
	if (GTK_RANGE (widget)->has_stepper_a)
		steppers |= CL_STEPPER_A;
	
	if (GTK_RANGE (widget)->has_stepper_b)
		steppers |= CL_STEPPER_B;

	if (GTK_RANGE (widget)->has_stepper_c)
		steppers |= CL_STEPPER_C;

	if (GTK_RANGE (widget)->has_stepper_d)
		steppers |= CL_STEPPER_D;

	return steppers;
}

ClearlooksJunction
clearlooks_scrollbar_get_junction (GtkWidget    *widget)
{	
	GtkAdjustment *adj;
	ClearlooksJunction junction = CL_JUNCTION_NONE;
	
	if (!GE_IS_RANGE (widget))
		return CL_JUNCTION_NONE;

	adj = GTK_RANGE (widget)->adjustment;
	
	if (adj->value <= adj->lower &&
		(GTK_RANGE (widget)->has_stepper_a || GTK_RANGE (widget)->has_stepper_b))
	{
		junction |= CL_JUNCTION_BEGIN;
	}
	
	if (adj->value >= adj->upper - adj->page_size &&
		(GTK_RANGE (widget)->has_stepper_c || GTK_RANGE (widget)->has_stepper_d))
	{
		junction |= CL_JUNCTION_END;
	}
	
	return junction;
}

void
clearlooks_set_toolbar_parameters (ToolbarParameters *toolbar, GtkWidget *widget, GdkWindow *window, gint x, gint y)
{
	toolbar->topmost = FALSE;

	if (x == 0 && y == 0) {
		if (widget->allocation.x == 0 && widget->allocation.y == 0)
		{
			if (widget->window == window && GE_IS_TOOLBAR (widget))
			{
				toolbar->topmost = TRUE;
			}
		}
	}
}

