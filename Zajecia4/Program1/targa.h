/*
(c) Janusz Ganczarski
http://www.januszg.hg.pl
JanuszG@enter.net.pl
*/

#ifndef __TARGA__H__
#define __TARGA__H__

#include <GL/gl.h>

// odczyt pliku graficznego w formacie TARGA
// filename - nazwa pliku
// width - szeroko�� obrazu
// height - wysoko�� obrazu
// format - format danych obrazu
// type - format danych pikseli obrazu
// pixels - wska�nik na tablic� z danymi obrazu

GLboolean load_targa (const char *filename, GLsizei &width, GLsizei &height,
                      GLenum &format, GLenum &type, GLvoid *&pixels);

// zapis pliku graficznego w formacie TARGA
// filename - nazwa pliku
// width - szeroko�� obrazu
// height - wysoko�� obrazu
// format - format danych obrazu
// type - format danych pikseli obrazu
// pixels - wska�nik na tablic� z danymi obrazu

GLboolean save_targa (const char *filename, GLsizei width, GLsizei height,
                      GLenum format, GLenum type, GLvoid *pixels);

#endif // __TARGA__H__

