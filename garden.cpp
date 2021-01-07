#ifndef _GARDEN_CPP
#define _GARDEN_CPP
#endif

#include "garden.h"

CGarden::CGarden()
{
	size = 500; position.x = 0; position.y = 0; position.z = 0;
}

CGarden::~CGarden()
{

}

void CGarden::Load(char* filename, char* tex_filename)
{
//	obj3dsLoader.Init_3ds("data/model/garun_farm06.3ds");
	obj3dsLoader.Init_3ds(filename, tex_filename);

	for(int i = 0 ; i < 20 ; i++)
	{
		CObject *unit = new CObject;
		unit->boundingBox = obj3dsLoader.bbox[i];
		unit->AttachTo(this);
	}
}

void CGarden::Unload3ds()
{
//	obj3dsLoader.Unload_3ds();
}

void CGarden::OnCollision(CObject *collisionObject)
{

}

void CGarden::OnDraw(CCamera *camera)
{
	obj3dsLoader.Render_3ds();
/*
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	for(int i = 0 ; i < obj3dsLoader.numObject ; i++)
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f( obj3dsLoader.bbox[i].min.x , obj3dsLoader.bbox[i].min.y , obj3dsLoader.bbox[i].min.z);
			glVertex3f( obj3dsLoader.bbox[i].max.x , obj3dsLoader.bbox[i].min.y , obj3dsLoader.bbox[i].min.z);
			glVertex3f( obj3dsLoader.bbox[i].max.x , obj3dsLoader.bbox[i].min.y , obj3dsLoader.bbox[i].max.z);
			glVertex3f( obj3dsLoader.bbox[i].min.x , obj3dsLoader.bbox[i].min.y , obj3dsLoader.bbox[i].max.z);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f( obj3dsLoader.bbox[i].min.x , obj3dsLoader.bbox[i].max.y , obj3dsLoader.bbox[i].min.z);
			glVertex3f( obj3dsLoader.bbox[i].max.x , obj3dsLoader.bbox[i].max.y , obj3dsLoader.bbox[i].min.z);
			glVertex3f( obj3dsLoader.bbox[i].max.x , obj3dsLoader.bbox[i].max.y , obj3dsLoader.bbox[i].max.z);
			glVertex3f( obj3dsLoader.bbox[i].min.x , obj3dsLoader.bbox[i].max.y , obj3dsLoader.bbox[i].max.z);
		glEnd();
		
		glBegin(GL_LINES);
			glVertex3f( obj3dsLoader.bbox[i].min.x , obj3dsLoader.bbox[i].min.y , obj3dsLoader.bbox[i].min.z);
			glVertex3f( obj3dsLoader.bbox[i].min.x , obj3dsLoader.bbox[i].max.y , obj3dsLoader.bbox[i].min.z);

			glVertex3f( obj3dsLoader.bbox[i].min.x , obj3dsLoader.bbox[i].min.y , obj3dsLoader.bbox[i].max.z);
			glVertex3f( obj3dsLoader.bbox[i].min.x , obj3dsLoader.bbox[i].max.y , obj3dsLoader.bbox[i].max.z);

			glVertex3f( obj3dsLoader.bbox[i].max.x , obj3dsLoader.bbox[i].min.y , obj3dsLoader.bbox[i].min.z);
			glVertex3f( obj3dsLoader.bbox[i].max.x , obj3dsLoader.bbox[i].max.y , obj3dsLoader.bbox[i].min.z);

			glVertex3f( obj3dsLoader.bbox[i].max.x , obj3dsLoader.bbox[i].min.y , obj3dsLoader.bbox[i].max.z);
			glVertex3f( obj3dsLoader.bbox[i].max.x , obj3dsLoader.bbox[i].max.y , obj3dsLoader.bbox[i].max.z);
		glEnd();
	}

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);*/
}

void CGarden::Render()
{
	obj3dsLoader.Render_3ds();
}