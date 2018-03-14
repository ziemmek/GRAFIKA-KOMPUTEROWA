

#include "stdafx.h"
#include <GL/glut.h>
#include "glext.h"
#ifndef WIN32
#define GLX_GLXEXT_LEGACY
#include <GL/glx.h>
#define wglGetProcAddress glXGetProcAddressARB
#endif
#include <stdlib.h>
#include <stdio.h>
#include "colors.h"
#include "targa.h"

// wska�nik na funkcj� glWindowPos2i

PFNGLWINDOWPOS2IPROC glWindowPos2i = NULL;

// sta�e do obs�ugi menu podr�cznego

enum
{
	TEXTURE_COMPRESSION_FASTEST,     // kompresja tekstur - GL_FASTEST
	TEXTURE_COMPRESSION_DONT_CARE,   // kompresja tekstur - GL_DONT_CARE
	TEXTURE_COMPRESSION_NICEST,      // kompresja tekstur - GL_NICEST
	TEXTURE_LENA,                    // tekstura lena
	TEXTURE_LENA_UNC,                // teksura lena nieskompresowana
	TEXTURE_LENA_GRAY,               // tekstura lena_gray
	TEXTURE_LENA_GRAY_UNC,           // tekstura lena_gray nieskompresowana
	FULL_WINDOW,                     // aspekt obrazu - ca�e okno
	ASPECT_1_1,                      // aspekt obrazu 1:1
	EXIT                             // wyj�cie
};

// aspekt obrazu

int aspect = FULL_WINDOW;

// usuni�cie definicji makr near i far

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

// rozmiary bry�y obcinania

const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble near = 3.0;
const GLdouble far = 7.0;

// identyfikatory tekstur

GLuint LENA, LENA_UNC, LENA_GRAY, LENA_GRAY_UNC;

// identyfikator bie��cej tekstury;

GLuint texture;

// wskaz�wki do kompresji tekstur

GLint texture_compression_hint = GL_DONT_CARE;

// funkcja rysuj�ca napis w wybranym miejscu
// (wersja korzystaj�ca z funkcji glWindowPos2i)

void DrawString(GLint x, GLint y, char *string)
{
	// po�o�enie napisu
	glWindowPos2i(x, y);

	// wy�wietlenie napisu
	int len = strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
}

// funkcja generuj�ca scen� 3D

void DisplayScene()
{
	// kolor t�a - zawarto�� bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT);

	// wyb�r macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	// przesuni�cie uk�adu wsp�rz�dnych obiekt�w do �rodka bry�y odcinania
	glTranslatef(0.0, 0.0, -(near + far) / 2);

	// przesuni�cie obiektu z tekstur� do g�ry okna
	glTranslatef(0.0, 1.0, 0.0);

	// w��czenie teksturowania dwuwymiarowego
	glEnable(GL_TEXTURE_2D);

	// ustawienie parametr�w �rodowiska tekstur
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// dowi�zanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, texture);

	// filtr powi�kszaj�cy
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// filtr pomniejszaj�cy
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// narysowanie kwadratu z tekstur�
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(-1.5, -1.5);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(-1.5, 1.5);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(1.5, 1.5);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(1.5, -1.5);
	glEnd();

	// wy��czenie teksturowania dwuwymiarowego
	glDisable(GL_TEXTURE_2D);

	// wy�wietlenie wybranych informacji
	char string[200];
	GLint var;
	glColor3fv(Black);

	// informacja czy tekstura jest skompresowana
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &var);
	if (var == GL_FALSE)
		sprintf(string, "GL_TEXTURE_COMPRESSED = GL_FALSE");
	else
		sprintf(string, "GL_TEXTURE_COMPRESSED = GL_TRUE");
	DrawString(2, 2, string);

	// rozmiar danych tekstury
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &var);
	sprintf(string, "GL_TEXTURE_COMPRESSED_IMAGE_SIZE = %i", var);
	DrawString(2, 16, string);

	// wewn�trzny format tekstury
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &var);
	switch (var)
	{
		// formaty rozszerzenia EXT_texture_compression_s3tc
	case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGB_S3TC_DXT1_EXT");
		break;
	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT");
		break;
	case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT");
		break;
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT");
		break;

		// formaty rozszerzenia 3DFX_texture_compression_FXT1
	case GL_COMPRESSED_RGB_FXT1_3DFX:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGB_FXT1_3DFX");
		break;
	case GL_COMPRESSED_RGBA_FXT1_3DFX:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGBA_FXT1_3DFX");
		break;

		// format rozszerzenia ATI_texture_compression_3dc (nie wyst�puje w pliku glext.h)
	case 0x8837:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGB_3DC_ATI");
		break;

		// wybrane formaty nieskompresowane
	case GL_RED:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_RED");
		break;
	case GL_GREEN:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_GREEN");
		break;
	case GL_BLUE:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_BLUE");
		break;
	case GL_ALPHA:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_ALPHA");
		break;
	case GL_RGB:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_RGB");
		break;
	case GL_RGBA:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_RGBA");
		break;
	case GL_LUMINANCE:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_LUMINANCE");
		break;
	case GL_LUMINANCE_ALPHA:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_LUMINANCE_ALPHA");
		break;

		// pozosta�e formaty
	default:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = nieznany");
		break;
	}
	DrawString(2, 30, string);

	// informacja o wskaz�wkach do kompresji tekstur
	glGetIntegerv(GL_TEXTURE_COMPRESSION_HINT, &var);
	switch (var)
	{
	case GL_FASTEST:
		sprintf(string, "GL_TEXTURE_COMPRESSION_HINT = GL_FASTEST");
		break;
	case GL_DONT_CARE:
		sprintf(string, "GL_TEXTURE_COMPRESSION_HINT = GL_DONT_CARE");
		break;
	case GL_NICEST:
		sprintf(string, "GL_TEXTURE_COMPRESSION_HINT = GL_NICEST");
		break;
	}
	DrawString(2, 44, string);

	// ilo�� obs�ugiwanych format�w kompresji tekstur
	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &var);
	sprintf(string, "GL_NUM_COMPRESSED_TEXTURE_FORMATS = %i", var);

	// wykaz obs�ugiwanych format�w kompresji tekstur
	GLint formats[256];
	glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, formats);
	for (int i = 0; i < var; i++)
	{
		switch (formats[i])
		{
			// formaty rozszerzenia EXT_texture_compression_s3tc
		case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
			sprintf(string, "GL_COMPRESSED_RGB_S3TC_DXT1_EXT");
			break;
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
			sprintf(string, "GL_COMPRESSED_RGBA_S3TC_DXT1_EXT");
			break;
		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
			sprintf(string, "GL_COMPRESSED_RGBA_S3TC_DXT3_EXT");
			break;
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			sprintf(string, "GL_COMPRESSED_RGBA_S3TC_DXT5_EXT");
			break;

			// formaty rozszerzenia 3DFX_texture_compression_FXT1
		case GL_COMPRESSED_RGB_FXT1_3DFX:
			sprintf(string, "GL_COMPRESSED_RGB_FXT1_3DFX");
			break;
		case GL_COMPRESSED_RGBA_FXT1_3DFX:
			sprintf(string, "GL_COMPRESSED_RGBA_FXT1_3DFX");
			break;

			// format rozszerzenia ATI_texture_compression_3dc
			// (nie wyst�puje w pliku glext.h)
		case 0x8837:
			sprintf(string, "GL_COMPRESSED_RGB_3DC_ATI");
			break;

			// pozostra�e formaty
		default:
			sprintf(string, "Format nieznany (0x%X)", formats[i]);
			break;
		}
		DrawString(2, 70 + 14 * i, string);
	}

	// skierowanie polece� do wykonania
	glFlush();

	// zamiana bufor�w koloru
	glutSwapBuffers();
}

// zmiana wielko�ci okna

void Reshape(int width, int height)
{
	// obszar renderingu - ca�e okno
	glViewport(0, 0, width, height);

	// wyb�r macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// parametry bry�y obcinania
	if (aspect == ASPECT_1_1)
	{
		// wysoko�� okna wi�ksza od wysoko�ci okna
		if (width < height && width > 0)
			glFrustum(left, right, bottom*height / width, top*height / width, near, far);
		else

			// szeroko�� okna wi�ksza lub r�wna wysoko�ci okna
			if (width >= height && height > 0)
				glFrustum(left*width / height, right*width / height, bottom, top, near, far);
	}
	else
		glFrustum(left, right, bottom, top, near, far);

	// generowanie sceny 3D
	DisplayScene();
}

// utworzenie list wy�wietlania

void GenerateTextures()
{
	// zmienne u�yte przy obs�udze plik�w TARGA
	GLsizei width, height;
	GLenum format, type;
	GLvoid *pixels;

	// tryb upakowania bajt�w danych tekstury
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// wskaz�wki do kompresji tesktur
	glHint(GL_TEXTURE_COMPRESSION_HINT, texture_compression_hint);

	// wczytanie tekstury lena.tga
	GLboolean error = load_targa("lena.tga", width, height, format, type, pixels);

	// b��d odczytu pliku
	if (error == GL_FALSE)
	{
		printf("Niepoprawny odczyt pliku lena.tga");
		exit(0);
	}

	// utworzenie identyfikatora tekstury
	glGenTextures(1, &LENA);

	// dowi�zanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, LENA);

	// definiowanie tekstury z kompresj�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, width, height, 0, format, type, pixels);

	// utworzenie identyfikatora tekstury
	glGenTextures(1, &LENA_UNC);

	// dowi�zanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, LENA_UNC);

	// definiowanie tekstury bez kompresji
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, type, pixels);

	// porz�dki
	delete[](unsigned char*)pixels;

	// wczytanie tekstury lena_gray.tga
	error = load_targa("lena_gray.tga", width, height, format, type, pixels);

	// b��d odczytu pliku
	if (error == GL_FALSE)
	{
		printf("Niepoprawny odczyt pliku lena_gray.tga");
		exit(0);
	}

	// utworzenie identyfikatora tekstury
	glGenTextures(1, &LENA_GRAY);

	// dowi�zanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, LENA_GRAY);

	// definiowanie tekstury z kompresj�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_LUMINANCE, width, height, 0, format, type, pixels);

	// utworzenie identyfikatora tekstury
	glGenTextures(1, &LENA_GRAY_UNC);

	// dowi�zanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, LENA_GRAY_UNC);

	// definiowanie tekstury bez kompresji
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, format, type, pixels);

	// porz�dki
	delete[](unsigned char*)pixels;

	// wyb�r bie��cej tekstury
	texture = LENA;
}

// obs�uga menu podr�cznego

void Menu(int value)
{
	GLint tmp_texture;
	switch (value)
	{
		// kompresja tekstur - GL_FASTEST
	case TEXTURE_COMPRESSION_FASTEST:
	{
		texture_compression_hint = GL_FASTEST;
		tmp_texture = texture;
		GenerateTextures();
		texture = tmp_texture;
		DisplayScene();
	}
	break;

	// kompresja tekstur - GL_DONT_CARE
	case TEXTURE_COMPRESSION_DONT_CARE:
	{
		texture_compression_hint = GL_DONT_CARE;
		tmp_texture = texture;
		GenerateTextures();
		texture = tmp_texture;
		DisplayScene();
	}
	break;

	// kompresja tekstur - GL_NICEST
	case TEXTURE_COMPRESSION_NICEST:
	{
		texture_compression_hint = GL_NICEST;
		tmp_texture = texture;
		GenerateTextures();
		texture = tmp_texture;
		DisplayScene();
	}
	break;

	// tekstura lena
	case TEXTURE_LENA:
	{
		texture = LENA;
		DisplayScene();
	}
	break;

	// teksura lena nieskompresowana
	case TEXTURE_LENA_UNC:
	{
		texture = LENA_UNC;
		DisplayScene();
	}
	break;

	// tekstura lena_gray
	case TEXTURE_LENA_GRAY:
	{
		texture = LENA_GRAY;
		DisplayScene();
	}
	break;

	// tekstura lena_gray nieskompresowana
	case TEXTURE_LENA_GRAY_UNC:
	{
		texture = LENA_GRAY_UNC;
		DisplayScene();
	}
	break;

	// obszar renderingu - ca�e okno
	case FULL_WINDOW:
	{
		aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
	break;

	// obszar renderingu - aspekt 1:1
	case ASPECT_1_1:
	{
		aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
	break;

	// wyj�cie
	case EXIT:
		exit(0);
	}
}

// sprawdzenie i przygotowanie obs�ugi wybranych rozszerze�

void ExtensionSetup()
{
	// pobranie numeru wersji biblioteki OpenGL
	const char *version = (char*)glGetString(GL_VERSION);

	// odczyt wersji OpenGL
	int major = 0, minor = 0;
	if (sscanf(version, "%d.%d", &major, &minor) != 2)
	{
#ifdef WIN32
		printf("B��dny format wersji OpenGL\n");
#else

		printf("Bledny format wersji OpenGL\n");
#endif

		exit(0);
	}

	// sprawdzenie czy jest co najmniej wersja 1.4
	if (major > 1 || minor >= 4)
	{
		// pobranie wska�nika wybranej funkcji OpenGL 1.4
		glWindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress("glWindowPos2i");
	}
	else
		// sprawdzenie czy jest obs�ugiwane rozszerzenie ARB_window_pos
		if (glutExtensionSupported("GL_ARB_window_pos"))
		{
			// pobranie wska�nika wybranej funkcji rozszerzenia ARB_window_pos
			glWindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress
			("glWindowPos2iARB");
		}
		else
		{
			printf("Brak rozszerzenia ARB_window_pos!\n");
			exit(0);
		}

	// sprawdzenie czy jest co najmniej wersja 1.3 OpenGL lub
	// czy jest obs�ugiwane rozszerzenie ARB_texture_compression
	if (!(major > 1 || minor >= 3) &&
		!glutExtensionSupported("GL_ARB_texture_compression"))
	{
		printf("Brak rozszerzenia GL_ARB_texture_compression!\n");
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);

	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// rozmiary g��wnego okna programu
	glutInitWindowSize(550, 550);

	// utworzenie g��wnego okna programu
	glutCreateWindow("Kompresja tekstur");

	// do��czenie funkcji generuj�cej scen� 3D
	glutDisplayFunc(DisplayScene);

	// do��czenie funkcji wywo�ywanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// utworzenie podmenu - Tekstura
	int MenuTexture = glutCreateMenu(Menu);
	glutAddMenuEntry("lena.tga (kompresja)", TEXTURE_LENA);
	glutAddMenuEntry("lena.tga (bez kompresji)", TEXTURE_LENA_UNC);
	glutAddMenuEntry("lena_gray.tga (kompresja)", TEXTURE_LENA_GRAY);
	glutAddMenuEntry("lena_gray.tga (bez kompresji)", TEXTURE_LENA_GRAY_UNC);

	// utworzenie podmenu - GL_TEXTURE_COMPRESSION_HINT
	int MenuTextureCompressionHint = glutCreateMenu(Menu);
	glutAddMenuEntry("GL_FASTEST", TEXTURE_COMPRESSION_FASTEST);
	glutAddMenuEntry("GL_DONT_CARE", TEXTURE_COMPRESSION_DONT_CARE);
	glutAddMenuEntry("GL_NICEST", TEXTURE_COMPRESSION_NICEST);

	// utworzenie podmenu - Aspekt obrazu
	int MenuAspect = glutCreateMenu(Menu);
#ifdef WIN32

	glutAddMenuEntry("Aspekt obrazu - ca�e okno", FULL_WINDOW);
#else

	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
#endif

	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);

	// menu g��wne
	glutCreateMenu(Menu);
	glutAddSubMenu("Tekstura", MenuTexture);
	glutAddSubMenu("GL_TEXTURE_COMPRESSION_HINT", MenuTextureCompressionHint);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);

#ifdef WIN32

	glutAddMenuEntry("Wyj�cie", EXIT);
#else

	glutAddMenuEntry("Wyjscie", EXIT);
#endif

	// okre�lenie przycisku myszki obs�uguj�cego menu podr�czne
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// utworzenie tekstur
	GenerateTextures();

	// sprawdzenie i przygotowanie obs�ugi wybranych rozszerze�
	ExtensionSetup();

	// wprowadzenie programu do obs�ugi p�tli komunikat�w
	glutMainLoop();
	return 0;
}
