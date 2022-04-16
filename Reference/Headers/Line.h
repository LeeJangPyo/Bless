#ifndef Line_h__
#define Line_h__

#include "Base.h"


_BEGIN(Engine)

class _ENGINE_DLL CLine : public CBase
{
private:
	explicit	 CLine(LPDIRECT3DDEVICE9 pDevice);
	virtual		 ~CLine(void);

public:
	HRESULT		Ready_Line(const _float& fWidth,
							const _bool& bAntiAlias,
							const _bool& bGLLines,
							const _long& dwPattern,
							const _float& fPatScale);

	void		Render_Line(_vec3* pVertexArray,
							const _long& dwArrayCount,
							const D3DXCOLOR& Color);


private:
	LPDIRECT3DDEVICE9			m_pDevice;
	LPD3DXLINE					m_pLine;

	_vec3						m_pBuffer[MIN_PATH];

public:
	static	CLine*		Create(LPDIRECT3DDEVICE9 pDevice,
		const _float& fWidth = 1.f,
		const _bool& bAntiAlias = true,	
		const _bool& bGLLines = false,
		const _long& dwPattern = 0,
		const _float& fPatScale = 1.f);
private:
	virtual void	Free(void) override;
};


_END



#endif // Line_h__
