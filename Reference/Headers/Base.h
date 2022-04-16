#pragma once

#include "Engine_Defines.h"

_BEGIN(Engine)

class _ENGINE_DLL CBase abstract
{
protected:
	explicit CBase();
	virtual ~CBase() = default;
public:
	// ���� : �����ǰ� �� ������ ���������� ����.
	// ���������� ������Ų��.
	_ulong AddRef();

	// ���� : ���ҵǱ� ���� ���������� ����.
	// ���������� ���ҽ�Ų�� or �����Ѵ�
	_ulong Release();
private:
	_ulong		m_dwRefCnt = 0;
protected:
	virtual void Free() = 0;
};

_END