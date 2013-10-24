#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef _WIN32
  #include <windows.h>
#endif

#ifdef __MAC__
  #include <OpenGL/gl.h>
  #include <GLUT/glut.h>
#else		// linux
  #include <GL/gl.h>
  #include <GL/glut.h>
#endif

#include <iostream>

using namespace std;

#endif
