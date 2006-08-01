
#include <gtk/gtk.h>
#include "industrial_style.h"
#include "industrial_rc_style.h"
#include <math.h>
#include <string.h>

#define INDUSTRIAL_GTK_VERSION 2

#define GET_CONTRAST(style) (INDUSTRIAL_STYLE((style))->contrast)
#define GET_CONTRAST_CENTER(style) (INDUSTRIAL_STYLE((style))->contrast_center)
#define GET_ROUNDED_BUTTONS(style) (INDUSTRIAL_STYLE((style))->rounded_buttons)
