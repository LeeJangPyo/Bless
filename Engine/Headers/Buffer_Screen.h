#ifndef Buffer_Screen_h__
#define Buffer_Screen_h__


#include "VIBuffer.h"
_BEGIN(Engine)

class CBuffer_Screen final : public CVIBuffer
{
public:
	explicit CBuffer_Screen(LPDIRECT3DDEVICE9 pDevice);
	explicit CBuffer_Screen(const CBuffer_Screen& rhs);
	virtual ~CBuffer_Screen(void) = default;

public:
	HRESULT	Ready_Buffer(void);
	HRESULT	Ready_Buffer(const _float& fStartX, const _float& fStartY, const _float& fSizeX, const _float& fSizeY);

public:
	static CBuffer_Screen*	Create(LPDIRECT3DDEVICE9 pDevice);
	static CBuffer_Screen*	Create(LPDIRECT3DDEVICE9 pDevice, const _float& fStartX, const _float& fStartY, const _float& fSizeX, const _float& fSizeY);
	virtual CComponent* Clone_Component() override;
protected:
	virtual void Free() override;
};

_END

#endif // Buffer_Screen_h__