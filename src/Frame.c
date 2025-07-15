#include <Desk/Desk.h>
#include <Desk/Frame.h>

static void widget_init(DeskWidget w) {}

static void widget_destroy(DeskWidget w) {}

static void widget_render(DeskWidget w) {}

DeskWidgetClassRec DeskFrameWidgetClassRec = {
    widget_init,    /* init */
    widget_destroy, /* destroy */
    widget_render   /* render */
};

DeskWidgetClass DeskFrameWidgetClass = (DeskWidgetClass)&DeskFrameWidgetClassRec;
