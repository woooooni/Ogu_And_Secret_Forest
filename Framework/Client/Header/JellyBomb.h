#pragma once
#include "FieldObject.h"
class CJellyBomb :    public CFieldObject
{
	CLONE(CJellyBomb)

protected:
	CJellyBomb(LPDIRECT3DDEVICE9 p_Dev);
	CJellyBomb(const CJellyBomb& rhs);
	virtual ~CJellyBomb();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CJellyBomb* Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum = 0, const _vec3& p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

public:
	virtual void		Set_SubscribeEvent(_uint pEvent) override;

	void	Reset();
	const	_uint& Get_EventNum() { return m_iExplodeEvent; }


protected:
	_bool		m_bHitted;
	_uint		m_iExplodeEvent;
	_float		m_fExplodeTime;
	_bool		m_bExplosing;
	CTexture* m_pBlurTex;
	_float		m_fBlurAlpha;

};

