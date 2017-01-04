/// Server.
/// The authoritative scene on the server.
/// The cube in this scene is driven by updates sent from the client
/// containing current client time and input. The server then advances its own
/// physics simulation ahead up to the most recent time sent from the client.
/// Press F2 to toggle visualization of the server cube.

struct Server : public Scene
{
    /// default constructor.

    Server()
    {
        log("server.log");
        cube.a = 0.45f;
        useImportantMoves = false;
    }

    /// update server physics with input

    void update(unsigned int t, Cube::Input &input, const std::vector<Move> &importantMoves)
    {
        // work around packet loss with important moves

        if (useImportantMoves)
        {
            for (unsigned int i=0; i<importantMoves.size(); i++)
            {
                const Move &move = importantMoves[i];

                while (time<move.time)
                    Scene::update(time);

                this->input = move.input;
            }
        }

        // update to time t

        while (time<t)
            Scene::update(time);

        this->input = input;
    }

    /// simulate a snap on the server for testing

    void snap()
    {
        Cube::State state = cube.state();
        
        state.position += Vector(1,0,0);

        for (unsigned int i=0; i<planes.size(); i++)
            planes[i].clip(state.position, 0.5f);

        cube.snap(state);
    }

    bool useImportantMoves;         ///< if true then server will use important moves to work around packet loss.
};
