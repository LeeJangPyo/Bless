
#ifndef GameEvent_Stage2_Flogas_h__
#define GameEvent_Stage2_Flogas_h__

#include "GameEvent.h"

_BEGIN(Client)
class CCamera_Dynamic;
class CCamera_CutScene;

class CGameEvent_Stage2_Flogas final :public CGameEvent
{
public:
	explicit CGameEvent_Stage2_Flogas();
	virtual ~CGameEvent_Stage2_Flogas() = default;

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

#endif // GameEvent_Stage2_Flogas_h__