#ifndef Font_Manager_h__
#define Font_Manager_h__

#include "Base.h"
#include "Font.h"

_BEGIN(Engine)

class _ENGINE_DLL CFont_Manager : public CBase
{
	_DECLARE_SINGLETON(CFont_Manager)

private:
	explicit		CFont_Manager(void);
	virtual			~CFont_Manager(void) = default;

public:
	HRESULT			Ready_Font(LPDIRECT3DDEVICE9 pDevice,
								const _tchar* pFontTag,
								const _tchar* pFontType,
								const _uint& iWidth,
								const _uint& iWeight,
								const _uint& iHeight);

	void			Render_Font(const _tchar* pFontTag,
								const _tchar* pString,
								const _vec2* pPosition,
								D3DXCOLOR Color);

	void			Render_Font(const _tchar* pFontTag,
								const _tchar* pString,
								const _vec3* pPosition,
								D3DXCOLOR Color);

private:
	CFont*		Find_Font(const _tchar* pFontTag);

private:
	map<const _tchar*, CFont*>		m_mapFont;

public:
	virtual		void	Free(void);

};

_END

#endif // Font_Manager_h__
