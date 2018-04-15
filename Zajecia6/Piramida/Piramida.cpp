// Piramida.cpp : Defines the entry point for the console application.
// Ziemowit Juroszek
// Torus


#include "stdafx.h"
#include <GL/glut.h>
#include <stdlib.h>

// stałe do obsługi menu podręcznego

enum
{
	FULL_WINDOW,       // aspekt obrazu - całe okno
	ASPECT_1_1,        // aspekt obrazu 1:1
	EXIT               // wyjście
};

// aspekt obrazu

int aspect = FULL_WINDOW;

// rozmiary bryły obcinania

const GLdouble left = -10.0;
const GLdouble right = 10.0;
const GLdouble bottom = -10.0;
const GLdouble top = 10.0;
const GLdouble near_ = 50.0;
const GLdouble far_ = 70.0;

// współczynnik skalowania

GLfloat scale = 1.0;

// kąty obrotu

GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;

// przesunięcie

GLfloat translatex = 0.0;
GLfloat translatey = 0.0;

// wskaznik naciśnięcia lewego przycisku myszki

int button_state = GLUT_UP;

// polozenie kursora myszki

int button_x, button_y;

//parametry torusa

GLdouble innerRadius = 0.4;
GLdouble outerRadius = 0.6;
GLint nsides = 50;
GLint rings = 50;

void Torus()
{
	glutWireTorus(0.1, 0.3, 10, 10);
}

// funkcja rysująca piramidę z torusow

void Pyramid()

{
	//poziom 14
	for (double i = -6.5; i <= 6.5; i++)
	{
		for (double j = -6.5; j <= 6.5; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 0.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom 13
	for (double i = -6; i <= 6; i++)
	{
		for (double j = -6; j <= 6; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 1.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom 12
	for (double i = -5.5; i <= 5.5; i++)
	{
		for (double j = -5.5; j <= 5.5; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 2.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom 11
	for (double i = -5; i <= 5; i++)
	{
		for (double j = -5; j <= 5; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 3.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom 10
	for (double i = -4.5; i <= 4.5; i++)
	{
		for (double j = -4.5; j <= 4.5; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 4.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom 9
	for (double i = -4; i <= 4; i++)
	{
		for (double j = -4; j <= 4; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 5.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom 8
	for (double i = -3.5; i <= 3.5; i++)
	{
		for (double j = -3.5; j <= 3.5; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 6.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom 7
	for (double i = -3; i <= 3; i++)
	{
		for (double j = -3; j <= 3; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 7.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom 6
	for (double i = -2.5; i <= 2.5; i++)
	{
		for (double j = -2.5; j <= 2.5; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 8.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom 5

	for (double i = -2; i <= 2; i++)
	{
		for (double j = -2; j <= 2; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 9.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom 4


	for (double i = -1.5; i <= 1.5; i++)
	{
		for (double j = -1.5; j <= 1.5; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 10.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom 3


	for (double i = -1; i <= 1; i++)
	{
		for (double j = -1; j <= 1; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 11.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom2

	for (double i = -0.5; i <= 0.5; i++)
	{
		for (double j = -0.5; j <= 0.5; j++)
		{
			glPushMatrix();

			glTranslatef(i, j, 12.0);

			Torus();

			glPopMatrix();
		}
	}
	//poziom 1

	glPushMatrix();

	glTranslatef(0, 0, 13.0);

	Torus();

	glPopMatrix();
}

// funkcja generująca scenę 3D

void Display()
{
	// kolor tła - zawartość bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	// przesunięcie układu współrzędnych obiektu do środka bryły odcinania
	glTranslatef(0, 0, -(near_ + far_) / 2);

	// przesunięcie obiektu - ruch myszką
	glTranslatef(translatex, translatey, 0.0);

	// skalowanie obiektu - klawisze "+" i "-"
	glScalef(scale, scale, scale);

	// obroty obiektu - klawisze kursora
	glRotatef(rotatex, 1.0, 0, 0);
	glRotatef(rotatey, 0, 1.0, 0);

	// kolor krawędzi obiektu
	glColor3f(0.0, 0.0, 0.0);

	// rysowanie piramidy
	Pyramid();

	// skierowanie poleceń do wykonania
	glFlush();

	// zamiana buforów koloru
	glutSwapBuffers();
}

// zmiana wielkości okna

void Reshape(int width, int height)
{
	// obszar renderingu - całe okno
	glViewport(0, 0, width, height);

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// parametry bryły obcinania
	if (aspect == ASPECT_1_1)
	{
		// wysokość okna większa od wysokości okna
		if (width < height && width > 0)
			glFrustum(left, right, bottom*height / width, top*height / width, near_, far_);
		else

			// szerokość okna większa lub równa wysokości okna
			if (width >= height && height > 0)
				glFrustum(left*width / height, right*width / height, bottom, top, near_, far_);
	}
	else
		glFrustum(left, right, bottom, top, near_, far_);

	// generowanie sceny 3D
	Display();
}

// obsługa klawiatury

void Keyboard(unsigned char key, int x, int y)
{
	// klawisz +
	if (key == '+')
		scale += 0.1;
	else

		// klawisz -
		if (key == '-' && scale > 0.1)
			scale -= 0.1;

	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obsługa klawiszy funkcyjnych i klawiszy kursora

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		// kursor w lewo
	case GLUT_KEY_LEFT:
		rotatey -= 1;
		break;

		// kursor w górę
	case GLUT_KEY_UP:
		rotatex -= 1;
		break;

		// kursor w prawo
	case GLUT_KEY_RIGHT:
		rotatey += 1;
		break;

		// kursor w dół
	case GLUT_KEY_DOWN:
		rotatex += 1;
		break;
	}

	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obsługa przycisków myszki

void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		// zapamiętanie stanu lewego przycisku myszki
		button_state = state;

		// zapamiętanie poło¿enia kursora myszki
		if (state == GLUT_DOWN)
		{
			button_x = x;
			button_y = y;
		}
	}
}

// obsługa ruchu kursora myszki

void MouseMotion(int x, int y)
{
	if (button_state == GLUT_DOWN)
	{
		translatex += 1.1 *(right - left) / glutGet(GLUT_WINDOW_WIDTH) * (x - button_x);
		button_x = x;
		translatey += 1.1 *(top - bottom) / glutGet(GLUT_WINDOW_HEIGHT) * (button_y - y);
		button_y = y;
		glutPostRedisplay();
	}
}

// obsługa menu podręcznego

void Menu(int value)
{
	switch (value)
	{
		// obszar renderingu - całe okno
	case FULL_WINDOW:
		aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// obszar renderingu - aspekt 1:1
	case ASPECT_1_1:
		aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// wyjście
	case EXIT:
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);

	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// rozmiary głównego okna programu
	glutInitWindowSize(800, 800);

	// utworzenie głównego okna programu
	glutCreateWindow("Torus Pyramid");

	// dołączenie funkcji generującej scenę 3D
	glutDisplayFunc(Display);

	// dołączenie funkcji wywoływanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// dołączenie funkcji obsługi klawiatury
	glutKeyboardFunc(Keyboard);

	// dołączenie funkcji obsługi klawiszy funkcyjnych i klawiszy kursora
	glutSpecialFunc(SpecialKeys);

	// obsługa przycisków myszki
	glutMouseFunc(MouseButton);

	// obsługa ruchu kursora myszki
	glutMotionFunc(MouseMotion);

	// utworzenie podmenu - aspekt obrazu
	int MenuAspect = glutCreateMenu(Menu);
#ifdef WIN32

	glutAddMenuEntry("Aspekt obrazu - całe okno", FULL_WINDOW);
#else

	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
#endif

	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);

	// menu główne
	glutCreateMenu(Menu);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);
#ifdef WIN32

	glutAddMenuEntry("Wyjście", EXIT);
#else

	glutAddMenuEntry("Wyjscie", EXIT);
#endif

	// określenie przycisku myszki obsługującego menu podręczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// wprowadzenie programu do obsługi pętli komunikatów
	glutMainLoop();
	return 0;
}
