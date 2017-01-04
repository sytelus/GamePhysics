// Font manager

struct Font
{
	/// initialize fonts.
	/// OpenGL must be fully initialized before calling this method.

	void initialize()
	{
		char system[1024];
		GetWindowsDirectory(system, sizeof(system));
		
		char arial[2048];
		sprintf(arial, "%s/fonts/arial.ttf", system);
		title.init(arial, 25);
		
		char courier[2048];
		sprintf(courier, "%s/fonts/courbd.ttf", system);
		items.init(courier, 15);
		status.init(courier, 15);
	}

	/// cleanup fonts

	~Font()
	{
        title.clean();
        items.clean();
        status.clean();
	}

	freetype::font_data title;			///< page title
	freetype::font_data items;			///< page item text
	freetype::font_data status;			///< status text (50% packet loss etc.)
};
