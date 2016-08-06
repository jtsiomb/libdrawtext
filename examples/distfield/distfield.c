/* Distance field libdrawtext example.
 *
 * Important parts are marked with XXX comments.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <GL/glew.h>
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include "drawtext.h"

void disp(void);
void reshape(int x, int y);
void keydown(unsigned char key, int x, int y);
void keyup(unsigned char key, int x, int y);
void mouse(int bn, int st, int x, int y);
void motion(int x, int y);

unsigned int create_program(const char *vsdr, const char *psdr);
unsigned int create_shader(unsigned int type, const char *sdr);

/* XXX fonts are represented by the opaque struct dtx_font type, so you
 * need to create at least one with dtx_open_font (see main).
 */
struct dtx_font *font, *font_dist;
int cur_sel = -1;
float zoom[2] = {1.0, 1.0};

struct vec2 {
	float x, y;
} pos[2] = {
	{0, -100},
	{0, 100}
};

#define FONT_SZ	24
#define SCALE_FACTOR 8


static const char *vsdr =
	"void main()\n"
	"{\n"
	"\tgl_Position = ftransform();\n"
	"\tgl_TexCoord[0] = gl_MultiTexCoord0;\n"
	"}\n";

static const char *psdr =
	"uniform sampler2D tex;\n"
	"uniform float softness;\n"
	"void main()\n"
	"{\n"
	"\tfloat alpha = texture2D(tex, gl_TexCoord[0].st).a;\n"
	"\tfloat mask = smoothstep(0.5 - softness, 0.5 + softness, alpha);\n"
	"\tvec3 color = vec3(1.0);// * smoothstep(0.51 - softness, 0.51 + softness, alpha);\n"
	"\tgl_FragColor.rgb = color;\n"
	"\tgl_FragColor.a = mask;\n"
	"}\n";

static unsigned int sdrprog;
static int uloc_soft = -1;
static float softness = 0.015;


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	dtx_set(DTX_PADDING, 64);

	if(!(font = dtx_open_font_glyphmap("regular.glyphmap")) ||
			dtx_get_glyphmap_ptsize(dtx_get_glyphmap(font, 0)) != FONT_SZ) {
		if(!(font = dtx_open_font("sans.ttf", 0))) {
			return 1;
		}
		dtx_prepare_range(font, FONT_SZ * SCALE_FACTOR, 32, 127);	/* printable ASCII range */
		dtx_resize_glyphmap(dtx_get_glyphmap(font, 0), 1, SCALE_FACTOR, DTX_LINEAR);
		dtx_save_glyphmap("regular.glyphmap", dtx_get_glyphmap(font, 0));
	}

	if(!(font_dist = dtx_open_font_glyphmap("distfield.glyphmap")) ||
			dtx_get_glyphmap_ptsize(dtx_get_glyphmap(font_dist, 0)) != FONT_SZ) {
		if(!(font_dist = dtx_open_font("sans.ttf", 0))) {
			return 1;
		}
		/* XXX use a large font size for the initial distance field generation
		 * then resize the resulting glyphmap to 1/8 of its size afterwards.
		 */
		dtx_prepare_range(font_dist, FONT_SZ * SCALE_FACTOR, 32, 127); /* printable ASCII range */
		dtx_calc_font_distfield(font_dist, 1, SCALE_FACTOR);
		dtx_save_glyphmap("distfield.glyphmap", dtx_get_glyphmap(font_dist, 0));
	}

	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("libdrawtext example: distance fields");

	glutDisplayFunc(disp);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glewInit();


	sdrprog = create_program(vsdr, psdr);
	glUseProgram(sdrprog);
	uloc_soft = glGetUniformLocation(sdrprog, "softness");

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

	/* regular font rendering */
	dtx_use_font(font, FONT_SZ);

	glPushMatrix();
	glTranslatef(pos[0].x - zoom[0] * dtx_string_width(text) / 2, pos[0].y, 0);
	glScalef(zoom[0], zoom[0], zoom[0]);
	dtx_string(text);
	glPopMatrix();

	/* distance field font rendering */
	dtx_use_font(font_dist, FONT_SZ);

	glPushMatrix();
	glTranslatef(pos[1].x - zoom[1] * dtx_string_width(text) / 2, pos[1].y, 0);
	glScalef(zoom[1], zoom[1], zoom[1]);
	glUseProgram(sdrprog);
	if(uloc_soft != -1) {
		glUniform1f(uloc_soft, softness);
	}
	dtx_string(text);
	glUseProgram(0);

	glPopMatrix();

	glutSwapBuffers();
}

void reshape(int x, int y)
{
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-x/2, x/2, -y/2, y/2, -1, 1);
}

void keydown(unsigned char key, int x, int y)
{
	switch(key) {
	case 27:
		exit(0);

	case '1':
	case '2':
		cur_sel = key - '1';
		break;

	case '=':
		softness += 0.005;
		printf("softness: %g\n", softness);
		glutPostRedisplay();
		break;

	case '-':
		softness -= 0.005;
		if(softness < 0.0) {
			softness = 0.0;
		}
		printf("softness: %g\n", softness);
		glutPostRedisplay();
		break;
	}
}

void keyup(unsigned char key, int x, int y)
{
	switch(key) {
	case '1':
	case '2':
		cur_sel = -1;
		break;
	}
}

static int bnstate[8];
static int prev_x, prev_y;

void mouse(int bn, int st, int x, int y)
{
	bnstate[bn - GLUT_LEFT] = st == GLUT_DOWN ? 1 : 0;
	prev_x = x;
	prev_y = y;
}

void motion(int x, int y)
{
	int dx = x - prev_x;
	int dy = y - prev_y;
	prev_x = x;
	prev_y = y;

	if((!dx && !dy) || cur_sel == -1) return;

	if(bnstate[0]) {
		pos[cur_sel].x += dx;
		pos[cur_sel].y -= dy;
		glutPostRedisplay();
	}
	if(bnstate[2]) {
		zoom[cur_sel] -= dy * 0.1;
		if(zoom[cur_sel] < 0.01) {
			zoom[cur_sel] = 0.01;
		}
		glutPostRedisplay();
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
