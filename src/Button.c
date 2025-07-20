#include <Desk/Desk.h>
#include <Desk/Font.h>
#include <Desk/Button.h>

#include <stdlib.h>

static void widget_init(DeskWidget w) {}

static void widget_destroy(DeskWidget w) {}

static void widget_render(DeskWidget w) {
	int x, y, width, height;
	const char* text = DeskGetString(w, DeskNtext);
	DeskGetGeometry(w, &x, &y, &width, &height);
	_DeskSetForegroundColor(w->window, 0, 0, 0);
	_DeskSetBorderWidth(w->window, 1);
	_DeskDrawRectangle(w->window, 0, 0, width, height);
	if(w->held){
		_DeskSetForegroundColor(w->window, 0, 0, 0);
		_DeskFillRectangle(w->window, 0, 0, width, height);
	}
	if(text != NULL){
		DeskFont font = DeskGetFont(w);
		DeskWidget root = DeskGetRoot(w);
		int bw, bh;
		int fsz = DeskGetFontSize(w);
		free(DeskFontRender(font, text, fsz, &bw, &bh));
		if(w->held){
			_DeskSetForegroundColor(w->window, 255, 255, 255);
		}else{
			_DeskSetForegroundColor(w->window, 0, 0, 0);
		}
		DeskFontDraw(w, font, text, width / 2 - bw / 2, height / 2 - bh / 2, fsz);
	}
}

DeskWidgetClassRec DeskButtonWidgetClassRec = {
    DeskFlagClickable, /* flag */
    widget_init,    /* init */
    widget_destroy, /* destroy */
    widget_render   /* render */
};

DeskWidgetClass DeskButtonWidgetClass = (DeskWidgetClass)&DeskButtonWidgetClassRec;
