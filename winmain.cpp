#ifndef _WINMAIN_CPP
#define _WINMAIN_CPP
#endif

#define WIN32_MEAN_AND_LEAN
#define WIN32_EXTRA_LEAN

#include "winmain.h"
#include "resource.h"

const float FPS_INTERVAL = 1.0f;
const char APP_NAME[] = "OPENGL GAME PROJECT";

HDC			hDC;
HWND		hWndMain = NULL;
HINSTANCE	hInstance = NULL;
bool		FullScreen = false;
bool		Quit = false;
bool		Active = false;
int			windowWidth,windowHeight;

CGfxOpenGL *g_glRender = NULL;
CInputSystem g_Input;

void SetupPixelFormat(HDC hDC)
{
	int pixelFormat;

	PIXELFORMATDESCRIPTOR pfd =
	{	
		sizeof(PIXELFORMATDESCRIPTOR),	// size
			1,							// version
			PFD_SUPPORT_OPENGL |		// OpenGL window
			PFD_DRAW_TO_WINDOW |		// render to window
			PFD_DOUBLEBUFFER,			// support double-buffering
			PFD_TYPE_RGBA,				// color type
			32,							// prefered color depth
			0, 0, 0, 0, 0, 0,			// color bits (ignored)
			0,							// no alpha buffer
			0,							// alpha bits (ignored)
			0,							// no accumulation buffer
			0, 0, 0, 0,					// accum bits (ignored)
			16,							// depth buffer
			0,							// no stencil buffer
			0,							// no auxiliary buffers
			PFD_MAIN_PLANE,				// main layer
			0,							// reserved
			0, 0, 0,					// no layer, visible, damage masks
	};

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hDC;
	static HGLRC hRC;
	int height, width;

	switch(message)
	{
		case WM_CREATE:			// window creation
			hDC = GetDC(hWnd);
			SetupPixelFormat(hDC);
			//SetupPalette();
			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);
			break;

		case WM_CLOSE:					// windows is closing

			// deselect rendering context and delete it
			wglMakeCurrent(hDC, NULL);
			wglDeleteContext(hRC);

			// send WM_QUIT to message queue
			PostQuitMessage(0);
			break;

		case WM_SIZE:
			height = HIWORD(lParam);		// retrieve width and height
			width = LOWORD(lParam);

			windowWidth = width;
			windowHeight = height;

			g_glRender->SetupProjection(width, height);
			break;

		case WM_ACTIVATEAPP:		// activate app
			break;

		case WM_PAINT:				// paint
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				Active=TRUE;						// Program Is Active
			}
			else
			{
				Active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}
		break;

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CHAR:
		{
			g_glRender->GetKeyChar(wParam);
		}
		break;

		case WM_KEYDOWN:
		{
			g_glRender->WindowKeyDown(wParam);

			switch (wParam)
			{
				case VK_ESCAPE:
//					g_glRender->Destroy();
//					Quit = true;
//					PostQuitMessage(0);
					break;
			}
		}
		break;

		case WM_KEYUP:
		{
			g_glRender->WindowKeyUp(wParam);
		}
		break;

		case WM_MOUSEMOVE:
			g_glRender->GetMousePos(LOWORD(lParam),HIWORD(lParam));
			break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int CreateWinApp(char *title, int width, int height, int bits, bool fullscreenflag)
{
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	FullScreen = fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);									// No Background Required For GL
	wc.lpszMenuName		= "OpenGL";										// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	g_glRender = new CGfxOpenGL;

	if (FullScreen)								// fullscreen?
	{
		DEVMODE dmScreenSettings;					// device mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);	
		dmScreenSettings.dmPelsWidth = width;			// screen width
		dmScreenSettings.dmPelsHeight = height;			// screen height
		dmScreenSettings.dmBitsPerPel = bits;				// bits per pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// 
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// setting display mode failed, switch to windowed
			MessageBox(NULL, "Display mode failed", NULL, MB_OK);
			FullScreen = FALSE;	
		}
	}
	
	if (FullScreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWndMain=CreateWindowEx(dwExStyle,						// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;
}

void DisplayTitle(HWND hwnd, float fps)
{
  static char title[255];
  sprintf(title, "%s - FPS: %.0f", "ZOKZEK GAME", fps);
  SetWindowText(hwnd, title);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstanc, LPSTR lpCmdLine, int nShowCmd)
{
	MSG		msg;									// Windows Message Structure
	float	m_fps;
	int		lastTime = timeGetTime();
	int		thisTime;

	CoInitialize(NULL);
/*	
	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		FullScreen=TRUE;							// Windowed Mode
	}
*/
	FullScreen = TRUE;
	// Create Our OpenGL Window
	if (!CreateWinApp("GARUN FARM", 800, 600, 16, FullScreen))
	{
		MessageBox(NULL,"WinApp Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Quit If Window Was Not Created
	}

	hDC = GetDC(hWndMain);

	// check if window creation failed (hwnd would equal NULL)
	if (!hWndMain)
		return 0;

	ShowWindow(hWndMain,nShowCmd);
	UpdateWindow(hWndMain);

	g_Input.Initialize(hWndMain, (HINSTANCE)GetModuleHandle(NULL), false, IS_USEKEYBOARD | IS_USEMOUSE);
	g_Input.AcquireAll();
	
	g_glRender->Init();

	while(!Quit)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				Quit=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if (Active)	// Active?  Was There A Quit Received?
			{
			    static int frame = 0;
			    static float fpsTime = 0.0;

			    thisTime = timeGetTime();
			    float elapsedTime = (float)(thisTime - lastTime)/1000;

			    fpsTime += elapsedTime;

			    if (fpsTime > FPS_INTERVAL)
			    {
			      DisplayTitle(hWndMain, frame/fpsTime);
				  m_fps = frame/fpsTime;
			      frame = 0;
			      fpsTime -= FPS_INTERVAL;
				}

				lastTime = thisTime;

				if(g_glRender->Render() == 1)
				{
					g_glRender->Animate(elapsedTime);
					SwapBuffers(hDC);
				
					frame++;

					g_Input.Update();
				}
				else
				{
					Quit = true;
				}
			}
		}
	}

	CoUninitialize();

	return (msg.wParam);							// Exit The Program
}