#include "Export_Function.h"
#include "GolemFist.h"
#include "SunGollem.h"
#include "FistEffect.h"
#include "TrashBummer.h"
#include "Effect_StoneSpike.h"
#include "Effect_Smoke.h"
#include "Pool.h"
#include "Effect_Smoke.h"


CGolemFist::CGolemFist(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CBullet(pGraphicDev,OBJ_ID::MONSTER_SKILL)
	, m_fEffectTime(0.01f)
	, m_fAccEffect(0.0f)
{
}
CGolemFist::CGolemFist(const CGolemFist& rhs)
	: CBullet(rhs)
{

}

CGolemFist::~CGolemFist()
{
}

HRESULT CGolemFist::Ready_Object(void)
{
	//227 / 315
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"SunGolem_Idle_Fist", L"Proto_Texture_SunGolem_Idle_Fist", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_Fist", L"Proto_Texture_SunGolem_Dirty_Fist", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_BummerFist", L"Proto_Texture_SunGolem_Dirty_BummerFist", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_Fist", true);

	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.6f * 1.5f, 1.f * 2.f, 1.f * 1.5f });
	m_pMonsterAim = CMonsterAim::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pMonsterAim, E_FAIL);
	m_pMonsterAim->Set_Active(true);
	return S_OK;
}

_int CGolemFist::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	int iExit = __super::Update_Object(fTimeDelta);

	_vec3 vDir = { 0.f, -1.f, 0.f };

	Generate_MotionTrail(fTimeDelta);

	if (m_bDirty == true)
	{
		if (m_bBummer == true)
			m_pAnimator->Play_Animation(L"SunGolem_Dirty_BummerFist", true);
		else
			m_pAnimator->Play_Animation(L"SunGolem_Dirty_Fist", true);
	}


	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 20.f);	
	m_pMonsterAim->Update_Object(fTimeDelta);



	return iExit;
}

void CGolemFist::LateUpdate_Object(void)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pMonsterAim->Get_TransformCom()->Set_Pos(&_vec3{ vPos.x,0.01f,vPos.z });
	m_pMonsterAim->Set_Red(int(vPos.y/7.f*255.f));
	__super::LateUpdate_Object();
	if (vPos.y < -1.f)
	{
		if (Is_Active())
		{			
			if (m_bDirty == true)
			{
				if (m_bBummer == true && rand() % 99 < 33)
				{
					CTrashBummer* pTrashBummer = CTrashBummer::Create(m_pGraphicDev);
					NULL_CHECK_RETURN(pTrashBummer, );
					vPos.y = 0.5f;
					pTrashBummer->Get_TransformCom()->Set_Pos(&vPos);
					CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::MONSTER);
					pLayer->Add_GameObject(L"GolemFist", pTrashBummer);
				}
				else
				{
					CEffect_StoneSpike* pFistEffect = CEffect_StoneSpike::Create(m_pGraphicDev);
					NULL_CHECK_RETURN(pFistEffect, );
					vPos.y = -0.5f;
					pFistEffect->Get_TransformCom()->Set_Pos(&vPos);
					pFistEffect->Set_Atk(m_iAtk);
					pFistEffect->Set_Owner(m_pOwner);
					CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
					pLayer->Add_GameObject(L"GolemFist", pFistEffect);
			
					m_pMonsterAim->Set_Active(false);
				}
			}
			CFistEffect* pFistEffect = CFistEffect::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pFistEffect, );
			vPos.y = 0.009f;
			pFistEffect->Get_TransformCom()->Set_Pos(&vPos);
			CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
			pLayer->Add_GameObject(L"GolemFist", pFistEffect);
			Set_Active(false);
			m_pMonsterAim->Set_Active(false);
			vPos.y = 0.5f;
			for (int i = 0; i < 6; i++)
			{
				CGameObject* pSmoke = CPool<CEffect_Smoke>::Get_Obj();
				if (pSmoke)
					dynamic_cast<CEffect_Smoke*>(pSmoke)->Get_Effect(vPos, _vec3(1.f, 1.f, 1.f), 148, 150, 148);
				else
				{
					pSmoke = dynamic_cast<CEffect_Smoke*>(pSmoke)->Create(Engine::Get_Device());
					if (pSmoke)
						dynamic_cast<CEffect_Smoke*>(pSmoke)->Get_Effect(vPos, _vec3(1.f, 1.f, 1.f), 148, 150, 148);
				}
			}

			Stop_Sound(CHANNELID::SOUND_BOSS);
			Play_Sound(L"SFX_38_SunGolemFist.wav", CHANNELID::SOUND_BOSS, .5f);

		}
	}
	m_pMonsterAim->LateUpdate_Object();
}

void CGolemFist::Render_Object(void)
{
	if (Is_Active())
	{
		__super::Render_Object();

		LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

		CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
		if (pCamera == nullptr)
			return;

		_vec3 vPos;
		pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		D3DVECTOR vCamPos = vPos;

		D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, 1.f };

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



		m_pMonsterAim->Render_Object();
	}
	
}

HRESULT CGolemFist::Add_Component(void)
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

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}




void CGolemFist::Generate_MotionTrail(_float fTimeDelta)
{
	m_fAccEffect += fTimeDelta;
	if (m_fAccEffect >= m_fEffectTime)
	{
		CEffect_Smoke* pEffectSmoke = CPool<CEffect_Smoke>::Get_Obj();

		if (pEffectSmoke == nullptr)
			pEffectSmoke = CEffect_Smoke::Create(m_pGraphicDev);

		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);


		pEffectSmoke->Get_Effect(vPos, _vec3{ 1.f, 1.f, 1.f });
		// Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"EffectSmoke", pEffectSmoke);
	}
}

CGolemFist* CGolemFist::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemFist* pInstance = new CGolemFist(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemFist Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemFist::Free()
{
	__super::Free();
}
