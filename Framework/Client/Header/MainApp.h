#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;

END
class CMainApp : public CBase 
{
private:		// 持失切, 社瑚切
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT		Ready_MainApp(void);
	int			Update_MainApp(const float& fTimeDelta);
	void		LateUpdate_MainApp();
	void		Render_MainApp();

private:
	HRESULT	SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT	Ready_Default_SamplerState();
	HRESULT	Ready_Default_RenderState();
	HRESULT	Ready_Proto_Component(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_Manager(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_Proto_Object(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT	Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagementClass);
	HRESULT	Ready_Proto_Event();
	HRESULT Ready_Pool();


private:
	CGraphicDev*			m_pGraphicDevClass;
	CManagement*			m_pManagementClass;

	LPDIRECT3DDEVICE9		m_pGraphicDev;

#ifdef _DEBUG
private:
	_float			m_fTimeAcc = 0.f;
	_tchar			m_szFPS[MAX_PATH] = TEXT("");
	_uint			m_iNumDraw = 0;
#endif


public:
	static CMainApp*		Create();


private:
	virtual void		Free();

};

