#include <Desk/Desk.h>

#include <stdlib.h>

#include <X11/Xlib.h>

static Display* display;

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

void* _DeskCreateWindow(void* parent, int x, int y, int w, int h) {
	window_t* win = malloc(sizeof(*win));
	Window	  pw;
	if(parent == NULL) {
		pw = RootWindow(display, DefaultScreen(display));
	} else {
		pw = ((window_t*)parent)->window;
	}

	win->window = XCreateSimpleWindow(display, pw, x, y, w, h, 0, BlackPixel(display, 0), WhitePixel(display, 0));
	XMapWindow(display, win->window);

	return win;
}

void _DeskDestroyWindow(void* win) {
	XDestroyWindow(display, ((window_t*)win)->window);
	free(win);
}
