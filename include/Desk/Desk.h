#ifndef __DESK_DESK_H__
#define __DESK_DESK_H__

#define DESK_DLL

#if defined(_WIN32) && defined(DESK_DLL) && defined(DESK_INTERNAL)
#define DESK_EXPORT extern __declspec(dllexport)
#elif defined(_WIN32) && defined(DESK_DLL)
#define DESK_EXPORT extern __declspec(dllimport)
#else
#define DESK_EXPORT extern
#endif

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char DeskBool;

typedef struct _DeskWidgetClassRec DeskWidgetClassRec;
typedef DeskWidgetClassRec* DeskWidgetClass;

typedef struct _DeskWidgetRec DeskWidgetRec;
typedef DeskWidgetRec* DeskWidget;

#include <Desk/StringDefs.h>
#include <Desk/Font.h>

struct _DeskWidgetClassRec {
	void (*init)(DeskWidget w);
	void (*destroy)(DeskWidget w);
	void (*render)(DeskWidget w);
};

struct _DeskWidgetRec {
	DeskWidgetClass wclass;
	DeskWidget parent;
	DeskWidget* children;
	void* opaque;
	void* window;
	DeskBool render;
	DeskBool pressed;
	DeskBool held;
	const char** texts;
	DeskFont font;
};

#define DeskTrue ((DeskBool)1)
#define DeskFalse ((DeskBool)0)

#define DeskWidgetNone ((DeskWidget)NULL)

#define DeskSetX (1 << 0)
#define DeskSetY (1 << 1)
#define DeskSetPos (DeskSetX | DeskSetY)

#define DeskSetWidth (1 << 2)
#define DeskSetHeight (1 << 3)
#define DeskSetSize (DeskSetWidth | DeskSetHeight)

DESK_EXPORT void DeskInit(void);

DESK_EXPORT void DeskStep(DeskWidget w);

DESK_EXPORT DeskBool DeskPending(DeskWidget w);

DESK_EXPORT void DeskMainLoop(DeskWidget w);

DESK_EXPORT DeskWidget DeskCreateWidget(DeskWidgetClass wclass, DeskWidget parent, ...);

DESK_EXPORT DeskWidget DeskGetRoot(DeskWidget w);

DESK_EXPORT const char* DeskGetText(DeskWidget w, const char* key);

DESK_EXPORT void DeskSetGeometry(DeskWidget w, int x, int y, int width, int height, int flag);

DESK_EXPORT void DeskGetGeometry(DeskWidget w, int* x, int* y, int* width, int* height);

DESK_EXPORT void DeskSetInteger(DeskWidget w, const char* key, int value);

DESK_EXPORT void DeskSetString(DeskWidget w, const char* key, const char* value);

#ifdef __cplusplus
}
#endif

#ifdef DESK_INTERNAL
#include <Desk/Internal.h>
#endif

#endif
