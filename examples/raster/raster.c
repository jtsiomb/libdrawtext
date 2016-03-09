#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "drawtext.h"

struct image {
	int width, height;
	unsigned char *pixels;
};

int save_image(const struct image *img, const char *fname);

int main(int argc, char **argv)
{
	struct dtx_font *font;
	struct image img;

	if(!(font = dtx_open_font("serif.ttf", 32))) {
		fprintf(stderr, "failed to open font\n");
		return 1;
	}
	dtx_use_font(font, 32);

	/* create the raster image we'll draw into */
	img.width = 800;
	img.height = 600;
	if(!(img.pixels = malloc(img.width * img.height * 4))) {
		perror("failed to allocate image memory");
		return 1;
	}
	memset(img.pixels, 0, img.width * img.height * 4);	/* clear to black */

	/* set the render target to libdrawtext and enable raster drawing */
	dtx_target_raster(img.pixels, img.width, img.height);
	dtx_color(1, 0, 0, 1);	/* red text */

	dtx_printf("hello world!");
	if(save_image(&img, "output.ppm") == -1) {
		return 1;
	}
	printf("output.ppm saved\n");
	return 0;
}

int save_image(const struct image *img, const char *fname)
{
	int i;
	FILE *fp;
	unsigned char *pixptr = img->pixels;

	if(!(fp = fopen(fname, "wb"))) {
		fprintf(stderr, "failed to open %s for writing: %s\n", fname, strerror(errno));
		return -1;
	}

	fprintf(fp, "P6\n%d %d\n255\n", img->width, img->height);
	for(i=0; i<img->width * img->height; i++) {
		fputc(*pixptr++, fp);
		fputc(*pixptr++, fp);
		fputc(*pixptr++, fp);
		++pixptr;
	}
	fclose(fp);
	return 0;
}
