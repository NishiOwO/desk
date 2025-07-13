#ifndef __DESK_DESK_H__
#define __DESK_DESK_H__

#define DESK_DLL

#ifdef DESK_INTERNAL
#include <Desk/Internal.h>
#endif

#if defined(_WIN32) && defined(DESK_DLL) && defined(DESK_INTERNAL)
#define DESK_EXPORT extern __declspec(dllexport)
#elif defined(_WIN32) && defined(DESK_DLL)
#define DESK_EXPORT extern __declspec(dllimport)
#else
#define DESK_EXPORT extern
#endif

typedef struct _DeskWidgetRec {
} DeskWidgetRec;
typedef DeskWidgetRec* DeskWidget;

typedef struct _DeskWidgetClassRec {
} DeskWidgetClassRec;
typedef DeskWidgetClassRec* DeskWidgetClass;

DESK_EXPORT void DeskInit(void);

DESK_EXPORT void DeskMainLoop(DeskWidget w);

#endif
