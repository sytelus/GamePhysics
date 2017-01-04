// Simple RK4 integration framework
// Copyright (c) 2004, Glenn Fiedler
// http://www.gaffer.org/articles

#include <stdio.h>
#include <math.h>

struct State
{
	float x;
	float v;
};

struct Derivative
{
	float dx;
	float dv;
};

float acceleration(const State &state, float t)
{
	const float k = 10;
	const float b = 1;
	return - k*state.x - b*state.v;
}

Derivative evaluate(const State &initial, float t)
{
	Derivative output;
	output.dx = initial.v;
	output.dv = acceleration(initial, t);
	return output;
}

Derivative evaluate(const State &initial, float t, float dt, const Derivative &d)
{
	State state;
	state.x = initial.x + d.dx*dt;
	state.v = initial.v + d.dv*dt;
	Derivative output;
	output.dx = state.v;
	output.dv = acceleration(state, t+dt);
	return output;
}

void integrate(State &state, float t, float dt)
{
	Derivative a = evaluate(state, t);
	Derivative b = evaluate(state, t, dt*0.5f, a);
	Derivative c = evaluate(state, t, dt*0.5f, b);
	Derivative d = evaluate(state, t, dt, c);

	const float dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
	const float dvdt = 1.0f/6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv);
	
	state.x = state.x + dxdt*dt;
	state.v = state.v + dvdt*dt;
}

int main() 
{
	State state;
	state.x = 100;
	state.v = 0;
	
	float t = 0;
	float dt = 0.1f;
	
	while (fabs(state.x)>0.001f || fabs(state.v)>0.001f)
	{
		printf("%.2f, %.2f\n", state.x, state.v);
		integrate(state, t, dt);
		t += dt;
	}

	getc(stdin);
	
    return 0;
}
