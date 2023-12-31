#pragma once

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CComponent;
class CVIBuffer;
class CTransform;
class CCollider;
class CTexture;
class CAnimator;
class CRigidBody;
class CShader;

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_TYPE _eType, OBJ_ID _eID);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	const wstring& Get_Name() { return m_strName; }
	void Set_Name(const wstring& _strName) { m_strName = _strName; }

public:
	virtual HRESULT	Ready_Object(void);
	virtual _int Update_Object(const _float& fTimeDelta);
	virtual void LateUpdate_Object(void);
	virtual void Render_Object(void);

public:
	CComponent* Get_Component(COMPONENT_TYPE eType, COMPONENTID eID);

	OBJ_TYPE GetObj_Type() { return m_eType; }
	void SetObj_Type(OBJ_TYPE _eType) { m_eType = _eType; }

	OBJ_DIR GetObj_Dir() { return m_eDir; }
	void SetObj_Dir(OBJ_DIR _eDir) { m_eDir = _eDir; }

	OBJ_ID GetObj_Id() { return m_eID; }
	void SetObj_Id(OBJ_ID _eID) { m_eID = _eID; }

	void Set_Active(_bool _bActive) { m_bActive = _bActive; }
	_bool Is_Active() { return m_bActive; }

	void Set_MinHeight(_float _fHeight) { m_fMinHeight = _fHeight; }
	_float Get_MinHeight() { return m_fMinHeight; }

	void Set_Alpha(_uint _iAlpha) { m_iAlpha = _iAlpha; }
	_uint Get_Alpha() { return m_iAlpha; }

	const D3DMATERIAL9& Get_Material() { return m_tMaterial; }
	void Set_Material(const D3DMATERIAL9& _material) { m_tMaterial = _material; }
public:
	CVIBuffer* Get_BufferCom() { return m_pBufferCom; }
	CTransform* Get_TransformCom() { return m_pTransformCom; }
	CCollider* Get_ColliderCom() { return m_pColliderCom; }
	CAnimator* Get_AnimatorCom() { return m_pAnimator; }
	CTexture* Get_TextureCom() { return m_pTextureCom; }
	CRigidBody* Get_RigidBodyCom() { return m_pRigidBodyCom; }
	CShader* Get_Shader() { return m_pShader; }

public:
	void Set_Billboard();

public:
	// 충돌 호출
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) {};
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)	{};
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)	{};
	
public:
	//이벤트 호출
	virtual void Event_Start(_uint iEventNum) {};
	virtual void Event_End(_uint iEventNum) {};
	virtual void Reset_Event() {};

	
private:
	CComponent* Find_Component(COMPONENT_TYPE eType, COMPONENTID eID);


protected:
	map<COMPONENT_TYPE, CComponent*>		m_mapComponent[ID_END];		
	LPDIRECT3DDEVICE9						m_pGraphicDev;

public:
	_float Get_ViewZ() { return m_fViewZ; }

protected:
	CVIBuffer* m_pBufferCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CCollider* m_pColliderCom = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CRigidBody*	m_pRigidBodyCom = nullptr;
	CShader* m_pShader = nullptr;

protected:
	wstring		m_strName;
	OBJ_TYPE	m_eType;
	OBJ_DIR		m_eDir;
	OBJ_ID		m_eID;

	_float		m_fViewZ;
	_float		m_fMinHeight;

	_uint		m_iAlpha;

	D3DMATERIAL9 m_tMaterial;
private:
	_bool		m_bActive;

public:
	virtual CGameObject* Clone(void)		PURE;
	virtual void Free();
};


END