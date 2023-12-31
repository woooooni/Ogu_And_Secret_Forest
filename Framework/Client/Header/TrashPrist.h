#pragma once
#include "Monster.h"
BEGIN(Engine)
class RcCol;
END
class CTrashPrist :
    public CMonster
{
    CLONE(CTrashPrist)
    private:
        explicit CTrashPrist(LPDIRECT3DDEVICE9 pGraphicDev);
        explicit CTrashPrist(const CTrashPrist& rhs);
        virtual ~CTrashPrist();
    public:
        virtual void Update_Idle(_float fTimeDelta) override;
        virtual void Update_Die(_float fTimeDelta) override;
        virtual void Update_Regen(_float fTimeDelta) override;
        virtual void Update_Move(_float fTimeDelta) override;
        virtual void Update_Attack(_float fTimeDelta) override;
    private:
        virtual HRESULT Ready_Object(void)							override;
        virtual _int	Update_Object(const _float& fTimeDelta)		override;
        virtual void Render_Object(void) override;
        virtual void	LateUpdate_Object(void)						override;

        HRESULT	Add_Component(void);
    public:
        static  CTrashPrist* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    private:
        _float m_fMoveTime;
        _float  m_fCount;
        _vec3 m_vDst = { 0.f,0.f,0.f };
        _int m_iMaxSummon = 0;
        virtual void Trace(_float fTimeDelta) override;

        void Summon_Monster(_float fTimeDelta);

        virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

        void Set_Animation();


};

