#include "Export_Function.h"
#include "GolemRightHand.h"
#include "SunGollem.h"

CGolemRightHand::CGolemRightHand(LPDIRECT3DDEVICE9 pGraphicDev) : CGolemPart(pGraphicDev)

{
}
CGolemRightHand::CGolemRightHand(const CGolemRightHand& rhs)
	: CGolemPart(rhs)
{

}

CGolemRightHand::~CGolemRightHand()
{

}

HRESULT CGolemRightHand::Ready_Object(void)
{

	//227 / 315
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"SunGolem_Idle_RightHand", L"Proto_Texture_SunGolem_Idle_RightHand", 0.2f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_RightHand", L"Proto_Texture_SunGolem_Dirty_RightHand", 0.2f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_BummerRightHand", L"Proto_Texture_SunGolem_Dirty_BummerRightHand", 0.2f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_RightHand", true);

	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({1.f,1.0f,1.f });

	Set_State(SUNGOLEM_STATE::REGEN);
	return S_OK;
}

_int CGolemRightHand::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	int iExit = __super::Update_Object(fTimeDelta);	

	switch (m_eState)
	{
	case SUNGOLEM_STATE::IDLE:
		Update_Idle(fTimeDelta);
		break;
	case SUNGOLEM_STATE::MOVE:
		Update_Move(fTimeDelta);
		Generate_MotionTrail(fTimeDelta);
		break;
	case SUNGOLEM_STATE::REGEN:
		Update_Regen(fTimeDelta);
		Generate_MotionTrail(fTimeDelta);
		break;
	case SUNGOLEM_STATE::ATTACK:
		Update_Attack(fTimeDelta);
		break;
	case SUNGOLEM_STATE::DIE:
		Update_Die(fTimeDelta);
		break;
	case SUNGOLEM_STATE::DIRTY:
		Update_Dirty(fTimeDelta);
		Generate_MotionTrail(fTimeDelta);
		break;
	}

	return iExit;
}

void CGolemRightHand::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemRightHand::Render_Object(void)
{

	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;

	D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, m_fAlpha / 255.0f };

	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 50.0f);


	IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);


	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

}

HRESULT CGolemRightHand::Add_Component(void)
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

void CGolemRightHand::Update_Idle(_float fTimeDelta)
{
	if (m_bExhale)
	{
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(2.1f, -1.7f, -0.012f), fTimeDelta, 3.f);
			if (m_fRotationAngle > 361.3)
				m_fRotationAngle -= 43.1f * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(2.8f, -0.2f, -0.011f), fTimeDelta, 3.f);
			if (m_fRotationAngle > 396.3)
				m_fRotationAngle -= 43.1f * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(2.49f, 1.f, -0.01f), fTimeDelta, 3.f);
			if (m_fRotationAngle > 402.2f)
				m_fRotationAngle -= 43.f * 2.f * fTimeDelta;
		}
	}
	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}

}

void CGolemRightHand::Update_Dirty(_float fTimeDelta)
{
	if (m_iArmNum != 1)
		m_pAnimator->Play_Animation(L"SunGolem_Dirty_RightHand", true);
	else 
	{
		m_pTransformCom->Set_Scale({ 0.7f * 2.5f, 0.7f * 2.5f,0.7f * 2.5f });
		m_pAnimator->Play_Animation(L"SunGolem_Dirty_BummerRightHand", true);
	}
	switch (m_iIndex)
	{
	case 0:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(1.439f, -1.65f, -0.013f), fTimeDelta, 2.f);
			m_fRotationAngle = 330.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(2.767f, -0.2f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 390.f)
				m_fRotationAngle += 60.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(2.9f, 2.1f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 437.f)
				m_fRotationAngle += 55.f * fTimeDelta;
		}
		break;
	case 1:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(1.21797f, -1.45396f, -0.013f), fTimeDelta, 2.f);

			if (m_fRotationAngle > 328.166f)
				m_fRotationAngle -= 60.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(2.0014f, -0.598564f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 347.836f)
				m_fRotationAngle -= 60.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(2.07986f, 0.758278f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 395.008f)
				m_fRotationAngle -= 55.f * fTimeDelta;
		}
		break;
	case 2:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(2.1f, -2.4f, -0.013f), fTimeDelta, 2.f);

			if (m_fRotationAngle < 356.1)
				m_fRotationAngle += 60.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(3.8f, 0.f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 360.f + 51.6f)
				m_fRotationAngle += 60.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(3.29f, 2.794f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 360.f + 90.f)
				m_fRotationAngle += 55.f * fTimeDelta;
		}
		break;

	default:
		break;
	}
}

void CGolemRightHand::Update_Move(_float fTimeDelta)
{
	if (m_bExhale)
	{
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(2.1f, -1.7f, -0.012f), fTimeDelta, 3.f);
			if (m_fRotationAngle > 361.3f)
				m_fRotationAngle -= 43.1f * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(2.8f, -0.2f, -0.011f), fTimeDelta, 3.f);
			if (m_fRotationAngle > 396.3f)
				m_fRotationAngle -= 43.1f * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(2.49f, 1.f, -0.01f), fTimeDelta, 3.f);
			if (m_fRotationAngle > 402.2f)
				m_fRotationAngle -= 43.f * 2.f * fTimeDelta;
		}
	}
	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}

}

void CGolemRightHand::Update_Attack(_float fTimeDelta)
{
	if (m_bExhale)
	{
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(2.1f, -1.7f, -0.012f), fTimeDelta,3.f);
			if (m_fRotationAngle > 361.3f)
				m_fRotationAngle -= 43.1f * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(2.8f, -0.2f, -0.011f), fTimeDelta, 3.f);
			if (m_fRotationAngle > 396.3f)
				m_fRotationAngle -= 43.1f * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(2.49f, 1.f, -0.01f), fTimeDelta, 3.f);
			if (m_fRotationAngle > 402.2f)
				m_fRotationAngle -= 43.f * 2.f * fTimeDelta;
		}
	}
	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}

}

void CGolemRightHand::Update_Die(_float fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	if (vPos.y > 0.5f)
		m_vOffset.y -= 5.f * fTimeDelta;
	else
		m_fAlpha -= 100.f * fTimeDelta;
	if (m_fAlpha <= 0.f)
		Set_Active(false);
}

void CGolemRightHand::Update_Regen(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		m_vOffset = { 0.9f,	 -1.2f,	0.009f };
		m_fRotationAngle = 329.f;
		break;
	case 1:
		Move_Offset(_vec3(0.9f, - 1.f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 2:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(1.439f, -1.65f, -0.013f), fTimeDelta, 2.f);
			m_fRotationAngle = 330.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(2.767f, -0.2f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 390.f)
				m_fRotationAngle += 60.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(2.9f, 2.1f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 437.f)
				m_fRotationAngle += 55.f * fTimeDelta;
		}
		break;	
	case 3:
			if (m_iArmNum == 0)
			{
				Move_Offset(_vec3(1.439f, -1.65f, -0.013f), fTimeDelta, 2.f);
				m_fRotationAngle = 330.f;
			}
			else if (m_iArmNum == 1)
			{
				Move_Offset(_vec3(2.767f, -0.2f, -0.012f), fTimeDelta, 2.f);
				if (m_fRotationAngle < 390.f)
					m_fRotationAngle += 60.f * fTimeDelta;
			}
			else if (m_iArmNum == 2)
			{
				Move_Offset(_vec3(2.9f, 2.1f, -0.011f), fTimeDelta, 2.f);
				if (m_fRotationAngle < 437.f)
					m_fRotationAngle += 55.f * fTimeDelta;
			}
			break;
	case 4:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(1.21797f, -1.45396f, -0.013f), fTimeDelta, 2.f);

			if (m_fRotationAngle > 328.166f)
				m_fRotationAngle -= 60.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(2.0014f, -0.598564f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 347.836f)
				m_fRotationAngle -= 60.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(2.07986f, 0.758278f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 395.008f)
				m_fRotationAngle -= 55.f * fTimeDelta;
		}
		break;
	case 5:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(2.1f, -2.4f, -0.013f), fTimeDelta, 2.f);

			if (m_fRotationAngle < 356.1)
				m_fRotationAngle += 60.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(3.8f, 0.f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 360.f+ 51.6f)
				m_fRotationAngle += 60.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(3.29f, 2.794f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 360.f + 90.f)
				m_fRotationAngle += 55.f * fTimeDelta;
		}
		break;

	default:
		break;
	}
}
CGolemRightHand* CGolemRightHand::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemRightHand* pInstance = new CGolemRightHand(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemRightHand Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemRightHand::Free()
{
	__super::Free();
}
