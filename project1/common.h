#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __MAC__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

// C++ headers
#include <iostream>
#include <typeinfo>
#include <vector>
#include <string>
#include <cassert>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

// macros
#define DEBUG   0


#endif
