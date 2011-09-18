/* Unicode libdrawtext example.
 *
 * Important parts are marked with XXX comments.
 */
#include <stdio.h>
#include <stdlib.h>

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include "drawtext.h"

void disp(void);
void reshape(int x, int y);
void keyb(unsigned char key, int x, int y);

/* XXX fonts are represented by the opaque struct dtx_font type, so you
 * need to create at least one with dtx_open_font (see main).
 */
struct dtx_font *fntmain, *fntcjk, *fntklingon;

#define FONT_SZ	24

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(512, 384);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("libdrawtext example: unicode");

	glutDisplayFunc(disp);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyb);

	/* XXX dtx_open_font opens a font file and returns a pointer to dtx_font */
	if(!(fntmain = dtx_open_font("serif.ttf", 0))) {
		return 1;
	}
	dtx_prepare_range(fntmain, FONT_SZ, 0, 256);			/* ASCII */
	dtx_prepare_range(fntmain, FONT_SZ, 0x370, 0x400);		/* greek */
	dtx_prepare_range(fntmain, FONT_SZ, 0x400, 0x500);		/* cyrilic */

	if(!(fntcjk = dtx_open_font("cjk.ttf", 0))) {
		return 1;
	}
	dtx_prepare_range(fntcjk, FONT_SZ, 0x4e00, 0x9fc0);		/* kanji */

	if(!(fntklingon = dtx_open_font("klingon.ttf", 0))) {
		return 1;
	}
	dtx_prepare_range(fntklingon, FONT_SZ, 0xf8d0, 0xf900);	/* klingon */

	glutMainLoop();
	return 0;
}

/* various UTF-8 strings */
const char *english_text = "Hello world!";
const char *greek_text = "\xce\x9a\xce\xbf\xcf\x8d\xcf\x81\xce\xb1\xcf\x83\xce\xb7";
const char *russian_text = "\xd0\xa0\xd0\xb0\xd1\x81\xd1\x86\xd0\xb2\xd0\xb5\xd1\x82\xd0\xb0\xd0\xbb\xd0\xb8 \xd1\x8f\xd0\xb1\xd0\xbb\xd0\xbe\xd0\xbd\xd0\xb8 \xd0\xb8 \xd0\xb3\xd1\x80\xd1\x83\xd1\x88\xd0\xb8";
const char *kanji_text = "\xe4\xb9\x97\xe4\xba\xac";
const char *klingon_text = "\xef\xa3\xa3\xef\xa3\x9d\xef\xa3\x93\xef\xa3\x98\xef\xa3\x9d\xef\xa3\xa2\xef\xa3\xa1\xef\xa3\x9d\xef\xa3\x99";

void draw_box(struct dtx_box *box)
{
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 1, 0);
	glVertex2f(box->x, box->y);
	glVertex2f(box->x + box->width, box->y);
	glVertex2f(box->x + box->width, box->y + box->height);
	glVertex2f(box->x, box->y + box->height);
	glEnd();
	glColor3f(1, 1, 1);
}

void disp(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	dtx_use_font(fntmain, FONT_SZ);

	glTranslatef(-200, 150, 0);
	/* XXX call dtx_string to draw utf-8 text.
	 * any transformations and the current color apply
	 */
	dtx_string(english_text);

	glTranslatef(0, -40, 0);
	dtx_string(greek_text);

	glTranslatef(0, -40, 0);
	dtx_string(russian_text);

	dtx_use_font(fntcjk, FONT_SZ);

	glTranslatef(0, -40, 0);
	dtx_string(kanji_text);

	dtx_use_font(fntklingon, FONT_SZ);

	glTranslatef(0, -40, 0);
	dtx_string(klingon_text);

	glutSwapBuffers();
}

void reshape(int x, int y)
{
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-x/2, x/2, -y/2, y/2, -1, 1);
}

void keyb(unsigned char key, int x, int y)
{
	if(key == 27) {
		exit(0);
	}
}
