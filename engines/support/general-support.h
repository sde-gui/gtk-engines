
#include <glib.h>

/* macros to make sure that things are sane ... */

#define CHECK_DETAIL(detail, value) ((detail) && (!strcmp(value, detail)))

#define CHECK_ARGS					\
  g_return_if_fail (window != NULL);			\
  g_return_if_fail (style != NULL);

#define SANITIZE_SIZE					\
  g_return_if_fail (width  >= -1);			\
  g_return_if_fail (height >= -1);			\
                                                        \
  if ((width == -1) && (height == -1))			\
    gdk_drawable_get_size (window, &width, &height);	\
  else if (width == -1)					\
    gdk_drawable_get_size (window, &width, NULL);	\
  else if (height == -1)				\
    gdk_drawable_get_size (window, NULL, &height);

#if defined(__GNUC__) && ((__GNUC__ * 100 + __GNUC_MINOR__) >= 303)
# define GE_EXPORT      __attribute__((visibility("default")))
# define GE_HIDDEN      __attribute__((visibility("hidden")))
# define GE_INTERNAL    __attribute__((visibility("internal")))
#elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x550)
# define GE_EXPORT      __global
# define GE_HIDDEN      __hidden
# define GE_INTERNAL    __hidden
#else /* not gcc >= 3.3 and not Sun Studio >= 8 */
# define GE_EXPORT
# define GE_HIDDEN
# define GE_INTERNAL
#endif 
