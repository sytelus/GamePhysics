/// Connection object.
/// A simulated network connection with adjustable latency and packet loss.
/// This allows me to develop the networked physics code while having complete
/// control over the data transmission properties.
/// Effectively this object simulates a two way connection from client to server,
/// the client sends a stream of input to the server, while the server sends a stream
/// of corrections back to the client.

class Connection
{
public:

    float latency;          ///< each way latency in seconds
    float packetLoss;       ///< percentage of packets lost

    Connection()
    {
        // defaults

        client = 0;
        server = 0;
        proxy = 0;

        latency = 0.0f;
        packetLoss = 0.0f;
        
        time = 0;

        #ifdef LOGGING
        logfile = fopen("sync.log", "w");
        #endif
    }

    ~Connection()
    {
        #ifdef LOGGING
        if (logfile)
        {
            fclose(logfile);
            logfile = 0;
        }
        #endif
    }          

    void initialize(Client &client, Server &server, Proxy &proxy)
    {
        this->client = &client;
        this->server = &server;
        this->proxy = &proxy;
    }

    void update(unsigned int t)
    {
        // update time

        time = t;

        // process event queues

        process(clientToServer);
        process(serverToClient);

        // send input event to server

        InputEvent *event = new InputEvent();

        event->time = client->time;
        event->input = client->input;
        client->history.importantMoveArray(event->importantMoves);
        insert(clientToServer, event);

        // step ahead

        time ++;
    }

protected:

    /// input event recieved on server side

    void input(unsigned int t, Cube::Input &input, const std::vector<Move> &importantMoves)
    {
        // update server with input

        server->update(t, input, importantMoves);

        // send sync event back to client side

        SyncEvent *event = new SyncEvent();
        event->time = server->time;
        event->state = server->cube.state();
        event->input = input;
        insert(serverToClient, event);

        #ifdef LOGGING
        if (logfile)
        {
            Vector position = event->state.position;
            Quaternion orientation = event->state.orientation;
            Cube::Input input = event->input;
            fprintf(logfile, "%d: position=(%f,%f,%f), orientation=(%f,%f,%f,%f), input=(%d,%d,%d,%d,%d)\n", event->time, position.x, position.y, position.z, orientation.w, orientation.x, orientation.y, orientation.z, input.left, input.right, input.forward, input.back, input.jump);
        }
        #endif
    }

    /// synchronize event received on client side

    void synchronize(unsigned int t, const Cube::State &state, const Cube::Input &input)
    {
        client->synchronize(t, state, input);
        proxy->synchronize(t, state, input);
    }

private:

    struct Event
    {
        unsigned int deliveryTime;
        virtual void execute(Connection &connection) = 0;
    };

    struct InputEvent : public Event
    {
        unsigned int time;
        Cube::Input input;
        std::vector<Move> importantMoves;
        void execute(Connection &connection)
        {
            connection.input(time, input, importantMoves);
        }
    };

    struct SyncEvent : public Event
    {
        unsigned int time;
        Cube::State state;
        Cube::Input input;
        void execute(Connection &connection)
        {
            connection.synchronize(time, state, input);
        }
    };

    typedef std::queue<Event*> EventQueue;

    EventQueue clientToServer;
    EventQueue serverToClient;

    /// insert an event into the queue

    void insert(EventQueue &queue, Event *event)
    {
        assert(event);
        event->deliveryTime = time + (unsigned int) (latency/timestep);
        queue.push(event);
    }

    /// process event queue and execute events ready for delivery

    void process(EventQueue &queue)
    {
        while (queue.size())
        {
            Event *event = queue.front();

            if (event->deliveryTime<=time)
            {
                if (!chance(packetLoss))
                    event->execute(*this);

                queue.pop();

                delete event;
            }
            else
                break;
        }
    }

    /// check if an event happens given a percentage frequency of occurance

    bool chance(float percent)
    {
        const float value = rand() / (float) RAND_MAX * 100;
        return value <= percent;
    }

    Client *client;
    Server *server;
    Proxy *proxy;

    FILE *logfile;

    unsigned int time;
};
