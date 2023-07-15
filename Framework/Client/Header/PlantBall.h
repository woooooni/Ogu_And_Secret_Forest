#pragma once
#include "GameObject.h"



class CPlantBall :
	public CGameObject
{

	CLONE(CPlantBall)

protected:
	explicit CPlantBall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlantBall(const CPlantBall& rhs);
	virtual ~CPlantBall();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

private:
	HRESULT	Add_Component(void);



public:

	void Set_Poison(_bool _bPoison) { if (m_bPoison == _bPoison) return; m_bPoison = _bPoison; }
	_bool Is_Poison() { return m_bPoison; }
	void Set_Dir(_vec3 _Dir) { m_vDir = _Dir; }
	static CPlantBall* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Set_Shooter(CGameObject* _pShooter) { if (m_pShooter == _pShooter) return; m_pShooter = _pShooter; }
	CGameObject*  Get_Shooter() { return  m_pShooter; }
	void Set_Atk(_uint _iAtk) { if (m_iAtk == _iAtk) return; m_iAtk = _iAtk; }
	_uint Get_Atk() { return  m_iAtk; }

private:
	OBJ_DIR m_eDir;
	_vec3 m_vDir;
	_bool	m_bPoison = false;
	CGameObject* m_pShooter;
	_uint m_iAtk=1;
private:
	_float m_fMoveTime=20.f;
private:
	virtual void Free() override;

	void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);

};