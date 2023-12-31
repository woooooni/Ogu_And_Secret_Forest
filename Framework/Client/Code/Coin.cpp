#include "Coin.h"
#include "Export_Function.h"
#include	"Player.h"
#include "UI_Wallet.h"
#include "UIMgr.h"

CCoin::CCoin(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev, ITEM_TYPE::COIN, OBJ_ID::ITEM)
	, m_bIsCollided(false)
	,m_fColidedTime(0.f)
{
}

CCoin::CCoin(const CCoin& rhs): CItem(rhs)
,m_bIsCollided(rhs.m_bIsCollided)
,m_fColidedTime(rhs.m_fColidedTime)
{
}

CCoin::~CCoin()
{
}

HRESULT CCoin::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// m_pTransformCom->Set_Pos(&_vec3(5.0f, 1.0f, 3.0f));
	m_pTransformCom->Set_Scale({ 0.5f, 0.5f, 0.5f });

	m_pAnimator->Add_Animation(L"Coin_Anime", L"Proto_Texture_Coin", 0.1f);
	m_pAnimator->Play_Animation(L"Coin_Anime", true);
	return S_OK;
}

_int CCoin::Update_Object(const _float& fTimeDelta)
{
	if (!m_bIsCollided)
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_ITEM);
	else
	{
		_vec3 src;
		m_pTransformCom->Get_Info(INFO_POS, &src);
		src.y += fTimeDelta * 1.f ;

		m_fColidedTime += fTimeDelta;
		if (m_fColidedTime > 1.f)
		{
			Set_Active(false);
			return 0;
		}

		m_pTransformCom->Set_Info(INFO_POS, &src);

	}
	Add_RenderGroup(RENDER_ALPHA, this);
	_int Result = __super::Update_Object(fTimeDelta);
	return Result;
}

void CCoin::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CCoin::Render_Object(void)
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
void CCoin::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER)
	{
		CPlayer* player = dynamic_cast<CPlayer*>(pCollider->GetOwner());
		NULL_CHECK_RETURN(player);
		player->Add_Money(m_iMoney);

		Stop_Sound(CHANNELID::SOUND_UI);
		Play_Sound(L"SFX_159_GoldCoinGet.wav", CHANNELID::SOUND_UI, 0.7f);

		m_bIsCollided = true;
		_vec3 src;
		m_pTransformCom->Get_Info(INFO_POS, &src);
		src.y +=  1.f;
		m_pTransformCom->Set_Info(INFO_POS, &src);

		CGameObject* pWallet = CUIMgr::GetInstance()->Get_Wallet();

		if (!pWallet)
		{
			pWallet = CUI_Wallet::Create(m_pGraphicDev);
			pWallet->Ready_Object();
		}
		dynamic_cast<CUI_Wallet*>(pWallet)->Set_WalletAlpha(255);
		pWallet->Set_Active(true);
	}
}
void CCoin::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}
CCoin* CCoin::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& pMoney, const _vec3& pPos)
{
	CCoin* pInstance = new CCoin(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Coin Create Failed");
		return nullptr;
	}

	pInstance->m_iMoney = pMoney;
	_vec3 src = pPos;
	src.y = 0.5f;
	pInstance->m_pTransformCom->Set_Pos(&src);

	return pInstance;
}
HRESULT CCoin::Add_Component(void)
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

void CCoin::Free()
{
	__super::Free();
}
