// OpenGL utility functions


/// Initialize common OpenGL state that does not change during the program
/// such as lighting, enabling depth buffering, background color etc.

void initializeOpenGL()
{
    // note: this should move to scene really...

	// depth buffer and backface culling
	
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
	
    glEnable(GL_SCISSOR_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// setup transforms
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, 0.1, 15);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,1.85f,8, 0,0.5f,0, 0,1,0);
	
	// set background color
	
	glClearColor(0.35f, 0.35f, 0.35f, 1);
}

/// Calculate frustum planes in world coordinates from current OpenGL projection and modelview.

void calculateFrustumPlanes(Plane &left, Plane &right, Plane &bottom, Plane &top, Plane &front, Plane &back)
{
	Matrix projection;
	glGetFloatv(GL_PROJECTION_MATRIX, projection.data());
	
	Matrix modelview;
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview.data());
	
	Matrix clip = modelview * projection;
	
	left.normal.x = clip(0,3) + clip(0,0);
	left.normal.y = clip(1,3) + clip(1,0);
	left.normal.z = clip(2,3) + clip(2,0);
	left.constant = - (clip(3,3) + clip(3,0));
	left.normalize();
	
	right.normal.x = clip(0,3) - clip(0,0);
	right.normal.y = clip(1,3) - clip(1,0);
	right.normal.z = clip(2,3) - clip(2,0);
	right.constant = - (clip(3,3) - clip(3,0));
	right.normalize();	
	
	bottom.normal.x = clip(0,3) + clip(0,1);
	bottom.normal.y = clip(1,3) + clip(1,1);
	bottom.normal.z = clip(2,3) + clip(2,1);
	bottom.constant = - clip(3,3) + clip(3,1);
	bottom.normalize();
	
	top.normal.x = clip(0,3) - clip(0,1);
	top.normal.y = clip(1,3) - clip(1,1);
	top.normal.z = clip(2,3) - clip(2,1);
	top.constant = - (clip(3,3) - clip(3,1));
	top.normalize();
	
	front.normal.x = clip(0,3) + clip(0,2);
	front.normal.y = clip(1,3) + clip(1,2);
	front.normal.z = clip(2,3) + clip(2,2);
	front.constant = - (clip(3,3) + clip(3,2));
	front.normalize();

	back.normal.x = clip(0,3) - clip(0,2);
	back.normal.y = clip(1,3) - clip(1,2);
	back.normal.z = clip(2,3) - clip(2,2);
	back.constant = - (clip(3,3) - clip(3,2));
	back.normalize();
}

/// Enter screen space

void enterScreenSpace()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, displayWidth, displayHeight, 0, 1, -1);      // todo: get width/height from viewport

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

/// Leave screen space

void leaveScreenSpace()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

/// Draw a quad from vectors

inline void quad(const Vector &a, const Vector &b, const Vector &c, const Vector &d)
{
    glVertex3f(a.x, a.y, a.z); 
    glVertex3f(b.x, b.y, b.z);
    glVertex3f(c.x, c.y, c.z);
    glVertex3f(d.x, d.y, d.z); 
}

/// Draw a 2D quad given x1,y1,x2,y2

inline void quad(float x1, float y1, float x2, float y2)
{
    glVertex2f(x1,y2);
    glVertex2f(x2,y2);
    glVertex2f(x2,y1);
    glVertex2f(x1,y1);
}
