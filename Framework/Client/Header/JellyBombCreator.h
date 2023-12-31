#pragma once
#include "FieldObject.h"
#include	"JellyBomb.h"
class CJellyBombCreator :  public CFieldObject
{
	CLONE(CJellyBombCreator)

protected:
	CJellyBombCreator(LPDIRECT3DDEVICE9 p_Dev);
	CJellyBombCreator(const CJellyBombCreator& rhs);
	virtual ~CJellyBombCreator();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CJellyBombCreator* Create(LPDIRECT3DDEVICE9 p_Dev,  CJellyBomb* pBomb ,const _uint& p_EventNum = 0, const _vec3& p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

public:
	virtual void		Set_SubscribeEvent(_uint pEvent) override;

protected:
	CJellyBomb* m_pJellyBomb;
	_bool		m_bIsCreate;

};

