#ifndef _ENEMY_H
#define _ENEMY_H

#include "entity.h"
#include "player.h"
#include "item.h"

class CEnemy : public CEntity
{
protected:
	void OnDraw(CCamera *camera) {}
	void OnAnimate(float deltaTime) 
	{
		if(LifePoint <= 0)
			aiState = AI_DEAD;
	}
	void OnCollision(CObject *collisionObject) 
	{
		if(collisionObject->type == MD_THUNDER)
		{
			aiState = AI_PAIN;
			target = player;
			collisionObject->isDead = true;
			damage = ((CThunder*)collisionObject)->attack;
		}
	}
	void OnProcessAI() {}

public:
	enum AIState_t
	{
		NONE,
		AI_UNCARING,	// enemy is not scared and does not care
		AI_HUNT,
		AI_JUST_HIT_OBJECT,
		AI_ATTACK,
		AI_SCARED,	// enemy is scared and running away
		AI_PAIN,
		AI_ANGRY,
		AI_DEAD
	};

	enum EnemyType
	{
		BLACK_WOLF,
		WHITE_WOLF,
		RED_WOLF,
		GREEN_GIANT,
		RED_GIANT,
		BLACK_GIANT
	};

 	CSoundSystem *sound;
	CPlayer *player;
	CObject *target;
	CObject *victim[10];
	CObject *place;
	CObject *objectHit;

	int LifePoint;
	int ScorePoint;
	int AttackPoint;
	int   damage;
	int hitPoints;           // hit points the enemy has left
    float distFromPlayer;    // distance this enemy is from player
	float distFromTarget;
    float movespeed;          // speed of enemy when running
	float inDirection;
    AIState_t aiState;       // state of enemy thought
	float enemy_time;

	CEnemy();
	~CEnemy();

	void SetPlayer(CPlayer *p) { player = p; } 
	void SetPlace(CObject *pl) { place = pl; }
	void SetSound(CSoundSystem *s) { sound = s; }
	void ProcessAI() { OnProcessAI(); }

	void FindVictim(type_model objectType);
	float GetDirToObject(CObject *object);
	bool IsSphereObstruct(CVector centerSphere, float radianofSphere, 
					  CVector targetPoint, CVector currentPoint);
};

#endif

