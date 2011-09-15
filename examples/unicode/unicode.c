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
struct dtx_font *freeserif, *klingon;

#define FONT_SIZE	24

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(512, 384);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("libdrawtext example: simple");

	glutDisplayFunc(disp);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyb);

	/* XXX dtx_open_font opens a font file and returns a pointer to dtx_font */
	if(!(freeserif = dtx_open_font("serif.ttf", 0))) {
		return 1;
	}
	dtx_prepare_range(freeserif, FONT_SIZE, 0, 256);	/* prepare ASCII */
	dtx_prepare_range(freeserif, FONT_SIZE, 0x0370, 0x0400);	/* greek */
	dtx_prepare_range(freeserif, FONT_SIZE, 0x4e00, 0x9fc0);	/* kanji */

	if(!(klingon = dtx_open_font("klingon.ttf", 0))) {
		return 1;
	}
	dtx_prepare_range(klingon, FONT_SIZE, 0xf8d0, 0xf900);

	glutMainLoop();
	return 0;
}

const char *ascii_text = "Hello world!";
const char *greek_text = "\xce\x9a\xce\xbf\xcf\x8d\xcf\x81\xce\xb1\xcf\x83\xce\xb7";
const char *kanji_text = "\xe4\xb9\x97\xe4\xba\xac";
const char *klingon_text = "\xef\xa3\xa3\xef\xa3\x9d\xef\xa3\x93\xef\xa3\x98\xef\xa3\x9d\xef\xa3\xa2\xef\xa3\xa1\xef\xa3\x9d\xef\xa3\x99";


void disp(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	dtx_use_font(freeserif, FONT_SIZE);

	glTranslatef(-200, 150, 0);
	/* XXX call dtx_string to draw utf-8 text.
	 * any transformations and the current color apply
	 */
	dtx_string(ascii_text);

	glTranslatef(0, -40, 0);
	dtx_string(greek_text);

	glTranslatef(0, -40, 0);
	dtx_string(kanji_text);

	dtx_use_font(klingon, FONT_SIZE);

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
