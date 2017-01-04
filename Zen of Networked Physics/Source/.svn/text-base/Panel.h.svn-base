/// Panel.
/// Provides a nice panel overlay over existing rendering which can fade in/out.
struct Panel
{
    float x1,y1,x2,y2;
    float border;
    float header;
    float spacing;
    float r,g,b;
    float opacity;
    bool visible;

    /// default constructor

    Panel()
    {
        x1 = 0.0f;
        y1 = 0.0f;
        x2 = 0.0f;
        y2 = 0.0f;

        border = 5;
        header = 75;
        spacing = 30;

        r = 1.0f;
        g = 1.0f;
        b = 1.0f;
        a = 0.0f;

        opacity = 0.9f;

        visible = false;
    }

    /// initialize panel

    void initialize(float x1, float y1, float x2, float y2)
    {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;

        // load pages

		load();
    }

	/// load panel pages from disk

	void load()
	{
        loadPages("content.txt");
		if (currentPage>=(int)pages.size())
			currentPage = (int)pages.size() - 1;
	}

    /// update panel

    void update(unsigned int t)
    {
        if (visible)
            a += (opacity - a) * 0.05f;
        else
            a += (0.0f - a) * 0.1f;

        for (unsigned int i=0; i<pages.size(); i++)
            pages[i].update(t);
    }

    /// render text

    void render()
    {
        if (a<=0.0001f)
            return;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // render screen darkening quad

        glBegin(GL_QUADS);
        glColor4f(0, 0, 0, a*0.3f);
        glVertex2f(0,0);
        glVertex2f(0,(float)displayHeight);
        glVertex2f((float)displayWidth,(float)displayHeight);
        glVertex2f((float)displayWidth,0);
        glEnd();

        // render panel

        glBegin(GL_QUADS);
        glColor4f(r, g, b, a*0.8f);
        glVertex2f(x1,y2);
        glVertex2f(x2,y2);
        glColor4f(r, g, b, a);
        glVertex2f(x2,y1);
        glVertex2f(x1,y1);
        glEnd();

        // render border

        glBegin(GL_QUADS);
        glColor4f(0,0,0,a);
        quad(x1-border, y1-border, x2, y1);
        quad(x2, y1-border, x2+border, y2);
        quad(x1, y2, x2+border, y2+border);
        quad(x1-border, y1, x1, y2+border);
        glEnd();

        // render page

        if (pages.size())
            pages[currentPage].render(a);

        glDisable(GL_BLEND);
    }

    void nextPage()
    {
        if (currentPage<(int)pages.size()-1)
            currentPage++;
    }

    void previousPage()
    {
        if (currentPage>0)
            currentPage--;
    }

private:

    /// load pages from text file

    void loadPages(const char filename[])
    {
		const charactersPerLine = 41;

        pages.resize(0);

        FILE *file = fopen(filename, "r+t");

        if (!file)
            return;

        int page = -1;

        std::string title;
        std::vector<std::string> items;

        char buffer[1024];

        while (fgets(buffer, sizeof(buffer), file))
        {
            if (buffer[0]=='*')
            {
                // initialize previous page

                if (page>=0)
                    pages[page].initialize(title, items, x1+spacing, y1+spacing, x2-spacing ,y2-spacing, charactersPerLine);

                // start gathering data for next page

                title = std::string(&buffer[1]);
				title.resize(title.size()-1);		// chomp newline
                items.clear();

                pages.resize(pages.size()+1);
                page ++;
            }
            else
            {
				// ignore comment lines (blah i will do this etc)

				if (buffer[0]=='(')
					continue;

                // gather non-whitespace lines of text as bullet point items

                int i = 0;
                while (buffer[i]!=0)
                {
                    if (!isspace(buffer[i]))
                    {
						std:: string item = std::string(buffer);
						item.resize(item.size()-1);		// chomp newline
                        items.push_back(item);
                        break;
                    }
                    i++;
                }
            }
        }

        if (page>=0)
            pages[page].initialize(title, items, x1+spacing, y1+spacing, x2-spacing ,y2-spacing, charactersPerLine);

        fclose(file);
    }

    int currentPage;            ///< index of current page
    std::vector<Page> pages;    ///< array of presentation pages

    float a;                    ///< alpha value used to fade in/out smoothly following visible flag
};
