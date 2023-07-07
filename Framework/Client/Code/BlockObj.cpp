#include "../Include/stdafx.h"
#include "BlockObj.h"
#include "Export_Function.h"


CBlockObj::CBlockObj(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev), m_iEventNum(0), m_bIsBlocking(false), m_iFollowingEvent(0)
{
}

CBlockObj::CBlockObj(const CBlockObj& rhs) : CFieldObject(rhs), m_iEventNum(rhs.m_iEventNum), m_bIsBlocking(rhs.m_bIsBlocking), m_iFollowingEvent(rhs.m_iFollowingEvent)
{
}

CBlockObj::~CBlockObj()
{
}

HRESULT CBlockObj::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Idle", L"Proto_Tex_Block_Idle", 0.1f);
	m_pAnimator->Add_Animation(L"Block_Idle", L"Proto_Tex_Block_Block_Idle", 0.1f);
	m_pAnimator->Add_Animation(L"UnBlocking", L"Proto_Tex_Block_UnBlocking", 0.1f);
	m_pAnimator->Add_Animation(L"Blocking", L"Proto_Tex_Block_Blocking", 0.1f);


	return S_OK;
}

_int CBlockObj::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	if(m_bIsBlocking)
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);
	return __super::Update_Object(fTimeDelta);
}

void CBlockObj::LateUpdate_Object(void)
{
	if (Check_Event_Start(m_iFollowingEvent) == S_OK)
	{
	}
}

void CBlockObj::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();
}

void CBlockObj::Free()
{
	__super::Free();
}

CBlockObj* CBlockObj::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos, _bool p_isFirstBlock)
{
	CBlockObj* ret = new CBlockObj(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("BlockObjCreate Failed");
		return nullptr;
	}

	ret->Set_SubscribeEvent(p_EventNum);
	ret->m_iFollowingEvent = p_EventNum;
	ret->m_pTransformCom->Set_Pos(&p_Pos);
	if (p_isFirstBlock)
		ret->m_pAnimator->Play_Animation(L"Block_Idle", false);
	else
		ret->m_pAnimator->Play_Animation(L"Idle", false);

	ret->m_bIsBlocking = p_isFirstBlock;

	return ret;
}

void CBlockObj::Collision_Enter(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

void CBlockObj::Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

void CBlockObj::Collision_Exit(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

HRESULT CBlockObj::Ready_Component()
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
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	return S_OK;
}

void CBlockObj::Event_Start(_uint iEventNum)
{
	Change_State();
}

void CBlockObj::Event_End(_uint iEventNum)
{
}

void CBlockObj::Set_SubscribeEvent(_uint pEvent)
{
	m_iFollowingEvent = pEvent;
	Add_Subscribe(pEvent, this);
}

void CBlockObj::Change_State()
{
	_uint MaxFrame = m_pAnimator->GetCurrAnimation()->Get_Size();
	_uint Frame = m_pAnimator->GetCurrAnimation()->Get_Idx();

	if (m_bIsBlocking)
	{
		m_pAnimator->Play_Animation(L"UnBlocking", false);
	}
	else
	{
		m_pAnimator->Play_Animation(L"Blocking", false);
	}

	if (MaxFrame != 1)
	{
		m_pAnimator->GetCurrAnimation()->Set_Idx(MaxFrame - 1 - Frame);
	}

	m_bIsBlocking = !m_bIsBlocking;
}
