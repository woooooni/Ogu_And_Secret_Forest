#pragma once
#include "CUI.h"
#include "TutorialNPC.h"

typedef enum class TextType
{
	COW, SHEEP, PIG, DOOGEE,

	TEXTTYPE_END

}TEXTTYPE;

struct tagTextInfo
{
	wstring			strDesc;
	TEXTTYPE		eType;
};

class CTextBox : public CUI
{
	CLONE(CTextBox)

protected:
	explicit CTextBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTextBox(const CTextBox& rhs);
	virtual ~CTextBox();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void	Set_Shown(bool _bShown) { m_bShown = _bShown; }
	void	Set_Type(TEXTTYPE eType);
	void	Next_Text();

//public:
//	_int	Split_String(LPCTSTR lpszTemp, TCHAR szSep, CStringArray& strArr);
//	_int	Splits_String(CString strTemp, CString strSep, CStringArray& strArr);
private:
	HRESULT			Add_Component(void);

private:
	tagTextInfo m_tInfo = {};
	bool		m_bShown = true;
	UINT		m_iIndex = 0;
	float		m_fAccTime = 0.f;
	float		m_fTextSpeed = 5.f;
	wstring		m_strCurrDesc = L"";

private:
	vector<tagTextInfo>		m_vecText;

public:
	static  CTextBox* Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTTYPE eType);

private:
	virtual void		Free() override;
};

