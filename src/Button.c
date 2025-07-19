#include <Desk/Desk.h>
#include <Desk/Font.h>
#include <Desk/Button.h>

#include <stdlib.h>

static void widget_init(DeskWidget w) {}

static void widget_destroy(DeskWidget w) {}

static void widget_render(DeskWidget w) {
	int x, y, width, height;
	const char* text = DeskGetText(w, DeskNtext);
	DeskGetGeometry(w, &x, &y, &width, &height);
	_DeskSetForegroundColor(w->window, 0, 0, 0);
	_DeskSetBorderWidth(w->window, 1);
	_DeskDrawRectangle(w->window, 0, 0, width, height);
	if(text != NULL){
		DeskWidget root = DeskGetRoot(w);
		int bw, bh;
		free(DeskFontRender(root->font, text, 20, &bw, &bh));
		DeskFontDraw(w, root->font, text, width / 2 - bw / 2, height / 2 - bh / 2, 20);
	}
}

DeskWidgetClassRec DeskButtonWidgetClassRec = {
    widget_init,    /* init */
    widget_destroy, /* destroy */
    widget_render   /* render */
};

DeskWidgetClass DeskButtonWidgetClass = (DeskWidgetClass)&DeskButtonWidgetClassRec;
