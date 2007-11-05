
static void
industrial_style_copy (GtkStyle * style, GtkStyle * src)
{
	IndustrialStyle *industrial_style = INDUSTRIAL_STYLE (style);
	IndustrialStyle *industrial_src = INDUSTRIAL_STYLE (src);

	industrial_style->contrast = industrial_src->contrast;
	industrial_style->rounded_buttons = industrial_src->rounded_buttons;

	parent_class->copy (style, src);
}

static void
industrial_style_init_from_rc (GtkStyle * style, GtkRcStyle * rc_style)
{
	IndustrialStyle *industrial_style = INDUSTRIAL_STYLE (style);
	parent_class->init_from_rc (style, rc_style);

	industrial_style->contrast = INDUSTRIAL_RC_STYLE (rc_style)->contrast;
	industrial_style->rounded_buttons = INDUSTRIAL_RC_STYLE (rc_style)->rounded_buttons;
	industrial_style->hint = INDUSTRIAL_RC_STYLE (rc_style)->hint;
}

static void
industrial_style_init (IndustrialStyle * style)
{
	style->contrast = 1.0;
	style->rounded_buttons = TRUE;
	style->hint = 0;
}

static void
industrial_style_class_init (IndustrialStyleClass * klass)
{
	GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);

	parent_class = g_type_class_peek_parent (klass);

	style_class->copy = industrial_style_copy;
	style_class->init_from_rc = industrial_style_init_from_rc;

	style_class->draw_focus = draw_focus;

	style_class->draw_handle = draw_handle;
	style_class->draw_vline = draw_vline;
	style_class->draw_hline = draw_hline;
	style_class->draw_slider = draw_slider;

	style_class->draw_check = draw_check;
	style_class->draw_box = draw_box;
	style_class->draw_shadow = draw_shadow;
	style_class->draw_box_gap = draw_box_gap;
	style_class->draw_shadow_gap = draw_shadow_gap;
	style_class->draw_extension = draw_extension;
	style_class->draw_option = draw_option;
	style_class->render_icon = render_icon;
}

GType industrial_type_style = 0;

void
industrial_style_register_type (GTypeModule * module)
{
	static const GTypeInfo object_info = {
		sizeof (IndustrialStyleClass),
		(GBaseInitFunc) NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc) industrial_style_class_init,
		NULL,		/* class_finalize */
		NULL,		/* class_data */
		sizeof (IndustrialStyle),
		0,		/* n_preallocs */
		(GInstanceInitFunc) industrial_style_init,
		NULL
	};

	industrial_type_style = g_type_module_register_type (module, GTK_TYPE_STYLE,
							     "IndustrialStyle",
							     &object_info, 0);
}
