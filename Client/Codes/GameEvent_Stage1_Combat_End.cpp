#include "stdafx.h"
#include "GameEvent_Stage1_Combat_End.h"

#include "Management.h"

#include "Camera_Dynamic.h"
#include "Camera_CutScene.h"

#include "GameObject_Dynamic.h"
#include "UI_Frame.h"
#include "Player_Manager.h"

#include "Sound_Manager.h"
_USING(Client)

CGameEvent_Stage1_Combat_End::CGameEvent_Stage1_Combat_End()
	:m_pCamera_CutScene(nullptr)
{
}
HRESULT CGameEvent_Stage1_Combat_End::Ready_GameEvent(const _tchar * pEventTag)
{
	m_wstrEventTag = pEventTag;
	return S_OK;
}

void CGameEvent_Stage1_Combat_End::Start_Event()
{
	CPlayer_Manager::GetInstance()->Set_CombatState(GENERAL);

	CPlayer_Manager::GetInstance()->Set_CharacterState(PLAYER_LUPS, L"End_Combat");
	CPlayer_Manager::GetInstance()->Set_CharacterState(PLAYER_SIRE, L"End_Combat");


	CPlayer_Manager::GetInstance()->Get_Player(PLAYER_LUPS)->Compute_Position_OnNaviMeshIndex();
	CPlayer_Manager::GetInstance()->Get_Player(PLAYER_SIRE)->Compute_Position_OnNaviMeshIndex();
	m_bIsUsed = true;
}

_int CGameEvent_Stage1_Combat_End::Update_Event()
{
	End_Event();
	return -1;
}

void CGameEvent_Stage1_Combat_End::End_Event()
{
	//Sound
	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BGM);
	CSound_Manager::GetInstance()->Play_BGM(L"Stage1_BGM.mp3");
		
}



CGameEvent * CGameEvent_Stage1_Combat_End::Create(const _tchar * pEventTag)
{
	CGameEvent_Stage1_Combat_End*	pInstance = new CGameEvent_Stage1_Combat_End();

	if (FAILED(pInstance->Ready_GameEvent(pEventTag)))
	{
		_MSG_BOX(L"CGameEvent_Stage1_Combat_End Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


