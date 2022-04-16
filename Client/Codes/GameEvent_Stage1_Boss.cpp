#include "stdafx.h"
#include "GameEvent_Stage1_Boss.h"


#include "Management.h"

#include "Camera_Dynamic.h"
#include "Camera_CutScene.h"

#include "GameObject_Dynamic.h"
#include "UI_Frame.h"

#include "Player_Manager.h"
#include "Sound_Manager.h"

_USING(Client)

CGameEvent_Stage1_Boss::CGameEvent_Stage1_Boss()
	:m_pCamera_CutScene(nullptr)
{
}

void CGameEvent_Stage1_Boss::Start_Event()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();
	{
		pManagement->Set_CameraMain(CAMERA_CUTSCENE, L"Stage1_Boss", (CCamera**)&m_pCamera_CutScene);

		//Monster State Set
		const list<CGameObject*>* pGameObjectList = pManagement->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_Stage1_Boss");
		if (pGameObjectList == nullptr)
			return;

		for (auto& pGameObject : *pGameObjectList)
		{
			CGameObject_Dynamic* pMonster = dynamic_cast<CGameObject_Dynamic*>(pGameObject);
			pMonster->Change_State(L"Ready_Combat");
		}

		//Player UI Set
		pGameObjectList = pManagement->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_UI_Lups");
		if (pGameObjectList == nullptr)
			return;

		for (auto& pGameObject : *pGameObjectList)
		{
			CUI_Frame* pUIFrame = dynamic_cast<CUI_Frame*>(pGameObject);
			pUIFrame->Set_Activate(false);
		}

	}
	Safe_Release(pManagement);

	m_bIsUsed = true;
}

_int CGameEvent_Stage1_Boss::Update_Event()
{
	if (m_pCamera_CutScene->Get_IsSceneEnd())
	{
		End_Event();
		return -1;
	}
	return 0;
}

void CGameEvent_Stage1_Boss::End_Event()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();
	{
		pManagement->Set_CameraMain(CAMERA_STATIC, L"Camera_Static");

		const list<CGameObject*>* pGameObjectList = pManagement->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_Stage1_Boss");

		for (auto& pGameObject : *pGameObjectList)
		{
			CGameObject_Dynamic* pMonster = dynamic_cast<CGameObject_Dynamic*>(pGameObject);
			pMonster->Change_State(L"Idle");
		}

		//Player UI Set
		pGameObjectList = pManagement->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_UI_Lups");
		if (pGameObjectList == nullptr)
			return;

		for (auto& pGameObject : *pGameObjectList)
		{
			CUI_Frame* pUIFrame = dynamic_cast<CUI_Frame*>(pGameObject);
			pUIFrame->Set_Activate(true);
		}


		//Set State
		CPlayer_Manager::GetInstance()->Set_CombatState(COMBAT);

		CPlayer_Manager::GetInstance()->Set_CharacterState(PLAYER_LUPS, L"Ready_Combat");
		CPlayer_Manager::GetInstance()->Set_CharacterState(PLAYER_SIRE, L"Ready_Combat");

		CPlayer_Manager::GetInstance()->Get_Player(PLAYER_LUPS)->Compute_Position_OnNaviMeshIndex();
		CPlayer_Manager::GetInstance()->Get_Player(PLAYER_SIRE)->Compute_Position_OnNaviMeshIndex();

		//Sound
		CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BGM);
		CSound_Manager::GetInstance()->Play_BGM(L"Combat_BGM.mp3");
	}
	Safe_Release(pManagement);
}

HRESULT CGameEvent_Stage1_Boss::Ready_GameEvent(const _tchar * pEventTag)
{
	m_wstrEventTag = pEventTag;
	return S_OK;
}

CGameEvent * CGameEvent_Stage1_Boss::Create(const _tchar * pEventTag)
{
	CGameEvent_Stage1_Boss*	pInstance = new CGameEvent_Stage1_Boss();

	if (FAILED(pInstance->Ready_GameEvent(pEventTag)))
	{
		_MSG_BOX(L"CGameEvent_Stage1_Boss Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
