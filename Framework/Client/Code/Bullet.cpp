
#include "..\Header\Bullet.h"
#include "Export_Function.h"
#include "TriCol.h"
#include "Transform.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_BULLET, _eID), m_iAtk(0), m_pOwner( nullptr)
{
}

CBullet::CBullet(const CBullet& rhs)
	: CGameObject(rhs),
	m_pOwner(rhs.m_pOwner), m_iAtk(rhs.m_iAtk)
{
}

CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_Object(void)
{
	return S_OK;
}

_int CBullet::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BULLET);
	return __super::Update_Object(fTimeDelta);
}

void CBullet::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBullet::Render_Object(void)
{
	__super::Render_Object();
}


void CBullet::Free()
{
	__super::Free();
}
