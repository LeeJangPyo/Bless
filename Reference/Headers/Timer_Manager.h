#pragma once

#include "Base.h"

// CTimer객체들을 여러개 모아서 보관 관리한다.

_BEGIN(Engine)

class CTimer;
class _ENGINE_DLL CTimer_Manager : public CBase
{
	_DECLARE_SINGLETON(CTimer_Manager)
private:
	explicit CTimer_Manager();
	virtual ~CTimer_Manager() = default;
public:
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float Get_TimeDelta(const _tchar* pTimerTag);
private:
	map<const _tchar*, CTimer*>			m_mapTimers;
	typedef map<const _tchar*, CTimer*>	MAPTIMERS;
private:
	CTimer* Find_Timer(const _tchar* pTimerTag);
public:
	virtual void Free(void);
};

_END