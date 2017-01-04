/// Client.
/// Represents the local scene on the client.
/// The client is complex because it implements client side prediction
/// and correction, eg. 'correction in the past' via the history buffer.
/// Otherwise this is a simple scene driven from the keyboard input of the
/// player.
/// Press F1 to toggle visualization of the client cube.

struct Client : public Scene
{
    /// default constructor.

    Client()
    {
        log("client.log");

        smoothed.r = 0.8f;
        smoothed.g = 0.4f;
        smoothed.b = 0.3f;
    }

    void update(unsigned int t)
    {
        // add to history

        Move move;
        move.time = t;
        move.input = input;
        move.state = cube.state();

        history.add(move);

        // update scene

        Scene::update(t);
    }

    /// synchronize client with server

    void synchronize(unsigned int t, const Cube::State &state, const Cube::Input &input)
    {
        Cube::State original = cube.state();

        history.correction(*this, t, state, input);

        if (original.compare(cube.state()))
            smooth();
    }

    History history;        ///< client side history of moves
};
