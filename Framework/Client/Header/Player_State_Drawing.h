#pragma once
#include "Player_State.h"
class CPlayer_State_Drawing : public CPlayer_State
{
public:
	CPlayer_State_Drawing(CGameObject* _pOwner);
	~CPlayer_State_Drawing();

public:

	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;
	virtual void			Reset_State(void) override;

private:
	bool	m_bFinished = false;

	_float m_fAccTime;
	_float m_fDrawTime;


	void					Update_Hat();
	void					Set_Hat();


	vector<_vec3> m_vecHatPos;
	float m_fAngle[18];
	float m_fScale[18];
};

