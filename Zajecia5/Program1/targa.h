﻿/*
(c) Janusz Ganczarski
http://www.januszg.hg.pl
JanuszG@enter.net.pl
*/

#ifndef __TARGA__H__
#define __TARGA__H__

#define _CRT_SECURE_NO_WARNINGS
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

// odczyt pliku graficznego w formacie TARGA
// filename - nazwa pliku
// width - szerokoœæ obrazu
// height - wysokoœæ obrazu
// format - format danych obrazu
// type - format danych pikseli obrazu
// pixels - wskaŸnik na tablicê z danymi obrazu

GLboolean load_targa(const char *filename, GLsizei &width, GLsizei &height,
	GLenum &format, GLenum &type, GLvoid *&pixels);

// zapis pliku graficznego w formacie TARGA
// filename - nazwa pliku
// width - szerokoœæ obrazu
// height - wysokoœæ obrazu
// format - format danych obrazu
// type - format danych pikseli obrazu
// pixels - wskaŸnik na tablicê z danymi obrazu

GLboolean save_targa(const char *filename, GLsizei width, GLsizei height,
	GLenum format, GLenum type, GLvoid *pixels);

#endif // __TARGA__H__
