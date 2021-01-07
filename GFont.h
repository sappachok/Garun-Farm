#ifndef GL_GRAPHIC_FONT_INCLUDE
#define GL_GRAPHIC_FONT_INCLUDE

#include <windows.h>			// standard Windows app include
#include <gl/gl.h>				// standard OpenGL include
#include <gl/glu.h>				// OpenGL utilties
#include <stdio.h>
#include <string.h>
#include "CTargaImage.h"

class GFont
{
private:
	unsigned FontTexture;
	unsigned FontTextureMask;
	bool b_Mask;
	int  size;
	float positionx,positiony;

public:

	GFont() { size = 50; }
	~GFont() {}
	BOOL LoadTextureTGA(char *filename);
	BOOL LoadTextureMaskTGA(char *filename);
	void DrawChar(char c, float posx, float posy);
	void ShowText2D(char *text, float posx, float posy, int fontsize=0);
	void ShowText2DR2L(char *text, float posx, float posy, int fontsize=0);
	void ShowText2D(char *text, float posx, float posy, int fontsize, int width_char);
};

#endif
