#include "LightPuzzleBase.h"
#include "Export_Function.h"

CLightPuzzleBase::CLightPuzzleBase(LPDIRECT3DDEVICE9 pGraphicDev) : CLightPuzzle(pGraphicDev), m_bMakeLight(false)
{
}

CLightPuzzleBase::CLightPuzzleBase(const CLightPuzzleBase& rhs) : CLightPuzzle(rhs), m_bMakeLight(rhs.m_bMakeLight)
{
}

CLightPuzzleBase::~CLightPuzzleBase()
{
}

HRESULT CLightPuzzleBase::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Base", L"Proto_Tex_MoonPuzzleBaseTile", 0.1f);
	m_pAnimator->Play_Animation(L"Base", false);

	return S_OK;
}

_int CLightPuzzleBase::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);

	return __super::Update_Object(fTimeDelta);
}

void CLightPuzzleBase::LateUpdate_Object(void)
{
}

void CLightPuzzleBase::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CLightPuzzleBase::Free()
{
	__super::Free();
}

CLightPuzzleBase* CLightPuzzleBase::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos)
{
	CLightPuzzleBase* ret = new CLightPuzzleBase(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("LightPuzzleBase Create Failed");
		return nullptr;
	}

	ret->m_pTransformCom->Set_Pos(&p_Pos);

	return ret;
}
