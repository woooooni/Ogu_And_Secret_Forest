#include "NueFlower.h"
#include    "Export_Function.h"

CNueFlower::CNueFlower(LPDIRECT3DDEVICE9 p_Dev) : CGameObject(p_Dev, OBJ_TYPE::OBJ_ENVIRONMENT, OBJ_ID::LIGHT_FLOWER), m_fMaxDistance(180.f), m_fFarSpeed(1.f)
{
}

CNueFlower::CNueFlower(const CNueFlower& rhs) : CGameObject(rhs), m_fMaxDistance(rhs.m_fMaxDistance), m_fFarSpeed(1.f)
{
}

CNueFlower::~CNueFlower()
{
}

HRESULT CNueFlower::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());

    m_pAnimator->Add_Animation(L"Flower", L"Proto_Tex_NueFlower", 0.1f);
    m_pAnimator->Play_Animation(L"Flower", false);


    return S_OK;
}

_int CNueFlower::Update_Object(const _float& fTimeDelta)
{
    Add_RenderGroup(RENDER_ALPHA, this);

    if (D3DXVec3Length(&m_vmyPos) < m_fMaxDistance)
    {
        _float size = D3DXVec3Length(&m_vmyPos);

        m_vmyPos += m_vmyPos * m_fFarSpeed * fTimeDelta / size;

    }
    else if (D3DXVec3Length(&m_vmyPos) > m_fMaxDistance)
    {
        m_vmyPos = *D3DXVec3Normalize(&m_vmyPos, &m_vmyPos) * m_fMaxDistance;
    }

    return __super::Update_Object(fTimeDelta);
}

void CNueFlower::LateUpdate_Object(void)
{
    __super::LateUpdate_Object();

}

void CNueFlower::Render_Object(void)
{
    m_pTransformCom->Set_Pos(&(m_vmyPos + m_voriginPos));
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    __super::Render_Object();
    m_pBufferCom->Render_Buffer();
}

void CNueFlower::Free()
{
    __super::Free();
}

CNueFlower* CNueFlower::Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3& p_Pos, const _vec3& pDir, const _float& pFirstLength, const _float& pLastLength)
{
    CNueFlower* ret = new CNueFlower(p_Dev);

    NULL_CHECK_RETURN(ret, nullptr);

    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("NueFlower Create Failed");
        return nullptr;
    }


    ret->m_voriginPos = p_Pos;
    D3DXVec3Normalize(&ret->m_vmyPos, &pDir);
    ret->m_vmyPos *= pFirstLength;
    ret->m_fMaxDistance = pLastLength;

    ret->m_pTransformCom->Set_Pos(&(ret->m_voriginPos + ret->m_vmyPos));

    return ret;
}

void CNueFlower::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CNueFlower::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CNueFlower::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

HRESULT CNueFlower::Ready_Component(void)
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
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(this);
    m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

    pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(this);
    m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);


    return S_OK;


}