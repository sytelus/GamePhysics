/// Scene class.
/// Represents the scene managing objects and collision geometry.

const float defaultTightness = 0.25f;
const float smoothTightness = 0.1f;

struct Scene
{
    /// default constructor.

    Scene()
    {
        // clear input to defaults

        input.left = false;
        input.right = false;
        input.forward = false;
        input.back = false;
        input.jump = false;

        // defaults

        logfile = 0;
        replaying = false;
        tightness = defaultTightness;

        // start simulation at t=0

        time = 0;
    }

    /// destructor.

    ~Scene()
    {
        #ifdef LOGGING
        if (logfile)
        {
            fclose(logfile);
            logfile = 0;
        }
        #endif
    }

    /// Initialize the scene.
    /// Must be called after OpenGL is initialized so that it can extract
    /// the clip planes for the view frustum.

	void initialize()
	{
        // setup collision planes in scene

		Plane left, right, bottom, top, front, back;
		calculateFrustumPlanes(left, right, bottom, top, front, back);

        front.constant += 2.0f;
		
        Plane ramp = Plane(Vector(0,1,-1).unit(), Vector(0,0,5));

		planes.push_back(left);
		planes.push_back(right);
		planes.push_back(front);
		planes.push_back(back);
        planes.push_back(ramp);

        Plane floor(Vector(0,1,0), 0);
        planes.push_back(floor);
    }

    void log(const char filename[])
    {
        #ifdef LOGGING
        logfile = fopen(filename, "w");
        #endif
    }

    /// Update the scene from integer time t to t+1.

	void update(unsigned int t)
	{
        // log for comparison

        #ifdef LOGGING
        if (logfile && !replaying)
        {
            Vector position = cube.state().position;
            Quaternion orientation = cube.state().orientation;
            fprintf(logfile, "%d: position=(%f,%f,%f), orientation=(%f,%f,%f,%f), input=(%d,%d,%d,%d,%d)\n", t, position.x, position.y, position.z, orientation.w, orientation.x, orientation.y, orientation.z, input.left, input.right, input.forward, input.back, input.jump);
        }
        #endif

        // time step

        cube.update(input, planes, timestep);

        // update smoothed cube

        if (!replaying)
            smoothed.smooth(cube.state(), tightness);

        // update smoothing tightness value for adaptive smoothing

        tightness += (defaultTightness - tightness) * 0.01f;

        // advance t

        time ++;
    }

    /// call this method when a snap occurs to smooooooth it out baby

    void smooth()
    {
        tightness = smoothTightness;
    }

public:

    unsigned int time;              ///< current scene time.

	Cube cube;                      ///< the cube object.
    Cube::Input input;              ///< current input for the cube.

    Cube smoothed;                  ///< smoothed cube following main cube.

    std::vector<Plane> planes;      ///< the set of collision planes in the scene.

    FILE *logfile;                  ///< file handle for logging (i diff logs to check sync)

    bool replaying;                 ///< true if currently replaying moves (client side correction)

    float tightness;                ///< current smoothing tightness
};
