#include "UI_SlotFour.h"
#include "Export_Function.h"

CUI_SlotFour::CUI_SlotFour(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_SlotFour::CUI_SlotFour(const CUI_SlotFour& rhs)
	: CUI(rhs)
{
}

CUI_SlotFour::~CUI_SlotFour()
{
}

HRESULT CUI_SlotFour::Ready_Object(void)
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_QuickSlot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	return S_OK;
}

_int CUI_SlotFour::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_SlotFour::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_SlotFour::Render_Object(void)
{
	_matrix matPreView, matPreProj;
	_vec3 vPos;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	vPos = { (2 * m_tInfo.fX / WINCX - 1) * (1 / m_matProj._11) ,
			(-2 * m_tInfo.fY / WINCY + 1) * (1 / m_matProj._22), 0.f };

	m_pTransformCom->Set_Pos(&vPos);

	_float fRatio = _float(WINCY) / _float(WINCX);
	_vec3 vScale = _vec3(m_tInfo.fCX * fRatio * 0.85, m_tInfo.fCY * 0.85 * fRatio, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

	__super::Render_Object();
}

CUI_SlotFour* CUI_SlotFour::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_SlotFour* pInstance = new CUI_SlotFour(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("QuickSlot2 Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_SlotFour::Free()
{
}
