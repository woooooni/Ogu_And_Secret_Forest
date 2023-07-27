#include "Coin.h"
#include "Export_Function.h"
#include	"Player.h"
CCoin::CCoin(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev, ITEM_TYPE::COIN, OBJ_ID::ITEM)
{
}

CCoin::CCoin(const CCoin& rhs): CItem(rhs)
{
}

CCoin::~CCoin()
{
}

HRESULT CCoin::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(&_vec3(5.0f, 1.0f, 3.0f));
	m_pTransformCom->Set_Scale({ 0.5f, 0.5f, 0.5f });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 0.5f, 0.5f, 0.5f });
	m_pAnimator->Add_Animation(L"Coin_Anime", L"Proto_Texture_Coin", 0.1f);
	m_pAnimator->Play_Animation(L"Coin_Anime", true);
	return S_OK;
}

_int CCoin::Update_Object(const _float& fTimeDelta)
{
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_ITEM);
	Add_RenderGroup(RENDER_ALPHA, this);
	_int Result = __super::Update_Object(fTimeDelta);
	return Result;
}

void CCoin::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CCoin::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
}
void CCoin::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER)
	{
		CPlayer* player = dynamic_cast<CPlayer*>(pCollider->GetOwner());
		NULL_CHECK_RETURN(player);
		player->Add_Money(m_iMoney);
		Set_Active(false);
	}
}
void CCoin::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}
CCoin* CCoin::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& pMoney, const _vec3& pPos)
{
	CCoin* pInstance = new CCoin(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Coin Create Failed");
		return nullptr;
	}

	pInstance->m_iMoney = pMoney;
	_vec3 src = pPos;
	src.y = 0.5f;
	pInstance->m_pTransformCom->Set_Pos(&src);

	return pInstance;
}
HRESULT CCoin::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	return S_OK;
}

void CCoin::Free()
{
	__super::Free();
}
