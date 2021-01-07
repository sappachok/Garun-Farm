#ifndef _WINMAIN_H
#define _WINMAIN_H

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <fstream>
#include <vector>									
#include <crtdbg.h>
#include "CTargaImage.h"
#include "CGfxOpenGL.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include "MusicSystem.h"

using namespace std;

#pragma comment(lib,"winmm.lib")

#define MAXTEXTURE  500
#define MAXUNIT		500
#define MAXENIME	20

extern HWND		hWndMain;

extern CInputSystem g_Input;
extern int windowWidth,windowHeight;
extern bool	FullScreen;
#endif
