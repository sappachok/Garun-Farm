#ifndef _ATTACK_INCLUDE
#define _ATTACK_INCLUDE

#include "entity.h"
#include "winmain.h"

#define HUMMER_HALO		1.5

struct t_line
{
	CVector str;
	CVector end;
};

class CWolfHit:public CEntity
{
protected:
	void OnPrepare();
	void OnCollision(CObject *collisionObject) { }
public:
	int attack;
	int count;
	CVector st_Shoot;
	CVector pTarget;

	CWolfHit()
	{
		type = MD_BULLET;
		size = 2;
		count = 0;
		attack = 0;
	}

	~CWolfHit() {}

	void Create(float stx,float sty,float stz)
	{
		position.x = st_Shoot.x = stx; 
		position.y = st_Shoot.y = sty;
		position.z = st_Shoot.z = stz;
	}
};

class CHummerHit:public CEntity
{
private:

protected:
	void OnAnimate(scalar_t deltaTime) {}
	void OnDraw(CCamera *camera) {}
	void OnPrepare();
	void OnCollision(CObject *collisionObject);
public:
	float heighEffect;
	bool damaged;

	CHummerHit()
	{
		type = MD_PLAYER_HIT;
		size = 1;
		heighEffect = 0;
		damaged = false;
	}

	~CHummerHit() {}

	void Create(float stx,float sty,float stz,float st_height);
};

class CWind:public CEntity
{
protected:
	void OnAnimate(scalar_t deltaTime) 
	{
		shoottime += deltaTime;
		
		if((shoottime >= 0.1) && (numLine < 19) && (m_object->isDead != true))
		{
			numLine++;
			stLine++;
			shoottime = 0;
			wind_line[numLine].str.x = wind_line[numLine-1].end.x;
			wind_line[numLine].str.y = wind_line[numLine-1].end.y;
			wind_line[numLine].str.z = wind_line[numLine-1].end.z;
		}
		else if((shoottime >= 0.1) && (numLine >= 19))
		{
			shoottime = 0;
			stLine++;
		}

		if(stLine == numLine) isDead = true;

		if(m_object->isDead == true)
		{

		}

		if(m_object == NULL)
		{
			wind_line[numLine].str.x = 0;
			wind_line[numLine].str.y = 100;
			wind_line[numLine].str.z = 0;
		}

	}

	void OnDraw(CCamera *camera) 
	{

	}

	void OnPrepare()
	{
		//////////////////////////////////////////////
		wind_line[numLine].end.x = m_object->position.x;
		wind_line[numLine].end.y = m_object->position.y;
		wind_line[numLine].end.z = m_object->position.z;

		/////// WIND ///////////////////////////////
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor3f(1,0,0);

		for(int i=stLine;i<numLine;i++)
		{
			if(i>=0)
			{
				glVertex3f( wind_line[i].str.x, wind_line[i].str.y, wind_line[i].str.z);
				glVertex3f( wind_line[i].end.x, wind_line[i].end.y, wind_line[i].end.z);
			}
		}
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		////////////////////////////////////////////
	}

public:
	float shoottime;
	int	numLine;
	int stLine;
	t_line wind_line[20];
	CObject *m_object;

	CWind() { numLine = 0; shoottime = 0; stLine = -10; };
	~CWind() {};

	void CreateWind(float stx,float sty,float stz)
	{
		position.x = stx;
		position.y = sty;
		position.z = stz;

		wind_line[0].str.x = position.x;
		wind_line[0].str.y = position.y;
		wind_line[0].str.z = position.z;
	}

	void SetAt(CObject *object)
	{
		m_object = object;
		CreateWind(object->position.x, object->position.y, object->position.z);
	}
};

class CArrow:public CEntity
{
protected:
	void OnAnimate(scalar_t deltaTime) 
	{
		directUp += (0.01f*position.y)*deltaTime;
		shoottime += deltaTime;

		if((shoottime >= 0.1) && (numLine < 19))
		{
			numLine++;
			shoottime = 0;
			wind_line[numLine].str.x = wind_line[numLine-1].end.x;
			wind_line[numLine].str.y = wind_line[numLine-1].end.y;
			wind_line[numLine].str.z = wind_line[numLine-1].end.z;
		}
	}
	void OnDraw(CCamera *camera) 
	{
		CMD2Model::SetAnimationCustom(0,0);
		Move(position.x,position.y,position.z);
		Render();
	}
	void OnPrepare()
	{
		//////	ARROW ///////////////////////////////
		position.x += float(cos(direction)*5);
		position.y += float(sin(-directUp)*5);
		position.z += float(sin(direction)*5);
		
		deta = deta+0.0f;

		if(position.y<0)
		{
			isDead = true;
		}

		CMD2Model::m_startFrame = 0;
	    CMD2Model::m_endFrame = 1;
		CMD2Model::m_currentFrame = 1;

		CMD2Model::SetAnimationCustom(0,0);
		Move(position.x,position.y,position.z);

		Render();

		//////////////////////////////////////////////
		wind_line[numLine].end.x = position.x;
		wind_line[numLine].end.y = position.y;
		wind_line[numLine].end.z = position.z;
	}


	void OnCollision(CObject *collisionObject)
	{
		if(collisionObject->type == MD_ENEMY)
		{
			if(!collisionObject->isDead)
			{
				if(m_sound != NULL)
				{
					m_sound->SetPosition(sound_attack.lpds3DBuffer,0,0,0, 0,0,0);
					m_sound->Play(sound_attack.lpdsBuffer);
				}
			}
		}
		else if(collisionObject->type == MD_FRIEND)
		{
			if(!collisionObject->isDead)
			{
				if(m_sound != NULL)
				{
					m_sound->SetPosition(sound_attack.lpds3DBuffer,0,0,0, 0,0,0);
					m_sound->Play(sound_attack.lpdsBuffer);
				}
			}
		}
	}

public:
	st_SoundBuffer sound_attack;

	CSoundSystem *m_sound;
	float attack;
	float directUp;
	float deta;
	float shoottime;

	t_line wind_line[20];
	int	numLine;

	CArrow()
	{		
		type = MD_ARROW;
		size = 0.3f;
		deta = 0;
		direction = 0;
		attack = 1;
		shoottime = 0;
		numLine = 0;

		Load("Data\\Model\\Garun\\arrow03.md2","Data\\Model\\Garun\\bult.tga",0.07f);
	}
	~CArrow(){}

	void Create(float stx,float sty,float stz)
	{
		position.x = stx;
		position.y = sty;
		position.z = stz;

		wind_line[0].str.x = position.x;
		wind_line[0].str.y = position.y;
		wind_line[0].str.z = position.z;
	}

	void LoadSound(CSoundSystem *sound)
	{
		m_sound = sound;
		m_sound->LoadFromFile(sound_attack.lpdsBuffer, sound_attack.lpds3DBuffer,
			"Data\\Sound\\Bow1.wav");
	}

	void ShootTo(float tox,float toy,float toz)
	{
		float theta;
		CVector diff;       // the vector from the enemy to the player
	    diff.x = position.x - tox;
		diff.y = position.y - toy;
		diff.z = position.z - toz;
	    diff.Normalize();

		if(diff.x <= 0)
		{
			theta = atan( diff.z / diff.x );
			direction = theta;
		}
		else
		{
			theta = atan( diff.z / diff.x );
			direction = theta - 135.1f;
		}
	}
};

class CTrap:public CEntity
{
protected:
	void OnAnimate(scalar_t deltaTime) {}
	void OnDraw(CCamera *camera) 
	{
		Move(position.x,0,position.z);
		Render();
	}
	void OnPrepare()
	{
		Move(position.x,0,position.z);
		Render();
	}

	void OnCollision(CObject *collisionObject)
	{
		if(collisionObject->type == MD_ENEMY)
		{
			if(!collisionObject->isDead)
			{
				if(m_sound != NULL)
				{
					m_sound->SetPosition(trap_close.lpds3DBuffer,0,0,0, 0,0,0);
					m_sound->Play(trap_close.lpdsBuffer);
				}
			}
		}
		else if(collisionObject->type == MD_PLAYER)
		{
			if(!collisionObject->isDead)
			{
				if(m_sound != NULL)
				{

				}
			}
		}
	}
public:
	st_SoundBuffer trap_close;

	float attack;
	float directUp;
	float deta;
	CSoundSystem *m_sound;

	CTrap()
	{		
		type = MD_TRAP;
		size = 1;
		deta = 0;
		direction = 0;
		attack = 3;
		Load("Data\\Model\\Garun\\trap02.md2","Data\\Model\\Garun\\trap.tga",0.5f);
	}
	~CTrap(){}

	void LoadSound(CSoundSystem *sound)
	{
		m_sound = sound;
		m_sound->LoadFromFile(trap_close.lpdsBuffer, trap_close.lpds3DBuffer, "Data\\Sound\\Close1.wav");
	}

	void Create(float stx,float sty,float stz)
	{
		position.x = stx;
		position.y = 0;
		position.z = stz;
	}

};

class CThunder:public CEntity
{
protected:
	void OnAnimate(scalar_t deltaTime) {}
	void OnDraw(CCamera *camera) 
	{

	}
	void OnPrepare()
	{

	}

	void OnCollision(CObject *collisionObject)
	{
	
	}
public:
	float attack;
	float directUp;
	float deta;

	CThunder()
	{		
		type = MD_THUNDER;
		size = 200;
		deta = 0;
		direction = 0;
		attack = 3;
	}
	~CThunder(){}

	void Create(float stx,float sty,float stz)
	{
		position.x = stx;
		position.y = 0;
		position.z = stz;
	}

};

class CGreenBox:public CEntity
{
protected:
	void OnAnimate(scalar_t deltaTime) {}
	void OnDraw(CCamera *camera) 
	{

	}
	void OnPrepare()
	{
	}

	void OnCollision(CObject *collisionObject)
	{
	
	}
public:
	float attack;
	float directUp;
	float deta;

	CGreenBox()
	{		
		type = MD_GREENBOX;
		size = 200;
		deta = 0;
		direction = 0;
		attack = 3;
	}
	~CGreenBox(){}

	void Create(float stx,float sty,float stz)
	{
		position.x = stx;
		position.y = 0;
		position.z = stz;
	}
};

class CSuperSonic:public CEntity
{
protected:
	void OnAnimate(scalar_t deltaTime) {}
	void OnDraw(CCamera *camera) 
	{

	}
	void OnPrepare()
	{
		position.x = position.x + cos(direction);
		position.z = position.z + sin(direction);
		position.y = 0;

		deta = deta+0.2f;

		if(position.y<0)
		{
			isDead = true;
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);


		for(int i=0;i<5;i++)
		{
		glPushMatrix();
		glTranslatef(
			position.x - (cos(direction)*(i*5)),
			position.y,
			position.z - (sin(direction)*(i*5)));

		glRotatef(rotate,0,1,0);
		glScalef(1-((float)i*0.2f),1-((float)i*0.2f),1-((float)i*0.2f));
		glBegin(GL_TRIANGLES);

			glColor3f(1,0,0); glVertex3f( 5, 0, 0);
			glColor3f(0,0,1); glVertex3f(-5, 5, 0);
			glColor3f(0,1,0); glVertex3f(-5, 0,-5);

			glColor3f(1,0,0); glVertex3f( 5, 0, 0);
			glColor3f(0,0,1); glVertex3f(-5, 5, 0);
			glColor3f(0,1,0); glVertex3f(-5, 0, 5);

		glEnd();
		glPopMatrix();
		}
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	}

	void OnCollision(CObject *collisionObject)
	{
		if(collisionObject->type == 0)
		{

		}
	}
public:
	float attack;
	float directUp;
	float deta;
	float rotate;

	CSuperSonic()
	{		
		type = MD_SUPERSONIC;
		size = 3;
		deta = 0;
		direction = 0;
		attack = 2;
		rotate = 0;
	}

	~CSuperSonic() {}

	void Create(float stx,float sty,float stz)
	{
		position.x = stx;
		position.y = 0;
		position.z = stz;
	}
};

#endif
