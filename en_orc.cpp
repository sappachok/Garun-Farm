#ifndef ENEMY_ORC_CPP
#define ENEMY_ORC_CPP
#endif

#include "en_orc.h"

void COrc::Load()
{
	if(bSpeciel==TRUE)
	{
		CMD2Model::Load("Data\\Model\\orc\\tris.md2","Data\\Model\\orc\\Blackrock.tga",0.2f);
		weapon.Load("Data\\Model\\orc\\weapon.md2","Data\\Model\\orc\\weapon.tga",0.2f);
		sound->LoadFromFile( sound3D_Monster_shoot.lpdsBuffer,
			sound3D_Monster_shoot.lpds3DBuffer,"Data\\Sound\\Blow2.wav");
	}
	else
	{
		CMD2Model::Load("Data\\Model\\orc\\tris.md2","Data\\Model\\orc\\Blackrock.tga",0.2f);
		weapon.Load("Data\\Model\\orc\\weapon.md2","Data\\Model\\orc\\weapon.tga",0.2f);
		sound->LoadFromFile( sound3D_Monster_shoot.lpdsBuffer,
			sound3D_Monster_shoot.lpds3DBuffer,"Data\\Sound\\Blow2.wav");
	}

	sound->LoadFromFile( sound3D_Monster_pain.lpdsBuffer,
		sound3D_Monster_pain.lpds3DBuffer,"Data\\Sound\\Monster2.wav");
}

void COrc::OnDraw(CCamera *camera)
{
	CMD2Model::Move(position.x,0,position.z);	
	CMD2Model::Render();

	weapon.Rotate(-direction);
	weapon.Move(position.x,1.0f,position.z);
	weapon.Render();
}

void COrc::OnCollision(CObject *collisionObject) 
{ 	
	if(collisionObject->type == MD_PLAYER_HIT)
	{
		aiState = AI_PAIN;
		target = player;

		sound->SetPosition( sound3D_Monster_pain.lpds3DBuffer,
		(player->position.x - position.x)*0.01f, 0, (player->position.z - position.z)*0.01f,
		0, 0, 0);

		sound->Play(sound3D_Monster_pain.lpdsBuffer);

		collisionObject->isDead = true;
		damage = 1;
	}
	if(collisionObject->type == MD_ARROW)
	{
		aiState = AI_PAIN;
		target = player;

		sound->SetPosition( sound3D_Monster_pain.lpds3DBuffer,
		(player->position.x - position.x)*0.01f, 0, (player->position.z - position.z)*0.01f,
		0, 0, 0);

		sound->Play(sound3D_Monster_pain.lpdsBuffer);

		collisionObject->isDead = true;
		damage = 1;
	}
	if(collisionObject->type == MD_TRAP)
	{
		aiState = AI_PAIN;
		target = player;

		sound->SetPosition( sound3D_Monster_pain.lpds3DBuffer,
		(player->position.x - position.x)*0.01f, 0, (player->position.z - position.z)*0.01f,
		0, 0, 0);

		sound->Play(sound3D_Monster_pain.lpdsBuffer);

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
			objectHit = collisionObject;
		}

		if(collisionObject->type == MD_ENEMY)
		{
			objectHit = collisionObject;
		}

		if(collisionObject->type == MD_FRIEND)
		{
//			aiState = AI_ATTACK;
			objectHit = collisionObject;
			target = collisionObject;
		}

		CVector nextPos;
		float cosYaw = (float)cos(DEG2RAD(direction));
		float sinYaw = (float)sin(DEG2RAD(direction)); 

		nextPos.x = position.x + float(cosYaw)*1.0;
		nextPos.z = position.z + float(sinYaw)*1.0;
	
		if(((nextPos - objectHit->position).Length() <= (size + objectHit->size)) &&
			objectHit != this)
		{
			velocity = CVector(0.0, 0.0, 0.0);
		}
	}
}

void COrc::OnPrepare()
{ 
	FindVictim(MD_FRIEND);
	
	ProcessAI();

	UpdateEnemy();

	CEntity::OnPrepare();
}

void COrc::ProcessAI()
{
	CVector diff = player->position - position;
	distFromPlayer = sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);

	if(target != player)
		target = victim[0];

	if((aiState != AI_DEAD) && (aiState != AI_PAIN) && (aiState != AI_ATTACK))
	{
		if(distFromPlayer < 0)
		{
			aiState = AI_SCARED;
			target = player;
		}
		else if(distFromPlayer < size + 3)
		{
			aiState = AI_ATTACK;
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

		if(distFromTarget < size + 2)
		{
			aiState = AI_ATTACK;
		}
	}
}

void COrc::OnAnimate(float deltaTime)
{
	deltaTime = deltaTime * 1.5f;

	if(LifePoint <= 0)
	{
		aiState = AI_DEAD;
	}

	float cosYaw = (float)cos(DEG2RAD(direction));
	float sinYaw = (float)sin(DEG2RAD(direction)); 

	float speed = velocity.z * deltaTime;

	position.x += float(cosYaw)*speed;
	position.z += float(sinYaw)*speed;

	Animate(deltaTime);
	weapon.Animate(deltaTime);
}

void COrc::UpdateEnemy()
{
    float dirToPlayer = GetDirToObject(player); // the angle of the enemy-player vector
    float dirToVictim = GetDirToObject(victim[0]);  // the angle of the enemy-player vector
	float dirToTarget;

	if(target != this)
		dirToTarget = GetDirToObject(target);

	switch(aiState)
	{
		case AI_UNCARING:	// enemy is not scared and does not care
		{
			CVector nextPos;
			float cosYaw = (float)cos(DEG2RAD(direction));
			float sinYaw = (float)sin(DEG2RAD(direction)); 

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

			nextPos.x = position.x + float(cosYaw)*1.0;
			nextPos.z = position.z + float(sinYaw)*1.0;
	
			if(((nextPos - objectHit->position).Length() <= (size + objectHit->size)) &&
				objectHit != this)
			{
				inDirection -= 90;
				velocity = CVector(0.0, 0.0, movespeed);
				Rotate(-direction);	
			}
			else
			{
				velocity = CVector(0.0, 0.0, movespeed);
				SetAnimation(CMD2Model::RUN);
				Rotate(-direction);	
			}
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
				
				CWolfHit *hit = new CWolfHit;
				hit->AttachTo(this);
				hit->attack = 1;
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
					LifePoint -= damage;
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
				CDiamond *diamond = new CDiamond;
				diamond->AttachTo(player);
				diamond->Load();
				diamond->CreateItem(position.x, position.y, position.z);			

				isDead = true;
				player->playerScore += ScorePoint; //// Get Score /////////
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

	weapon.SetAnimation(CMD2Model::GetAnimation());
}

void COrc::OnCollisionBox(CObject *collisionObject)
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

