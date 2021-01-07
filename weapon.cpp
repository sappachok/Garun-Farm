#ifndef _WEAPON_CPP
#define _WEAPON_CPP
#endif

#include "world.h"
#include "weapon.h"
#include "attack.h"

float x = 0;

CWeapon::CWeapon()
{
	type =  MD_WEAPON;
	position.x = 0;
	position.y = 1.5f;
	position.z = 0;
	isAttack = false;

	hummer = new CMD2Model;
	arrow = new CMD2Model;
	trap = new CMD2Model;
	life_box = new CMD2Model;

	weapon = hummer;
}

CWeapon::~CWeapon()
{

}

void CWeapon::Load()
{
	hummer->Load("Data\\Model\\Garun\\ham.md2","Data\\Model\\Garun\\ham.tga",1.5f);
	arrow->Load("Data\\Model\\Garun\\bult02.md2","Data\\Model\\Garun\\bult.tga",1.5f);
	trap->Load("Data\\Model\\Garun\\trap.md2","Data\\Model\\Garun\\trap.tga",1.5f);
	life_box->Load("Data\\Model\\Garun\\ham.md2","Data\\Model\\Garun\\ham3.tga",1.5f);
}

void CWeapon::SetPlayer(CPlayer *player)
{
	m_player = player;

	weapon->Rotate(m_player->direction);
	hummer->SetAnimation(m_player->GetAnimation());
}

void CWeapon::OnDraw(CCamera *camera)
{
	if(m_player->bDeath == true)
	{
		weapon->SetAnimationCustom(197,197);	
	}
	else
	{
		hummer->SetAnimation(m_player->GetAnimation());
		hummer->Move(m_player->position.x, position.y, m_player->position.z);
		hummer->Rotate(m_player->pl_Rotate);

		if(m_player->GetAnimation() != ATTACK)
		{
			arrow->SetAnimation(m_player->GetAnimation());
		}

		arrow->Move(m_player->position.x, position.y-0.2f, m_player->position.z);
		arrow->Rotate(m_player->pl_Rotate);

		trap->SetAnimation(m_player->GetAnimation());
		trap->Move(m_player->position.x, position.y, m_player->position.z);
		trap->Rotate(m_player->pl_Rotate);

		life_box->SetAnimation(m_player->GetAnimation());
		life_box->Move(m_player->position.x, position.y, m_player->position.z);
		life_box->Rotate(m_player->pl_Rotate);
	}

//	weapon->Render();
}

void CWeapon::OnAnimate(scalar_t deltaTime)
{
	hummer->Animate(deltaTime);
	arrow->Animate(deltaTime);
	trap->Animate(deltaTime);
	life_box->Animate(deltaTime);
}

void CWeapon::OnCollision(CObject *collisionobject)
{

}

void CWeapon::OnPrepare()
{
	if(m_player->bDeath != true)
	{
		frame = weapon->GetFrameAnimate();

		if(m_player->weapon[0])
		{
			if(weapon != hummer)
			{
				weapon = hummer;
				weapon->SetAnimation(m_player->GetAnimation());
				position.y = 1.5f;
			}
		}
		else if(m_player->weapon[1])
		{
			if(weapon != arrow)
			{
				weapon = arrow;
				weapon->SetAnimation(m_player->GetAnimation());
				position.y = 1.2f;
			}
		}
		else if(m_player->weapon[2])
		{
			if(weapon != trap)
			{
				weapon = trap;
				weapon->SetAnimation(m_player->GetAnimation());
				position.y = 1.5f;
			}
		}
		else if(m_player->weapon[3])
		{
			if(weapon != life_box)
			{
				weapon = life_box;
				weapon->SetAnimation(m_player->GetAnimation());
				position.y = 1.5f;
			}
		}
	}
}
