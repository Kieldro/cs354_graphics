// GLUT tutorial

// for Linux
#include <GL/gl.h>
#include <GL/glut.h>	// find with `sudo updatedb; locate glut.h`	
#include <iostream>

// all variables initialized to 1.0, meaning
// the triangle will initially be white
float red=1.0f, blue=1.0f, green=1.0f;
double angle = 0;

void foo_renderScene(void) {
	// Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Reset transformations
  glLoadIdentity();
  
  // Set the camera
  gluLookAt(  0.0f, 0.0f, 10.0f,
              0.0f, 0.0f,  0.0f,
              0.0f, 1.0f,  0.0f);

  glRotatef(angle, 0.0f, 1.0f, 0.0f);

  // the function responsible for setting the color
  glColor3f(red,green,blue);
  glBegin(GL_TRIANGLES);
    glVertex3f(-2.0f,-2.0f, 0.0f);
    glVertex3f( 2.0f, 0.0f, 0.0);
    glVertex3f( 0.0f, 2.0f, 0.0);
  glEnd();
  angle += 0.4f;

  glutSwapBuffers();
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	double ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

        // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45, ratio, .1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void processNormalKeys(unsigned char key, int x, int y) {

  if (key == 'q')
    exit(0);
}

void processSpecialKeys(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_F1 :
        red = 1.0;
        green = 0.0;
        blue = 0.0; break;
    case GLUT_KEY_F2 :
        red = 0.0;
        green = 1.0;
        blue = 0.0; break;
    case GLUT_KEY_F3 :
        red = 0.0;
        green = 0.0;
        blue = 1.0; break;
  }
}

// void processSpecialKeys(int key, int x, int y) {

//   int mod;
//   switch(key) {
//     case GLUT_KEY_F1 :
//        mod = glutGetModifiers();
//        if (mod == (GLUT_ACTIVE_CTRL|GLUT_ACTIVE_ALT)) {
//         red = 1.0; green = 0.0; blue = 0.0;
//        }
//        break;
//     case GLUT_KEY_F2 :
//        red = 0.0;
//        green = 1.0;
//        blue = 0.0; break;
//     case GLUT_KEY_F3 :
//        red = 0.0;
//        green = 0.0;
//        blue = 1.0; break;
//   }
// }

int main(int argc, char **argv){
	
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Hello Title 2013");
	
	// register callbacks
	glutDisplayFunc(foo_renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(foo_renderScene);
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);
	
	// enter GLUT event processing cycle
	glutMainLoop();
	
	
	return 1;	// should never be reached
}