#ifndef _WORLD_H
#define _WORLD_H

//#include <cguid.h>
#include <time.h>
#include "md2.h"
#include "object.h"
#include "entity.h"
#include "world.h"
#include "player.h"
#include "garden.h"
#include "en_wolf.h"
#include "en_redwolf.h"
#include "en_whitewolf.h"
#include "en_greengoblin.h"
#include "en_redgoblin.h"
#include "en_worriorgoblin.h"
#include "en_boss01.h"
#include "en_boss02.h"
#include "en_boss03.h"
#include "en_boss04.h"
#include "en_boss05.h"
#include "en_orc.h"
#include "weapon.h"
#include "cow.h"
#include "tree.h"
#include "weapon.h"

#define MAX_ENEMIES 10
#define MAX_COW		5
#define WORLD_MIN	-75
#define WORLD_MAX	75

class CPlayer;
class CEnemy;
class CCow;

class CWorld
{
private:
  	unsigned int limitTime;

	enum t_enemy
	{
		BLACKWOLF,
		REDWOLF,
		WHITEWOLF,
		GREENGIANT,
		BLUEGIANT,
		BLACKGIANT,
		GREENGOBLIN,
		REDGOBLIN,
		WORRIORGOBLIN,
		BLACKORC,
		BOSS01,
		BOSS02,
		BOSS03,
		BOSS04,
		BOSS05
	};

protected:
	void OnAnimate(scalar_t deltaTime);
	void OnDraw();
	void OnPrepare();

public:
//	HWND hWnd;
	bool gameDone;
	bool PresentTime;
	int numStage;
	int numMonster;
	int numCow;
	int ScoreWorld;

	CObject *enemy;
	CObject *item;

	CSoundSystem *sound;
	CHummerHit *hit;
	CWeapon *weapon;
	CGarden *garden;
	CGoblin *greengoblin;
	CPlayer		*player;
//	CWolf		*blackwolf;
	CRedWolf	*redwolf;
	CWhiteWolf	*whitewolf;
	CCow	*cow;

	float timeStart;
	float timeElapsed;
	t_enemy	EnemyOfStage[32];

	CWorld();
	~CWorld();

	bool LoadWorld();
	void LoadMonsterStage(int nState);
	void LoadMonster(t_enemy enemytype);
	void UnloadWorld();

	void LoadEnemy();
	void LoadItem(type_model typeItem);
	void LoadItemStage(int stage);

	void SetSound(CSoundSystem *s){ sound = s; }
	void Animate(float deltatime);
	void Draw(CCamera *camera);
	void Prepare() { OnPrepare(); }

	void CreateCow();

	void ResetTime();
	bool IsGameDone;
	void QuitGame();
	void Destroy();

	void GetKey(int virKey);
	void GameInput();

	int CountObjectTypes(CObject *object, type_model typeobject);

	int TimeCounter(int tMillisecond)
	{
		if(limitTime < clock())
		{
			limitTime += tMillisecond;
			return 1;
		}		

		return 0;
	}

};

#endif