#include <Desk/Desk.h>
#include <Desk/Font.h>
#include <Desk/Label.h>

#include <stdlib.h>

static void widget_init(DeskWidget w){
}

static void widget_destroy(DeskWidget w){
}

static void widget_render(DeskWidget w){
	const char* text = DeskGetString(w, DeskNtext);
	if(text != NULL){
		int x, y, width, height, bw, bh;
		DeskFont font = DeskGetFont(w);
		DeskGetGeometry(w, &x, &y, &width, &height);
		_DeskSetForegroundColor(w->window, 0, 0, 0);
		free(DeskFontRender(font, text, 16, &bw, &bh));
		DeskFontDraw(w, font, text, width / 2 - bw / 2, height / 2 - bh / 2, 16);
	}
}

DeskWidgetClassRec DeskLabelWidgetClassRec = {
	widget_init, /* init */
	widget_destroy, /* destroy */
	widget_render /* render */
};

DeskWidgetClass DeskLabelWidgetClass = (DeskWidgetClass)&DeskLabelWidgetClassRec;
