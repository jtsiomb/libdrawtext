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
float frand(void);
unsigned int get_msec(void);

int main(int argc, char **argv)
{
	int i, count = 128;
	struct dtx_font *font;
	struct image img;
	unsigned int start_msec, dur;

	for(i=1; i<argc; i++) {
		if(argv[i][0] == '-') {
			if(strcmp(argv[i], "-opaque") == 0) {
				printf("using opaque rendering\n");
				dtx_set(DTX_RASTER_THRESHOLD, -1);
				dtx_set(DTX_RASTER_BLEND, 0);
			} else if(strcmp(argv[i], "-threshold") == 0) {
				int thres = atoi(argv[++i]);
				dtx_set(DTX_RASTER_THRESHOLD, thres);
				printf("using threshold rendering with threshold: %d\n", thres);
			} else if(strcmp(argv[i], "-blend") == 0) {
				printf("using alpha blended rendering\n");
				dtx_set(DTX_RASTER_BLEND, 1);	/* enable alpha blended drawing */
			} else {
				printf("usage: [-opaque|-threshold <n>|-blend] [num hello worlds]\n");
				return 0;
			}
		} else {
			count = atoi(argv[i]);
		}
	}

	if(!(font = dtx_open_font("serif.ttf", 32))) {
		fprintf(stderr, "failed to open font\n");
		return 1;
	}
	dtx_use_font(font, 32);

	/* create the raster image we'll draw into */
	img.width = 640;
	img.height = 480;
	if(!(img.pixels = malloc(img.width * img.height * 4))) {
		perror("failed to allocate image memory");
		return 1;
	}
	memset(img.pixels, 0x20, img.width * img.height * 4);	/* clear fb */

	/* set the render target to libdrawtext and enable raster drawing */
	dtx_target_raster(img.pixels, img.width, img.height);

	printf("printing %d hello worlds\n", count);
	start_msec = get_msec();

	for(i=0; i<count; i++) {
		dtx_color(frand(), frand(), frand(), 1);
		dtx_position((frand() * 1.25 - 0.25) * img.width,
				(frand() * 1.25 - 0.25) * img.height);
		dtx_printf("hello world!");
	}

	dur = get_msec() - start_msec;
	printf("printed %d hello worlds in %u milliseconds\n", i, dur);

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

float frand(void)
{
	return (float)rand() / (float)RAND_MAX;
}

#if defined(__unix__) || defined(__APPLE__)
#include <time.h>

unsigned int get_msec(void)
{
	static struct timespec ts0;
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	if(ts.tv_sec == 0 && ts.tv_nsec == 0) {
		ts0 = ts;
		return 0;
	}
	return (ts.tv_sec - ts0.tv_sec) * 1000 + (ts.tv_nsec - ts0.tv_nsec) / 1000000;
}

#elif defined(WIN32)
#include <windows.h>
#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")
#endif

unsigned int get_msec(void)
{
	return timeGetTime();
}

#else

unsigned int get_msec(void)
{
	static int warned;
	if(!warned) {
		fprintf(stderr, "sorry, benchmark mode not implemented on your platform.\n");
		warned = 1;
	}
	return 0;
}
#endif
