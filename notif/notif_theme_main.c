#include <gtk/gtk.h>

/* Theme functions to export */
void                theme_init(GtkThemeEngine * engine);
void                theme_exit(void);

/* Exported vtable from th_draw */

extern GtkStyleClass motif_default_class;

/* internals */

static guint
theme_parse_rc_style(GScanner * scanner,
		     GtkRcStyle * rc_style)
{
  guint               token;

  token = g_scanner_peek_next_token(scanner);
  while (token != G_TOKEN_RIGHT_CURLY)
    {
      switch (token)
	{
	default:
	  g_scanner_get_next_token(scanner);
	  token = G_TOKEN_RIGHT_CURLY;
	  break;
	}

      if (token != G_TOKEN_NONE)
	return token;

      token = g_scanner_peek_next_token(scanner);
    }

  g_scanner_get_next_token(scanner);

  rc_style->engine_data = NULL;

  return G_TOKEN_NONE;
}

static void
theme_merge_rc_style(GtkRcStyle * dest,
		     GtkRcStyle * src)
{
}

static void
theme_rc_style_to_style(GtkStyle * style,
			GtkRcStyle * rc_style)
{
  style->klass = &motif_default_class;
}

static void
theme_duplicate_style(GtkStyle * dest,
		      GtkStyle * src)
{
}

static void
theme_realize_style(GtkStyle * style)
{
}

static void
theme_unrealize_style(GtkStyle * style)
{
}

static void
theme_destroy_rc_style(GtkRcStyle * rc_style)
{
}

static void
theme_destroy_style(GtkStyle * style)
{
}

void
theme_init(GtkThemeEngine * engine)
{

  printf("Motif Theme Init\n");

  engine->parse_rc_style = theme_parse_rc_style;
  engine->merge_rc_style = theme_merge_rc_style;
  engine->rc_style_to_style = theme_rc_style_to_style;
  engine->duplicate_style = theme_duplicate_style;
  engine->realize_style = theme_realize_style;
  engine->unrealize_style = theme_unrealize_style;
  engine->destroy_rc_style = theme_destroy_rc_style;
  engine->destroy_style = theme_destroy_style;
  engine->set_background = NULL;

}

void
theme_exit(void)
{
  printf("Motif Theme Exit\n* Need to add memory deallocation code here *\n");
}
