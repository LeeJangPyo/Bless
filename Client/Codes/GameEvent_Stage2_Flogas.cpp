#include "stdafx.h"
#include "GameEvent_Stage2_Flogas.h"

#include "Management.h"
#include "Player_Manager.h"

#include "Camera_Dynamic.h"
#include "Camera_CutScene.h"

#include "GameObject_Dynamic.h"
#include "UI_Frame.h"



_USING(Client)

CGameEvent_Stage2_Flogas::CGameEvent_Stage2_Flogas()
	:m_pCamera_CutScene(nullptr)
{
}
HRESULT CGameEvent_Stage2_Flogas::Ready_GameEvent(const _tchar * pEventTag)
{
	m_wstrEventTag = pEventTag;
	return S_OK;
}

void CGameEvent_Stage2_Flogas::Start_Event()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();
	{	
		pManagement->Set_CameraMain(CAMERA_CUTSCENE, L"Stage2_Flogas", (CCamera**)&m_pCamera_CutScene);

		//Monster State Set
		const list<CGameObject*>* pGameObjectList = pManagement->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_Stage2_Flogas");
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


		//Breath 
		CGameObject_Dynamic* pQuatra = dynamic_cast<CGameObject_Dynamic*>(pManagement->Get_GameObject(SCENE_STAGE, L"Layer_Stage2_Quatra"));
		pQuatra->Change_State(L"Breath_End");


	}
	Safe_Release(pManagement);

	m_bIsUsed = true;
}

_int CGameEvent_Stage2_Flogas::Update_Event()
{
	if (m_pCamera_CutScene->Get_IsSceneEnd())
	{
		End_Event();
		return -1;
	}
	return 0;
}

void CGameEvent_Stage2_Flogas::End_Event()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();
	{
		pManagement->Set_CameraMain(CAMERA_STATIC, L"Camera_Static");

		const list<CGameObject*>* pGameObjectList = pManagement->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_Stage2_Flogas");

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

		CPlayer_Manager::GetInstance()->Set_Camera(PLAYERCAMERA_DEFAULT);

		CPlayer_Manager::GetInstance()->Set_CombatState(COMBAT);

		CPlayer_Manager::GetInstance()->Set_CharacterState(PLAYER_LUPS, L"Ready_Combat");
		CPlayer_Manager::GetInstance()->Set_CharacterState(PLAYER_SIRE, L"Ready_Combat");

		CPlayer_Manager::GetInstance()->Get_Player(PLAYER_LUPS)->Compute_Position_OnNaviMeshIndex();
		CPlayer_Manager::GetInstance()->Get_Player(PLAYER_SIRE)->Compute_Position_OnNaviMeshIndex();
	}
	Safe_Release(pManagement);
}



CGameEvent * CGameEvent_Stage2_Flogas::Create(const _tchar * pEventTag)
{
	CGameEvent_Stage2_Flogas*	pInstance = new CGameEvent_Stage2_Flogas();

	if (FAILED(pInstance->Ready_GameEvent(pEventTag)))
	{
		_MSG_BOX(L"CGameEvent_Stage2_Flogas Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


