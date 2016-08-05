/* Distance field libdrawtext example.
 *
 * Important parts are marked with XXX comments.
 */
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include "drawtext.h"

void disp(void);
void reshape(int x, int y);
void keyb(unsigned char key, int x, int y);

unsigned int create_program(const char *vsdr, const char *psdr);
unsigned int create_shader(unsigned int type, const char *sdr);

/* XXX fonts are represented by the opaque struct dtx_font type, so you
 * need to create at least one with dtx_open_font (see main).
 */
struct dtx_font *font, *font_dist;
float zoom = 1.0;

#define FONT_SZ	40


static const char *vsdr =
	"void main()\n"
	"{\n"
	"\tgl_Position = ftransform();\n"
	"\tgl_TexCoord[0] = gl_MultiTexCoord0;\n"
	"}\n";

static const char *psdr =
	"uniform sampler2D tex;\n"
	"void main()\n"
	"{\n"
	"\tfloat alpha = texture2D(tex, gl_TexCoord[0].st).a;\n"
	"\tfloat mask = smoothstep(0.48, 0.5, alpha);\n"
	"\tvec3 color = vec3(1.0) * smoothstep(0.49, 0.5, alpha);\n"
	"\tgl_FragColor.rgb = color;\n"
	"\tgl_FragColor.a = mask;\n"
	"}\n";

static unsigned int sdrprog;


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("libdrawtext example: distance fields");

	glutDisplayFunc(disp);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyb);

	glewInit();

	dtx_set(DTX_PADDING, 16);

	if(!(font = dtx_open_font_glyphmap("regular.glyphmap"))) {
		if(!(font = dtx_open_font("sans.ttf", 0))) {
			return 1;
		}
		dtx_prepare_range(font, FONT_SZ * 4, 32, 127);	/* printable ASCII range */
		dtx_resize_glyphmap(dtx_get_font_glyphmap(font, FONT_SZ * 4, 'a'), 1, 4, DTX_LINEAR);
		dtx_save_glyphmap("regular.glyphmap", dtx_get_font_glyphmap(font, FONT_SZ, 'a'));
	}

	if(!(font_dist = dtx_open_font_glyphmap("distfield.glyphmap"))) {
		if(!(font_dist = dtx_open_font("sans.ttf", 0))) {
			return 1;
		}
		/* XXX use a large font size for the initial distance field generation
		 * then resize the resulting glyphmap to 1/4 of its size afterwards.
		 */
		dtx_prepare_range(font_dist, FONT_SZ * 4, 32, 127); /* printable ASCII range */
		dtx_calc_font_distfield(font_dist, 1, 4);
		dtx_save_glyphmap("distfield.glyphmap", dtx_get_font_glyphmap(font_dist, FONT_SZ, 'a'));
	}

	sdrprog = create_program(vsdr, psdr);

	glutMainLoop();
	return 0;
}

void disp(void)
{
	static const char *text = "hello world!";

	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(zoom, zoom, zoom);

	/* regular font rendering */
	dtx_use_font(font, FONT_SZ);
	glTranslatef(-dtx_string_width(text) / 2, -120, 0);
	dtx_string(text);

	glTranslatef(0, 160, 0);

	/* distance field font rendering */
	dtx_use_font(font_dist, FONT_SZ);
	/*glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL, 0.5);*/
	glUseProgram(sdrprog);
	dtx_string(text);
	/*glDisable(GL_ALPHA_TEST);*/
	glUseProgram(0);

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
	switch(key) {
	case 27:
		exit(0);

	case '=':
		zoom *= 1.2;
		glutPostRedisplay();
		break;

	case '-':
		zoom *= 0.8;
		glutPostRedisplay();
		break;
	}
}

unsigned int create_program(const char *vsdr, const char *psdr)
{
	unsigned int vs, ps, prog;
	int status;

	if(!(vs = create_shader(GL_VERTEX_SHADER, vsdr))) {
		return 0;
	}
	if(!(ps = create_shader(GL_FRAGMENT_SHADER, psdr))) {
		glDeleteShader(vs);
		return 0;
	}

	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, ps);
	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if(!status) {
		fprintf(stderr, "failed to link shader program\n");
		glDeleteProgram(prog);
		prog = 0;
	}
	return prog;
}

unsigned int create_shader(unsigned int type, const char *src)
{
	unsigned int sdr;
	int status, info_len;

	sdr = glCreateShader(type);
	glShaderSource(sdr, 1, &src, 0);
	glCompileShader(sdr);

	glGetShaderiv(sdr, GL_COMPILE_STATUS, &status);
	if(!status) {
		fprintf(stderr, "failed to compile %s shader\n", type == GL_VERTEX_SHADER ? "vertex" : "pixel");
	}

	glGetShaderiv(sdr, GL_INFO_LOG_LENGTH, &info_len);
	if(info_len) {
		char *buf = alloca(info_len + 1);
		glGetShaderInfoLog(sdr, info_len, 0, buf);
		buf[info_len] = 0;
		if(buf[0]) {
			fprintf(stderr, "compiler output:\n%s\n", buf);
		}
	}

	if(!status) {
		glDeleteShader(sdr);
		sdr = 0;
	}
	return sdr;
}
