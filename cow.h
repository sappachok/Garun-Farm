#ifndef _COW_H
#define _COW_H

#include "md2.h"
#include "attack.h"

enum VICAI_state
{
	VICTIM_AI_UNCARING,
	VICTIM_AI_SCARED,
	VICTIM_AI_PAIN,
	VICTIM_AI_DEAD
};


class CCow:public CMD2Model
{
private:
	bool		canDo;
	VICAI_state aiState;
	int			lifePoint;
protected:

	void OnAnimate(float deltaTime);
	void OnPrepare();
	void OnCollision(CObject *collisionObject);
	void OnCollisionBox(CObject *collisionObject);
	void OnDraw(CCamera *camera);
	void OnProcessAI();

public:

	float direction;
	int damage;
	CSoundSystem *sound;
	CObject *player;
	CObject *m_Object[5];
	CObject *place;

	CCow() 
	{ 
		type = MD_FRIEND; 
		lifePoint = 5; 
		size = 3.5f;  
		m_Object[0] = this; 
		damage = 0; 
	}
	~CCow() {}

	void Create(float x, float y, float z)
	{
		position.x = x;
		position.y = 2.5f;
		position.z = z;
		direction = 0;

		Load();
	}

	void Load();

	void SetPlayer(CObject *p) { player = p; }
	void SetPlace(CObject *pl) { place = pl; }
	void SetSound(CSoundSystem *s) { sound = s; }

	void ProcessAI() { OnProcessAI(); }

	void FindEnemy(type_model objectType)
	{
		int nearest = 999999;
		int count = 0;
		int distFromEnemy = 0;

		CObject *c1;
		CObject *c2;

		c1 = (CObject*)place->childNode;

		while (c1 != NULL)
		{
			c2 = c1;
			do
			{
				if (c2->type == objectType)
				{
					count++;

					if(m_Object[0] != this)
					{
						CVector diff = c2->position - position;
						distFromEnemy = sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);
					}

					if(distFromEnemy < nearest)
					{
						m_Object[0] = c2;
						nearest = distFromEnemy;
					}
				}

				c2 = (CObject*)c2->nextNode;
			}while (!c2->IsFirstChild());

			c1 = (CObject*)c1->childNode;
		}
	}

	float GetDirToEnemy(CObject *object)
	{
	    float dirToObject,theta;  // the angle of the enemy-player vector

		CVector diff;       // the vector from the enemy to the player
	    diff.x = position.x - object->position.x;
	    diff.z = position.z - object->position.z;
	    diff.Normalize();

		if(diff.x <= 0)
		{
			theta = atan( diff.z / diff.x );
			dirToObject = RAD2DEG(theta);
		}
		else
		{
			theta = atan( diff.z / diff.x );
			dirToObject = RAD2DEG(theta - 135.1f);
		}

//		dirToObject = 0;

		return dirToObject;
	}
};

#endif