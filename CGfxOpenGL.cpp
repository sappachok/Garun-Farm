#ifdef _WINDOWS
#include <windows.h>
#endif


#define SKYBOX_FRONT	0
#define SKYBOX_BACK		1
#define SKYBOX_LEFT		2
#define SKYBOX_RIGHT	3 
#define SKYBOX_TOP		4
#define SKYBOX_BOTTOM	5

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
#define GL_CLAMP_TO_EDGE	0x812F		// Define this for glTexParameteri
//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

#include "CGfxOpenGL.h"

#ifdef _WINDOWS
#include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "CGfxOpenGL.h"
#include "world.h"
#include "3dsLoader.h"

// disable implicit float-double casting
#pragma warning(disable:4305)

CWorld *world;
COGLCursor cursor;
//C3dsLoader obj3dsLoader;

CMenu button_start;
CMenu button_option;
CMenu button_score;
CMenu button_help;

CMenu button_optionBack;
CMenu button_ok;
CMenu button_cancel;

CMenu button_screen;
int choice_screen = 0;
CMenu button_difficult;
int choice_difficult = 0;
CMenu button_InputDevice; 
int choice_device = 0;

CMenu button_resume;
CMenu button_tostart;
CMenu button_quit;
bool mouseUp = false;

POINT mousePos;
bool pause = false;
bool gameover = false;
bool gamewin = false;
bool process_score = false;
float count = 0;
int player_score_no = 0;
float curtain_alpha = 1.0f;

unsigned int TitleBGTexture;
unsigned int TitleBGTexture_mask;
unsigned int TitleBackTexture;
unsigned int TitleNameTexture;
unsigned int TitleNameTexture_mask;
unsigned int TitleCloundTexture;
unsigned int TitleCloundTexture_mask;

unsigned int OptionBGTexture;
unsigned int OptionBGTexture_mask;
unsigned int ScreenBUTTexture[3];
unsigned int DifficultBUTTexture[3];
unsigned int DeviceBUTTexture[3];
unsigned int TextureSkyBox[6];
unsigned int PlayerConsoleTexture;
unsigned int ItemTexture;

unsigned int HelpBox01Texture;
unsigned int Loading01Texture;

IDirectMusicSegment *lpdms_Open = NULL;
IDirectMusicSegment *lpdms_Play01 = NULL;
IDirectMusicSegment *lpdms_BattleBoss = NULL;
IDirectMusicSegment *lpdms_Over = NULL;
IDirectMusicSegment *lpdms_Win = NULL;

LPDIRECTSOUNDBUFFER lpdsb_Open = NULL;
LPDIRECTSOUNDBUFFER lpdsb_Battle = NULL;
LPDIRECTSOUNDBUFFER lpdsb_GameOver = NULL;
LPDIRECTSOUNDBUFFER lpdsb_BattleEnd = NULL;
LPDIRECTSOUNDBUFFER lpdsb_BattleBoss = NULL;
LPDIRECTSOUNDBUFFER lpdsb_ScoreMode = NULL;

LPDIRECTSOUNDBUFFER lpdsb_SelectMenu = NULL;
LPDIRECTSOUNDBUFFER lpdsb_ShowMenu = NULL;

CSoundSystem *g_sound;
CMusicSystem *g_music;

GFont *g_GraphicFont;
CCanvas2D *ConsolePlayer;
CCanvas2D *Hummer_canvas;
CCanvas2D *Arrow_canvas;
CCanvas2D *Trap_canvas;

CCanvas2D *Canvas;
float Clound_Pos = 0;
float Name_Pos = 0;
float time_logic = 0;
bool b_command = 0;

extern CSoundSystem m_sound;
extern bool FullScreen;

void DrawBoxAlpha(float stX,float enX,float stY,float enY) 
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor3f(0,0,0);
	glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glVertex2f( stX, stY);
		glVertex2f( enX, stY);
		glVertex2f( enX, enY);
		glVertex2f( stX, enY);	
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void DrawLifeGrade(float stX,float enX,float stY,float enY)
{
	glPushMatrix();
	glBegin(GL_QUADS);
		glColor3f(1.0f,0.0f,0.0f); glVertex2f( stX, stY);
		glColor3f(1.0f,0.0f,0.0f); glVertex2f( enX, stY);
		glColor3f(0.0f,0.0f,1.0f); glVertex2f( enX, enY);
		glColor3f(0.0f,0.0f,1.0f); glVertex2f( stX, enY);	
	glEnd();
	glPopMatrix();
}

void LoadTexture(UINT *texture, LPSTR strFileName)
{
	AUX_RGBImageRec *pBitMap = NULL;
	FILE *pFile = NULL;									
	int type;
	char tempstring[5] = {0};
	
	strncpy(tempstring, strFileName + strlen(strFileName)-4, 4);
	
	char FilePath[200];
	
	sprintf(FilePath, "data/texture/%s", strFileName);
	
	if(!strFileName) return;
	
	if(!strcmp(tempstring, ".bmp"))
	{
		pFile = fopen(FilePath,"rb");					
		
		if(pFile)										
		{
			pBitMap = auxDIBImageLoad(FilePath);		
			type = 1;
		}
		else
		{
			MessageBox(NULL, FilePath /*"couldn't find texture!"*/, "Error!", MB_OK);
			exit(0);
		}
	} 

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitMap->sizeX, pBitMap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitMap->data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	
	if (pBitMap)									
	{
		if (pBitMap->data)					
		{
			free(pBitMap->data);			
		}
		free(pBitMap);
	}
}

bool LoadTGATexture(char *filename,unsigned int *textureObject)
{
	CTargaImage	*texture;
	texture = new CTargaImage;
	
	glEnable(GL_TEXTURE_2D);

	if(!texture->Load(filename))
	{
		MessageBox(NULL,"NONE IMAGE","MSG",MB_OK);
		return FALSE;
	}

	glGenTextures(1, textureObject);

	glBindTexture(GL_TEXTURE_2D, *textureObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// now that the texture object is bound, specify a texture for it
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB4, texture->GetWidth(), texture->GetHeight(),
				 0, GL_RGB, GL_UNSIGNED_BYTE, texture->GetImage());

//	glDisable(GL_TEXTURE_2D);

	return TRUE;
}

void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
	// Center the Skybox around the given x,y,z position
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;


	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, TextureSkyBox[SKYBOX_FRONT]);
	glBegin(GL_QUADS);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, TextureSkyBox[SKYBOX_BACK]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, TextureSkyBox[SKYBOX_LEFT]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);		
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, TextureSkyBox[SKYBOX_RIGHT]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, TextureSkyBox[SKYBOX_TOP]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, TextureSkyBox[SKYBOX_BOTTOM]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
	glEnd();

}

CGfxOpenGL::CGfxOpenGL()
{
	m_gameState = START;
	m_gameStartMode = ST_NONE;
	m_gameDlg = DLG_NONE;
	bGetchar = false;
}

CGfxOpenGL::~CGfxOpenGL()
{

}

bool CGfxOpenGL::Init()
{   
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
//	glShadeModel(GL_FLAT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode (GL_PROJECTION);

	glLoadIdentity ();

	gluOrtho2D (0, windowWidth, 0, windowHeight);

	glMatrixMode (GL_MODELVIEW);

	glLoadIdentity ();
	
	glRasterPos2i(0, 0);

	glLoadIdentity ();
	g_GraphicFont = new GFont;
	g_GraphicFont->LoadTextureTGA("Data\\texture\\GFont\\Font04.tga");
	g_GraphicFont->LoadTextureMaskTGA("Data\\texture\\GFont\\Font02_mask.tga");

	g_sound = new CSoundSystem;
	g_sound->Create(hWndMain);

	g_music = new CMusicSystem;
	g_music->InitDirectMusic(hWndMain);
	
	ShowCursor(FALSE);
	//////// Load Cursor TGA Image //////////////
	//cursor.LoadCursor();

	cursor.CreateCursor(80,80,windowWidth,windowHeight);
	LoadTGATexture("Data\\Image\\CURSOR2.tga",&cursor.m_texture);
	LoadTGATexture("Data\\Image\\CURSOR_CLICK2.tga",&cursor.m_texture_click);
	LoadTGATexture("Data\\Image\\_CURSOR2.tga",&cursor.m_texture_mask);
	/////////////////////////////////////////////

	cursor.ShowCursor(TRUE);
	InitGameStart();

	return TRUE;										// Initialization Went OK
}

bool CGfxOpenGL::Shutdown()
{

    return true;
}

void CGfxOpenGL::SetupProjection(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,500.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();

    m_windowWidth = width;
    m_windowHeight = height;
}

void CGfxOpenGL::Prepare(float dt)
{
	
}

void CGfxOpenGL::Animate(float dt)
{
	if(m_gameState==START)
	{
		Clound_Pos -= 0.1f;
		Name_Pos += 0.3f;
	}
	else if(m_gameState==PLAY)
	{
		if(pause != true)
		{
			count -= dt;
			world->Animate(dt);
		}
	}

	if(time_logic >= 1)
	{
		if(b_command == true)
		{
			b_command = false;
		}
		else
		{
			b_command = true;
		}

		time_logic = 0;
	}

	time_logic += dt*2.0;
}

bool CGfxOpenGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	
	glEnable(GL_TEXTURE_2D);
	
	if(m_gameState==START)
	{
		StartMode();
	}
	else if(m_gameState==PLAY)
	{
		PlayMode();
	}
	else if(m_gameState==PROCESS_SCORE)
	{
		ProcessScoreMode();
	}
	else if(m_gameState==MEM_SCORE)
	{
		MemScoreMode();
	}
	else if(m_gameState==QUIT)
	{
		cursor.ShowCursor(FALSE);
		return 0;
	}

	DlgDraw();

	if(g_Input.MouseDown(0))
	{
		cursor.m_click = true;
	}
	else
	{
		cursor.m_click = false;
	}

	cursor.GetCursorPosition(mousePos);
	cursor.Show();

	return 1;
}

void CGfxOpenGL::InitGameStart()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

	DrawBoxAlpha(0,windowWidth,90,130); 
	g_GraphicFont->ShowText2D("GAME LOADING...",100,100,20);
	SwapBuffers(GetWindowDC(hWndMain));

	LoadTGATexture("Data\\Texture\\Background\\Title02.tga",&TitleBGTexture);
	LoadTGATexture("Data\\Texture\\Background\\Title02_mask.tga",&TitleBGTexture_mask);
	LoadTGATexture("Data\\Texture\\Background\\Title_back.tga",&TitleBackTexture);
	LoadTGATexture("Data\\Texture\\Background\\Garun_fram.tga",&TitleNameTexture);
	LoadTGATexture("Data\\Texture\\Background\\Garun_fram_mask.tga",&TitleNameTexture_mask);
	LoadTGATexture("Data\\Texture\\Background\\Clound.tga",&TitleCloundTexture);
	LoadTGATexture("Data\\Texture\\Background\\Clound_mask.tga",&TitleCloundTexture_mask);


	LoadTGATexture("Data\\Texture\\Background\\bg_option.tga",&OptionBGTexture);
	LoadTGATexture("Data\\Texture\\Background\\bg_option_mesk.tga",&OptionBGTexture_mask);

	LoadTGATexture("Data\\Texture\\Menu\\start.tga",&button_start.texture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\start_over.tga",&button_start.texture[1]);
	LoadTGATexture("Data\\Texture\\Menu\\start_mask.tga",&button_start.textureMask[0]);

	LoadTGATexture("Data\\Texture\\Menu\\score.tga",&button_score.texture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\score_over.tga",&button_score.texture[1]);
	LoadTGATexture("Data\\Texture\\Menu\\start_mask.tga",&button_score.textureMask[0]);

	LoadTGATexture("Data\\Texture\\Menu\\help.tga",&button_help.texture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\help_over.tga",&button_help.texture[1]);
	LoadTGATexture("Data\\Texture\\Menu\\start_mask.tga",&button_help.textureMask[0]);


	LoadTGATexture("Data\\Texture\\Menu\\quit.tga",&button_quit.texture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\quit_over.tga",&button_quit.texture[1]);
	LoadTGATexture("Data\\Texture\\Menu\\start_mask.tga",&button_quit.textureMask[0]);

	LoadTGATexture("Data\\Texture\\Menu\\option.tga",&button_option.texture[0]);

	LoadTGATexture("Data\\Texture\\Menu\\ok.tga",&button_ok.texture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\ok_over.tga",&button_ok.texture[1]);
	LoadTGATexture("Data\\Texture\\Menu\\start_mask.tga",&button_ok.textureMask[0]);

	LoadTGATexture("Data\\Texture\\Menu\\cancel.tga",&button_cancel.texture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\cancel_over.tga",&button_cancel.texture[1]);
	LoadTGATexture("Data\\Texture\\Menu\\start_mask.tga",&button_cancel.textureMask[0]);

	LoadTGATexture("Data\\Image\\Loading01.tga",&Loading01Texture);
	LoadTGATexture("Data\\Image\\help01.tga",&HelpBox01Texture);

	/*	LoadTGATexture("Data\\Texture\\Menu\\cancel.tga",&button_cancel.texture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\screen800x600.tga",&ScreenBUTTexture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\screen1024x768.tga",&ScreenBUTTexture[1]);
	LoadTGATexture("Data\\Texture\\Menu\\easy.tga",&DifficultBUTTexture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\normal.tga",&DifficultBUTTexture[1]);
	LoadTGATexture("Data\\Texture\\Menu\\hard.tga",&DifficultBUTTexture[2]);
	LoadTGATexture("Data\\Texture\\Menu\\mouse&keyboard.tga",&DeviceBUTTexture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\joystrick.tga",&DeviceBUTTexture[1]);
*/
	lpdms_Open = g_music->LoadMIDI("Data\\Music\\Shop1.mid");
	lpdms_Play01 = g_music->LoadMIDI("Data\\Music\\Ride3.mid");
	lpdms_Win = g_music->LoadMIDI("Data\\Music\\BattleEnd2.mid");

	lpdsb_Open = g_sound->LoadFromFile("Data\\Sound\\RIO.wav");
	lpdsb_Battle = g_sound->LoadFromFile("Data\\Sound\\TRNGPNT.wav");
	lpdsb_BattleBoss = g_sound->LoadFromFile("Data\\Sound\\battle2.wav");
	lpdsb_GameOver = g_sound->LoadFromFile("Data\\Sound\\GameOver2.wav");
	lpdsb_BattleEnd = g_sound->LoadFromFile("Data\\Sound\\BattleEnd2.wav");
	lpdsb_ScoreMode = g_sound->LoadFromFile("Data\\Sound\\LAZYRN.wav");
	lpdsb_SelectMenu = g_sound->LoadFromFile("Data\\Sound\\MENUITEM.wav");
	lpdsb_ShowMenu = g_sound->LoadFromFile("Data\\Sound\\Item1.wav");

	Canvas = new CCanvas2D();	

	FILE *file;
	file = fopen("Data/score.txt","rb");
	fread(stScore,sizeof(stScore),1,file);
	fclose(file);
}

void CGfxOpenGL::StartMode()
{
	glEnable(GL_DEPTH_TEST);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0, windowWidth, 0, windowHeight);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glRasterPos2i(0, 0);
	glLoadIdentity ();

	if(!g_music->IsPlay(lpdms_Open))
	{
		g_music->Play(lpdms_Open);
	}

	int centerX = windowWidth/2;

	button_start.create( centerX-100, centerX+100, 200, 240 );
//	button_option.create( centerX-100, centerX+100, 200, 240 );
	button_score.create(  centerX-100, centerX+100, 150, 190 );
	button_help.create( centerX-100, centerX+100, 100, 140 );
	button_quit.create(  centerX-100, centerX+100, 50, 90 );

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	RECT r_Bacground;

	GetWindowRect(hWndMain, &r_Bacground);

	glBindTexture(GL_TEXTURE_2D,TitleBackTexture);	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2f( r_Bacground.left, r_Bacground.bottom);
		glTexCoord2f(1.0, 1.0); glVertex2f( r_Bacground.right, r_Bacground.bottom);
		glTexCoord2f(1.0, 0.0); glVertex2f( r_Bacground.right, r_Bacground.top);
		glTexCoord2f(0.0, 0.0); glVertex2f( r_Bacground.left, r_Bacground.top);
	glEnd();

	RECT r_Clound;
	r_Clound.left = r_Bacground.left + Clound_Pos;
	r_Clound.right = r_Bacground.right+ Clound_Pos;
	r_Clound.top = r_Bacground.top;
	r_Clound.bottom = r_Bacground.bottom;
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR,GL_ZERO);	
	glBindTexture(GL_TEXTURE_2D,TitleCloundTexture_mask);	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2f( r_Clound.left, r_Clound.bottom);
		glTexCoord2f(1.0, 1.0); glVertex2f( r_Clound.right, r_Clound.bottom);
		glTexCoord2f(1.0, 0.0); glVertex2f( r_Clound.right, r_Clound.top);
		glTexCoord2f(0.0, 0.0); glVertex2f( r_Clound.left, r_Clound.top);
	glEnd();

	glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D,TitleCloundTexture);	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2f( r_Clound.left, r_Clound.bottom);
		glTexCoord2f(1.0, 1.0); glVertex2f( r_Clound.right, r_Clound.bottom);
		glTexCoord2f(1.0, 0.0); glVertex2f( r_Clound.right, r_Clound.top);
		glTexCoord2f(0.0, 0.0); glVertex2f( r_Clound.left, r_Clound.top);
	glEnd();

	glBlendFunc(GL_DST_COLOR,GL_ZERO);	
	glBindTexture(GL_TEXTURE_2D,TitleBGTexture_mask);	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2f( r_Bacground.left, r_Bacground.bottom);
		glTexCoord2f(1.0, 1.0); glVertex2f( r_Bacground.right, r_Bacground.bottom);
		glTexCoord2f(1.0, 0.0); glVertex2f( r_Bacground.right, r_Bacground.top);
		glTexCoord2f(0.0, 0.0); glVertex2f( r_Bacground.left, r_Bacground.top);
	glEnd();

	glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D,TitleBGTexture);	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2f( r_Bacground.left, r_Bacground.bottom);
		glTexCoord2f(1.0, 1.0); glVertex2f( r_Bacground.right, r_Bacground.bottom);
		glTexCoord2f(1.0, 0.0); glVertex2f( r_Bacground.right, r_Bacground.top);
		glTexCoord2f(0.0, 0.0); glVertex2f( r_Bacground.left, r_Bacground.top);
	glEnd();

	RECT r_Name;

	if(m_gameStartMode == ST_SCORE)
	{
		r_Name.left = r_Bacground.left+100;
		r_Name.right = r_Bacground.right-100;
		r_Name.top = (r_Bacground.top+300) + (cos(Name_Pos)*3);
		r_Name.bottom = (r_Bacground.bottom) + (cos(Name_Pos)*3);
	}
	else
	{
		r_Name.left = r_Bacground.left+100;
		r_Name.right = r_Bacground.right-100;
		r_Name.top = (r_Bacground.top+200) + (cos(Name_Pos)*3);
		r_Name.bottom = (r_Bacground.bottom-100) + (cos(Name_Pos)*3);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR,GL_ZERO);	
	glBindTexture(GL_TEXTURE_2D,TitleNameTexture_mask);	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2f( r_Name.left, r_Name.bottom);
		glTexCoord2f(1.0, 1.0); glVertex2f( r_Name.right, r_Name.bottom);
		glTexCoord2f(1.0, 0.0); glVertex2f( r_Name.right, r_Name.top);
		glTexCoord2f(0.0, 0.0); glVertex2f( r_Name.left, r_Name.top);
	glEnd();

	glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D,TitleNameTexture);	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2f( r_Name.left, r_Name.bottom);
		glTexCoord2f(1.0, 1.0); glVertex2f( r_Name.right, r_Name.bottom);
		glTexCoord2f(1.0, 0.0); glVertex2f( r_Name.right, r_Name.top);
		glTexCoord2f(0.0, 0.0); glVertex2f( r_Name.left, r_Name.top);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_ZERO);
	glDisable(GL_BLEND);

	mousePos.x = mX;
	mousePos.y = mY;

//	g_sound->Play(lpdsb_Open);

	if(	m_gameStartMode == ST_NONE )
	{
		if(button_start.Release(mousePos,g_Input.MouseDown(0)))
		{
			lpdsb_SelectMenu->Play(0,0,0);
			g_music->Stop(lpdms_Open);
			ShowCursor(false);
			cursor.ShowCursor(FALSE);
			InitGamePlay();
			return;
		}

		else if(button_option.Release(mousePos,g_Input.MouseDown(0)))
		{
			lpdsb_SelectMenu->Play(0,0,0);
			m_gameStartMode = ST_OPTION;
		}

		else if(button_score.Release(mousePos,g_Input.MouseDown(0)))
		{
			lpdsb_SelectMenu->Play(0,0,0);
			m_gameStartMode = ST_SCORE;
		}

		else if(button_help.Release(mousePos,g_Input.MouseDown(0)))
		{
			lpdsb_SelectMenu->Play(0,0,0);
			m_gameStartMode = ST_HELP;
		}

		else if(button_quit.Release(mousePos,g_Input.MouseDown(0)))
		{
//			lpdsb_SelectMenu->Play(0,0,0);
			m_gameState = QUIT;
			return;
		}

		glDisable(GL_BLEND);

		button_start.Draw();
		button_option.Draw();
		button_score.Draw();
		button_help.Draw();
		button_quit.Draw();
	}

	if(	m_gameStartMode == ST_OPTION )
	{
		OptionMode();
	}
	else if(	m_gameStartMode == ST_SCORE )
	{
		ScoreMode();
	}
	else if( m_gameStartMode == ST_HELP )
	{
		HelpMode();
	}

	glDisable(GL_BLEND);
}

void CGfxOpenGL::OptionMode()
{
/*	int centerX = windowWidth/2;
	int width = 400;
	int height = 200;
	POINT mousePos;
	mousePos.x = mX;
	mousePos.y = mY;


	RECT r_Background;

	r_Background.left = centerX - width/2;
	r_Background.right =  centerX + width/2;
	r_Background.top = 300;
	r_Background.bottom = 50;

	glEnable(GL_BLEND);

	glBlendFunc(GL_DST_COLOR,GL_ZERO);	
	glBindTexture(GL_TEXTURE_2D,OptionBGTexture_mask);	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2f( r_Background.left, r_Background.top);
		glTexCoord2f(1.0, 1.0); glVertex2f( r_Background.right, r_Background.top);
		glTexCoord2f(1.0, 0.0); glVertex2f( r_Background.right, r_Background.bottom);
		glTexCoord2f(0.0, 0.0); glVertex2f( r_Background.left, r_Background.bottom);
	glEnd();

	glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D,OptionBGTexture);	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2f( r_Background.left, r_Background.top);
		glTexCoord2f(1.0, 1.0); glVertex2f( r_Background.right, r_Background.top);
		glTexCoord2f(1.0, 0.0); glVertex2f( r_Background.right, r_Background.bottom);
		glTexCoord2f(0.0, 0.0); glVertex2f( r_Background.left, r_Background.bottom);
	glEnd();

	glDisable(GL_BLEND);

	button_ok.create( centerX-110, centerX-10, 40, 80 );
	button_cancel.create( centerX+10, centerX+110, 40, 80 );
	button_screen.create( centerX+20, centerX+220, 195, 235);
	button_difficult.create( centerX+20, centerX+220, 150, 190);
	button_InputDevice.create( centerX+20, centerX+220, 105, 145);

	if(g_Input.MouseDown(0) && button_InputDevice.mouseOver(mousePos) && mouseUp == true)
	{
		lpdsb_SelectMenu->Play(0,0,0);
		choice_device++;
		if(choice_device>1) choice_device = 0;
		mouseUp = false;
	}
	if(g_Input.MouseDown(0) && button_difficult.mouseOver(mousePos) && mouseUp == true)
	{
		lpdsb_SelectMenu->Play(0,0,0);
		choice_difficult++;
		if(choice_difficult>2) choice_difficult = 0;
		mouseUp = false;
	}
	if(g_Input.MouseDown(0) && button_screen.mouseOver(mousePos) && mouseUp == true)
	{
		lpdsb_SelectMenu->Play(0,0,0);
		choice_screen++;
		if(choice_screen>1) choice_screen = 0;
		mouseUp = false;
	}
	if(button_ok.Release(mousePos, g_Input.MouseDown(0)))
	{
		lpdsb_SelectMenu->Play(0,0,0);
		m_gameStartMode = ST_NONE;
	}
	if(button_cancel.Release(mousePos, g_Input.MouseDown(0)))
	{
		lpdsb_SelectMenu->Play(0,0,0);
		m_gameStartMode = ST_NONE;
	}

	if(g_Input.MouseUp(0)) mouseUp = true;

	button_ok.Draw();
	button_cancel.Draw();

	if(choice_screen == 0)
		button_screen.texture[0] = ScreenBUTTexture[0];
	if(choice_screen == 1)
		button_screen.texture[0] = ScreenBUTTexture[1];

	button_screen.Draw();

	if(choice_difficult == 0)
		button_difficult.texture[0] = DifficultBUTTexture[0];
	if(choice_difficult == 1)
		button_difficult.texture[0] = DifficultBUTTexture[1];
	if(choice_difficult == 2)
		button_difficult.texture[0] = DifficultBUTTexture[2];

	button_difficult.Draw();

	if(choice_device == 0)
		button_InputDevice.texture[0] = DeviceBUTTexture[0];
	if(choice_device == 1)
		button_InputDevice.texture[0] = DeviceBUTTexture[1];

	button_InputDevice.Draw();
*/
}

void CGfxOpenGL::ScoreMode()
{
	char str[32];
	int centerX = windowWidth/2;
	int width = 400;
	int height = 200;

	mousePos.x = mX;
	mousePos.y = mY;

	button_ok.create( centerX-60, centerX+60, 40, 80 );

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor3f(0,0,0);
	glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glVertex2f( 100, 340);
		glVertex2f( 700, 340);
		glVertex2f( 700, 100);
		glVertex2f( 100, 100);
	glEnd();

	glEnable(GL_TEXTURE_2D);

	if(button_ok.Release(mousePos,g_Input.MouseDown(0)))
	{
		lpdsb_SelectMenu->Play(0,0,0);
		m_gameStartMode = ST_NONE;
	}

	for(int i = 0; i < 10; i++)
	{
		if(stScore[i].score >= 0)
		{
			sprintf(str,"%d",i+1);
			g_GraphicFont->ShowText2DR2L(str,200,300-i*20,20);
			g_GraphicFont->ShowText2D(stScore[i].name,250,300-i*20,20);
			sprintf(str,"%d",stScore[i].score);
			g_GraphicFont->ShowText2DR2L(str,600,300-i*20,20);
		}
	}

	button_ok.Draw();
}

void CGfxOpenGL::HelpMode()
{
	char str[32];
	int centerX = windowWidth/2;
	int width = 400;
	int height = 200;
	RECT helpBox = { 100,100, windowWidth-100,windowHeight-100 };

	mousePos.x = mX;
	mousePos.y = mY;

	button_ok.create( centerX-60, centerX+60, 40, 80 );

	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA,GL_ZERO);
	glBegin(GL_QUADS);
	glColor3f(0,0.5,1);
		glVertex2f(helpBox.left-5,helpBox.bottom+5);
		glVertex2f(helpBox.right+5,helpBox.bottom+5);
		glVertex2f(helpBox.right+5,helpBox.top-5);
		glVertex2f(helpBox.left-5,helpBox.top-5);
	glEnd();

	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor3f(0,0,0);
	glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glVertex2f(0,helpBox.bottom+15);
		glVertex2f(windowWidth,helpBox.bottom+15);
		glVertex2f(windowWidth,helpBox.top-15);
		glVertex2f(0,helpBox.top-15);
	glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,HelpBox01Texture);	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2f( helpBox.left, helpBox.bottom);
		glTexCoord2f(1.0, 1.0); glVertex2f( helpBox.right, helpBox.bottom);
		glTexCoord2f(1.0, 0.0); glVertex2f( helpBox.right, helpBox.top);
		glTexCoord2f(0.0, 0.0); glVertex2f( helpBox.left, helpBox.top);
	glEnd();

	if(button_ok.Release(mousePos,g_Input.MouseDown(0)))
	{
		lpdsb_SelectMenu->Play(0,0,0);
		m_gameStartMode = ST_NONE;
	}

	button_ok.Draw();
}

void CGfxOpenGL::InitGamePlay()
{
	lpdsb_Open->Stop();
	lpdsb_Open->SetCurrentPosition(0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,Loading01Texture);	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2f( 0, windowHeight);
		glTexCoord2f(1.0, 1.0); glVertex2f( windowWidth, windowHeight);
		glTexCoord2f(1.0, 0.0); glVertex2f( windowWidth, 0);
		glTexCoord2f(0.0, 0.0); glVertex2f( 0, 0);
	glEnd();

	DrawBoxAlpha(0,windowWidth,90,130); 
	g_GraphicFont->ShowText2D("GAME LOADING...",100,100,20);
	SwapBuffers(GetWindowDC(hWndMain));

	LoadTexture(&TextureSkyBox[SKYBOX_FRONT],"Skybox\\front.bmp");
	LoadTexture(&TextureSkyBox[SKYBOX_BACK],"Skybox\\back.bmp");
	LoadTexture(&TextureSkyBox[SKYBOX_LEFT],"Skybox\\left.bmp");
	LoadTexture(&TextureSkyBox[SKYBOX_RIGHT],"Skybox\\right.bmp");
	LoadTexture(&TextureSkyBox[SKYBOX_TOP],"Skybox\\top.bmp");
	LoadTexture(&TextureSkyBox[SKYBOX_BOTTOM],"Skybox\\bottom.bmp");

	LoadTGATexture("Data\\Texture\\Other\\player_console.tga",&PlayerConsoleTexture);

	ConsolePlayer = new CCanvas2D();

	LoadTGATexture("Data\\Texture\\Other\\player_console.tga",&ConsolePlayer->Texture);
	LoadTGATexture("Data\\Texture\\Other\\player_console_mask.tga",&ConsolePlayer->Texture_Mask);
	LoadTGATexture("Data\\Texture\\Other\\item.tga",&ItemTexture);

	LoadTGATexture("Data\\Texture\\Menu\\resume.tga",&button_resume.texture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\resume_over.tga",&button_resume.texture[1]);
	LoadTGATexture("Data\\Texture\\Menu\\start_mask.tga",&button_resume.textureMask[0]);

	LoadTGATexture("Data\\Texture\\Menu\\return_mainmenu.tga",&button_tostart.texture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\return_mainmenu_over.tga",&button_tostart.texture[1]);
	LoadTGATexture("Data\\Texture\\Menu\\start_mask.tga",&button_tostart.textureMask[0]);

	LoadTGATexture("Data\\Texture\\Menu\\quit.tga",&button_quit.texture[0]);
	LoadTGATexture("Data\\Texture\\Menu\\quit_over.tga",&button_quit.texture[1]);
	LoadTGATexture("Data\\Texture\\Menu\\start_mask.tga",&button_quit.textureMask[0]);

	m_camera.Position_Camera( 15,10.0f,0.0f, 0,5,0.0f, 0,1,0 );

	world = new CWorld;

	if(!world->LoadWorld())
	{
		MessageBox(NULL,"","",MB_OK);
	}

	world->SetSound(g_sound);
	world->LoadEnemy();
	world->ResetTime();

	SetupProjection(windowWidth,windowHeight);

	gamescore = 0;
	count = 8;
	pause = false;

	lpdms_Over = g_music->LoadMIDI("Data\\Music\\GameOver2.mid");
	lpdms_BattleBoss = g_music->LoadMIDI("Data\\Music\\Battle2.mid");

	m_gameState = PLAY;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
}

void CGfxOpenGL::PlayMode()
{
	int numAmmo=0;
	char str[256];

////////// Projection 3D Scene //////////////////////

	SetupProjection(windowWidth,windowHeight);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 5.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_POSITION,light_position);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,light_diffuse);

	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);

	gluLookAt(m_camera.mPos.x ,m_camera.mPos.y, m_camera.mPos.z,
		m_camera.mView.x, m_camera.mView.y, m_camera.mView.z,
		m_camera.mUp.x, m_camera.mUp.y, m_camera.mUp.z);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
//	glDisable(GL_TEXTURE_2D);

	world->Draw(&m_camera);

	if(pause != true)
	{
		if(world->player->bDeath == false)
		{
			world->GameInput();
		}
		world->Prepare();
	}

	Draw_Skybox(0,0,0,600,600,600);

///////////////////////////////////////////////////

////////// 2D Projection /////////////////////////

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0, windowWidth, 0, windowHeight);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glRasterPos2i(0, 0);
	glLoadIdentity ();

	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

/*	glColor3f(0,0,0);
	glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glVertex2f( 0, windowHeight);
		glVertex2f( windowWidth, windowHeight);
		glVertex2f( windowWidth, windowHeight-80);
		glVertex2f( 0, windowHeight-80);
	glEnd();
*/
	glBlendFunc(GL_ONE,GL_SRC_ALPHA);
	glBegin(GL_QUADS);
	glColor4f(0,0,0,0.5f);
		glVertex2f( 200,50);
		glVertex2f( 440,50);
		glVertex2f( 440,20);
		glVertex2f( 200,20);
	glEnd();
	glDisable(GL_BLEND);


	for(int i=0; i<world->player->LifePoint; i++)
		DrawLifeGrade(200+(i*12),210+(i*12),20,50);

	if(world->player->weapon[0])
	{
		numAmmo = 0;
	}

	if(world->player->weapon[1])
	{
		numAmmo = world->player->numArrow;
	}
	
	if(world->player->weapon[2])
	{
		numAmmo = world->player->numTrap;
	}
	
	if(world->player->weapon[3])
	{
		numAmmo = world->player->numLifeBox;
	}

	if(world->player->bSpecielLook)
	{
		float centerX = windowWidth/2;
		float centerY = windowHeight/2;

		glBegin(GL_LINES);
		glColor3f(0,1,0);
			glVertex2f( centerX-10, centerY );
			glVertex2f( centerX+10, centerY );

			glVertex2f( centerX, centerY-10 );
			glVertex2f( centerX, centerY+10 );
		glEnd();
	}

	glEnable(GL_TEXTURE_2D);

	if(world->player->weapon[0])
	{
		ConsolePlayer->DrawPicture(650,20,750,120,
			0,0.75f,0.125f,1,ItemTexture);
	}
	else if(world->player->weapon[1])
	{
		ConsolePlayer->DrawPicture(650,20,750,120,
			0.125f,0.75f,0.25f,1,ItemTexture);
	}
	else if(world->player->weapon[2])
	{
		ConsolePlayer->DrawPicture(650,20,750,120,
			0.25f,0.75f,0.375f,1,ItemTexture);
	}
	else if(world->player->weapon[3])
	{
		ConsolePlayer->DrawPicture(650,20,750,120,
			0.375f,0.75f,0.5f,1,ItemTexture);
	}

	if(gamescore < world->ScoreWorld) 
		gamescore = world->ScoreWorld;

	if(count >= 0)
	{
		DrawBoxAlpha(0,windowWidth,300,390);
		if(count > 4)
		{
			sprintf(str,"LEVEL %d",world->numStage);
			g_GraphicFont->ShowText2D(str,100,300,90);
		}
		else if(count > 1)
		{
			sprintf(str,"%d",(int)count);
			g_GraphicFont->ShowText2D(str,350,300,90);
		}
		else
		{
			g_GraphicFont->ShowText2D("GO",300,300,90);
		}

		world->timeElapsed = 0;
	}

	int time = (int)world->timeElapsed;
	sprintf(str,"TIME %d",time);
	g_GraphicFont->ShowText2D(str,20,550,30);

	sprintf(str,"SCORE");
	g_GraphicFont->ShowText2D(str,400,550,30);

	sprintf(str,"%d",gamescore);
	g_GraphicFont->ShowText2DR2L(str,700,550,30);

	ConsolePlayer->DrawPic(10,-20,200,180,true);
	g_GraphicFont->ShowText2D("Press [F1] - Help",200,70,15,10);

	sprintf(str,"%d",numAmmo);
	g_GraphicFont->ShowText2DR2L(str,710,20,20);

	if(gameover == false) // GAME PLAY
	{
		if(world->player->cowcoin>0)
		{
			world->CreateCow();
			world->player->cowcoin--;
		}
	}
	else // GAME OVER
	{

	}

	// End Game
	if((world->gameDone == true))
	{
		// Game Win
		if(gamewin == true)
		{
			lpdsb_BattleEnd->Stop();
			lpdsb_BattleEnd->SetCurrentPosition(0);
			gamewin = false;

			glClearColor(0.0, 0.0, 0.0, 0.0);
			lpdsb_ScoreMode->Play(0,0,0);
			m_gameState = PROCESS_SCORE;
			ShowCursor(TRUE);
			cursor.ShowCursor(TRUE);

			curtain_alpha = 1.0f;
			return;
		}

		g_music->Stop(lpdms_Play01);
		g_music->Stop(lpdms_BattleBoss);

		lpdsb_BattleEnd->Play(0,0,0);
		DrawBoxAlpha(0,windowWidth,300,350);
	 	g_GraphicFont->ShowText2D("GAME WIN",230,300,50);
	

		if(curtain_alpha <= 0)
		{
			gamewin = true;
		}
		else
		{
			curtain_alpha -= 0.0015f / curtain_alpha;
		}
	}
	else
	{
		// All Cow Death
		if(world->numCow <= 0)
		{
			if(g_music->IsPlay(lpdms_Over) == false)
			{
				g_music->Play(lpdms_Over);
			}

			gameover = true;

			DrawBoxAlpha(0,windowWidth,300,350);
			g_GraphicFont->ShowText2D("GAME OVER",180,300,50);

			if(curtain_alpha <= 0)
			{
				if((gameover==true) && (pause != true) )
				{
					glClearColor(0.0, 0.0, 0.0, 0.0);
					m_gameState = PROCESS_SCORE;
					ShowCursor(TRUE);
					cursor.ShowCursor(TRUE);

					curtain_alpha = 1.0f;
					sprintf(MessageChar,"");
					return;
				}
			}
			else
			{
				curtain_alpha -= 0.002f / curtain_alpha;
			}	
		}

		// Player Death
		else if(world->player->bDeath == true)
		{
//			g_music->Stop(lpdms_BattleBoss);

			if(g_music->IsPlay(lpdms_Over) == false)
			{
				g_music->Play(lpdms_Over);
			}

			gameover = true;

			DrawBoxAlpha(0,windowWidth,300,350);
			g_GraphicFont->ShowText2D("GAME OVER",180,300,50);

			if(curtain_alpha <= 0)
			{
				if((gameover==true) && (pause != true) )
				{
					glClearColor(0.0, 0.0, 0.0, 0.0);
					m_gameState = PROCESS_SCORE;
					ShowCursor(TRUE);
					cursor.ShowCursor(TRUE);

					curtain_alpha = 1.0f;
					return;
				}
			}
			else
			{
				curtain_alpha -= 0.002f / curtain_alpha;
			}	
		}

		//	Battle Boss
		else if(world->CountObjectTypes(world->enemy,MD_ENEMY) <= 0)
		{
			if(!g_music->IsPlay(lpdms_BattleBoss))
			{
				g_music->Stop(lpdms_Play01);
				g_music->Play(lpdms_BattleBoss);
			}
		}
		else
		{
			if(!g_music->IsPlay(lpdms_Play01))
			{
				g_music->Play(lpdms_Play01);
			}
		}
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_SRC_ALPHA);
	glBegin(GL_QUADS);
	glColor4f(0,0,0,curtain_alpha);
		glVertex2f(			 0,windowHeight);
		glVertex2f(windowWidth,windowHeight);
		glVertex2f(windowWidth,			  0);
		glVertex2f(			 0,			  0);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	if(Keys[VK_ESCAPE] == TRUE)
	{
		if(pause == false)
		{
			g_sound->Play(lpdsb_ShowMenu);
//			lpdsb_ShowMenu->Play(0,0,0);
			m_gameDlg = DLG_PLAYMODE_MENU;
			pause = true;
			ShowCursor(TRUE);
			cursor.ShowCursor(TRUE);
		}
		else
		{
			g_sound->Play(lpdsb_ShowMenu);
//			lpdsb_ShowMenu->Play(0,0,0);
			m_gameDlg = DLG_NONE;
			pause = false;
			ClearBufferKey();
			ShowCursor(FALSE);
			cursor.ShowCursor(FALSE);
		}

		Keys[VK_ESCAPE] = FALSE;
	}

	if(Keys[VK_RETURN] == TRUE)
	{
		if(bGetchar != true)
		{
			SetKeyBuffer();
//			bGetchar = true;
			pause = true;
		}
		else 
		{
			MessageChar[strlen(MessageChar)-1] = NULL;
			CommandLine(MessageChar);
			sprintf(MessageChar,"");
			SetKeyBuffer(0);
//			bGetchar = false;
			pause = false;
			ClearBufferKey();
		}
	
		Keys[VK_RETURN] = false;
	}

	if((Keys[VK_F1] == TRUE) && (m_gameDlg != DLG_PLAYMODE_MENU))
	{
		if((m_gameDlg != DLG_HELP01) && (m_gameDlg != DLG_PLAYMODE_MENU))
		{
			g_sound->Play(lpdsb_ShowMenu);
			m_gameDlg = DLG_HELP01;
			pause = true;
			cursor.ShowCursor(TRUE);
		}
		else
		{
			m_gameDlg = DLG_NONE;
			g_sound->Play(lpdsb_ShowMenu);
			pause = false;
			cursor.ShowCursor(FALSE);
		}

		Keys[VK_F1] = FALSE;
	}

	if(bGetchar == true)
	{
		int num = strlen(MessageChar);
	
		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glColor3f(0,0,0);
		glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
		glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
			glVertex2f( 0, 190);
			glVertex2f( windowWidth, 190);
			glVertex2f( windowWidth, 230);
			glVertex2f( 0, 230);
		glEnd();

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		sprintf(str,"Enter Command >");
		g_GraphicFont->ShowText2D(str,20,200,15);

		sprintf(str,"%s",MessageChar);
		g_GraphicFont->ShowText2D(str,250,200,15);
				
		if(b_command == true)
		{
			g_GraphicFont->ShowText2D("_",250+(15*num),200,15,10);
		}
	
		return;
	}

	////////////////////////////////////////////////
}

void CGfxOpenGL::DestroyGamePlay()
{
	delete world;
	delete ConsolePlayer;
}

void CGfxOpenGL::ProcessScoreMode()
{	
////////// Projection 3D Scene //////////////////////

	SetupProjection(windowWidth,windowHeight);

	m_camera.Position_Camera( 0,5,0.0f, 30,10,-30, 0,1,0 );

	gluLookAt(m_camera.mPos.x ,m_camera.mPos.y, m_camera.mPos.z,
		m_camera.mView.x, m_camera.mView.y, m_camera.mView.z,
		m_camera.mUp.x, m_camera.mUp.y, m_camera.mUp.z);

	world->garden->Render();
	Draw_Skybox(0,0,0,600,600,600);

////////// 2D Projection /////////////////////////

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0, windowWidth, 0, windowHeight);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glRasterPos2i(0, 0);
	glLoadIdentity ();

	char str[32];
	int sum_score = 0;
	POINT mousePos;
	mousePos.x = mX;
	mousePos.y = mY;
	int time = 0;

	glColor3f(1,0,0);

	float centerX = windowWidth/2;

	button_ok.create( centerX-60, centerX+60, 40, 80 );
	
	glEnable(GL_TEXTURE_2D);

	if(gameover != true)
	{
		sprintf(str,"Stage %d Complete",world->numStage);
		g_GraphicFont->ShowText2D(str,60,500,50,40);	
	}
	else
	{
		sprintf(str,"Stage %d Failed",world->numStage);
		g_GraphicFont->ShowText2D(str,100,500,50,40);	
	}

	time = (int)world->timeElapsed;
	sprintf(str,"Time Stage %d",time);
 	g_GraphicFont->ShowText2D(str,100,400,30);

	sprintf(str,"Score = %d",gamescore);
 	g_GraphicFont->ShowText2D(str,100,360,30);
	sum_score += gamescore;


	if(gameover != true)
	{
		sprintf(str,"Cow x %d = %d",world->CountObjectTypes(world->garden,MD_FRIEND),
			world->CountObjectTypes(world->garden,MD_FRIEND)*200);
		g_GraphicFont->ShowText2D(str,100,320,30);
		sum_score += world->CountObjectTypes(world->garden,MD_FRIEND)*200;
	}
	else
	{
		sprintf(str,"Cow x %d = NONE",world->CountObjectTypes(world->garden,MD_FRIEND));
		g_GraphicFont->ShowText2D(str,100,320,30);
	}

	int stage_score = 0;

	if(gameover != true)
	{
//		sum_score += time*50;

		if(world->numStage == 1)
			stage_score = 1000;
		else if(world->numStage == 2)
			stage_score = 2000;
		else if(world->numStage == 3)
			stage_score = 3000;
		else if(world->numStage == 4)
			stage_score = 4000;
		else if(world->numStage == 5)
			stage_score = 5000;
		else if(world->numStage == 6)
			stage_score = 6000;

		sprintf(str,"Bonus Stage = %d",stage_score);
		g_GraphicFont->ShowText2D(str,100,280,30);
		sum_score += stage_score;
	}
	else
	{
		sprintf(str,"Bonus Stage = NONE");
		g_GraphicFont->ShowText2D(str,100,280,30);
	}

//	sum_score += world->player->playerScore;

	sprintf(str,"Total Score = %d",sum_score);
 	g_GraphicFont->ShowText2D(str,100,240,30);

	if(button_ok.Release((mousePos), g_Input.MouseDown(0)))
	{
		lpdsb_ScoreMode->Stop();

		if((gameover != true) && (world->numStage < 5))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
			glLoadIdentity();									// Reset The Current Modelview Matrix
			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,Loading01Texture);	
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0); glVertex2f( 0, windowHeight);
				glTexCoord2f(1.0, 1.0); glVertex2f( windowWidth, windowHeight);
				glTexCoord2f(1.0, 0.0); glVertex2f( windowWidth, 0);
				glTexCoord2f(0.0, 0.0); glVertex2f( 0, 0);
			glEnd();

			DrawBoxAlpha(0,windowWidth,90,130); 
			g_GraphicFont->ShowText2D("GAME LOADING...",100,100,20);
			SwapBuffers(GetWindowDC(hWndMain));

			SetupProjection(windowWidth,windowHeight);
			m_camera.Position_Camera( 15,10.0f,0.0f, 0,5,0.0f, 0,1,0 );



			int num_t = world->CountObjectTypes(world->garden, MD_TRAP) + 
				world->player->numTrap;

			world->gameDone = false;
			world->numStage++;

			world->LoadWorld();
			world->player->numTrap = num_t;
			world->timeElapsed = 0;

			world->ResetTime();

			world->player->position.x = 0;
			world->player->position.z = 0;

			world->player->diamond = 0;
			world->player->giantdiamond = 0;
			world->player->playerScore = sum_score;

			count = 10;
			pause = false;

			m_gameState = PLAY;
			ShowCursor(FALSE);
			cursor.ShowCursor(FALSE);
			g_music->Play(lpdms_Play01);
			return;	
		}
		else
		{
			gamescore = world->player->playerScore;

			if(gamescore>=0)
			{
				for(int i=0; i<10; i++)
				{
					if((stScore[i].score <= gamescore))
					{
						player_score_no = i;

						for(int j=9; j>player_score_no; j--)
						{
							sprintf(stScore[j].name,"%s",stScore[j-1].name);
							stScore[j].score = stScore[j-1].score;
						}

						stScore[player_score_no].score = gamescore;
//						gamescore = 0;
						i=10;
					}
					else
					{
						player_score_no = -1;
					}
				}

			}
			else
			{
				player_score_no = -1;
			}

			g_music->Stop(lpdms_Over);
			delete world->garden;
			m_gameState = MEM_SCORE;

			return;
		}
	}

	button_ok.Draw();
}

void CGfxOpenGL::MemScoreMode()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);	

	glMatrixMode (GL_PROJECTION);

	glLoadIdentity ();

	gluOrtho2D (0, windowWidth, 0, windowHeight);

	glMatrixMode (GL_MODELVIEW);

	glLoadIdentity ();
	
	glRasterPos2i(0, 0);

	glLoadIdentity ();

	char str[256];
	int num = strlen(MessageChar);
	float centerX = windowWidth/2;
//	int width = 400;
//	int height = 200;

	sprintf(str,"HIGH SCORE");
 	g_GraphicFont->ShowText2D(str,250,500,30);

	for(int i = 0; i < 10; i++)
	{
		if(i==player_score_no)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 0.0f);

			glEnable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glColor3f(0,0,0);
			glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
				glVertex2f( 0, 350-(i*20));
				glVertex2f( windowWidth, 350-(i*20));
				glVertex2f( windowWidth, 350-(i*20) + 20);
				glVertex2f( 0, (350-i*20) + 20);
			glEnd();

			glEnable(GL_TEXTURE_2D);

			if(b_command == true)
			{
				g_GraphicFont->ShowText2D("_",
					150+(strlen(stScore[player_score_no].name)*20),
					350-i*20,
					20);
			}
		}

		if(stScore[i].score >= 0)
		{
			sprintf(str,"%d",i+1);
			g_GraphicFont->ShowText2DR2L(str,100,350-i*20,20);
			sprintf(str,"%s",stScore[i].name);
			g_GraphicFont->ShowText2D(str,150,350-i*20,20);
			sprintf(str,"%d",stScore[i].score);
			g_GraphicFont->ShowText2DR2L(str,500,350-i*20,20);

		}
	}

	POINT mousePos;
	mousePos.x = mX;
	mousePos.y = mY;

	sprintf(stScore[player_score_no].name,"%s",MessageChar);

	if(player_score_no != -1)
	{
//		bGetchar = true;
		SetKeyBuffer(7);

		sprintf(str,"Score No. %d",player_score_no+1);
		g_GraphicFont->ShowText2D(str,200,450,20);
		sprintf(str,"Enter Name :");
		g_GraphicFont->ShowText2D(str,200,400,20);

		sprintf(str,"%s",MessageChar);
		g_GraphicFont->ShowText2D(str,450,400,20);

		if(b_command == true)
		{
			g_GraphicFont->ShowText2D("_",450+(20*num),400,20);
		}
	}
	else
	{	 
		SetKeyBuffer(7);
		g_GraphicFont->ShowText2D("Your Score Aren't On Chart",150,450,20);
	}

	if(m_gameDlg == DLG_NONE)
	{
		button_ok.create( centerX-60, centerX+60, 40, 80 );
		button_ok.Draw();

		if(button_ok.Release((mousePos), g_Input.MouseDown(0)))
		{
			if((player_score_no!=-1)&&(strlen(stScore[player_score_no].name) == 0))
			{
				lpdsb_SelectMenu->Play(0,0,0);
				m_gameDlg = DLG_CONFIRM_MEMSCORE;
			}
			else
			{
				lpdsb_SelectMenu->Play(0,0,0);
				FILE *file;
				file = fopen("Data/score.txt","w");
				fwrite(stScore,sizeof(stScore),1,file);
				fclose(file);

				m_gameState = START;
				bGetchar = false;
			}
		}
	}
}

void CGfxOpenGL::DlgDraw()
{
	char str[32];
	int centerX = windowWidth/2;
	int centerY = windowHeight/2;
	mousePos.x = mX;
	mousePos.y = mY;
	RECT helpBox = { 0,0, windowWidth,windowHeight };

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	switch(m_gameDlg)
	{
		case DLG_NONE:

		break;

		case DLG_PLAYMODE_MENU:
		{
			button_resume.create(centerX-100, centerX+100, centerY+30 ,centerY+70 );
			button_tostart.create(centerX-100, centerX+100, centerY-20 ,centerY+20 );
			button_quit.create(centerX-100, centerX+100, centerY-70 ,centerY-30);
	
			if(button_resume.Release(mousePos,g_Input.MouseDown(0)))
			{
				ShowCursor(FALSE);
				cursor.ShowCursor(FALSE);
				pause = false;
				m_gameDlg = DLG_NONE;
				lpdsb_SelectMenu->Play(0,0,0);
			}
			else if(button_tostart.Release(mousePos,g_Input.MouseDown(0)))
			{
				lpdsb_SelectMenu->Play(0,0,0);
				m_gameDlg = DLG_CONFIRM_TOMAINMENU;
						}
			else if(button_quit.Release(mousePos,g_Input.MouseDown(0)))
			{
				m_gameDlg = DLG_CONFIRM_QUIT;
				lpdsb_SelectMenu->Play(0,0,0);
				return;
			}
			else
			{
				button_resume.Draw();
				button_quit.Draw();
				button_tostart.Draw();
			}
		}
		break;

		case DLG_CONFIRM_QUIT:
		{
			glEnable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glColor3f(0,0,0);
			glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
				glVertex2f( 0, centerY + 40);
				glVertex2f( windowWidth, centerY + 40);
				glVertex2f( windowWidth, centerY - 40);
				glVertex2f( 0, centerY - 40);
			glEnd();

			glEnable(GL_TEXTURE_2D);
			button_ok.create( centerX-130, centerX-30, centerY-90, centerY-50 );
			button_cancel.create( centerX+30, centerX+130, centerY-90, centerY-50 );	

			g_GraphicFont->ShowText2D("Are you want quit?",centerX-170,centerY,20);

			if(button_ok.Release(mousePos,g_Input.MouseDown(0)))
			{
				m_gameState = QUIT;		
			}
			else if(button_cancel.Release(mousePos,g_Input.MouseDown(0)))
			{
				m_gameDlg = DLG_PLAYMODE_MENU;
				lpdsb_SelectMenu->Play(0,0,0);
			}

			button_ok.Draw();
			button_cancel.Draw();
		}
		break;

		case DLG_CONFIRM_TOMAINMENU:
		{
			glEnable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glColor3f(0,0,0);
			glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
				glVertex2f( 0, centerY + 40);
				glVertex2f( windowWidth, centerY + 40);
				glVertex2f( windowWidth, centerY - 40);
				glVertex2f( 0, centerY - 40);
			glEnd();

			glEnable(GL_TEXTURE_2D);
			button_ok.create( centerX-130, centerX-30, centerY-90, centerY-50 );
			button_cancel.create( centerX+30, centerX+130, centerY-90, centerY-50 );	

			g_GraphicFont->ShowText2D("Are you want to go mainmenu?",centerX-270,centerY,20);

			if(button_ok.Release(mousePos,g_Input.MouseDown(0)))
			{
				lpdsb_SelectMenu->Play(0,0,0);
				g_music->Stop(lpdms_Play01);
				curtain_alpha = 1.0f;
				m_gameState = START;
				delete world->garden;
				m_gameDlg = DLG_NONE;
			}
			else if(button_cancel.Release(mousePos,g_Input.MouseDown(0)))
			{
				lpdsb_SelectMenu->Play(0,0,0);
				m_gameDlg = DLG_PLAYMODE_MENU;
			}

			button_ok.Draw();
			button_cancel.Draw();
		}
		break;

		case DLG_CONFIRM_MEMSCORE:
		{
			glEnable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glColor3f(0,0,0);
			glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
				glVertex2f( 0, centerY + 40);
				glVertex2f( windowWidth, centerY + 40);
				glVertex2f( windowWidth, centerY - 40);
				glVertex2f( 0, centerY - 40);
			glEnd();

			glEnable(GL_TEXTURE_2D);
			button_ok.create( centerX-60, centerX+60, centerY-90, centerY-50 );

			g_GraphicFont->ShowText2D("Don't Input Your Name?",centerX-200,centerY,20);

			if(button_ok.Release(mousePos,g_Input.MouseDown(0)))
			{
				lpdsb_SelectMenu->Play(0,0,0);
				m_gameDlg = DLG_NONE;
			}

			button_ok.Draw();
//			button_cancel.Draw();
		}
		break;

		case DLG_HELP01:
		{
			glEnable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glBlendFunc(GL_SRC_ALPHA,GL_ZERO);
			glBegin(GL_QUADS);
			glColor3f(1,1,1);
				glVertex2f(helpBox.left-10,helpBox.bottom+10);
				glVertex2f(helpBox.right+10,helpBox.bottom+10);
				glVertex2f(helpBox.right+10,helpBox.top-10);
				glVertex2f(helpBox.left-10,helpBox.top-10);
			glEnd();

			glDisable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,HelpBox01Texture);	
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0); glVertex2f( helpBox.left, helpBox.bottom);
				glTexCoord2f(1.0, 1.0); glVertex2f( helpBox.right, helpBox.bottom);
				glTexCoord2f(1.0, 0.0); glVertex2f( helpBox.right, helpBox.top);
				glTexCoord2f(0.0, 0.0); glVertex2f( helpBox.left, helpBox.top);
			glEnd();

			g_GraphicFont->ShowText2D("Press [ESC] - Exit",600,30,15,10);
		}
		break;

		case DLG_HELP02:
		{

		}
		break;

	}
	glEnable(GL_TEXTURE_2D);
}

void CGfxOpenGL::ConfirmQuit()
{
	float centerX = windowWidth/2;
	float centerY = windowHeight/2;
}

void CGfxOpenGL::Loading()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode (GL_PROJECTION);

	glLoadIdentity ();

	gluOrtho2D (0, windowWidth, 0, windowHeight);

	glMatrixMode (GL_MODELVIEW);

	glLoadIdentity ();
	
	glRasterPos2i(0, 0);

	glLoadIdentity ();

	RECT r_Bacground;

	GetWindowRect(hWndMain, &r_Bacground);

	glDisable(GL_BLEND);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
		glVertex2f( r_Bacground.left, r_Bacground.bottom);
		glVertex2f( r_Bacground.right, r_Bacground.bottom);
		glVertex2f( r_Bacground.right, r_Bacground.top);
		glVertex2f( r_Bacground.left, r_Bacground.top);
	glEnd();

}

void CGfxOpenGL::GetKeyChar(char keychar, int numLimit)
{
	if(bGetchar != true)
	{
		sprintf(MessageChar,"");
	}
	else
	{
		if((keychar != 8))
		{
			if(strlen(MessageChar) < key_limit)
				sprintf(MessageChar,"%s%c",MessageChar,keychar);
		}
		else
		{
			MessageChar[strlen(MessageChar)-1] = NULL;
		}
	}
}

void CGfxOpenGL::Destroy()
{
	world->Destroy();
	delete world;
}

void CGfxOpenGL::CommandLine(char *text)
{
	if(strcmpi("",text) == 0)
	{
		return;
	}
	else if(strcmpi("/Quit",text) == 0)
	{
		m_gameState = QUIT;
		return;
	}
	else if(strcmpi("/Get The Score",text) == 0)
	{
		world->player->playerScore += 5000;
		return;
	}
	else if(strcmpi("/Kill All Cow",text) == 0)
	{
		CObject *c1;
		CObject *c2;

		c1 = (CObject*)world->garden->childNode;

		while (c1 != NULL)
		{
			c2 = c1;
			do
			{
				if (c2->type == MD_FRIEND)
				{
					c2->isDead = true;
				}

				c2 = (CObject*)c2->nextNode;
			}while (!c2->IsFirstChild());

			c1 = (CObject*)c1->childNode;
		}
	}
	else if(strcmpi("/Love The Score",text) == 0)
	{
		for(int i = 0; i < 10; i++)
		{
			if(stScore[i].score >= 0)
			{
				stScore[i].score = 0;
			}

			if(stScore[i].name)
			{
				for(int j = 0; j<strlen(stScore[i].name); j++)
				{
					stScore[i].name[j] = NULL;
				}
			}
		}

		FILE *file;
		file = fopen("Data/score.txt","w");
		fwrite(stScore,sizeof(stScore),1,file);
		fclose(file);
	}
	else if(strcmpi("/Kick Player",text) == 0)
	{
		world->player->LifePoint = 0;
	}
	else if(strcmpi("/Give me the arrow",text) == 0)
	{
		world->player->numArrow += 20;
	}
	else if(strcmpi("/Give me the red box",text) == 0)
	{
		world->player->numLifeBox += 5;
	}
	else if(strcmpi("/Mission Complete",text) == 0)
	{
		world->gameDone = true;	
	}
	else if(strcmpi("/Boot speed",text) == 0)
	{

	}
	else if(strcmpi("/Kill enemy on farm",text) == 0)
	{
		CObject *c1;
		CObject *c2;

		c1 = (CObject*)world->garden->childNode;

		while (c1 != NULL)
		{
			c2 = c1;
			do
			{
				if (c2->type == MD_ENEMY)
				{
					c2->isDead = true;
				}

				c2 = (CObject*)c2->nextNode;
			}while (!c2->IsFirstChild());

			c1 = (CObject*)c1->childNode;
		}		
	}
	else if(strcmpi("/Test Boss",text) == 0)
	{
		CObject *c1;
		CObject *c2;

		c1 = (CObject*)world->enemy->childNode;

		while (c1 != NULL)
		{
			c2 = c1;
			do
			{
				if (c2->type == MD_ENEMY)
				{
					c2->isDead = true;
				}

				c2 = (CObject*)c2->nextNode;
			}while (!c2->IsFirstChild());

			c1 = (CObject*)c1->childNode;
		}
	}
	else if(strcmpi("/Magic Win",text) == 0)
	{
		world->gameDone = true;	
		gamewin = true;
	}
	else if(strcmpi("/Mission Clear",text) == 0)
	{
		world->gameDone = true;	
		gamewin = true;
		world->numStage = 6;
	}
	else if(strcmpi("/Get Medicine",text) == 0)
	{
		world->player->LifePoint = 20;
	}
	else if(strcmpi("/End Game",text) == 0)
	{
		world->player->LifePoint = 0;
		gameover=true;
		curtain_alpha = 0;
	}
	else if(strcmpi("/Present Time",text) == 0)
	{
		world->PresentTime = true;
	}
}