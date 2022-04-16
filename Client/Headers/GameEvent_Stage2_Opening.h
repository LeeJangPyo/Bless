
#ifndef GameEvent_Stage2_Opening_h__
#define GameEvent_Stage2_Opening_h__

#include "GameEvent.h"

_BEGIN(Client)

class CCamera_Dynamic;
class CCamera_CutScene;

class CGameEvent_Stage2_Opening final : public CGameEvent
{
public:
	explicit CGameEvent_Stage2_Opening();
	virtual ~CGameEvent_Stage2_Opening()  = default;
	

public:
	virtual void Start_Event() override;
	virtual _int Update_Event() override;
	virtual void End_Event() override;
	
private:
	HRESULT		Ready_GameEvent(const _tchar* pEventTag);


	CCamera_CutScene* m_pCamera_CutScene;
public:
	static CGameEvent*	Create(const _tchar* pEventTag);

protected:
	virtual	void	Free() override;


};

_END
#endif // Event_Stage2_Opening_h__