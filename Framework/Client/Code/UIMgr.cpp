#include "Export_Function.h"
#include "UIMgr.h"
#include "Pool.h"
#include "Item_Hat_Drill.h"
#include "Item_Hat_Light.h"
#include "Item_Hat_Mask.h"
#include "DefaultItem.h"
#include "GameMgr.h"

IMPLEMENT_SINGLETON(CUIMgr)

CUIMgr::CUIMgr() : m_bUpdateUI(false)
{
}

CUIMgr::~CUIMgr()
{
    Free();
}

HRESULT CUIMgr::Ready_UIMgr(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    FAILED_CHECK_RETURN(Add_Icon(_pGraphicDev), E_FAIL);

    m_pHpBar = CUI_HPBar::Create(_pGraphicDev);
    m_pShop = CUI_Shop::Create(_pGraphicDev);
    m_pShortCutKey = CUI_ShortCutKey::Create(_pGraphicDev);
    m_pQuickSlot = CQuickSlot::Create(_pGraphicDev);
    m_pDialog = CUI_Dialog::Create(_pGraphicDev);
    m_pInventory = CInventoryUI::Create(_pGraphicDev);
    m_pWallet = CUI_Wallet::Create(_pGraphicDev);
    m_pNewQuest = CUI_NewQuest::Create(_pGraphicDev);
    m_pEnding = CUI_Ending::Create(_pGraphicDev);
    m_pMouse = CUI_Mouse::Create(_pGraphicDev);

    m_pQuestList = CUI_QuestList::Create(_pGraphicDev);
    m_pFade = CUI_Fade::Create(_pGraphicDev);

    NULL_CHECK_RETURN(m_pDialog, E_FAIL);
    NULL_CHECK_RETURN(m_pHpBar, E_FAIL);
    NULL_CHECK_RETURN(m_pShop, E_FAIL);
    NULL_CHECK_RETURN(m_pQuickSlot, E_FAIL);
    NULL_CHECK_RETURN(m_pShortCutKey, E_FAIL); 
    NULL_CHECK_RETURN(m_pInventory, E_FAIL);
    NULL_CHECK_RETURN(m_pWallet, E_FAIL);
    NULL_CHECK_RETURN(m_pNewQuest, E_FAIL);
    NULL_CHECK_RETURN(m_pMouse, E_FAIL);
    NULL_CHECK_RETURN(m_pQuestList, E_FAIL);
    NULL_CHECK_RETURN(m_pEnding, E_FAIL);
    NULL_CHECK_RETURN(m_pFade, E_FAIL);
    //NULL_CHECK_RETURN(m_pMapName, E_FAIL);

    // m_pVeil = CUI_Veil::Create(_pGraphicDev);
    // NULL_CHECK_RETURN(m_pVeil, E_FAIL);

    // m_pShop->Add_Item(CItem_Hat_Mask::Create(_pGraphicDev, Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player")), SHOPITEMTYPE::UISHOP_LEAF);
    // m_pShop->Add_Item(CItem_Hat_Drill::Create(_pGraphicDev, Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player")), SHOPITEMTYPE::UISHOP_CLOTH);
    // m_pShop->Add_Item(CItem_Hat_Light::Create(_pGraphicDev, Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player")), SHOPITEMTYPE::UISHOP_LEAF);

    m_pNewQuest->Set_Active(false);
    m_pQuestList->Set_Active(false);
    m_pEnding->Set_Active(false);
    FAILED_CHECK_RETURN(Add_Frame(_pGraphicDev), E_FAIL);

    return S_OK;
}

void CUIMgr::Update_UIMgr(const _float& fTimeDelta)
{
    m_vecIcon[HEART]->Update_Object(fTimeDelta);
    m_vecIcon[PLAYERHP_BACK]->Update_Object(fTimeDelta);
    m_vecIcon[HEART]->Update_Object(fTimeDelta);
    m_vecIcon[KEYBOARD]->Update_Object(fTimeDelta);
    m_vecIcon[QUEST]->Update_Object(fTimeDelta);

    m_pHpBar->Update_Object(fTimeDelta);
    m_pShop->Update_Object(fTimeDelta);
    m_pQuickSlot->Update_Object(fTimeDelta);
    //m_pMapName->Update_Object(fTimeDelta);


    m_vecIcon[PLAYERHP_FRAME]->Update_Object(fTimeDelta);
    m_vecIcon[KEYBUTTON_1]->Update_Object(fTimeDelta);
    m_vecIcon[KEYBUTTON_2]->Update_Object(fTimeDelta);
    m_vecIcon[KEYBUTTON_3]->Update_Object(fTimeDelta);
    m_vecIcon[KEYBUTTON_4]->Update_Object(fTimeDelta);
    m_vecIcon[KEYBUTTON_L]->Update_Object(fTimeDelta);



    if (m_pShortCutKey->Is_Active())
        m_pShortCutKey->Update_Object(fTimeDelta);

    if (m_pDialog->Is_Active())
        m_pDialog->Update_Object(fTimeDelta);

    if (m_pWallet->Is_Active())
        m_pWallet->Update_Object(fTimeDelta);

    
    m_pNewQuest->Update_Object(fTimeDelta);

    if (m_pQuestList->Is_Active())
        m_pQuestList->Update_Object(fTimeDelta);

    if (KEY_TAP(KEY::I))
    {
        Stop_Sound(CHANNELID::SOUND_UI);
        Play_Sound(L"SFX_67_UIBig_Open.wav", CHANNELID::SOUND_UI, 0.5f);

        m_bUpdateUI = !m_bUpdateUI;
        m_pQuickSlot->Set_Use(!m_bUpdateUI);
        if (!m_bUpdateUI)
        {
            m_pInventory->Reset_Index();
        }
    }


    if (KEY_TAP(KEY::P))
    {
        m_pFade->Set_White(true);
        m_pFade->Set_Fade(!m_pFade->Get_Fade(), 3.f);

    }
    if (m_bUpdateUI)
        m_pInventory->Update_Object(fTimeDelta);

    m_pMouse->Update_Object(fTimeDelta);

    m_pFade->Update_Object(fTimeDelta);

    if (m_pEnding->Is_Active())
        m_pEnding->Update_Object(fTimeDelta);


}

void CUIMgr::Late_Update_UIMgr()
{
    m_vecIcon[PLAYERHP_BACK]->LateUpdate_Object();
    m_vecIcon[HEART]->LateUpdate_Object();
    m_vecIcon[KEYBOARD]->LateUpdate_Object();
    m_vecIcon[QUEST]->LateUpdate_Object();

    m_pDialog->LateUpdate_Object();
    m_pHpBar->LateUpdate_Object();
    m_pShop->LateUpdate_Object();
    m_pShortCutKey->LateUpdate_Object();
    m_pQuickSlot->LateUpdate_Object();
    //m_pItemWindow->LateUpdate_Object();
    // m_pCurrentUI->LateUpdate_Object();
    //m_pMapName->LateUpdate_Object();

//   if (m_pNewQuest->Is_Active() || m_pDialog->Is_Active())
//       m_pQuestList->Set_Active(false);
 
    if (m_pWallet->Is_Active())
        m_pWallet->LateUpdate_Object();

    if (m_bUpdateUI)
        m_pInventory->LateUpdate_Object();

    if (m_pNewQuest->Is_Active())
        m_pNewQuest->LateUpdate_Object();

    if (m_pQuestList->Is_Active())
        m_pQuestList->LateUpdate_Object();

    m_vecIcon[PLAYERHP_FRAME]->LateUpdate_Object();
    m_vecIcon[KEYBUTTON_1]->LateUpdate_Object();
    m_vecIcon[KEYBUTTON_2]->LateUpdate_Object();
    m_vecIcon[KEYBUTTON_3]->LateUpdate_Object();
    m_vecIcon[KEYBUTTON_4]->LateUpdate_Object();
    m_vecIcon[KEYBUTTON_L]->LateUpdate_Object();

    m_pFade->LateUpdate_Object();
}

void CUIMgr::Render_UIMgr()
{
}


HRESULT CUIMgr::Add_Icon(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    m_vecIcon.reserve(ICONTYPE::ICONTYPE_END);

    CIcon* pHpBack = CIcon::Create(_pGraphicDev, ICONTYPE::PLAYERHP_BACK);
    m_vecIcon.push_back(pHpBack);
    CIcon* pHeart = CIcon::Create(_pGraphicDev, ICONTYPE::HEART);
    m_vecIcon.push_back(pHeart);
    CIcon* pKeyboardIcon = CIcon::Create(_pGraphicDev, ICONTYPE::KEYBOARD);
    m_vecIcon.push_back(pKeyboardIcon);
    CIcon* pQuestIcon = CIcon::Create(_pGraphicDev, ICONTYPE::QUEST);
    m_vecIcon.push_back(pQuestIcon);

    return S_OK;
}

HRESULT CUIMgr::Add_Frame(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CIcon* pHpFrame = CIcon::Create(_pGraphicDev, ICONTYPE::PLAYERHP_FRAME);
    m_vecIcon.push_back(pHpFrame);
    CIcon* pButton1 = CIcon::Create(_pGraphicDev, ICONTYPE::KEYBUTTON_1);
    m_vecIcon.push_back(pButton1);
    CIcon* pButton2 = CIcon::Create(_pGraphicDev, ICONTYPE::KEYBUTTON_2);
    m_vecIcon.push_back(pButton2);
    CIcon* pButton3 = CIcon::Create(_pGraphicDev, ICONTYPE::KEYBUTTON_3);
    m_vecIcon.push_back(pButton3);
    CIcon* pButton4 = CIcon::Create(_pGraphicDev, ICONTYPE::KEYBUTTON_4);
    m_vecIcon.push_back(pButton4);
    CIcon* pButtonL = CIcon::Create(_pGraphicDev, ICONTYPE::KEYBUTTON_L);
    m_vecIcon.push_back(pButtonL);

    return S_OK;
}

void CUIMgr::Free()
{

    if(m_pDialog)
        Safe_Release(m_pDialog);
    if(m_pHpBar)
        Safe_Release(m_pHpBar);
    if(m_pShop)
        Safe_Release(m_pShop);
    if(m_pItemWindow)
        Safe_Release(m_pItemWindow);
    if(m_pVeil)
        Safe_Release(m_pVeil);
    if(m_pInventory)
        Safe_Release(m_pInventory);
    if(m_pShortCutKey)
        Safe_Release(m_pShortCutKey);
    if(m_pQuickSlot)
        Safe_Release(m_pQuickSlot);
    if(m_pBossHpBar)
        Safe_Release(m_pBossHpBar);
    if (m_pWallet)
        Safe_Release(m_pWallet);
    if (m_pQuestList)
        Safe_Release(m_pQuestList);

    for (size_t i = 0; i < ICONTYPE::ICONTYPE_END; ++i)
        Safe_Release(m_vecIcon[i]);
    
}
