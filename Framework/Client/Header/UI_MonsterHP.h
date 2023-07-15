#pragma once
#include "CUI.h"
#include "Monster.h"

// Back, Frame, HP

typedef enum class MonsterHP
{
	UI_FRAME,
	UI_GAUGE,
	UI_BACK,

	MONSTERHP_END

}MONSTERHP;

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END;

class CUI_MonsterHP : public CUI
{
private:
	CUI_MonsterHP(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_MonsterHP(const CUI_MonsterHP& rhs);
	virtual ~CUI_MonsterHP();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	HRESULT	Add_Component(void);
	void	Set_Type(MONSTERHP eType);

private:
	MONSTERHP	m_eUIType;
	_vec3		m_vDefaultPos; // UI
	_int		m_iMaxHP; // ���� �ִ� ü��
	_int		m_iHP;	  // ���� ���� ü��
	_bool		m_bShown = true; // UI�� ����°�?
	_vec3		m_vMonsterPos; // ���� ���� ��ġ

public:
	static  CUI_MonsterHP* Create(LPDIRECT3DDEVICE9 pGraphicDev, MONSTERHP eType);

private:
	virtual void		Free() override;
};
