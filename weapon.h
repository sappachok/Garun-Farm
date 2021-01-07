#ifndef _WEAPON_H
#define _WEAPON_H

#include "md2.h"
#include "player.h"
#include "camera.h"
#include "attack.h"

enum weaponState
{
	WP_NONE,
	WP_ATTACK
};

class CPlayer;
class CHummerHit;

class CWeapon:public CMD2Model
{
protected:
	void OnDraw(CCamera *camera);
	void OnAnimate(scalar_t deltaTime);
	void OnCollision(CObject *collisionobject);
	void OnPrepare();

public:
	int		frame;
	weaponState wp_state;
	bool		isAttack;
	CPlayer *m_player;
	CObject *place;
	CMD2Model *hummer;
	CMD2Model *arrow;
	CMD2Model *trap;
	CMD2Model *life_box;

	CMD2Model *weapon;

	CWeapon();
	~CWeapon();

	void Load();
	void SetPlayer(CPlayer *player);
	void SetPlace(CObject *pl) { place = pl; }
};

#endif