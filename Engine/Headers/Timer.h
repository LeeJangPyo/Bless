#pragma once

#include "Base.h"

_BEGIN(Engine)

// 한 프레임당 타임델타를 계산한다.

class CTimer : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() = default;
public:
	HRESULT Ready_Timer(void);
	_float Get_TimeDelta();
private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_CpuTick;
private:
	_float					m_fTimeDelta = 0.f;
public:
	static CTimer* Create(void);
	virtual void Free(void);
};

_END