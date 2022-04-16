#ifndef Buffer_RcTex_h__
#define Buffer_RcTex_h__

#include "VIBuffer.h"

_BEGIN(Engine)

class _ENGINE_DLL CBuffer_RcTex final : public CVIBuffer
{
private:
	explicit CBuffer_RcTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CBuffer_RcTex(const CBuffer_RcTex& rhs);
	virtual ~CBuffer_RcTex(void);

public:
	HRESULT	Ready_Buffer(void);

public:
	static CBuffer_RcTex*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone_Component() override;
protected:
	virtual void Free() override;

};

_END
#endif // Buffer_RcTex_h__
