#ifndef Font_h__
#define Font_h__

#include "Base.h"

_BEGIN(Engine)

class CFont final : public CBase
{
private:
	explicit	 CFont(LPDIRECT3DDEVICE9 pDevice);
	virtual		 ~CFont(void);


public:
	HRESULT		Ready_Font(const _tchar* pFontType,
							const _uint& iWidth,
							const _uint& iHeight,
							const _uint& iWeight);

	void		Render_Font(const _tchar* pString,
							const _vec2* pPosition,
							D3DXCOLOR Color);
	void		Render_Font(const _tchar* pString,
							const _vec3* pPosition,
							D3DXCOLOR Color);

	void		Make_ViewportMatrix(_matrix* pOut, const D3DVIEWPORT9* pViewport);

private:
	LPDIRECT3DDEVICE9			m_pDevice;
	LPD3DXFONT					m_pFont;
	LPD3DXSPRITE				m_pSprite;
public:
	static	CFont*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
							const _tchar* pFontType,
							const _uint& iWidth,
							const _uint& iHeight,
							const _uint& iWeight);
private:
	virtual void	Free(void);
};

_END


#endif // Font_h__
