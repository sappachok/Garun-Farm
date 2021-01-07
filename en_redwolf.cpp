#ifndef _REDWOLF_CPP
#define _REDWOLF_CPP
#endif

#include "en_redwolf.h"

float RandDirection(int value,float deltaTime);

void CRedWolf::Load()
{
	if(bSpeciel == TRUE)
	{
		CMD2Model::Load("Data\\Model\\darkwolf\\darkwolf.md2","Data\\Model\\darkwolf\\redwolf.tga",0.2f);
		sound->LoadFromFile( sound3D_Monster_shoot.lpdsBuffer,
			sound3D_Monster_shoot.lpds3DBuffer,"Data\\Sound\\Blow2.wav");
	}
	else
	{
		CMD2Model::Load("Data\\Model\\darkwolf\\darkwolf.md2","Data\\Model\\darkwolf\\redwolf.tga",0.2f);
		sound->LoadFromFile( sound3D_Monster_shoot.lpdsBuffer,
			sound3D_Monster_shoot.lpds3DBuffer,"Data\\Sound\\Blow2.wav");
	}
}

void CRedWolf::OnDraw(CCamera *camera)
{
	CMD2Model::Move(position.x,position.y,position.z);	
	CMD2Model::Render();
}

void CRedWolf::OnCollision(CObject *collisionObject) 
{ 	
	if(collisionObject->type == MD_PLAYER_HIT)
	{
		aiState = AI_PAIN;
		target = player;
		collisionObject->isDead = true;
		damage = 1;
	}
	if(collisionObject->type == MD_ARROW)
	{
		aiState = AI_PAIN;
		target = player;
		collisionObject->isDead = true;
		damage = 1;
	}
	if(collisionObject->type == MD_TRAP)
	{
		aiState = AI_PAIN;
		target = player;
		collisionObject->isDead = true;
		damage = 3;
	}
	if(collisionObject->type == MD_THUNDER)
	{
		aiState = AI_PAIN;
		target = player;
		collisionObject->isDead = true;
		damage = ((CThunder*)collisionObject)->attack;
	}

	if(aiState != AI_DEAD)
	{
		if(collisionObject->type == MD_PLAYER)
		{
			float disToTarget;
			CVector nextPos;

			disToTarget = (position - collisionObject->position).Length();

			float cosYaw = (float)cos(DEG2RAD(direction));
			float sinYaw = (float)sin(DEG2RAD(direction)); 
	
			nextPos.x = position.x + float(cosYaw)*1;
			nextPos.z = position.z + float(sinYaw)*1;
		
			if((nextPos - collisionObject->position).Length() <= (size + collisionObject->size))
			{
				if(aiState != AI_ATTACK)
				{
					velocity = CVector(0.0, 0.0, 0.0);
				}
				hitOject = true;
			}
			IsHit = true;
		}
		else
			IsHit = false;

		if(collisionObject->type == MD_ENEMY)
		{
			float disToTarget;
			CVector nextPos;

			disToTarget = (position - collisionObject->position).Length();

			float cosYaw = (float)cos(DEG2RAD(direction));
			float sinYaw = (float)sin(DEG2RAD(direction)); 
	
			nextPos.x = position.x + float(cosYaw)*1;
			nextPos.z = position.z + float(sinYaw)*1;
		
			if((nextPos - collisionObject->position).Length() <= (size + collisionObject->size))
			{
				if(aiState != AI_ATTACK)
				{
					velocity = CVector(0.0, 0.0, -movespeed);
				}
					hitOject = true;
			}
		}
		else hitOject = false;


		if(collisionObject->type == MD_FRIEND)
		{
			aiState = AI_ATTACK;
			target = collisionObject;
		}
	}
}

void CRedWolf::OnPrepare()
{ 
	FindVictim(MD_FRIEND);
	
	ProcessAI();

	UpdateEnemy();

	CEntity::OnPrepare();

	Render();
}

void CRedWolf::ProcessAI()
{
	CVector diff = player->position - position;
	distFromPlayer = sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);

	if(target != player)
		target = victim[0];

	if((aiState != AI_DEAD) && (aiState != AI_PAIN) && (aiState != AI_ATTACK) &&
		(((CPlayer*)player)->LifePoint>0))
	{
		if(distFromPlayer < 0)
		{
			aiState = AI_SCARED;
			target = player;
		}
		else if(distFromPlayer < size + target->size + 2)
		{
			aiState = AI_ATTACK;
			target = player;
		}
		else
		{
			aiState = AI_UNCARING;	
		}
	}

	if((aiState != AI_DEAD) && (aiState != AI_PAIN)  && (aiState != AI_ATTACK) && 
		(target->type == MD_FRIEND))
	{
		CVector diff2 = target->position - position;
		distFromTarget = sqrt(diff2.x*diff2.x + diff2.y*diff2.y + diff2.z*diff2.z);

		if(distFromTarget < size + target->size)
		{
			aiState = AI_ATTACK;
		}
		else
		{
			aiState = AI_UNCARING;	
		}
	}
}

void CRedWolf::OnAnimate(float deltaTime)
{
	deltaTime = deltaTime * 1.5f;

	if(LifePoint <= 0)
	{
		aiState = AI_DEAD;
	}
	if(((CPlayer*)player)->LifePoint<=0)
	{
		target = victim[0];
	}

	float cosYaw = (float)cos(DEG2RAD(direction));
	float sinYaw = (float)sin(DEG2RAD(direction)); 

	float speed = velocity.z * deltaTime;

	position.x += float(cosYaw)*speed;
	position.z += float(sinYaw)*speed;

	Animate(deltaTime);
}

void CRedWolf::UpdateEnemy()
{
    float dirToPlayer = GetDirToObject(player); // the angle of the enemy-player vector
    float dirToVictim = GetDirToObject(victim[0]);  // the angle of the enemy-player vector
	float dirToTarget;

	if(target != this)
		dirToTarget = GetDirToObject(target);

	switch(aiState)
	{
		case AI_UNCARING:	// enemy is not scared and does not care
			if(hitOject == false)
			{
				if(target == victim[0])
				{
					target = victim[0];			
					direction = (dirToVictim);
				}
				if(target == player)
				{
					target = player;			
					direction = (dirToPlayer);
				}

				SetAnimation(CMD2Model::RUN);
				velocity = CVector(0.0, 0.0, movespeed);
				Rotate(-direction);	
			}
			else
			{
				aiState = AI_JUST_HIT_OBJECT;
			}

			break;
		case AI_JUST_HIT_OBJECT:
				velocity = CVector(0.0, 0.0, 0.0);
			break;
		case AI_ATTACK:

			direction = dirToTarget;

			if(CEnemy::TimeCounter(1000)==1)
			{
				SetAnimation(CMD2Model::ATTACK);
				velocity = CVector(0.0, 0.0, 0.0);
			}

			if((m_nextFrame == (m_startFrame+4)) && (canAttack == true))
			{
				hit = new CWolfHit;
				hit->attack = 3;
				hit->AttachTo(this);
				hit->Create(position.x+(float)cos(DEG2RAD(direction))*4,
					position.y,
					position.z+(float)sin(DEG2RAD(direction))*4);

				sound->SetPosition( sound3D_Monster_shoot.lpds3DBuffer,
				(player->position.x - position.x)*0.01f, 0, (player->position.z - position.z)*0.01f,
				0, 0, 0);

				sound->Play(sound3D_Monster_shoot.lpdsBuffer);

				canAttack = false;
			}
			else
			{

			}

			if(m_nextFrame == m_startFrame)
			{
				canAttack = true;
				aiState = NONE;
			}

			Rotate(-direction);
			break;

		case AI_SCARED:	// enemy is scared and running away
			if(CEnemy::TimeCounter(100)==1)
			{
				direction = (dirToPlayer - 180); 
				SetAnimation(CMD2Model::RUN);
				velocity = CVector(0.0, 0.0, 7.0);
				Rotate(-direction);
			}
			break;

		case AI_PAIN:
			{
				CMD2Model::SetAnimation(CMD2Model::PAIN1);
				velocity = CVector(0.0, 0.0, 0.0);

				if((m_nextFrame == m_startFrame+1) && (canDo == true))
				{
					LifePoint-=damage;
					canDo = false;
				}

				if(m_nextFrame == m_startFrame)
				{
					canDo = true;
					aiState = AI_SCARED;
				}
			}
			break;

		case AI_DEAD:
			CMD2Model::SetAnimation(CMD2Model::DEATH1);

			if(m_nextFrame == m_startFrame)
			{
				isDead = true;
				player->playerScore += ScorePoint;

				if(type == 0)
				{
					CRedDiamond *reddiamond = new CRedDiamond;
					reddiamond->AttachTo(place);
					reddiamond->Load();
					reddiamond->CreateItem(position.x, 0, position.z);	
				}
				else if(type<7)
				{
					CBlueDiamond *bluediamond = new CBlueDiamond;
					bluediamond->AttachTo(place);
					bluediamond->Load();
					bluediamond->CreateItem(position.x, 0, position.z);	
				}
				else if(type<10)
				{

				}
			}
			break;
	}

	if(position.x > 75)
		position.x = 75;
	if(position.x < -75)
		position.x = -75;
	if(position.z > 75)
		position.z = 75;
	if(position.z < -75)
		position.z = -75;

	Move(position.x,position.y,position.z);
}

void CRedWolf::OnCollisionBox(CObject *collisionObject)
{
	float diffXmin,diffXmax,diffZmin,diffZmax;
	diffXmin = (position.x - collisionObject->boundingBox.min.x);
	diffXmax = (collisionObject->boundingBox.max.x - position.x);
	diffZmin = (position.z - collisionObject->boundingBox.min.z);
	diffZmax = (collisionObject->boundingBox.max.z - position.z);

	if((diffXmin < diffXmax) && (diffXmin < diffZmin) && (diffXmin < diffZmax))
	{
		position.x = collisionObject->boundingBox.min.x;
	}
	else if((diffXmax < diffXmin) && (diffXmax < diffZmin) && (diffXmax < diffZmax))
	{
		position.x = collisionObject->boundingBox.max.x;

	}
	else if((diffZmin < diffZmax) && (diffZmin < diffXmin) && (diffZmin < diffXmax))
	{
		position.z = collisionObject->boundingBox.min.z;

	}
	else if((diffZmax < diffZmin) && (diffZmax < diffXmin) && (diffZmax < diffXmax))
	{
		position.z = collisionObject->boundingBox.max.z;
	}
}

