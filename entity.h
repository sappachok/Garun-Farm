#ifndef _ENTITY_H
#define _ENTITY_H

#include <stdlib.h>
#include <time.h>
#include <typeinfo.h>

#include "md2.h"
#include "camera.h"
#include "SoundSystem.h"

class CEntity : public CMD2Model
{
protected:
	void OnAnimate(float deltaTime);
	void OnDraw(CCamera *camera);
	void OnCollision(CObject *collisionObject);
	void OnPrepare();

public:
	float direction;

	CEntity();
	~CEntity();

	int stateStart, stateEnd;	// state keyframe start/end
	float deltaT;
	float animSpeed;
	long timer;

	CObject* FindObjectType(type_model objectType);
};

#endif