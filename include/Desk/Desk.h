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

#ifdef DESK_INTERNAL
#include <Desk/Internal.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _DeskWidgetClassRec DeskWidgetClassRec;
typedef DeskWidgetClassRec*	   DeskWidgetClass;

typedef struct _DeskWidgetRec DeskWidgetRec;
typedef DeskWidgetRec*	      DeskWidget;

struct _DeskWidgetClassRec {
	DeskWidget (*create)(void);
	void (*destroy)(DeskWidget w);
};

struct _DeskWidgetRec {
	DeskWidgetClass wclass;
	DeskWidget	parent;
	DeskWidget*	children;
	void*		opaque;
};

DESK_EXPORT void DeskInit(void);

DESK_EXPORT void DeskMainLoop(DeskWidget w);

DESK_EXPORT DeskWidget DeskGetRoot(DeskWidget w);

#ifdef __cplusplus
}
#endif

#endif
