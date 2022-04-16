#ifndef Texture_h__
#define Texture_h__

#include "Component.h"
_BEGIN(Engine)

class _ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TEXTURETYPE
	{
		TEX_NORMAL, TEX_CUBE, TEX_COLOR, TEX_END
	};

protected:
	explicit	CTexture(LPDIRECT3DDEVICE9	pDevice);
	explicit	CTexture(const CTexture& rhs);
	virtual		~CTexture(void) = default;

public:
	void		Set_Texture(const _uint& iIndex = 0);
	void		Set_Texture(LPD3DXEFFECT pEffect, const char* pShaderName, const _uint& iIndex = 0);
	_uint		Get_TextureSize();
public:
	HRESULT		Ready_Texture(const _tchar* pPath, TEXTURETYPE	eType, const _uint& iCount = 1);
	HRESULT		Ready_Texture();
	HRESULT		SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, const _uint& iIndex = 0);
private:
	vector<IDirect3DBaseTexture9*>			m_vecTexture;

public:
	static	CTexture*	Create(LPDIRECT3DDEVICE9 pDevice,
		const _tchar* pPath,
		TEXTURETYPE eType,
		const _uint& iCount = 1);
	static CTexture*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone_Component(void) override;
protected:
	virtual void Free(void);
};

_END


#endif // Texture_h__
