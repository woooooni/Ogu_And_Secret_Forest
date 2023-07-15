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

	// 원래 HPBar 가로길이
	m_tInfo.fX = _float(m_pTextureCom->Get_TextureDesc(0).Width);

	m_vDefaultPos = { ((WINCX - (m_tInfo.fX)) / WINCX - 1.52f) * (1 / m_matProj._11),
					 ((-1 * WINCY) / WINCY + 1.91f) * (1 / m_matProj._22), 0.f };

	return S_OK;
}

_int CUI_HPBar::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	// 현재 MaxHP, HP 모두 0으로 되어있어 주석처리함. Player 세팅 완료시 사용
	
	//CGameObject* pPlayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
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
	_float fCurHP, fMaxHP, fHP;
	
	fMaxHP = _float(m_iMaxHP);
	fCurHP = _float(m_iHP);
	fHP = fCurHP / fMaxHP;
	
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	// 프레임 맞춰서 Pos 1차 수정필요. Scale.x 줄어든만큼 위치값옮기기
	_float fOriginWidth = m_tInfo.fX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fWidth = fOriginWidth * fHP; // HPBar 남은 길이
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
	_float fRatio = _float(WINCY) / _float(WINCX);

	// HPBar 줄어든길이 -> 줄어든 길이만큼 Pos를 옮겨줘야함.
	_float fX = fOriginWidth - fWidth; // HPBar 줄어든 길이

	// HP Bar 프레임 키운만큼 같이 확대시킴
	_vec3 vScale = _vec3(fWidth * fRatio * 2.5, fHeight * fRatio * 2, 0.f);
	m_pTransformCom->Set_Scale(vScale);

	_vec3 vMovePos = { m_vDefaultPos.x - fX + 15.f, m_vDefaultPos.y, 0.f };
	// 15.f = 보정값

	m_pTransformCom->Set_Pos(&vMovePos);

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
