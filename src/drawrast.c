/*
libdrawtext - a simple library for fast text rendering in OpenGL
Copyright (C) 2011-2016  John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "drawtext.h"
#include "drawtext_impl.h"

static const char *drawchar(const char *str, float *xpos, float *ypos, int *should_flush);
static void flush(void);
static void draw_glyph(struct glyph *g, float x, float y);

static unsigned char *fb_pixels;
static int fb_width, fb_height;
static struct dtx_glyphmap *gmap;

void dtx_target_raster(unsigned char *pixels, int width, int height)
{
	fb_pixels = pixels;
	fb_width = width;
	fb_height = height;
	dtx_drawchar = drawchar;
	dtx_drawflush = flush;
}

static const char *drawchar(const char *str, float *xpos, float *ypos, int *should_flush)
{
	float px, py;
	int code = dtx_utf8_char_code(str);
	str = dtx_utf8_next_char((char*)str);

	*should_flush = 0;	/* the raster renderer never buffers output */

	px = *xpos;
	py = *ypos;

	if((gmap = dtx_proc_char(code, xpos, ypos))) {
		int idx = code - gmap->cstart;
		draw_glyph(gmap->glyphs + idx, px, py);
	}
	return str;
}

static void flush(void)
{
}

static void draw_glyph(struct glyph *g, float x, float y)
{
	int i, j;
	int ix = (int)(x - g->orig_x);
	int iy = (int)(y - g->orig_y);
	int gx = (int)g->x;
	int gy = (int)g->y;
	int gwidth = (int)g->width;
	int gheight = (int)g->height;
	int *col = dtx_cur_color_int;
	int xend = ix + gwidth;
	int yend = iy + gheight;

	if(ix >= fb_width || iy >= fb_height)
		return;

	if(ix < 0) {
		gwidth += ix;
		ix = 0;
	}
	if(iy < 0) {
		gheight += iy;
		iy = 0;
	}
	if(xend >= fb_width) {
		gwidth = fb_width - ix;
	}
	if(yend >= fb_height) {
		gheight = fb_height - iy;
	}

	if(gwidth <= 0 || gheight <= 0)
		return;

	unsigned char *dest = fb_pixels + (iy * fb_width + ix) * 4;
	unsigned char *src = gmap->pixels + gy * gmap->xsz + gx;
	for(i=0; i<gheight; i++) {
		for(j=0; j<gwidth; j++) {
			int val = src[j];
			*dest++ = val * col[0] / 255;
			*dest++ = val * col[1] / 255;
			*dest++ = val * col[2] / 255;
			*dest++ = val;
		}
		dest += (fb_width - gwidth) * 4;
		src += gmap->xsz;
	}
}
