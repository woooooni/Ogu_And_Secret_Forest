
#include "TrashBummer.h"
#include "SludgeBall.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Pool.h"
#include "Effect_Hit.h"

CTrashBummer::CTrashBummer(LPDIRECT3DDEVICE9 pGraphicDev) :CMonster(pGraphicDev, OBJ_ID::TRASH_BUMMER)
{
}

CTrashBummer::CTrashBummer(const CTrashBummer& rhs) : CMonster(rhs)
{
}

CTrashBummer::~CTrashBummer()
{
}



HRESULT CTrashBummer::Ready_Object(void)
{
	m_fMoveTime = 0.f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_Down", L"Proto_Texture_TrashBummer_Idle_Down", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_Up", L"Proto_Texture_TrashBummer_Idle_Up", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_Left", L"Proto_Texture_TrashBummer_Idle_Left", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_Right", L"Proto_Texture_TrashBummer_Idle_Right", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_RightDown", L"Proto_Texture_TrashBummer_Idle_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_RightUp", L"Proto_Texture_TrashBummer_Idle_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_LeftDown", L"Proto_Texture_TrashBummer_Idle_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_LeftUp", L"Proto_Texture_TrashBummer_Move_LeftUp", 0.1f);
	
	m_pAnimator->Add_Animation(L"TrashBummer_Move_Down", L"Proto_Texture_TrashBummer_Move_Down", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_Up", L"Proto_Texture_TrashBummer_Move_Up", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_Left", L"Proto_Texture_TrashBummer_Move_Left", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_Right", L"Proto_Texture_TrashBummer_Move_Right", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_RightDown", L"Proto_Texture_TrashBummer_Move_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_RightUp", L"Proto_Texture_TrashBummer_Move_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_LeftDown", L"Proto_Texture_TrashBummer_Move_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_LeftUp", L"Proto_Texture_TrashBummer_Move_LeftUp", 0.1f);

	m_pAnimator->Add_Animation(L"TrashBummer_Attack_Down", L"Proto_Texture_TrashBummer_Attack_Down", 0.3f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_Up", L"Proto_Texture_TrashBummer_Attack_Up", 0.3f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_Left", L"Proto_Texture_TrashBummer_Attack_Left", 0.3f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_Right", L"Proto_Texture_TrashBummer_Attack_Right", 0.3f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_RightDown", L"Proto_Texture_TrashBummer_Attack_RightDown", 0.3f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_RightUp", L"Proto_Texture_TrashBummer_Attack_RightUp", 0.3f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_LeftDown", L"Proto_Texture_TrashBummer_Attack_LeftDown", 0.3f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_LeftUp", L"Proto_Texture_TrashBummer_Attack_LeftUp", 0.3f);

	m_pAnimator->Add_Animation(L"TrashBummer_Regen_Down", L"Proto_Texture_TrashBummer_Regen_Down", 0.4f);

	m_pTransformCom->Set_Info(INFO_POS, &_vec3(4.0f, .5f, 4.0f));
	Set_Speed(4.f);
	Set_State(MONSTER_STATE::REGEN);
	m_pAnimator->Play_Animation(L"TrashBummer_Regen_Down", true);
	m_tStat = {2,2,1 };
	m_fMinHeight = 0.5f;
	m_pTransformCom->Set_Scale(_vec3(1.f, 1.f, 1.f));

	m_pUIBack = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_BACK);
	if (m_pUIBack != nullptr)
		m_pUIBack->Set_Owner(this);

	m_pUIGauge = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_GAUGE);
	if (m_pUIGauge != nullptr)
		m_pUIGauge->Set_Owner(this);

	m_pUIFrame = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_FRAME);
	if (m_pUIFrame != nullptr)
		m_pUIFrame->Set_Owner(this);

	int iSound = rand() % 5 + 4;
	Stop_Sound((CHANNELID)iSound);
	Play_Sound(L"SFX_336_MonsterGarbageBummer_In.wav", (CHANNELID)iSound, 0.5f);

	return S_OK;
}

_int CTrashBummer::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;
	if (m_tStat.iHp < 1.f || m_tStat.iMaxHp < m_tStat.iHp)
		Set_State(MONSTER_STATE::DIE);
	_int iExit = __super::Update_Object(fTimeDelta);
	_vec3  vPos;
	if (Get_State() != MONSTER_STATE::DIE)
		Engine::Add_CollisionGroup(m_pColliderCom, COLLIDE_STATE::COLLIDE_MONSTER);

	m_pTransformCom->Get_Info(INFO_POS, &vPos);

   	if (Get_State() != MONSTER_STATE::REGEN && Get_State() != MONSTER_STATE::ATTACK && Get_State() != MONSTER_STATE::DIE)
	{
		CGameObject* pTarget = CGameMgr::GetInstance()->Get_Player();
		if (nullptr == pTarget)
			return S_OK;
		Set_Target(pTarget);
		_vec3 vTargetPos,  vDir;

		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);


		vDir = vTargetPos - vPos;
		m_vDir = vTargetPos - vPos;

		if (D3DXVec3Length(&vDir) <= 10.f)
		{
			Set_State(MONSTER_STATE::ATTACK);
			m_pAnimator->Play_Animation(L"TrashBummer_Move_Down", true);

		}
	}

	vPos.y += 0.5f;
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
void CTrashBummer::LateUpdate_Object(void)
{
	if (!Is_Active())
		return ;

	Set_Animation();

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
void CTrashBummer::Render_Object(void)
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


void CTrashBummer::Update_Idle(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::MOVE);
			m_pAnimator->Play_Animation(L"TrashBummer_Move_Down", true);
		}

		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CTrashBummer::Update_Die(_float fTimeDelta)
{
	if (Is_Active()) 
	{
		On_Death();
		Set_Active(false);

		int iSound = rand() % 5 + 4;
		Stop_Sound((CHANNELID)iSound);
		Play_Sound(L"SFX_336_MonsterGarbageBummer_In.wav", (CHANNELID)iSound, 0.5f);
	}
}

void CTrashBummer::Update_Regen(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		Set_State(MONSTER_STATE::IDLE);
		m_bShoot = true;
	}
}

void CTrashBummer::Update_Move(_float fTimeDelta)
{
	_vec3 vDir, vPos, vDst;
	if (m_fMoveTime > 5.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"TrashBummer_Idle_Down", true);
		}

		vDst = { float(rand() % 10) - 5.f,0.f,float(rand() % 10) - 5.f };
		if (vDst != m_vDst)
			m_vDst = vDst;
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = m_vDst;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	m_vLook = vDir;
	m_pTarget = nullptr;
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());

}

void CTrashBummer::Update_Attack(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;
	CGameObject* pTarget = CGameMgr::GetInstance()->Get_Player();
	if (nullptr == pTarget)
		return;
	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	vDir = vTargetPos - vPos;
	m_vLook = vDir;

	if (D3DXVec3Length(&vDir) > 10.f && !m_bShoot)
	{
		m_bShooting = false;
		Set_State(MONSTER_STATE::IDLE);
		m_bShoot = true;
	}
	if (D3DXVec3Length(&vDir) < 10.f && D3DXVec3Length(&vDir) >= 7.f)
	{
		m_bShooting = false;
		vDir.y = 0.f;
		D3DXVec3Normalize(&vDir, &vDir);
		m_vLook = vDir;
		m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
		m_bShoot = true;

		int iSound = rand() % 5 + 4;
		Stop_Sound((CHANNELID)iSound);
		Play_Sound(L"SFX_330_MonsterGarbageBummer_Walk.wav", (CHANNELID)iSound, 0.3f);
	}
	else
		Trace(fTimeDelta);
}
HRESULT CTrashBummer::Add_Component(void)
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

CTrashBummer* CTrashBummer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrashBummer* pInstance = new CTrashBummer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TrashBummer Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTrashBummer::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;
	CGameObject* pTarget = CGameMgr::GetInstance()->Get_Player();
	if (nullptr == pTarget)
		return;
	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_bShooting = true;
	vDir = vTargetPos - vPos;
	m_vLook = vDir;
	if (D3DXVec3Length(&vDir) < 7.f && m_bShoot && m_pAnimator->GetCurrAnimation()->Get_Idx() == 4)
	{
		D3DXVec3Normalize(&vDir, &vDir);
		CSludgeBall* pSludgeBall = CSludgeBall::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pSludgeBall, );
		_vec3 BulletPos = vPos;
		BulletPos.y += 0.5f;
		BulletPos.z -= 0.01f;
		pSludgeBall->Get_TransformCom()->Set_Pos(&BulletPos);
		pSludgeBall->Set_Dst(vTargetPos);
		pSludgeBall->Set_StartPoint(BulletPos);
		pSludgeBall->Set_Owner(this);
		pSludgeBall->Set_Atk(m_tStat.iAttack);
		pSludgeBall->Get_RigidBodyCom()->SetMass(10.f);
		pSludgeBall->Get_RigidBodyCom()->AddForce(_vec3(0.0f, 100.0f, 0.0f));
		CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
		pLayer->Add_GameObject(L"SludgeBall", pSludgeBall);

		int iSound = rand() % 5 + 4;
		Stop_Sound((CHANNELID)iSound);
		Play_Sound(L"SFX_315_MonsterGarbageBummer_Attack.wav", (CHANNELID)iSound, 0.6f);
		
		m_bShoot = false;
	}


	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		Set_State(MONSTER_STATE::IDLE);
		m_bShoot = true;
	}

}
void CTrashBummer::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
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

		int iSound = rand() % 5 + 4;
		Stop_Sound((CHANNELID)iSound);
		Play_Sound(L"SFX_313_MonsterGarbageBummer_Hit.wav", (CHANNELID)iSound, 0.5f);

		vDir.y = 0.0f;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pRigidBodyCom->AddForce(vDir * 100.0f);
		m_tStat.iHp -= 1;
		if (m_tStat.iHp < 1)
			Set_State(MONSTER_STATE::DIE);
	}
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_TRIGGER && pCollider->GetOwner()->GetObj_Id() == OBJ_ID::CLEAR_FIELD)
	{
		Set_State(MONSTER_STATE::DIE);
	}
	
}


void CTrashBummer::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	__super::Collision_Stay(pCollider, _eCollisionGroup, _iColliderID);
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_TRIGGER && pCollider->GetOwner()->GetObj_Id() == OBJ_ID::CLEAR_FIELD)
	{
		Set_State(MONSTER_STATE::DIE);
	}
}
void CTrashBummer::Set_Animation()
{

	OBJ_DIR eDir = OBJ_DIR::DIR_END;
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	_vec3 vAxis(0.f, 0.f, 1.f);
	_float fAngle = D3DXVec3Dot(&m_vLook, &vAxis);
	fAngle = acosf(fAngle);

	if (m_vLook.x < 0.0f)
		fAngle = D3DX_PI * 2 - fAngle;


	fAngle = D3DXToDegree(fAngle);



	_uint iDir = fAngle / 22.5f;

	if (iDir == 0 || iDir == 15 || iDir == 16)
	{
		eDir = OBJ_DIR::DIR_U;
	}
	else if (iDir == 1 || iDir == 2)
	{
		eDir = OBJ_DIR::DIR_RU;
	}
	else if (iDir == 3 || iDir == 4)
	{
		eDir = OBJ_DIR::DIR_R;
	}
	else if (iDir == 5 || iDir == 6)
	{
		eDir = OBJ_DIR::DIR_RD;
	}
	else if (iDir == 7 || iDir == 8)
	{
		eDir = OBJ_DIR::DIR_D;
	}
	else if (iDir == 9 || iDir == 10)
	{
		eDir = OBJ_DIR::DIR_LD;
	}
	else if (iDir == 11 || iDir == 12)
	{
		eDir = OBJ_DIR::DIR_L;
	}
	else if (iDir == 13 || iDir == 14)
	{
		eDir = OBJ_DIR::DIR_LU;
	}
	else
		return;
	MONSTER_STATE eState = Get_State();
	_uint iIndex = m_pAnimator->GetCurrAnimation()->Get_Idx();
	switch (eState)
	{
	case Engine::MONSTER_STATE::IDLE:
		switch (eDir)
		{
		case Engine::OBJ_DIR::DIR_U:
			m_pAnimator->Play_Animation(L"TrashBummer_Idle_Up", true);
			break;
		case Engine::OBJ_DIR::DIR_D:
			m_pAnimator->Play_Animation(L"TrashBummer_Idle_Down", true);
			break;
		case Engine::OBJ_DIR::DIR_L:
			m_pAnimator->Play_Animation(L"TrashBummer_Idle_Left", true);
			break;
		case Engine::OBJ_DIR::DIR_R:
			m_pAnimator->Play_Animation(L"TrashBummer_Idle_Right", true);
			break;
		case Engine::OBJ_DIR::DIR_LU:
			m_pAnimator->Play_Animation(L"TrashBummer_Idle_LeftUp", true);
			break;
		case Engine::OBJ_DIR::DIR_RU:
			m_pAnimator->Play_Animation(L"TrashBummer_Idle_RightUp", true);
			break;
		case Engine::OBJ_DIR::DIR_LD:
			m_pAnimator->Play_Animation(L"TrashBummer_Idle_LeftDown", true);
			break;
		case Engine::OBJ_DIR::DIR_RD:
			m_pAnimator->Play_Animation(L"TrashBummer_Idle_RightDown", true);
			break;
		case Engine::OBJ_DIR::DIR_END:
			return;
		default:
			break;
		}
		break;
	case Engine::MONSTER_STATE::MOVE:
		switch (eDir)
		{
		case Engine::OBJ_DIR::DIR_U:
			m_pAnimator->Play_Animation(L"TrashBummer_Move_Up", true);
			break;
		case Engine::OBJ_DIR::DIR_D:
			m_pAnimator->Play_Animation(L"TrashBummer_Move_Down", true);
			break;
		case Engine::OBJ_DIR::DIR_L:
			m_pAnimator->Play_Animation(L"TrashBummer_Move_Left", true);
			break;
		case Engine::OBJ_DIR::DIR_R:
			m_pAnimator->Play_Animation(L"TrashBummer_Move_Right", true);
			break;
		case Engine::OBJ_DIR::DIR_LU:
			m_pAnimator->Play_Animation(L"TrashBummer_Move_LeftUp", true);
			break;
		case Engine::OBJ_DIR::DIR_RU:
			m_pAnimator->Play_Animation(L"TrashBummer_Move_RightUp", true);
			break;
		case Engine::OBJ_DIR::DIR_LD:
			m_pAnimator->Play_Animation(L"TrashBummer_Move_LeftDown", true);
			break;
		case Engine::OBJ_DIR::DIR_RD:
			m_pAnimator->Play_Animation(L"TrashBummer_Move_RightDown", true);
			break;
		case Engine::OBJ_DIR::DIR_END:
			return;
		default:
			break;
		}
		break;
	case Engine::MONSTER_STATE::REGEN:
			m_pAnimator->Play_Animation(L"TrashBummer_Regen_Down", true);
		break;
	case Engine::MONSTER_STATE::ATTACK:
		if ( m_bShooting)
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"TrashBummer_Attack_Up", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"TrashBummer_Attack_Down", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"TrashBummer_Attack_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"TrashBummer_Attack_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"TrashBummer_Attack_LeftUp", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"TrashBummer_Attack_RightUp", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"TrashBummer_Attack_LeftDown", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"TrashBummer_Attack_RightDown", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;
			default:
				break;
			}
		else
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"TrashBummer_Move_Up", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"TrashBummer_Move_Down", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"TrashBummer_Move_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"TrashBummer_Move_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"TrashBummer_Move_LeftUp", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"TrashBummer_Move_RightUp", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"TrashBummer_Move_LeftDown", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"TrashBummer_Move_RightDown", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;
			default:
				break;
			}
		break;
	case Engine::MONSTER_STATE::DIE:
		m_pAnimator->Play_Animation(L"TrashBummer_Idle_Down", true);
		break;
	default:
		break;
	}
	if (m_ePreviousState == eState)
	{
		if(m_ePreviousState != MONSTER_STATE::ATTACK)
		m_pAnimator->GetCurrAnimation()->Set_Idx(iIndex);
		else if (m_bShootState == m_bShooting && m_ePreviousState == MONSTER_STATE::ATTACK)
		{
			m_pAnimator->GetCurrAnimation()->Set_Idx(iIndex);
		}
	}
	m_bShootState = m_bShooting;
	m_ePreviousState = eState;
	m_eDir = eDir;
}
