#ifndef _ENTITY_CPP
#define _ENTITY_CPP
#endif

#include "entity.h"

CEntity::CEntity()
{

}

CEntity::~CEntity()
{

}

void CEntity::OnDraw(CCamera *camera)
{
//	Render();
}

void CEntity::OnAnimate(float deltaTime)
{
/*	float cosYaw = (float)cos(DEG2RAD(direction));
	float sinYaw = (float)sin(DEG2RAD(direction)); 

	float speed = velocity.z * deltaTime;

	position.x += float(cosYaw)*speed;
	position.z += float(sinYaw)*speed;

	Animate(deltaTime);*/
}

void CEntity::OnPrepare()
{
	ProcessCollisions(FindRoot());
}

void CEntity::OnCollision(CObject *collisionObject)
{

}
