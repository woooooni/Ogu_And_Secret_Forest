#include "UI_NewQuest.h"
#include "Export_Function.h"
#include "UIMgr.h"
#include "GameMgr.h"
#include "InventoryMgr.h"

CUI_NewQuest::CUI_NewQuest(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CUI(pGraphicDev)
{
}

CUI_NewQuest::CUI_NewQuest(const CUI_NewQuest& rhs) 
	: CUI(rhs)
{
}

CUI_NewQuest::~CUI_NewQuest()
{
}

HRESULT CUI_NewQuest::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	m_pWindow = CUI_Notification::Create(m_pGraphicDev, 1.f, 0.79f);
	if (m_pWindow != nullptr)
		m_pWindow->Set_Owner(this);
	
	m_pTitleBox = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_TITLE);
	if (m_pTitleBox != nullptr)
		m_pTitleBox->Set_Owner(this);

	m_pContentsBox = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_CONTENTS);
	if (m_pContentsBox != nullptr)
		m_pContentsBox->Set_Owner(this);

	m_pExclamIcon = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_ICON);
	if (m_pExclamIcon != nullptr)
		m_pExclamIcon->Set_Owner(this);

	m_pCloseKey = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_CLOSE);
	if (m_pCloseKey != nullptr)
		m_pCloseKey->Set_Owner(this);

	m_pQuestList = CUIMgr::GetInstance()->Get_QuestList();
	m_pShortcutKey = CUIMgr::GetInstance()->Get_ShortcutKey();

	Set_Active(false);

	return S_OK;
}

_int CUI_NewQuest::Update_Object(const _float& fTimeDelta)
{

	if (m_bReserve 
		&& !CUIMgr::GetInstance()->Get_Dialog()->Is_Active() 
		&& !CGameMgr::GetInstance()->Get_Player()->Is_GetItem()
		&& CGameMgr::GetInstance()->Get_Player()->Get_State() != PLAYER_STATE::GETTIEM
		&& CGameMgr::GetInstance()->Get_Player()->Get_NextState() != PLAYER_STATE::GETTIEM)
	{
		CGameObject* pObj = Engine::Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"UI_GetItem");
		if (pObj == nullptr)
		{
			Set_Active(true);
			m_bReserve = false;
		}
	}
		
	if (!Is_Active())
	{
		if (!m_pShortcutKey->Get_Shown())
			m_pShortcutKey->Set_Active(true);

		return S_OK;
	}

	
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	m_pWindow->Update_Object(fTimeDelta);

	m_pCloseKey->Update_Object(fTimeDelta);
	m_pTitleBox->Update_Object(fTimeDelta);
	m_pContentsBox->Update_Object(fTimeDelta);
	m_pExclamIcon->Update_Object(fTimeDelta);

	Key_Input();

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_NewQuest::LateUpdate_Object(void)
{
	if (Is_Active())
	{
		m_pShortcutKey->Set_Shown(false);
	}

	m_pWindow->LateUpdate_Object();

	m_pCloseKey->LateUpdate_Object();
	m_pTitleBox->LateUpdate_Object();
	m_pContentsBox->LateUpdate_Object();
	m_pExclamIcon->LateUpdate_Object();

	m_fCurWidth = m_pWindow->Get_CurWidth();
	m_fMaxWidth = m_pWindow->Get_MaxWidth();

	if (m_fCurWidth > m_fMaxWidth)
	{
		m_pCloseKey->Set_Shown(true);
		m_pTitleBox->Set_Shown(true);
		m_pContentsBox->Set_Shown(true);
		m_pExclamIcon->Set_Shown(true);

		m_bShown = true;
	}

	if (!m_bShown)
	{
		FMOD_RESULT pRes = FMOD_Channel_SetPaused(Get_Channel(CHANNELID::SOUND_BGM), true);
		Stop_Sound(CHANNELID::SOUND_UI);
		Play_Sound(L"SFX_446_QuestClear.wav", CHANNELID::SOUND_UI, 1.f);
	}

	__super::LateUpdate_Object();
}

void CUI_NewQuest::Render_Object(void)
{

	if (m_bShown)
	{
		// ����Ʈ â Title
		RECT rcTitle = { 0, 0, WINCX, WINCY / 2 - 50.f };
		TCHAR szTitleBuf[256] = L"";

		swprintf_s(szTitleBuf, L"����Ʈ�� �޾Ҵ�!");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szTitleBuf, lstrlen(szTitleBuf), &rcTitle, DT_CENTER | DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		// ����Ʈ ��
		RECT rcQuest = { WINCX / 4 + 30, 5, WINCX, WINCY - 100 };
		TCHAR szQuestBuf[256] = L"";

		swprintf_s(szQuestBuf, m_strTitle.c_str());
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szQuestBuf, lstrlen(szQuestBuf), &rcQuest, DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		// ����Ʈ ����
		RECT rcContents = { 0, 170, WINCX, WINCY };
		TCHAR szConBuf[256] = L"";

		swprintf_s(szConBuf, m_strDesc.c_str());
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			szConBuf, lstrlen(szConBuf), &rcContents, DT_CENTER | DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CUI_NewQuest::Key_Input()
{
	if (m_bShown)
	{
		if (KEY_AWAY(KEY::TAB))
		{
			Set_Active(false);
			m_bShown = false;

			m_pWindow->Set_Active(false);
			m_pWindow->Set_InitSize(64.f, 43.f);
			m_pShortcutKey->Set_Shown(true);

			m_pCloseKey->Set_Active(false);
			m_pCloseKey->Set_Shown(false);

			m_pTitleBox->Set_Active(false);
			m_pTitleBox->Set_Shown(false);

			m_pContentsBox->Set_Active(false);
			m_pContentsBox->Set_Shown(false);

			m_pExclamIcon->Set_Active(false);
			m_pExclamIcon->Set_Shown(false);

			FMOD_Channel_SetPaused(Get_Channel(CHANNELID::SOUND_BGM), false);
		}
	}
}

CUI_NewQuest* CUI_NewQuest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_NewQuest* pInstance = new CUI_NewQuest(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("New Quest Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_NewQuest::Free()
{
}


