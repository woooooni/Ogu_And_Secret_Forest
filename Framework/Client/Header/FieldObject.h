#pragma once
#include "Engine_Define.h"
#include "GameObject.h"
class CFieldObject : public CGameObject
{
public:
	CFieldObject(LPDIRECT3DDEVICE9 pGraphicDev);
	CFieldObject(const CFieldObject& rhs);
	virtual ~CFieldObject();


	const OBJ_INFO& Get_ObjInfo() { return m_tInfo; }

	virtual void		Set_SubscribeEvent(_uint pEvent) {};

	virtual void		Render_Object() override;


protected:
	OBJ_INFO m_tInfo;

	virtual HRESULT		Ready_Component() ;

};

