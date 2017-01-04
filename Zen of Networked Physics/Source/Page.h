/// Page.
/// One page of the presentation: title and bullet point items.

struct Page
{
    float x1,y1,x2,y2;
	int charactersPerLine;
        
    /// default constructor

    Page()
    {
        x1 = 0.0f;
        y1 = 0.0f;
        x2 = 0.0f;
        y2 = 0.0f;
		charactersPerLine = 0;
    }

    /// initialize page

    void initialize( const std::string &title, const std::vector<std::string> &items, 
					 float x1, float y1, float x2, float y2, int charactersPerLine )
    {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
		this->charactersPerLine = charactersPerLine;

        // setup title text layout

		const float titleHeight = 27;
        const float textHeight = 15;
        const float textSpacing = 10;

        float x = x1;
        float y = y1 + titleHeight;

        titleText.text = title;
        titleText.font = &font.title;
        titleText.x = x;
        titleText.y = y;
        titleText.r = 1;
        titleText.g = 0.2f;
        titleText.b = 0.1f ;
        titleText.visible = true;

        y += textHeight + textSpacing * 4;
        
        // setup items text layout

        itemText.resize(items.size());

        for (unsigned int i=0; i<items.size(); i++)
        {
            itemText[i].text = items[i];
            itemText[i].font = &font.items;
            itemText[i].x = x;
            itemText[i].y = y;
            itemText[i].r = 0;
            itemText[i].g = 0;
            itemText[i].b = 0;
            itemText[i].visible = true;

            y += textHeight + textSpacing * 3;
        }
    }

    /// update page

    void update(unsigned int t)
    {
        titleText.update(t);
        for (unsigned int i=0; i<itemText.size(); i++)
            itemText[i].update(t);
    }

    /// render page

    void render(float parentAlpha = 1.0f)
    {
        glScissor((int)x1, (int)y1, (int)(x2-x1), (int)(y2-y1));

		const float horizontalSpacing = 12;
		const float verticalSpacing = 18;

		float offset = 0;

        titleText.render(parentAlpha);
        for (unsigned int i=0; i<itemText.size(); i++)
            offset += itemText[i].renderWithLineBreaks(charactersPerLine, offset, horizontalSpacing, verticalSpacing, parentAlpha) * verticalSpacing;

        glScissor(0, 0, displayWidth, displayHeight);
    }

private:

    Text titleText;
    std::vector<Text> itemText;
};
