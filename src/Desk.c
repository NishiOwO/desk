#include <Desk/Desk.h>
#include <Desk/Font.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stb_ds.h>

void DeskSleep(int ms) {
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif
}

void DeskInit(void) {
	_DeskInit();
}

void DeskStep(DeskWidget w) {
	int i;
	for(i = 0; i < arrlen(w->children); i++) {
		DeskStep(w->children[i]);
	}
	_DeskStep(w->window, &w->render);

	if(w->render && w->wclass->render != NULL) {
		w->wclass->render(w);
		w->render = 0;
	}
}

DeskBool DeskPending(DeskWidget w) {
	int i;
	int p = 0;
	for(i = 0; i < arrlen(w->children); i++) {
		if(DeskPending(w->children[i]) || w->render) {
			return DeskTrue;
		}
	}
	return _DeskPending(w->window);
}

void DeskMainLoop(DeskWidget w) {
	LOG("Main loop", "");
	while(1) {
		DeskStep(w);
		while(!DeskPending(w)) DeskSleep(10);
	}
}

DeskWidget DeskGetRoot(DeskWidget w) {
	if(w->parent == NULL) return w;
	return DeskGetRoot(w->parent);
}

void DeskDestroy(DeskWidget w) {
	int i;
	for(i = 0; i < arrlen(w->children); i++) {
		DeskDestroy(w->children[i]);
	}
	arrfree(w->children);
	if(w->parent != NULL) {
		DeskWidget p = w->parent;
		for(i = 0; i < arrlen(p->children); i++) {
			if(p->children[i] == w) {
				arrdel(p->children, i);
				break;
			}
		}
	}
	if(w->wclass->destroy != NULL) w->wclass->destroy(w);
	if(w->texts != NULL){
		arrfree(w->texts);
	}
	free(w);
}

DeskWidget DeskCreateWidget(DeskWidgetClass wclass, DeskWidget parent, ...) {
	DeskWidget widget = malloc(sizeof(*widget));
	va_list va;
	LOG("Created widget", "");
	memset(widget, 0, sizeof(*widget));
	widget->wclass = wclass;
	widget->render = 1;
	widget->window = _DeskCreateWindow(parent == DeskWidgetNone ? NULL : parent->window, 50, 50, 200, 200);
	widget->parent = parent;
	if(parent != DeskWidgetNone) {
		arrput(parent->children, widget);
	}else{
		widget->font = DeskFontOpen(NULL);
		if(widget->font != DeskFontNone){
			LOG("Loaded default font", "");
		}
	}
	if(wclass->init != NULL) wclass->init(widget);

	va_start(va, parent);
	while(1) {
		const char* k = va_arg(va, const char*);
		if(k == NULL) break;
		if(k[0] == 'I') {
			int v = va_arg(va, int);
			DeskSetInteger(widget, k, v);
		} else if(k[0] == 'S') {
			const char* v = va_arg(va, const char*);
			DeskSetString(widget, k, v);
		}
	}
	va_end(va);

	return widget;
}

const char* DeskGetText(DeskWidget w, const char* key){
	int i;
	for(i = 0; i < arrlen(w->texts); i += 2){
		if(strcmp(w->texts[i], key) == 0){
			return w->texts[i + 1];
		}
	}
	return NULL;
}

void DeskSetGeometry(DeskWidget w, int x, int y, int width, int height, int flag) { _DeskSetGeometry(w->window, x, y, width, height, flag); }

void DeskGetGeometry(DeskWidget w, int* x, int* y, int* width, int* height) { _DeskGetGeometry(w->window, x, y, width, height); }

void DeskSetInteger(DeskWidget w, const char* key, int value) {
	if(strcmp(key, DeskNx) == 0) {
		DeskSetGeometry(w, value, 0, 0, 0, DeskSetX);
	} else if(strcmp(key, DeskNy) == 0) {
		DeskSetGeometry(w, 0, value, 0, 0, DeskSetY);
	} else if(strcmp(key, DeskNwidth) == 0) {
		DeskSetGeometry(w, 0, 0, value, 0, DeskSetWidth);
	} else if(strcmp(key, DeskNheight) == 0) {
		DeskSetGeometry(w, 0, 0, 0, value, DeskSetHeight);
	}
}

void DeskSetString(DeskWidget w, const char* key, const char* value) {
	if(strcmp(key, DeskNtitle) == 0) {
		_DeskSetTitle(w->window, value);
	}else{
		arrput(w->texts, key);
		arrput(w->texts, value);
	}
}
