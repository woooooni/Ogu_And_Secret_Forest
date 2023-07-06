#pragma once
#include "FieldObject.h"
class CBalpanObj : public CFieldObject
{
protected:
	CBalpanObj(LPDIRECT3DDEVICE9 p_Dev);
	CBalpanObj(const CBalpanObj& rhs);
	virtual ~CBalpanObj();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CBalpanObj* Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum,const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CGameObject* pCollisionObj, UINT _iColliderID)override;
	virtual void Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID) override;
	virtual void Collision_Exit(CGameObject* pCollisionObj, UINT _iColliderID) override;


public:
	void		Reset_Pushed();
	void		Set_AutoReset();
	void		Set_TargName(const wstring& pName)
	{
		m_wstrTargName = pName;
	}
	void		Set_EventNum(const _uint& pNum) { m_iPushedEventNum = pNum; }
	void		Add_ActivateState(const _uint& pNum);

protected:
	virtual HRESULT		Ready_Component();
	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;


	_uint		m_iPushedEventNum;
	list<_uint>	m_listActivateNum;
	bool		m_bIsPushed = false;
	bool		m_bIsAutoReset = false;
	wstring		m_wstrTargName = L"Player";

};
