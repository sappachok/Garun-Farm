#ifndef _INPUT_SYSTEM_INCLUDE
#define _INPUT_SYSTEM_INCLUDE

#include <dinput.h>

#pragma comment(lib, "dinput.lib")
#pragma comment(lib, "dxguid.lib")

#define IS_USEKEYBOARD  1
#define IS_USEMOUSE     2
#define IS_USEJOYSTICK  4

class CKeyboard
{
public:
  CKeyboard(LPDIRECTINPUT7 pDI, HWND hwnd, HINSTANCE hInst);
  ~CKeyboard();

  bool  KeyDown(char key) { return (m_keys[key] & 0x80) ? true : false; }
  bool  KeyUp(char key) { return (m_keys[key] & 0x80) ? false : true; }

  bool  Update();

  void  Clear() { ZeroMemory(m_keys, 256 * sizeof(char)); }

  bool  Acquire();
  bool  Unacquire();

private:
  LPDIRECTINPUTDEVICE7  m_pDIDev;
  char    m_keys[256];
};

class CMouse
{
public:
  CMouse(LPDIRECTINPUT7 pDI, HWND hwnd, bool isExclusive = true);
  ~CMouse();

  bool  MouseDown(int button) { return (m_state.rgbButtons[button] & 0x80) ? true : false; }
  bool  MouseUp(int button) { return (m_state.rgbButtons[button] & 0x80) ? false : true; }
  void  GetMovement(int &dx, int &dy) { dx = m_state.lX; dy = m_state.lY; }
  POINT GetMovement() 
  { 
	  POINT mouse;
	  mouse.x = m_state.lX;
	  mouse.y = m_state.lY;
	  return mouse;
  }

  bool  Update();

  bool  Acquire();
  bool  Unacquire();

private:
  LPDIRECTINPUTDEVICE7  m_pDIDev;
  DIMOUSESTATE          m_state;
};

class CJoystick
{
public:
  CJoystick(LPDIRECTINPUT7 pDI, HINSTANCE appInstance);
  ~CJoystick();

  bool  Update();

  bool  Acquire();
  bool  Unacquire();

private:
  LPDIRECTINPUTDEVICE7  m_pDIDev;
};

class CInputSystem
{
public:
	CInputSystem() { }
	~CInputSystem() { Shutdown(); }
  bool  Initialize(HWND hwnd, HINSTANCE appInstance, bool isExclusive, DWORD flags = 0);
  bool  Shutdown();

  void  AcquireAll();
  void  UnacquireAll();

  CKeyboard  *GetKeyboard() { return m_pKeyboard; }
  CMouse     *GetMouse()    { return m_pMouse; }
  CJoystick  *GetJoystick() { return m_pJoystick; }

  bool  Update();

  bool  KeyDown(char key) { return (m_pKeyboard && m_pKeyboard->KeyDown(key)); }
  bool  KeyUp(char key) { return (m_pKeyboard && m_pKeyboard->KeyUp(key)); }

  bool  MouseDown(int button) { return (m_pMouse && m_pMouse->MouseDown(button)); }
  bool  MouseUp(int button) { return (m_pMouse && m_pMouse->MouseUp(button)); }
  void  GetMouseMovement(int &dx, int &dy) { if (m_pMouse) m_pMouse->GetMovement(dx, dy); }
  POINT  GetMousePos() 
  { POINT p; p.x = m_pMouse->GetMovement().x; p.y = m_pMouse->GetMovement().y; return p;}

private:
  CKeyboard  *m_pKeyboard;
  CMouse     *m_pMouse;
  CJoystick  *m_pJoystick;
  
  LPDIRECTINPUT7 m_pDI;
};

#endif