#pragma once
#include "LightPuzzle.h"
class CLightPuzzlePiece :    public CLightPuzzle
{
	CLONE(CLightPuzzlePiece)

protected:
	CLightPuzzlePiece(LPDIRECT3DDEVICE9 pGraphicDev);
	CLightPuzzlePiece(const CLightPuzzlePiece& rhs);
	virtual ~CLightPuzzlePiece();
public:

	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;

	virtual void	Reset_Event();

	virtual void    Free() override;

	static			CLightPuzzlePiece* Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum = 0, const _vec3 p_Pos = { 0, 0, 0 }, const _tchar* p_FirstName = L"Base");

	virtual		void	Set_Lighting(const _bool& pBool) override;


protected:
	_vec3 originPos;
};

