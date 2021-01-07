
#ifndef _3DSLOADER_H
#define _3DSLOADER_H

#pragma comment(lib, "data/lib/jpeg.lib")

#include "CJPEGFile.h"
#include "winmain.h"
#include "3ds.h"

#include "object.h"


//////////////////////////////////////
//The 3dsLoader Class
//////////////////////////////////////
class C3dsLoader:public CObject
{
	public:
		int	numObject;
		tBounding bbox[500];

		CLoad3DS mLoad3ds;						
		UINT TextureArray3ds[MAXTEXTURE];	
		t3DModel m3DModel;	

		void Init_3ds(char* filename);
		void Init_3ds(char* filename, char* tex_filename);		
		void Render_3ds();
		void Texture_3ds(UINT textureArray[], LPSTR strFileName, int ID);
		void Unload_3ds();
};


#endif

//Ronny André Reierstad
//www.morrowland.com
//apron@morrowland.com
