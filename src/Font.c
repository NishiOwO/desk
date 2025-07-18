#include <Desk/Desk.h>
#include <Desk/Font.h>
#include <Desk/Unicode.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FONT_TTF "font.ttf"

DeskFont DeskFontOpen(const char* path){
	DeskFont f;
	long sz;
	int st;
	FILE* fp = fopen(path == NULL ? FONT_TTF : path, "rb");
	if(fp == NULL){
		return DeskFontNone;
	}
	
	fseek(fp, 0, SEEK_END);
	sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	f = malloc(sizeof(*f));
	memset(f, 0, sizeof(*f));

	f->buffer = malloc(sz);
	fread(f->buffer, 1, sz, fp);

	f->ttf = malloc(sizeof(*f->ttf));
	st = stbtt_InitFont(f->ttf, f->buffer, 0);
	fclose(fp);

	if(!st){
		free(f->buffer);
		free(f->ttf);
		free(f);
		return DeskFontNone;
	}

	return f;
}

unsigned char* DeskFontRender(DeskFont font, const char* str, int size, int* width, int* height){
	long* l = malloc(sizeof(*l) * strlen(str) * 5);
	int incr = 0;
	int i;
	int ascent, descent, linegap;
	const char* s = str;
	double scale;
	unsigned char* r = NULL;
	int ix = 0, iy = 0;
	l[0] = 0;

	while(s[0] != 0){
		int sz = DeskUnicode8To32(s, l + incr);
		if(sz == 0){
			free(l);
			l = malloc(sizeof(*l) * 10);
			l[0] = '(';
			l[1] = 'I';
			l[2] = 'N';
			l[3] = 'V';
			l[4] = 'U';
			l[5] = 'T';
			l[6] = 'F';
			l[7] = '8';
			l[8] = ')';
			l[9] = 0;
			break;
		}
		incr++;
		l[incr] = 0;
		s += sz;
	}

	stbtt_GetFontVMetrics(font->ttf, &ascent, &descent, &linegap);
	scale	   = (double)size / (ascent - descent);

	for(i = 0; l[i] != 0; i++){
		int x1, x2, y1, y2, ax, lsb, kern;
		unsigned char* buf;
		stbtt_GetCodepointBitmapBox(font->ttf, l[i], scale, scale, &x1, &y1, &x2, &y2);
		stbtt_GetCodepointHMetrics(font->ttf, l[i], &ax, &lsb);
		buf = malloc((x2 - x1) * (y2 - y1));
		stbtt_MakeCodepointBitmap(font->ttf, buf, x2 - x1, y2 - y1, x2 - x1, scale, scale, l[i]);
		free(buf);
	}

	free(l);

	return r;
}

void DeskFontDestroy(DeskFont font){
	free(font->buffer);
	free(font->ttf);
	free(font);
}
