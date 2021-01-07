#ifndef _ATTACK_CPP
#define _ATTACK_CPP
#endif

#include "attack.h"

void CWolfHit::OnPrepare()
{
/*	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1,0,0);

	glBegin(GL_QUADS);
		glVertex3f(position.x-1, 3, position.z+1);
		glVertex3f(position.x+1, 3, position.z+1);
		glVertex3f(position.x+1, 3, position.z-1);
		glVertex3f(position.x-1, 3, position.z-1);
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
*/

	count++;

	if(count == 10)
	{
		isDead = true;
	}

}

void CHummerHit::Create(float stx,float sty,float stz,float st_height)
{
		position.x = stx; 
		position.y = sty;
		position.z = stz;

		heighEffect = st_height;
		damaged = false;
}

void CHummerHit::OnPrepare()
{
	if(damaged == true)
	{
		size = -3;
		glColor3f(1,0,0);
	}
	else
		glColor3f(0,0,1);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);

	glPushMatrix();
	glBlendFunc(GL_ONE,GL_ONE);
	glBegin(GL_QUADS);
		glVertex3f(position.x-HUMMER_HALO, position.y-HUMMER_HALO, position.z-HUMMER_HALO);
		glVertex3f(position.x+HUMMER_HALO, position.y-HUMMER_HALO, position.z-HUMMER_HALO);
		glVertex3f(position.x+HUMMER_HALO, position.y+heighEffect, position.z-HUMMER_HALO);
		glVertex3f(position.x-HUMMER_HALO, position.y+heighEffect, position.z-HUMMER_HALO);

		glVertex3f(position.x-HUMMER_HALO, position.y-HUMMER_HALO, position.z+HUMMER_HALO);
		glVertex3f(position.x+HUMMER_HALO, position.y-HUMMER_HALO, position.z+HUMMER_HALO);
		glVertex3f(position.x+HUMMER_HALO, position.y+heighEffect, position.z+HUMMER_HALO);
		glVertex3f(position.x-HUMMER_HALO, position.y+heighEffect, position.z+HUMMER_HALO);

		glVertex3f(position.x-HUMMER_HALO, position.y-HUMMER_HALO, position.z-HUMMER_HALO);
		glVertex3f(position.x-HUMMER_HALO, position.y-HUMMER_HALO, position.z+HUMMER_HALO);
		glVertex3f(position.x-HUMMER_HALO, position.y+heighEffect, position.z+HUMMER_HALO);
		glVertex3f(position.x-HUMMER_HALO, position.y+heighEffect, position.z-HUMMER_HALO);

		glVertex3f(position.x+HUMMER_HALO, position.y-HUMMER_HALO, position.z-HUMMER_HALO);
		glVertex3f(position.x+HUMMER_HALO, position.y-HUMMER_HALO, position.z+HUMMER_HALO);
		glVertex3f(position.x+HUMMER_HALO, position.y+heighEffect, position.z+HUMMER_HALO);
		glVertex3f(position.x+HUMMER_HALO, position.y+heighEffect, position.z-HUMMER_HALO);
	glEnd();
	glPopMatrix();

	heighEffect -= heighEffect/2;

	if(heighEffect<=0.5f)
		isDead = true;

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void CHummerHit::OnCollision(CObject *collisionObject)
{ 
	if(collisionObject->type == MD_FRIEND)
	{
		damaged = true;
	}
	if(collisionObject->type == MD_ENEMY)
	{
		damaged = true;
	}
}

