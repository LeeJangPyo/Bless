#pragma once

#include "Base.h"

_BEGIN(Engine)

class CFrame;
class _ENGINE_DLL CFrame_Manager : public CBase
{
	_DECLARE_SINGLETON(CFrame_Manager)
private:
	explicit CFrame_Manager();
	virtual ~CFrame_Manager() = default;
public:
	HRESULT Add_Frame(const _tchar* pFrameTag, const _float& fCallCnt);
	_bool Permit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
private:
	map<const _tchar*, CFrame*>			m_mapFrame;
	typedef map<const _tchar*, CFrame*>	MAPFRAME;
private:
	CFrame* Find_Frame(const _tchar* pFrameTag);
public:
	virtual void Free(void);

};

_END