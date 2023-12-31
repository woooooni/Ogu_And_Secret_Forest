#include  "../Include/stdafx.h"
#include "PushStone.h"
#include "Export_Function.h"
#include	"Pool.h"
#include	"Effect_CatapultHit.h"

CPushStone::CPushStone(LPDIRECT3DDEVICE9 pDev) 
												: CFieldObject(pDev, OBJ_ID::PUSH_STONE) 
												, m_bIsFlying(false)
												, m_bIsClean(false)
	, m_bIsOff(false)
{
	m_tInfo.m_bIsPushable = true;
}

CPushStone::CPushStone(const CPushStone& rhs) : CFieldObject(rhs), m_bIsFlying(rhs.m_bIsFlying), m_bIsClean(rhs.m_bIsClean), m_bIsOff(rhs.m_bIsOff)
{
}

CPushStone::~CPushStone()
{
}

HRESULT CPushStone::Ready_Object(void)
{

	FAILED_CHECK(Ready_Component());


	Set_MinHeight(0.5f);

	m_pRigidBodyCom->SetMass(10.f);
	m_pRigidBodyCom->SetMaxVelocity(10000.f);
	m_pRigidBodyCom->SetFricCoeff(999.f);

	m_pAnimator->Add_Animation(L"Base", L"Proto_Texture_Stone_Clean", 0.f);
	m_pAnimator->Play_Animation(L"Base", false);


    return S_OK;
}

_int CPushStone::Update_Object(const _float& fTimeDelta)
{



	if (!m_bIsOff)
	{
		_vec3 src;
		m_pTransformCom->Get_Info(INFO_POS, &src);
		if (m_bIsFlying && m_pRigidBodyCom->IsGround())
		{
			Set_Active(false);
			return 0;
		}
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);
		Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	}



	
    return	__super::Update_Object(fTimeDelta);
	
}

void CPushStone::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CPushStone::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

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


	IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(0);
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);



	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();



	m_pColliderCom->Render_Component();



	if (m_bIsClean)
	{
		_matrix world = *m_pTransformCom->Get_WorldMatrix();
		_vec3 look;
		m_pTransformCom->Get_Info(INFO_LOOK, &look);
		world._41 -= look.x * 0.005f;
		world._42 -= look.y * 0.005f;
		world._43 -= look.z * 0.005f;

		m_pAnimator->Render_Component();


		LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

		CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
		if (pCamera == nullptr)
			return;

		_vec3 vPos;
		pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		D3DVECTOR vCamPos = vPos;


		pEffect->SetMatrix("g_WorldMatrix", &world);
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



void CPushStone::Free()
{
	__super::Free();

}

CPushStone* CPushStone::Create(const _vec3& p_Pos, LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPushStone* pInstance = new CPushStone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player Create Failed");
		return nullptr;
	}
	pInstance->m_pTransformCom->Set_Pos(&p_Pos);
	pInstance->m_bOriginPos = p_Pos;

	return pInstance;

}

void CPushStone::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BOSS && m_bIsFlying)
	{
		CPool<CPushStone>::Return_Obj(this);

		CLayer* pLayerEff = Engine::Get_Layer(LAYER_TYPE::EFFECT);
		NULL_CHECK_RETURN(pLayerEff, );

		for (_uint i = 0; i < 3; ++i)
		{
			CEffect_CatapultHit* pParticle = CEffect_CatapultHit::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pParticle, );
			NULL_CHECK_RETURN(m_pTransformCom, );

			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			vPos.x += .5f * (rand() % 11 - 5);
			vPos.y += .5f * (rand() % 11 - 5);
			vPos.z -= .5f;

			pParticle->Get_Effect(vPos, _vec3(2.f, 2.f, 2.f));
			pLayerEff->Add_GameObject(L"Stone_Particle", pParticle);
		}

		Stop_Sound(CHANNELID::SOUND_EFFECT_INTERACTION);
		Play_Sound(L"SFX_41_Catapult_StoneHit.wav", CHANNELID::SOUND_EFFECT_ENVIRONMENT, .5f);
	}
}

void CPushStone::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BALPAN || _eCollisionGroup == COLLISION_GROUP::COLLIDE_TRIGGER || _eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER || _eCollisionGroup == COLLISION_GROUP::COLLIDE_BULLET)
		return;

	if(!m_bIsFlying)
		Push_Me(pCollider);
	m_pColliderCom->Update_Component(0.f);
}

void CPushStone::Reset_Event()
{
	m_bIsClean = false; m_bIsFlying = false;
	m_bIsOff = false;
	m_pTransformCom->Set_Pos(&m_bOriginPos);
}
void CPushStone::Set_FieldState(const FIELD_OBJ_STATE& pState)
{
	m_eFState = pState;
	switch (m_eFState)
	{
	case CFieldObject::NOT_INTERACT:
		break;
	case CFieldObject::PUSHING:
		break;
	case CFieldObject::LIFTING:
		
		break;
	case CFieldObject::STATE_END:
		break;
	default:
		break;
	}
}

HRESULT CPushStone::Ready_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Stone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);


	return S_OK;

}


