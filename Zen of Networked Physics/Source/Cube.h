/// A cube with self contained physics simulation.
///
/// This class is responsible for maintaining and integrating its
/// physics state using an RK4 integrator. The nature of the
/// integrator requires that we structure this class in such a
/// way that all forces can be calculated from the current physics
/// state at any time. See Cube::integrate for details.

class Cube
{
public:

    /// Cube color

    float r,g,b,a;

    /// Input data.

    struct Input
    {
        bool left;
        bool right;
        bool forward;
        bool back;
        bool jump;
    };

    /// Physics state.
    
    struct State
    {
        /// primary physics state

        Vector position;                ///< the position of the cube center of mass in world coordinates (meters).
        Vector momentum;                ///< the momentum of the cube in kilogram meters per second.
        Quaternion orientation;         ///< the orientation of the cube represented by a unit quaternion.
        Vector angularMomentum;         ///< angular momentum vector.

        // secondary state

        Vector velocity;                ///< velocity in meters per second (calculated from momentum).
        Quaternion spin;                ///< quaternion rate of change in orientation.
        Vector angularVelocity;         ///< angular velocity (calculated from angularMomentum).
        Matrix bodyToWorld;             ///< body to world coordinates matrix.
        Matrix worldToBody;             ///< world to body coordinates matrix.

        /// constant state

        float size;                     ///< length of the cube sides in meters.
        float mass;                     ///< mass of the cube in kilograms.
        float inverseMass;              ///< inverse of the mass used to convert momentum to velocity.
        float inertiaTensor;            ///< inertia tensor of the cube (i have simplified it to a single value due to the mass properties a cube).
        float inverseInertiaTensor;     ///< inverse inertia tensor used to convert angular momentum to angular velocity.

        /// Recalculate secondary state values from primary values.

        void recalculate()
        {
            assert(position==position);
            assert(momentum==momentum);
            assert(orientation==orientation);
            assert(angularMomentum==angularMomentum);

            velocity = momentum * inverseMass;
            angularVelocity = angularMomentum * inverseInertiaTensor;
            orientation.normalize();
            spin = 0.5 * Quaternion(0, angularVelocity.x, angularVelocity.y, angularVelocity.z) * orientation;
            Matrix translation;
            translation.translate(position);
            bodyToWorld = translation * orientation.matrix();
            worldToBody = bodyToWorld.inverse();
        }

        /// equality operator (primary quantities only)

        bool operator==(const State &other) const
        {
            return position==other.position && orientation==other.orientation && 
                momentum==other.momentum && angularMomentum==other.angularMomentum;
        }

        /// inequality operator (primary quantities only)

        bool operator!=(const State &other) const
        {
            return !(*this==other);
        }

        /// compare with another physics state for "significant" differences.
        /// used for detecting position or orientation snaps which need smoothing.

        bool compare(const State &other) const
        {
            const float threshold = 0.1f * 0.1f;

            return (other.position-position).lengthSquared()>threshold || 
                (other.orientation-orientation).norm()>threshold;
        }
    };

    /// Default constructor.
	
	Cube()
	{
		current.size = 1;
		current.mass = 1;
		current.inverseMass = 1.0f / current.mass;
		current.position = Vector(0,40,0);
		current.momentum = Vector(0,0,0);
		current.orientation.identity();
		current.angularMomentum = Vector(0,0,0);
		current.inertiaTensor = current.mass * current.size * current.size * 1.0f / 6.0f;
		current.inverseInertiaTensor = 1.0f / current.inertiaTensor;
		current.recalculate();
		
		previous = current;

        r = g = b = a = 1;
	}

    /// Update physics state.
    /// @param input the current input data.
    /// @param planes the set of world collision planes to collide against.
    /// @param dt delta time to advance ahead in seconds.

    void update(const Input &input, const std::vector<Plane> &planes, float dt)
    {
        previous = current;
        integrate(input, planes, current, dt);
    }

    /// Smooth physics state towards target.

    void smooth(const State &target, float tightness)
    {
        previous = current;
        current = target;
        current.position = previous.position + (target.position-previous.position) * tightness;
        current.orientation = slerp(previous.orientation, target.orientation, tightness);
        current.recalculate();
    }

    /// Render cube at interpolated state.
    /// Calculates interpolated state then renders cube at the interpolated 
	/// position and orientation using OpenGL.
	/// @param alpha interpolation alpha in [0,1]

    void render(const Vector &light, float alpha = 1.0f)
	{	
		glPushMatrix();

		State state = interpolate(previous, current, alpha);
		
		glTranslatef(state.position.x, state.position.y, state.position.z); 
		
		float angle;
		Mathematics::Vector axis;		
		state.orientation.angleAxis(angle, axis);
		glRotatef(angle/Mathematics::pi*180, axis.x, axis.y, axis.z);
		
        // render cube

        GLfloat color[] = { r, g, b, a };

		glMaterialfv(GL_FRONT, GL_AMBIENT, color);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
		
		glEnable(GL_LIGHTING);
		
        if (a!=1.0f)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        glDepthFunc(GL_ALWAYS);

        const float s = state.size * 0.5f;

        glBegin(GL_QUADS);
		
			glNormal3f(0,0,+1);
			glVertex3f(-s,-s,+s);
			glVertex3f(+s,-s,+s);
			glVertex3f(+s,+s,+s);
			glVertex3f(-s,+s,+s);

			glNormal3f(0,0,-1);
			glVertex3f(-s,-s,-s);
			glVertex3f(-s,+s,-s);
			glVertex3f(+s,+s,-s);
			glVertex3f(+s,-s,-s);
			
			glNormal3f(0,+1,0);
			glVertex3f(-s,+s,-s);
			glVertex3f(-s,+s,+s);
			glVertex3f(+s,+s,+s);
			glVertex3f(+s,+s,-s);
			
			glNormal3f(0,-1,0);
			glVertex3f(-s,-s,-s);
			glVertex3f(+s,-s,-s);
			glVertex3f(+s,-s,+s);
			glVertex3f(-s,-s,+s);

			glNormal3f(+1,0,0);
			glVertex3f(+s,-s,-s);
			glVertex3f(+s,+s,-s);
			glVertex3f(+s,+s,+s);
			glVertex3f(+s,-s,+s);
			
			glNormal3f(-1,0,0);
			glVertex3f(-s,-s,-s);
			glVertex3f(-s,-s,+s);
			glVertex3f(-s,+s,+s);
			glVertex3f(-s,+s,-s);
			
		glEnd();

        glDepthFunc(GL_LEQUAL);

        glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);

        // render shadow volume

        if (a==1.0f)
        {
            // enter state for rendering shadow volumes to stencil

            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDepthMask(GL_FALSE);
            
            glEnable(GL_STENCIL_TEST);

            Vector bodySpaceLight = state.worldToBody * light;

            // render front faces

            glStencilFunc(GL_ALWAYS, 0x0, 0xff);
            glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

            renderShadowVolume(state, bodySpaceLight);

            // render shadow volume back faces

            glCullFace(GL_FRONT);
            glStencilFunc(GL_ALWAYS, 0x0, 0xff);
            glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
            
            renderShadowVolume(state, bodySpaceLight);

            // restore normal rendering state

            glCullFace(GL_BACK);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthMask(GL_TRUE);

            glDisable(GL_STENCIL_TEST);
        }

		glPopMatrix();
	}

    void snap(const State &state)
    {
        current = state;
        previous = state;
    }

    const State &state() const
    {
        return current;
    }
	
private:

    /// render shadow volume

    static void renderShadowVolume(const State &state, const Vector &light)
    {
        glBegin(GL_QUADS);

        const float s = state.size * 0.5f;

        silhouette(light, Vector(-s,+s,-s), Vector(+s,+s,-s));
        silhouette(light, Vector(+s,+s,-s), Vector(+s,+s,+s));
        silhouette(light, Vector(+s,+s,+s), Vector(-s,+s,+s));
        silhouette(light, Vector(-s,+s,+s), Vector(-s,+s,-s));

        silhouette(light, Vector(-s,-s,-s), Vector(+s,-s,-s));
        silhouette(light, Vector(+s,-s,-s), Vector(+s,-s,+s));
        silhouette(light, Vector(+s,-s,+s), Vector(-s,-s,+s));
        silhouette(light, Vector(-s,-s,+s), Vector(-s,-s,-s));

        silhouette(light, Vector(-s,+s,-s), Vector(-s,-s,-s));
        silhouette(light, Vector(+s,+s,-s), Vector(+s,-s,-s));
        silhouette(light, Vector(+s,+s,+s), Vector(+s,-s,+s));
        silhouette(light, Vector(-s,+s,+s), Vector(-s,-s,+s));

        glEnd();
    }

    /// test and render edge a-b if its a silhouette edge relative to light

    static void silhouette(const Vector &light, Vector a, Vector b)
    {
        // determine edge normals

        Vector midpoint = (a + b) * 0.5f;
        
        Vector leftNormal;

        if (midpoint.x!=0)
            leftNormal = Vector(midpoint.x, 0, 0);
        else
            leftNormal = Vector(0, midpoint.y, 0);

        Vector rightNormal = midpoint - leftNormal;

        // check if silhouette edge

        const Vector differenceA = a - light;

        const float leftDot = leftNormal.dot(differenceA);
        const float rightDot = rightNormal.dot(differenceA);

        if ((leftDot<0 && rightDot>0) || (leftDot>0 && rightDot<0))
        {
            // extrude quad

            const Vector differenceB = b - light;

            Vector _a = a + differenceA * 100;
            Vector _b = b + differenceB * 100;

            // ensure correct winding order for silhouette edge

            const Vector cross = (b - a).cross(differenceA);
            
            if (cross.dot(a)<0)
            {
                Vector t = a;
                a = b;
                b = t;

                t = _a;
                _a = _b;
                _b = t;
            }

            // render extruded quad

            glVertex3f(a.x, a.y, a.z); 
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(_b.x, _b.y, _b.z);
            glVertex3f(_a.x, _a.y, _a.z); 
        }
    }

    /// Interpolate between two physics states.
	
	static State interpolate(const State &a, const State &b, float alpha)
	{
		State state = b;
		state.position = a.position*(1-alpha) + b.position*alpha;
		state.momentum = a.momentum*(1-alpha) + b.momentum*alpha;
		state.orientation = slerp(a.orientation, b.orientation, alpha);
		state.angularMomentum = a.angularMomentum*(1-alpha) + b.angularMomentum*alpha;
		state.recalculate();
		return state;
	}

	State previous;		///< previous physics state.
    State current;		///< current physics state.

    /// Derivative values for primary state.
    /// This structure stores all derivative values for primary state in Cube::State.
    /// For example velocity is the derivative of position, force is the derivative
    /// of momentum etc. Storing all derivatives in this structure makes it easy
    /// to implement the RK4 integrator cleanly because it needs to calculate the
    /// and store derivative values at several points each timestep.

	struct Derivative
	{
		Vector velocity;                ///< velocity is the derivative of position.
		Vector force;                   ///< force in the derivative of momentum.
		Quaternion spin;                ///< spin is the derivative of the orientation quaternion.
		Vector torque;                  ///< torque is the derivative of angular momentum.
	};	

    /// Evaluate all derivative values for the physics state at time t.
    /// @param input the current input data.
    /// @param planes the set of all collision planes in the scene.
    /// @param state the physics state of the cube.

	static Derivative evaluate(const Input &input, const std::vector<Plane> &planes, const State &state)
	{
		Derivative output;
		output.velocity = state.velocity;
		output.spin = state.spin;
		forces(input, planes, state, output.force, output.torque);
		return output;
	}
	
    /// Evaluate derivative values for the physics state at future time t+dt 
    /// using the specified set of derivatives to advance dt seconds from the 
    /// specified physics state.
    /// @param input the current input data.
    /// @param planes the set of all collision planes in the scene.
    /// @param state the physics state of the cube at the start of the timestep.
    /// @param dt the time in seconds to advance forward.
    /// @param derivative the set of derivative values to use to advance forward in time from state.

	static Derivative evaluate(const Input &input, const std::vector<Plane> &planes, State state, float dt, const Derivative &derivative)
	{
		state.position += derivative.velocity * dt;
		state.momentum += derivative.force * dt;
		state.orientation += derivative.spin * dt;
		state.angularMomentum += derivative.torque * dt;
		state.recalculate();
		
		Derivative output;
		output.velocity = state.velocity;
		output.spin = state.spin;
		forces(input, planes, state, output.force, output.torque);
		return output;
	}

    /// Integrate physics state forward by dt seconds.
    /// Uses an RK4 integrator to accurately numerically integrate with error O(5).
    /// This involves evaluating derivatives at multiple points in the timestep
    /// using the Cube::evaluate method then updating the primary state values as 
    /// a weighted sum of these values then finally recalculating secondary state.

	static void integrate(const Input &input, const std::vector<Plane> &planes, State &state, float dt)
	{
		Derivative a = evaluate(input, planes, state);
		Derivative b = evaluate(input, planes, state, dt*0.5f, a);
		Derivative c = evaluate(input, planes, state, dt*0.5f, b);
		Derivative d = evaluate(input, planes, state, dt, c);
		
		state.position += 1.0f/6.0f * dt * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);
		state.momentum += 1.0f/6.0f * dt * (a.force + 2.0f*(b.force + c.force) + d.force);
		state.orientation += 1.0f/6.0f * dt * (a.spin + 2.0f*(b.spin + c.spin) + d.spin);
		state.angularMomentum += 1.0f/6.0f * dt * (a.torque + 2.0f*(b.torque + c.torque) + d.torque);
		state.recalculate();
	}	

    /// Calculate force and torque for physics state at time t.
    /// Due to the way that the RK4 integrator works we need to calculate
    /// force implicitly from state rather than explictly applying forces
    /// to the rigid body once per update. This is because the RK4 achieves
    /// its accuracy by detecting curvature in derivative values over the 
    /// timestep so we need our force values to supply the curvature.

	static void forces(const Input &input, const std::vector<Plane> &planes, const State &state, Vector &force, Vector &torque)
	{
		force.zero();
		torque.zero();
		
		gravity(force);
        damping(state, force, torque);
        collision(planes, state, force, torque);
        control(input, state, force, torque);

        assert(force==force);
        assert(torque==torque);
	}

    /// Calculate gravity force.
    /// @param force the force accumulator.

	static void gravity(Vector &force)
	{
		force.y -= 9.8f;
	}
	
    /// Calculate a simple linear and angular damping force.
    /// This roughly simulates energy loss due to heat dissipation
    /// or air resistance or whatever you like.
    /// @param state the current cube physics state.
    /// @param force the force accumulator.
    /// @param torque the torque accumulator.

	static void damping(const State &state, Vector &force, Vector &torque)
	{
		const float linear = 0.001f;
		const float angular = 0.001f;
		
		force -= linear * state.velocity;
		torque -= angular * state.angularVelocity;
	}

    /// Calculate collision response force and torque.
    ///
    /// This is a very basic collision response implemented at the force
    /// level by simply checking each vertex of the cube against each plane
    /// in the scene. For each cube vertex that is inside a plane a set of
    /// penalty forces and friction forces are applied to simulate collision
    /// response. See Cube::collisionForPoint for details.
    ///
    /// @param planes the set of collision planes in the scene.
    /// @param state the current cube physics state.
    /// @param force the force accumulator.
    /// @param torque the torque accumulator.

    static void collision(const std::vector<Plane> &planes, const State &state, Vector &force, Vector &torque)
	{
		Vector a = state.bodyToWorld * (Vector(-1,-1,-1) * state.size * 0.5);
		Vector b = state.bodyToWorld * (Vector(+1,-1,-1) * state.size * 0.5);
		Vector c = state.bodyToWorld * (Vector(+1,+1,-1) * state.size * 0.5);
		Vector d = state.bodyToWorld * (Vector(-1,+1,-1) * state.size * 0.5);
		Vector e = state.bodyToWorld * (Vector(-1,-1,+1) * state.size * 0.5);
		Vector f = state.bodyToWorld * (Vector(+1,-1,+1) * state.size * 0.5);
		Vector g = state.bodyToWorld * (Vector(+1,+1,+1) * state.size * 0.5);
		Vector h = state.bodyToWorld * (Vector(-1,+1,+1) * state.size * 0.5);
		
		for (unsigned int i=0; i<planes.size(); i++)
		{
			collisionForPoint(state, force, torque, a, planes[i]);
			collisionForPoint(state, force, torque, b, planes[i]);
			collisionForPoint(state, force, torque, c, planes[i]);
			collisionForPoint(state, force, torque, d, planes[i]);
			collisionForPoint(state, force, torque, e, planes[i]);
			collisionForPoint(state, force, torque, f, planes[i]);
			collisionForPoint(state, force, torque, g, planes[i]);
			collisionForPoint(state, force, torque, h, planes[i]);
		}
	}
	
    /// Calculate collision response force and torque for a point against a plane.
    ///
    /// If the point is inside the plane then a penalty force is applied to push
    /// the point out. A damping force is also applied to make the collision inelastic
    /// otherwise the cube would bounce off plane without losing any energy.
    ///
    /// Velocity constraint forces are also applied when the point is inside the plane
    /// and the point is moving further into the plane. This tightens up the collision
    /// response from what would be achieved using penetration depth penalty forces
    /// alone giving a more realistic result.
    ///
    /// An approximation of tangential friction force is also applied. This is not
    /// true coulomb friction which would be proportional to the normal force between
    /// the two objects, instead it is more of a rolling type friction proportional
    /// to the tangential velocity between the two surfaces. This gives basically
    /// correct effects.
    ///
    /// Finally please note that this collision response is very basic. The correct
    /// way to implement this would be to develop a solver which could simultaneously
    /// satisify a number of constaints. The tradeoff made here is that we allow some
    /// softness in the collision response to make the calculations easier. This
    /// small amount of give during collision lets us calculate collision response
    /// easily without needing a complicated solver and without the jitter that you
    /// normally see in an impulse based collision response.
    ///
    /// @param state the current cube physics state.
    /// @param force the force accumulator.
    /// @param torque the torque accumulator.
    /// @param point the point to check for collision.
    /// @param plane the collision plane.

    static void collisionForPoint(const State &state, Vector &force, Vector &torque, const Vector &point, const Plane &plane)
	{
		const float c = 10;
		const float k = 100;
		const float b = 5;
		const float f = 3;
		
		const float penetration = plane.constant - point.dot(plane.normal);
		
		if (penetration>0)
		{
			Vector velocity = state.angularVelocity.cross(point-state.position) + state.velocity;
            assert(velocity==velocity);

			const float relativeSpeed = - plane.normal.dot(velocity);
            assert(relativeSpeed==relativeSpeed);
			
			if (relativeSpeed>0)
			{
				Vector collisionForce = plane.normal * (relativeSpeed * c);
                assert(collisionForce==collisionForce);
				force += collisionForce;
				torque += (point-state.position).cross(collisionForce);
            }
			
            Vector tangentialVelocity = velocity + (plane.normal * relativeSpeed);
            Vector frictionForce = - tangentialVelocity * f;
            assert(frictionForce==frictionForce);
            force += frictionForce;
            torque += (point-state.position).cross(frictionForce);

            Vector penaltyForce = plane.normal * (penetration * k);
            assert(penaltyForce==penaltyForce);
			force += penaltyForce;
			torque += (point-state.position).cross(penaltyForce);
			
			Vector dampingForce = plane.normal * (relativeSpeed * penetration * b);
            assert(dampingForce==dampingForce);
			force += dampingForce;
			torque += (point-state.position).cross(dampingForce);
		}
	}

    /// Control forces

    static void control(const Input &input, const State &state, Vector &force, Vector &torque)
    {
        const float f = 50.0f;

        if (input.left)
            force.x -= f;

        if (input.right)
            force.x += f;

        if (input.forward)
            force.z -= f;

        if (input.back)
            force.z += f;

        if (input.jump && state.velocity.y>=-0.1f)
        {
            const float j = 20;
            const float k = 5;

            const float difference = j - state.velocity.y;

            Vector a = state.bodyToWorld * (Vector(-1,-1,-1) * state.size * 0.5);
            Vector b = state.bodyToWorld * (Vector(+1,-1,-1) * state.size * 0.5);
            Vector c = state.bodyToWorld * (Vector(+1,+1,-1) * state.size * 0.5);
            Vector d = state.bodyToWorld * (Vector(-1,+1,-1) * state.size * 0.5);
            Vector e = state.bodyToWorld * (Vector(-1,-1,+1) * state.size * 0.5);
            Vector f = state.bodyToWorld * (Vector(+1,-1,+1) * state.size * 0.5);
            Vector g = state.bodyToWorld * (Vector(+1,+1,+1) * state.size * 0.5);
            Vector h = state.bodyToWorld * (Vector(-1,+1,+1) * state.size * 0.5);

            float lowest = a.y;
            if (b.y<lowest) lowest = b.y;
            if (c.y<lowest) lowest = c.y;
            if (d.y<lowest) lowest = d.y;
            if (e.y<lowest) lowest = e.y;
            if (f.y<lowest) lowest = f.y;
            if (g.y<lowest) lowest = g.y;
            if (h.y<lowest) lowest = h.y;

            if (difference>0 && lowest<0.05f)
                force.y += difference * k;
        }
    }
};
