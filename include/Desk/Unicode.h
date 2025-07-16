#ifndef __DESK_UNICODE_H__
#define __DESK_UNICODE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <Desk/Desk.h>

DESK_EXPORT int DeskUnicode8To32(const char* input, long* output);

#ifdef __cplusplus
}
#endif

#endif
