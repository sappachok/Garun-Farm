#ifndef _MUSIC_SYSTEM_CPP
#define _MUSIC_SYSTEM_CPP
#endif

#include "MusicSystem.h"
#include "dmutil.h"

CMusicSystem::CMusicSystem()
{
	pdm = NULL;
	pdml = NULL;
	pdmp = NULL;
}

CMusicSystem::~CMusicSystem()
{

}

bool CMusicSystem::InitDirectMusic(HWND hWindow)
{
	pdmp = CreatePerformance();
	if(pdmp == NULL)
	{
		MessageBox(NULL,"Create Music Performance Failed!!","Error",MB_OK);
		return false;
	}

	pdm = CreateDirectMusic(pdmp, NULL, hWindow);
	if(pdm == NULL)
	{
		MessageBox(NULL,"Create DirectMusic Failed!!","Error",MB_OK);
		return false;
	}

	pdml = CreateLoader();
	if(pdml == NULL)
	{
		MessageBox(NULL,"Create MusicLoader Failed!!","Error",MB_OK);
		return false;
	}

	pdms1 = LoadMIDISegment(pdml,pdmp,"Data\\Music\\002.mid");

	if(pdms1 == NULL)
	{
		MessageBox(NULL,"Load MIDISegment Failed!!","Error",MB_OK);
		return false;
	}


	return true;
}

bool CMusicSystem::LoadMIDI(IDirectMusicSegment *pdms,char *filename)
{
	pdms = LoadMIDISegment(pdml,pdmp,filename);

	if(pdms == NULL)
	{
		MessageBox(NULL,"Load MIDISegment Failed!!","Error",MB_OK);
		return false;
	}

	return true;
}

IDirectMusicSegment* CMusicSystem::LoadMIDI(char *filename)
{
	pdms1 = LoadMIDISegment(pdml,pdmp,filename);

	if(pdms1 == NULL)
	{
		MessageBox(NULL,"Load MIDISegment Failed!!","Error",MB_OK);
		return false;
	}

	return pdms1;
}

bool CMusicSystem::IsPlay(IDirectMusicSegment *pdms)
{
	if(pdmp->IsPlaying(pdms, NULL) == S_FALSE)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CMusicSystem::Play(IDirectMusicSegment *pdms)
{
	pdms->SetRepeats(200);
	pdmp->PlaySegment(pdms, 0, 0, NULL);
}

void CMusicSystem::Stop(IDirectMusicSegment *pdms)
{
	pdmp->Stop(pdms, NULL, 0, 0);
}

void CMusicSystem::DestroyMusic()
{
	pdm->Release();
	pdml->Release();
	pdmp->Release();
}
