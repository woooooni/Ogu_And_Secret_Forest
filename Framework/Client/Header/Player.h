#pragma once

#include "GameObject.h"
#include "Player_State.h"
#include "Player_State_Skill.h"
#include "Item_Hat.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;
class CAnimator;

END

class CPlayer : public Engine::CGameObject
{
	CLONE(CPlayer)
	typedef struct Player_stats
	{
		_uint iMaxHp = 0;
		_uint iMaxMp = 0;
		_uint iHp = 0;
		_uint iMp = 0;
		_uint iMoney = 0;
		_uint iAttack = 0;
	}STAT;
	
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
	

private:
	HRESULT			Ready_Component(void);
public:
	void			Change_State(PLAYER_STATE _eState)
	{
		if (m_bStop)
			_eState = PLAYER_STATE::IDLE;

		if (!m_bStateChange)
		{
			m_bStateChange = true;
			m_eChangeState = _eState;
		}
	}
	void			Change_State_Now(PLAYER_STATE _eState)
	{
		if (m_bStop)
			_eState = PLAYER_STATE::IDLE;

		m_bStateChange = true;
		m_eChangeState = _eState;
	}
	void			Player_Move(_float fTimeDelta);
	PLAYER_STATE Get_NextState() { return m_eChangeState; }
public:
	// 충돌 호출
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	CCollider* Get_PlayerCol(COLLIDER_PLAYER _eCollider) { return m_pCollider[(_uint)_eCollider]; }
	CGameObject* Get_LiftObj() { return m_pLiftObj; }
	//스텟
	STAT&			Get_PlayerStat() { return m_tPlayerStat; }
	
	void			Add_HP(_uint _iHp) 
	{ 
		m_tPlayerStat.iHp += _iHp; 
		if (m_tPlayerStat.iHp > m_tPlayerStat.iMaxHp)
			m_tPlayerStat.iHp = m_tPlayerStat.iMaxHp;
	}
	void			Add_MP(_uint _iMp)
	{
		m_tPlayerStat.iMp += _iMp;
		if (m_tPlayerStat.iMp > m_tPlayerStat.iMaxMp)
			m_tPlayerStat.iMp = m_tPlayerStat.iMaxMp;
	}
	void			Add_Attack(_uint _iAttack)
	{
		m_tPlayerStat.iAttack += _iAttack;
	}
	void			Add_Money(_uint _iMoney)
	{
		m_tPlayerStat.iMoney += _iMoney;
	}
	void			Minus_Hp(_uint _iHp)
	{
		m_tPlayerStat.iHp -= _iHp;
		if (m_tPlayerStat.iHp < 0)
		{
			m_tPlayerStat.iHp = 0;
			Change_State_Now(PLAYER_STATE::DOWN);
		}
	}
	void			Minus_Mp(_uint _iMp)
	{
		m_tPlayerStat.iMp -= _iMp;
		if (m_tPlayerStat.iMp < 0)
			m_tPlayerStat.iMp = 0;
	}
	void			Minus_Money(_uint _iMoney)
	{
		m_tPlayerStat.iMoney -= _iMoney;
		if (m_tPlayerStat.iMoney < 0)
			m_tPlayerStat.iMoney = 0;
	}
	//아이템
	void			Set_Hat(CGameObject* _pHat) 
	{
		if (m_pHat)
			dynamic_cast<CItem_Hat*>(m_pHat)->UnHat();

		m_pHat = _pHat;
		if (m_pHat)
			dynamic_cast<CPlayer_State_Skill*>(m_vecState[(_uint)PLAYER_STATE::SKILL])->Set_Skill(dynamic_cast<CItem_Hat*>(m_pHat)->Get_Skill());
		else
			dynamic_cast<CPlayer_State_Skill*>(m_vecState[(_uint)PLAYER_STATE::SKILL])->Set_Skill(PLAYER_SKILL::NONE);
	}
	CItem_Hat*		Get_Hat() { return dynamic_cast<CItem_Hat*>(m_pHat); }

	//스킬
	
	void			Set_Skill(PLAYER_SKILL _eSkill) 
	{
		dynamic_cast<CPlayer_State_Skill*>(m_vecState[(_uint)PLAYER_STATE::SKILL])->Set_Skill(_eSkill);
	};

	bool			Is_HaveSkill() { return dynamic_cast<CPlayer_State_Skill*>(m_vecState[(_uint)PLAYER_STATE::SKILL])->Is_HaveSkill(); }
	PLAYER_SKILL	Get_Skill() { return dynamic_cast<CPlayer_State_Skill*>(m_vecState[(_uint)PLAYER_STATE::SKILL])->Get_Skill(); }


	void			Set_SkillRange(CGameObject* _pObj) { m_pSkillRange = _pObj; }
	CGameObject*	Get_SkillRange() { return m_pSkillRange; }

	CGameObject*	Get_Aim() { return m_pAim; }
	void			Set_Aim(CGameObject* _pObj) { m_pAim = _pObj; }

	//상태
	PLAYER_STATE	Get_State() { return m_eState; }

	bool			Is_GetItem() { return m_bGetItem; }
	void			Set_GetItem(bool _bGetItem) { m_bGetItem = _bGetItem; }

	ITEM_CODE		Get_GetItemCode() { return m_eGetItemCode; }
	void			Set_GetItemCode(ITEM_CODE _eItemCode) { m_eGetItemCode = _eItemCode; }

	void			Set_BalloonFly(bool _bBalloonFly) { m_bBalloonFly = _bBalloonFly; }
	void			Set_Flying(bool _bFlying) { m_bFlying = _bFlying; }
	bool			Is_Flying() { return m_bFlying; }
	bool			Is_BalloonFly() { return m_bBalloonFly; }

	bool			Is_Push() { return m_bPush; }
	void			Set_Push(bool _bPush) { m_bPush = _bPush; }

	void			Set_Grab(bool _bGrab) { m_bGrab = _bGrab; }
	bool			Is_Grab() { return m_bGrab; }
	void			Reset_LiftObj() { m_pLiftObj = nullptr; }

	void			Set_DrawingFinish(bool _bFinish) { m_bDrawingFinish = _bFinish; }
	bool			Is_DrawFinish() { return m_bDrawingFinish; }

	void			Set_ItemEffect(ITEM_CODE eItemCode);

	void			Set_Invincible() { m_fAccInvinTime = 0.0f; m_bInvincible = true; }

	bool			Is_Stop() { return m_bStop; }
	void			Set_Stop(bool _bStop) { m_bStop = _bStop; Change_State_Now(PLAYER_STATE::IDLE); }

	CGameObject*	Get_Shadow() { return m_pShadow; }

	void			Set_PlayerHat(PLAYER_HAT _eHat) { m_eHat = _eHat; Set_Hat(m_vecHats[(_uint)m_eHat]); }

	void			Set_Player_SpeedUp(const _float& pSpeed) { m_fSpeedUpTime += pSpeed; m_fSpeed += 1.f; }

	const _float&	Get_Speed() { return m_fSpeed; }

	void			Set_LiftObj(CGameObject* _pObj) { m_pLiftObj = _pObj; }
private:
	_vec3			m_vDir;
	_float			m_fSpeed;

	_vec3			m_vDest;

	bool			m_bStateChange;
	PLAYER_STATE	m_eState;
	PLAYER_STATE	m_eChangeState;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	void Collision_Stay_Push(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);
	void Collision_Enter_Grab(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);
	void Collision_Enter_Hit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);
	void Collision_Enter_BulletHit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);

private:
	virtual void Free() override;

	CCollider* m_pCollider[(_uint)COLLIDER_PLAYER::COLLIDER_END];


	_float		m_fSpeedUpTime;

	CGameObject* m_pLiftObj = nullptr;
	CGameObject* m_pSkillRange = nullptr;
	CGameObject* m_pAim = nullptr;
	CGameObject* m_pHat = nullptr;

	//플레이어 스텟
	STAT m_tPlayerStat;

	//플레이어 상태
	vector<CPlayer_State*> m_vecState;

	bool m_bGetItem = false;


	bool m_bBalloonFly = false;
	bool m_bFlying = false;

	bool m_bPush = false;

	bool m_bGrab = false;

	bool m_bStop = false;
	bool m_bDrawingFinish = false;
	//

	vector<CGameObject*> m_vecHats;
	PLAYER_HAT m_eHat;
	void		Key_Input(const _float& fTimeDelta);

	ITEM_CODE m_eGetItemCode;

	CGameObject* m_pShadow = nullptr;

	bool m_bInvincible = false;
	_float m_fInvinTime;
	_float m_fAccInvinTime;
	_float m_fBlinkTime;

	_float m_fAccMotionTrail;
	_float m_fMotionTrailTime;
};

