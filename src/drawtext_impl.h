#ifndef TEXT_IMPL_H_
#define TEXT_IMPL_H_

struct glyph {
	int code;
	float x, y, width, height;
	/* normalized coords [0, 1] */
	float nx, ny, nwidth, nheight;
	float orig_x, orig_y;
	float advance;
	struct glyph *next;
};

struct dtx_glyphmap {
	int ptsize;

	int xsz, ysz;
	unsigned char *pixels;
	unsigned int tex;

	int cstart, cend;	/* character range */
	int crange;

	float line_advance;

	struct glyph *glyphs;
	struct dtx_glyphmap *next;
};

struct dtx_font {
	/* freetype FT_Face */
	void *face;

	/* list of glyphmaps */
	struct dtx_glyphmap *gmaps;

	/* last returned glyphmap (cache) */
	struct dtx_glyphmap *last_gmap;
};


#define fperror(str) \
	fprintf(stderr, "%s: %s: %s\n", __func__, (str), strerror(errno))

#endif	/* TEXT_IMPL_H_ */
