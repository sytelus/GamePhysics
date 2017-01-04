/// Text element.
/// Manages rendering text on the screen with alpha blend in/out

struct Text
{
    std::string text;
	freetype::font_data *font;
    float x,y;
    float r,g,b;
    bool visible;

    /// default constructor

    Text()
    {
		font = 0;

        x = 0.0f;
        y = 0.0f;

        r = 1.0f;
        g = 1.0f;
        b = 1.0f;
        a = 0.0f;
        
        visible = false;
    }

    /// update text

    void update(unsigned int t)
    {
        if (visible)
            a += (1.0f - a) * 0.05f;
        else
            a += (0.0f - a) * 0.1f;
    }

    /// render text

    void render(float parentAlpha = 1.0f)
    {
		if (!font)
			return;

        if (a>0.0001f && parentAlpha>0.0001f)
        {
			// render text normally

			glColor4f(r,g,b,a*parentAlpha);
			freetype::print(*font, x, y, text.c_str());
        }
    }

    /// render text with line breaks

    int renderWithLineBreaks( int charactersPerLine,
							  float offset,
						      float horizonalSpacing,
							  float verticalSpacing,
							  float parentAlpha = 1.0f )
    {
		if (!font)
			return 0;

		float cx = x;
		float cy = y + offset;

		int characters = 0;
		int lines = 0;

        if (a>0.0001f && parentAlpha>0.0001f)
		{
			glColor4f(r,g,b,a*parentAlpha);

			char buffer[1024];
			strncpy(buffer, text.c_str(), sizeof(buffer));
			char *token = strtok(buffer, " ");
			while (token)
			{
				const int wordLength = (int) strlen(token) - 1;

				if (characters+wordLength>charactersPerLine-1)
				{
					cx = x;
					cy += verticalSpacing;
					characters = 0;
					lines ++;
				}

				freetype::print(*font, cx, cy, token);

				cx += horizonalSpacing * (wordLength+2);
				characters += wordLength + 2;

				token = strtok(0, " ");
			}
        }

		return lines;
    }

private:

    float a;        ///< alpha value used to fade in/out smoothly following visible flag
};
