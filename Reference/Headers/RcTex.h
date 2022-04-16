#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

_BEGIN(Engine)

class _ENGINE_DLL CRcTex final : public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex(void);

public:
	HRESULT	Ready_Buffer(void);

public:
	static CRcTex*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone_Component() override;
protected:
	virtual void Free() override;

};

_END
#endif // RcTex_h__
