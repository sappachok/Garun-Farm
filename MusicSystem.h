#ifndef _MUSIC_SYSTEM_INCLUDE
#define _MUSIC_SYSTEM_INCLUDE

#include <dmusicc.h>
#include <dmusici.h>

class CMusicSystem
{
private:
	IDirectMusic			*pdm;
	IDirectMusicLoader		*pdml;
	IDirectMusicPerformance *pdmp;
	IDirectMusicSegment		*pdms1;

public:
	CMusicSystem();
	~CMusicSystem();

	bool InitDirectMusic(HWND hWindow);
	bool LoadMIDI(IDirectMusicSegment *pdms,char *filename);
	IDirectMusicSegment* LoadMIDI(char *filename);
	void Play(IDirectMusicSegment *pdms);
	bool IsPlay(IDirectMusicSegment *pdms);
	void Stop(IDirectMusicSegment *pdms);
	void DestroyMusic();
};

#endif