#include "TrashSlime.h"
#include "Export_Function.h"
#include "GameMgr.h"
CTrashSlime::CTrashSlime(LPDIRECT3DDEVICE9 pGraphicDev) :CMonster(pGraphicDev, OBJ_ID::TRASH_SLIME)
{
}

CTrashSlime::CTrashSlime(const CTrashSlime& rhs) : CMonster(rhs)
{
}

CTrashSlime::~CTrashSlime()
{
}

HRESULT CTrashSlime::Ready_Object(void)
{
	m_fMoveTime = 0.f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_Down",		L"Proto_Texture_TrashSlime_Idle_Down", 0.2f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_Up",			L"Proto_Texture_TrashSlime_Idle_Up", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_Left",		L"Proto_Texture_TrashSlime_Idle_Left", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_Right",		L"Proto_Texture_TrashSlime_Idle_Right", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_RightDown",	L"Proto_Texture_TrashSlime_Idle_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_RightUp",	L"Proto_Texture_TrashSlime_Idle_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_LeftDown",	L"Proto_Texture_TrashSlime_Idle_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_LeftUp",		L"Proto_Texture_TrashSlime_Idle_LeftUp", 0.1f);
	
	m_pAnimator->Add_Animation(L"TrashSlime_Move_Down",		L"Proto_Texture_TrashSlime_Move_Down", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_Up",			L"Proto_Texture_TrashSlime_Move_Up", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_Left",		L"Proto_Texture_TrashSlime_Move_Left", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_Right",		L"Proto_Texture_TrashSlime_Move_Right", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_RightDown",	L"Proto_Texture_TrashSlime_Move_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_RightUp",	L"Proto_Texture_TrashSlime_Move_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_LeftDown",	L"Proto_Texture_TrashSlime_Move_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_LeftUp",		L"Proto_Texture_TrashSlime_Move_LeftUp", 0.1f);
	m_tStat = { 3,3,1 };
	m_pAnimator->Add_Animation(L"TrashSlime_Regen_Down", L"Proto_Texture_TrashSlime_Regen_Down", 0.1f);
	m_pTransformCom->Set_Pos(&_vec3(5.0f, 1.0f, 5.0f));
	Set_Speed(2.f);
	Set_State(MONSTER_STATE::REGEN);
	m_pAnimator->Play_Animation(L"TrashSlime_Regen_Down", false);
	m_fMinHeight = 0.5f;

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

_int CTrashSlime::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_MONSTER);
	_int iExit = __super::Update_Object(fTimeDelta);

	_vec3 vTargetPos, vPos, vDir;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (MONSTER_STATE::ATTACK != Get_State())
	{
		CGameObject* pTarget = CGameMgr::GetInstance()->Get_Player();
		if (nullptr == pTarget)
			return S_OK; 

		Set_Target(pTarget);

		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		vDir = vTargetPos - vPos;

		if (D3DXVec3Length(&vDir) < 5.f)
		{
			Set_State(MONSTER_STATE::ATTACK);
			m_pAnimator->Play_Animation(L"TrashSlime_Move_Down", true);
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
void CTrashSlime::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

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

void CTrashSlime::Render_Object(void)
{
	if (!Is_Active())
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();

	m_pBufferCom->Render_Buffer();

	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->Render_Object();
		m_pUIGauge->Render_Object();
		m_pUIFrame->Render_Object();
	}
}


void CTrashSlime::Update_Idle(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::MOVE);
			m_pAnimator->Play_Animation(L"TrashSlime_Move_Down", true);
		}
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CTrashSlime::Update_Die(_float fTimeDelta)
{
	if (Is_Active())
		Set_Active(false);
}

void CTrashSlime::Update_Regen(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_Down", true);
		}
	}

}

void CTrashSlime::Update_Move(_float fTimeDelta)
{
	_vec3 vDir, vPos, vDst;
	if (m_fMoveTime > 5.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_Down", true);
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
	m_vLook = vDir;
	D3DXVec3Normalize(&vDir, &vDir);
	m_pTarget = nullptr;
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
}

void CTrashSlime::Update_Attack(_float fTimeDelta)
{
	Trace(fTimeDelta);
}
HRESULT CTrashSlime::Add_Component(void)
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

	return S_OK;
}

CTrashSlime* CTrashSlime::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrashSlime* pInstance = new CTrashSlime(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BlueBeatle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTrashSlime::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = vTargetPos - vPos;
	vDir.y = 0.f;
	if (D3DXVec3Length(&vDir) > 5.f)
	{
		Set_State(MONSTER_STATE::IDLE);
		m_pAnimator->Play_Animation(L"TrashSlime_Idle_Down", true);
		return;
	}
	D3DXVec3Normalize(&vDir, &vDir);
	m_vLook = vDir;
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());

}
void CTrashSlime::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (Get_State() == MONSTER_STATE::DIE)
		return;

	__super::Collision_Enter(pCollider, _eCollisionGroup, _iColliderID);

	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
	{
		_vec3 vTargetPos;
		_vec3 vPos;
		_vec3 vDir;
		pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vDir = vPos - vTargetPos;
		vDir.y = 0.0f;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pRigidBodyCom->AddForce(vDir * 80.0f);
		m_tStat.iHp -= 1;
		if (m_tStat.iHp < 1)
			Set_State(MONSTER_STATE::DIE);
	}
}
void CTrashSlime::Set_Animation()
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
	if (m_ePreviousState == eState && eDir == m_eDir)
		return;

	_uint iIndex = m_pAnimator->GetCurrAnimation()->Get_Idx();
	switch (eState)
	{
	case Engine::MONSTER_STATE::IDLE:
		switch (eDir)
		{
		case Engine::OBJ_DIR::DIR_U:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_Up", true);
			break;
		case Engine::OBJ_DIR::DIR_D:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_Down", true);
			break;
		case Engine::OBJ_DIR::DIR_L:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_Left", true);
			break;
		case Engine::OBJ_DIR::DIR_R:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_Right", true);
			break;
		case Engine::OBJ_DIR::DIR_LU:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_LeftUp", true);
			break;
		case Engine::OBJ_DIR::DIR_RU:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_RightUp", true);
			break;
		case Engine::OBJ_DIR::DIR_LD:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_LeftDown", true);
			break;
		case Engine::OBJ_DIR::DIR_RD:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_RightDown", true);
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
			m_pAnimator->Play_Animation(L"TrashSlime_Move_Up", true);
			break;
		case Engine::OBJ_DIR::DIR_D:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_Down", true);
			break;
		case Engine::OBJ_DIR::DIR_L:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_Left", true);
			break;
		case Engine::OBJ_DIR::DIR_R:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_Right", true);
			break;
		case Engine::OBJ_DIR::DIR_LU:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_LeftUp", true);
			break;
		case Engine::OBJ_DIR::DIR_RU:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_RightUp", true);
			break;
		case Engine::OBJ_DIR::DIR_LD:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_LeftDown", true);
			break;
		case Engine::OBJ_DIR::DIR_RD:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_RightDown", true);
			break;
		case Engine::OBJ_DIR::DIR_END:
			return;
		default:
			break;
		}
		break;
	case Engine::MONSTER_STATE::REGEN:
		m_pAnimator->Play_Animation(L"TrashSlime_Regen_Down", true);
		break;
	case Engine::MONSTER_STATE::ATTACK:
		switch (eDir)
		{
		case Engine::OBJ_DIR::DIR_U:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_Up", true);
			break;
		case Engine::OBJ_DIR::DIR_D:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_Down", true);
			break;
		case Engine::OBJ_DIR::DIR_L:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_Left", true);
			break;
		case Engine::OBJ_DIR::DIR_R:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_Right", true);
			break;
		case Engine::OBJ_DIR::DIR_LU:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_LeftUp", true);
			break;
		case Engine::OBJ_DIR::DIR_RU:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_RightUp", true);
			break;
		case Engine::OBJ_DIR::DIR_LD:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_LeftDown", true);
			break;
		case Engine::OBJ_DIR::DIR_RD:
			m_pAnimator->Play_Animation(L"TrashSlime_Move_RightDown", true);
			break;
		case Engine::OBJ_DIR::DIR_END:
			return;
		default:
			break;
		}
		break;
	case Engine::MONSTER_STATE::DIE:
		switch (eDir)
		{
		case Engine::OBJ_DIR::DIR_U:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_Up", true);
			break;
		case Engine::OBJ_DIR::DIR_D:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_Down", true);
			break;
		case Engine::OBJ_DIR::DIR_L:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_Left", true);
			break;
		case Engine::OBJ_DIR::DIR_R:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_Right", true);
			break;
		case Engine::OBJ_DIR::DIR_LU:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_LeftUp", true);
			break;
		case Engine::OBJ_DIR::DIR_RU:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_RightUp", true);
			break;
		case Engine::OBJ_DIR::DIR_LD:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_LeftDown", true);
			break;
		case Engine::OBJ_DIR::DIR_RD:
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_RightDown", true);
			break;
		case Engine::OBJ_DIR::DIR_END:
			return;
		default:
			break;
		}
		break;
	default:
		break;
	}
	if (m_ePreviousState == eState)
		m_pAnimator->GetCurrAnimation()->Set_Idx(iIndex);
	m_ePreviousState = eState;
	m_eDir = eDir;
}