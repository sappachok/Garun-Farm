#ifndef _WOLF_H
#define _WOLF_H

#include "enemy.h"
#include "attack.h"
#include "math.h"

class CPlayer;
class CEnemy;

class CWolf:public CEnemy
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
	CWolfHit *hit;
	
	st_SoundBuffer sound3D_Monster_shoot;
	st_SoundBuffer sound3D_Monster_pain;


	bool	canAttack;
	bool	canDo;
	bool	hitOject;
	bool	IsHit;

	CWolf() 
	{ 
		type = MD_ENEMY; 
		size = 2.0f;
		IsHit = false;
		direction = 0;
		target = this;
		hitOject = false;
		canDo = true;
		bSpeciel = false;
		damage = 0;
		movespeed = 9;

		LifePoint = 1;
		ScorePoint = 100;
		position.x = 0;
		position.y = CMD2Model::m_boundingSphere.center.y;
		position.z = 0;
	}

	CWolf(bool speciel) 
	{ 
		type = MD_ENEMY; 
		size = 3.5f;
		IsHit = false;
		direction = 0;
		target = this;
		hitOject = false;
		canDo = true;
		bSpeciel = speciel;
		movespeed = 7;

		LifePoint = 10;
		ScorePoint = 2000;
	}

	~CWolf() {}

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

//////// AI funtion //////////
	void Uncaring();
	void Scared();
//////////////////////////////
};

#endif