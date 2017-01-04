// Physics in 3D Example
// Copyright (c) Glenn Fiedler 2004
// http://www.gaffer.org/articles

#include "Mathematics.h"
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

using namespace Mathematics;

bool openDisplay(const char title[], int width, int height, bool fullscreen = false);
void updateDisplay();
void closeDisplay();
float time();

void onQuit();

#include <vector>
#include "Apple.h"
#include "Windows.h"
#include "OpenGL.h"
#include "Cube.h"

bool quit = false;

void onQuit()
{
	quit = true;
}

Cube cube;

int main()
{
	const int width = 800;
	const int height = 600;

	if (!openDisplay("Physics in 3D", width, height))
        return 1;

	initializeOpenGL();

	const float dt = 0.01f;

	float t = 0.0f;
	float accumulator = 0.0f;
	float currentTime = 0.0f;

	while (!quit)
	{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// update current time

        float newTime = time();
        float deltaTime = newTime - currentTime;

        if (deltaTime<=0.0f)
            continue;

		currentTime = newTime;

        // update discrete time

		accumulator += deltaTime;

		while (accumulator>=dt)
        {
			cube.update(t, dt);
            accumulator -= dt;
            t += dt;
        }

		cube.render();

        // update display

        updateDisplay();
	}
	
	closeDisplay();
	
	return 0;
}
