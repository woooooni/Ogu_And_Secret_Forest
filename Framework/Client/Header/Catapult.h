#pragma once
#include "FieldObject.h"

BEGIN(Engine)

class CCubeTex;

END

class CCatapult :    public CFieldObject
{
	CLONE(CCatapult)

protected:
	CCatapult(LPDIRECT3DDEVICE9 p_Dev);
	CCatapult(const CCatapult& rhs);
	virtual ~CCatapult();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CCatapult* Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum = 0, const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

public:
	virtual void		Set_SubscribeEvent(_uint pEvent) override;

protected:
	CGameObject* m_pThrowingStone;
	_vec3 m_vStonePos;
	CTexture* m_pRevTexture;
	_vec3 m_vCenterPos;
	_float m_fThrowAngle;
	_bool m_bIsThrowing;

	CCubeTex* m_pBodyBufferCom;
	CTexture* m_pCubeTex;

	void			Throw_Stone();

};

