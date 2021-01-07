/************************************************************
*	APRON TUTORIAL PRESENTED BY MORROWLAND					*
*************************************************************
*	Author					: Ronny André Reierstad			*
*	Web Page				: www.morrowland.com			*
*	E-Mail					: apron@morrowland.com			*
************************************************************/

#include "3dsloader.h"
#include "winmain.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//										 INIT 3DS
/////////////////////////////////////////////////////////////////////////////////////////////////
void C3dsLoader::Init_3ds(char* filename)
{
	char str_file[64];
	numObject = 0;

	ZeroMemory(&m3DModel,sizeof(m3DModel));

	mLoad3ds.Import3DS(&m3DModel, filename);
	
	for(int i = 0; i < m3DModel.numOfMaterials; i++)				
	{
		if(strlen(m3DModel.pMaterials[i].strFile) > 0)				
		{
			sprintf(str_file,"data/texture/%s",m3DModel.pMaterials[i].strFile);
			Texture_3ds(TextureArray3ds, str_file, i);			
		}
		
		m3DModel.pMaterials[i].texureId = i;						
	}

	for(i = 0; i < m3DModel.numOfObjects; i++)
	{
		t3DObject *pObject = &m3DModel.pObject[i];

		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
				
				if(pObject->bHasTexture) {
					
					if(pObject->pTexVerts) {
						glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
					}
				} else {
					
					if(m3DModel.pMaterials.size() < pObject->materialID) 
					{
						BYTE *pColor = m3DModel.pMaterials[pObject->materialID].color;
						
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}
				
				glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
				
				if (bbox[i].min.x == 0) bbox[i].min.x =  999999.9f;
				if (bbox[i].max.x == 0) bbox[i].max.x = -999999.9f;
				if (bbox[i].min.z == 0) bbox[i].min.z =  999999.9f;
				if (bbox[i].max.z == 0) bbox[i].max.z = -999999.9f;
				if (bbox[i].min.y == 0) bbox[i].min.y =  999999.9f;
				if (bbox[i].max.y == 0) bbox[i].max.y = -999999.9f;
				
				if(pObject->pVerts[ index ].x < bbox[i].min.x) 
					bbox[i].min.x = pObject->pVerts[ index ].x; 
				if(pObject->pVerts[ index ].y < bbox[i].min.y) 
					bbox[i].min.y = pObject->pVerts[ index ].y; 
				if(pObject->pVerts[ index ].z < bbox[i].min.z) 
					bbox[i].min.z = pObject->pVerts[ index ].z; 

				if(pObject->pVerts[ index ].x > bbox[i].max.x) 
					bbox[i].max.x = pObject->pVerts[ index ].x; 
				if(pObject->pVerts[ index ].y > bbox[i].max.y) 
					bbox[i].max.y = pObject->pVerts[ index ].y; 
				if(pObject->pVerts[ index ].z > bbox[i].max.z) 
					bbox[i].max.z = pObject->pVerts[ index ].z; 

				char str[255];
				strncpy(str,pObject->strName,sizeof(pObject->strName));

				if(strcmpi("Plane01",str) == 0)
				{
					bbox[i].min.x = 0;
					bbox[i].max.x = 0;
					bbox[i].min.z = 0;
					bbox[i].max.z = 0;
					bbox[i].min.y = 0;
					bbox[i].max.y = 0;
				}
			}
		}

		numObject++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//										 INIT 3DS
/////////////////////////////////////////////////////////////////////////////////////////////////
void C3dsLoader::Init_3ds(char* filename, char* tex_filename)		
{
	char str_file[64];
	numObject = 0;

	ZeroMemory(&m3DModel,sizeof(m3DModel));

	mLoad3ds.Import3DS(&m3DModel, filename);
	
	for(int i = 0; i < m3DModel.numOfMaterials; i++)				
	{
		if(strlen(m3DModel.pMaterials[i].strFile) > 0)				
		{
			sprintf(str_file,"%s/%s",tex_filename,m3DModel.pMaterials[i].strFile);
			Texture_3ds(TextureArray3ds, str_file, i);			
//			Texture_3ds(TextureArray3ds, m3DModel.pMaterials[i].strFile, i);			
		}
		
		m3DModel.pMaterials[i].texureId = i;						
	}

	for(i = 0; i < m3DModel.numOfObjects; i++)
	{
		t3DObject *pObject = &m3DModel.pObject[i];

		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
				
				if(pObject->bHasTexture) {
					
					if(pObject->pTexVerts) {
						glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
					}
				} else {
					
					if(m3DModel.pMaterials.size() < pObject->materialID) 
					{
						BYTE *pColor = m3DModel.pMaterials[pObject->materialID].color;
						
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}
				
				glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
				
				if (bbox[i].min.x == 0) bbox[i].min.x =  999999.9f;
				if (bbox[i].max.x == 0) bbox[i].max.x = -999999.9f;
				if (bbox[i].min.z == 0) bbox[i].min.z =  999999.9f;
				if (bbox[i].max.z == 0) bbox[i].max.z = -999999.9f;
				if (bbox[i].min.y == 0) bbox[i].min.y =  999999.9f;
				if (bbox[i].max.y == 0) bbox[i].max.y = -999999.9f;
				
				if(pObject->pVerts[ index ].x < bbox[i].min.x) 
					bbox[i].min.x = pObject->pVerts[ index ].x; 
				if(pObject->pVerts[ index ].y < bbox[i].min.y) 
					bbox[i].min.y = pObject->pVerts[ index ].y; 
				if(pObject->pVerts[ index ].z < bbox[i].min.z) 
					bbox[i].min.z = pObject->pVerts[ index ].z; 

				if(pObject->pVerts[ index ].x > bbox[i].max.x) 
					bbox[i].max.x = pObject->pVerts[ index ].x; 
				if(pObject->pVerts[ index ].y > bbox[i].max.y) 
					bbox[i].max.y = pObject->pVerts[ index ].y; 
				if(pObject->pVerts[ index ].z > bbox[i].max.z) 
					bbox[i].max.z = pObject->pVerts[ index ].z; 

				char str[255];
				strncpy(str,pObject->strName,sizeof(pObject->strName));

				if(strcmpi("Plane01",str) == 0)
				{
					bbox[i].min.x = 0;
					bbox[i].max.x = 0;
					bbox[i].min.z = 0;
					bbox[i].max.z = 0;
					bbox[i].min.y = 0;
					bbox[i].max.y = 0;
				}
			}
		}

		numObject++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//										RENDER 3DS
/////////////////////////////////////////////////////////////////////////////////////////////////
void C3dsLoader::Render_3ds()
{
	for(int i = 0; i < m3DModel.numOfObjects; i++)
	{
		
		if(m3DModel.pObject.size() <= 0) break;						
		
		t3DObject *pObject = &m3DModel.pObject[i];					
		
		if(pObject->bHasTexture)									
		{									
//			glEnable(GL_TEXTURE_2D);								
			
			glColor3ub(255, 255, 255);								
			
			glBindTexture(GL_TEXTURE_2D, TextureArray3ds[pObject->materialID]); 
		} 
		else 
		{
			glDisable(GL_TEXTURE_2D);								
			
			glColor3ub(255, 255, 255);								
		}
		
		glBegin(GL_TRIANGLES);	
		
		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
				
				if(pObject->bHasTexture) {
					
					if(pObject->pTexVerts) {
						glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
					}
				} else {
					
					if(m3DModel.pMaterials.size() < pObject->materialID) 
					{
						BYTE *pColor = m3DModel.pMaterials[pObject->materialID].color;
						
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}
				
				glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
				
			}
		}
		
		glEnd();
	}
	
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										TEXTURE 3DS			jpeg or bmp
/////////////////////////////////////////////////////////////////////////////////////////////////
void C3dsLoader::Texture_3ds(UINT textureArray[], LPSTR strFileName, int ID)
{
	AUX_RGBImageRec *pBitMap = NULL;
	FILE *pFile = NULL;									
	unsigned char *pJpeg = NULL;
	unsigned long width, height; 
	int type;
	CJPEGFile jpeg;
	char tempstring[5] = {0};
	
	strncpy(tempstring, strFileName + strlen(strFileName)-4, 4);
	
	char FilePath[200];
	
	sprintf(FilePath, "%s", strFileName);
	
	
	if(!strFileName) return;
	
	if(!strcmp(tempstring, ".BMP"))
	{
		pFile = fopen(FilePath,"r");					
		
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
	else if(!strcmp(tempstring, ".JPG") || !strcmp(tempstring, "JPEG")) 
	{
		jpeg.GetSize(FilePath, width, height);
		pJpeg = new unsigned char[width*height*3];
		jpeg.Load(FilePath, 24, pJpeg);
		type = 2;
	}
	
	
	glGenTextures(1, &textureArray[ID]);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	
	glBindTexture(GL_TEXTURE_2D, textureArray[ID]);
	
	if(type==1) gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitMap->sizeX, pBitMap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitMap->data);
	if(type==2) gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, pJpeg);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	if (pBitMap)								
	{
		if (pBitMap->data)						
		{
			free(pBitMap->data);				
		}
		free(pBitMap);							
	}
	
	if (pJpeg) delete [] pJpeg;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										UNLOAD 3DS
/////////////////////////////////////////////////////////////////////////////////////////////////
void C3dsLoader::Unload_3ds()
{
	for(int i = 0; i < m3DModel.numOfObjects; i++)	
	{
		delete [] m3DModel.pObject[i].pFaces;
		delete [] m3DModel.pObject[i].pNormals;
		delete [] m3DModel.pObject[i].pVerts;
		delete [] m3DModel.pObject[i].pTexVerts;
	}
}












