#include "stdafx.h"
#include "GameEvent_Stage1_Boss_End.h"


#include "Management.h"
#include "Player_Manager.h"

#include "Camera_Dynamic.h"
#include "Camera_CutScene.h"

#include "GameObject_Dynamic.h"
#include "UI_Frame.h"

#include "Sound_Manager.h"

_USING(Client)

CGameEvent_Stage1_Boss_End::CGameEvent_Stage1_Boss_End()
	:m_pCamera_CutScene(nullptr)
{
}

void CGameEvent_Stage1_Boss_End::Start_Event()
{
	CPlayer_Manager::GetInstance()->Set_CombatState(GENERAL);

	CPlayer_Manager::GetInstance()->Set_CharacterState(PLAYER_LUPS, L"General");
	CPlayer_Manager::GetInstance()->Set_CharacterState(PLAYER_SIRE, L"AI");


	CPlayer_Manager::GetInstance()->Get_Player(PLAYER_LUPS)->Compute_Position_OnNaviMeshIndex();
	CPlayer_Manager::GetInstance()->Get_Player(PLAYER_SIRE)->Compute_Position_OnNaviMeshIndex();

	m_bIsUsed = true;
}

_int CGameEvent_Stage1_Boss_End::Update_Event()
{

	End_Event();
	return -1;
}

void CGameEvent_Stage1_Boss_End::End_Event()
{
	//Sound
	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BGM);
	CSound_Manager::GetInstance()->Play_BGM(L"Stage1_BGM.mp3");
}

HRESULT CGameEvent_Stage1_Boss_End::Ready_GameEvent(const _tchar * pEventTag)
{
	m_wstrEventTag = pEventTag;
	return S_OK;
}

CGameEvent * CGameEvent_Stage1_Boss_End::Create(const _tchar * pEventTag)
{
	CGameEvent_Stage1_Boss_End*	pInstance = new CGameEvent_Stage1_Boss_End();

	if (FAILED(pInstance->Ready_GameEvent(pEventTag)))
	{
		_MSG_BOX(L"CGameEvent_Stage1_Boss_End Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
