// Simple Windows OpenGL framework

#ifdef _WIN32

#define VC_EXTRALEAN 
#define WIN32_LEAN_AND_MEAN 

#include <windows.h>
#include <windowsx.h>

#include <gl/gl.h>
#include <gl/glu.h>

HWND window = 0;
HDC device = 0;
HGLRC context = 0;
bool active = true;

int displayWidth = 0;
int displayHeight = 0;
bool displayFullscreen = false;

GLuint fontBase = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
            ValidateRect(hWnd, NULL);
            break;

        case WM_KEYDOWN:
            switch (wParam&0xFF)
            {
                case VK_LEFT:     onKeyDown(Left);     break;
                case VK_RIGHT:    onKeyDown(Right);    break;
                case VK_UP:       onKeyDown(Up);       break;
                case VK_DOWN:     onKeyDown(Down);     break;
                case VK_SPACE:    onKeyDown(Space);    break;
                case VK_RETURN:   onKeyDown(Enter);    break;
                case VK_CONTROL:  onKeyDown(Control);  break;
                case VK_ESCAPE:   onKeyDown(Esc);      break;
                case 33:          onKeyDown(PageUp);   break;
                case 34:          onKeyDown(PageDown); break;
                case VK_F1:       onKeyDown(F1);       break;
                case VK_F2:       onKeyDown(F2);       break;
                case VK_F3:       onKeyDown(F3);       break;
                case VK_F4:       onKeyDown(F4);       break;
                case VK_F5:       onKeyDown(F5);       break;
                case VK_F6:       onKeyDown(F6);       break;
                case VK_F7:       onKeyDown(F7);       break;
                case VK_F8:       onKeyDown(F8);       break;
                case VK_F9:       onKeyDown(F9);       break;
            }
            break;

        case WM_KEYUP:
            switch (wParam&0xFF)
            {
                case VK_LEFT:     onKeyUp(Left);       break;
                case VK_RIGHT:    onKeyUp(Right);      break;
                case VK_UP:       onKeyUp(Up);         break;
                case VK_DOWN:     onKeyUp(Down);       break;
                case VK_SPACE:    onKeyUp(Space);      break;
                case VK_RETURN:   onKeyUp(Enter);      break;
                case VK_CONTROL:  onKeyUp(Control);    break;
                case VK_ESCAPE:   onKeyUp(Esc);        break;
                case 33:          onKeyUp(PageUp);     break;
                case 34:          onKeyUp(PageDown);   break;
                case VK_F1:       onKeyUp(F1);         break;
                case VK_F2:       onKeyUp(F2);         break;
                case VK_F3:       onKeyUp(F3);         break;
                case VK_F4:       onKeyUp(F4);         break;
                case VK_F5:       onKeyUp(F5);         break;
                case VK_F6:       onKeyUp(F6);         break;
                case VK_F7:       onKeyUp(F7);         break;
                case VK_F8:       onKeyUp(F8);         break;
                case VK_F9:       onKeyUp(F9);         break;
            }
            break;

        case WM_CLOSE:
            onQuit();
            break;

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

bool openDisplay(const char title[], int width, int height, bool fullscreen)
{
    displayWidth = width;
    displayHeight = height;
    displayFullscreen = fullscreen;

    HINSTANCE instance = GetModuleHandle(0);

    WNDCLASSEX windowClass;
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_OWNDC;
    windowClass.lpfnWndProc = &WndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = instance;
    windowClass.hIcon = 0;
    windowClass.hCursor = LoadCursor(NULL,IDC_ARROW);
    windowClass.hbrBackground = NULL;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = title;
    windowClass.hIconSm = NULL;

    UnregisterClass(title, instance);

    if (!RegisterClassEx(&windowClass)) 
        return false;

    // determine window style

    int style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;

    if (fullscreen)
        style = WS_POPUP;

    // create window

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    AdjustWindowRect(&rect, style, 0);
    rect.right -= rect.left;
    rect.bottom -= rect.top;

    int x = 0;
    int y = 0;

    if (!fullscreen)
    {
        x = (GetSystemMetrics(SM_CXSCREEN) - rect.right) >> 1;
        y = (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) >> 1;
    }

    window = CreateWindow( title, title, style,
                           x, y, rect.right, rect.bottom,
                           NULL, NULL, instance, NULL );

    if (!window)
        return false;

    // change display mode

    if (fullscreen)
    {
        DEVMODE mode;
        memset(&mode, 0, sizeof(mode));
        mode.dmSize = sizeof(mode);
        mode.dmPelsWidth = width;
        mode.dmPelsHeight = height;
        mode.dmBitsPerPel = 32;
        mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&mode, CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
            return false;

        ShowCursor(0);
    }

    // initialize wgl

    PIXELFORMATDESCRIPTOR descriptor;
    memset(&descriptor, 0, sizeof(descriptor));
    descriptor.nVersion = 1;
    descriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    descriptor.iPixelType = PFD_TYPE_RGBA;
    descriptor.cColorBits = 32;
    descriptor.cDepthBits = 16;
    descriptor.cStencilBits = 8;
    descriptor.iLayerType = PFD_MAIN_PLANE;

    device = GetDC(window);
    if (!device)
        return false;

    GLuint format = ChoosePixelFormat(device, &descriptor);
    if (!format)
        return false;

    if (!SetPixelFormat(device, format, &descriptor))
        return false;

    context = wglCreateContext(device);
    if (!context)
        return false;

    if (!wglMakeCurrent(device, context))
        return false;

    // build font

    HFONT font;
    HFONT oldfont;

    fontBase = glGenLists(96);

    font = CreateFont( 24,				// Height Of Font
        0,								// Width Of Font
        0,								// Angle Of Escapement
        0,								// Orientation Angle
        FW_BOLD,						// Font Weight
        FALSE,							// Italic
        FALSE,							// Underline
        FALSE,							// Strikeout
        ANSI_CHARSET,					// Character Set Identifier
        OUT_TT_PRECIS,					// Output Precision
        CLIP_DEFAULT_PRECIS,			// Clipping Precision
        ANTIALIASED_QUALITY,			// Output Quality
        FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
        "Courier New");					// Font Name

    oldfont = (HFONT) SelectObject(device, font);
    wglUseFontBitmaps(device, 32, 96, fontBase);
    SelectObject(device, oldfont);
    DeleteObject(font);

    // show window

    ShowWindow(window, SW_NORMAL);

    return true;
}

void updateDisplay()
{
	// show rendering
	
    SwapBuffers(device);
    
	// process window messages

    MSG message;

    while (true)
    {
        if (active)
        {
            if (!PeekMessage(&message, window, 0, 0, PM_REMOVE)) 
                break;

            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else
        {
            if (!GetMessage(&message, window, 0, 0)) 
                break;

            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
}

void closeDisplay()
{	
    glDeleteLists(fontBase, 96);

    ReleaseDC(window, device);
    device = 0;

    DestroyWindow(window);
    window = 0;
}

float time()
{
    static __int64 start = 0;
    static __int64 frequency = 0;

    if (start==0)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&start);
        QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
        return 0.0f;
    }

    __int64 counter = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&counter);
    return (float) ((counter - start) / double(frequency));
}

void drawText(float x, float y, const char text[], Vector color, float alpha)
{
    // note: user is responsible for setting up screenspace matrices etc.

    glColor4f(color.x, color.y, color.z, alpha);

    glRasterPos2f(x, y);

    glPushAttrib(GL_LIST_BIT);
    glListBase(fontBase - 32);
    glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);
    glPopAttrib();
}

// naughty! :)

int main();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    main();
}

#endif
