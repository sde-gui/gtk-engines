#ifndef CLEARLOOKS_TYPES_H
#define CLEARLOOKS_TYPES_H

typedef unsigned char boolean;
typedef unsigned char uint8;

typedef enum
{
	CL_STATE_NORMAL,
	CL_STATE_ACTIVE,
	CL_STATE_SELECTED,
	CL_STATE_INSENSITIVE
} ClearlooksStateType;

typedef enum
{
	CL_CORNER_NONE        = 0,
	CL_CORNER_TOPLEFT     = 1,
	CL_CORNER_TOPRIGHT    = 2,
	CL_CORNER_BOTTOMLEFT  = 4,
	CL_CORNER_BOTTOMRIGHT = 8,
	CL_CORNER_ALL         = 15
} ClearlooksCorners;

typedef enum
{
	CL_JUNCTION_NONE      = 0,
	CL_JUNCTION_BEGIN     = 1,
	CL_JUNCTION_END       = 2
} ClearlooksJunction;

typedef enum
{
	CL_STEPPER_UNKNOWN    = 0,
	CL_STEPPER_A          = 1,
	CL_STEPPER_B          = 2,
	CL_STEPPER_C          = 4,
	CL_STEPPER_D          = 8
} ClearlooksStepper;

typedef enum
{
	CL_ORDER_FIRST,
	CL_ORDER_MIDDLE,
	CL_ORDER_LAST
} ClearlooksOrder;

typedef enum
{
	CL_ORIENTATION_LEFT_TO_RIGHT,
	CL_ORIENTATION_RIGHT_TO_LEFT,
	CL_ORIENTATION_BOTTOM_TO_TOP,
	CL_ORIENTATION_TOP_TO_BOTTOM
} ClearlooksOrientation;

typedef enum
{
	CL_GAP_LEFT,
	CL_GAP_RIGHT,
	CL_GAP_TOP,
	CL_GAP_BOTTOM
} ClearlooksGapSide;

typedef enum
{
	CL_SHADOW_NONE,
	CL_SHADOW_IN,
	CL_SHADOW_OUT,
	CL_SHADOW_ETCHED_IN,
	CL_SHADOW_ETCHED_OUT
} ClearlooksShadowType;

typedef enum
{
	CL_HANDLE_TOOLBAR,
	CL_HANDLE_SPLITTER
} ClearlooksHandleType;

typedef enum
{
	CL_ARROW_NORMAL,
	CL_ARROW_COMBO
} ClearlooksArrowType;

typedef enum
{
	CL_DIRECTION_UP,
	CL_DIRECTION_DOWN,
	CL_DIRECTION_LEFT,
	CL_DIRECTION_RIGHT
} ClearlooksDirection;

typedef enum
{
	CL_PROGRESSBAR_CONTINUOUS,
	CL_PROGRESSBAR_DISCRETE
} ClearlooksProgressBarStyle;

typedef enum
{
	CL_WINDOW_EDGE_NORTH_WEST,
	CL_WINDOW_EDGE_NORTH,
	CL_WINDOW_EDGE_NORTH_EAST,
	CL_WINDOW_EDGE_WEST,
	CL_WINDOW_EDGE_EAST,
	CL_WINDOW_EDGE_SOUTH_WEST,
	CL_WINDOW_EDGE_SOUTH,
	CL_WINDOW_EDGE_SOUTH_EAST  
} ClearlooksWindowEdge;

typedef struct
{
	double r;
	double g;
	double b;
} CairoColor;

typedef struct
{
	double x;
	double y;
	double width;
	double height;
} ClearlooksRectangle;

typedef struct
{
	CairoColor bg[5];
	CairoColor base[5];
	
	CairoColor shade[9];
	CairoColor spot[3];
} ClearlooksColors;

typedef struct
{
	boolean active;
	boolean prelight;
	boolean disabled;
	boolean focus;
	boolean is_default;
	
	ClearlooksStateType state_type;
	
	uint8 corners;
	uint8 xthickness;
	uint8 ythickness;

	CairoColor parentbg;
} WidgetParameters;

typedef struct
{
	boolean inverted;
	int     fill_size;
	boolean horizontal;
} SliderParameters;

typedef struct
{
	ClearlooksOrientation orientation;
	ClearlooksProgressBarStyle style;
} ProgressBarParameters;

typedef struct
{
	int linepos;
} OptionMenuParameters;

typedef struct
{
	ClearlooksShadowType shadow;
	ClearlooksGapSide gap_side;
	int gap_x;
	int gap_width;
	CairoColor *border;
} FrameParameters;

typedef struct
{
	ClearlooksGapSide gap_side;
} TabParameters;

typedef struct
{
	ClearlooksCorners    corners;
	ClearlooksShadowType shadow;	
} ShadowParameters;

typedef struct
{
	boolean horizontal;
} SeparatorParameters;

typedef struct
{
	ClearlooksOrder order;
	boolean         resizable;
} ListViewHeaderParameters;

typedef struct
{
	CairoColor         color;
	ClearlooksJunction junction;       /* On which sides the slider junctions */
	ClearlooksStepper  steppers;       /* The visible steppers */
	boolean            horizontal;
	boolean            has_color;
} ScrollBarParameters;

typedef struct
{
	ClearlooksHandleType type;
	boolean              horizontal; 
} HandleParameters;

typedef struct
{
	ClearlooksStepper stepper;         /* Which stepper to draw */
} ScrollBarStepperParameters;

typedef struct
{
	ClearlooksArrowType type;
	ClearlooksDirection direction;
} ArrowParameters;

typedef struct
{
	ClearlooksWindowEdge edge;
} ResizeGripParameters;

typedef struct
{
	int style;
} MenuBarParameters;

#define CLEARLOOKS_RECTANGLE_SET(rect, _x, _y, _w, _h) rect.x      = _x; \
                                                       rect.y      = _y; \
                                                       rect.width  = _w; \
                                                       rect.height = _h;

#endif /* CLEARLOOKS_TYPES_H */
