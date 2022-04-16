
#ifndef State_h__
#define State_h__

#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL CState abstract : public CBase
{
public:
	explicit CState();
	virtual ~CState() = default;

public:
	virtual HRESULT	Begin() = 0;
	virtual _int	Update(const _float& fTimeDelta) = 0;
	virtual _int	LastUpdate(const _float& fTimeDelta) = 0;
	virtual	HRESULT	End() = 0;
protected:
	virtual void Free() = 0;

};

_END

#endif // State_h__
