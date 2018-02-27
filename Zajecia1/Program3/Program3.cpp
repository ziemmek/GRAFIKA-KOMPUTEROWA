//Program3
//Ziemowit Juroszek

#include "stdafx.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

GLfloat x1 = -11.25f; GLfloat y1 = -48.0f;
GLfloat x2 = -31.25f; GLfloat y2 = -38.0f;
GLfloat x3 = -44.5f; GLfloat y3 = -21.5f;
GLfloat x4 = -49.5f; GLfloat y4 = 0.0f;
GLfloat x5 = x3; GLfloat y5 = -y3;
GLfloat x6 = x2; GLfloat y6 = -y2;
GLfloat x7 = x1; GLfloat y7 = -y1;
GLfloat x8 = -x1; GLfloat y8 = -y1;
GLfloat x9 = -x2; GLfloat y9 = -y2;
GLfloat x10 = -x3; GLfloat y10 = -y3;
GLfloat x11 = -x4; GLfloat y11 = y4;
GLfloat x12 = -x3; GLfloat y12 = y3;
GLfloat x13 = -x2; GLfloat y13 = y2;
GLfloat x14 = -x1; GLfloat y14 = y1;

GLfloat windowwleft;
GLfloat windowwright;
GLfloat windowhdown;
GLfloat windowhup;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.75f, 0.75f, 0.75f);

	glBegin(GL_POLYGON);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glVertex2f(x5, y5);
	glVertex2f(x6, y6);
	glVertex2f(x7, y7);
	glVertex2f(x8, y8);
	glVertex2f(x9, y9);
	glVertex2f(x10, y10);
	glVertex2f(x11, y11);
	glVertex2f(x12, y12);
	glVertex2f(x13, y13);
	glVertex2f(x14, y14);

	glEnd();

	glFlush();
}

void TimerFunction(int value)
{
	if (x11 > windowwright || x4 < windowwleft)
		xstep = -xstep;

	if (y8 > windowhup || y1 < windowhdown)
		ystep = -ystep;

	x1 += xstep; y1 += ystep;
	x2 += xstep; y2 += ystep;
	x3 += xstep; y3 += ystep;
	x4 += xstep; y4 += ystep;
	x5 += xstep; y5 += ystep;
	x6 += xstep; y6 += ystep;
	x7 += xstep; y7 += ystep;
	x8 += xstep; y8 += ystep;
	x9 += xstep; y9 += ystep;
	x10 += xstep; y10 += ystep;
	x11 += xstep; y11 += ystep;
	x12 += xstep; y12 += ystep;
	x13 += xstep; y13 += ystep;
	x14 += xstep; y14 += ystep;

	glutPostRedisplay();
	glutTimerFunc(16, TimerFunction, 1);
}

void SetupRC(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(int w, int h)
{
	GLfloat aspectRatio;

	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)
	{
		glOrtho(-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio, 1.0, -1.0);
		windowwleft = -100.0;
		windowwright = 100.0;
		windowhdown = -100.0 / aspectRatio;
		windowhup = 100.0 / aspectRatio;
	}
	else
	{
		glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, 1.0, -1.0);
		windowwleft = -100.0 * aspectRatio;
		windowwright = 100.0 * aspectRatio;
		windowhdown = -100.0;
		windowhup = 100.0;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Czternastokat Bounce");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(16, TimerFunction, 1);
	SetupRC();
	glutMainLoop();
	return 0;
}
