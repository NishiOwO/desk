#ifndef __DESK_INTERNAL_H__
#define __DESK_INTERNAL_H_

#include <Desk/Desk.h>

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stderr, "[Desk, %s] " fmt "\n", __FILE__, __VA_ARGS__)
#else
#define LOG(...)
#endif

DESK_EXPORT void _DeskInit(void);

DESK_EXPORT void* _DeskCreateWindow(void* parent, int x, int y, int width, int height);

DESK_EXPORT void _DeskDestroyWindow(void* win);

DESK_EXPORT int _DeskPending(void* win);

DESK_EXPORT void _DeskStep(void* win, int* render);

DESK_EXPORT void _DeskSetCoord(void* win, int x, int y, int width, int height, int flag);

#ifdef __cplusplus
}
#endif

#endif
