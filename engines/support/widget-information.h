
/* Object Type Lookups/Macros
   
   Based on/modified from functions in
   Smooth-Engine.
*/ 
#define GE_IS_WIDGET(object) ((object)  && ge_object_is_a (object, "GtkWidget"))
#define GE_IS_CONTAINER(object) ((object)  && ge_object_is_a (object, "GtkContainer"))
#define GE_IS_BIN(object) ((object)  && ge_object_is_a (object, "GtkBin"))

#define GE_IS_ARROW(object) ((object)  && ge_object_is_a (object, "GtkArrow"))

#define GE_IS_SEPARATOR(object) ((object)  && ge_object_is_a (object, "GtkSeparator"))
#define GE_IS_VSEPARATOR(object) ((object)  && ge_object_is_a (object, "GtkVSeparator"))
#define GE_IS_HSEPARATOR(object) ((object)  && ge_object_is_a (object, "GtkHSeparator"))
 
#define GE_IS_HANDLE_BOX(object) ((object) && ge_object_is_a (object, "GtkHandleBox"))
#define GE_IS_HANDLE_BOX_ITEM(object) ((object) && GE_IS_HANDLE_BOX(object->parent))
#define GE_IS_BONOBO_DOCK_ITEM(object) ((object) && ge_object_is_a (object, "BonoboDockItem"))
#define GE_IS_BONOBO_DOCK_ITEM_GRIP(object) ((object) && ge_object_is_a (object, "BonoboDockItemGrip"))
#define GE_IS_BONOBO_TOOLBAR(object) ((object) && ge_object_is_a (object, "BonoboUIToolbar"))
#define GE_IS_EGG_TOOLBAR(object) ((object) && ge_object_is_a (object, "Toolbar"))
#define GE_IS_TOOLBAR(object) ((object) && ge_object_is_a (object, "GtkToolbar"))
#define GE_IS_PANEL_WIDGET(object) ((object) && (ge_object_is_a (object, "PanelWidget") || ge_object_is_a (object, "PanelApplet")))

#define GE_IS_COMBO_BOX_ENTRY(object) ((widget) && ge_object_is_a (widget, "GtkComboBoxEntry"))
#define GE_IS_COMBO_BOX(object) ((widget) && ge_object_is_a (widget, "GtkComboBox"))
#define GE_IS_COMBO(object) ((widget) && ge_object_is_a (widget, "GtkCombo"))
#define GE_IS_OPTION_MENU(object) ((widget) && ge_object_is_a (widget, "GtkOptionMenu"))
 
#define GE_IS_TOGGLE_BUTTON(object) ((object) && ge_object_is_a (object, "GtkToggleButton"))
#define GE_IS_CHECK_BUTTON(object) ((object) && ge_object_is_a (object, "GtkCheckButton"))
#define GE_IS_SPIN_BUTTON(object) ((object) && ge_object_is_a (object, "GtkSpinButton"))
 
#define GE_IS_STATUS_BAR(object) ((object) && ge_object_is_a (object, "GtkStatusbar"))
#define GE_IS_PROGRESS_BAR(object) ((object) && ge_object_is_a (object, "GtkProgressBar"))
 
#define GE_IS_MENU_SHELL(object) ((object) && ge_object_is_a (object, "GtkMenuShell"))
#define GE_IS_MENU(object) ((object) && ge_object_is_a (object, "GtkMenu"))
#define GE_IS_MENU_BAR(object) ((object) && ge_object_is_a (object, "GtkMenuBar"))
#define GE_IS_MENU_ITEM(object) ((object) && ge_object_is_a (object, "GtkMenuItem"))

#define GE_IS_CHECK_MENU_ITEM(object) ((object) && ge_object_is_a (object, "GtkCheckMenuItem"))

#define GE_IS_RANGE(object) ((object) && ge_object_is_a (object, "GtkRange"))
 
#define GE_IS_SCROLLBAR(object) ((object) && ge_object_is_a (object, "GtkScrollbar"))
#define GE_IS_VSCROLLBAR(object) ((object) && ge_object_is_a (object, "GtkVScrollbar"))
#define GE_IS_HSCROLLBAR(object) ((object) && ge_object_is_a (object, "GtkHScrollbar"))
 
#define GE_IS_SCALE(object) ((object) && ge_object_is_a (object, "GtkScale"))
#define GE_IS_VSCALE(object) ((object) && ge_object_is_a (object, "GtkVScale"))
#define GE_IS_HSCALE(object) ((object) && ge_object_is_a (object, "GtkHScale"))
  
#define GE_IS_PANED(object) ((object) && ge_object_is_a (object, "GtkPaned"))
#define GE_IS_VPANED(object) ((object) && ge_object_is_a (object, "GtkVPaned"))
#define GE_IS_HPANED(object) ((object) && ge_object_is_a (object, "GtkHPaned"))
 
#define GE_IS_BOX(object) ((object) && ge_object_is_a (object, "GtkBox"))
#define GE_IS_VBOX(object) ((object) && ge_object_is_a (object, "GtkVBox"))
#define GE_IS_HBOX(object) ((object) && ge_object_is_a (object, "GtkHBox"))

#define GE_IS_CLIST(object) ((object) && ge_object_is_a (object, "GtkCList"))
#define GE_IS_TREE_VIEW(object) ((object) && ge_object_is_a (object, "GtkTreeView"))
#define GE_IS_ENTRY(object) ((object) && ge_object_is_a (object, "GtkEntry"))
#define GE_IS_BUTTON(object) ((object) && ge_object_is_a (object, "GtkButton"))
#define GE_IS_FIXED(object) ((object) && ge_object_is_a (object, "GtkFixed"))
 
#define TOGGLE_BUTTON(object) (IS_TOGGLE_BUTTON(object)?(GtkToggleButton *)object:NULL)
 
#define GE_IS_STYLE(object) ((object) && ge_object_is_a (object, "GtkStyle"))
#define GE_IS_NOTEBOOK(object) ((object) && ge_object_is_a (object, "GtkNotebook"))
#define GE_IS_CELL_RENDERER_TOGGLE(object) ((object) && ge_object_is_a (object, "GtkCellRendererToggle"))

GE_INTERNAL gboolean ge_object_is_a (const GObject * object, const gchar * type_name);
 
GE_INTERNAL gboolean ge_combo_box_is_using_list (GtkWidget * widget);

GE_INTERNAL gboolean ge_is_combo_box_entry (GtkWidget * widget);
GE_INTERNAL gboolean ge_is_combo_box (GtkWidget * widget, gboolean as_list);
GE_INTERNAL gboolean ge_is_combo (GtkWidget * widget);
GE_INTERNAL gboolean ge_is_in_combo_box (GtkWidget * widget);
  
GE_INTERNAL gboolean ge_is_toolbar_item (GtkWidget * widget);
  
GE_INTERNAL gboolean ge_is_panel_widget_item (GtkWidget * widget);
 
GE_INTERNAL gboolean ge_is_bonobo_dock_item (GtkWidget * widget);

GE_INTERNAL GtkWidget *ge_find_combo_box_widget_parent (GtkWidget * widget);

GE_INTERNAL gboolean ge_cell_renderer_toggle_get_inconsistent (GtkWidget * widget);
GE_INTERNAL gboolean ge_toggle_get_inconsistent (GtkWidget * widget, const gchar *detail, GtkShadowType shadow_type);

GE_INTERNAL void ge_option_menu_get_props (GtkWidget * widget, 
                            GtkRequisition * indicator_size, 
                            GtkBorder * indicator_spacing);

GE_INTERNAL void ge_button_get_default_border (GtkWidget *widget, 
                                               GtkBorder *border);

GE_INTERNAL gboolean ge_widget_is_ltr (GtkWidget *widget);

