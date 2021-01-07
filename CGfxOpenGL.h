#ifndef __GL_COMPONENT_H
#define __GL_COMPONENT_H

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "winmain.h"
#include "camera.h"
#include "GFont.h"

//#include "world.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

enum gameState
{
	START,
	OPTION,
	SCORE,
	PLAY,
	PROCESS_SCORE,
	MEM_SCORE,
	END,
	QUIT
};

enum gameStartMode
{
	ST_NONE,
	ST_OPTION,
	ST_SCORE,
	ST_HELP
};

enum gameDlg
{
	DLG_NONE,
	DLG_PLAYMODE_MENU,
	DLG_CONFIRM_QUIT,
	DLG_CONFIRM_TOMAINMENU,
	DLG_CONFIRM_MEMSCORE,
	DLG_HELP01,
	DLG_HELP02
};

typedef struct t_Menu
{
	int xmin;
	int xmax;
	int ymin;
	int ymax;
	unsigned int texture;
	unsigned int textureMesk;
}t_Menu;

class CGfxOpenGL
{
	struct Player
	{
		char name[16];
		int	 score;
	}stScore[20];

private:
	int m_windowWidth;
	int m_windowHeight;
	int Keys[256];
	char MessageChar[32];

	t_Menu startMenu;
	t_Menu optionMenu;
	t_Menu scoreMenu;

	float m_angle;
	int gamescore;
	int mX,mY;
	int key_limit;
	gameState m_gameState;
	gameStartMode m_gameStartMode;
	gameDlg m_gameDlg;
	CCamera m_camera;
	bool bGetchar;

public:

	CGfxOpenGL();

	virtual ~CGfxOpenGL();

	bool Init();
	bool Shutdown();

	void SetupProjection(int width, int height);
	void GetMousePos(int x, int y) { mX = x; mY = m_windowHeight - y; };
	void WindowKeyDown(int virkeys) { Keys[virkeys] = true; }
	void WindowKeyUp(int virkeys) { Keys[virkeys] = false; }
	void GetKeyChar(char keychar,int numLimit = 32);
	void SetKeyBuffer(int numLimit = 32) 
	{ (numLimit == 0) ? bGetchar = false:bGetchar = true; key_limit = numLimit; }
	void ClearBufferKey(void) { sprintf(MessageChar,""); }
	void Prepare(float dt);
	void Animate(float dt);
	bool Render();

	void Loading();
	void InitGameStart();
	void StartMode();
	void InitGamePlay();
	void PlayMode();
	void DestroyGamePlay();
	void ProcessScoreMode();
	void MemScoreMode();

	void OptionMode();
	void ScoreMode();
	void HelpMode();
	void ConfirmQuit();
	void DlgDraw();

	void Destroy();

	void DrawPicture(float sx, float sy,float ex, float ey,unsigned int TextureTGA)
	{
		glBindTexture(GL_TEXTURE_2D,TextureTGA);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0); glVertex2f( sx, ey);
			glTexCoord2f(1.0, 1.0); glVertex2f( ex, ey);
			glTexCoord2f(1.0, 0.0); glVertex2f( ex, sy);
			glTexCoord2f(0.0, 0.0); glVertex2f( sx, sy);
		glEnd();
	}

	void CommandLine(char* text);
};

class CMenu
{
private:

public:
	int xmin,xmax,ymin,ymax;
	unsigned int texture[2];
	unsigned int textureMask[2];
	bool click;
	bool over;

	CMenu() { click = false; over = false; }
	~CMenu() {}
	
	void create(int x_min, int x_max, int y_min, int y_max)
	{
		xmin = x_min;
		xmax = x_max;
		ymin = y_min;
		ymax = y_max;
	}

	void Draw()
	{
		if(click != true) //// DON'T CLICK /////////////
		{
			if(over != true) //// MOUSE OUT /////////////
			{
				if(texture[0])
				{
					glEnable(GL_BLEND);

					glBlendFunc(GL_DST_COLOR,GL_ZERO);	
					glBindTexture(GL_TEXTURE_2D,textureMask[0]);	
					glBegin(GL_QUADS);
						glTexCoord2f(0.0, 1.0); glVertex2f( xmin, ymax);
						glTexCoord2f(1.0, 1.0); glVertex2f( xmax, ymax);
						glTexCoord2f(1.0, 0.0); glVertex2f( xmax, ymin);
						glTexCoord2f(0.0, 0.0); glVertex2f( xmin, ymin);
					glEnd();

					glBlendFunc(GL_ONE, GL_ONE);
					glBindTexture(GL_TEXTURE_2D,texture[0]);
					glBegin(GL_QUADS);
						glTexCoord2f(0.0, 1.0); glVertex2f( xmin, ymax);
						glTexCoord2f(1.0, 1.0); glVertex2f( xmax, ymax);
						glTexCoord2f(1.0, 0.0); glVertex2f( xmax, ymin);
						glTexCoord2f(0.0, 0.0); glVertex2f( xmin, ymin);
					glEnd();

					glDisable(GL_BLEND);
				}
				else
				{
					glDisable(GL_LIGHTING);
					glDisable(GL_TEXTURE_2D);
//					glColor3f(0,1,0);
					glBegin(GL_QUADS);
						glVertex2f( xmin, ymax);
						glVertex2f( xmax, ymax);
						glVertex2f( xmax, ymin);
						glVertex2f( xmin, ymin);
					glEnd();
				}
			}
			else //// MOUSE OVER /////////////
			{
				if(texture[1])
				{
					glEnable(GL_BLEND);

					glBlendFunc(GL_DST_COLOR,GL_ZERO);	
					glBindTexture(GL_TEXTURE_2D,textureMask[0]);	
					glBegin(GL_QUADS);
						glTexCoord2f(0.0, 1.0); glVertex2f( xmin, ymax);
						glTexCoord2f(1.0, 1.0); glVertex2f( xmax, ymax);
						glTexCoord2f(1.0, 0.0); glVertex2f( xmax, ymin);
						glTexCoord2f(0.0, 0.0); glVertex2f( xmin, ymin);
					glEnd();

					glBlendFunc(GL_ONE, GL_ONE);
					glBindTexture(GL_TEXTURE_2D,texture[1]);
					glBegin(GL_QUADS);
						glTexCoord2f(0.0, 1.0); glVertex2f( xmin, ymax);
						glTexCoord2f(1.0, 1.0); glVertex2f( xmax, ymax);
						glTexCoord2f(1.0, 0.0); glVertex2f( xmax, ymin);
						glTexCoord2f(0.0, 0.0); glVertex2f( xmin, ymin);
					glEnd();
	
					glDisable(GL_BLEND);
				}
				else
				{
					glDisable(GL_LIGHTING);
					glDisable(GL_TEXTURE_2D);
					glBegin(GL_QUADS);
//					glColor3f(1,0,0);
						glVertex2f( xmin, ymax);
						glVertex2f( xmax, ymax);
						glVertex2f( xmax, ymin);
						glVertex2f( xmin, ymin);
					glEnd();
				}
			}
		}
		else
		{
			if(texture[1])
			{
				glEnable(GL_BLEND);

				glBlendFunc(GL_DST_COLOR,GL_ZERO);	
				glBindTexture(GL_TEXTURE_2D,textureMask[0]);	
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 1.0); glVertex2f( xmin, ymax);
					glTexCoord2f(1.0, 1.0); glVertex2f( xmax, ymax);
					glTexCoord2f(1.0, 0.0); glVertex2f( xmax, ymin);
					glTexCoord2f(0.0, 0.0); glVertex2f( xmin, ymin);
				glEnd();

				glBlendFunc(GL_ONE, GL_ONE);
				glBindTexture(GL_TEXTURE_2D,texture[1]);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 1.0); glVertex2f( xmin, ymax);
					glTexCoord2f(1.0, 1.0); glVertex2f( xmax, ymax);
					glTexCoord2f(1.0, 0.0); glVertex2f( xmax, ymin);
					glTexCoord2f(0.0, 0.0); glVertex2f( xmin, ymin);
				glEnd();

				glDisable(GL_BLEND);
			}
			else
			{
				glDisable(GL_LIGHTING);
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
//					glColor3f(1,0,0);
					glVertex2f( xmin, ymax);
					glVertex2f( xmax, ymax);
					glVertex2f( xmax, ymin);
					glVertex2f( xmin, ymin);
				glEnd();
			}
		}

		glDisable(GL_BLEND);	

/*		glEnable(GL_BLEND);

		glPushMatrix();
		glBlendFunc(GL_DST_COLOR,GL_ZERO);	
		glBindTexture(GL_TEXTURE_2D,textureMesk);	
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0); glVertex2f( xmin, ymax);
			glTexCoord2f(1.0, 1.0); glVertex2f( xmax, ymax);
			glTexCoord2f(1.0, 0.0); glVertex2f( xmax, ymin);
			glTexCoord2f(0.0, 0.0); glVertex2f( xmin, ymin);
		glEnd();

		glBlendFunc(GL_ONE, GL_ONE);
		glBindTexture(GL_TEXTURE_2D,texture);	
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0); glVertex2f( xmin, ymax);
			glTexCoord2f(1.0, 1.0); glVertex2f( xmax, ymax);
			glTexCoord2f(1.0, 0.0); glVertex2f( xmax, ymin);
			glTexCoord2f(0.0, 0.0); glVertex2f( xmin, ymin);
		glEnd();
		glPopMatrix();

		glDisable(GL_BLEND);
*/	}

	bool mouseOver(POINT mp)
	{
		if(mp.x < xmin)
			return 0;
		if(mp.x > xmax)
			return 0;
		if(mp.y < ymin)
			return 0;
		if(mp.y > ymax)
			return 0;

		return 1;
	}

	bool Click(POINT mp,bool bMouseDown)
	{
		if(bMouseDown == true)
		{
			if(mp.x < xmin)
				return 0;
			if(mp.x > xmax)
				return 0;
			if(mp.y < ymin)
				return 0;
			if(mp.y > ymax)
				return 0;

			click = true;
			return 1;
		}
	
		return 0;
	}

	bool Release(POINT mp,bool bMouseDown)
	{
		if(mouseOver(mp))
		{
			over = true;
		}
		else
		{
			over = false;
		}

		if(bMouseDown == true)
		{
			if(mp.x < xmin)
			{
				click = false;
				return 0;
			}
			if(mp.x > xmax)
			{
				click = false;
				return 0;
			}
			if(mp.y < ymin)
			{
				click = false;
				return 0;
			}
			if(mp.y > ymax)
			{
				click = false;
				return 0;
			}

			click = true;
		}

		if(bMouseDown == false)
		{
			if(click == true)
			{
				if(mp.x < xmin)
				{
					click = false;
					return 0;
				}
				if(mp.x > xmax)
				{
					click = false;
					return 0;
				}
				if(mp.y < ymin)
				{
					click = false;
					return 0;
				}
				if(mp.y > ymax)
				{
					click = false;
					return 0;
				}

				click = false;
				return 1;
			}
		}

		return 0;
	}
};

class CCanvas2D
{
private:

public:
	unsigned int Texture;
	unsigned int Texture_Mask;

	CCanvas2D() {}
	~CCanvas2D() {}

	void DrawPic(float sx, float sy,float ex, float ey,bool bMask)
	{
		if(bMask)
		{
			glEnable(GL_BLEND);

			glBlendFunc(GL_DST_COLOR,GL_ZERO);	
			glBindTexture(GL_TEXTURE_2D,Texture_Mask);	
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0); glVertex2f( sx, ey);
				glTexCoord2f(1.0, 1.0); glVertex2f( ex, ey);
				glTexCoord2f(1.0, 0.0); glVertex2f( ex, sy);
				glTexCoord2f(0.0, 0.0); glVertex2f( sx, sy);
			glEnd();

			glBlendFunc(GL_ONE, GL_ONE);
			glBindTexture(GL_TEXTURE_2D,Texture);	
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0); glVertex2f( sx, ey);
				glTexCoord2f(1.0, 1.0); glVertex2f( ex, ey);
				glTexCoord2f(1.0, 0.0); glVertex2f( ex, sy);
				glTexCoord2f(0.0, 0.0); glVertex2f( sx, sy);
			glEnd();

			glDisable(GL_BLEND);	
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D,Texture);	
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0); glVertex2f( sx, ey);
				glTexCoord2f(1.0, 1.0); glVertex2f( ex, ey);
				glTexCoord2f(1.0, 0.0); glVertex2f( ex, sy);
				glTexCoord2f(0.0, 0.0); glVertex2f( sx, sy);
			glEnd();
		}
		glDisable(GL_BLEND);	
	}

	void DrawPicture(float sx, float sy,float ex, float ey,
		float txmin, float tymin, float txmax, float tymax,
		unsigned int TextureTGA)
	{
		glBindTexture(GL_TEXTURE_2D,TextureTGA);
			
		glBegin(GL_QUADS);
			glTexCoord2f(txmin, tymax); glVertex2f( sx, ey);
			glTexCoord2f(txmax, tymax); glVertex2f( ex, ey);
			glTexCoord2f(txmax, tymin); glVertex2f( ex, sy);
			glTexCoord2f(txmin, tymin); glVertex2f( sx, sy);
		glEnd();
	}
};

class COGLCursor
{
private:
	BOOL m_show;
	BOOL m_fullscreen;
	int window_width,window_height;
	int width,height;
	POINT m_mousePos;
public:
	BOOL m_click;

	unsigned int m_texture;
	unsigned int m_texture_mask;

	unsigned int m_texture_click;

	COGLCursor() { m_show=FALSE; m_fullscreen = FALSE; m_click = FALSE; }
	COGLCursor(int width,int height) { window_width=width; window_height=height; }
	~COGLCursor() {}

	void CreateCursor(int cursorwidth,int cursorheight,int winwidth,int winheight) {
		width = cursorwidth; height = cursorheight; 
		window_width=winwidth; window_height=winheight; }

	bool LoadCursor(unsigned int texture, unsigned int texture_mask) 
	{
	}

	void GetCursorPosition(POINT cursorPos)
	{
		m_mousePos = cursorPos;
	}

	void Show() 
	{
		if(m_show == TRUE)
		{
			if(m_texture_mask)
			{
				if(m_texture)
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_DST_COLOR,GL_ZERO);	
					glBindTexture(GL_TEXTURE_2D,m_texture_mask);
					glBegin(GL_QUADS);
						glTexCoord2f(0, 1); glVertex2f( m_mousePos.x - width/2, m_mousePos.y + height/2);
						glTexCoord2f(1, 1); glVertex2f( m_mousePos.x + width/2, m_mousePos.y + height/2);
						glTexCoord2f(1, 0); glVertex2f( m_mousePos.x + width/2, m_mousePos.y - height/2);
						glTexCoord2f(0, 0); glVertex2f( m_mousePos.x - width/2, m_mousePos.y - height/2);
					glEnd();

					if(!m_click)
					{
						glBlendFunc(GL_ONE, GL_ONE);
						glBindTexture(GL_TEXTURE_2D,m_texture);
						glBegin(GL_QUADS);
							glTexCoord2f(0, 1); glVertex2f( m_mousePos.x - width/2, m_mousePos.y + height/2);
							glTexCoord2f(1, 1); glVertex2f( m_mousePos.x + width/2, m_mousePos.y + height/2);
							glTexCoord2f(1, 0); glVertex2f( m_mousePos.x + width/2, m_mousePos.y - height/2);
							glTexCoord2f(0, 0); glVertex2f( m_mousePos.x - width/2, m_mousePos.y - height/2);
						glEnd();
						glDisable(GL_BLEND);
					}
					else
					{
						glBlendFunc(GL_ONE, GL_ONE);
						glBindTexture(GL_TEXTURE_2D,m_texture_click);
						glBegin(GL_QUADS);
							glTexCoord2f(0, 1); glVertex2f( m_mousePos.x - width/2, m_mousePos.y + height/2);
							glTexCoord2f(1, 1); glVertex2f( m_mousePos.x + width/2, m_mousePos.y + height/2);
							glTexCoord2f(1, 0); glVertex2f( m_mousePos.x + width/2, m_mousePos.y - height/2);
							glTexCoord2f(0, 0); glVertex2f( m_mousePos.x - width/2, m_mousePos.y - height/2);
						glEnd();
						glDisable(GL_BLEND);
					}
				}
			}
			else
			{
				glDisable(GL_LIGHTING);
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
					glVertex2f( m_mousePos.x + 25, m_mousePos.y + 50);
					glVertex2f( m_mousePos.x - 25, m_mousePos.y + 50);
					glVertex2f( m_mousePos.x - 25, m_mousePos.y);
					glVertex2f( m_mousePos.x + 25, m_mousePos.y);
				glEnd();
			}
		}
	}

	void ShowCursor(BOOL bShow)
	{
		m_show = bShow;
	}
};

#endif