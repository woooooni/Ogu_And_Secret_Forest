#pragma once
#include "Component.h"


BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
public:
	explicit CTexture();
	explicit CTexture(LPDIRECT3DDEVICE9 _pDevice);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	_int Update_Component(const _float& fTimeDelta) override;

public:
	HRESULT		Ready_Texture(TEXTUREID _eType, const _tchar* _pPath, const _uint& iCnt);
	void		Render_Texture(const _uint& iIndex = 0);

public:
	_uint Get_Size() { return m_vecTexture.size(); }
	LPDIRECT3DBASETEXTURE9	Get_Texture(_uint iTextNum) { return m_vecTexture[iTextNum]; }
	D3DXIMAGE_INFO	Get_TextureDesc(_uint _iIdx) { return m_vecTextureInfo[_iIdx]; }

	void Set_Idx(_uint _iIdx);
	_uint Get_Idx() { return m_iIdx; }

	vector<D3DXIMAGE_INFO>& Get_TextureDescVec() { return m_vecTextureInfo; }

	_bool Is_Finished() { return m_bFinished; }
	void Set_Finished(_bool _bFinished) { m_bFinished = _bFinished; }

	void Set_Frame(_float _fFrame) { m_fFrameTime = _fFrame; }
private:
	vector<IDirect3DBaseTexture9*>	m_vecTexture;
	vector<D3DXIMAGE_INFO>			m_vecTextureInfo;

	_bool							m_bFinished;

	_float							m_fAccTime;
	_float							m_fFrameTime;
	_uint							m_iIdx;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 _pDevice, TEXTUREID eType, _tchar * pPath, const _uint & iCnt = 1);
	virtual CComponent*	Clone();

private:
	virtual void Free();

};

END

