
#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

_BEGIN(Engine)
class _ENGINE_DLL CCubeTex final : public CVIBuffer
{
private:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex(void);


public:
	HRESULT	Ready_Buffer(void);

public:
	static	CCubeTex*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone_Component() override;
protected:
	virtual void Free() override;
};

_END

#endif // CubeTex_h__