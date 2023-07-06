#pragma once
#include "GameObject.h"


BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CGolemLeftArm : public Engine::CGameObject
{
protected:
	explicit CGolemLeftArm(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGolemLeftArm(const CGolemLeftArm& rhs);
	virtual ~CGolemLeftArm();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);


public:

	SUNGOLEM_STATE Get_State() { return m_eState; }
	void Set_State(SUNGOLEM_STATE _eState) { if (m_eState == _eState) return; m_eState = _eState; }

public:
	void Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }
public:
	virtual void Update_Idle(_float fTimeDelta)	;
	virtual void Update_Dirty(_float fTimeDelta);
	virtual void Update_Move(_float fTimeDelta)	;
	virtual void Update_Attack(_float fTimeDelta);
	virtual void Update_Die(_float fTimeDelta)	;
	virtual void Update_Regen(_float fTimeDelta);

	static CGolemLeftArm* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void Set_ArmNum(_int _iNum) { m_iArmNum = _iNum; }
private:
	SUNGOLEM_STATE	m_eState;
	bool m_bBreath = false;
	_int m_iArmNum = 0;
protected:
	CGameObject* m_pTarget;
	_float m_fMoveTime;
protected:
	virtual void Free() override;

};