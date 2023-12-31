#pragma once
#include "Effect.h"
class CEffect_Hit : public CEffect
{
	CLONE(CEffect_Hit)
protected:
	explicit CEffect_Hit(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Hit(const CEffect& rhs);
	virtual ~CEffect_Hit();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Return_Pool(void)							override;


	static CEffect_Hit* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void			Get_Effect(_vec3& _vPos, _vec3& _vScale, _uint _iR = 255, _uint _iG = 255, _uint _iB = 255);
protected:
	virtual HRESULT	Add_Component(void);


	_uint m_iR;
	_uint m_iG;
	_uint m_iB;
protected:
	virtual void Free() override;
};

