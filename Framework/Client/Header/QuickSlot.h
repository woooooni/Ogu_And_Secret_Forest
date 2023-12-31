#pragma once
#include "CUI.h"
#include "UI_SlotItems.h"

enum SLOTNUM
{
	SLOT_ONE, SLOT_TWO, SLOT_THREE, SLOT_FOUR,
	SLOT_END
};

class CQuickSlot : public CUI
{
	CLONE(CQuickSlot)
protected:
	explicit CQuickSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CQuickSlot(const CQuickSlot& rhs);
	virtual ~CQuickSlot();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	HRESULT		Add_Slot(void);

	void		Set_Item(SLOTNUM _eSlotNum, _uint _iInvenIndex, ITEM_CODE _eCodeType);

	_bool		Get_Filled(SLOTNUM _eSlotNum);
	void		Set_Filled(SLOTNUM _eSlotNum, _bool _bFilled);

	//vector<ITEM_CODE>	Get_SlotItems() { return m_vecSlotItems; }
	vector<ITEM_CODE>	Get_SlotItems() { return m_vecInSlot; }
	vector<_bool>		Get_IsSetCode() { return m_vecSetCode; }
	void				Set_SetCode(SLOTNUM _eSlotNum, _bool _bSetCode);

	const ITEM_CODE&	Get_ItemCode(SLOTNUM _eSlotNum);
	//void				Set_ItemCode(SLOTNUM _eSlotNum, ITEM_CODE _eCodeType) { m_vecSlotItems[_eSlotNum] = _eCodeType; }
	void				Set_ItemCode(SLOTNUM _eSlotNum, ITEM_CODE _eCodeType) { m_vecInSlot[_eSlotNum] = _eCodeType; }
	const vector<CUI*>	Get_Slots() { return m_vecSlots; }

	void			Set_Use(const _bool& pBool) { m_bCanUse = pBool; }
	const _bool		Get_Use() { return m_bCanUse; }

private:
	vector<CUI*>		m_vecSlots;
	vector<ITEM_CODE>	m_vecInSlot; // 예비 변수
	vector<_bool>		m_vecSetCode;
	vector<_uint>		m_vecCount;
	vector<_uint>		m_vecInvenIndex;

	_bool				m_bCanUse; // 인벤토리창이 열려 있는지(슬롯 아이템을 사용할 수 있는 상태인지)
	_uint				m_iCount;

public:
	static  CQuickSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

