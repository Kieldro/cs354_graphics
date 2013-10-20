/*
 * drawing.c
 * ---------
 * Drawing subroutines for each of the various display modes in the canvas.
 * Also contains the quadrilateral information for a cube and the
 * triangulation information for a cone.
 *
 * Starter code for Project 1.
 *
 * Group Members: Ian Buitrago
 */

#include "common.h"
#include "drawing.h"
#include "vrml.h"

#include <cstdio>
#include <cmath>
// #include <algorithm>

const string modeStrings[] = {"Cube using glut", "Cube using quadrilaterals", 
  "Cube using quadrilateral arrays", "Cone using glut", 
  "Cone using triangles", "Cone using triangle arrays", 
  "Cone using calculated triangles", "VRML objects", "Freeform scene"};

/* The current display mode */
int disp_mode;

/* The current display style */
int disp_style;

double height;
double radius;
int base_tri;


/***********************************************************
 * Begin Cube Data
 ***********************************************************/
/*
 * Vertices used in the manually rendered cube.  This is cube
 * that is 1 x 1 x 1 centered at the origin.  Each group of
 * 3 numbers is a vertex, specified in Cartesian coordinates.
 */
GLfloat cube_vertices[] = {
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
};

/*
 * The colors of each vertex in the low level cube.  The index
 * into this array corresponds to the index into cube_vert.
 */
GLfloat cube_colors[] = {
    0.5f, 0.5f, 0.5f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
};

/*
 * Indices into cube_vert, groups of 4 can be used to draw quadrilaterals
 * which represents a face of the cube.  For instance, the first
 * quad means that vertices 0, 1, 3, 2 of the cube_vertices[]
 * are used, in that order, to form the first face of the cube.
 *
 * Note that all front facing quads are specified in a counterclockwise order
 * (that is, if you are looking at the front of a quad, the vertices will
 * appear in counterclockwise order).
 */
GLuint cube_indices[] = {
  0, 2, 3, 1,
  2, 6, 7, 3,
  7, 6, 4, 5,
  4, 0, 1, 5,
  1, 3, 7, 5,
  0, 4, 6, 2,
};
/***********************************************************
 * End Cube Data
 ***********************************************************/


/***********************************************************
 * Begin Cone Data
 ***********************************************************/

/*
 * Data for the triangulation of the surface of a cone that is one
 * unit tall has a unit circle for its base.  The base is composed
 * of 8 equally sized triangles, and the lateral surface of the cone
 * is composed of a different set of 8 equally sized triangles.
 *
 * See documentation in the Cube Data section for information on
 * the meaning of the data in each array.
 */

GLfloat cone_vertices[] = {
    1.0, -0.5, 0.0,         /* begin unit circle at y = -0.5 */
    0.707, -0.5, 0.707,
    0.0, -0.5, 1.0,
    -0.707, -0.5, 0.707,
    -1.0, -0.5, 0.0,
    -0.707, -0.5, -0.707,
    0.0, -0.5, -1.0,
    0.707, -0.5, -0.707,    /* end unit circle at y = -0.5 */
    0.0, -0.5, 0.0,         /* center of the base */
    0.0, 0.5, 0.0,          /* top of the cone */
};

GLfloat cone_colors[] = {
    0.0f, 0.0f, 0.5f,
    0.0f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
};

/*
 * Each set of 3 indices represent the vertices of a triangle.
 * Each triangle faces to the outside of the cone.  The vertices
 * of these forward facing triangles are specified in
 * counterclockwise order.
 */
GLuint cone_indices[] = {
    0, 1, 8,
    1, 2, 8,
    2, 3, 8,
    3, 4, 8,
    4, 5, 8,
    5, 6, 8,
    6, 7, 8,
    7, 0, 8,
    1, 0, 9,
    2, 1, 9,
    3, 2, 9,
    4, 3, 9,
    5, 4, 9,
    6, 5, 9,
    7, 6, 9,
    0, 7, 9,
};
/***********************************************************
 * End Cone Data
 ***********************************************************/


/* Uses glut to draw a cube */
void draw_cube_glut(void) {
	/* Draw the cube using glut */

  glColor3f(1.0, 0.0f, 0.0f);
  if (disp_style == DS_SOLID) {
    glutSolidCube(1.0f);
  } else if (disp_style == DS_WIRE) {
    glutWireCube(1.0f);
  }
}

/*
 * Draws a cube using the data arrays at the top of this file.
 * Iteratively draws each quad in the cube.
 */
void draw_cube_quad(void) {
  int num_indices = sizeof(cube_indices) / sizeof(GLuint);

  /*
   * Loop over all quads that need to be drawn.
   * Step i by 4 because there are 4 vertices per quad.
   */
  for (int i = 0; i < num_indices; i += 4) {
    /*
     * Find the index into the vertex array.  The value
     * in the cube_indices array refers to the index
     * of the ordered triples, not the index for the
     * actual GLfloats that comprise the cube_vertices array.
     * Thus, we need to multiple by 3 to get the real index.
     */

    glBegin(GL_QUADS);

    /* All arguments here are pointers */
    for(int j = 0; j < 4; ++j){
      int index = cube_indices[i + j] * 3;
      glColor3fv (&(cube_colors  [index]) );
      glVertex3fv(&(cube_vertices[index]) );
    }
    glEnd();
  }
}

/*
 * Draws a cube using the data arrays at the top of this file.
 * Uses GL's vertex arrays, index arrays, color arrays, etc.
 */
void draw_cube_quad_arrays(void) {
  int num_indices = sizeof(cube_indices) / sizeof(GLuint);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
  glColorPointer(3, GL_FLOAT, 0, cube_colors);
  glDrawElements(GL_QUADS, num_indices,
          GL_UNSIGNED_INT, cube_indices);

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

/*
 * Uses glut to draw a cone.  Must render in either solid and wire
 * frame modes, based on the value of the variable disp_style.
 */
void draw_cone_glut(void) {
  float radius = 1;
  float height = 2;
  int slices = 8;
  int stacks = 1;
  
  glColor3f(0, 0.0f, 1);
  if (disp_style == DS_SOLID) {
    glutSolidCone(radius, height, slices, stacks);
  }else if (disp_style == DS_WIRE) {
    glutWireCone(radius, height, slices, stacks);
  }
}

/*
 * Draws a cone using the data arrays at the top of this file.
 * Iteratively draws each triangle in the cone.
 */
void draw_cone_tri(void) {
  int num_indices = sizeof(cone_indices) / sizeof(GLuint);
  /*
   * Loop over all tris that need to be drawn.
   * Step i by 3 because there are 3 vertices per triangle.
   */
  for (int i = 0; i < num_indices; i += 3) {
    /*
     * Find the index into the vertex array.  The value
     * in the cone_indices array refers to the index
     * of the ordered triples, not the index for the
     * actual GLfloats that comprise the cone_vertices array.
     * Thus, we need to multiple by 3 to get the real index.
     */

    glBegin(GL_TRIANGLES);

      /* All arguments here are pointers */
      for(int j = 0; j < 3; ++j){
        int index = cone_indices[i + j] * 3;
        glColor3fv (&(cone_colors  [index]) );
        glVertex3fv(&(cone_vertices[index]) );
      }
    glEnd();
  }

}

/*
 * Draws a cone using the data arrays at the top of this file.
 * Uses GL's vertex arrays, index arrays, color arrays, etc.
 */
void draw_cone_tri_arrays(void) {
  int num_indices = sizeof(cone_indices) / sizeof(GLuint);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, cone_vertices);
  glColorPointer(3, GL_FLOAT, 0, cone_colors);
  glDrawElements(GL_TRIANGLES, num_indices,
          GL_UNSIGNED_INT, cone_indices);

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

}

/*
 * Draws a cone using a calculated triangulation of the cone surface.
 *
 * Args
 * ----
 * The HEIGHT and RADIUS of the cone.
 *
 * BASE_TRI refers to the number of triangles used to represent
 * the base of the cone.  Each of these triangles should share a
 * common vertex, namely, the center of the base.
 *
 * The final triangulation of the cone surface should include
 * exactly 2 * BASE_TRI.
 */
void draw_cone_tri_calc(void) {
  // if(DEBUG) cerr << "DRAW CONE! " << ' ' << endl;
  double x, y = -height/2, z, theta;
  double centers[] = {y, -y};
  
  // draw base
  for(double c: centers){
    glBegin(GL_TRIANGLE_FAN);
      glColor3f(0.0f, 0.0f, 1.0f);    // blue
      glVertex3d(0, c, 0);    // center of fan
      
      glColor3f(0.0f, 0.0f, .5f);    // create a gradient toward center
      for (int n = 0; n <= base_tri; ++n) {
        theta = 2 * M_PI / base_tri * n;   // in radians
        x = cos(theta) * radius;
        z = sin(theta) * radius;
        // GLdouble vertex[] = {x, y, z};
        // if(DEBUG) cerr << "n  = " << n << endl;
        // if(DEBUG) cerr << "theta  = " << theta/(2*M_PI)*360 << endl;
        
        // char str[64];
        // sprintf(str, "vertex = (%f, %f, %f)", vertex[0], vertex[1], vertex[2]);
        // if(DEBUG) cerr << str << endl;
        
        glVertex3d(x, y, z);
      }
    glEnd();
  }
}

/* Draw the various vrml scenes */
const vector<void(*)(void)> vrmlFunctions = {draw_vrml_cube, 
  draw_vrml_dodecahedron, draw_vrml_icosahedron, draw_vrml_pyramid};

void draw_vrml(void) {
	/* NOTE: you should be calling a function or functions in vrml.c++ */
  vrmlFunctions[vr_object]();
}


/* Draws a freeform scene */
void draw_free_scene(void) {
	/* ADD YOUR CODE HERE */
	/* NOTE: Modify or remove the existing code in this func, as necessary */

	/*
	 * Draw a red torus.
	 *
	 * glutWireTorus args: (inner radius, outer radius,
	 * sides per radial section, # of radial sections)
	 */
	glColor3f(1.0f, 0.0f, 0.0f);		/* red */
	glutWireTorus(0.1, 0.4, 10, 20);

	/*
	 * Draw a green cube at an offset of (0, 1, 0) from the center of
	 * the torus.  Note that the glPushMatrix remembers the current
	 * drawing position (the center of the torus), the glTranslatef
	 * translates the drawing point by and offset, and the
	 * glPopMatrix restores the drawing point to the center of
	 * the torus.
	 */
	glPushMatrix();
	glTranslatef(1.0f, 0.0f, 1.0f);		/* the drawing offset */
	glColor3f(0.0f, 1.0f, 0.0f);		/* green */
	glutWireCube(1.0f);
	glPopMatrix();
}

/* Prints to stdout the current display mode */
void print_disp_mode( void ) {
  if(disp_mode < 0 or disp_mode >= DM_MAX){
    cout << "Warning: unknown display mode" << endl;
    return;
  }
  
  cout << "Display mode: " << modeStrings[disp_mode] << endl;
}

/* Prints to stdout the current display style */
void print_disp_style( void ) {
	if (disp_style == DS_SOLID) {
		printf("Display Style: solid (for glut modes only)\n");
	} else if (disp_style == DS_WIRE) {  
		printf("Display Style: wire (for glut modes only)\n");
	} else {
		printf("Warning: unknown display style\n");
	}
}

// end of drawing.c 