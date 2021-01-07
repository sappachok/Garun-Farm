#ifndef _MONSTER_H
#define _MONSTER_H

#include "enemy.h"
#include "entity.h"
#include "attack.h"
#include "math.h"

class CMonster:public CEnemy
{
protected:
	void OnAnimate(float deltaTime);
	void OnCollision(CObject *collisionObject);
	void OnCollisionBox(CObject *collisionObject);
	void OnProcessAI();
	void OnPrepare();

public:
	st_SoundBuffer sound3D_Monster_shoot;
	CWolfHit *hit;
	bool	canAttack;
	bool	canDo;
	bool	hitOject;
	int		LifePoint;
	bool	IsHit;

	CMonster(); 
	~CMonster();

	void Create(float posx,float posy,float posz) 
	{ 
		position.x = posx;
		position.y = posy; 
		position.z = posz; 

		FindVictim(MD_FRIEND);
		target = victim[0];
	}

	void Load();
	void Draw();
	void ProcessAI();
	void UpdateEnemy();
};

#endif