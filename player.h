#ifndef __PLAYER_H
#define __PLAYER_H

#include <typeinfo.h>

#include "object.h"
#include "md2.h"
#include "weapon.h"
#include "camera.h"
#include "attack.h"
#include "SoundSystem.h"

#define PLAYER_SPEED 0.05f

class CItem;
class CDiamond;

class CPlayer:public CMD2Model
{
protected:
	void OnDraw(CCamera *camera);
	void OnAnimate(scalar_t deltaTime);
	void OnCollision(CObject *collisionobject);
	void OnCollisionBox(CObject *collisionobject);
	void OnPrepare();

public:
	int numLife;
	int LifePoint;
	int Attack;
	int Damage;
	bool bDeath;
	bool bSpecielLook;
	CSoundSystem *sound;
	CObject *place;
	CCamera *m_camera;
	CObject *m_object;
	CVector nor_pos,nor_view,sp_pos,sp_view;
	
	int	 playerScore;
	int  diamond;
	int  giantdiamond;
	int  cowcoin;

	int	 numArrow;
	int  numTrap;
	int  numLifeBox;
	int  numThunder;
	bool Up,Dow,Lef,Rig;
	bool attack;
	bool specielAttack;
	bool weapon[10];
	bool dontmove;
	float direction;
	float speed;
	float pl_Rotate;

	CPlayer();
	~CPlayer();

	void Load();
	void SetCamera(CCamera *camera) { m_camera = camera; }
	void SetPlace(CObject *pl) { place = pl; }
	void SetSound(CSoundSystem *s) { sound = s; }
	void ProcessAI();

	void Input();
};

#endif