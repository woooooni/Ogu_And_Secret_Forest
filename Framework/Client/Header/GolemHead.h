#pragma once
#include "GolemPart.h"
#include "Effect_EyeTrail.h"


BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CGolemHead : public CGolemPart
{
	CLONE(CGolemHead)

protected:
	explicit CGolemHead(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGolemHead(const CGolemHead& rhs);
	virtual ~CGolemHead();

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
	void Set_Target(CGolemPart* _pTarget) { m_pTarget = _pTarget; }
public:
	virtual void Update_Idle(_float fTimeDelta)	;
	virtual void Update_Dirty(_float fTimeDelta);
	virtual void Update_Move(_float fTimeDelta)	;
	virtual void Update_Attack(_float fTimeDelta);
	virtual void Update_Die(_float fTimeDelta)	;
	virtual void Update_Regen(_float fTimeDelta);

	static CGolemHead* Create(LPDIRECT3DDEVICE9 pGraphicDev);



	void		Set_FirstEye();
	void		End_FirstEye();
	void		Set_SecondEye();
	void		End_SecondEye();
private:

protected:
	_float m_fMoveTime;
protected:
	virtual void Free() override;
	

private:
	CEffect_EyeTrail* m_pLeftEye = nullptr;
	CEffect_EyeTrail* m_pRightEye = nullptr;
};