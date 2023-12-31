#pragma once
#include "Monster.h"
#include "MothOrb.h"
BEGIN(Engine)
class RcCol;
END
class CMothMage :
    public CMonster
{
    CLONE(CMothMage)
private:
    explicit CMothMage(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMothMage(const CMothMage& rhs);
    virtual ~CMothMage();
public:
    virtual void Update_Idle(_float fTimeDelta) override;
    virtual void Update_Die(_float fTimeDelta) override;
    virtual void Update_Regen(_float fTimeDelta) override;
    virtual void Update_Move(_float fTimeDelta) override;
    virtual void Update_Attack(_float fTimeDelta) override;
    virtual void Update_DefenceMode(_float fTimeDelta);

    void Update_DefenceAttack(_float fTimeDelta);
    

public:
    virtual HRESULT Ready_Object(void) override;
    virtual _int Update_Object(const _float& fTimeDelta) override;
    virtual void LateUpdate_Object(void) override;
    virtual void Render_Object(void) override;


public:
    virtual void Trace(_float fTimeDelta) override;

public:
    virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

    virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

 
private:
    HRESULT	Add_Component(void);
    void Set_Animation();

    
private:
    _float m_fMoveTime;
    _vec3 m_vDst = { 0.f,0.f,0.f };
    _vec3 m_vDir = { 0.f,0.f,0.f };

    _bool m_bShoot = false;
    _bool m_bShooting = false;
    _bool m_bShootState = false;

    CMothOrb* m_pMothOrb;



    _float m_fShootTime;
    _float m_fAccShootTime;

public:
    static  CMothMage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

