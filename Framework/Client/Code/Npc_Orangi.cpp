#include "../Include/stdafx.h"
#include "Npc_Orangi.h"
#include "Export_Function.h"

CNpc_Orangi::CNpc_Orangi(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev, NPC_CODE::NPC_ORANGI, L"������")
{
}

CNpc_Orangi::CNpc_Orangi(const CNpc_Orangi& rhs)
	: CNpc(rhs)
{
}

CNpc_Orangi::~CNpc_Orangi()
{
}

HRESULT CNpc_Orangi::Ready_Object(void)
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

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);


	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Monkey_Orangi_Idle", L"Proto_Texture_NPC_Wholesaler_Orangi_Idle", 0.5f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Monkey_Orangi_React", L"Proto_Texture_NPC_Wholesaler_Orangi_React", 0.5f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Monkey_Orangi_Idle", TRUE), E_FAIL);
	m_pTransformCom->Set_Scale(_vec3(1.5f, 1.5f, 1.5f));
	return S_OK;
}

_int CNpc_Orangi::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNpc_Orangi::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNpc_Orangi::Render_Object(void)
{

	m_pAnimator->Render_Component();

	__super::Render_Object();
}

CNpc_Orangi* CNpc_Orangi::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_Orangi* pInstance = new CNpc_Orangi(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC Saler Orangi Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_Orangi::Free()
{
}

