#ifndef EN_GREENGOBLIN_H
#define EN_GREENGOBLIN_H

#include "enemy.h"
#include "attack.h"
#include "math.h"

class CGoblin:public CEnemy
{
protected:
	bool bSpeciel;

	void OnAnimate(float deltaTime);
	void OnCollision(CObject *collisionObject);
	void OnCollisionBox(CObject *collisionObject);
	void OnProcessAI();
	void OnDraw(CCamera *camera);
	void OnPrepare();

public:
	CMD2Model weapon;
	CWolfHit *hit;
	
	st_SoundBuffer sound3D_Monster_shoot;
	st_SoundBuffer sound3D_Monster_pain;

	bool	canAttack;
	bool	canDo;
	bool	hitOject;
	bool	IsHit;

	CGoblin() 
	{ 
		type = MD_ENEMY; 
		size = 2.0f;
		IsHit = false;
		direction = 0;
		target = this;
		hitOject = false;
		canDo = true;
		bSpeciel = false;
		canAttack = true;
		damage = 0;
		movespeed = 7;

		LifePoint = 1;
		ScorePoint = 100;
		position.x = 0;
		position.y = CMD2Model::m_boundingSphere.center.y;
		position.z = 0;
	}

	~CGoblin() {}

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