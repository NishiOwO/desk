#ifndef __DESK_INTERNAL_H__
#define __DESK_INTERNAL_H_

#include <Desk/Desk.h>

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stderr, "[Desk] " fmt "\n", __VA_ARGS__)
#else
#define LOG(...)
#endif

DESK_EXPORT void _DeskInit(void);

#ifdef __cplusplus
}
#endif

#endif
