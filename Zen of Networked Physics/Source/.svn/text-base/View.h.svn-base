/// View class.
/// Responsible for managing lights, rendering the scene etc.

struct View
{
    /// default constructor.

    View()
    {
        client = 0;
        server = 0;
        proxy = 0;
    }

    void initialize(Client &client, Server &server, Proxy &proxy)
    {
        // initialize scenes

        client.initialize();
        server.initialize();
        proxy.initialize();

        this->client = &client;
        this->server = &server;
        this->proxy = &proxy;

        // setup light

        glEnable(GL_LIGHT0);
        glShadeModel(GL_SMOOTH);

        GLfloat lightAmbientColor[] = { 0.5, 0.5, 0.5, 1.0 };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbientColor);

        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01f);

        GLfloat lightPosition[] = { 25.0, 100.0, 25.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        light.x = lightPosition[0];
        light.y = lightPosition[1];
        light.z = lightPosition[2];

        // setup text

        packetLoss.text = "x% packet loss";
		packetLoss.font = &font.status;
        packetLoss.x = 20.0f;
        packetLoss.y = 30.0f;

        latency.text = "x milliseconds latency";
		latency.font = &font.status;
        latency.x = 20.0f;
        latency.y = 50.0f;

        importantMoves.text = "sending important moves";
		importantMoves.font = &font.status;
        importantMoves.r = 1.0f;
        importantMoves.g = 0.7f;
        importantMoves.b = 0.1f;
        importantMoves.x = 20.0f;
        importantMoves.y = 70.0f;

        // initialize panel

        panel.initialize(120, 30, 680, 570);

        // default render flags

        renderClient = true;
        renderServer = false;
        renderProxy = false;
        renderHistory = false;
        renderSmoothedClient = false;
        renderSmoothedProxy = false;
    }

    void update(unsigned int t)
    {
        packetLoss.update(t);
        latency.update(t);
        importantMoves.update(t);
        panel.update(t);
    }

    void render(float alpha = 1.0f)
    {
        // clear color, depth and stencil buffers

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // render ground plane

        const float d = 100;
        const float h = -0.025f;

        glBegin(GL_QUADS);
        glColor3f(80.0f/255.0f, 99.0f/255.0f, 123.0f/255.0f);
        glVertex3f(-d,h,+d);
        glVertex3f(+d,h,+d);
        glVertex3f(+d,h,-d);
        glVertex3f(-d,h,-d);
        glEnd();

		// render various scene elements

		if (renderHistory)
			client->history.render();

		if (renderSmoothedProxy)
			proxy->smoothed.render(light, alpha);

		if (renderSmoothedClient)
			client->smoothed.render(light, alpha);

		if (renderClient)
			client->cube.render(light, alpha);

		if (renderServer)
			server->cube.render(light, alpha);

		if (renderProxy)
			proxy->cube.render(light, alpha);

		// render shadow overlay quad

		// todo: render in screenspace

		glEnable(GL_STENCIL_TEST);

		glStencilFunc(GL_NOTEQUAL, 0x0, 0xff);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);

		glColor4f(0.25f, 0.25f, 0.25f, 0.5f);

		const float s = 10;
		glVertex3f(-s, -s, 7);
		glVertex3f(+s, -s, 7);
		glVertex3f(+s, +s, 7);
		glVertex3f(-s, +s, 7);

		glEnd();

		glDisable(GL_BLEND);

		glDisable(GL_STENCIL_TEST);        

        // begin screen space rendering

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        enterScreenSpace();

		// render status text

		packetLoss.render();
		latency.render();
		importantMoves.render();

        // render panel

        panel.render();

        // leave screen space

        leaveScreenSpace();

        glDisable(GL_BLEND);
    }

    // rendering flags

    bool renderClient;
    bool renderServer;
    bool renderProxy;
    bool renderHistory;
    bool renderSmoothedClient;
    bool renderSmoothedProxy;

    // text objects

    Text packetLoss;
    Text latency;
    Text importantMoves;

    // panel for presentation

    Panel panel;

private:

    Client *client;
    Server *server;
    Proxy *proxy;

    Vector light;                   ///< the light vector for the scene (just one light for now)
};
