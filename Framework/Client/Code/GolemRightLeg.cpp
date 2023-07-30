#include "Export_Function.h"
#include "GolemRightLeg.h"
#include "SunGollem.h"

CGolemRightLeg::CGolemRightLeg(LPDIRECT3DDEVICE9 pGraphicDev) : CGolemPart(pGraphicDev)

{
}
CGolemRightLeg::CGolemRightLeg(const CGolemRightLeg& rhs)
	: CGolemPart(rhs)
{

}

CGolemRightLeg::~CGolemRightLeg()
{
}

HRESULT CGolemRightLeg::Ready_Object(void)
{
	//227 / 315

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_RightLeg", L"Proto_Texture_SunGolem_Idle_RightLeg", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_RightLeg", L"Proto_Texture_SunGolem_Dirty_RightLeg", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_RightLeg", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));

	m_pTransformCom->Set_Scale({ 0.227f*3.f*1.5f, 0.315f*3.f * 2.f,1.f * 1.5f });

	Set_State(SUNGOLEM_STATE::REGEN);
	return S_OK;
}

_int CGolemRightLeg::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	

	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	switch (m_eState)
	{
	case SUNGOLEM_STATE::IDLE:
		Update_Idle(fTimeDelta);
		break;
	case SUNGOLEM_STATE::MOVE:
		Update_Move(fTimeDelta);
		break;
	case SUNGOLEM_STATE::REGEN:
		Update_Regen(fTimeDelta);
		break;
	case SUNGOLEM_STATE::ATTACK:
		Update_Attack(fTimeDelta);
		break;
	case SUNGOLEM_STATE::DIE:
		Update_Die(fTimeDelta);
		break;
	case SUNGOLEM_STATE::DIRTY:
		Update_Dirty(fTimeDelta);
		break;
	}

	return iExit;
}

void CGolemRightLeg::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemRightLeg::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
}

HRESULT CGolemRightLeg::Add_Component(void)
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
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);
	return S_OK;
}

void CGolemRightLeg::Update_Idle(_float fTimeDelta)
{

	
	if (m_bExhale)
		Move_Offset(_vec3(0.9f, -1.8f, m_vOffset.z), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}

}

void CGolemRightLeg::Update_Dirty(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_RightLeg", true);
	switch (m_iIndex)
	{
	case 0:
		Move_Offset(_vec3(1.f, -2.2f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 1:
		Move_Offset(_vec3(0.983166f, -2.0f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 2:
		Move_Offset(_vec3(1.f, -1.8f, m_vOffset.z), fTimeDelta, 2.f);
		break;


	default:
		break;
	}
}

void CGolemRightLeg::Update_Move(_float fTimeDelta)
{
	if (m_bExhale)
		Move_Offset(_vec3(0.9f, -1.8f, m_vOffset.z), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemRightLeg::Update_Attack(_float fTimeDelta)
{
	if (m_bExhale)
		Move_Offset(_vec3(0.9f, -1.8f, m_vOffset.z), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemRightLeg::Update_Die(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		Move_Offset(_vec3(0.9f, -1.8f, m_vOffset.z), fTimeDelta, 3.f);
		break;
	case 1:
		Move_Offset(_vec3(0.9f, -1.7f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 2:
		Move_Offset(_vec3(0.9f, -1.6f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 3:
		Move_Offset(_vec3(1.f, -1.5f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 4:
		Move_Offset(_vec3(1.5, -1.f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 5:
		Move_Offset(_vec3(-2.0f, -0.f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	default:
		break;
	}
}

void CGolemRightLeg::Update_Regen(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		m_vOffset.y = -1.5f;
		break;
	case 1:
		Move_Offset(_vec3(0.9f, -2.f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 2:
		Move_Offset(_vec3(0.9f, -1.7f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 3:
		Move_Offset(_vec3(1.f, -2.2f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 4:
		Move_Offset(_vec3(0.983166f, -2.0f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 5:
		Move_Offset(_vec3(1.f, -1.8f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	default:
		break;
	}

}
CGolemRightLeg* CGolemRightLeg::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemRightLeg* pInstance = new CGolemRightLeg(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemRightLeg Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemRightLeg::Free()
{
	__super::Free();
}
