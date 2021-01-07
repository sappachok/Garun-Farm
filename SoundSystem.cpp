#ifndef CDSOUND_CPP
#define CDSOUND_CPP
#endif

#include "SoundSystem.h"

LPDIRECTSOUND lpDS = NULL;
LPDIRECTSOUNDBUFFER lpdsb[20];

LPDIRECTSOUNDBUFFER lpDSBuffer = NULL;



//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

LPDIRECTSOUND           g_pDS             = NULL;   // DirectSound object
LPDIRECTSOUNDBUFFER     g_pDSBuffer       = NULL;   // Secondary sound buffer
LPDIRECTSOUND3DBUFFER   g_pDS3DBuffer     = NULL;   // 3D sound buffer
LPDIRECTSOUND3DLISTENER g_pDSListener     = NULL;   // 3D listener object

DS3DBUFFER              g_dsBufferParams;           // 3D buffer properties
DS3DLISTENER            g_dsListenerParams;         // Listener properties
DWORD                   g_dwBufferBytes;
extern BOOL             g_bDeferSettings;

CWAVLoader	wav;

BOOL CSoundSystem::Create(HWND hwnd)
{
	DirectSoundCreate(NULL,&lpDS,NULL);

	if(!lpDS)
	{
		lpDS->Release();
		lpDS = NULL;
	}

//	lpDS->SetCooperativeLevel(hwnd,DSSCL_NORMAL);
	lpDS->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);

	return TRUE;
}

LPDIRECTSOUNDBUFFER CSoundSystem::LoadFromFile(LPCTSTR lpszFileName)
{
	LPDIRECTSOUNDBUFFER lpsb;
	DSBUFFERDESC dsbd;

	wav.Load(lpszFileName);

	memset(&dsbd,0,sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_STATIC;
	dsbd.dwBufferBytes = wav.GetLength();
	dsbd.lpwfxFormat = wav.GetFormat();

	dsbd.guid3DAlgorithm = DS3DALG_HRTF_FULL;

	lpDS->CreateSoundBuffer(&dsbd,&lpsb,NULL);
//	g_pDS->CreateSoundBuffer( &dsbd, &g_pDSBuffer, NULL ); 

    lpsb->QueryInterface( IID_IDirectSound3DBuffer, 
                                                  (VOID**)&g_pDS3DBuffer );

    DWORD dwApplyFlag = DS3D_IMMEDIATE;

    g_dsListenerParams.flDopplerFactor = 1.0f;
    g_dsListenerParams.flRolloffFactor = 1.0f;

	g_dwBufferBytes = dsbd.dwBufferBytes;

   // Get the 3D buffer parameters
    g_dsBufferParams.dwSize = sizeof(DS3DBUFFER);
    g_pDS3DBuffer->GetAllParameters( &g_dsBufferParams );

    g_dsBufferParams.dwMode = DS3DMODE_HEADRELATIVE;
    g_pDS3DBuffer->SetAllParameters( &g_dsBufferParams, DS3D_IMMEDIATE );  

    if( g_pDSListener )
        g_pDSListener->SetAllParameters( &g_dsListenerParams, dwApplyFlag );

    g_dsBufferParams.flMinDistance = 0.1f;
    g_dsBufferParams.flMaxDistance = 20.0f;

    if( g_pDS3DBuffer )
        g_pDS3DBuffer->SetAllParameters( &g_dsBufferParams, dwApplyFlag );

	DWORD buflen,buflen2;
	void* bufptr;

	lpsb->Lock(0,g_dwBufferBytes,&bufptr,&buflen,
		NULL,&buflen2,0L);

	memcpy(bufptr,wav.GetData(),wav.GetLength());
	lpsb->Unlock(bufptr,buflen,NULL,buflen2);

	return lpsb;
}

BOOL CSoundSystem::LoadFromFile(LPDIRECTSOUNDBUFFER &lpdsBuffer, 
								LPDIRECTSOUND3DBUFFER &lpds3DBuffer,LPCTSTR lpszFileName)
{
	DSBUFFERDESC dsbd;

	wav.Load(lpszFileName);

	memset(&dsbd,0,sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_STATIC;
	dsbd.dwBufferBytes = wav.GetLength();
	dsbd.lpwfxFormat = wav.GetFormat();

	dsbd.guid3DAlgorithm = DS3DALG_HRTF_FULL;

	lpDS->CreateSoundBuffer(&dsbd,&lpdsBuffer,NULL);
//	g_pDS->CreateSoundBuffer( &dsbd, &g_pDSBuffer, NULL ); 

    lpdsBuffer->QueryInterface( IID_IDirectSound3DBuffer, 
                                                  (VOID**)&lpds3DBuffer );

    DWORD dwApplyFlag = DS3D_IMMEDIATE;

    g_dsListenerParams.flDopplerFactor = 1.0f;
    g_dsListenerParams.flRolloffFactor = 1.0f;

	g_dwBufferBytes = dsbd.dwBufferBytes;

   // Get the 3D buffer parameters
    g_dsBufferParams.dwSize = sizeof(DS3DBUFFER);
    lpds3DBuffer->GetAllParameters( &g_dsBufferParams );

    g_dsBufferParams.dwMode = DS3DMODE_HEADRELATIVE;
    lpds3DBuffer->SetAllParameters( &g_dsBufferParams, DS3D_IMMEDIATE );  

    if( g_pDSListener )
        g_pDSListener->SetAllParameters( &g_dsListenerParams, dwApplyFlag );

    g_dsBufferParams.flMinDistance = 0.1f;
    g_dsBufferParams.flMaxDistance = 20.0f;

    if( lpds3DBuffer )
        lpds3DBuffer->SetAllParameters( &g_dsBufferParams, dwApplyFlag );

	DWORD buflen,buflen2;
	void* bufptr;

	lpdsBuffer->Lock(0,g_dwBufferBytes,&bufptr,&buflen,
		NULL,&buflen2,0L);

	memcpy(bufptr,wav.GetData(),wav.GetLength());
	lpdsBuffer->Unlock(bufptr,buflen,NULL,buflen2);

	return TRUE;
}

void CSoundSystem::SetPosition(LPDIRECTSOUNDBUFFER lpsb,float posx,float posy,float posz,
		float velx,float vely,float velz)
{
    DWORD dwApplyFlag = DS3D_IMMEDIATE;

	D3DVECTOR vPosition;
    vPosition.x = posx;
    vPosition.y = posy;
    vPosition.z = posz;

    D3DVECTOR vVelocity;
    vVelocity.x = velx;
    vVelocity.y = vely;
    vVelocity.z = velz;

    memcpy( &g_dsBufferParams.vPosition, &vPosition, sizeof(D3DVECTOR) );
    memcpy( &g_dsBufferParams.vVelocity, &vVelocity, sizeof(D3DVECTOR) );

    g_pDS3DBuffer->SetAllParameters( &g_dsBufferParams, dwApplyFlag );
}

void CSoundSystem::SetPosition(LPDIRECTSOUND3DBUFFER lpdb3DBuffer,float posx,float posy,float posz,
		float velx,float vely,float velz)
{
    DWORD dwApplyFlag = DS3D_IMMEDIATE;

	D3DVECTOR vPosition;
    vPosition.x = posx;
    vPosition.y = posy;
    vPosition.z = posz;

    D3DVECTOR vVelocity;
    vVelocity.x = velx;
    vVelocity.y = vely;
    vVelocity.z = velz;

    memcpy( &g_dsBufferParams.vPosition, &vPosition, sizeof(D3DVECTOR) );
    memcpy( &g_dsBufferParams.vVelocity, &vVelocity, sizeof(D3DVECTOR) );

    lpdb3DBuffer->SetAllParameters( &g_dsBufferParams, dwApplyFlag );
}

void CSoundSystem::Play(LPDIRECTSOUNDBUFFER lpdsb)
{
	if(lpdsb == NULL)
		return;

	lpdsb->Stop();
	lpdsb->SetCurrentPosition(0);

	DWORD dwStatus;
	lpdsb->GetStatus(&dwStatus);
	lpdsb->Play(0,0,0);
}

void CSoundSystem::Stop(LPDIRECTSOUNDBUFFER lpdsb)
{
	if(lpdsb == NULL) return;

	lpdsb->Stop();
	lpdsb->SetCurrentPosition(0L);
}

void CWAVLoader::Load(LPCTSTR lpszFilename)
{
	char Buffer[5];
	Buffer[4] = 0;
	DWORD dwNumRead;
	DWORD dwLength;

	UCHAR* ucTemp;
	HANDLE hfile = CreateFile(lpszFilename,GENERIC_READ,
		FILE_SHARE_READ,NULL,OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,NULL);
	ReadFile(hfile,Buffer,4,&dwNumRead,NULL);
	ReadFile(hfile,&dwLength,sizeof(dwLength),&dwNumRead,NULL);
	ReadFile(hfile,Buffer,4,&dwNumRead,NULL);
	
	bool done = false;
	while(!done)
	{
		ReadFile(hfile,Buffer,4,&dwNumRead,NULL);
		ReadFile(hfile,&dwLength,sizeof(dwLength),&dwNumRead,NULL);
		ucTemp =  new UCHAR[dwLength];
		ReadFile(hfile,ucTemp,dwLength,&dwNumRead,NULL);

		if(strcmp("fmt ",Buffer)==0)
		{
			lpWfx = new WAVEFORMATEX;
			memset(lpWfx,0,sizeof(WAVEFORMATEX));
			memcpy(lpWfx,ucTemp,dwLength);
		}
		if(strcmp("data",Buffer)==0)
		{
			ucData = new UCHAR[dwLength];
			dwDataLength=dwLength;
			memcpy(ucData,ucTemp,dwDataLength);

			done=true;
		}
		delete ucTemp;
	}
	CloseHandle(hfile);
}

HRESULT CSoundSystem::InitDirectSound( HWND hDlg )
{
    HRESULT             hr;
    DSBUFFERDESC        dsbdesc;
    LPDIRECTSOUNDBUFFER pDSBPrimary;

    // Initialize COM
    if( hr = CoInitialize( NULL ) )
        return hr;

    // Create IDirectSound using the primary sound device
    if( FAILED( hr = DirectSoundCreate( NULL, &lpDS, NULL ) ) )
        return hr;

    // Set coop level to DSSCL_PRIORITY
    if( FAILED( hr = lpDS->SetCooperativeLevel( hDlg, DSSCL_PRIORITY ) ) )
        return hr;

    // Obtain primary buffer, asking it for 3D control
    ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
    if( FAILED( hr = lpDS->CreateSoundBuffer( &dsbdesc, &pDSBPrimary, NULL ) ) )
        return hr;

    if( FAILED( hr = pDSBPrimary->QueryInterface( IID_IDirectSound3DListener, 
                                                  (VOID**)&g_pDSListener ) ) )
        return hr;

    // Get listener parameters
    g_dsListenerParams.dwSize = sizeof(DS3DLISTENER);
    g_pDSListener->GetAllParameters( &g_dsListenerParams );

    // Set primary buffer format to 22kHz and 16-bit output.
    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    wfx.nChannels       = 2; 
    wfx.nSamplesPerSec  = 22050; 
    wfx.wBitsPerSample  = 16; 
    wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    if( FAILED( hr = pDSBPrimary->SetFormat(&wfx) ) )
        return hr;

    // Release the primary buffer, since it is not need anymore
    SAFE_RELEASE( pDSBPrimary );

    return S_OK;
}	
