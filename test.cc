#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <ubertk.h>
#include "test_text.h"

#define TEXT_PT_SIZE	18

void redraw(void);
void idle_handler(void);
void draw_grid(float spacing, float sz);
void reshape(int x, int y);
void keyb(unsigned char key, int x, int y);
void keyb_up(unsigned char key, int x, int y);
void mouse_button(int bn, int state, int x, int y);
void mouse_motion(int x, int y);


void utk_color(int r, int g, int b, int a);
void utk_clip(int x1, int y1, int x2, int y2);
void utk_image(int x, int y, void *pix, int xsz, int ysz);

void utk_rect(int x1, int y1, int x2, int y2);
void utk_line(int x1, int y1, int x2, int y2, int width);
void utk_text(int x, int y, const char *txt, int sz);
int utk_text_spacing();
int utk_text_width(const char *txt, int sz);

unsigned int get_msec(void);

int xsz, ysz;

utk::Container *utkroot;
utk::Window *win;
utk::Slider *r, *g, *b;
utk::Label *melaxrines;
utk::CheckBox *ninja;
utk::Container *vbox;
utk::TrackBall *tball;
utk::ColorBox *cbox;
float max_descent;


void foo_handler(utk::Event *event)
{
	printf("a button was just pressed!\n");
}

void exit_bn_handler(utk::Event *event)
{
	exit(0);
}

int main(int argc, char **argv)
{
	float lpos[] = {-60, 50, 100, 1};

	glutInitWindowSize(800, 600);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("ubertk test");
	xsz = glutGet(GLUT_WINDOW_WIDTH);
	ysz = glutGet(GLUT_WINDOW_HEIGHT);

	glutDisplayFunc(redraw);
	glutIdleFunc(idle_handler);
	glutKeyboardFunc(keyb);
	glutKeyboardUpFunc(keyb_up);
	glutMouseFunc(mouse_button);
	glutMotionFunc(mouse_motion);
	glutPassiveMotionFunc(mouse_motion);
	glutReshapeFunc(reshape);

	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0);
	/*glEnable(GL_SCISSOR_TEST);*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0, (float)xsz / (float)ysz, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	if(!CreateFont("georgia.ttf", TEXT_PT_SIZE)) {
		fprintf(stderr, "failed to load font\n");
		return EXIT_FAILURE;
	}
	max_descent = GetMaxDescent();

	utk::gfx::color = utk_color;
	utk::gfx::clip = utk_clip;
	utk::gfx::image = utk_image;
	utk::gfx::rect = utk_rect;
	utk::gfx::line = utk_line;
	utk::gfx::text = utk_text;
	utk::gfx::text_spacing = utk_text_spacing;
	utk::gfx::text_width = utk_text_width;

	melaxrines = new utk::Label("kai m'aresoun oi melaxrines ...");

	// create the windows
	utkroot = utk::init(xsz, ysz);

	win = new utk::Window;
	win->set_pos(100, 100);
	win->set_size(300, 500);
	win->set_alpha(220);
	win->set_text("test window g");
	win->set_visible(true);
	utkroot->add_child(win);

	vbox = new utk::VBox;
	win->add_child(vbox);

	vbox->add_child(new utk::Label("a label"));

	//r = new utk::ScrollBar();
	//g = new utk::ScrollBar();
	r = new utk::Slider(0, 1);
	g = new utk::Slider(0, 1);
	b = new utk::Slider(0, 1);
	vbox->add_child(r);
	vbox->add_child(g);
	vbox->add_child(b);

	vbox->add_child(new utk::RadioBox("check me"));
	vbox->add_child(new utk::RadioBox("check me out"));
	vbox->add_child(new utk::RadioBox("check me out of the blue"));

	vbox->add_child(new utk::Entry);

	vbox->add_child(new utk::Button("Exit", exit_bn_handler));

	ninja = new utk::CheckBox("Eimai ninja!");
	vbox->add_child(ninja);
	
	cbox = new utk::ColorBox();
	vbox->add_child(cbox);

	tball = new utk::TrackBall(50, 50);
	vbox->add_child(tball);

	glutMainLoop();

	return 0;
}

void redraw(void)
{
	float h = b->get_value();
	cbox->set_h(h);

	float s, v;

	s = cbox->get_s();
	v = cbox->get_v();

	float r, g, b;
	utk::hsv_to_rgb(&r, &g, &b, h, s, v);

	float t = (float)get_msec() / 1000.0;
	//glClearColor(r->get_percent() / 100.0f, g->get_percent() / 100.0f, b->get_percent() / 100.0f, 1);
	glClearColor(r, g, b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -6);
	glRotatef(30.0, 1, 0, 0);
	//glRotatef(t * 10.0, 0, 1, 0);
	glRotatef(360 * tball->get_phi() / 6.28f, 1, 0, 0);
	glRotatef(360 * tball->get_theta() / 6.28f, 0, 1, 0);

	draw_grid(2.0, 100.0);
	glTranslatef(0, 0.8, 0);
	glutWireTeapot(1.0);
	
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	utk::draw(utkroot);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	glutSwapBuffers();
}

void draw_grid(float spacing, float sz)
{
	float hsz = sz * 0.5;
	int lines = (int)(hsz / spacing) + 1;

	glLineWidth(1.0);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(0.4, 0.4, 0.4);
	for(int i=1; i<lines; i++) {
		glVertex3f(i * spacing, 0, -hsz);
		glVertex3f(i * spacing, 0, hsz);
		glVertex3f(-i * spacing, 0, -hsz);
		glVertex3f(-i * spacing, 0, hsz);
		glVertex3f(-hsz, 0, i * spacing);
		glVertex3f(hsz, 0, i * spacing);
		glVertex3f(-hsz, 0, -i * spacing);
		glVertex3f(hsz, 0, -i * spacing);
	}
	glEnd();

	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(-hsz, 0, 0);
	glVertex3f(hsz, 0, 0);
	glVertex3f(0, 0, -hsz);
	glVertex3f(0, 0, hsz);
	glEnd();
	glLineWidth(1.0);
	glEnable(GL_LIGHTING);
}

void idle_handler(void)
{
	glutPostRedisplay();
}

void reshape(int x, int y)
{
	xsz = x;
	ysz = y;
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)x / (float)y, 1.0, 1000.0);
}

void keyb(unsigned char key, int x, int y)
{
	switch(key) {
	case 27:
		exit(0);

	default:
		break;
	}

	utk::KeyboardEvent e;
	e.key = key;
	e.pressed = true;

	event(&e);
}

void keyb_up(unsigned char key, int x, int y)
{
	utk::KeyboardEvent e;
	e.key = key;
	e.pressed = false;

	event(&e);
}

void mouse_button(int bn, int state, int x, int y)
{
	utk::MButtonEvent e;
	e.button = bn;
	e.pressed = (state == GLUT_DOWN);
	e.x = x;
	e.y = y;

	event(&e);
}

void mouse_motion(int x, int y)
{
	utk::MMotionEvent e;
	e.x = x;
	e.y = y;

	event(&e);
}


#define CONVX(x)	(2.0 * (float)(x) / (float)xsz - 1.0)
#define CONVY(y)	(1.0 - 2.0 * (float)(y) / (float)ysz)

void utk_color(int r, int g, int b, int a)
{
	glColor4ub(r, g, b, a);
	SetTextColor(Color(r / 255.0, g / 255.0, b / 255.0, a / 255.0));
}

void utk_clip(int x1, int y1, int x2, int y2)
{
	if(x1 == x2 && y1 == y2 && x1 == y1 && x1 == 0) {
		glDisable(GL_SCISSOR_TEST);
	} else {
		glEnable(GL_SCISSOR_TEST);
	}
	glScissor(x1, ysz - y2, x2 - x1, y2 - y1);
}

void utk_image(int x, int y, void *pix, int w, int h)
{

	glPixelZoom(1, -1);
	glRasterPos2f(CONVX(x), CONVY(y));
	glDrawPixels(w, h, GL_BGRA, GL_UNSIGNED_BYTE, pix);
}

void utk_rect(int x1, int y1, int x2, int y2)
{
	glRectf(CONVX(x1), CONVY(y1), CONVX(x2), CONVY(y2));
}

void utk_line(int x1, int y1, int x2, int y2, int width)
{
	glLineWidth((float)width);
	glBegin(GL_LINES);
	glVertex2f(CONVX(x1), CONVY(y1));
	glVertex2f(CONVX(x2), CONVY(y2));
	glEnd();
}

void utk_text(int x, int y, const char *txt, int sz)
{
	float fx = (float)x / (float)xsz;
	float fy = (float)y / (float)ysz;

	SetTextPos(Vector2(fx, fy - max_descent));
	//SetTextPos(Vector2(0.25, 0.25));
	SetTextSize((float)sz / (float)TEXT_PT_SIZE);
	PrintString(txt);
}

int utk_text_spacing()
{
	return (int)(GetLineAdvance() * (float)ysz);
}

int utk_text_width(const char *txt, int sz)
{
	float prev_sz = GetTextSize();
	SetTextSize((float)sz / (float)TEXT_PT_SIZE);

	int width = (int)(GetTextWidth(txt) * (float)xsz);

	SetTextSize(prev_sz);
	return width;
}

#if defined(unix) || defined(__unix__)
#include <time.h>
#include <sys/time.h>
#else
#include <windows.h>
#endif

unsigned int get_msec(void)
{
#if defined(unix) || defined(__unix__)
	static struct timeval tv, first_tv;

	gettimeofday(&tv, 0);

	if(first_tv.tv_sec == 0) {
		first_tv = tv;
		return 0;
	}
	return (tv.tv_sec - first_tv.tv_sec) * 1000 + (tv.tv_usec - first_tv.tv_usec) / 1000;
#else
	return GetTickCount();
#endif
}
