#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

class CNpc_Fishing : public CNpc
{
private:
	explicit CNpc_Fishing(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_Fishing(const CNpc_Fishing& rhs);
	virtual ~CNpc_Fishing();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static  CNpc_Fishing* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

