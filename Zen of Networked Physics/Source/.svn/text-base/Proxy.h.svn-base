/// Proxy
/// Represents another client's view of the scene on the server,
/// or alternatively, the 'round trip' view of the simulation sent back
/// to the client from the server.
/// Updates time normally as per a client scene, however each time a synchornize
/// event is recevied from the server, the state and input is snapped to the values
/// sent from the server. Smoothing is used to even out snaps and pops caused by
/// packet loss.
/// Press F3 while running to toggle visualization of the proxy cube.

struct Proxy : public Scene
{
    /// default constructor.

    Proxy()
    {
        log("proxy.log");

        cube.a = 0.15f;

        smoothed.r = 0.4f;
        smoothed.g = 0.3f;
        smoothed.b = 0.8f;

        lastSyncTime = 0;
        updating = false;
    }

	/// synchronize event sent from server back to client

    void synchronize(unsigned int t, const Cube::State &state, const Cube::Input &input)
    {
        if (t<lastSyncTime)
            return;

        lastSyncTime = t;
        updating = true;

        // set proxy input

        this->input = input;

        // correct if significantly different

        if (state.compare(cube.state()))
        {
            cube.snap(state);
            smooth();
        }
    }

	/// update proxy simulation.
	/// note that unlike the server simulation, the proxy does not operate in 'lock step', it runs
	/// on its own concept of time *approximating* the server physics with snapping when errors occur.

    void update(unsigned int t)
    {
        if (updating)
            Scene::update(t);
    }

private:

    unsigned int lastSyncTime;
    bool updating;
};
