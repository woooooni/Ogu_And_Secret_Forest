#pragma once
#include "CUI.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

enum SHOPITEMTYPE
{
	SHOPKEY_L,
	SHOP_WALLET, SHOP_TEXTBOX, SHOP_IMGBOX,
	SHOP_VERLINE, SHOP_HORLINE, SHOP_PRICETAG,
	UISHOP_LEAF, UISHOP_LEAF_INFO, UISHOP_CLOTH,
	UISHOP_CLOTH_INFO, UISHOP_BRANCH, UISHOP_BRANCH_INFO,	
	SHOPITEM_END
};

struct tagShopItemInfo
{
	SHOPITEMTYPE	eType;
	_uint			iPrice;
};

class CUI_ItemInfo : public CUI
{
	CLONE(CUI_ItemInfo)

private:
	explicit CUI_ItemInfo(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_ItemInfo(const CUI_ItemInfo& rhs);
	virtual ~CUI_ItemInfo();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	HRESULT			Set_Item(CItem* pItem);
	tagShopItemInfo Get_ItemInfo(SHOPITEMTYPE _eType) { return m_tItemInfo; }
	void			Set_Shown(_bool _bShown) { m_bShown = _bShown; }

private:
	HRESULT			Add_Component(void);
	void			Set_Type(SHOPITEMTYPE eType, const _uint& _ePrice = 1);
	void			Set_Cursor(_uint _iX, _uint _iY)
	{
		m_iCursorX = _iX;
		m_iCursorY = _iY;
	}
	void			Key_Input();


private:
	vector<tagShopItemInfo>	m_vecShopItem;
	tagShopItemInfo		m_tItemInfo;
	_vec3				m_vDefaultPos;
	_bool				m_bShown = false;
	_bool				m_bCursorMove = false;
	_uint				m_iCursorX = 0;
	_uint				m_iCursorY = 0;
	_uint				m_iPlayerMoney = 0;

	CItem* m_pItem;
public:
	static  CUI_ItemInfo* Create(LPDIRECT3DDEVICE9 pGraphicDev, SHOPITEMTYPE eType);

private:
	virtual void		Free() override;
};

