#ifndef Line_Manager_h__
#define Line_Manager_h__

#include "Base.h"


_BEGIN(Engine)

class CLine;
class _ENGINE_DLL CLine_Manager : public CBase
{
	_DECLARE_SINGLETON(CLine_Manager)

private:
	explicit		CLine_Manager(void);
	virtual			~CLine_Manager(void);

public:
	HRESULT			Ready_Line(LPDIRECT3DDEVICE9 pDevice,
								const _tchar* pLineTag,
								const _float& fWidth = 1.f,
								const _bool& bAntiAlias = true,
								const _bool& bGLLines = false,
								const _long& dwPattern = 0,
								const _float& fPatScale = 1.f);

	void			Render_Line(const _tchar* pLineTag, 
								 _vec3* pVertexArray,
								const _long& dwArrayCount,
								const D3DXCOLOR& Color);

private:
	CLine*		Find_Line(const _tchar* pLineTag);

private:
	map<const _tchar*, CLine*>		m_mapLine;

public:
	virtual		void	Free(void);

};

_END

#endif // Line_Manager_h__
