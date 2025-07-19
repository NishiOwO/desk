#include <Desk/Desk.h>

#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

static Display* display;
static long mask = ExposureMask | StructureNotifyMask;

typedef struct window {
	Window window;
	GC gc;
	int x, y, w, h;
} window_t;

void _DeskInit(void) {
	display = XOpenDisplay(NULL);
	if(display == NULL) {
		exit(1);
	}
	LOG("X11 seems to be working", "");
}

void* _DeskCreateWindow(void* parent, int x, int y, int width, int height) {
	window_t* win = malloc(sizeof(*win));
	Window pw;
	XEvent ev;
	if(parent == NULL) {
		pw = RootWindow(display, DefaultScreen(display));
	} else {
		pw = ((window_t*)parent)->window;
	}

	win->window = XCreateSimpleWindow(display, pw, x, y, width, height, 0, BlackPixel(display, 0), WhitePixel(display, 0));
	win->gc = XCreateGC(display, win->window, 0, 0);
	win->x = x;
	win->y = y;
	win->w = width;
	win->h = height;
	XSelectInput(display, win->window, mask);
	XMapWindow(display, win->window);

	return win;
}

void _DeskDestroyWindow(void* win) {
	window_t* w = (window_t*)win;
	XFreeGC(display, w->gc);
	XDestroyWindow(display, w->window);
	free(win);
}

DeskBool _DeskPending(void* win) {
	XEvent ev;
	if(XCheckWindowEvent(display, ((window_t*)win)->window, mask, &ev)) {
		XPutBackEvent(display, &ev);
		return DeskTrue;
	}
	return DeskFalse;
}

void _DeskStep(void* win, DeskBool* render) {
	XEvent ev;
	window_t* w = (window_t*)win;
	if(XCheckWindowEvent(display, w->window, mask, &ev)) {
		if(ev.type == Expose) {
			*render = DeskTrue;
		} else if(ev.type == ConfigureNotify) {
			w->x = ev.xconfigure.x;
			w->y = ev.xconfigure.y;
			w->w = ev.xconfigure.width;
			w->h = ev.xconfigure.height;
		}
	}
}

void _DeskPutImage(void* win, int x, int y, int width, int height, unsigned char* data){
	window_t* w = (window_t*)win;
	int i, j;
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			int idx = (i * width + j) * 4;
			if(!data[idx + 3]) continue;
			_DeskSetForegroundColor(win, data[idx + 0], data[idx + 1], data[idx + 2]);
			XFillRectangle(display, w->window, w->gc, x + j, y + i, 1, 1);
		}
	}
}

void _DeskSetGeometry(void* win, int x, int y, int width, int height, int flag) {
	window_t* w = (window_t*)win;
	if(flag & DeskSetX) {
		w->x = x;
	}
	if(flag & DeskSetY) {
		w->y = y;
	}
	if(flag & DeskSetWidth) {
		w->w = width;
	}
	if(flag & DeskSetHeight) {
		w->h = height;
	}
	XMoveResizeWindow(display, w->window, w->x, w->y, w->w, w->h);
	XFlush(display);
}

void _DeskGetGeometry(void* win, int* x, int* y, int* width, int* height) {
	window_t* w = (window_t*)win;
	*x = w->x;
	*y = w->y;
	*width = w->w;
	*height = w->h;
}

void _DeskSetTitle(void* win, const char* title) { XSetStandardProperties(display, ((window_t*)win)->window, title, "Desk GUI", None, (char**)NULL, 0, NULL); }

void _DeskSetForegroundColor(void* win, int r, int g, int b) {
	window_t* w = (window_t*)win;
	unsigned long c = 0;

	c = c << 8;
	c = c | r;
	c = c << 8;
	c = c | g;
	c = c << 8;
	c = c | b;

	XSetForeground(display, w->gc, c);
}

int _DeskGetBorderWidth(void* win) {
	XGCValues gv;
	window_t* w = (window_t*)win;
	XGetGCValues(display, w->gc, GCLineWidth, &gv);
	return gv.line_width;
}

void _DeskSetBorderWidth(void* win, int bw) {
	XGCValues gv;
	window_t* w = (window_t*)win;
	gv.line_width = bw;
	XChangeGC(display, w->gc, GCLineWidth, &gv);
}

void _DeskDrawRectangle(void* win, int x, int y, int width, int height) {
	window_t* w = (window_t*)win;
	double sb = (double)_DeskGetBorderWidth(win) / 2;
	if((sb - (int)sb) > 0.5) sb = 1 + (int)sb;
	XDrawRectangle(display, w->window, w->gc, x + sb, y + sb, width - sb * 2, height - sb * 2);
}
