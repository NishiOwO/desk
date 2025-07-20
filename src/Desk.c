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
	_DeskStep(w->window, &w->render, &w->held, &w->pressed);

	if(w->render && w->wclass->render != NULL) {
		_DeskClear(w->window);
		w->wclass->render(w);
		w->render = 0;
	}

	if(w->pressed && (w->wclass->flag & DeskFlagClickable)){
		DeskCallback cb = DeskGetCallback(w, DeskNpressCallback);
		if(cb != NULL) cb(w);
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
	if(w->integers != NULL){
		shfree(w->integers);
	}
	if(w->callbacks != NULL){
		shfree(w->callbacks);
	}
	free(w);
}

DeskWidget DeskCreateWidget(DeskWidgetClass wclass, DeskWidget parent, ...) {
	DeskWidget widget = malloc(sizeof(*widget));
	va_list va;
	LOG("Created widget", "");
	memset(widget, 0, sizeof(*widget));
	sh_new_strdup(widget->integers);
	sh_new_strdup(widget->callbacks);
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
		} else if(k[0] == 'C') {
			DeskCallback v = va_arg(va, DeskCallback);
			DeskSetCallback(widget, k, v);
		}
	}
	va_end(va);

	return widget;
}

int DeskGetIntegerEx(DeskWidget w, const char* key, int placeholder){
	int idx = shgeti(w->integers, key);
	if(idx == -1) return placeholder;
	return w->integers[idx].value;
}

int DeskGetInteger(DeskWidget w, const char* key){
	return DeskGetIntegerEx(w, key, 0);
}

const char* DeskGetString(DeskWidget w, const char* key){
	int i;
	for(i = 0; i < arrlen(w->texts); i += 2){
		if(strcmp(w->texts[i], key) == 0){
			return w->texts[i + 1];
		}
	}
	return NULL;
}

DeskCallback DeskGetCallback(DeskWidget w, const char* key){
	int idx = shgeti(w->callbacks, key);
	if(idx == -1) return NULL;
	return w->callbacks[idx].value;
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
	}else{
		shput(w->integers, key, value);
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

void DeskSetCallback(DeskWidget w, const char* key, DeskCallback value) {
	shput(w->callbacks, key, value);
}

DeskFont DeskGetFont(DeskWidget w){
	DeskFont f = w->font;
	DeskWidget root;
	if(f == DeskFontNone){
		root = DeskGetRoot(w);
		f = root->font;
	}
	return f;
}

#define Inherit(type,utype,prop,dval) \
	DeskWidget widget = w; \
	type res = dval; \
	while(1){ \
		res = DeskGet ## utype ## Ex(widget, prop, dval); \
		if(res != dval) break; \
		if(widget->parent == DeskWidgetNone) break; \
		widget = widget->parent; \
	}

int DeskGetFontSize(DeskWidget w){
	Inherit(int, Integer, DeskNfontSize, -1);

	if(res == -1) res = DeskDefaultFontSize;
	return res;
}
