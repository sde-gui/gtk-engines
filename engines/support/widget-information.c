#include <gtk/gtk.h>

#include "widget-information.h"
#include <math.h>

/* Widget Type Lookups/Macros
   
   Based on/modified from functions in
   Smooth-Engine.
*/ 
gboolean
ge_object_is_a (const GtkWidget * widget, const gchar * type_name)
{
  gboolean result = FALSE;
 
  if ((widget))
    {
      GType tmp = g_type_from_name (type_name);
      if (tmp)
	result = g_type_check_instance_is_a ((GTypeInstance *) widget, tmp);
    }
 
  return result;
}
 
gboolean
ge_is_combo_box_entry (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent))
    {
      if (IS_COMBO_BOX_ENTRY (widget->parent))
	result = TRUE;
      else
	result = ge_is_combo_box_entry (widget->parent);
    }
  return result;
}
 
gboolean
ge_combo_box_is_using_list (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (IS_COMBO_BOX (widget->parent)))
    {
      gboolean *tmp = NULL;
 
      gtk_widget_style_get (widget, "appears-as-list", &result, NULL);
 
      if (tmp)
	result = *tmp;
    }
 
  return result;
}
 
gboolean
ge_is_combo_box (GtkWidget * widget, gboolean as_list)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent))
    {
      if (IS_COMBO_BOX (widget->parent))
        {
          if (as_list)
            result = (ge_combo_box_is_using_list(widget));
          else
            result = (!ge_combo_box_is_using_list(widget));
        }
      else
	result = ge_is_combo_box (widget->parent, as_list);
    }
  return result;
}
 
gboolean
ge_is_combo (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent))
    {
      if (IS_COMBO (widget->parent))
	result = TRUE;
      else
	result = ge_is_combo (widget->parent);
    }
  return result;
}
 
gboolean
ge_is_in_combo_box (GtkWidget * widget)
{
  return ((ge_is_combo (widget) || ge_is_combo_box (widget, TRUE) || ge_is_combo_box_entry (widget)));
}
 
gboolean
ge_is_toolbar_item (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent)) {
    if ((IS_BONOBO_TOOLBAR (widget->parent))
	|| (IS_BONOBO_DOCK_ITEM (widget->parent))
	|| (IS_EGG_TOOLBAR (widget->parent))
	|| (IS_TOOLBAR (widget->parent))
	|| (IS_HANDLE_BOX (widget->parent)))
      result = TRUE;
    else
      result = ge_is_toolbar_item (widget->parent);
  }
  return result;
}
 
gboolean
ge_is_panel_widget_item (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent))
    {
      if (IS_PANEL_WIDGET (widget->parent))
	result = TRUE;
      else
	result = ge_is_panel_widget_item (widget->parent);
    }
  return result;
}
 
gboolean 
ge_is_bonobo_dock_item (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget))
    {
      if (IS_BONOBO_DOCK_ITEM(widget) || IS_BONOBO_DOCK_ITEM (widget->parent))
	result = TRUE;
      else if (IS_BOX(widget) || IS_BOX(widget->parent))
        {
          GtkContainer *box = IS_BOX(widget)?GTK_CONTAINER(widget):GTK_CONTAINER(widget->parent);
          GList *children = NULL, *child = NULL;
 
          children = gtk_container_get_children(box);
              
          for (child = g_list_first(children); child; child = g_list_next(child))
            {
	      if (IS_BONOBO_DOCK_ITEM_GRIP(child->data))
	        {
	          result = TRUE;
	          child = NULL;
	        }
            }	            
         
          if (children)   
  	    g_list_free(children);
	}
    }
  return result;
}

static GtkWidget *
ge_find_combo_box_entry_widget (GtkWidget * widget)
{
  GtkWidget *result = NULL;

  if (widget)
    {
      if (IS_COMBO_BOX_ENTRY (widget))
	result = widget;
      else
	result = ge_find_combo_box_entry_widget (widget->parent);
    }

  return result;
}

static GtkWidget *
ge_find_combo_box_widget (GtkWidget * widget, gboolean as_list)
{
  GtkWidget *result = NULL;
 
  if (widget)
    {
      if (IS_COMBO_BOX (widget))
        {
          if (as_list)
            result = (ge_combo_box_is_using_list(widget))?widget:NULL;
          else
            result = (!ge_combo_box_is_using_list(widget))?widget:NULL;
        }
      else
	result = ge_find_combo_box_widget (widget->parent, as_list);
    }
  return result;
}
 
static GtkWidget *
ge_find_combo_widget (GtkWidget * widget)
{
  GtkWidget *result = NULL;
 
  if (widget)
    {
      if (IS_COMBO (widget))
	result = widget;
      else
	result = ge_find_combo_widget(widget->parent);
    }
  return result;
}

GtkWidget*
ge_find_combo_box_widget_parent (GtkWidget * widget)
{
   GtkWidget *result = NULL;
   
   if (!result)
     result = ge_find_combo_widget(widget);
  
   if (!result)
     result = ge_find_combo_box_widget(widget, TRUE);

   if (!result)
     result = ge_find_combo_box_entry_widget(widget);

  return result;
}

gboolean
ge_cell_renderer_toggle_get_inconsistent (GtkWidget * widget)
{
  gboolean result = FALSE;
  
  g_object_get (widget, "inconsistent", &result, NULL);
  
  return result;
}

/***********************************************
 * option_menu_get_props -
 *  
 *   Find Option Menu Size and Spacing
 *
 *   Taken from Smooth
 ***********************************************/ 
void
ge_option_menu_get_props (GtkWidget * widget,
		       GtkRequisition * indicator_size,
		       GtkBorder * indicator_spacing)
{
  GtkRequisition *tmp_size = NULL;
  GtkBorder *tmp_spacing = NULL;
 
  if ((widget) && IS_OPTION_MENU(widget))
    gtk_widget_style_get (widget,
			  "indicator_size", &tmp_size,
			  "indicator_spacing", &tmp_spacing, NULL);
 
  if (tmp_size)
    {
      *indicator_size = *tmp_size;
      g_free (tmp_size);
    }
  else
    *indicator_size = (GtkRequisition){ 9, 5 };
 
  if (tmp_spacing)
    {
      *indicator_spacing = *tmp_spacing;
      g_free (tmp_spacing);
    }
  else
    *indicator_spacing = (GtkBorder){ 7, 5, 2, 2 };
}
  
gboolean
ge_widget_is_ltr (GtkWidget *widget)
{
	GtkTextDirection dir = GTK_TEXT_DIR_NONE;
	
	if (GTK_IS_WIDGET (widget))
		dir = gtk_widget_get_direction (widget);

	if (dir == GTK_TEXT_DIR_NONE)
		dir = gtk_widget_get_default_direction ();

	if (dir == GTK_TEXT_DIR_RTL)
		return FALSE;
	else
		return TRUE;
}
