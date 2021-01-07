#include "InputSystem.h"


CKeyboard::CKeyboard(LPDIRECTINPUT7 pDI, HWND hwnd, HINSTANCE hInst)
{
  if (FAILED(DirectInputCreateEx(hInst, DIRECTINPUT_VERSION, 
        IID_IDirectInput7, (void**)&pDI, NULL)))
  {

  }

  if (FAILED(pDI->CreateDeviceEx(GUID_SysKeyboard, IID_IDirectInputDevice7,
        (void**)&m_pDIDev, NULL)))
  {
    // error processing
  }

  if (FAILED(m_pDIDev->SetDataFormat(&c_dfDIKeyboard)))
  {
    // error processing
  }

  if (FAILED(m_pDIDev->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
  {
    // error processing
  }

  if (FAILED(m_pDIDev->Acquire()))
  {
    // error processing
  }

  Clear();
}


CKeyboard::~CKeyboard()
{
  if (m_pDIDev)
  {
    m_pDIDev->Unacquire();
    m_pDIDev->Release();
  }
}


bool CKeyboard::Acquire()
{
  Clear();
  return (!FAILED(m_pDIDev->Acquire()));
}


bool CKeyboard::Unacquire()
{
  Clear();
  return (!FAILED(m_pDIDev->Unacquire()));
}


bool CKeyboard::Update()
{
  if (FAILED(m_pDIDev->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
  {
    if (FAILED(m_pDIDev->Acquire()))
    {
      return false;
    }
    if (FAILED(m_pDIDev->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
    {
      return false;
    }
  }
  return true;
}


CMouse::CMouse(LPDIRECTINPUT7 pDI, HWND hwnd, bool isExclusive)
{
  if (FAILED(pDI->CreateDeviceEx(GUID_SysMouse, IID_IDirectInputDevice7,
        (void**)&m_pDIDev, NULL)))
  {
    // error processing
  }

  if (FAILED(m_pDIDev->SetDataFormat(&c_dfDIMouse)))
  {
    // error processing
  }

  DWORD flags;

  if (isExclusive)
    flags = DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
  else
    flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;


  if (FAILED(m_pDIDev->SetCooperativeLevel(hwnd, flags)))
  {
    // error processing
  }

  if (FAILED(m_pDIDev->Acquire()))
  {
    // error processing
  }

  if (FAILED(m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state)))
  {
    // error processing
  }
}


bool CMouse::Update()
{
  if (FAILED(m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state)))
  {
    if (FAILED(m_pDIDev->Acquire()))
    {
      return false;
    }
    if (FAILED(m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state)))
    {
      return false;
    }
  }

  return true;
}




CMouse::~CMouse()
{
  if (m_pDIDev)
  {
    m_pDIDev->Unacquire();
    m_pDIDev->Release();
  }
}


bool CMouse::Acquire()
{
  return (!FAILED(m_pDIDev->Acquire()));
}


bool CMouse::Unacquire()
{
  return (!FAILED(m_pDIDev->Unacquire()));
}


CJoystick::CJoystick(LPDIRECTINPUT7 pDI, HINSTANCE appInstance)
{
}


CJoystick::~CJoystick()
{
  if (m_pDIDev)
  {
    m_pDIDev->Unacquire();
    m_pDIDev->Release();
  }
}


bool CInputSystem::Initialize(HWND hwnd, HINSTANCE appInstance, bool isExclusive, DWORD flags)
{
  m_pKeyboard = NULL; 
  m_pMouse = NULL; 
  m_pJoystick = NULL; 

  if (FAILED(DirectInputCreateEx(appInstance, DIRECTINPUT_VERSION, 
        IID_IDirectInput7, (void**)&m_pDI, NULL)))
    return false;

  if (flags & IS_USEKEYBOARD)
  {
    m_pKeyboard = new CKeyboard(m_pDI, hwnd, appInstance);
  }
  if (flags & IS_USEMOUSE)
  {
    m_pMouse = new CMouse(m_pDI, hwnd, false);
  }
  if (flags & IS_USEJOYSTICK)
  {
    m_pJoystick = new CJoystick(m_pDI, appInstance);
  }
  return true;
}


bool CInputSystem::Shutdown()
{
  UnacquireAll();
  if (m_pKeyboard)
  {
    delete m_pKeyboard;
    m_pKeyboard = NULL;
  }

  if (m_pKeyboard)
  {
    delete m_pMouse;
    m_pMouse = NULL;
  }

  if (m_pJoystick)
  {
    delete m_pJoystick;
    m_pJoystick = NULL;
  }

  if (FAILED(m_pDI->Release()))
    return false;

  return true;
}

bool CInputSystem::Update()
{
  if (m_pKeyboard)
    m_pKeyboard->Update();
  if (m_pMouse)
    m_pMouse->Update();
  if (m_pJoystick)
    m_pJoystick->Update();

  return true;
}


void CInputSystem::AcquireAll()
{
  if (m_pKeyboard)
    m_pKeyboard->Acquire();
  if (m_pMouse)
    m_pMouse->Acquire();
  if (m_pJoystick)
    m_pJoystick->Acquire();
}


void CInputSystem::UnacquireAll()
{
  if (m_pKeyboard)
    m_pKeyboard->Unacquire();
  if (m_pMouse)
    m_pMouse->Unacquire();
  if (m_pJoystick)
    m_pJoystick->Unacquire();
}


bool CJoystick::Acquire()
{
  return (!FAILED(m_pDIDev->Acquire()));
}


bool CJoystick::Unacquire()
{
  return (!FAILED(m_pDIDev->Unacquire()));
}


bool CJoystick::Update()
{
  return true;
}
