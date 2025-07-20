#include <Desk/Desk.h>
#include <Desk/Font.h>
#include <Desk/Unicode.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FONT_TTF "font.ttf"

void DeskFontDraw(DeskWidget w, DeskFont font, const char* str, int x, int y, int size){
	int width, height;
	unsigned char* d = DeskFontRender(font, str, size, &width, &height);
	unsigned char* fb = malloc(width * height * 4);
	int i;
	for(i = 0; i < width * height; i++){
		unsigned char px;
		memset(&fb[i * 4], 0, 4);
		if((px = d[i]) != 0){
			fb[i * 4 + 3] = px;
		}
	}
	_DeskPutImage(w->window, x, y, width, height, fb);
	free(fb);
	free(d);
}

DeskFont DeskFontOpen(const char* path){
	DeskFont f;
	long sz;
	int st;
	FILE* fp = fopen(path == NULL ? FONT_TTF : path, "rb");
	if(fp == NULL){
		LOG("Could not open %s", path == NULL ? FONT_TTF : path);
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

	*width = 0;
	*height = 0;

	if(font == DeskFontNone){
		free(l);
		unsigned char* px = malloc(4);
		*width = 1;
		*height = 1;
		memset(px, 0, 4);
		return px;
	}

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

	*height = (ascent - descent + linegap) * scale;

	for(i = 0; l[i] != 0; i++){
		int ax, lsb, kern;
		kern = 0;
		stbtt_GetCodepointHMetrics(font->ttf, l[i], &ax, &lsb);

		if(l[i + 1] != 0) kern = stbtt_GetCodepointKernAdvance(font->ttf, l[i], l[i + 1]);
		*width += (ax + lsb + kern) * scale;
	}

	r = malloc((*width) * (*height));
	memset(r, 0, (*width) * (*height));

	for(i = 0; l[i] != 0; i++){
		int x1, x2, y1, y2, ax, lsb, kern, x, y;
		unsigned char* buf;
		kern = 0;
		stbtt_GetCodepointBitmapBox(font->ttf, l[i], scale, scale, &x1, &y1, &x2, &y2);
		stbtt_GetCodepointHMetrics(font->ttf, l[i], &ax, &lsb);
		if(l[i + 1] != 0) kern = stbtt_GetCodepointKernAdvance(font->ttf, l[i], l[i + 1]);
		buf = malloc((x2 - x1) * (y2 - y1));
		stbtt_MakeCodepointBitmap(font->ttf, buf, x2 - x1, y2 - y1, x2 - x1, scale, scale, l[i]);

		ix += lsb * scale;
		for(y = 0; y < (y2 - y1); y++){
			for(x = 0; x < (x2 - x1); x++){
				int px = ix + x, py = iy + y + y1 + (ascent) * scale;
				if(px < 0 || px >= (*width) || py < 0 || py >= (*height)) continue;
				r[py * (*width) + px] = buf[y * (x2 - x1) + x];
			}
		}

		ix += ax * scale + kern * scale;

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
