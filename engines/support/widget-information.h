#if 0
#include <gtk/gtk.h>

/* While the UNKOWN cases should never happen, they shall be
 * handled in a sane way by the theme. ie. don't fuck up in that case ;-) */
typedef enum {
  GE_HEADER_UNKOWN = 0,
  GE_HEADER_FIRST,
  GE_HEADER_MIDDLE,
  GE_HEADER_LAST,
} GEHeaderPosition;

typedef enum {
  GE_STEPPER_UNKOWN = 0,
  GE_STEPPER_A,
  GE_STEPPER_B,
  GE_STEPPER_C,
  GE_STEPPER_D,
} GEStepper;

/* get the text direction of the widget, if widget is NULL the
 * default direction will be returned. */
GtkTextDirection ge_get_direction	(GtkWidget *widget) G_GNUC_INTERNAL;

/* Will try to get the header index that is drawn*/
GEHeaderPosition ge_get_header_index	(GtkWidget	*widget,
                                         GtkWidget	*button,
                                         gboolean	 resizable) G_GNUC_INTERNAL;

GEStepper ge_get_stepper                (GtkWidget      *widget,
                                         GdkRectangle   *position) G_GNUC_INTERNAL;


#endif

/* Object Type Lookups/Macros
   
   Based on/modified from functions in
   Smooth-Engine.
*/ 
#define IS_WIDGET(object) ((object)  && ge_object_is_a (object, "GtkWidget"))
#define IS_CONTAINER(object) ((object)  && ge_object_is_a (object, "GtkContainer"))
#define IS_BIN(object) ((object)  && ge_object_is_a (object, "GtkBin"))
 
#define IS_HANDLE_BOX(object) ((object) && ge_object_is_a (object, "GtkHandleBox"))
#define IS_HANDLE_BOX_ITEM(object) ((object) && IS_HANDLE_BOX(object->parent))
#define IS_BONOBO_DOCK_ITEM(object) ((object) && ge_object_is_a (object, "BonoboDockItem"))
#define IS_BONOBO_DOCK_ITEM_GRIP(object) ((object) && ge_object_is_a (object, "BonoboDockItemGrip"))
#define IS_BONOBO_TOOLBAR(object) ((object) && ge_object_is_a (object, "BonoboUIToolbar"))
#define IS_EGG_TOOLBAR(object) ((object) && ge_object_is_a (object, "Toolbar"))
#define IS_TOOLBAR(object) ((object) && ge_object_is_a (object, "GtkToolbar"))
#define IS_PANEL_WIDGET(object) ((object) && ge_object_is_a (object, "PanelWidget"))
 
#define IS_COMBO_BOX_ENTRY(object) ((widget) && ge_object_is_a (widget, "GtkComboBoxEntry"))
#define IS_COMBO_BOX(object) ((widget) && ge_object_is_a (widget, "GtkComboBox"))
#define IS_COMBO(object) ((widget) && ge_object_is_a (widget, "GtkCombo"))
#define IS_OPTION_MENU(object) ((widget) && ge_object_is_a (widget, "GtkOptionMenu"))
 
#define IS_TOGGLE_BUTTON(object) ((object) && ge_object_is_a (object, "GtkToggleButton"))
#define IS_SPIN_BUTTON(object) ((object) && ge_object_is_a (object, "GtkSpinButton"))
 
#define IS_STATUS_BAR(object) ((object) && ge_object_is_a (object, "GtkStatusBar"))
#define IS_PROGRESS_BAR(object) ((object) && ge_object_is_a (object, "GtkProgressBar"))
 
#define IS_MENU_SHELL(object) ((object) && ge_object_is_a (object, "GtkMenuShell"))
#define IS_MENU(object) ((object) && ge_object_is_a (object, "GtkMenu"))
#define IS_MENU_BAR(object) ((object) && ge_object_is_a (object, "GtkMenuBar"))
#define IS_MENU_ITEM(object) ((object) && ge_object_is_a (object, "GtkMenuItem"))
 
#define IS_SCROLLBAR(object) ((object) && ge_object_is_a (object, "GtkScrollbar"))
#define IS_VSCROLLBAR(object) ((object) && ge_object_is_a (object, "GtkVScrollbar"))
#define IS_HSCROLLBAR(object) ((object) && ge_object_is_a (object, "GtkHScrollbar"))
 
#define IS_SCALE(object) ((object) && ge_object_is_a (object, "GtkScale"))
#define IS_VSCALE(object) ((object) && ge_object_is_a (object, "GtkVScale"))
#define IS_HSCALE(object) ((object) && ge_object_is_a (object, "GtkHScale"))
  
#define IS_PANED(object) ((object) && ge_object_is_a (object, "GtkPaned"))
#define IS_VPANED(object) ((object) && ge_object_is_a (object, "GtkVPaned"))
#define IS_HPANED(object) ((object) && ge_object_is_a (object, "GtkHPaned"))
 
#define IS_BOX(object) ((object) && ge_object_is_a (object, "GtkBox"))
#define IS_VBOX(object) ((object) && ge_object_is_a (object, "GtkVBox"))
#define IS_HBOX(object) ((object) && ge_object_is_a (object, "GtkHBox"))
 
#define TOGGLE_BUTTON(object) (IS_TOGGLE_BUTTON(object)?(GtkToggleButton *)object:NULL)
 
gboolean ge_object_is_a (GtkWidget * widget, gchar * type_name) G_GNUC_INTERNAL;
 
gboolean ge_combo_box_is_using_list (GtkWidget * widget) G_GNUC_INTERNAL;

gboolean ge_is_combo_box_entry (GtkWidget * widget) G_GNUC_INTERNAL;
gboolean ge_is_combo_box (GtkWidget * widget, gboolean as_list) G_GNUC_INTERNAL;
gboolean ge_is_combo (GtkWidget * widget) G_GNUC_INTERNAL;
gboolean ge_is_in_combo_box (GtkWidget * widget) G_GNUC_INTERNAL;
  
gboolean ge_is_toolbar_item (GtkWidget * widget) G_GNUC_INTERNAL;
  
gboolean ge_is_panel_widget_item (GtkWidget * widget) G_GNUC_INTERNAL;
 
gboolean ge_is_bonobo_dock_item (GtkWidget * widget) G_GNUC_INTERNAL;

void ge_option_menu_get_props (GtkWidget * widget, 
                            GtkRequisition * indicator_size, 
                            GtkBorder * indicator_spacing) G_GNUC_INTERNAL;
 
