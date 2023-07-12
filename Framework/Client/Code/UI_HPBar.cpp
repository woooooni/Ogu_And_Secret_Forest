#include "UI_HPBar.h"
#include "Export_Function.h"

CUI_HPBar::CUI_HPBar(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_HPBar::CUI_HPBar(const CUI_HPBar& rhs) : CUI(rhs)
{
}

CUI_HPBar::~CUI_HPBar()
{
}

HRESULT CUI_HPBar::Ready_Object(void)
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_HPGauge"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	return S_OK;
}

_int CUI_HPBar::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	//CGameObject* pPlayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Player");
	//m_iMaxHP = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iMaxHp;
	//m_iHP = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iHp;
	
	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_HPBar::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_HPBar::Render_Object(void)
{
	_matrix matPreView, matPreProj;
	_float fHP = 0.67f; // ���� Hp / Max Hp�� �����ʿ�
	
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	// ������ ���缭 Pos 1�� �����ʿ�. Scale.x �پ�縸ŭ ��ġ���ű��
	_float fOriginWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fWidth = fOriginWidth * fHP; // HPBar ���� ����
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
	_float fRatio = _float(WINCY) / _float(WINCX);
	//_vec3 vScale = _vec3(fWidth * fRatio * 2.5, fHeight * fRatio * 2, 0.f);

	// HPBar �پ����� -> �پ�� ���̸�ŭ Pos�� �Ű������.
	_float fX = (fOriginWidth - (fOriginWidth - fWidth)) * 2; // HPBar �پ�� ����

	// HP Bar ������ Ű�ŭ ���� Ȯ���Ŵ
	_vec3 vScale = _vec3(fWidth * fRatio * 2.5, fHeight * fRatio * 2, 0.f);
	m_pTransformCom->Set_Scale(vScale);

	_vec3 vPos = { ((WINCX - (fX / 2)) / (WINCX) - 1.66f) * (1 / m_matProj._11),
	 ((-1 * WINCY) / WINCY + 1.91f) * (1 / m_matProj._22), 0.f };

	m_pTransformCom->Set_Pos(&vPos);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

	__super::Render_Object();
}

CUI_HPBar* CUI_HPBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_HPBar* pInstance = new CUI_HPBar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("HP Bar Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_HPBar::Free()
{
}