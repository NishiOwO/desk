#include <Desk/Desk.h>

#include <stdlib.h>

#include <X11/Xlib.h>

static Display* display;
static long mask = ExposureMask;

typedef struct window {
	Window window;
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
	if(parent == NULL) {
		pw = RootWindow(display, DefaultScreen(display));
	} else {
		pw = ((window_t*)parent)->window;
	}

	win->window = XCreateSimpleWindow(display, pw, x, y, width, height, 1, BlackPixel(display, 0), WhitePixel(display, 0));
	XSelectInput(display, win->window, mask);
	XMapWindow(display, win->window);

	return win;
}

void _DeskDestroyWindow(void* win) {
	XDestroyWindow(display, ((window_t*)win)->window);
	free(win);
}

int _DeskPending(void* win){
	XEvent ev;
	if(XCheckWindowEvent(display, ((window_t*)win)->window, mask, &ev)){
		XPutBackEvent(display, &ev);
		return 1;
	}
	return 0;
}

void _DeskStep(void* win, int* render){
	XEvent ev;
	*render = 0;
	if(XCheckWindowEvent(display, ((window_t*)win)->window, mask, &ev)){
		if(ev.type == Expose){
			*render = 1;
		}
	}
}

void _DeskSetCoord(void* win, int x, int y, int width, int height, int flag){
	XWindowAttributes attr;
	int wx, wy, ww, wh;
	XGetWindowAttributes(display, ((window_t*)win)->window, &attr);
	wx = attr.x;
	wy = attr.y;
	ww = attr.width;
	wh = attr.height;
	if(flag & DeskSetX){
		wx = x;
	}
	if(flag & DeskSetY){
		wy = y;
	}
	if(flag & DeskSetWidth){
		ww = width;
	}
	if(flag & DeskSetHeight){
		wh = height;
	}
	XMoveResizeWindow(display, ((window_t*)win)->window, wx, wy, ww, wh);
}
