#ifndef _ENEMY_CPP
#define _ENEMY_CPP
#endif

#include "enemy.h"

CEnemy::CEnemy()
{
	hitPoints = 0;
	distFromPlayer = 0;
	aiState = NONE;
	enemy_time = 0;
	objectHit = this;

	inDirection = 0;
}

CEnemy::~CEnemy()
{

}

void CEnemy::FindVictim(type_model objectType)
{
	int nearest = 999999;
	int count = 0;
	CObject *c1;
	CObject *c2;

	c1 = (CObject*)place->childNode;

	while (c1 != NULL)
	{
		c2 = c1;
		do
		{
			if ((c2->type == objectType) && (c2->isDead==false))//typeObject)
			{
				victim[count] = c2;
				count++;

				CVector diff = c2->position - position;
				int distFromVictim = sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);

				if(distFromVictim < nearest)
				{
					victim[0] = c2;
					nearest = distFromVictim;
				}
			}

			c2 = (CObject*)c2->nextNode;
		}while (!c2->IsFirstChild());

		c1 = (CObject*)c1->childNode;
	}
}

float CEnemy::GetDirToObject(CObject *object)
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

	return dirToObject;
}

bool CEnemy::IsSphereObstruct(CVector centerSphere, float radianofSphere, 
					  CVector targetPoint, CVector currentPoint)
{
    float dirToTarget,theta;  
	CVector diff,point;       	// the angle of Line

    diff.x = currentPoint.x - targetPoint.x;
    diff.z = currentPoint.z - targetPoint.z;
    diff.Normalize();

	if(diff.x <= 0)
	{
		theta = atan( diff.z / diff.x );
		dirToTarget = RAD2DEG(theta);
	}
	else
	{
		theta = atan( diff.z / diff.x );
		dirToTarget = RAD2DEG(theta - 135);
	}

	float angleNormal = DEG2RAD(dirToTarget + 90);

	point.x = centerSphere.x + cos(angleNormal)*radianofSphere;
	point.z = centerSphere.z + sin(angleNormal)*radianofSphere;

//	if((point.x < targetPoint.x) && (point.z < targetPoint.z))
//		return true;

//	if((point.x > currentPoint.x) && (point.z > currentPoint.z))
//		return true;

	
	return false;
}
