#ifndef GameEvent_Manager_h__
#define GameEvent_Manager_h__

#include "Base.h"
#include "GameEvent.h"

_BEGIN(Client)
class CLups;
class CGameEvent_Manager final	: public CBase
{
	_DECLARE_SINGLETON(CGameEvent_Manager)
public:
	explicit CGameEvent_Manager();
	virtual ~CGameEvent_Manager() = default;


public:
	HRESULT	Ready_GameEvent();
	void	Update_GameEvent(const _float& fTimeDelta);
	
	void	Set_Stage(STAGE eStage) { 
		m_eStage = eStage; }
	const STAGE&	Get_Stage() {
		return m_eStage;	}
private:
	CGameEvent*		Find_Event(const _tchar* pEventTag);
private:
	list<CGameEvent*> m_GameEventList;
	
	CGameEvent*		m_pCurrent_GameEvent;

	STAGE	m_eStage;


protected:
	virtual void Free() override;
};

_END
#endif // Event_Manager_h__

