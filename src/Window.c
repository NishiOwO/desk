#include <Desk/Desk.h>
#include <Desk/Window.h>

static void widget_init(DeskWidget w) {}

static void widget_destroy(DeskWidget w) {}

static void widget_render(DeskWidget w) {}

DeskWidgetClassRec DeskWindowWidgetClassRec = {
    widget_init,    /* init */
    widget_destroy, /* destroy */
    widget_render   /* render */
};

DeskWidgetClass DeskWindowWidgetClass = (DeskWidgetClass)&DeskWindowWidgetClassRec;
