// OpenGL utility functions


/// Initialize common OpenGL state that does not change during the program
/// such as lighting, enabling depth buffering, background color etc.

void initializeOpenGL()
{
	// depth buffer and backface culling
	
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
	
	// setup transforms
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, 0.1, 15);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0,5, 0,0,0, 0,1,0);
	
	// set background color
	
	glClearColor(0, 0, 0, 1);

	// setup light

    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);

    GLfloat lightAmbientColor[] = { 0.5, 0.5, 0.5, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbientColor);

    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01f);

    GLfloat lightPosition[] = { 25.0, 10.0, 25.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}
