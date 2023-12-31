#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	//CComponent*			Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);

public:
	HRESULT			Add_GameObject(const wstring& _strObjName, CGameObject* pGameObject);
	CGameObject*	Find_GameObject(const wstring& _strObjName);
	vector<CGameObject*>& Get_GameObjectVec() { return m_vecObject; }

public:
	HRESULT	Ready_Layer();
	_int Update_Layer(const _float& fTimeDelta);
	void LateUpdate_Layer();

private:
	vector<CGameObject*> m_vecObject;
	vector<CGameObject*> m_vecReserveObj;

public:
	static		CLayer*		Create();
	virtual		void		Free() override;
};

END