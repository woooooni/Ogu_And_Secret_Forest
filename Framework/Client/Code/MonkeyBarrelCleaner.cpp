#include "MonkeyBarrelCleaner.h"
#include "Export_Function.h"
#include "ClearBomb.h"
#include "BarrelBomb.h"
CMonkeyBarrelCleaner::CMonkeyBarrelCleaner(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev, OBJ_ID::SPIT_CACTUS)
{
}

CMonkeyBarrelCleaner::CMonkeyBarrelCleaner(const CMonkeyBarrelCleaner& rhs)
	: CMonster(rhs)
{
}

CMonkeyBarrelCleaner::~CMonkeyBarrelCleaner()
{
}




HRESULT CMonkeyBarrelCleaner::Ready_Object(void)
{
	Set_State(MONSTER_STATE::MOVE);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"MonkeyBarrelCleaner_Attack_Left", L"Proto_Texture_MonkeyBarrelCleaner_Attack_Left", 0.2f);
	m_pAnimator->Add_Animation(L"MonkeyBarrelCleaner_Attack_Right", L"Proto_Texture_MonkeyBarrelCleaner_Attack_Right", 0.2f);
	m_pAnimator->Add_Animation(L"MonkeyBarrelCleaner_Idle_Left", L"Proto_Texture_MonkeyBarrelCleaner_Idle_Left", 0.2f);
	m_pAnimator->Add_Animation(L"MonkeyBarrelCleaner_Idle_Right", L"Proto_Texture_MonkeyBarrelCleaner_Idle_Right", 0.2f);
	m_pAnimator->Add_Animation(L"MonkeyBarrelCleaner_Movement_Left", L"Proto_Texture_MonkeyBarrelCleaner_Movement_Left", 0.1f);
	m_pAnimator->Add_Animation(L"MonkeyBarrelCleaner_Movement_Right", L"Proto_Texture_MonkeyBarrelCleaner_Movement_Right", 0.1f);

	m_fMinHeight = 0.75f;
	m_pTransformCom->Set_Pos(&_vec3(1.0f, 0.0f, 9.0f));
	m_pTransformCom->Set_Scale({ 1.5f,1.5f ,1.5f });
	m_pAnimator->Play_Animation(L"MonkeyBarrelCleaner_Idle_Left", true);
	m_tStat = { 3,3,1 };

	m_bPushable = false;
	return S_OK;
}

_int CMonkeyBarrelCleaner::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}
void CMonkeyBarrelCleaner::LateUpdate_Object(void)
{
	Set_Right(m_bRight);
	__super::LateUpdate_Object();
}

void CMonkeyBarrelCleaner::Render_Object(void)
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
	}
}

void CMonkeyBarrelCleaner::Update_Attack(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 2&&m_bShoot)
	{

		_vec3  vPos, vDir;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		if (rand() % 100 < 70)
		{
			CClearBomb* pClearBomb = CClearBomb::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pClearBomb, );
			_vec3 BulletPos = vPos;
			BulletPos.y += 0.25f;

			if (m_bRight)
				vDir = { 2.f, float(rand() % 20 + 4), float(rand() % 3 - 1) * 0.2f };
			else
				vDir = { -2.f, float(rand() % 20 + 4), float(rand() % 3 - 1) * 0.2f };

			D3DXVec3Normalize(&vDir, &vDir);

			pClearBomb->Get_TransformCom()->Set_Pos(&BulletPos);
			pClearBomb->Get_RigidBodyCom()->AddForce(vDir * 150.f);

			CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::INTERACTION_OBJ);
			pLayer->Add_GameObject(L"ClearBomb", pClearBomb);
		}
		else
		{
			CBarrelBomb* pBarrelBomb = CBarrelBomb::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pBarrelBomb, );
			_vec3 BulletPos = vPos;
			BulletPos.y += 0.25f;

			if (m_bRight)
				vDir = { 2.f, float(rand() % 20 + 4), float(rand() % 3 - 1) * 0.2f };
			else
				vDir = { -2.f, float(rand() % 20 + 4), float(rand() % 3 - 1) * 0.2f };

			D3DXVec3Normalize(&vDir, &vDir);

			pBarrelBomb->Get_TransformCom()->Set_Pos(&BulletPos);
			pBarrelBomb->Get_RigidBodyCom()->AddForce(vDir * 150.f);

			CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::INTERACTION_OBJ);
			pLayer->Add_GameObject(L"BarrelBomb", pBarrelBomb);
		}

		m_bShoot = false;
	}
	if (m_pAnimator->GetCurrAnimation()->Get_Idx()==3)
	{
		m_bShoot = true;
		Set_State(MONSTER_STATE::IDLE);
	}
}

void CMonkeyBarrelCleaner::Update_Move(_float fTimeDelta)
{
	_vec3 vDir;
	if (m_bRight)
		vDir = { 1,0,0 };
	else
		vDir = { -1,0,0 };
	m_pTransformCom->Move_Pos(&vDir, 1.f, fTimeDelta);
	if (m_fMoveTime > 10.f&& m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		Set_State(MONSTER_STATE::IDLE);
		m_fMoveTime = 0.f;
	}
	if (m_fMoveTime < 10.f && m_pAnimator->GetCurrAnimation()->Get_Idx() == 7)
	{
		m_pAnimator->GetCurrAnimation()->Set_Idx(1);
	}
	m_fMoveTime += 10.f * fTimeDelta;
}
void CMonkeyBarrelCleaner::Update_Idle(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		m_fMoveTime += 10.f;
		if (m_fMoveTime > 20.f)
		{
			CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::INTERACTION_OBJ);
			vector<Engine::CGameObject*> ObjectVec = pLayer->Get_GameObjectVec();
			_uint iClearBombNum(0);
			for (_uint i = 0; i < ObjectVec.size(); ++i)
			{
				if (ObjectVec[i]->GetObj_Id() == OBJ_ID::CLEAR_BOMB)
					iClearBombNum++;
			}
			if (iClearBombNum < 2)
			{
				m_fMoveTime = 0.f;
				Set_State(MONSTER_STATE::ATTACK);
			}
		}
	}

}
void CMonkeyBarrelCleaner::Update_Die(_float fTimeDelta)
{

}

void CMonkeyBarrelCleaner::Update_Regen(_float fTimeDelta)
{
}


HRESULT CMonkeyBarrelCleaner::Add_Component(void)
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
void CMonkeyBarrelCleaner::Set_Right(_bool _bRight)
{
	m_bRight = _bRight;
	if (_bRight)
		switch (Get_State())
		{
		case Engine::MONSTER_STATE::IDLE:
			m_pAnimator->Play_Animation(L"MonkeyBarrelCleaner_Idle_Right", true);
			break;
		case Engine::MONSTER_STATE::MOVE:
			m_pAnimator->Play_Animation(L"MonkeyBarrelCleaner_Movement_Right", false);
			break;
		case Engine::MONSTER_STATE::ATTACK:
			m_pAnimator->Play_Animation(L"MonkeyBarrelCleaner_Attack_Right", false);
			break;
		}
	else
		switch (Get_State())
		{
		case Engine::MONSTER_STATE::IDLE:
			m_pAnimator->Play_Animation(L"MonkeyBarrelCleaner_Idle_Left", true);
			break;
		case Engine::MONSTER_STATE::MOVE:
			m_pAnimator->Play_Animation(L"MonkeyBarrelCleaner_Movement_Left", false);
			break;
		case Engine::MONSTER_STATE::ATTACK:
			m_pAnimator->Play_Animation(L"MonkeyBarrelCleaner_Attack_Left", false);
			break;
		}
}
CMonkeyBarrelCleaner* CMonkeyBarrelCleaner::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonkeyBarrelCleaner* pInstance = new CMonkeyBarrelCleaner(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MonkeyBarrelCleaner Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMonkeyBarrelCleaner::Trace(_float fTimeDelta)
{
}

void CMonkeyBarrelCleaner::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

