#include "BarrelBomb.h"
#include    "Export_Function.h"
#include "Monster.h"
#include    "Player.h"

CBarrelBomb::CBarrelBomb(LPDIRECT3DDEVICE9 p_Dev)
    : CFieldObject(p_Dev, OBJ_ID::JELLY_BOMB)
    , m_bHitted(false), m_fExplodeTime(0.f), m_iExplodeEvent(0), m_bExplosing(false)
    , m_fBlurAlpha(0.f)
    ,m_bIsPlusAlpha(false)
{
}

CBarrelBomb::CBarrelBomb(const CBarrelBomb& rhs)
    : CFieldObject(rhs), m_bHitted(rhs.m_bHitted), m_fExplodeTime(rhs.m_fExplodeTime), m_iExplodeEvent(rhs.m_iExplodeEvent)
    , m_fBlurAlpha(rhs.m_fBlurAlpha)
    , m_bExplosing(rhs.m_bExplosing)
    ,m_bIsPlusAlpha(rhs.m_bIsPlusAlpha)
{
    m_tInfo.m_bIsAttackable = true;
    m_tInfo.m_bIsPushable = true;

}

CBarrelBomb::~CBarrelBomb()
{
}

HRESULT CBarrelBomb::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());

    m_pAnimator->Add_Animation(L"Base", L"Proto_Tex_Barrel_Explosive", 1.f);
    m_pAnimator->Add_Animation(L"Explode", L"Proto_Tex_JellyBomb_Explode", 0.1f);
    m_pAnimator->Play_Animation(L"Base", false);

    CComponent* pCOm = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Barrel_Explosive_Blur"));
    NULL_CHECK_RETURN(pCOm, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pCOm);

    return S_OK;
}

_int CBarrelBomb::Update_Object(const _float& fTimeDelta)
{
    if (m_bHitted)
    {
        if (m_fBlurAlpha < 1.f && m_bIsPlusAlpha)
        {
            m_fBlurAlpha += fTimeDelta * 10;
        }
        else if (m_fBlurAlpha >= 1.f && m_bIsPlusAlpha)
        {
            m_fBlurAlpha = 1.f;
            m_bIsPlusAlpha = false;
        }
        else if (m_fBlurAlpha > 0.f && !m_bIsPlusAlpha)
        {
            m_fBlurAlpha -= fTimeDelta * 10;
        }
        else if (m_fBlurAlpha <= 0.f && !m_bIsPlusAlpha)
        {
            m_fBlurAlpha = 0.f;
            m_bIsPlusAlpha = true;
        }

        m_fExplodeTime += fTimeDelta;

        if (m_fExplodeTime > 1.f)
        {
            m_bHitted = false;
            m_pAnimator->Play_Animation(L"Explode", false);
            m_bExplosing = true;
            m_fExplodeTime = 0.f;
            m_fBlurAlpha = 0.f;
            m_bIsPlusAlpha = false;
            dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 3.f, 3.f, 3.f });
            m_pTransformCom->Set_Scale({ 3.f, 3.f, 3.f });
            _vec3 mypos;
            m_pTransformCom->Get_Info(INFO_POS, &mypos);
            mypos.y = 1.5f;
            m_pTransformCom->Set_Pos(&mypos);
            
        }
    }

    if (m_bExplosing)
    {
        m_fExplodeTime += fTimeDelta;

        if (m_fExplodeTime > 1.6f)
        {
            Set_Active(false);
            return 0;
        }

        Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BOMB);
        
    }
    else
        Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);

    Add_RenderGroup(RENDER_ALPHA, this);

    return __super::Update_Object(fTimeDelta);
}

void CBarrelBomb::LateUpdate_Object(void)
{
    __super::LateUpdate_Object();
}

void CBarrelBomb::Render_Object(void)
{
    _matrix world = *m_pTransformCom->Get_WorldMatrix();

    
    __super::Render_Object();

    if (m_bHitted)
    {
        world._43 -= 0.01f;
        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((LONG)(255.f * m_fBlurAlpha), 255, 255, 255));
        m_pGraphicDev->SetTransform(D3DTS_WORLD, &world);
        m_pTextureCom->Render_Texture();
        m_pBufferCom->Render_Buffer();
        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

    }
}

void CBarrelBomb::Free()
{
    __super::Free();
}

CBarrelBomb* CBarrelBomb::Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3& p_Pos)
{
    CBarrelBomb* ret = new CBarrelBomb(p_Dev);
    NULL_CHECK_RETURN(ret, nullptr);
    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("CBarrelBomb Failed");
        return nullptr;
    }

    ret->m_pTransformCom->Set_Pos(&p_Pos);

    return ret;
}

void CBarrelBomb::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
    if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && !m_bHitted && !m_bExplosing)
    {
        m_bHitted = true;
        m_bIsPlusAlpha = true;

    }
}

void CBarrelBomb::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBarrelBomb::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBarrelBomb::Event_Start(_uint iEventNum)
{
}

void CBarrelBomb::Event_End(_uint iEventNum)
{
}

void CBarrelBomb::Set_SubscribeEvent(_uint pEvent)
{
}

void CBarrelBomb::Reset()
{
}
