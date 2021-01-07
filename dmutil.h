/*
		dmuti.cpp (Direct Sound Utility)
		Version 1.0
		written by Manop Pornpeanvichanon
*/

#include<dmusicc.h>
#include<dmusici.h>

IDirectMusicPerformance* CreatePerformance(void);

IDirectMusicLoader* CreateLoader(void);

IDirectMusic* CreateDirectMusic(
				IDirectMusicPerformance*,
				IDirectSound*, HWND);

IDirectMusicSegment* LoadMIDISegment(
		IDirectMusicLoader*, 
		IDirectMusicPerformance *, 
		char*);

void PlayDMSegment(IDirectMusicPerformance*,
				   IDirectMusicSegment*);

/*IDirectMusicPort* CreateAndAddDMPort(
			IDirectMusic*,
			IDirectMusicPerformance*);

void ReleaseDMPort(IDirectMusicPerformance*,
				 IDirectMusicPort*);*/

void ReleaseDMAll(IDirectMusic*,
				  IDirectMusicPerformance*,
				  IDirectMusicLoader*,
				  IDirectMusicSegment*);

void ReleaseDMSegment(IDirectMusicPerformance*,
					  IDirectMusicLoader*,
					  IDirectMusicSegment*);

void ReleaseDMAll(IDirectMusic* pdm,
				  IDirectMusicPerformance* pdmp,
				  IDirectMusicLoader* pdml,
				  IDirectMusicSegment* pdms)
{
	pdmp->Stop( NULL, NULL, 0, 0 );
     
    pdms->SetParam(GUID_Unload, -1, 0, 0, (void*)pdmp);
 
    pdms->Release();

    pdmp->CloseDown();
    pdmp->Release();

    pdml->Release();

	pdm->Release();

    CoUninitialize();
}


void ReleaseDMSegment(IDirectMusicPerformance* pdmp,
					  IDirectMusicLoader* pdml,
					  IDirectMusicSegment* pdms)
{
	IDirectMusicObject  *pdmo = NULL;

	pdmp->Stop(NULL, NULL, 0, 0);

	pdms->SetParam(GUID_Unload,
					0xFFFFFFFF,
					0, 0, (void*)pdmp);

	if(SUCCEEDED(pdms->QueryInterface(IID_IDirectMusicObject, (void**)pdmo)))
	{
		pdml->ReleaseObject(pdmo);
	}

	pdms->Release();

	pdms = NULL;
}

void PlayDMSegment(IDirectMusicPerformance* pdmp,
				   IDirectMusicSegment* pdms)
{
	pdmp->PlaySegment(pdms, 0, 0, NULL);
}


/*void ReleaseDMPort(IDirectMusicPerformance* pdmp,
				 IDirectMusicPort* pdmpo)
{
	pdmp->RemovePort(pdmpo);
	pdmpo->Release();
}

IDirectMusicPort* CreateAndAddDMPort(
			IDirectMusic* pdm,
			IDirectMusicPerformance* pdmp)
{

	IDirectMusicPort*	pdmpo;
	DMUS_PORTPARAMS     portParams;
	DMUS_PORTCAPS*		pPortCaps;
	LPUNKNOWN			lpunknow;
	
	ZeroMemory(&portParams, sizeof(portParams));
	portParams.dwEffectFlags = DMUS_EFFECT_NONE;

	portParams.dwChannelGroups = 1;
	portParams.dwValidParams =
		DMUS_PORTPARAMS_CHANNELGROUPS | 
		DMUS_PORTPARAMS_EFFECTS;

	pdm->EnumPort(0, pPortCaps);

	pdm->CreatePort(
		&pPortCaps->guidPort,
		&portParams,
		&pdmpo,
		(_GUID*)NULL);
	

	pdmpo->Activate(TRUE);

	pdmp->AddPort(pdmpo);

	pdmp->AssignPChannelBlock(0, pdmpo, 1);

	return pdmpo;
}*/

IDirectMusic* CreateDirectMusic(
				IDirectMusicPerformance* pdmp,
				IDirectSound* pds, HWND hwnd)
{
	IDirectMusic* pdm = NULL;

	pdmp->Init(&pdm, pds, hwnd);

	return pdm;
}

IDirectMusicPerformance* CreatePerformance(void)
{
    IDirectMusicPerformance*	pPerf = NULL;

	CoInitialize(NULL);
 
    if (FAILED(CoCreateInstance(
            CLSID_DirectMusicPerformance,
            NULL,
            CLSCTX_INPROC, 
            IID_IDirectMusicPerformance2,
            (void**)&pPerf
        )))
    {
        pPerf = NULL;
    }

    return pPerf;
}


IDirectMusicLoader* CreateLoader(void)
{
    IDirectMusicLoader* pLoader;
 
    if (FAILED(CoCreateInstance(
            CLSID_DirectMusicLoader,
            NULL,
            CLSCTX_INPROC, 
            IID_IDirectMusicLoader,
            (void**)&pLoader
        )))
    {
        pLoader = NULL;
    }
    return pLoader;
}


IDirectMusicSegment* LoadMIDISegment(
		IDirectMusicLoader* pLoader, 
		IDirectMusicPerformance *g_pPerf, 
		char* pfn)
{
	IDirectMusicSegment*	pSegment = NULL;
	IDirectMusicObject*		pObject = NULL;
	DMUS_OBJECTDESC     dmodFile;

    dmodFile.dwSize = sizeof(dmodFile);
    dmodFile.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME | DMUS_OBJ_FULLPATH;
    dmodFile.guidClass = CLSID_DirectMusicSegment;

	MultiByteToWideChar(
		CP_ACP, 0, pfn, -1,
        dmodFile.wszFileName,
        sizeof(dmodFile.wszFileName)/sizeof(dmodFile.wszFileName[0]));

	pLoader->GetObject(
			&dmodFile,
            IID_IDirectMusicSegment2,
			(void**)&pSegment);

	g_pPerf->AddPort(NULL);

	pSegment->SetParam(GUID_StandardMIDIFile,
					0xFFFFFFFF,
					0, 0, (void*)g_pPerf);

	pSegment->SetParam(GUID_Download, -1, 0, 0, (void*)g_pPerf);

	return pSegment;
}