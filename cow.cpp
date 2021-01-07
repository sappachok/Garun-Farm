#include "cow.h"

LPDIRECTSOUNDBUFFER lpdsb_Cow_Pain = NULL;
st_SoundBuffer sound3D_cow_pain;
st_SoundBuffer sound3D_cow_die;

void CCow::Load()
{
	CMD2Model::Load("Data\\Model\\cow\\cow03.md2","Data\\Model\\cow\\cow.tga",0.075f);

	sound->LoadFromFile( sound3D_cow_pain.lpdsBuffer, sound3D_cow_pain.lpds3DBuffer, 
		"Data\\Sound\\001_COW2.wav");

	sound->LoadFromFile( sound3D_cow_die.lpdsBuffer, sound3D_cow_die.lpds3DBuffer, 
		"Data\\Sound\\Cow.wav");

}

void CCow::OnPrepare()
{ 
	float DirToEnemy;

	OnProcessAI();

	if(m_Object[0] != this)
	{
		DirToEnemy = GetDirToEnemy(m_Object[0]);
	}

	switch(aiState)
	{
		case VICTIM_AI_UNCARING:
		{
			CMD2Model::SetAnimation(CMD2Model::IDLE);			
			velocity = CVector(0.0, 0.0, 0.0);
		}
		break;
		case VICTIM_AI_SCARED:
		{
			if(TimeCounter(500)==1)
			{
				direction = (DirToEnemy - 180) + float(rand()%45)-45; 
				SetAnimation(CMD2Model::RUN);
				velocity = CVector(0.0, 0.0, 6.0f);
				Rotate(-direction-90);
			}

			CMD2Model::SetAnimation(CMD2Model::RUN);
		}
		break;
		case VICTIM_AI_PAIN:
		{
			sound->SetPosition(sound3D_cow_pain.lpds3DBuffer,
				(player->position.x - position.x)*0.01f, 0, (player->position.z - position.z)*0.01f,
				0, 0, 0);
			sound->Play(sound3D_cow_pain.lpdsBuffer);
			CMD2Model::SetAnimation(CMD2Model::PAIN1);

			velocity = CVector(0.0, 0.0, 0.0);

			if((m_nextFrame == m_startFrame+1) && (canDo == true))
			{
				lifePoint -= damage;
				canDo = false;
			}

			if(m_nextFrame == m_startFrame)
			{
				aiState = VICTIM_AI_SCARED;
				canDo = true;
			}
		}
		break;
		case VICTIM_AI_DEAD:
		{
			CMD2Model::SetAnimation(CMD2Model::DEATH3);
			sound->SetPosition(sound3D_cow_die.lpds3DBuffer,
				(player->position.x - position.x)*0.01f, 0, (player->position.z - position.z)*0.01f,
				0, 0, 0);
			sound->Play(sound3D_cow_die.lpdsBuffer);

			if(m_nextFrame == m_startFrame)
			{
				isDead = true;
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

}

void CCow::OnProcessAI()
{
	FindEnemy(MD_ENEMY);
	
	CVector diff2 = m_Object[0]->position - position;
	int distFromTarget = sqrt(diff2.x*diff2.x + diff2.y*diff2.y + diff2.z*diff2.z);

	if((aiState != VICTIM_AI_DEAD) && (m_Object[0] == this) && (aiState != VICTIM_AI_PAIN))
	{
		aiState = VICTIM_AI_UNCARING;
	}

	if(((aiState != VICTIM_AI_DEAD) && (aiState != VICTIM_AI_PAIN)) && (m_Object[0] != this))
	{
		if(distFromTarget < 20)
		{
			aiState = VICTIM_AI_SCARED;
		}
		else 
		{
			aiState = VICTIM_AI_UNCARING;
		}
	}

}

void CCow::OnDraw(CCamera *camera)
{
	Move(position.x, 0, position.z);
	Render();
}

void CCow::OnAnimate(float deltaTime)
{
	if(lifePoint <= 0)
		aiState = VICTIM_AI_DEAD;

	float cosYaw = (float)cos(DEG2RAD(direction));
	float sinYaw = (float)sin(DEG2RAD(direction)); 

	float speed = velocity.z * deltaTime;

	position.x += float(cosYaw)*speed;
	position.z += float(sinYaw)*speed;

	Animate(deltaTime);
}

void CCow::OnCollision(CObject *collisionObject)
{
	if(lifePoint > 0)
	{
	if(collisionObject->type == MD_BULLET)
	{
		aiState = VICTIM_AI_PAIN;
		collisionObject->isDead = true;
		damage = ((CWolfHit*)(collisionObject))->attack;
	}
	
	if(collisionObject->type == MD_SUPERSONIC)
	{
		aiState = VICTIM_AI_PAIN;
		damage = ((CSuperSonic*)(collisionObject))->attack;
	}

	if(collisionObject->type == MD_ARROW)
	{
		aiState = VICTIM_AI_PAIN;
		collisionObject->isDead = true;
		damage = ((CArrow*)(collisionObject))->attack;	
	}

	if(collisionObject->type == MD_FRIEND)
	{
		velocity = CVector(0.0, 0.0, 0.0);
	}

	if(collisionObject->type == MD_PLAYER_HIT)
	{
//		aiState = VICTIM_AI_PAIN;
	}

	if(collisionObject->type == MD_THUNDER)
	{
//		isDead = true;
	}

	if(collisionObject->type == MD_GREENBOX)
	{
		lifePoint += 3;

		if(lifePoint > 5)
		{
			lifePoint = 5;
		}

		collisionObject->isDead = true;
	}
	}
}

void CCow::OnCollisionBox(CObject *collisionObject)
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
