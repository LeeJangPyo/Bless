#pragma once

#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pDevice);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;
protected:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	_bool					m_isClone = false;
public:
	virtual CComponent* Clone_Component() = 0;
protected:
	virtual void Free();
};

_END