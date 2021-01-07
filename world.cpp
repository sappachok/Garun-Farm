#ifndef _WORLD_CPP
#define _WORLD_CPP
#endif

#include "world.h"

float timelimit = 10.0f;
int lastsend = 0;
int lastTime = 0;
int bonus_score = 10000;

CWorld::CWorld()
{
	PresentTime = false;  // FOR PRESENT PROJECT ONLY
	gameDone = false;

	timeStart = 300.0;
	timeElapsed = 0;
	limitTime = 0;
	numMonster = 0;
	numStage = 1;

////////// USE GET ALL ENEMY OBJECT //////////////
	enemy = new CObject;
//////////////////////////////////////////////////

////////// USE SET ALL ITEM OBJECT //////////////
	item = new CObject;
//////////////////////////////////////////////////

//	numCow = MAX_COW;
	numCow = 5;

	player = new CPlayer;
	player->Load();
	player->size = 1.0f;
}

CWorld::~CWorld()
{
	if(player != NULL)
	{
		delete player;
	}
	if(weapon != NULL)
	{
		delete weapon;
	}

	garden->Unload();
	delete garden;
	delete sound;
}

void CWorld::Animate(float deltatime)
{
	deltatime = deltatime;
	if(!gameDone)
	{
		if((timeElapsed >= timelimit) && (!PresentTime))
		{
			if(enemy->childNode != NULL)
			{
				enemy->childNode->AttachTo(garden);
				enemy = (CObject*)enemy->nextNode;
			}

			float nextTime = rand()%20 + 3;

			timelimit += nextTime;
		}

		timeElapsed += deltatime;

		if((((int)timeElapsed % 20) == 0) && (lastsend != (int)timeElapsed))
		{
			int type = 	(int)(rand()%150);
			
			if(type < 5)
			{
				LoadItem(MD_COWCOIN_ITEM);
			}
			else if(type < 15)
			{
				LoadItem(MD_GREEN_LIFEBOX_ITEM);
			}
			else if(type < 30)
			{
				LoadItem(MD_ARROW_ITEM);
			}
			else if(type < 60)
			{
				LoadItem(MD_LIFEBOX_ITEM);		
			}
			else if(type < 100)
			{
				LoadItem(MD_TRAP_ITEM);
			}
			else if(type < 150)
			{
				LoadItem(MD_ARROW_ITEM);
			}

			if(item->childNode != NULL)
			{
				float x = (float)(rand()%100)-40;
				float z = (float)(rand()%100)-40;

				((CItem*)(item->childNode))->CreateItem(x,0,z);
				item->childNode->AttachTo(garden);
				item = (CObject*)item->nextNode;
				lastsend = (int)timeElapsed;
			}	
		}

		if(!((int)timeElapsed % 5) && (lastTime != (int)timeElapsed))
		{
			int type = (int)(rand()%5);

			float x = (float)(rand()%100)-40;
			float z = (float)(rand()%100)-40;

			if(type == 0)
			{
				CRedDiamond *reddiamond = new CRedDiamond;
				reddiamond->AttachTo(garden);
				reddiamond->Load();
				reddiamond->CreateItem(x,0,z);

				lastTime = (int)timeElapsed;
			}
			else if(type>0)
			{
				CBlueDiamond *bluediamond = new CBlueDiamond;
				bluediamond->AttachTo(garden);
				bluediamond->Load();
				bluediamond->CreateItem(x,0,z);
				
				lastTime = (int)timeElapsed;
			}
		}
	}
	else
	{

	}

	garden->Animate(deltatime);
}

void CWorld::Draw(CCamera *camera)
{
	garden->Draw(camera);
}

void CWorld::OnPrepare()
{
	garden->Prepare();

	srand((unsigned int)time(NULL));

	numMonster = CountObjectTypes(garden,MD_ENEMY);
	numCow = CountObjectTypes(garden,MD_FRIEND);

	if(CountObjectTypes(enemy,MD_ENEMY) <= 0)
	{
		if(numMonster <=0)
		{
			gameDone = true;
		}
	}

	ScoreWorld = player->playerScore;

	if(ScoreWorld >= bonus_score)
	{
		int num = ScoreWorld / bonus_score;

		for(int i=0;i<num;i++)
		{
			CreateCow();
		}

		bonus_score += bonus_score;
	}

	if(player->cowcoin > 0)
	{
/*		int num = player->cowcoin;

		for(int i=0;i<num;i++)
		{
			CreateCow();
		}

		player->cowcoin = 0;
*/	}
}

void CWorld::UnloadWorld()
{
	garden->Unload();
	player->Unload();
	weapon->Unload();
}

void CWorld::ResetTime()
{
	timelimit = 10.0f;
	timeElapsed = 0;
}

bool CWorld::LoadWorld()
{
	int cowIdx,enemyIdx = 0;
	int rndInt = 0;
	numMonster = 0;

	garden = new CGarden;

	if(numStage == 1)
	{
		garden->Load("data/model/garun_farm06.3ds","data/texture/garun_house");
	}
	else if(numStage == 2)
	{
		garden->Load("data/model/garun_farm06.3ds","data/texture/garun_house02");
	}
	else if(numStage == 3)
	{
		garden->Load("data/model/garun_farm06.3ds","data/texture/garun_house03");
	}
	else if(numStage == 4)
	{
		garden->Load("data/model/garun_farm06.3ds","data/texture/garun_house04");
	}
	else if(numStage == 5)
	{
		garden->Load("data/model/garun_farm06.3ds","data/texture/garun_house05");
	}

	player->AttachTo(garden);
	player->SetPlace(garden);

	weapon = new CWeapon;
	weapon->Load();
	weapon->size = 2;
	weapon->AttachTo(garden);
	weapon->SetPlayer(player);
	weapon->SetPlace(garden);

	srand((unsigned int)time(NULL));

	rndInt = (rand() % (MAX_ENEMIES));

	for (cowIdx = 0; cowIdx < numCow; cowIdx++)
	{
		CreateCow();
	}

	while(enemy->childNode != NULL)
	{
		delete enemy->childNode;
		enemy = (CObject*)enemy->nextNode;
	}

	LoadItemStage(numStage);
	LoadMonsterStage(numStage);

	return 1;
}

void CWorld::CreateCow()
{
		CCow *cow = new CCow;
		cow->AttachTo(garden);
		cow->SetPlace(garden);
		cow->SetPlayer(player);
		cow->SetSound(sound);
		cow->size = 3.0f;
		cow->position.x = (float)(rand()%100)-50;
		cow->position.y = 2.5f;

		if((cow->position.x > -3) && (cow->position.x < 3))
		{
			while((cow->position.z > -3) && (cow->position.z < 3))
			{
				cow->position.z = (float)(rand()%100)-50;
			}
		}
		else
		{
			cow->position.z = (float)(rand()%100)-50;
		}

		cow->Create(cow->position.x, cow->position.y-2.5f, cow->position.z);
}

void CWorld::LoadEnemy()
{

}

void CWorld::LoadMonsterStage(int nState)
{
	switch(nState)
	{
	case 1:
		LoadMonster(GREENGOBLIN);
		LoadMonster(GREENGOBLIN);
		LoadMonster(BLACKWOLF);
		LoadMonster(GREENGOBLIN);
		LoadMonster(GREENGOBLIN);
		LoadMonster(BLACKWOLF);
		LoadMonster(GREENGOBLIN);
		LoadMonster(GREENGOBLIN);
		LoadMonster(BLACKWOLF);
		LoadMonster(GREENGOBLIN);
		LoadMonster(GREENGOBLIN);
		LoadMonster(BLACKWOLF);
		LoadMonster(GREENGOBLIN);
		LoadMonster(BLACKWOLF);
		LoadMonster(BLACKWOLF);
		LoadMonster(BOSS01);
		break;
	case 2:
		LoadMonster(GREENGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(BLACKWOLF);
		LoadMonster(GREENGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(GREENGOBLIN);
		LoadMonster(BLACKWOLF);
		LoadMonster(REDWOLF);
		LoadMonster(GREENGOBLIN);
		LoadMonster(BLACKWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDWOLF);
		LoadMonster(BOSS02);
		break;
	case 3:
		LoadMonster(REDGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(BLACKWOLF);
		LoadMonster(GREENGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(GREENGOBLIN);
		LoadMonster(REDWOLF);
		LoadMonster(REDWOLF);
		LoadMonster(GREENGOBLIN);
		LoadMonster(BLACKWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(WHITEWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDWOLF);
		LoadMonster(WHITEWOLF);
		LoadMonster(WORRIORGOBLIN);
		LoadMonster(BOSS03);
		break;
	case 4:
		LoadMonster(REDGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(BLACKWOLF);
		LoadMonster(WORRIORGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(GREENGOBLIN);
		LoadMonster(WHITEWOLF);
		LoadMonster(REDWOLF);
		LoadMonster(WORRIORGOBLIN);
		LoadMonster(BLACKWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(WHITEWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(WHITEWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(WHITEWOLF);
		LoadMonster(WORRIORGOBLIN);
		LoadMonster(BOSS04);
		break;
	case 5:
		LoadMonster(WHITEWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDWOLF);
		LoadMonster(WORRIORGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDWOLF);
		LoadMonster(WHITEWOLF);
		LoadMonster(REDWOLF);
		LoadMonster(WORRIORGOBLIN);
		LoadMonster(REDGOBLIN);
		LoadMonster(WORRIORGOBLIN);
		LoadMonster(REDWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(WHITEWOLF);
		LoadMonster(REDGOBLIN);
		LoadMonster(REDWOLF);
		LoadMonster(WHITEWOLF);
		LoadMonster(WORRIORGOBLIN);
		LoadMonster(WHITEWOLF);
		LoadMonster(WHITEWOLF);
		LoadMonster(WORRIORGOBLIN);
		LoadMonster(WORRIORGOBLIN);
		LoadMonster(BOSS05);
		break;

	default:

		break;
	}
}

void CWorld::LoadMonster(t_enemy enemytype)
{
	float direct = rand()%4;
	float posx,posz;

	if(direct == 0)
	{
		posx = -75;
		posz = rand()%150-75;
	}
	else if(direct == 1)
	{
		posx = 75;
		posz = rand()%150-75;
	}
	else if(direct == 2)
	{
		posx = rand()%150-75;
		posz = -75;
	}
	else if(direct == 3)
	{
		posx = rand()%150-75;
		posz = 75;
	}

	switch(enemytype)
	{
	case BLACKWOLF:
		{
			CWolf *blackwolf = new CWolf;
			blackwolf->Load();	
			blackwolf->SetPlayer(player);
			blackwolf->SetPlace(garden);
			blackwolf->SetSound(sound);
			blackwolf->position.x = posx;
			blackwolf->position.y = blackwolf->GetBoundingSphere().center.y;
			blackwolf->position.z = posz;
			blackwolf->Create(blackwolf->position.x, blackwolf->position.y, blackwolf->position.z);
			blackwolf->AttachTo(enemy);
		}
		break;
	case REDWOLF:
		redwolf = new CRedWolf;
		redwolf->Load();
		redwolf->SetPlayer(player);
		redwolf->SetPlace(garden);
		redwolf->SetSound(sound);
		redwolf->position.x = posx;
		redwolf->position.y = 0;
		redwolf->position.z = posz;
		redwolf->Create(redwolf->position.x, redwolf->position.y, redwolf->position.z);
		redwolf->AttachTo(enemy);
		break;
	case WHITEWOLF:
		{
			whitewolf = new CWhiteWolf;
			whitewolf->Load();
			whitewolf->SetPlayer(player);
			whitewolf->SetPlace(garden);
			whitewolf->SetSound(sound);
			whitewolf->position.x = posx;
			whitewolf->position.y = 0;
			whitewolf->position.z = posz;
			whitewolf->Create(whitewolf->position.x, whitewolf->position.y, whitewolf->position.z);
			whitewolf->AttachTo(enemy);
		}
		break;
	case GREENGIANT:

		break;
	case BLUEGIANT:

		break;
	case BLACKGIANT:

		break;
	case GREENGOBLIN:
		{
			CGoblin *greengoblin = new CGoblin;
			greengoblin->AttachTo(enemy);
			greengoblin->Load();	//// Load Speciel BlackWolf /////////
			greengoblin->SetPlayer(player);
			greengoblin->SetPlace(garden);
			greengoblin->SetSound(sound);
			greengoblin->position.x = posx;
			greengoblin->position.y = greengoblin->GetBoundingSphere().center.y;
			greengoblin->position.z = posz;
			greengoblin->Create(greengoblin->position.x, greengoblin->position.y, greengoblin->position.z);
		}
		break;

	case REDGOBLIN:
		{
			CRedGoblin *redgoblin = new CRedGoblin;
			redgoblin->AttachTo(enemy);
			redgoblin->Load();	//// Load Speciel BlackWolf /////////
			redgoblin->SetPlayer(player);
			redgoblin->SetPlace(garden);
			redgoblin->SetSound(sound);
			redgoblin->position.x = posx;
			redgoblin->position.y = redgoblin->GetBoundingSphere().center.y;
			redgoblin->position.z = posz;
			redgoblin->Create(redgoblin->position.x, redgoblin->position.y, redgoblin->position.z);
		}
		break;

	case WORRIORGOBLIN:
		{
			CWorriorGoblin *worriorgoblin = new CWorriorGoblin;
			worriorgoblin->AttachTo(enemy);
			worriorgoblin->Load();	//// Load Speciel BlackWolf /////////
			worriorgoblin->SetPlayer(player);
			worriorgoblin->SetPlace(garden);
			worriorgoblin->SetSound(sound);
			worriorgoblin->position.x = posx;
			worriorgoblin->position.y = worriorgoblin->GetBoundingSphere().center.y;
			worriorgoblin->position.z = posz;
			worriorgoblin->Create(worriorgoblin->position.x, worriorgoblin->position.y, worriorgoblin->position.z);

		}
		break;
	case BOSS01:
		{
			CBoss01 *boss01 = new CBoss01;
			boss01->AttachTo(enemy);
			boss01->Load();	//// Load Speciel BlackWolf /////////
			boss01->SetPlayer(player);
			boss01->SetPlace(garden);
			boss01->SetSound(sound);
			boss01->position.x = posx;
			boss01->position.y = boss01->GetBoundingSphere().center.y;
			boss01->position.z = posz;
			boss01->Create(boss01->position.x, boss01->position.y, boss01->position.z);
		}
		break;

	case BOSS02:
		{
			CBoss02 *boss02 = new CBoss02;
			boss02->AttachTo(enemy);
			boss02->Load();	//// Load Speciel BlackWolf /////////
			boss02->SetPlayer(player);
			boss02->SetPlace(garden);
			boss02->SetSound(sound);
			boss02->position.x = posx;
			boss02->position.y = boss02->GetBoundingSphere().center.y;
			boss02->position.z = posz;
			boss02->Create(boss02->position.x, boss02->position.y, boss02->position.z);
		}
		break;
	case BOSS03:
		{
			CBoss03 *boss03 = new CBoss03;
			boss03->AttachTo(enemy);
			boss03->Load();	//// Load Speciel BlackWolf /////////
			boss03->SetPlayer(player);
			boss03->SetPlace(garden);
			boss03->SetSound(sound);
			boss03->position.x = posx;
			boss03->position.y = boss03->GetBoundingSphere().center.y;
			boss03->position.z = posz;
			boss03->Create(boss03->position.x, boss03->position.y, boss03->position.z);
		}
		break;
	case BOSS04:
		{
			CBoss04 *boss04 = new CBoss04;
			boss04->AttachTo(enemy);
			boss04->Load();	//// Load Speciel BlackWolf /////////
			boss04->SetPlayer(player);
			boss04->SetPlace(garden);
			boss04->SetSound(sound);
			boss04->position.x = posx;
			boss04->position.y = boss04->GetBoundingSphere().center.y;
			boss04->position.z = posz;
			boss04->Create(boss04->position.x, boss04->position.y, boss04->position.z);
		}
		break;
	case BOSS05:
		{
			CBoss05 *boss05 = new CBoss05;
			boss05->AttachTo(enemy);
			boss05->Load();	//// Load Speciel BlackWolf /////////
			boss05->SetPlayer(player);
			boss05->SetPlace(garden);
			boss05->SetSound(sound);
			boss05->position.x = posx;
			boss05->position.y = boss05->GetBoundingSphere().center.y;
			boss05->position.z = posz;
			boss05->Create(boss05->position.x, boss05->position.y, boss05->position.z);
		}
		break;
	default:

		break;
	}
}

void CWorld::GameInput()
{
	player->Input();
}

void CWorld::Destroy()
{
	delete garden;
}

void CWorld::LoadItem(type_model typeItem)
{
	switch(typeItem)
	{
	case MD_DIAMOND_ITEM:
	{
		CDiamond* diamond = new CDiamond;
		diamond->Load();
		diamond->AttachTo(item);
	}
	break;

	case MD_BLUEDIAMOND_ITEM:
	{
		CBlueDiamond* bluediamond = new CBlueDiamond;
		bluediamond->Load();
		bluediamond->AttachTo(item);
	}

	case MD_REDDIAMOND_ITEM:
	{
		CRedDiamond* reddiamond = new CRedDiamond;
		reddiamond->Load();
		reddiamond->AttachTo(item);
	}
	case MD_ARROW_ITEM:
	{
		CArrowItem* arrow = new CArrowItem;
		arrow->Load();
		arrow->AttachTo(item);
	}
	break;
	
	case MD_TRAP_ITEM:
	{
		CTrapItem* trap = new CTrapItem;
		trap->Load();
		trap->AttachTo(item);
	}
	break;

	case MD_LIFEBOX_ITEM:
	{
		CLifeBoxItem* lifebox = new CLifeBoxItem;
		lifebox->Load();
		lifebox->AttachTo(item);
	}
	break;
	
	case MD_GREEN_LIFEBOX_ITEM:
	{
		CGreenLifeBoxItem* lifebox = new CGreenLifeBoxItem;
		lifebox->Load();
		lifebox->AttachTo(item);
	}
	break;

	case MD_COWCOIN_ITEM:
	{
		CCowCoinItem *coin = new CCowCoinItem;
		coin->Load();
		coin->AttachTo(item);
	}

	default:

		break;
	}
}

void CWorld::LoadItemStage(int stage)
{
/*	switch(stage)
	{
		case 1:
			LoadItem(MD_ARROW_ITEM);
			LoadItem(MD_TRAP_ITEM);
			LoadItem(MD_ARROW_ITEM);
			LoadItem(MD_GREEN_LIFEBOX_ITEM);
			LoadItem(MD_TRAP_ITEM);
			LoadItem(MD_LIFEBOX_ITEM);
			break;
		case 2:
			LoadItem(MD_ARROW_ITEM);
			LoadItem(MD_TRAP_ITEM);
			LoadItem(MD_LIFEBOX_ITEM);
			LoadItem(MD_GREEN_LIFEBOX_ITEM);
			LoadItem(MD_ARROW_ITEM);
			LoadItem(MD_TRAP_ITEM);
			break;
		case 3:
			LoadItem(MD_ARROW_ITEM);
			LoadItem(MD_TRAP_ITEM);
			LoadItem(MD_LIFEBOX_ITEM);
			LoadItem(MD_GREEN_LIFEBOX_ITEM);
			LoadItem(MD_ARROW_ITEM);
			LoadItem(MD_TRAP_ITEM);
			break;
		case 4:
			LoadItem(MD_ARROW_ITEM);
			LoadItem(MD_TRAP_ITEM);
			LoadItem(MD_LIFEBOX_ITEM);
			LoadItem(MD_GREEN_LIFEBOX_ITEM);
			LoadItem(MD_ARROW_ITEM);
			LoadItem(MD_TRAP_ITEM);
			break;
		case 5:
			LoadItem(MD_ARROW_ITEM);
			LoadItem(MD_TRAP_ITEM);
			LoadItem(MD_LIFEBOX_ITEM);
			LoadItem(MD_GREEN_LIFEBOX_ITEM);
			LoadItem(MD_ARROW_ITEM);
			LoadItem(MD_TRAP_ITEM);
			break;
		default:

			break;
	}*/
}

int CWorld::CountObjectTypes(CObject *object, type_model typeObject)
{
	int count = 0;
	CObject *c1;
	CObject *c2;

	c1 = (CObject*)object->childNode;

	while (c1 != NULL)
	{
		c2 = c1;
		do
		{
			if (c2->type == typeObject)
			{
				count++;
			}

			c2 = (CObject*)c2->nextNode;
		}while (!c2->IsFirstChild());

		c1 = (CObject*)c1->childNode;
	}

	return count;
}