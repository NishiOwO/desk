#include <Desk/Desk.h>

#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

static Display* display;
static long	mask = ExposureMask | StructureNotifyMask;

typedef struct window {
	Window window;
	GC     gc;
	int    x, y, w, h;
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
	Window	  pw;
	XEvent	  ev;
	if(parent == NULL) {
		pw = RootWindow(display, DefaultScreen(display));
	} else {
		pw = ((window_t*)parent)->window;
	}

	win->window = XCreateSimpleWindow(display, pw, x, y, width, height, 0, BlackPixel(display, 0), WhitePixel(display, 0));
	win->gc	    = XCreateGC(display, win->window, 0, 0);
	win->x	    = x;
	win->y	    = y;
	win->w	    = width;
	win->h	    = height;
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

int _DeskPending(void* win) {
	XEvent ev;
	if(XCheckWindowEvent(display, ((window_t*)win)->window, mask, &ev)) {
		XPutBackEvent(display, &ev);
		return 1;
	}
	return 0;
}

void _DeskStep(void* win, int* render) {
	XEvent	  ev;
	window_t* w = (window_t*)win;
	*render	    = 0;
	if(XCheckWindowEvent(display, w->window, mask, &ev)) {
		if(ev.type == Expose) {
			*render = 1;
		} else if(ev.type == ConfigureNotify) {
			w->x = ev.xconfigure.x;
			w->y = ev.xconfigure.y;
			w->w = ev.xconfigure.width;
			w->h = ev.xconfigure.height;
		}
	}
}

void _DeskSetCoord(void* win, int x, int y, int width, int height, int flag) {
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

void _DeskSetTitle(void* win, const char* title) { XSetStandardProperties(display, ((window_t*)win)->window, title, "Desk GUI", None, (char**)NULL, 0, NULL); }
