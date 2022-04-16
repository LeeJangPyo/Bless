
#ifndef Buffer_CubeTex_h__
#define Buffer_CubeTex_h__

#include "VIBuffer.h"

_BEGIN(Engine)
class _ENGINE_DLL CBuffer_CubeTex final : public CVIBuffer
{
private:
	explicit CBuffer_CubeTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CBuffer_CubeTex(const CBuffer_CubeTex& rhs);
	virtual ~CBuffer_CubeTex(void);


public:
	HRESULT	Ready_Buffer(void);

public:
	static	CBuffer_CubeTex*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone_Component() override;
protected:
	virtual void Free() override;
};

_END

#endif // Buffer_CubeTex_h__