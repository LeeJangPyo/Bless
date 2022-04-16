
#ifndef GameEvent_Stage1_Combat_End_h__
#define GameEvent_Stage1_Combat_End_h__

#include "GameEvent.h"

_BEGIN(Client)
class CCamera_Dynamic;
class CCamera_CutScene;

class CGameEvent_Stage1_Combat_End final :public CGameEvent
{
public:
	explicit CGameEvent_Stage1_Combat_End();
	virtual ~CGameEvent_Stage1_Combat_End() = default;

public:
	virtual void Start_Event() override;
	virtual _int Update_Event() override;
	virtual void End_Event() override;
private:
	HRESULT		Ready_GameEvent(const _tchar* pEventTag);

	CCamera_CutScene* m_pCamera_CutScene;
public:
	static CGameEvent*	Create(const _tchar* pEventTag);
};


_END

#endif // GameEvent_Stage1_Combat_End_h__