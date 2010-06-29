/* Clearlooks - a cairo based GTK+ engine
 * Copyright (C) 2006 Andrew Johnson <acjgenius@earthlink.net>
 * Copyright (C) 2007 Benjamin Berg <benjamin@sipsolutions.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Project contact: <gnome-themes-list@gnome.org>
 *
 */


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
		if ( gtk_tree_view_column_get_widget (column) == header )
		{
			*column_index = *columns;
			*resizable = gtk_tree_view_column_get_resizable (column);
		}
		if ( gtk_tree_view_column_get_visible (column) )
			(*columns)++;
	} while ((list = g_list_next(list)));

	g_list_free (list_start);
}

void
clearlooks_get_parent_bg (const GtkWidget *widget, CairoColor *color)
{
	GtkStateType state_type;
	const GtkWidget *parent;
	GdkColor *gcolor;
	gboolean stop;
	GtkStyle *style;
	
	if (widget == NULL)
		return;
	
	parent = gtk_widget_get_parent ((GtkWidget *) widget);
	stop = FALSE;
	
	while (parent && !stop)
	{
		stop = FALSE;

		stop |= !gtk_widget_get_has_window ((GtkWidget *) parent);
		stop |= GTK_IS_NOTEBOOK (parent) &&
		        gtk_notebook_get_show_tabs (GTK_NOTEBOOK (parent)) &&
		        gtk_notebook_get_show_border (GTK_NOTEBOOK (parent));

		if (GTK_IS_TOOLBAR (parent))
		{
			GtkShadowType shadow = GTK_SHADOW_OUT;
			gtk_widget_style_get (GTK_WIDGET (parent), "shadow-type", &shadow, NULL);
			
			stop |= (shadow != GTK_SHADOW_NONE);
		}

		if (!stop)
			parent = gtk_widget_get_parent ((GtkWidget *) parent);
	}

	if (parent == NULL)
		return;
	
	state_type = gtk_widget_get_state ((GtkWidget *) parent);

	style = gtk_widget_get_style ((GtkWidget *) parent);
	gcolor = &style->bg[state_type];
	
	ge_gdk_color_to_cairo (gcolor, color);
}

ClearlooksJunction
clearlooks_scrollbar_get_junction (GtkWidget    *widget)
{	
	GtkAdjustment *adj;
	ClearlooksJunction junction = CL_JUNCTION_NONE;
	
	if (!GE_IS_RANGE (widget))
		return CL_JUNCTION_NONE;

	adj = gtk_range_get_adjustment(GTK_RANGE (widget));

	/* XXX: fix the stepper detection, possibly by reading out the scrollbars style properties */
	if (gtk_adjustment_get_value(adj) <= gtk_adjustment_get_lower(adj) /*&&
		(GTK_RANGE (widget)->has_stepper_a || GTK_RANGE (widget)->has_stepper_b)*/)
	{
		if (!gtk_range_get_inverted (GTK_RANGE (widget)))
			junction |= CL_JUNCTION_BEGIN;
		else
			junction |= CL_JUNCTION_END;
	}
	
	if (gtk_adjustment_get_value(adj) >= gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj) /*&&
		(GTK_RANGE (widget)->has_stepper_c || GTK_RANGE (widget)->has_stepper_d)*/)
	{
		if (!gtk_range_get_inverted (GTK_RANGE (widget)))
			junction |= CL_JUNCTION_END;
		else
			junction |= CL_JUNCTION_BEGIN;
	}
	
	return junction;
}

void
clearlooks_set_toolbar_parameters (ToolbarParameters *toolbar,
                                   GtkWidget *widget,
                                   GdkWindow *window,
                                   gint x, gint y)
{
	GtkAllocation allocation;

	gtk_widget_get_allocation (widget, &allocation);
	toolbar->topmost = FALSE;

	if (x == 0 && y == 0) {
		if (widget && allocation.x == 0 && allocation.y == 0)
		{
			if (gtk_widget_get_window(widget) == window && GE_IS_TOOLBAR (widget))
			{
				toolbar->topmost = TRUE;
			}
		}
	}
}

void
clearlooks_get_notebook_tab_position (GtkWidget *widget,
                                      gboolean  *start,
                                      gboolean  *end)
{
	/* default value */
	*start = TRUE;
	*end = FALSE;

	if (GE_IS_NOTEBOOK (widget)) {
		gboolean found_tabs = FALSE;
		gint i, n_pages;
		GtkNotebook *notebook = GTK_NOTEBOOK (widget);

		/* got a notebook, so walk over all the tabs and decide based
		 * on that ...
		 * It works like this:
		 *   - If there is any visible tab that is expanded, set both.
		 *   - Set start/end if there is any visible tab that is at
		 *     the start/end.
		 *   - If one has the child_visibility set to false, arrows
		 *     are present; so none
		 * The heuristic falls over if there is a notebook that just
		 * happens to fill up all the available space. ie. All tabs
		 * are left aligned, but it does not require scrolling.
		 * (a more complex heuristic could calculate the tabs width
		 * and add them all up) */

		n_pages = gtk_notebook_get_n_pages (notebook);
		for (i = 0; i < n_pages; i++) {
			GtkWidget *tab_child;
			GtkWidget *tab_label;
			gboolean expand;
			GtkPackType pack_type;
						
			tab_child = gtk_notebook_get_nth_page (notebook, i);

			/* Skip invisible tabs */
			tab_label = gtk_notebook_get_tab_label (notebook, tab_child);
			if (!tab_label || !gtk_widget_get_visible (tab_label))
				continue;
			/* This is the same what the notebook does internally. */
			if (tab_label && !gtk_widget_get_child_visible (tab_label)) {
				/* One child is hidden because scroll arrows are present.
				 * So both corners are rounded. */
				*start = FALSE;
				*end = FALSE;
				return;
			}

                        gtk_container_child_get (GTK_CONTAINER (notebook), tab_child,
                                                 "expand", &expand,
                                                 "pack-type", &pack_type,
                                                 NULL);

			if (!found_tabs) {
				found_tabs = TRUE;
				*start = FALSE;
				*end = FALSE;
			}

			if (expand) {
				*start = TRUE;
				*end = TRUE;
			} else if (pack_type == GTK_PACK_START) {
				*start = TRUE;
			} else {
				*end = TRUE;
			}
		}
	}
}


