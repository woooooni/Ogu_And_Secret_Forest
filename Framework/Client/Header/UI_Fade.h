#pragma once
#include "CUI.h"
class CUI_Fade :    public CUI
{
public:
	CLONE(CUI_Fade);

protected:
	explicit CUI_Fade(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Fade(const CUI_Fade& rhs);
	virtual ~CUI_Fade();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	static CUI_Fade* Create(LPDIRECT3DDEVICE9 p_GraphicDev);

	void				Set_Fade(const _bool& pFadeType , const _float& pFadeTime) 
	{ 
		m_bIsFadeOut = pFadeType; 
		m_fAlpha = (float)pFadeType;
		m_fFadeTime = pFadeTime;
		m_bIsComplete = false;
	}
	const _bool&		Get_Fade() { return m_bIsFadeOut; }

	void				Set_White(const _bool& p_IsWhite)
	{
		m_bIsWhite = p_IsWhite;
	}

	void				Finish()
	{
		m_bIsComplete = true;
		m_fFadeTime = 0.f;
	}

protected:
	HRESULT				Ready_Component();
	_bool				m_bIsFadeOut;
	_bool				m_bIsWhite;
	_bool				m_bIsComplete;
	_float				m_fAlpha;
	_float				m_fFadeTime;
	

};

