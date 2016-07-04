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
#include <stdarg.h>
#ifdef _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#endif
#include "drawtext.h"
#include "drawtext_impl.h"

void dtx_set(enum dtx_option opt, int val)
{
	dtx_gl_setopt(opt, val);
	dtx_rast_setopt(opt, val);
}

int dtx_get(enum dtx_option opt)
{
	int val;

	if(dtx_gl_getopt(opt, &val) != -1) {
		return val;
	}
	if(dtx_rast_getopt(opt, &val) != -1) {
		return val;
	}
	return -1;
}


void dtx_position(float x, float y)
{
	dtx_cur_offset[0] = x;
	dtx_cur_offset[1] = y;
}

void dtx_color(float r, float g, float b, float a)
{
	dtx_cur_color[0] = r;
	dtx_cur_color[1] = g;
	dtx_cur_color[2] = b;
	dtx_cur_color[3] = a;

	dtx_cur_color_int[0] = r > 1.0 ? 255 : (int)(r * 255.0);
	dtx_cur_color_int[1] = g > 1.0 ? 255 : (int)(g * 255.0);
	dtx_cur_color_int[2] = b > 1.0 ? 255 : (int)(b * 255.0);
	dtx_cur_color_int[3] = a > 1.0 ? 255 : (int)(a * 255.0);
}

void dtx_string(const char *str)
{
	int should_flush = dtx_buf_mode == DTX_NBF;
	float pos_x = dtx_cur_offset[0];
	float pos_y = dtx_cur_offset[1];

	if(!dtx_font) {
		return;
	}

	while(*str) {
		str = dtx_drawchar(str, &pos_x, &pos_y, &should_flush);
	}

	if(should_flush) {
		dtx_drawflush();
	}
}

void dtx_printf(const char *fmt, ...)
{
	va_list ap;
	int buf_size;
	char *buf, tmp;

	if(!dtx_font) {
		return;
	}

	va_start(ap, fmt);
	buf_size = vsnprintf(&tmp, 0, fmt, ap);
	va_end(ap);

	if(buf_size == -1) {
		buf_size = 512;
	}

	buf = alloca(buf_size + 1);
	va_start(ap, fmt);
	vsnprintf(buf, buf_size + 1, fmt, ap);
	va_end(ap);

	dtx_string(buf);
}

void dtx_flush(void)
{
	dtx_drawflush();
}
