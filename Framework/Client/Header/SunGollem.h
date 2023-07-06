#pragma once
#include "GameObject.h"
#include "GolemHead.h"

BEGIN(Engine)

	class CRcTex;
class CTransform;
class CCollider;

END

class CSunGollem : public Engine::CGameObject
{
public:
	enum BODYPARTS {
		LEFTARM0 = 0, LEFTARM1, LEFTARM2, RIGHTARM0,
		RIGHTARM1, RIGHTARM2, RIGHTHAND0, RIGHTHAND1, RIGHTHAND2,
		HEAD, LOWERJAW, UPPERJAW, LEFTLEG, RIGHTLEG, PARTSEND};
protected:
	explicit CSunGollem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSunGollem(const CSunGollem& rhs);
	virtual ~CSunGollem();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);
private:
	//vector<CGameObject> vecParts;

public:
	void Set_Speed(_float _fSpeed) { m_fSpeed = _fSpeed; }
	_float Get_Speed() { return m_fSpeed; }

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

	static CSunGollem* Create(LPDIRECT3DDEVICE9 pGraphicDev);


protected:

	CTransform* m_pTransformCom = nullptr;
	CCollider* m_pColliderCom = nullptr;

private:
	_float			m_fSpeed = 5.f;
	_float			m_fHealth = 5.f;
	SUNGOLEM_STATE	m_eState;
	_vec3 m_vLeftArmPos[3];
	_vec3 m_vRightArmPos[3];
	_vec3 m_vLeftHandPos[3];
	_vec3 m_vRightHandPos[3];
	_vec3 m_vHeadPos, m_vLowerJawPos, m_vUpperJawPos, m_vLeftLegPos, m_vRightLegPos;


protected:
	CGameObject* m_pTarget;
	_float m_fMoveTime;
protected:
	virtual void Free() override;

};