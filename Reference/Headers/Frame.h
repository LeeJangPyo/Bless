#pragma once

#include "Base.h"

// CFrame : 1초에 몇번 트루리턴하면 될지에 대한 기능을 구현하는 함수를 가지고 있는 클래스ㅏ

_BEGIN(Engine)

class CFrame : public CBase
{
private:
	explicit CFrame();
	virtual ~CFrame() = default;
public:
	HRESULT Ready_Frame(const _float& fCallPerSec);
	bool Permit_Call(const _float& fTimeDelta);
private:
	_float			m_fTimeAcc = 0.f;
	_float			m_fInterval = 0.f;
public:
	static CFrame* Create(const _float& fCallPerSec);
	virtual void Free(void);
};

_END