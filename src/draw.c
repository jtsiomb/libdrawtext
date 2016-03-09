#include <stdio.h>
#include <stdarg.h>
#ifdef _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#endif
#include "drawtext.h"
#include "drawtext_impl.h"
#include "draw.h"

const char *dtx_put_char(const char *str, float *pos_x, float *pos_y, int *should_flush);


void dtx_string(const char *str)
{
	int should_flush = dtx_buf_mode == DTX_NBF;
	float pos_x = 0.0f;
	float pos_y = 0.0f;

	if(!dtx_font) {
		return;
	}

	while(*str) {
		str = dtx_put_char(str, &pos_x, &pos_y, &should_flush);
	}

	if(should_flush) {
		dtx_flush();
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

	buf_size = vsnprintf(&tmp, 0, fmt, ap);

	if(buf_size == -1) {
		buf_size = 512;
	}

	buf = alloca(buf_size + 1);
	va_start(ap, fmt);
	vsnprintf(buf, buf_size + 1, fmt, ap);
	va_end(ap);

	dtx_string(buf);
}
