#include "Export_Function.h"
#include "GolemFace.h"
#include "SunGollem.h"

CGolemFace::CGolemFace(LPDIRECT3DDEVICE9 pGraphicDev) : CGolemPart(pGraphicDev)

{
}
CGolemFace::CGolemFace(const CGolemFace& rhs)
	: CGolemPart(rhs)

{

}

CGolemFace::~CGolemFace()
{
}

HRESULT CGolemFace::Ready_Object(void)
{
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_FaceChange", L"Proto_Texture_SunGolem_Dirty_FaceChange", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_Face", L"Proto_Texture_SunGolem_Dirty_Face", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_FaceDeath", L"Proto_Texture_SunGolem_Dirty_FaceDeath", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_FaceChange", false);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 3.f,4.f,3.f });
	Set_Active(false);
	Set_State(SUNGOLEM_STATE::DIRTY);

	return S_OK;
}

_int CGolemFace::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	switch (Get_State())
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

void CGolemFace::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemFace::Render_Object(void)
{
	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;


	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetFloat("g_AlphaRef", 0.0f);


	IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

	MATERIAL.material.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	MATERIAL.material.Diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	MATERIAL.material.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	MATERIAL.material.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	MATERIAL.material.Power = 0.0f;

	pEffect->SetValue("g_Material", &MATERIAL.material, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	
}

HRESULT CGolemFace::Add_Component(void)
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

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}

void CGolemFace::Update_Idle(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_Face", true);
}

void CGolemFace::Update_Dirty(_float fTimeDelta)
{
	///if(m)
}

void CGolemFace::Update_Move(_float fTimeDelta)
{
}

void CGolemFace::Update_Attack(_float fTimeDelta)
{
}

void CGolemFace::Update_Die(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_FaceDeath", false);
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
		Set_Active(false);
}

void CGolemFace::Update_Regen(_float fTimeDelta)
{
}
CGolemFace* CGolemFace::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemFace* pInstance = new CGolemFace(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemFace Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemFace::Free()
{
	__super::Free();
}
