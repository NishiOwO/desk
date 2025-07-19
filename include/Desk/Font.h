#ifndef __DESK_FONT_H__
#define __DESK_FONT_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _DeskFontRec DeskFontRec;
typedef DeskFontRec* DeskFont;

#include <Desk/Desk.h>

#ifdef DESK_INTERNAL
#include <stb_truetype.h>
typedef stbtt_fontinfo* DeskFontTTF;
#else
typedef void* DeskFontTTF;
#endif

struct _DeskFontRec {
	DeskFontTTF ttf;
	unsigned char* buffer;
};

#define DeskFontNone ((DeskFont)NULL)

DESK_EXPORT DeskFont DeskFontOpen(const char* path);

DESK_EXPORT unsigned char* DeskFontRender(DeskFont font, const char* str, int size, int* width, int* height);

DESK_EXPORT void DeskFontDraw(DeskWidget w, DeskFont font, const char* str, int x, int y, int size);

DESK_EXPORT void DeskFontDestroy(DeskFont font);

#ifdef __cplusplus
}
#endif

#endif
