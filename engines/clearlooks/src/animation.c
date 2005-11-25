/* Yes, this is evil code. But many people seem to like hazardous things, so
 * it exists. Most of this was written by Kulyk Nazar.
 */

#define CL_IS_PROGRESS_BAR(widget) GTK_IS_PROGRESS_BAR(widget) && widget->allocation.x != -1 && widget->allocation.y != -1

struct _Widget_Animation {
	gint8 max_frames;
	gint8 frame;
	gint8 loop;
};

typedef struct _Widget_Animation Widget_Animation;

static GSList     *signaled_widgets      = NULL;
static GHashTable *async_widgets         = NULL;
static int         async_widget_timer_id = 0;


static gboolean cl_async_animation_timer_func (gpointer data);
static gpointer cl_async_animation_lookup     (gconstpointer data);
static void     cl_async_animation_remove     (gpointer data);
static void     cl_async_animation_add        (gpointer data,
                                               gint8 count, gint8 loop);



/* Find and return a pointer to the data linked to this widget, if it exists */
static gpointer
cl_async_animation_lookup(gconstpointer data)
{
	if (async_widgets)
		return g_hash_table_lookup (async_widgets, data);
	
	return NULL;
}

/* Remove a widget and its data from the hash table */
static void
cl_async_animation_remove (gpointer data)
{
	Widget_Animation *value = cl_async_animation_lookup(data);
	
	if (value == NULL)
		return;
	
	g_free(value);
	
	g_hash_table_remove(async_widgets, data);
	
	g_object_disconnect (data, "any_signal::unrealize", G_CALLBACK (cl_async_animation_remove), data, NULL);
	
	g_object_unref (data);
	
	if ((g_hash_table_size(async_widgets) == 0)&&(async_widget_timer_id != 0)) {
		g_source_remove(async_widget_timer_id);
		async_widget_timer_id = 0;
	}
}

/* Add a widget to the hash table */
static void
cl_async_animation_add (gpointer data, gint8 count, gint8 loop)
{
	Widget_Animation *value;
	
	/* object already in the list, not adding twice */
	if(cl_async_animation_lookup(data))
		return;
	
	if(async_widgets == NULL)
		async_widgets = g_hash_table_new(g_direct_hash, g_direct_equal);
	
	value = g_new(Widget_Animation, sizeof(Widget_Animation));
	value->frame = count;
	value->max_frames= count;
	value->loop = loop;
	g_hash_table_insert (async_widgets, data, value);
	
	g_object_ref (data);
	
	g_signal_connect ((GObject*)data, "unrealize", G_CALLBACK (cl_async_animation_remove), data);
	
	if (async_widget_timer_id == 0)
		async_widget_timer_id = g_timeout_add (100, cl_async_animation_timer_func, NULL);
}

/* Update the animation. This goes trough the entire list of widgets and queues
 * a redraw on them.
 */
static gboolean
cl_async_animation_update (gpointer data, gpointer value, gpointer user_data)
{
	Widget_Animation *my_value = value;
	
	if ((data == NULL) || (value == NULL))
		return;
		
	if (--(my_value->frame) < 0)
	{
		if(my_value->loop == 0)
			my_value->frame = my_value->max_frames;
		else if(my_value->loop == 1)
		{
			//animation is done
			g_free((Widget_Animation*)value);
			return TRUE;
		}
		else if (my_value->loop > 1)
		{
			my_value->loop--;
			my_value->frame = my_value->max_frames;
		}
	}
	
	if(GTK_IS_PROGRESS_BAR ((GtkWidget*)data))
	{
		gfloat fraction = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR (data));
		/* update only if not filled */
		/* trick for progressbar, remove if it 100% from animation list, or animate if not) */
		if (fraction < 1.0)
			gtk_widget_queue_resize ((GtkWidget*)data);
		else
		{
			/* animation is done */
			g_free((Widget_Animation*)value);
			return TRUE;
		}
	}
	else
		gtk_widget_queue_draw ((GtkWidget*)data);
	
	return FALSE;
}

/* This gets called by the glib main loop every once in a while. */
static gboolean
cl_async_animation_timer_func (gpointer data)
{
	//printf("** TICK **\n");
	g_hash_table_foreach_remove (async_widgets, cl_async_animation_update, NULL);
	
	if(g_hash_table_size(async_widgets)==0)
	{
		g_source_remove(async_widget_timer_id);
		async_widget_timer_id = 0;
	}
	
	return (g_hash_table_size(async_widgets) != 0);
}

/* Return a structure with the data values */
static Widget_Animation
cl_async_animation_getdata (gpointer data)
{
	Widget_Animation  res = { 0, 0, 0 };
	Widget_Animation *tmp = (Widget_Animation*)cl_async_animation_lookup(data);
	
	if(tmp)
		return *tmp;
	else
		return res;
}

static void
cl_checkbox_toggle (gpointer data)
{
	//printf("togled %d\n",data);
	Widget_Animation *value;
	if((value = cl_async_animation_lookup(data))!= NULL)
		value->frame = value->max_frames - value->frame;
	cl_async_animation_add(data,5,1);
}


static void
cl_progressbar_add (gpointer data)
{
	cl_async_animation_add(data,9,0);
}

static gboolean
cl_disconnect(gpointer data, gpointer value, gpointer user_data)
{
	if(GTK_IS_CHECK_BUTTON (data))
		g_object_disconnect (data, "any_signal::toggled", G_CALLBACK (cl_checkbox_toggle ), data, NULL);
	
	return TRUE;
}
