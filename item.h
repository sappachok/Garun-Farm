#ifndef ITEM_INCLUDE
#define ITEM_INCLUDE

#include "md2.h"

class CItem:public CMD2Model
{
private:
	float deta;
protected:

	void OnAnimate(scalar_t deltaTime)
	{
		time += deltaTime;
	}

	void OnDraw(CCamera *camera)
	{
		CMD2Model::SetAnimationCustom(1,1);
		Move(position.x, position.y, position.z);
		Rotate(deta);

		if(blend != TRUE)
		{
			Render();
		}
		else
		{
			glEnable(GL_BLEND);
			Render();
			glDisable(GL_BLEND);
		}

	}

	void OnPrepare()
	{
		deta += 1.0f;
	}

public:
	BOOL blend;
	float time;

	CItem()
	{ 
		type = MD_ITEM; 
		size = 2; 
		deta = 0; 
		time = 0;
		blend = FALSE;
	}

	~CItem(){}

	void CreateItem(float x, float y, float z)
	{
		position.x = x;
		position.y = y+1;
		position.z = z;
	}
};

class CDiamond:public CItem
{
public:
	CDiamond() { type = MD_DIAMOND_ITEM; size = 2; }
	~CDiamond() {}

	void Load()
	{
		CMD2Model::Load("Data\\model\\item\\diamond02.md2","Data\\model\\item\\diamond02.tga",0.05f);
	}
};

class CBlueDiamond:public CItem
{
protected:
	void OnPrepare()
	{
		if(time > 7)
		{
			blend = TRUE;
		}

		if(time > 10)
		{
			isDead = true;
		}

		CItem::OnPrepare();
	}

public:	
	CBlueDiamond() { type = MD_BLUEDIAMOND_ITEM; size = 2; }
	~CBlueDiamond() {}

	void Load()
	{
		CMD2Model::Load("Data\\model\\item\\diamond02.md2","Data\\model\\item\\diamond02.tga",0.05f);
	}
};

class CRedDiamond:public CItem
{
protected:
	void OnPrepare()
	{
		if(time > 7)
		{
			blend = TRUE;
		}

		if(time > 10)
		{
			isDead = true;
		}

		CItem::OnPrepare();
	}

public:
	CRedDiamond() { type = MD_REDDIAMOND_ITEM; size = 2; }
	~CRedDiamond() {}

	void Load()
	{
		CMD2Model::Load("Data\\model\\item\\diamond02.md2","Data\\model\\item\\reddiamond.tga",0.1f);
	}
};

class CArrowItem:public CItem
{
public:
	CArrowItem() { type = MD_ARROW_ITEM; size = 2; }
	~CArrowItem() {}

	void Load()
	{
		CMD2Model::Load("Data\\model\\garun\\arrow03.md2","Data\\model\\item\\demon.tga",0.2f);
	}
};

class CTrapItem:public CItem
{
public:
	CTrapItem() { type = MD_TRAP_ITEM; size = 2; }
	~CTrapItem() {}

	void Load()
	{
		CMD2Model::Load("Data\\model\\item\\trap02.md2","Data\\model\\item\\demon.tga",0.5f);
	}
};

class CLifeBoxItem:public CItem
{
public:
	CLifeBoxItem() { type = MD_LIFEBOX_ITEM; size = 2; }
	~CLifeBoxItem() {}

	void Load()
	{
		CMD2Model::Load("Data\\model\\item\\box.md2","Data\\model\\item\\lifebox.tga",0.5f);
	}
};

class CGreenLifeBoxItem:public CItem
{
public:
	CGreenLifeBoxItem() { type = MD_GREEN_LIFEBOX_ITEM; size = 2; }
	~CGreenLifeBoxItem() {}

	void Load()
	{
		CMD2Model::Load("Data\\model\\item\\box.md2","Data\\model\\item\\greenlifebox.tga",0.5f);
	}
};

class CCowCoinItem:public CItem
{
public:
	CCowCoinItem() { type = MD_COWCOIN_ITEM; size = 1; }
	~CCowCoinItem() {}

	void Load()
	{
		CMD2Model::Load("Data\\model\\item\\coin.md2","Data\\model\\item\\coin.tga",0.1f);
	}
};

#endif