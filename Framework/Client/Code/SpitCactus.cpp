#include "SpitCactus.h"
#include "Export_Function.h"
#include "CactusNeedle.h"
#include "Pool.h"
#include "Effect_Hit.h"
#include "Player.h"

CSpitCactus::CSpitCactus(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev, OBJ_ID::SPIT_CACTUS)
{
}

CSpitCactus::CSpitCactus(const CSpitCactus& rhs)
	: CMonster(rhs)
{
}

CSpitCactus::~CSpitCactus()
{
}

HRESULT CSpitCactus::Ready_Object(void)
{
	Set_State(MONSTER_STATE::REGEN);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SpitCactus_Idle_Down", L"Proto_Texture_SpitCactus_Idle_Down", 0.1f);
	m_pAnimator->Add_Animation(L"SpitCactus_Attack_Down", L"Proto_Texture_SpitCactus_Attack_Down", 0.1f);
	m_pAnimator->Add_Animation(L"SpitCactus_Regen_Down", L"Proto_Texture_SpitCactus_Regen_Down", 0.1f);
	m_pAnimator->Add_Animation(L"SpitCactus_Death_Down", L"Proto_Texture_SpitCactus_Death_Down", 0.1f);

	m_fMinHeight = 0.5f;
	m_pTransformCom->Set_Pos(&_vec3(1.0f, 1.0f, 9.0f));
	m_pTransformCom->Set_Scale(_vec3(1.f, 1.f, 1.f));

	m_pAnimator->Play_Animation(L"SpitCactus_Regen_Down", true);
	m_tStat = { 3,3,1 };

	m_bPushable = false;

	m_pUIBack = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_BACK);
	if (m_pUIBack != nullptr)
		m_pUIBack->Set_Owner(this);

	m_pUIGauge = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_GAUGE);
	if (m_pUIGauge != nullptr)
		m_pUIGauge->Set_Owner(this);

	m_pUIFrame = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_FRAME);
	if (m_pUIFrame != nullptr)
		m_pUIFrame->Set_Owner(this);

	return S_OK;
}

_int CSpitCactus::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;
	if (m_tStat.iHp < 1.f || m_tStat.iMaxHp < m_tStat.iHp)
		Set_State(MONSTER_STATE::DIE);
	_int iExit = __super::Update_Object(fTimeDelta);
	if (Get_State() != MONSTER_STATE::DIE)
		Engine::Add_CollisionGroup(m_pColliderCom, COLLIDE_STATE::COLLIDE_MONSTER);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	vPos.y += 0.6f;
	vPos.z -= 0.01f;

	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->Update_Object(fTimeDelta);
		m_pUIBack->Get_TransformCom()->Set_Pos(&vPos);

		vPos.z -= 0.005f;
		m_pUIGauge->Update_Object(fTimeDelta);

		if (m_tStat.iHp == m_tStat.iMaxHp)
			m_pUIGauge->Get_TransformCom()->Set_Pos(&vPos);
		else if (m_tStat.iHp > 0 && m_tStat.iHp < m_tStat.iMaxHp)
		{
			_vec3 vMovePos = vPos;

			_float fMaxHP = _float(m_tStat.iMaxHp);
			_float fCurHP = _float(m_tStat.iHp);
			_float fHP = fCurHP / fMaxHP;

			_float fOriginWidth = _float(m_pUIGauge->Get_TextureCom()->Get_TextureDesc(0).Width);
			_float fWidth = fOriginWidth - fOriginWidth * fHP;

			_float fIndex = fWidth * 0.004f * 0.5f;

			vMovePos = _vec3((vMovePos.x - fIndex), vMovePos.y, vMovePos.z);
			m_pUIGauge->Get_TransformCom()->Set_Pos(&vMovePos);
		}

		vPos.z -= 0.005f;
		m_pUIFrame->Update_Object(fTimeDelta);
		m_pUIFrame->Get_TransformCom()->Set_Pos(&vPos);
	}

	return iExit;
}
void CSpitCactus::LateUpdate_Object(void)
{
	if (!Is_Active())
		return ;

	__super::LateUpdate_Object();

	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->LateUpdate_Object();
		m_pUIGauge->LateUpdate_Object();
		m_pUIFrame->LateUpdate_Object();
	}
}

void CSpitCactus::Render_Object(void)
{
	if (!Is_Active())
		return ;
	
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



	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->Render_Object();
		m_pUIGauge->Render_Object();
		m_pUIFrame->Render_Object();
	}
}


void CSpitCactus::Update_Idle(_float fTimeDelta)
{
	if (rand() % 10 > 8)
	{
		Set_State(MONSTER_STATE::ATTACK);
		m_pAnimator->Play_Animation(L"SpitCactus_Attack_Down", false);
	}
}
void CSpitCactus::Update_Regen(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		Set_State(MONSTER_STATE::IDLE);
		m_pAnimator->Play_Animation(L"SpitCactus_Idle_Down", true);
	}
}


void CSpitCactus::Update_Attack(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 3&&m_bShoot)
	{
		for (_int i = 0; i < 8; i++)
		{
			CCactusNeedle* pCactusNeedle = CCactusNeedle::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pCactusNeedle, );
			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			pCactusNeedle->Get_TransformCom()->Set_Pos(&vPos);
			_vec3 AxisY = { 0,1,0 };
			pCactusNeedle->Get_TransformCom()->RotationAxis(AxisY, D3DXToRadian(45.f*(float)i));
			pCactusNeedle->Set_Poison(m_bPoison);
			pCactusNeedle->Set_Owner(this);
			CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::MONSTER);
			pLayer->Add_GameObject(L"CactusNeedle", pCactusNeedle);
			m_bShoot = false;
		}

		_vec3 vDir, vPos, vPlayerPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player"));
		pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);

		vDir = vPlayerPos - vPos;

		if (D3DXVec3Length(&vDir) <= 10.f)
		{
			int iSound = rand() % 5 + (_uint)CHANNELID::SOUND_EFFECT_MONSTER;
			Stop_Sound((CHANNELID)iSound);
			Play_Sound(L"SFX_85_MonsterSpitCactus_Shoot.wav", (CHANNELID)iSound, 0.8f);
		}
	}

	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		Set_State(MONSTER_STATE::IDLE);
		m_pAnimator->Play_Animation(L"SpitCactus_Idle_Down", true);
		m_bShoot = true;
	}
}



void CSpitCactus::Update_Move(_float fTimeDelta)
{
}
void CSpitCactus::Update_Die(_float fTimeDelta)
{
	if (Is_Active() && m_pAnimator->GetCurrAnimation()->Is_Finished()) {
		On_Death();
		Set_Active(false);

		int iSound = rand() % 5 + (_uint)CHANNELID::SOUND_EFFECT_MONSTER;
		Stop_Sound((CHANNELID)iSound);
		Play_Sound(L"SFX_87_MonsterSpitCactus_Death.wav", (CHANNELID)iSound, 0.5f);
	}
}


HRESULT CSpitCactus::Add_Component(void)
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

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;

}
CSpitCactus* CSpitCactus::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpitCactus* pInstance = new CSpitCactus(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SpitCactus Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSpitCactus::Trace(_float fTimeDelta)
{
}

void CSpitCactus::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (Get_State() == MONSTER_STATE::DIE)
		return;
	__super::Collision_Enter(pCollider, _eCollisionGroup, _iColliderID);

	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
	{
		_vec3 vTargetPos;
		_vec3 vPos;
		_vec3 vDir;
		_vec3 vEffectPos;

		pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vDir = vPos - vTargetPos;

		vEffectPos = vDir;
		D3DXVec3Normalize(&vEffectPos, &vEffectPos);
		vEffectPos *= 0.5f;
		vEffectPos += vPos;
		vEffectPos.z = vPos.z - 0.05f;

		CGameObject* pEffect = CPool<CEffect_Hit>::Get_Obj();
		if (!pEffect)
		{
			pEffect = CEffect_Hit::Create(m_pGraphicDev);
			pEffect->Ready_Object();
		}
		dynamic_cast<CEffect_Hit*>(pEffect)->Get_Effect(vEffectPos, _vec3(2.0f, 2.0f, 2.0f));

		vDir.y = 0.0f;
		D3DXVec3Normalize(&vDir, &vDir);
		m_tStat.iHp -= 1;
		if (m_tStat.iHp < 1)
		{
			Set_State(MONSTER_STATE::DIE);
			m_pAnimator->Play_Animation(L"SpitCactus_Death_Down", true);
		}

		int iSound = rand() % 5 + (_uint)CHANNELID::SOUND_EFFECT_MONSTER;
		Stop_Sound((CHANNELID)iSound);
		Play_Sound(L"SFX_86_MonsterSpitCactus_Hit.wav", (CHANNELID)iSound, 0.5f);
	}
}
void CSpitCactus::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	__super::Collision_Stay(pCollider, _eCollisionGroup, _iColliderID);

}