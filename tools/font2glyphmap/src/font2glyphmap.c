#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "drawtext.h"

#define SUFFIX	"glyphmap"
#define DEF_SIZE	12

struct coderange {
	int start, end;
	struct coderange *next;
};

void print_usage(const char *argv0);
int font2glyphmap(struct dtx_font *font, const char *infname, const char *outfname, int size, int rstart, int rend,
		int conv_dist, int scale_num, int scale_denom);

int main(int argc, char **argv)
{
	int conv_dist = 0;
	int scale_num = -1, scale_denom = 1;
	int i, font_size = DEF_SIZE, suffix_len = strlen(SUFFIX);
	struct coderange *clist = 0;
	char *outfile_override = 0;

	for(i=1; i<argc; i++) {
		if(argv[i][0] == '-') {
			if(strcmp(argv[i], "-range") == 0) {
				struct coderange *node;
				int start, end;

				if(sscanf(argv[++i], "%i-%i", &start, &end) != 2) {
					fprintf(stderr, "-range must be followed by a range of the form: START-END\n");
					return 1;
				}

				if(!(node = malloc(sizeof *node))) {
					perror("failed to allocate memory");
					return 1;
				}
				node->start = start;
				node->end = end;
				node->next = clist;
				clist = node;
			} else if(strcmp(argv[i], "-size") == 0) {
				char *endp;

				font_size = strtol(argv[++i], &endp, 10);
				if(endp == argv[i]) {
					fprintf(stderr, "-size must be followed by the font size\n");
					return 1;
				}
			} else if(strcmp(argv[i], "-dist") == 0) {
				conv_dist = 1;

			} else if(strcmp(argv[i], "-padding") == 0) {
				char *endp;
				int pad = strtol(argv[++i], &endp, 10);
				if(endp == argv[i]) {
					fprintf(stderr, "-padding must be followed by a number\n");
					return 1;
				}
				dtx_set(DTX_PADDING, pad);

			} else if(strcmp(argv[i], "-scale") == 0) {
				if(sscanf(argv[++i], "%d/%d", &scale_num, &scale_denom) < 1 || scale_num <= 0) {
					fprintf(stderr, "-scale must be followed by a non-zero fraction of the form 1/x or x/1\n");
					return 1;
				}

			} else if(strcmp(argv[i], "-o") == 0) {
				outfile_override = argv[++i];

			} else {
				if(strcmp(argv[i], "-help") != 0 && strcmp(argv[i], "-h") != 0) {
					fprintf(stderr, "invalid option: %s\n", argv[i]);
				}
				print_usage(argv[0]);
				return 1;
			}
		} else {
			char *basename, *dotptr, *outfile, *lastslash;
			struct dtx_font *font;

			if(!(font = dtx_open_font(argv[i], clist ? 0 : font_size))) {
				fprintf(stderr, "failed to open font file: %s\n", argv[i]);
				return -1;
			}

			if(outfile_override) {
				outfile = outfile_override;
			} else {
				basename = alloca(strlen(argv[i]) + suffix_len + 1);
				strcpy(basename, argv[i]);

				if((dotptr = strrchr(basename, '.'))) {
					*dotptr = 0;
				}
				if((lastslash = strrchr(basename, '/'))) {
					basename = lastslash + 1;
				}

				outfile = alloca(strlen(basename) + 64);
			}

			if(clist) {
				while(clist) {
					struct coderange *r = clist;
					clist = clist->next;

					if(!outfile_override) {
						sprintf(outfile, "%s_s%d_r%04x-%04x.%s", basename, font_size, r->start, r->end, SUFFIX);
					}
					font2glyphmap(font, argv[i], outfile, font_size, r->start, r->end, conv_dist, scale_num, scale_denom);

					free(r);
				}
				clist = 0;
			} else {
				if(!outfile_override) {
					sprintf(outfile, "%s_s%d.%s", basename, font_size, SUFFIX);
				}
				font2glyphmap(font, argv[i], outfile, font_size, -1, -1, conv_dist, scale_num, scale_denom);
			}
		}
	}

	return 0;
}

void print_usage(const char *argv0)
{
	printf("usage: %s [options] <font-1> [<font-2> ... <font-n>]\n", argv0);
	printf("options:\n");
	printf("  -size <pt>: point size (default: %d)\n", DEF_SIZE);
	printf("  -range <low>-<high>: unicode range (default: ascii)\n");
	printf("  -dist: convert to distance field glyphmap\n");
	printf("  -scale <factor>: scale the glyphmap by factor before saving it\n");
	printf("  -pad <pixels>: padding to leave between glyphs\n");
	printf("  -o <filename>: output filename\n");
	printf("  -help: print usage information and exit\n");
}

int font2glyphmap(struct dtx_font *font, const char *infname, const char *outfname, int size, int rstart, int rend,
		int conv_dist, int scale_num, int scale_denom)
{
	struct dtx_glyphmap *gmap;

	if(rstart != -1) {
		dtx_prepare_range(font, size, rstart, rend);
		if(!(gmap = dtx_get_font_glyphmap(font, size, rstart))) {
			fprintf(stderr, "failed to retrieve unicode glyphmap (code range: %d-%d)\n", rstart, rend);
			return -1;
		}
	} else {
		dtx_prepare(font, size);
		if(!(gmap = dtx_get_font_glyphmap(font, size, ' '))) {
			fprintf(stderr, "failed to retrieve ASCII glyphmap!\n");
			return -1;
		}
	}

	if(conv_dist) {
		if(dtx_calc_glyphmap_distfield(gmap) == -1) {
			fprintf(stderr, "failed to convert font to distance field!\n");
			return -1;
		}
		if(scale_num <= 0.0) {
			scale_num = 1;
			scale_denom = 2;
		}
	}

	if(scale_num > 0) {
		int xsz = dtx_get_glyphmap_width(gmap);
		int ysz = dtx_get_glyphmap_height(gmap);
		int new_xsz = scale_num * dtx_get_glyphmap_width(gmap) / scale_denom;
		int new_ysz = scale_num * dtx_get_glyphmap_height(gmap) / scale_denom;
		printf("scaling %dx%d -> %dx%d\n", xsz, ysz, new_xsz, new_ysz);

		if(dtx_resize_glyphmap(gmap, scale_num, scale_denom, DTX_LINEAR) == -1) {
			fprintf(stderr, "failed to resize glyphmap to %dx%d\n", new_xsz, new_ysz);
			return -1;
		}
	}

	if(dtx_save_glyphmap(outfname, gmap) == -1) {
		fprintf(stderr, "failed to save glyphmap to: %s\n", outfname);
		return -1;
	}

	return 0;
}
