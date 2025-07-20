#ifndef __DESK_INTERNAL_H__
#define __DESK_INTERNAL_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <Desk/Desk.h>

#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stderr, "[Desk, %s] " fmt "\n", __FILE__, __VA_ARGS__)
#else
#define LOG(...)
#endif

DESK_EXPORT void _DeskInit(void);

DESK_EXPORT void* _DeskCreateWindow(void* parent, int x, int y, int width, int height);

DESK_EXPORT void _DeskDestroyWindow(void* win);

DESK_EXPORT DeskBool _DeskPending(void* win);

DESK_EXPORT void _DeskStep(void* win, DeskBool* render, DeskBool* held, DeskBool* pressed);

DESK_EXPORT void _DeskSetGeometry(void* win, int x, int y, int width, int height, int flag);

DESK_EXPORT void _DeskGetGeometry(void* win, int* x, int* y, int* width, int* height);

DESK_EXPORT void _DeskSetTitle(void* win, const char* title);

DESK_EXPORT void _DeskSetForegroundColor(void* win, int r, int g, int b);

DESK_EXPORT void _DeskSetBorderWidth(void* win, int bw);

DESK_EXPORT int _DeskGetBorderWidth(void* win);

DESK_EXPORT void _DeskDrawRectangle(void* win, int x, int y, int width, int height);

DESK_EXPORT void _DeskPutImage(void* win, int x, int y, int width, int height, unsigned char* data);

DESK_EXPORT void _DeskFillRectangle(void* win, int x, int y, int width, int height);

DESK_EXPORT void _DeskClear(void* win);

#ifdef __cplusplus
}
#endif

#endif
