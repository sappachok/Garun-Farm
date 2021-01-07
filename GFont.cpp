#ifndef GL_GRAPHIC_FONT_CPP
#define GL_GRAPHIC_FONT_CPP
#endif

#include "GFont.h"

BOOL GFont::LoadTextureTGA(char *filename)
{
	CTargaImage	*texture;
	texture = new CTargaImage;
	
	glEnable(GL_TEXTURE_2D);

	if(!texture->Load(filename))
	{
		MessageBox(NULL,"NONE IMAGE","MSG",MB_OK);
		return FALSE;
	}

	glGenTextures(1, &FontTexture);

	glBindTexture(GL_TEXTURE_2D, FontTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// now that the texture object is bound, specify a texture for it
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB4, texture->GetWidth(), texture->GetHeight(),
				 0, GL_RGB, GL_UNSIGNED_BYTE, texture->GetImage());

	return TRUE;
}

BOOL GFont::LoadTextureMaskTGA(char *filename)
{
	CTargaImage	*texture;
	texture = new CTargaImage;
	
	glEnable(GL_TEXTURE_2D);

	if(!texture->Load(filename))
	{
		MessageBox(NULL,"NONE IMAGE","MSG",MB_OK);
		return FALSE;
	}

	glGenTextures(1, &FontTextureMask);

	glBindTexture(GL_TEXTURE_2D, FontTextureMask);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// now that the texture object is bound, specify a texture for it
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB4, texture->GetWidth(), texture->GetHeight(),
				 0, GL_RGB, GL_UNSIGNED_BYTE, texture->GetImage());

	return TRUE;
}

void GFont::ShowText2D(char *text, float posx, float posy, int fontsize)
{
	char str[256];
	int  length;

	if(!fontsize)
		fontsize = size;
	else
		size = fontsize;

	sprintf(str,text);

	length = strlen(str);

	for(int i=0; i<length; i++)
	{
		DrawChar(str[i],posx+(size*i),posy);
	}
}

void GFont::ShowText2DR2L(char *text, float posx, float posy, int fontsize)
{
	char str[256];
	int  length;

	if(!fontsize)
		fontsize = size;
	else
		size = fontsize;

	sprintf(str,text);

	length = strlen(str);

	for(int i=length-1; i>=0; i--)
	{
		DrawChar(str[i],posx+(size*+(i-length+1)),posy);
	}
}

void GFont::ShowText2D(char *text, float posx, float posy, int fontsize, int width_char)
{
	char str[256];
	int  length;

	if(!fontsize)
		fontsize = size;
	else
		size = fontsize;

	sprintf(str,text);

	length = strlen(str);

	for(int i=0; i<length; i++)
	{
		DrawChar(str[i],posx+(width_char*i),posy);
	}
}
 
void GFont::DrawChar(char c, float posx, float posy)
{
	int row,col;
	float xmin,xmax,ymin,ymax;
	switch(c)
	{
	case ' ':
		row =1; 
		col =1;
		break;
	case '!':

		break;
	case '"':

		break;
	case '#':

		break;
	case '-':
		row =1; 
		col =13;
		break;
	case '.':
		row =1; 
		col =14;
		break;
	case '/':
		row =1;
		col =15;
		break;
	case ':':
		row =2; 
		col =10;
		break;

	case '0':
		row =1; 
		col =16;
		break;
	case '1':
		row =2; 
		col =1;
		break;
	case '2':
		row =2; 
		col =2;
		break;
	case '3':
		row =2; 
		col =3;
		break;
	case '4':
		row =2; 
		col =4;
		break;
	case '5':
		row =2; 
		col =5;
		break;
	case '6':
		row =2; 
		col =6;
		break;
	case '7':
		row =2; 
		col =7;
		break;
	case '8':
		row =2; 
		col =8;
		break;
	case '9':
		row =2; 
		col =9;
		break;
	case '<':
		row =2; 
		col =12;
		break;
	case '=':
		row =2; 
		col =13;
		break;
	case '>':
		row =2; 
		col =14;
		break;

	case 'A':
		row =3; 
		col =1;
		break;
	case 'B':
		row =3; 
		col =2;
		break;
	case 'C':
		row =3; 
		col =3;
		break;
	case 'D':
		row =3; 
		col =4;
		break;
	case 'E':
		row =3; 
		col =5;
		break;
	case 'F':
		row =3; 
		col =6;
		break;
	case 'G':
		row =3; 
		col =7;
		break;
	case 'H':
		row =3; 
		col =8;
		break;
	case 'I':
		row =3; 
		col =9;
		break;
	case 'J':
		row =3; 
		col =10;
		break;
	case 'K':
		row =3; 
		col =11;
		break;
	case 'L':
		row =3; 
		col =12;
		break;
	case 'M':
		row =3; 
		col =13;
		break;
	case 'N':
		row =3; 
		col =14;
		break;
	case 'O':
		row =3; 
		col =15;
		break;
	case 'P':
		row =3; 
		col =16;
		break;
	case 'Q':
		row =4; 
		col =1;
		break;
	case 'R':
		row =4; 
		col =2;
		break;
	case 'S':
		row =4; 
		col =3;
		break;
	case 'T':
		row =4; 
		col =4;
		break;
	case 'U':
		row =4; 
		col =5;
		break;
	case 'V':
		row =4; 
		col =6;
		break;
	case 'W':
		row =4; 
		col =7;
		break;
	case 'X':
		row =4; 
		col =8;
		break;
	case 'Y':
		row =4; 
		col =9;
		break;
	case 'Z':
		row =4; 
		col =10;
		break;

	case '[':
		row =4; 
		col =11;
		break;

	case ']':
		row =4; 
		col =13;
		break;


	case 'a':
		row =5; 
		col =1;
		break;
	case 'b':
		row =5; 
		col =2;
		break;
	case 'c':
		row =5; 
		col =3;
		break;
	case 'd':
		row =5; 
		col =4;
		break;
	case 'e':
		row =5; 
		col =5;
		break;
	case 'f':
		row =5; 
		col =6;
		break;
	case 'g':
		row =5; 
		col =7;
		break;
	case 'h':
		row =5; 
		col =8;
		break;
	case 'i':
		row =5; 
		col =9;
		break;
	case 'j':
		row =5; 
		col =10;
		break;
	case 'k':
		row =5; 
		col =11;
		break;
	case 'l':
		row =5; 
		col =12;
		break;
	case 'm':
		row =5; 
		col =13;
		break;
	case 'n':
		row =5; 
		col =14;
		break;
	case 'o':
		row =5; 
		col =15;
		break;
	case 'p':
		row =5; 
		col =16;
		break;
	case 'q':
		row =6; 
		col =1;
		break;
	case 'r':
		row =6; 
		col =2;
		break;
	case 's':
		row =6; 
		col =3;
		break;
	case 't':
		row =6; 
		col =4;
		break;
	case 'u':
		row =6; 
		col =5;
		break;
	case 'v':
		row =6; 
		col =6;
		break;
	case 'w':
		row =6; 
		col =7;
		break;
	case 'x':
		row =6; 
		col =8;
		break;
	case 'y':
		row =6; 
		col =9;
		break;
	case 'z':
		row =6; 
		col =10;
		break;

	case '_':
		row =4; 
		col =15;
		break;

	default:
		return;
		break;
	}

	row = 16 - (row-1);
	
	xmin = ((float)(col-1))/16.0f;
	xmax = ((float)col)/16.0f;
	ymin = ((float)(row-1))/16.0f;
	ymax = ((float)row)/16.0f;

	if(FontTextureMask)
	{
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR,GL_ZERO);	
	glBindTexture(GL_TEXTURE_2D,FontTextureMask);	
	glBegin(GL_QUADS);
		glTexCoord2f(xmin, ymax); glVertex2f( posx, posy+size);
		glTexCoord2f(xmax, ymax); glVertex2f( posx+size, posy+size);
		glTexCoord2f(xmax, ymin); glVertex2f( posx+size, posy);
		glTexCoord2f(xmin, ymin); glVertex2f( posx, posy);
	glEnd();

	glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D,FontTexture);	
	glBegin(GL_QUADS);
		glTexCoord2f(xmin, ymax); glVertex2f( posx, posy+size);
		glTexCoord2f(xmax, ymax); glVertex2f( posx+size, posy+size);
		glTexCoord2f(xmax, ymin); glVertex2f( posx+size, posy);
		glTexCoord2f(xmin, ymin); glVertex2f( posx, posy);
	glEnd();
	glDisable(GL_BLEND);
	}
	else
	{
	glBindTexture(GL_TEXTURE_2D,FontTexture);	
	glBegin(GL_QUADS);
		glTexCoord2f(xmin, ymax); glVertex2f( posx, posy+size);
		glTexCoord2f(xmax, ymax); glVertex2f( posx+size, posy+size);
		glTexCoord2f(xmax, ymin); glVertex2f( posx+size, posy);
		glTexCoord2f(xmin, ymin); glVertex2f( posx, posy);
	glEnd();
	}
}