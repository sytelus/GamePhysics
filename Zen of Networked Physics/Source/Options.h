/// Options

struct Options : public Input::Listener
{
    /// constructor

    Options()
    {
        defaults();

        #ifdef LOGGING
        logfile = fopen("input.log", "w");
        #endif
    }

    /// destructor

    ~Options()
    {
        #ifdef LOGGING
        if (logfile)
        {
            fclose(logfile);
            logfile = 0;
        }
        #endif
    }

    void update(float t, float dt)
    {
        // update visibility

        view.renderClient = renderClient;
        view.renderServer = renderServer;
        view.renderProxy = renderProxy;
        view.renderHistory = renderHistory;
        view.renderSmoothedClient = renderSmoothedClient;
        view.renderSmoothedProxy = renderSmoothedProxy;

        // handle latency controls

        switch (latency)
        {
            case NoLatency: 
                connection.latency = 0.0f; 
                break;

            case FiftyMillisecondsLatency: 
                connection.latency = 50.0f/1000.0f * 0.5f; 
                break;

            case TwoHundredMillisecondsLatency: 
                connection.latency = 200.0f/1000.0f * 0.5f; 
                break;

            case TwoSecondsLatency: 
                connection.latency = 1.0f; 
                break;
        }

        // handle packet loss controls

        switch (packetLoss)
        {
            case NoPacketLoss: 
                connection.packetLoss = 0.0f; 
                break;

            case FivePercentPacketLoss: 
                connection.packetLoss = 5.0f;
                break;

            case TenPercentPacketLoss:
                connection.packetLoss = 10.0f; 
                break;

            case FiftyPercentPacketLoss: 
                connection.packetLoss = 50.0f; 
                break;
        }

        // update text visiblitiy

        if (connection.packetLoss>0.0f || connection.latency>0.0f)
        {
            view.packetLoss.visible = true;
            view.latency.visible = true;
        }
        else
        {
            view.packetLoss.visible = false;
            view.latency.visible = false;
        }

        // update packet loss text output

        if (view.packetLoss.visible)
        {
            char buffer[256];

            if (connection.packetLoss<=10.0)
                sprintf(buffer, "%d%% packet loss", (int) connection.packetLoss);
            else
                sprintf(buffer, "%d%% packet loss!", (int) connection.packetLoss);

            view.packetLoss.text = buffer;
        }

        // update latency text output

        if (view.latency.visible)
        {
            const int milliseconds = (int) (connection.latency * 2.0f * 1000.0f);

            char buffer[256];

            if (milliseconds<=500)
                sprintf(buffer, "%d milliseconds latency", milliseconds);
            else
                sprintf(buffer, "%d milliseconds latency!", milliseconds);

            view.latency.text = buffer;
        }

        // update important moves text output

        if (server.useImportantMoves && view.packetLoss.visible)
            view.importantMoves.visible = true;
        else
            view.importantMoves.visible = false;
    }

    void pressed(Key key)
    {
        switch (key)
        {
            case Esc:
                view.panel.visible = !view.panel.visible;
                break;

            case PageUp:
                view.panel.previousPage();
                break;

            case PageDown:
                view.panel.nextPage();
                break;

            case F1:
                renderClient = !renderClient;
                break;

            case F2:
                renderServer = !renderServer;
                break;

            case F3:
                renderProxy = !renderProxy;
                break;

            case F4:
                renderHistory = !renderHistory;
                break;

            case F5:
                renderSmoothedClient = !renderSmoothedClient;
                break;

            case F6:
                renderSmoothedProxy = !renderSmoothedProxy;
                break;

            case F7:
                latency++;
                if (latency>=LatencyCount)
                    latency = 0;
                break;

            case F8:
                packetLoss++;
                if (packetLoss>=PacketLossCount)
                    packetLoss = 0;
                break;

            case F9:
                server.useImportantMoves = !server.useImportantMoves;
                break;

            case Control:
                server.snap();
                break;

            case Enter:
				if (view.panel.visible)
				{
					// reload pages
					view.panel.load();
				}
                break;
        }
    }

    void defaults()
    {
        renderClient = true;
        renderServer = false;
        renderProxy = false;
        renderHistory = false;
        
        renderSmoothedClient = false;
        renderSmoothedProxy = false;

        packetLoss = NoPacketLoss;
        latency = NoLatency;
    }

    bool renderClient;
    bool renderServer;
    bool renderProxy;
    bool renderHistory;

    bool renderSmoothedClient;
    bool renderSmoothedProxy;

    enum PacketLoss
    {
        NoPacketLoss,
        FivePercentPacketLoss,
        TenPercentPacketLoss,
        FiftyPercentPacketLoss,
        PacketLossCount
    };

    int packetLoss;

    enum Latency
    {
        NoLatency,
        FiftyMillisecondsLatency,
        TwoHundredMillisecondsLatency,
        TwoSecondsLatency,
        LatencyCount
    };

    int latency;

private:

    FILE *logfile;
};
