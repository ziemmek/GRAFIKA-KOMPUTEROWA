// opengl_tekstura_2d.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
/*
(c) Janusz Ganczarski
http://www.januszg.hg.pl
JanuszG@enter.net.pl
*/
#include <GL/glut.h>
#include "glext.h"
#ifndef WIN32
#define GLX_GLXEXT_LEGACY
#include <GL/glx.h>
#define wglGetProcAddress glXGetProcAddressARB
#endif
#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "colors.h"
#include "targa.h"
#include <GLFW/glfw3.h>
// wskaŸnik na funkcję glWindowPos2i
PFNGLWINDOWPOS2IPROC glWindowPos2i = NULL;
// stałe do obsługi menu podręcznego
// rozmiar nagłówka pliku
#define TARGA_HEADER_SIZE 0x12

// nieskompresowany obraz RGB(A)
#define TARGA_UNCOMP_RGB_IMG 0x02

// nieskompresowany obraz w odcieniach szarości
#define TARGA_UNCOMP_BW_IMG 0x03
enum
{
	Jupiter_TEX, // Jowiesz
	FULL_WINDOW, // aspekt obrazu - całe okno
	ASPECT_1_1, // aspekt obrazu 1:1
	EXIT // wyjście
};
// aspekt obrazu
int aspect = FULL_WINDOW;
// usunięcie definicji makr near i far
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif
// rozmiary bryły obcinania
const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble near = 3.0;
const GLdouble far = 7.0;
// kąty obrotu
GLfloat rotatex = 270.0;
GLfloat rotatez = 0.0;
// wskaŸnik naciśnięcia lewego przycisku myszki
int button_state = GLUT_UP;
// położenie kursora myszki
int button_x, button_y;
// współczynnik skalowania
GLfloat scale = 1.5;
// identyfikatory tekstur
GLuint Jupiter;
// identyfikator bieżącej tekstury
GLuint texture;
// filtr pomniejszający
GLint min_filter = GL_LINEAR_MIPMAP_LINEAR;
// funkcja rysująca napis w wybranym miejscu
// (wersja korzystająca z funkcji glWindowPos2i)
GLboolean load_targa(const char *filename, GLsizei &width, GLsizei &height,
	GLenum &format, GLenum &type, GLvoid *&pixels)
{
	// pocz¹tkowe wartości danych wyjściowych
	pixels = NULL;
	width = 0;
	height = 0;

	// otwarcie pliku do odczytu
	FILE *tga = fopen(filename, "rb");

	// sprawdzenie poprawności otwarcia pliku
	if (!tga)
		return GL_FALSE;

	// tablica na nagłówek pliku TGA
	unsigned char header[TARGA_HEADER_SIZE];

	// odczyt nagłówka pliku
	fread(header, TARGA_HEADER_SIZE, 1, tga);

	// ominięcie pola ImageID
	fseek(tga, header[0], SEEK_CUR);

	// szerokość obrazu
	width = header[12] + (header[13] << 8);

	// wysokość obrazu
	height = header[14] + (header[15] << 8);

	// obraz w formacie BGR - 24 bity na piksel
	if (header[2] == TARGA_UNCOMP_RGB_IMG && header[16] == 24)
	{
		pixels = new unsigned char[width * height * 3];
		fread((void*)pixels, width * height * 3, 1, tga);
		format = GL_BGR;
		type = GL_UNSIGNED_BYTE;
	}
	else

		// obraz w formacie BGRA - 32 bity na piksel
		if (header[2] == TARGA_UNCOMP_RGB_IMG && header[16] == 32)
		{
			pixels = new unsigned char[width * height * 4];
			fread((void*)pixels, width * height * 4, 1, tga);
			format = GL_BGRA;
			type = GL_UNSIGNED_BYTE;
		}
		else

			// obraz w odcieniach szarości - 8 bitów na piksel
			if (header[2] == TARGA_UNCOMP_BW_IMG && header[16] == 8)
			{
				pixels = new unsigned char[width * height];
				fread((void*)pixels, width * height, 1, tga);
				format = GL_LUMINANCE;
				type = GL_UNSIGNED_BYTE;
			}
			else
				return GL_FALSE;

	// zamknięcie pliku
	fclose(tga);

	// sukces
	return GL_TRUE;
}
void DrawString(GLint x, GLint y, char * string)
{
	// położenie napisu
	glWindowPos2i(x, y);
	// wyświetlenie napisu
	int len = strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
}
GLboolean save_targa(const char *filename, GLsizei width, GLsizei height,
	GLenum format, GLenum type, GLvoid *pixels)

{
	// sprawdzenie formatu danych obrazu
	if (format != GL_BGR && format != GL_BGRA && format != GL_LUMINANCE)
		return GL_FALSE;

	// sprawdzenie formatu pikseli obrazu
	if (type != GL_UNSIGNED_BYTE)
		return GL_FALSE;

	// otwarcie pliku do zapisu
	FILE *tga = fopen(filename, "wb");

	// sprawdzenie poprawności otwarcia pliku
	if (tga == NULL)
		return GL_FALSE;

	// nagłówek pliku TGA
	unsigned char header[TARGA_HEADER_SIZE];

	// wyzerowanie pól nagłówka
	memset(header, 0, TARGA_HEADER_SIZE);

	// pole Image Type
	if (format == GL_BGR || format == GL_BGRA)
		header[2] = TARGA_UNCOMP_RGB_IMG;
	else
		if (format == GL_LUMINANCE)
			header[2] = TARGA_UNCOMP_BW_IMG;

	// pole Width
	header[12] = (unsigned char)width;
	header[13] = (unsigned char)(width >> 8);

	// pole Height
	header[14] = (unsigned char)height;
	header[15] = (unsigned char)(height >> 8);

	// pole Pixel Depth
	if (format == GL_BGRA)
		header[16] = 32;
	else
		if (format == GL_BGR)
			header[16] = 24;
		else
			if (format == GL_LUMINANCE)
				header[16] = 8;

	// zapis nagłówka pliku TARGA
	fwrite(header, TARGA_HEADER_SIZE, 1, tga);

	// zapis danych obrazu
	if (format == GL_BGRA)
		fwrite(pixels, width*height * 4, 1, tga);
	else
		if (format == GL_BGR)
			fwrite(pixels, width*height * 3, 1, tga);
		else
			if (format == GL_LUMINANCE)
				fwrite(pixels, width*height, 1, tga);

	// zamknięcie pliku
	fclose(tga);

	// sukces
	return GL_TRUE;
}

// funkcja generująca scenę 3D
void DisplayScene()
{
	// kolor tła - zawartość bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);
	// czyszczenie bufora koloru i bufora głębokości
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);
	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();
	// przesunięcie układu współrzędnych obiektów do środka bryły odcinania
	glTranslatef(0.0, 0.0, -(near + far) / 2);
	// obroty obiektu
	glRotatef(rotatex, 1.0, 0.0, 0.0);
	glRotatef(rotatez, 0.0, 0.0, 1.0);
	// skalowanie obiektu - klawisze "+" i "-"
	glScalef(scale, scale, scale);
	// włączenie testu bufora głębokości
	glEnable(GL_DEPTH_TEST);
	// włączenie teksturowania dwuwymiarowego
	glEnable(GL_TEXTURE_2D);
	// filtr powiększający
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// filtr pomniejszający
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	// dowiązanie wybranej tekstury
	glBindTexture(GL_TEXTURE_2D, texture);
	// ustawienie parametów środowiska tekstur
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	// utworzenie kwadryki
	GLUquadricObj *quadobj = gluNewQuadric();
	// styl (wygląd) generowanej kwadryki
	gluQuadricDrawStyle(quadobj, GLU_FILL);
	// sposób generacji wektorów normalnych
	gluQuadricNormals(quadobj, GLU_SMOOTH);
	// nałożenie tekstury na kwadrykę
	gluQuadricTexture(quadobj, GL_TRUE);
	// narysowanie kuli
	gluSphere(quadobj, 1.0, 30, 30);
	// usunięcie kwadryki
	gluDeleteQuadric(quadobj);
	// wyłączenie teksturowania dwuwymiarowego
	glDisable(GL_TEXTURE_2D);
	// informacje o wybranych parametrach bieżącej tekstury
	char string[200];
	GLfloat var;
	glColor3fv(Black);
	// wartość priorytetu tekstury
	glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, &var);
	sprintf_s(string, "GL_TEXTURE_PRIORITY = %f", var);
	DrawString(2, 2, string);
	// czy tekstura jest rezydentna
	glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_RESIDENT, &var);
	if (var == GL_FALSE)
		strcpy_s(string, "GL_TEXTURE_RESIDENT = GL_FALSE");
	else
		strcpy_s(string, "GL_TEXTURE_RESIDENT = GL_TRUE");
	DrawString(2, 16, string);
	// szerokość tekstury (poziom 0)
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &var);
	sprintf_s(string, "GL_TEXTURE_WIDTH = %f", var);
	DrawString(2, 30, string);
	// wysokość tekstury (poziom 0)
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &var);
	sprintf_s(string, "GL_TEXTURE_HEIGHT = %f", var);
	DrawString(2, 46, string);
	// skierowanie poleceñ do wykonania
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
			glFrustum(left, right, bottom* height / width, top* height / width, near, far);
		else
			// szerokość okna większa lub równa wysokości okna
			if (width >= height && height > 0)
				glFrustum(left* width / height, right* width / height, bottom, top, near,
					far);
	}
	else
		glFrustum(left, right, bottom, top, near, far);
	// generowanie sceny 3D
	DisplayScene();
}
// obsługa klawiatury
void Keyboard(unsigned char key, int x, int y)
{
	// klawisz +
	if (key == '+')
		scale += 0.05;
	else
		// klawisz -
		if (key == '-' && scale > 0.05)
			scale -= 0.05;
	// narysowanie sceny
	DisplayScene();
}
// obsługa przycisków myszki
void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		// zapamiętanie stanu lewego przycisku myszki
		button_state = state;
		// zapamiętanie położenia kursora myszki
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
		rotatez += 30 * (right - left) / glutGet(GLUT_WINDOW_WIDTH) * (x - button_x);
		button_x = x;
		rotatex -= 30 * (top - bottom) / glutGet(GLUT_WINDOW_HEIGHT) * (button_y - y);
		button_y = y;
		glutPostRedisplay();
	}
}
// obsługa menu podręcznego
void Menu(int value)
{
	switch (value)
	{
		// Wenus
	case Jupiter_TEX:
		texture = Jupiter;
		DisplayScene();
		break;
		// Ziemia
		// filtr pomniejszający
	case GL_NEAREST_MIPMAP_NEAREST:
	case GL_NEAREST_MIPMAP_LINEAR:
	case GL_LINEAR_MIPMAP_NEAREST:
	case GL_LINEAR_MIPMAP_LINEAR:
		min_filter = value;
		DisplayScene();
		break;
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
// utworzenie tekstur
void GenerateTextures()
{
	// zmienne użyte przy obsłudze plików TARGA
	GLsizei width, height;
	GLenum format, type;
	GLvoid *pixels;
	// tryb upakowania bajtów danych tekstury
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// wczytanie tekstury Venus utworzonej ze zdjęć sondy Magellan
	// plik: http://maps.jpl.nasa.gov/pix/ven0aaa2.tif
	GLboolean error = load_targa("Jowisz.tga", width, height, format, type, pixels);
	// błąd odczytu pliku
	if (error == GL_FALSE)
	{
		printf("Niepoprawny odczyt pliku Jowisz.tga");
		exit(0);
	}
	// utworzenie identyfikatora tekstury
	glGenTextures(1, &Jupiter);
	// dowiązanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, Jupiter);
	// utworzenie tekstury wraz z mipmapami
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, format, type, pixels);
	// porządki
	delete[](unsigned char *)pixels;
	// wczytanie tekstury z hipsometryczną (fizyczną) mapą Ziemi
	// plik: http://maps.jpl.nasa.gov/pix/ear0xuu2.tif
	/*error = load_targa("ear0xuu2.tga", width, height, format, type, pixels);
	// błąd odczytu pliku
	if (error == GL_FALSE)
	{
	printf("Niepoprawny odczyt pliku ear0xuu2.tga");
	exit(0);
	}
	// utworzenie identyfikatora tekstury
	glGenTextures(1, &EARTH);
	// dowiązanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, EARTH);
	// utworzenie tekstury wraz z mipmapami
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, format, type, pixels);
	// porządki
	delete[](unsigned char *)pixels;
	// wczytanie tekstury Marsa utworzonej ze zdjęć sond Viking
	// plik: http://maps.jpl.nasa.gov/pix/mar0kuu2.tif
	error = load_targa("mar0kuu2.tga", width, height, format, type, pixels);
	// błąd odczytu pliku
	if (error == GL_FALSE)
	{
	printf("Niepoprawny odczyt pliku mar0kuu2.tga");
	exit(0);
	}
	// utworzenie identyfikatora tekstury
	glGenTextures(1, &MARS);
	// dowiązanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, MARS);
	// utworzenie tekstury wraz z mipmapami
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, format, type, pixels);
	// porządki
	delete[](unsigned char *)pixels;
	// identyfikator bieżącej tekstury*/
	texture = Jupiter;
}
// sprawdzenie i przygotowanie obsługi wybranych rozszerzeñ
void ExtensionSetup()
{
	// pobranie numeru wersji biblioteki OpenGL
	const char *version = (char *)glGetString(GL_VERSION);
	// odczyt wersji OpenGL
	int major = 0, minor = 0;
	if (sscanf_s(version, "%d.%d", &major, &minor) != 2)
	{
#ifdef WIN32
		printf("Błędny format wersji OpenGL\n");
#else
		printf("Bledny format wersji OpenGL\n");
#endif
		exit(0);
	}
	// sprawdzenie czy jest co najmniej wersja 1.4
	if (major > 1 || minor >= 4)
	{
		// pobranie wskaŸnika wybranej funkcji OpenGL 1.4
		glWindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress("glWindowPos2i");
	}
	else
		// sprawdzenie czy jest obsługiwane rozszerzenie ARB_window_pos
		if (glutExtensionSupported("GL_ARB_window_pos"))
		{
			// pobranie wskaŸnika wybranej funkcji rozszerzenia ARB_window_pos
			glWindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress
			("glWindowPos2iARB");
		}
		else
		{
			printf("Brak rozszerzenia ARB_window_pos!\n");
			exit(0);
		}
}
int main(int argc, char * argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);
	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// rozmiary głównego okna programu
	glutInitWindowSize(500, 500);
	// utworzenie głównego okna programu
	glutCreateWindow("Jowisz");
	// dołączenie funkcji generującej scenę 3D
	glutDisplayFunc(DisplayScene);
	// dołączenie funkcji wywoływanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);
	// dołączenie funkcji obsługi klawiatury
	glutKeyboardFunc(Keyboard);
	// obsługa przycisków myszki
	glutMouseFunc(MouseButton);
	// obsługa ruchu kursora myszki
	glutMotionFunc(MouseMotion);
	// utworzenie podmenu - Planeta
	int MenuPlanet = glutCreateMenu(Menu);
	glutAddMenuEntry("Jowisz", Jupiter_TEX);
	// utworzenie podmenu - Filtr pomniejszający
	int MenuMinFilter = glutCreateMenu(Menu);
	glutAddMenuEntry("GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST);
	glutAddMenuEntry("GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR);
	glutAddMenuEntry("GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST);
	glutAddMenuEntry("GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR);
	// utworzenie podmenu - Aspekt obrazu
	int MenuAspect = glutCreateMenu(Menu);
#ifdef WIN32
	glutAddMenuEntry("Aspekt obrazu - całe okno", FULL_WINDOW);
#else
	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
#endif
	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);
	// menu główne
	glutCreateMenu(Menu);
	glutAddSubMenu("Planeta", MenuPlanet);
#ifdef WIN32
	glutAddSubMenu("Filtr pomniejszający", MenuMinFilter);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);
	glutAddMenuEntry("Wyjście", EXIT);
#else
	glutAddSubMenu("Filtr pomniejszajacy", MenuMinFilter);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);
	glutAddMenuEntry("Wyjscie", EXIT);
#endif
	// określenie przycisku myszki obsługującego menu podręczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	// utworzenie tekstur
	GenerateTextures();
	// sprawdzenie i przygotowanie obsługi wybranych rozszerzeñ
	ExtensionSetup();
	// wprowadzenie programu do obsługi pętli komunikatów
	glutMainLoop();
	return 0;
}