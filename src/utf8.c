#include "drawtext.h"

#define	U8_IS_FIRST(x)		(((((x) >> 7) & 1) == 0) || ((((x) >> 6) & 3) == 3))

static const char first_mask[] = {
	0,
	0x7f,	/* single byte, 7 bits valid */
	0x1f,	/* two-bytes, 5 bits valid */
	0xf,	/* three-bytes, 4 bits valid */
	0x7		/* four-bytes, 3 bits valid */
};
static const char first_shift[] = { 7, 5, 4, 3 };	/* see above */

#define CONT_PREFIX	0x80
#define CONT_MASK	0x3f
#define CONT_SHIFT	6

/* last charcodes for 1, 2, 3 or 4-byte utf8 chars */
static const int utf8_lastcode[] = { 0x7f, 0x7ff, 0xfff, 0x1fffff };

#define prefix_mask(x)	(~first_mask[x])
#define prefix(x)		((prefix_mask(x) << 1) & 0xff)


char *dtx_utf8_next_char(char *str)
{
	return str + dtx_utf8_nbytes(str);
}

int dtx_utf8_char_code(const char *str)
{
	int i, nbytes, shift, code = 0;
	char mask;

	if(!U8_IS_FIRST(*str)) {
		return -1;
	}

	nbytes = dtx_utf8_nbytes(str);
	mask = first_mask[nbytes];
	shift = 0;

	for(i=0; i<nbytes; i++) {
		if(!*str) {
			break;
		}

		code = (code << shift) | (*str++ & mask);
		mask = 0x3f;
		shift = i == 0 ? first_shift[nbytes] : 6;
	}

	return code;
}

int dtx_utf8_nbytes(const char *str)
{
	int i, numset = 0;
	int c = *str;

	if(!U8_IS_FIRST(c)) {
		for(i=0; !U8_IS_FIRST(str[i]); i++);
		return i;
	}

	/* count the leading 1s */
	for(i=0; i<4; i++) {
		if(((c >> (7 - i)) & 1) == 0) {
			break;
		}
		numset++;
	}

	if(!numset) {
		return 1;
	}
	return numset;
}

int dtx_utf8_char_count(const char *str)
{
	int n = 0;

	while(*str) {
		n++;
		str = dtx_utf8_next_char((char*)str);
	}
	return n;
}

size_t dtx_utf8_from_char_code(int code, char *buf)
{
	size_t nbytes = 0;
	int i;

	for(i=0; i<4; i++) {
		if(code <= utf8_lastcode[i]) {
			nbytes = i + 1;
			break;
		}
	}

	if(!nbytes && buf) {
		for(i=0; i<nbytes; i++) {
			int idx = nbytes - i - 1;
			int mask, shift, prefix;

			if(idx > 0) {
				mask = CONT_MASK;
				shift = CONT_SHIFT;
				prefix = CONT_PREFIX;
			} else {
				mask = first_mask[nbytes];
				shift = first_shift[nbytes];
				prefix = prefix(nbytes);
			}

			buf[idx] = (code & mask) | (prefix & ~mask);
			code >>= shift;
		}
	}
	return nbytes;
}

size_t dtx_utf8_from_string(const wchar_t *str, char *buf)
{
	size_t nbytes = 0;
	char *ptr = buf;

	while(*str) {
		int cbytes = dtx_utf8_from_char_code(*str++, ptr);
		if(ptr) {
			ptr += cbytes;
		}
		nbytes += cbytes;
	}
	return nbytes;
}
