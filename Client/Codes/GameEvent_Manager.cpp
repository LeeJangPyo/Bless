#include "stdafx.h"
#include "..\Headers\GameEvent_Manager.h"

#include "Management.h"

#include "Player_Manager.h"

//Stage1
#include "GameEvent_Opening.h"
#include "GameEvent_Stage1_Combat.h"
#include "GameEvent_Stage1_Combat_End.h"
#include "GameEvent_Stage1_Boss.h"
#include "GameEvent_Stage1_Boss_End.h"

//Stage2
#include "GameEvent_Stage2_Opening.h"
#include "GameEvent_Stage2_Quatra.h"
#include "GameEvent_Stage2_Flogas.h"


_USING(Client)

_IMPLEMENT_SINGLETON(CGameEvent_Manager)

Client::CGameEvent_Manager::CGameEvent_Manager()
	: m_pCurrent_GameEvent(nullptr)
	, m_eStage(STAGE1)
{

}

HRESULT CGameEvent_Manager::Ready_GameEvent()
{
	//Create Stage1 Event
	CGameEvent* pGameEvent = CGameEvent_Opening::Create(L"Opening");
	if (pGameEvent == nullptr)
		return E_FAIL;
	m_GameEventList.push_back(pGameEvent);

	pGameEvent = CGameEvent_Stage1_Combat::Create(L"Stage1_Combat");
	if (pGameEvent == nullptr)
		return E_FAIL;
	m_GameEventList.push_back(pGameEvent);

	pGameEvent = CGameEvent_Stage1_Combat_End::Create(L"Stage1_Combat_End");
	if (pGameEvent == nullptr)
		return E_FAIL;
	m_GameEventList.push_back(pGameEvent);

	pGameEvent = CGameEvent_Stage1_Boss::Create(L"Stage1_Boss");
	if (pGameEvent == nullptr)
		return E_FAIL;
	m_GameEventList.push_back(pGameEvent);


	pGameEvent = CGameEvent_Stage1_Boss_End::Create(L"Stage1_Boss_End");
	if (pGameEvent == nullptr)
		return E_FAIL;
	m_GameEventList.push_back(pGameEvent);


	//Create Stage2 Event
	pGameEvent = CGameEvent_Stage2_Opening::Create(L"Stage2_Opening");
	if (pGameEvent == nullptr)
		return E_FAIL;
	m_GameEventList.push_back(pGameEvent);
	
	pGameEvent = CGameEvent_Stage2_Quatra::Create(L"Stage2_Quatra");
	if (pGameEvent == nullptr)
		return E_FAIL;
	m_GameEventList.push_back(pGameEvent);

	pGameEvent = CGameEvent_Stage2_Flogas::Create(L"Stage2_Flogas");
	if (pGameEvent == nullptr)
		return E_FAIL;
	m_GameEventList.push_back(pGameEvent);

	return S_OK;
}

void CGameEvent_Manager::Update_GameEvent(const _float & fTimeDelta)
{

	//이벤트 업데이트
	if (m_pCurrent_GameEvent != nullptr)
	{
		if(m_pCurrent_GameEvent->Update_Event() == -1)
		{
			auto iter = find_if(m_GameEventList.begin(), m_GameEventList.end(), [&](CGameEvent *pEvent){
				if (pEvent == m_pCurrent_GameEvent)
					return true;
				return false;
			});
			m_GameEventList.erase(iter);
			Safe_Release(m_pCurrent_GameEvent);
		}
	}

	if (m_eStage == STAGE1)
	{
		//Event 
		if (CPlayer_Manager::GetInstance()->Get_CurrentPlayer()->Get_NaviOnIndex() == 8)
		{
			m_pCurrent_GameEvent = Find_Event(L"Opening");
			if (m_pCurrent_GameEvent == nullptr || m_pCurrent_GameEvent->Get_IsUsed())
				return;
			m_pCurrent_GameEvent->Start_Event();
			return;
		}

		if (CPlayer_Manager::GetInstance()->Get_CurrentPlayer()->Get_NaviOnIndex() == 20)
		{
			m_pCurrent_GameEvent = Find_Event(L"Stage1_Combat");
			if (m_pCurrent_GameEvent == nullptr || m_pCurrent_GameEvent->Get_IsUsed())
				return;
			m_pCurrent_GameEvent->Start_Event();
			return;
		}

		if (CPlayer_Manager::GetInstance()->Get_CurrentPlayer()->Get_NaviOnIndex() == 40)
		{
			m_pCurrent_GameEvent = Find_Event(L"Stage1_Boss");
			if (m_pCurrent_GameEvent == nullptr || m_pCurrent_GameEvent->Get_IsUsed())
				return;
			m_pCurrent_GameEvent->Start_Event();
			return;
		}

		if (CPlayer_Manager::GetInstance()->Get_CombatState() == COMBAT)
		{
			const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_Stage1_Combat");
			if (pGameObjectList == nullptr)
				return;

			if (pGameObjectList->size() == 0)
			{
				m_pCurrent_GameEvent = Find_Event(L"Stage1_Combat_End");
				if (m_pCurrent_GameEvent == nullptr || m_pCurrent_GameEvent->Get_IsUsed())
					return;
				m_pCurrent_GameEvent->Start_Event();
				return;
			}

			pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_Stage1_Boss");
			if (pGameObjectList == nullptr)
				return;

			if (pGameObjectList->size() == 0)
			{
				m_pCurrent_GameEvent = Find_Event(L"Stage1_Boss_End");
				if (m_pCurrent_GameEvent == nullptr || m_pCurrent_GameEvent->Get_IsUsed())
					return;
				m_pCurrent_GameEvent->Start_Event();
				return;
			}
		}
		
	}
	else
	{
		//이벤트 조건문
		if (CPlayer_Manager::GetInstance()->Get_CurrentPlayer()->Get_NaviOnIndex() == 8)
		{
			m_pCurrent_GameEvent = Find_Event(L"Stage2_Opening");
			if (m_pCurrent_GameEvent == nullptr || m_pCurrent_GameEvent->Get_IsUsed())
				return;

			m_pCurrent_GameEvent->Start_Event();
			return;
		}
		//이벤트 조건문
		if (CPlayer_Manager::GetInstance()->Get_CurrentPlayer()->Get_NaviOnIndex() == 62)
		{
			m_pCurrent_GameEvent = Find_Event(L"Stage2_Flogas");
			if (m_pCurrent_GameEvent == nullptr || m_pCurrent_GameEvent->Get_IsUsed())
				return;
			m_pCurrent_GameEvent->Start_Event();
			return;
		}


		if (CPlayer_Manager::GetInstance()->Get_CombatState() == COMBAT)
		{
			const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_Stage2_Combat");
			if (pGameObjectList == nullptr)
				return;

			if (pGameObjectList->size() == 0)
			{
				m_pCurrent_GameEvent = Find_Event(L"Stage2_Quatra");
				if (m_pCurrent_GameEvent == nullptr || m_pCurrent_GameEvent->Get_IsUsed())
					return;
				m_pCurrent_GameEvent->Start_Event();
				return;
			}
		}
	}
	
}

CGameEvent * CGameEvent_Manager::Find_Event(const _tchar * pEventTag)
{
	for (auto& pGameEvent : m_GameEventList)
	{
		if (pGameEvent->Get_CompareTag(pEventTag))
			return pGameEvent;
	}
	return nullptr;
}

void CGameEvent_Manager::Free()
{
	for (auto& pGameEvent : m_GameEventList)
		Safe_Release(pGameEvent);

	m_GameEventList.clear();
}

