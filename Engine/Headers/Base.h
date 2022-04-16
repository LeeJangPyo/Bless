#pragma once

#include "Engine_Defines.h"

_BEGIN(Engine)

class _ENGINE_DLL CBase abstract
{
protected:
	explicit CBase();
	virtual ~CBase() = default;
public:
	// 리턴 : 증가되고 난 이후의 참조갯수를 리턴.
	// 참조갯수를 증가시킨다.
	_ulong AddRef();

	// 리턴 : 감소되기 전의 참조갯수를 리턴.
	// 참조갯수를 감소시킨다 or 삭제한다
	_ulong Release();
private:
	_ulong		m_dwRefCnt = 0;
protected:
	virtual void Free() = 0;
};

_END