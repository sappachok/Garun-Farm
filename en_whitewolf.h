#ifndef _WHITEWOLF_H
#define _WHITEWOLF_H

#include "enemy.h"
#include "attack.h"
#include "math.h"

class CPlayer;
class CEnemy;

class CWhiteWolf:public CEnemy
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

	CWhiteWolf() 
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
		movespeed = 10;

		LifePoint = 3;
		ScorePoint = 500;
	}

	CWhiteWolf(bool speciel) 
	{ 
		if(speciel != true)
		{
			size = 3.5f;
			movespeed = 10;
			LifePoint = 2;
			ScorePoint = 8000;
		}
		else
		{
			size = 3.5f;
			movespeed = 7;
			LifePoint = 2;
			ScorePoint = 8000;
		}

		type = MD_ENEMY; 
		direction = 0;
		target = this;
		hitOject = false;
		canDo = true;
		bSpeciel = speciel;
	}

	~CWhiteWolf() {}

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