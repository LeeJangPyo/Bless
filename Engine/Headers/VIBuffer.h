#ifndef VIBuffer_h__
#define VIBuffer_h__


#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL	CVIBuffer  abstract : public CComponent
{
public:
	enum BUFFERID
	{
		BUFFER_RCTRI,BUFFER_RCCOL,BUFFER_RCTEX,BUFFER_TERRAIN,BUFFER_CUBE, BUFFER_SPHERECOL
	};

protected:
	explicit	CVIBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit	CVIBuffer(const CVIBuffer& rhs);
	virtual		~CVIBuffer(void) = default;

public:
	_ulong		Get_TriCnt(void) { return m_dwTriCnt; }

public:
	HRESULT		Ready_Buffer(void);
	void		Render_Buffer(void);


protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;


	DWORD			m_dwVtxSize;
	DWORD			m_dwVtxCnt;
	DWORD			m_dwVtxFVF;
	DWORD			m_dwTriCnt;

	DWORD					m_dwIdxSize;
	D3DFORMAT				m_IdxFmt;


public:
	virtual CComponent* Clone_Component() = 0;

protected:
	virtual void Free(void);

};


_END

#endif // VIBuffer_h__
