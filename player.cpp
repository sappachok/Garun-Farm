#ifndef _PLAYER_CPP
#define _PLAYER_CPP
#endif

#include "player.h"
#include "world.h"

st_SoundBuffer pl_attack;
st_SoundBuffer pl_getItem;
st_SoundBuffer pl_damage;
st_SoundBuffer pl_changeWeapon;

CMD2Model *hammer;
CMD2Model *arrow;
CMD2Model *trap;
CMD2Model *life_box;

CMD2Model *pl_weapon;

bool canAttack = true;
int delay = 0;

CPlayer::CPlayer()
{
	position.x = 0;
	position.y = 0;
	position.z = 0;

	size = 3.0f;
	speed = PLAYER_SPEED;
	type =  MD_PLAYER;
	m_object = this;
	m_object->size = -10;

	bDeath = false;
	numLife = 3;
	LifePoint = 20;
	Damage = 0;

	playerScore = 0;
	diamond = 0;
	giantdiamond = 0;
	cowcoin = 0;

	bSpecielLook = false;

	weapon[0] = true;
	weapon[1] = false;
	weapon[2] = false;
	weapon[3] = false;

	numArrow = 10;
	numTrap = 3;
	numLifeBox = 1;
	numThunder = 10;

	dontmove = false;

	hammer = new CMD2Model;
	arrow = new CMD2Model;
	trap = new CMD2Model;
	life_box = new CMD2Model;

	pl_weapon = hammer;
}

CPlayer::~CPlayer()
{

}

void CPlayer::Load()
{
	CMD2Model::Load("Data\\Model\\Garun\\garun.md2","Data\\Model\\Garun\\garun.tga",1.5f);
	Rotate(direction);

	sound->LoadFromFile( pl_attack.lpdsBuffer, pl_attack.lpds3DBuffer, "Data\\Sound\\Earth3.wav");
	sound->LoadFromFile( pl_getItem.lpdsBuffer, pl_getItem.lpds3DBuffer, "Data\\Sound\\Recovery3.wav");
	sound->LoadFromFile( pl_damage.lpdsBuffer, pl_damage.lpds3DBuffer, "Data\\Sound\\Damage2.wav");
	sound->LoadFromFile( pl_changeWeapon.lpdsBuffer, pl_changeWeapon.lpds3DBuffer, 
		"Data\\Sound\\Open1.wav");


	hammer->Load("Data\\Model\\Garun\\ham.md2","Data\\Model\\Garun\\ham.tga",1.5f);
	arrow->Load("Data\\Model\\Garun\\bult02.md2","Data\\Model\\Garun\\bult.tga",1.5f);
	trap->Load("Data\\Model\\Garun\\trap.md2","Data\\Model\\Garun\\trap.tga",1.5f);
	life_box->Load("Data\\Model\\Garun\\redbox.md2","Data\\Model\\Garun\\lifebox.tga",1.5f);

}

void CPlayer::Input()
{
    float viewToPos,theta;  // the angle of the enemy-player vector

    CVector diff;       // the vector from the enemy to the player
    diff.x = m_camera->mPos.x - m_camera->mView.x;
    diff.z = m_camera->mPos.z - m_camera->mView.z;
    diff.Normalize();

    // find the angle in the world of the vector from the enemy to the player
    // in relation the negative z-axis

//    dirToPlayer = RAD2DEG(diff.Angle(CVector(0,0,-1)));

	if(diff.x <= 0)
	{
		theta = atan( diff.z / diff.x );
		viewToPos = RAD2DEG(theta);
	}
	else
	{
		theta = atan( diff.z / diff.x );
		viewToPos = RAD2DEG(theta - 135);
	}

	g_Input.Update();
	tVector3 vVector;
	CVector f_pos;

	f_pos.x = position.x+10;

	vVector = m_camera->mView-m_camera->mPos;

	if((LifePoint <= 0) && (bDeath == false))
	{
		CMD2Model::SetAnimation(CMD2Model::DEATH3);

		if(m_nextFrame == m_endFrame)
		{
			bDeath = true;
		}
	}
	else if((LifePoint > 0) && (bDeath == false) && (CMD2Model::m_state!=PAIN1))
	{
	if(attack == true || specielAttack == true)
	{
		if(attack == true)
		{
			if(weapon[0])
			{
				CMD2Model::SetAnimation(CMD2Model::ATTACK);

				if((CMD2Model::GetFrameAnimate() == 49) && (canAttack == true))
				{
/*					sound->SetPosition(pl_attack.lpds3DBuffer,0,0,0, 0,0,0);
					sound->Play(pl_attack.lpdsBuffer);
					CHummerHit *hit = new CHummerHit;
					hit->AttachTo(place);
					hit->Create(
					position.x+sin((pl_Rotate/180)*PI+(PI/2))*3.5,
					0,
					position.z+cos((pl_Rotate/180)*PI+(PI/2))*3.5,
					5);
	
					canAttack = false;
*/				}

				if(m_nextFrame == m_endFrame)
				{
					attack = false;
					canAttack = true;
				}
			
			}
			else if(weapon[1]&&(numArrow>0))
			{
				if((weapon[1]==true)&&(numArrow>0)&&(canAttack==true))
				{

				}

				if(m_nextFrame == m_endFrame)
				{
					attack = false;
					canAttack = true;
				}
			}
			else if((weapon[2]==true)&&(numTrap>0))
			{
				CMD2Model::SetAnimation(CMD2Model::ATTACK);

				if((CMD2Model::GetFrameAnimate() == 49))
				{
//					sound->SetPosition(pl_attack.lpds3DBuffer,0,0,0, 0,0,0);
//					sound->Play(pl_attack.lpdsBuffer);
					CTrap *trap = new CTrap;
					trap->LoadSound(sound);
					trap->AttachTo(place);
					trap->Rotate(pl_Rotate);
					trap->Create(position.x,position.y,position.z);
					numTrap--;
					attack = false;
				}
			
				if(m_nextFrame == m_startFrame)
				{
					attack = false;
				}
			}
			else if((weapon[3]==true)&&(numLifeBox>0)&&(canAttack==true))
			{
				LifePoint += 5;
				if(LifePoint > 20)
				{
					LifePoint = 20;
				}
				numLifeBox--;
				attack = false;
				canAttack = false;
				delay = 30;
			}
		}

		else if(specielAttack == true)
		{
			if((weapon[1]==true)&&(numArrow>0)&&(canAttack==true))
			{
				sound->SetPosition(pl_attack.lpds3DBuffer,0,0,0, 0,0,0);
				sound->Play(pl_attack.lpdsBuffer);
	
				CArrow *arrow = new CArrow;
				arrow->LoadSound(sound);
				arrow->AttachTo(place);
				arrow->Create(m_camera->mView.x, m_camera->mView.y, m_camera->mView.z);	
				arrow->direction = m_camera->GetDirect();
				arrow->Rotate(-viewToPos);
				arrow->directUp = m_camera->GetPitch();
				numArrow--;

//				CWind *wind = new CWind;
//				wind->AttachTo(place);
//				wind->SetAt(arrow);

				canAttack = false;
				specielAttack = false;

				delay = 30;
			}

			if(m_nextFrame == m_endFrame)
			{

			}
		}
	}
	else
	{
		CVector diff;       // the vector from the enemy to the player
		diff.x = m_camera->mView.x - m_camera->mPos.x;
		diff.z = m_camera->mView.z - m_camera->mPos.z;
		diff.Normalize();

		if(g_Input.KeyDown(DIK_W))
		{
			CMD2Model::SetAnimation(CMD2Model::RUN);
			direction = 0;

			f_pos.x = position.x + diff.x;
			f_pos.z = position.z + diff.z;

			if(m_object != this)
			{
				if(((f_pos-m_object->position).Length()) >= (size + m_object->size) )
				{
					m_camera->Move_Camera(speed);
				}
			}
			else
			{
				if((f_pos.x >= WORLD_MIN) && (f_pos.z >= WORLD_MIN) && 
				(f_pos.x <= WORLD_MAX) && (f_pos.z <= WORLD_MAX))
				{
					m_camera->Move_Camera(speed);
				}
			}
		}
		if(g_Input.KeyDown(DIK_S))
		{
			CMD2Model::SetAnimation(CMD2Model::RUN);
			direction = 180;

			f_pos.x = position.x - diff.x;
			f_pos.z = position.z - diff.z;

			if(m_object != this)
			{
				if(((f_pos-m_object->position).Length()) >= (size + m_object->size) )
				{
					m_camera->Move_Camera(-speed);
				}
			}
			else
			{
				if((f_pos.x >= WORLD_MIN) && (f_pos.z >= WORLD_MIN) && 
				(f_pos.x <= WORLD_MAX) && (f_pos.z <= WORLD_MAX))
				{
					m_camera->Move_Camera(-speed);
				}
			}
		}
		if(g_Input.KeyDown(DIK_A))
		{
			CMD2Model::SetAnimation(CMD2Model::RUN);
			direction = 90;

			f_pos.x = position.x + diff.z;
			f_pos.z = position.z - diff.x;

			if(m_object != this)
			{
				if(((f_pos-m_object->position).Length()) >= (size + m_object->size))
				{
					m_camera->Slide_Camera(speed);
				}
			}
			else
			{
				if((f_pos.x >= WORLD_MIN) && (f_pos.z >= WORLD_MIN) && 
				(f_pos.x <= WORLD_MAX) && (f_pos.z <= WORLD_MAX))
				{
					m_camera->Slide_Camera(speed);
				}
			}

			if(g_Input.KeyDown(DIK_W))
			{
				direction = 45;
			}
			else if(g_Input.KeyDown(DIK_S))
			{
				direction = 135;
			}
		}
		if(g_Input.KeyDown(DIK_D))
		{
			CMD2Model::SetAnimation(CMD2Model::RUN);
			direction = 270;

			f_pos.x = position.x - diff.z;
			f_pos.z = position.z + diff.x;

			if(m_object != this)
			{
				if(((f_pos-m_object->position).Length()) >= (size + m_object->size))
				{
					m_camera->Slide_Camera(-speed);
				}
			}
			else
			{
				if((f_pos.x >= WORLD_MIN) && (f_pos.z >= WORLD_MIN) && 
				(f_pos.x <= WORLD_MAX) && (f_pos.z <= WORLD_MAX))
				{
					m_camera->Slide_Camera(-speed);
				}
			}

			if(g_Input.KeyDown(DIK_W))
			{
				direction = 315;
			}
			else if(g_Input.KeyDown(DIK_S))
			{
				direction = 225;
			}
		}

		if(!(g_Input.KeyDown(DIK_W)) && !(g_Input.KeyDown(DIK_S)) && !(g_Input.KeyDown(DIK_A)) &&
			!(g_Input.KeyDown(DIK_D)))
		{
			CMD2Model::SetAnimation(CMD2Model::IDLE);
		}
		else
		{
			pl_Rotate = (-viewToPos) + direction;
			CMD2Model::Rotate(pl_Rotate);
		}

		}
	}
	else if(CMD2Model::m_state==PAIN1)
	{
		if(m_nextFrame == m_startFrame)
		{
			LifePoint -= Damage;
			CMD2Model::SetAnimation(CMD2Model::IDLE);
		}
	}

/////////// FOR CHANGE WEAPON //////////////////

	if(g_Input.KeyDown(DIK_1))
	{
		if(weapon[0] != true)
		{
			weapon[0] = true;
			weapon[1] = false;
			weapon[2] = false;
			weapon[3] = false;
			weapon[4] = false;

			sound->SetPosition(pl_changeWeapon.lpds3DBuffer,0,0,0, 0,0,0);
			sound->Play(pl_changeWeapon.lpdsBuffer);
			pl_weapon = hammer;
			pl_weapon->AnimateSet(this);
			attack = false;
			canAttack = true;
		}
	}
	else if(g_Input.KeyDown(DIK_2))
	{
		if(weapon[1] != true)
		{
			weapon[0] = false;
			weapon[1] = true;
			weapon[2] = false;
			weapon[3] = false;
			weapon[4] = false;

			sound->SetPosition(pl_changeWeapon.lpds3DBuffer,0,0,0, 0,0,0);
			sound->Play(pl_changeWeapon.lpdsBuffer);
			pl_weapon = arrow;
			pl_weapon->AnimateSet(this);
		}
	}
	else if(g_Input.KeyDown(DIK_3))
	{
		if(weapon[2] != true)
		{
			weapon[0] = false;
			weapon[1] = false;
			weapon[2] = true;
			weapon[3] = false;
			weapon[4] = false;

			sound->SetPosition(pl_changeWeapon.lpds3DBuffer,0,0,0, 0,0,0);
			sound->Play(pl_changeWeapon.lpdsBuffer);
			pl_weapon = trap;
			pl_weapon->AnimateSet(this);
			attack = false;
			canAttack = true;
		}
	}
	else if(g_Input.KeyDown(DIK_4))
	{
		if(weapon[3] != true)
		{
			weapon[0] = false;
			weapon[1] = false;
			weapon[2] = false;
			weapon[3] = true;
			weapon[4] = false;

			sound->SetPosition(pl_changeWeapon.lpds3DBuffer,0,0,0, 0,0,0);
			sound->Play(pl_changeWeapon.lpdsBuffer);
			pl_weapon = life_box;
			pl_weapon->AnimateSet(this);
			attack = false;
			canAttack = true;
		}
	}
///////////////////////////////////////////////////
	
	if(g_Input.MouseDown(1) && weapon[1])
	{
		if(g_Input.MouseDown(0) && (canAttack == true) && (numArrow>0))
		{
			specielAttack = true;
		}
		else if(numArrow<=0)
		{
			specielAttack = false;
			canAttack = true;			
		}
		else if(delay <= 0)
		{
			delay = 0;
			canAttack = true;
			specielAttack = false;
			attack = false;
		}
	
		if(!bSpecielLook)
		{
			m_camera->mPos.x = sp_pos.x;
			m_camera->mPos.z = sp_pos.z;
			m_camera->mPos.y = 5;
			m_camera->mView.x = sp_view.x;
			m_camera->mView.z = sp_view.z;
			bSpecielLook = true;
		}

		position.x = m_camera->mPos.x;
		position.y = 0;
		position.z = m_camera->mPos.z;
	
		CMD2Model::Move(m_camera->mPos.x, 0, m_camera->mPos.z);

		nor_pos.x = m_camera->mPos.x+diff.x*+15;
		nor_pos.z = m_camera->mPos.z+diff.z*+15;
		nor_view.x = m_camera->mView.x+diff.x*+15;
		nor_view.z = m_camera->mView.z+diff.z*+15;


		direction = 0;
		pl_Rotate = (-viewToPos) + direction;
		CMD2Model::Rotate(pl_Rotate);
		m_camera->Mouse_Move_1P(800,600);
	}
	else
	{
		if(g_Input.MouseDown(0))
		{
			if(weapon[0])
			{
				attack = true;
			}
			else if(weapon[1] && (numArrow>0))
			{
				attack = false;
			}
			else if(weapon[2] && (numTrap>0))
			{
				attack = true;
			}
			else if(weapon[3] && (numLifeBox>0) && (canAttack==true))
			{
				attack = true;
			}
			else if(delay <= 0)
			{
				delay = 0;
				canAttack = true;
				specielAttack = false;
				attack = false;
			}
			else
			{
				specielAttack = false;
				attack = false;
			}
		}

		sp_pos.x = m_camera->mPos.x+diff.x*-15;
		sp_pos.z = m_camera->mPos.z+diff.z*-15;
		sp_view.x = m_camera->mView.x+diff.x*-15;
		sp_view.z = m_camera->mView.z+diff.z*-15;

		position.x = m_camera->mView.x;
		position.y = 0;
		position.z = m_camera->mView.z;

		if(bSpecielLook)
		{
			m_camera->mPos.x = nor_pos.x;
			m_camera->mPos.y = 12;
			m_camera->mPos.z = nor_pos.z;
			m_camera->mView.x = nor_view.x;
			m_camera->mView.z = nor_view.z;

			nor_pos.x = 0;
			nor_pos.z = 0;

			bSpecielLook = false;
		}

		m_camera->Mouse_Move(800,600);
		CMD2Model::Move(m_camera->mView.x, 0, m_camera->mView.z);
	}
}

void CPlayer::OnDraw(CCamera *camera)
{
	m_camera = camera;

	if(bSpecielLook)
	{

	}
	else
	{
		Render();
		pl_weapon->Render();
	}

	ProcessCollisions(FindRoot());
}

void CPlayer::OnCollision(CObject *collisionObject)
{
	if(LifePoint>0)
	{
	if(collisionObject->type == MD_ENEMY)
	{
		if(!collisionObject->isDead)
		{
			m_object = collisionObject;
		}
		else
		{
			m_object = this;
		}
	}
	else if(collisionObject->type == MD_FRIEND)
	{
		if(!collisionObject->isDead)
		{
			m_object = collisionObject;
		}
		else
		{
			m_object = this;
		}
	}
	else
	{
		m_object = this;
	}

	if(collisionObject->type == MD_TRAP)
	{
		if(weapon[2]==true)
		{
			if(g_Input.MouseDown(1) && (weapon[2]==true))
			{
				numTrap++;
				collisionObject->isDead = true;
			}
		}
		else
		{
			sound->SetPosition(pl_damage.lpds3DBuffer,0,0,0, 0,0,0);
			sound->Play(pl_damage.lpdsBuffer);

			CMD2Model::SetAnimation(CMD2Model::PAIN1);
			collisionObject->isDead = true;
			Damage = ((CTrap*)collisionObject)->attack+2;
			attack = false;
			canAttack = true;
		}

	}

	if(collisionObject->type == MD_BULLET)
	{
		sound->SetPosition(pl_damage.lpds3DBuffer,0,0,0, 0,0,0);
		sound->Play(pl_damage.lpdsBuffer);

		CMD2Model::SetAnimation(CMD2Model::PAIN1);
		collisionObject->isDead = true;

		Damage = ((CWolfHit*)collisionObject)->attack;
		attack = false;
		canAttack = true;
	}

	if(collisionObject->type == MD_SUPERSONIC)
	{
		CMD2Model::SetAnimation(CMD2Model::PAIN1);

		Damage = ((CSuperSonic*)(collisionObject))->attack;
		attack = false;
	}

	if(collisionObject->type == MD_DIAMOND_ITEM)
	{
		sound->SetPosition(pl_getItem.lpds3DBuffer,0,0,0, 0,0,0);
		sound->Play(pl_getItem.lpdsBuffer);
		diamond+=1;
		collisionObject->isDead = true;
	}

	if(collisionObject->type == MD_BLUEDIAMOND_ITEM)
	{
		sound->SetPosition(pl_getItem.lpds3DBuffer,0,0,0, 0,0,0);
		sound->Play(pl_getItem.lpdsBuffer);
		diamond+=1;
		playerScore += 50;
		collisionObject->isDead = true;
	}

	if(collisionObject->type == MD_REDDIAMOND_ITEM)
	{
		sound->SetPosition(pl_getItem.lpds3DBuffer,0,0,0, 0,0,0);
		sound->Play(pl_getItem.lpdsBuffer);
		giantdiamond+=1;
		playerScore += 200;
		collisionObject->isDead = true;
	}

	if(collisionObject->type == MD_ARROW_ITEM)
	{
		sound->SetPosition(pl_getItem.lpds3DBuffer,0,0,0, 0,0,0);
		sound->Play(pl_getItem.lpdsBuffer);
		numArrow += 5;
		collisionObject->isDead = true;
	}

	if(collisionObject->type == MD_TRAP_ITEM)
	{
		sound->SetPosition(pl_getItem.lpds3DBuffer,0,0,0, 0,0,0);
		sound->Play(pl_getItem.lpdsBuffer);
		numTrap += 1;
		collisionObject->isDead = true;
	}

	if(collisionObject->type == MD_LIFEBOX_ITEM)
	{
		sound->SetPosition(pl_getItem.lpds3DBuffer,0,0,0, 0,0,0);
		sound->Play(pl_getItem.lpdsBuffer);
		numLifeBox += 1;
		collisionObject->isDead = true;	
	}

	if(collisionObject->type == MD_GREEN_LIFEBOX_ITEM)
	{
		sound->SetPosition(pl_getItem.lpds3DBuffer,0,0,0, 0,0,0);
		sound->Play(pl_getItem.lpdsBuffer);

		CGreenBox* greenbox = new CGreenBox;
		greenbox->Create(position.x,0,position.z);
		greenbox->AttachTo(place);

		collisionObject->isDead = true;	
	}

	if(collisionObject->type == MD_COWCOIN_ITEM)
	{
		sound->SetPosition(pl_getItem.lpds3DBuffer,0,0,0, 0,0,0);
		sound->Play(pl_getItem.lpdsBuffer);
		
		cowcoin++;
		collisionObject->isDead = true;	
	}

	}
}

void CPlayer::OnPrepare()
{
	if(bDeath == true)
	{
		CMD2Model::SetAnimationCustom(197,197);
	}

	if((CMD2Model::GetFrameAnimate() == 49) && (weapon[0] == true) && (canAttack == true))
	{
		sound->SetPosition(pl_attack.lpds3DBuffer,0,0,0, 0,0,0);
		sound->Play(pl_attack.lpdsBuffer);
		CHummerHit *hit = new CHummerHit;
		hit->AttachTo(place);
		hit->Create(
		position.x+sin((pl_Rotate/180)*PI+(PI/2))*3.5,
		0,
		position.z+cos((pl_Rotate/180)*PI+(PI/2))*3.5,
		5);
		canAttack = false;
	}


	pl_weapon->AnimateSet(this);
	pl_weapon->Rotate(pl_Rotate);

	if(pl_weapon == arrow)
	{
		pl_weapon->Move(position.x, position.y + 1.0f, position.z);

	}
	else
	{
		pl_weapon->Move(position.x, position.y + 1.5f, position.z);
	}

}

void CPlayer::OnAnimate(scalar_t deltaTime)
{
	if(delay > 0)
	{
		delay -= 1;
	}
	else 
	{
		delay = 0;
	}

	CMD2Model::Animate(deltaTime * 1.5f);
	pl_weapon->Animate(deltaTime * 1.5f);
}

void CPlayer::OnCollisionBox(CObject *collisionobject)
{
	float diffXmin,diffXmax,diffZmin,diffZmax;
	diffXmin = (position.x - collisionobject->boundingBox.min.x);
	diffXmax = (collisionobject->boundingBox.max.x - position.x);
	diffZmin = (position.z - collisionobject->boundingBox.min.z);
	diffZmax = (collisionobject->boundingBox.max.z - position.z);
	
	CVector diff;       // the vector from the enemy to the player
    diff.x = m_camera->mView.x - m_camera->mPos.x;
    diff.z = m_camera->mView.z - m_camera->mPos.z;
    diff.Normalize();

	if(!bSpecielLook)
	{
		if((diffXmin < diffXmax) && (diffXmin < diffZmin) && (diffXmin < diffZmax))
		{
			m_camera->mView.x = collisionobject->boundingBox.min.x;// + (float(cosYaw)*2);
			m_camera->mPos.x = m_camera->mView.x + diff.x*-15;
			m_camera->mPos.z = m_camera->mView.z + diff.z*-15;
		}
		if((diffXmax < diffXmin) && (diffXmax < diffZmin) && (diffXmax < diffZmax))
		{
			m_camera->mView.x = collisionobject->boundingBox.max.x;// + (float(cosYaw)*2);
			m_camera->mPos.x = m_camera->mView.x + diff.x*-15;
			m_camera->mPos.z = m_camera->mView.z + diff.z*-15;
		}
		if((diffZmin < diffZmax) && (diffZmin < diffXmin) && (diffZmin < diffXmax))
		{
			m_camera->mView.z = collisionobject->boundingBox.min.z;
			m_camera->mPos.x = m_camera->mView.x + diff.x*-15;
			m_camera->mPos.z = m_camera->mView.z + diff.z*-15;
		}
		if((diffZmax < diffZmin) && (diffZmax < diffXmin) && (diffZmax < diffXmax))
		{
			m_camera->mView.z = collisionobject->boundingBox.max.z;// + (float(sinYaw)*2);
			m_camera->mPos.x = m_camera->mView.x + diff.x*-15;
			m_camera->mPos.z = m_camera->mView.z + diff.z*-15;
		}
	}
	else
	{
		if((diffXmin < diffXmax) && (diffXmin < diffZmin) && (diffXmin < diffZmax))
		{
			m_camera->mView.x = collisionobject->boundingBox.min.x + diff.x*15;// + (float(cosYaw)*2);
		}
		if((diffXmax < diffXmin) && (diffXmax < diffZmin) && (diffXmax < diffZmax))
		{
			m_camera->mView.x = collisionobject->boundingBox.max.x + diff.x*15;// + (float(cosYaw)*2);
		}
		if((diffZmin < diffZmax) && (diffZmin < diffXmin) && (diffZmin < diffXmax))
		{
			m_camera->mView.z = collisionobject->boundingBox.min.z + diff.z*15;
		}
		if((diffZmax < diffZmin) && (diffZmax < diffXmin) && (diffZmax < diffXmax))
		{
			m_camera->mView.z = collisionobject->boundingBox.max.z + diff.z*15;// + (float(sinYaw)*2);
		}

		m_camera->mPos.x = m_camera->mView.x + diff.x*-15;
		m_camera->mPos.z = m_camera->mView.z + diff.z*-15;
	}
}


