GtkShadowType
thinice_shadow_type (GtkStyle *style, const char *detail, GtkShadowType requested) G_GNUC_INTERNAL;

void
thinice_slash_two(cairo_t *cr,
                  CairoColor *color1,
                  CairoColor *color2,
                  gint x,
                  gint y,
                  gint width,
                  gint height) G_GNUC_INTERNAL;

void
thinice_slash_one(cairo_t *cr,
                  CairoColor *color1,
                  CairoColor *color2,
                  gint x,
                  gint y,
                  gint width,
                  gint height) G_GNUC_INTERNAL;

void
thinice_dot(cairo_t *cr,
            CairoColor *color1,
            CairoColor *color2,
            gint x,
            gint y) G_GNUC_INTERNAL;

void thinice_arrow (cairo_t *cr,
			CairoColor *color,
			GtkArrowType arrow_type, 
			gint x, 
			gint y, 
			gint width, 
			gint height) G_GNUC_INTERNAL;

void thinice_draw_separator(cairo_t *cr, gboolean horizontal,
                           int x, int y, int width, int height) G_GNUC_INTERNAL;
