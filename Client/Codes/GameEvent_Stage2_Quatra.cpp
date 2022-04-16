#include "stdafx.h"
#include "GameEvent_Stage2_Quatra.h"

#include "Management.h"

#include "Camera_Dynamic.h"
#include "Camera_CutScene.h"

#include "GameObject_Dynamic.h"
#include "UI_Frame.h"

#include "Player_Manager.h"

_USING(Client)

CGameEvent_Stage2_Quatra::CGameEvent_Stage2_Quatra()
	:m_pCamera_CutScene(nullptr)
{
}
HRESULT CGameEvent_Stage2_Quatra::Ready_GameEvent(const _tchar * pEventTag)
{
	m_wstrEventTag = pEventTag;
	return S_OK;
}

void CGameEvent_Stage2_Quatra::Start_Event()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();
	{	
		pManagement->Set_CameraMain(CAMERA_CUTSCENE, L"Stage2_Quatra", (CCamera**)&m_pCamera_CutScene);

		//Monster State Set
		const list<CGameObject*>* pGameObjectList = pManagement->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_Stage2_Quatra");
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

_int CGameEvent_Stage2_Quatra::Update_Event()
{
	if (m_pCamera_CutScene->Get_IsSceneEnd())
	{
		End_Event();
		return -1;
	}
	return 0;
}

void CGameEvent_Stage2_Quatra::End_Event()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();
	{
		pManagement->Set_CameraMain(CAMERA_STATIC, L"Camera_Static");

		const list<CGameObject*>* pGameObjectList = pManagement->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_Stage2_Quatra");

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


		CPlayer_Manager::GetInstance()->Set_Camera(PLAYERCAMERA_MIDDLE);

		CPlayer_Manager::GetInstance()->Set_CombatState(GENERAL);

		CPlayer_Manager::GetInstance()->Set_CharacterState(PLAYER_LUPS, L"End_Combat");
		CPlayer_Manager::GetInstance()->Set_CharacterState(PLAYER_SIRE, L"End_Combat");


		CPlayer_Manager::GetInstance()->Get_Player(PLAYER_LUPS)->Compute_Position_OnNaviMeshIndex();
		CPlayer_Manager::GetInstance()->Get_Player(PLAYER_SIRE)->Compute_Position_OnNaviMeshIndex();
	}
	Safe_Release(pManagement);
}



CGameEvent * CGameEvent_Stage2_Quatra::Create(const _tchar * pEventTag)
{
	CGameEvent_Stage2_Quatra*	pInstance = new CGameEvent_Stage2_Quatra();

	if (FAILED(pInstance->Ready_GameEvent(pEventTag)))
	{
		_MSG_BOX(L"CGameEvent_Stage2_Quatra Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


