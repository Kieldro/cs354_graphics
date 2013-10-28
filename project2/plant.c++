/*
 * plant.c++
 * ---------
 * Contains function callbacks and bookkeeping used by glut.  Starter code
 * for Project 2.
 *
 * Group Members: Ian Buitrago
 */

#ifdef _WIN32
  #include <windows.h>
#endif


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>

#include "common.h"
#include "drawplant.h"

/* Constants for specifying the 3 coordinate axes */
#define X_AXIS      0
#define Y_AXIS      1
#define Z_AXIS      2

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W = 800;		/* window width */
int H = 600;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */

/* local function declarations */
void display(void);
void init(void);
void keyHandler(unsigned char ch, int x, int y);
void rotateCamera(double deg, int axis);

int main (int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Project 2: Fractal Plants");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyHandler);
  glutMainLoop();
  
  return 0;
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-40.0, 40.0, -40.0, 40.0, -10.0, 10.0);
}


void display() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* See drawplant.c++ for the definition of this routine */
	drawPlant();

  glFlush();  /* Flush all executed OpenGL ops finish */

  /*
   * Since we are using double buffers, we need to call the swap
   * function every time we are done drawing.
   */
  glutSwapBuffers();
}

/* Handle user input */
void keyHandler(unsigned char ch, int x, int y) {
  int increment = 5;
  switch(ch) {
    // case 'c':
    //   resetCamera();
    //   printf("Camera reset.\n");
    //   break;

    case ',':
      // cerr << "BOOYAKASHA!" << endl;
      rotateCamera(increment, X_AXIS);
      break;

    case '<':
      rotateCamera(-increment, X_AXIS);
      break;

    case '.':
      rotateCamera(increment, Y_AXIS);
      break;

    case '>':
      rotateCamera(-increment, Y_AXIS);
      break;

    case '/':
      rotateCamera(increment, Z_AXIS);
      break;

    case '?':
      rotateCamera(-increment, Z_AXIS);
      break;

    // case '+':
    //   /* Zoom in */
    //   zoomCamera(-0.1);
    //   break;

    // case '=':
    //   /* Zoom out */
    //   zoomCamera(0.1);
    //   break;

    // case 'z':
    //   myResize(win_width, win_height);
    //   printf("Window set to default size.\n");
    //   break;
    
    case 'q':
      cout << endl << "Exiting canvas." << endl << endl;
      exit(0);

    default:
      /* Unrecognized key press, just return */
      return;
  }

  /*
   * If control reaches here, the key press was recognized.  Refresh
   * the screen, since most key presses change the display in some way.
   */
  display();

  return;
}

/*
 * The rotation is specified in degrees about a certain axis of
 * the original model.
 *
 * AXIS should be either X_AXIS, Y_AXIS, or Z_AXIS.
 *
 * Positive degrees rotate in the counterclockwise direction.
 */
void rotateCamera(double deg, int axis) {
  double x = 0;
  double y = 0;
  double z = 0;

  if (axis == X_AXIS) {
    x = 1.0;
  } else if (axis == Y_AXIS) {
    y = 1.0;
  } else if (axis == Z_AXIS) {
    z = 1.0;
  }
 
  glMatrixMode( GL_PROJECTION );
  glRotatef(deg, x, y, z);
}
/* end of plant.c++ */
