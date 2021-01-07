#ifndef CDSOUND_H
#define CDSOUND_H

#include <dsound.h>

#pragma comment(lib,"dsound.lib")

enum TypeSound
{
	S_OPEN,
	S_CURSOR,
	S_PLAYER_ATTACK,
	S_MONSTER_CRY,

	S_TEPACITY
};

typedef struct st_SoundBuffer
{
	LPDIRECTSOUNDBUFFER     lpdsBuffer;   // Secondary sound buffer
	LPDIRECTSOUND3DBUFFER   lpds3DBuffer;   // 3D sound buffer
}st_SoundBuffer;

class CSoundSystem
{
private:

public:
	CSoundSystem() {}
	~CSoundSystem() {}
	HRESULT InitDirectSound( HWND hDlg );
	BOOL Create(HWND hwnd);
	LPDIRECTSOUNDBUFFER LoadFromFile(LPCTSTR lpszFileName);
	BOOL LoadFromFile(LPDIRECTSOUNDBUFFER &lpdsBuffer, LPDIRECTSOUND3DBUFFER &lpds3DBuffer,LPCTSTR lpszFileName);

	void SetPosition(LPDIRECTSOUNDBUFFER lpsb,float posx,float posy,float posz,
		float velx,float vely,float velz);
	void SetPosition(LPDIRECTSOUND3DBUFFER lpdb3DBuffer,float posx,float posy,float posz,
		float velx,float vely,float velz);

	void CreateBuffer(LPDIRECTSOUNDBUFFER lpsb);
	void SetSound(TypeSound gs);
	void Play(void);
	void Play(LPDIRECTSOUNDBUFFER lpdsb);
	void Play(LPDIRECTSOUNDBUFFER lpdsb,DWORD dwReserved,DWORD dwPriority,DWORD dwFlags);
	void Play(TypeSound typeSound);
	void Stop(LPDIRECTSOUNDBUFFER lpdsb);
	void Stop(TypeSound typeSound);
};

class CWAVLoader
{
private:
	LPWAVEFORMATEX lpWfx;
	UCHAR*	ucData;
	DWORD	dwDataLength;
public:
//	CWAVLoader();
//	~CWAVLoader();
	DWORD GetLength() {return dwDataLength;};
	UCHAR* GetData() {return ucData;};
	LPWAVEFORMATEX GetFormat() {return lpWfx;};
	void Load(LPCTSTR lpszFilename);
	void Destroy();
};

#endif