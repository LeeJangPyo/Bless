#ifndef GameEvent_h__
#define GameEvent_h__

#include "Defines.h"
#include "Base.h"

_BEGIN(Client)

class CGameEvent abstract : public CBase
{
protected:
	explicit	CGameEvent();
	virtual		~CGameEvent() = default;

public:
	_bool	Get_CompareTag(const _tchar* pEventTag);
	_bool	Get_IsUsed() { return m_bIsUsed; }
public:
	virtual	void	Start_Event() = 0;
	virtual _int	Update_Event() = 0;
	virtual	void	End_Event() = 0;
	
protected:
	wstring			m_wstrEventTag;
	_bool			m_bIsUsed;
protected:
	virtual	void	Free() override;

};

_END

#endif // GameEvent_h__