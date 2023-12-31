#include "../Include/stdafx.h"
#include "SkyBox.h"
#include "Export_Function.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_SKYBOX, OBJ_ID::SKYBOX) , m_iIndex(0)
{
}

CSkyBox::CSkyBox(const CSkyBox& rhs)
	: CGameObject(rhs), m_iIndex(rhs.m_iIndex)
{
}

CSkyBox::~CSkyBox()
{
}

HRESULT CSkyBox::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f });
	return S_OK;
}

_int CSkyBox::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_PRIORITY, this);
	_int iExit = __super::Update_Object(fTimeDelta);
	m_pTransformCom->Set_Scale({ 50.f, 50.f, 50.f });
	return iExit;
}

void CSkyBox::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3 vPos = _vec3(matView._41, matView._42 + 3.f, matView._43);
	m_pTransformCom->Set_Pos(&vPos);
}

void CSkyBox::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Idx(m_iIndex);
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CSkyBox::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_RcCube"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_SkyBox"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iIndex)
{
	CSkyBox* pInstance = new CSkyBox(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("SkyBox Create Failed");
		return nullptr;
	}
	pInstance->Set_Index(_iIndex);
	return pInstance;
}

void CSkyBox::Free()
{
	__super::Free();
}
