#pragma once
#include "CUI.h"

BEGIN(Engine)
class CRcTex;
class CTransform;
class CTexture;
END

enum QUESTICON
{QUEST_TITLE, QUEST_CONTENTS, QUEST_CLOSE, QUEST_ICON, QUEST_SLOT, QUESTICON_END};

class CUI_QuestIcon : public CUI
{
	CLONE(CUI_QuestIcon)

private:
	CUI_QuestIcon(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_QuestIcon(const CUI_QuestIcon& rhs);
	virtual ~CUI_QuestIcon();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void			Set_Owner(CUI* _pOwner) { m_pOwner = _pOwner; }
	void			Set_Shown(_bool _bshown) { m_bShown = _bshown; }

private:
	HRESULT			Add_Component(void);
	void			Set_Type(QUESTICON eType);

private:
	QUESTICON		m_eIconType;
	_bool			m_bShown = false;
	CUI*			m_pOwner = nullptr;

public:
	static  CUI_QuestIcon* Create(LPDIRECT3DDEVICE9 pGraphicDev, QUESTICON eType);

private:
	virtual void		Free() override;

};

