#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "InputDevice.h"
#include "FontMgr.h"

BEGIN(Engine)

// GraphicDev
//////////////
inline LPD3DXFONT	Get_Font();
//////////////
inline HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev);
inline void			Render_Begin(D3DXCOLOR Color);
inline void			Render_End();
inline LPDIRECT3DDEVICE9 Get_Device();

// TimerMgr
inline _float		Get_TimeDelta(const _tchar* pTimerTag);
inline void			Set_TimeDelta(const _tchar* pTimerTag);
inline	HRESULT		Ready_Timer(const _tchar* pTimerTag);
inline	CTimer* Find_Timer(const _tchar* pTimerTag);

// FrameMgr
inline _bool	Ispermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
inline HRESULT Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);

//InputDevice
inline _byte	Get_DIKeyState(_ubyte byKeyID);
inline _byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
inline _long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
inline HRESULT	Ready_InputDev(HINSTANCE hInst, HWND hWnd);
inline void		Update_InputDev(void);

// FontMgr
inline HRESULT Ready_Font(LPDIRECT3DDEVICE9 pDevice);
inline const LPD3DXFONT& Get_Font(FONT_TYPE _eType);


// Release
inline void			Release_System();

#include "Export_System.inl"

END

#endif // Export_System_h__


