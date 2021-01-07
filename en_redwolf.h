#ifndef _REDWOLF_H
#define _REDWOLF_H

#include "enemy.h"
#include "attack.h"
#include "math.h"

class CPlayer;
class CEnemy;

class CRedWolf:public CEnemy
{
private:
	bool bSpeciel;

protected:
	void OnAnimate(float deltaTime);
	void OnCollision(CObject *collisionObject);
	void OnCollisionBox(CObject *collisionObject);
	void OnDraw(CCamera *camera);
	void OnProcessAI();
	void OnPrepare();

public:
	CWolfHit *hit;
	
	st_SoundBuffer sound3D_Monster_shoot;

	bool	canAttack;
	bool	canDo;
	bool	hitOject;
	bool	IsHit;

	CRedWolf() 
	{ 
		type = MD_ENEMY; 
		size = 3;
		IsHit = false;
		direction = 0;
		target = this;
		hitOject = false;
		canDo = true;
		bSpeciel = false;
		movespeed = 9;
		damage = 0;

		LifePoint = 3;
		ScorePoint = 300;
	}
	CRedWolf(bool speciel) 
	{ 
		type = MD_ENEMY; 
		size = 2.5f;
		IsHit = false;
		direction = 0;
		target = this;
		hitOject = false;
		canDo = true;
		bSpeciel = speciel;

		LifePoint = 10;
		ScorePoint = 3000;
	}
	~CRedWolf() {}

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