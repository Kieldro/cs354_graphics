/*
 * canvas.c
 * --------
 * Implements a canvas with multiple display modes.
 * Starter code for Project 1.
 *
 * Group Members: Ian Buitrago
 */

#include "common.h"
#include "drawing.h"
#include "vrml.h"
#include "mouse.h"

#include <stdio.h>
#include <stdlib.h>

/* Function Declarations */
void myInit (int argc, char **argv);

void myDisplay (void);
void myReshape (int, int);
void myKeyHandler (unsigned char, int, int);

void resetCamera(void);
int endCanvas(int status);
void performanceTest(void);
void initLighting(void);

/* The current vrml object */
int vr_object;

/*************************************************************
 * Global Variables / Constants
 * ----------------------------
 * The following variables are visible globally, and most
 * control the global state of the canvas or contain data
 * that would be cumbersome to pass around.
 *************************************************************/


/* The canvas's width and height, in pixels */
const int win_width = 500;
const int win_height = 500;

/* The dimensions of the viewing frustum */
GLfloat fleft   = -1.0;
GLfloat fright  =  1.0;
GLfloat fbottom = -1.0;
GLfloat ftop    =  1.0;
GLfloat zNear   = -2.0;
GLfloat zFar    = -7.0;


/* Global zoom factor.  Modified by user input. Initially 1.0 */
GLfloat zoomFactor = 1.0; 

/* Constants for specifying the 3 coordinate axes */
#define X_AXIS			0
#define Y_AXIS			1
#define Z_AXIS			2

/*************************************************************
 * End Global Variables / Constants
 *************************************************************/

/* Begin function definitions */

/*
 * Performs specific initializations for this program (as opposed to
 * glut initialization).
 */
void myInit (int argc, char **argv) {
	/* Set the default display mode and style */
	disp_mode = DM_CUBE_GLUT;
	disp_style = DS_WIRE;
	vr_object = VR_CUBE;
	
	height = 2;
	radius = 1;
	base_tri = 6;
	

	/* Set up a black background */
	glClearColor(0.0, 0.0, 0.0, 0.0);

	resetCamera();
	if(DEBUG) glRotatef(15, 1.0, 0, 0);
}


// function look up table mapping drawing modes to respective functions
const vector<void(*)(void)> functionTable = {draw_cube_glut, draw_cube_quad, 
		draw_cube_quad_arrays, draw_cone_glut, draw_cone_tri, draw_cone_tri_arrays, 
		draw_cone_tri_calc, draw_vrml, draw_free_scene};

/*
 * The main drawing routine.  Based on the current display mode, other
 * helper functions may be called.
 */
void myDisplay (void) {
	glEnable(GL_DEPTH_TEST);	/* Use the Z - buffer for visibility */
	glMatrixMode(GL_MODELVIEW);	/* All matrix operations are for the model */

	/* Clear the pixels (aka colors) and the z-buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	assert(disp_mode >= 0 and disp_mode < DM_MAX);
	
	functionTable[disp_mode]();		// call rendering function

	glFlush();	/* Flush all executed OpenGL ops finish */

	/*
	 * Since we are using double buffers, we need to call the swap
	 * function every time we are done drawing.
	 */
	glutSwapBuffers();
}


/*
 * Changes the size of the canvas's window, and will implicitly
 * the function bound by glutReshapeFunc(), which should be
 * Reshape().
 */
void myResize (int x, int y) {
	glViewport(0,0,x,y);
	glutReshapeWindow(x, y);
}


/* Stretch the image to fit the reshaped window */
void myReshape (int x, int y) {
 	glViewport(0,0,x,y);
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
	double x, y, z;

	x = 0;
	y = 0;
	z = 0;

	if (axis == X_AXIS) {
		x = 1.0f;
	} else if (axis == Y_AXIS) {
		y = 1.0f;
	} else if (axis == Z_AXIS) {
		z = 1.0f;
	}
 
	glRotatef(deg, x, y, z);
}


/*
 * Changes the level of zooming by adjusting the dimenstions of the viewing
 * frustum.
 *
 * Args: delta - the change in the zoom factor.  Negative deltas cause the
 * camera to zoom in, while positive values cause the camera to zoom out.
 */
void zoomCamera(double delta) {
	zoomFactor += delta;

	if (zoomFactor <= 0.0) {
		/* The zoom factor should be positive */
		zoomFactor = 0.001;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	 * glFrustum must receive positive values for the near and far clip planes
	 * ( arguments 5 and 6 ).
	 */
	glFrustum(fleft*zoomFactor, fright*zoomFactor,
		fbottom*zoomFactor, ftop*zoomFactor,
		-zNear, -zFar);

}

/*
 * Resets the viewing frustum and moves the drawing point to the center of
 * the frustum.
 */
void resetCamera( void ) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	 * glFrustum must receive positive values for the near and far clip planes
	 * ( arguments 5 and 6 ).
	 */
	glFrustum(fleft, fright, fbottom, ftop, -zNear, -zFar);

	/* Set the drawing point at the center of the frustum */
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	/* Go to the center of the scene */
	glTranslatef(
		(fleft + fright) / 2,
		(fbottom + ftop) / 2,
		(zNear + zFar) / 2);

	zoomFactor = 1.0;
}


void performanceTest(void) {
	int start, end;
	int i;
	int curr_width, curr_height;

	/* Give a warning if the window has been resized */
	curr_width = glutGet(GLUT_WINDOW_WIDTH);
	curr_height = glutGet(GLUT_WINDOW_HEIGHT);

	if ((curr_width != win_width) || (curr_height != win_height)) {
		printf("*** Warning ***\n");
		printf("The window has been resized and results may be inaccurate.\n");
		printf("First press 'z' to restore the default window size.\n");
		printf("*** Warning ***\n");
	}

	resetCamera();

	printf("Initiating Performance Test\n");
	start = glutGet(GLUT_ELAPSED_TIME);

	/* For every rotation, the display loop will be recalled */
	for (i = 0; i < 360; i++) {
		rotateCamera(1.0, X_AXIS);
		myDisplay();		/* refresh the screen */
	}
	for (i = 0; i < 360; i++) {
		rotateCamera(1.0, Y_AXIS);
		myDisplay();		/* refresh the screen */
	}
	for (i = 0; i < 360; i++) {
		rotateCamera(1.0, Z_AXIS);
		myDisplay();		/* refresh the screen */
	}

	end = glutGet(GLUT_ELAPSED_TIME);

	/* Return the number of milliseconds elapsed */
	printf("Performance Test completed in %.2f sec\n",
		(end - start) / 1000.0f);
}


/* Handle user input */
void myKeyHandler(unsigned char ch, int x, int y) {
	switch(ch) {
		case 'c':
			resetCamera();
			printf("Camera reset.\n");
			break;

		case 's':
		case 'S':
			if (disp_style == DS_SOLID) {
				disp_style = DS_WIRE;
			} else {
				disp_style = DS_SOLID;
			}
			print_disp_style();
			break;

		case 'd':
			/* Cycle through the various display modes */
			disp_mode += 1;
			disp_mode %= DM_MAX;
			print_disp_mode();
			break;

		case 'D':
			/* Cycle through the various display modes backwards */
			/* By adding DM_MAX, the args to "%" will never be negative */
			disp_mode += DM_MAX - 1;
			disp_mode %= DM_MAX;
			print_disp_mode();
			break;
		// vrml
		case 'v':
			vr_object += 1;
			vr_object %= VR_MAX;
			print_vrml_object();
			break;

		case ',':
			rotateCamera(5, X_AXIS);
			break;

		case '<':
			rotateCamera(-5, X_AXIS);
			break;

		case '.':
			rotateCamera(5, Y_AXIS);
			break;

		case '>':
			rotateCamera(-5, Y_AXIS);
			break;

		case '/':
			rotateCamera(5, Z_AXIS);
			break;

		case '?':
			rotateCamera(-5, Z_AXIS);
			break;

		case '+':
			/* Zoom in */
			zoomCamera(-0.1);
			break;

		case '=':
			/* Zoom out */
			zoomCamera(0.1);
			break;

		case 'z':
			myResize(win_width, win_height);
			printf("Window set to default size.\n");
			break;
		case 't':
			performanceTest();
			break;
		// for draw_cone_tri_calc
		case 'I':
			radius += .1;
			cout << "radius set to " << radius << " (calculated triangulation only)" << endl;
			break;
		case 'i':
			radius -= .1;
			if(radius < .1)
				radius = .1;
			cout << "radius set to " << radius << " (calculated triangulation only)" << endl;
			break;
		case 'O':
			height += .1;
			cout << "height set to " << height << " (calculated triangulation only)" << endl;
			break;
		case 'o':
			height -= .1;
			if(height < .1)
				height = .1;
			cout << "height set to " << height << " (calculated triangulation only)" << endl;
			break;
		case 'P':
			base_tri += 1;
			cout << "base_tri set to " << base_tri << " (calculated triangulation only)" << endl;
			break;
		case 'p':
			base_tri -= 1;
			if(base_tri < 3)
				base_tri = 3;
			cout << "base_tri set to " << base_tri << " (calculated triangulation only)" << endl;
			break;

		case 'q':
			/* Quit with exit code 0 */
			endCanvas(0);
			break;


		/*********************************************/
		/* ADD ADDITIONAL KEYS HERE                  */
		/*********************************************/


		default:
			/* Unrecognized key press, just return */
			return;

			break;
	}

	/*
	 * If control reaches here, the key press was recognized.  Refresh
	 * the screen, since most key presses change the display in some way.
	 */
	myDisplay();

	return;
}


int endCanvas(int status) {
  printf("\nQuitting canvas.\n\n");
  fflush(stdout);

  exit(status);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	// Set initial window size and screen offset 
	glutInitWindowSize(win_width, win_height);
	glutInitWindowPosition(1200, 200);

	/* Using: RGB (no alpha), double buffering, z-buffer */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Canvas for project 1");

	/* Set the function callbacks */
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(myKeyHandler);
	glutMouseFunc(myMouseButton);
	glutMotionFunc(myMouseMotion);
	// if(DEBUG) cerr << "BOOYAKASHA  = " << sizeof(float) << endl;
	// if(DEBUG)fprintf(stderr, "BOOYAKASHA  = %s\n", typeid(myKeyHandler));

	/* User specific initialization */
	myInit(argc, argv);

	glutMainLoop();	/* Go into the main glut control loop, will not return */

	/* Control flow will never reach here */
	return 0;
}

/* end of canvas.c++ */
