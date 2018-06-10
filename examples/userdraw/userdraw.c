/* User-supplied drawing callback libdrawtext example.
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

/* XXX callback prototype */
void drawcb(struct dtx_vertex *v, int vcount, struct dtx_pixmap *pixmap, void *cls);

struct dtx_font *font;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(512, 384);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("libdrawtext example: user-supplied draw callback");

	glutDisplayFunc(disp);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyb);

	if(!(font = dtx_open_font("serif.ttf", 24))) {
		fprintf(stderr, "failed to open font\n");
		return 1;
	}
	dtx_use_font(font, 24);

	/* XXX call dtx_target_user to enable the user-supplied callback drawing
	 * and set the callback function
	 */
	dtx_target_user(drawcb, 0);

	glutMainLoop();
	return 0;
}

const char *text = "Some sample text goes here.\n"
	"Yada yada yada, more text...\n"
	"foobar xyzzy\n";

void disp(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(-200, 150, 0);
	glColor3f(1, 1, 1);
	dtx_string(text);
	glPopMatrix();

	glutSwapBuffers();
}

void drawcb(struct dtx_vertex *v, int vcount, struct dtx_pixmap *pixmap, void *cls)
{
	int i;
	unsigned int tex;

	tex = (unsigned int)pixmap->udata;
	if(!tex) {
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, pixmap->width, pixmap->height,
				0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixmap->pixels);
		pixmap->udata = (void*)tex;
	}

	glBindTexture(GL_TEXTURE_2D, tex);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_TRIANGLES);
	for(i=0; i<vcount; i++) {
		glTexCoord2f(v->s, v->t);
		glVertex2f(v->x, v->y);
		++v;
	}
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
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
	switch(key) {
	case 27:
		exit(0);

	case ' ':
		{
			static int use_cb = 1;
			use_cb = !use_cb;
			if(use_cb) {
				dtx_target_user(drawcb, 0);
			} else {
				dtx_target_opengl();
			}
			glutPostRedisplay();
		}
		break;
	}
}
