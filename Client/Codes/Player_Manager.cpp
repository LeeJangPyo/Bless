#include "stdafx.h"
#include "..\Headers\Player_Manager.h"

#include "Management.h"

_USING(Client)

_IMPLEMENT_SINGLETON(CPlayer_Manager)

CPlayer_Manager::CPlayer_Manager()
	:m_eCurrentPlayer(PLAYER_LUPS)
	,m_eCurrentCamera(PLAYERCAMERA_DEFAULT)
	,m_eCurrentCombatState(GENERAL)
{

}

void CPlayer_Manager::Set_Character(const PLAYERTYPE& eType)
{
	//UI Activate False
	m_arrPlayer[m_eCurrentPlayer]->Set_UI_Activate(m_eCurrentPlayer,false);

	m_eCurrentPlayer = eType;
	m_arrPlayer[m_eCurrentPlayer]->Set_ControlType(CONTROL_PLAYER);

	m_arrPlayer[m_eCurrentPlayer]->Set_UI_Activate(m_eCurrentPlayer, true);
}

void CPlayer_Manager::Set_Camera(PLAYERCAMERA_TYPE eType)
{
	m_eCurrentCamera = eType;
}

void CPlayer_Manager::Set_CombatState(COMBATSTATE eState)
{
	m_eCurrentCombatState = eState;
}

void CPlayer_Manager::Set_CharacterState(const PLAYERTYPE& eType, const _tchar * pStateTag)
{
	m_arrPlayer[eType]->Change_State(pStateTag);
}



void CPlayer_Manager::Change_Player_NaviMesh()
{
	for (auto& pPlayer : m_arrPlayer)
		pPlayer->Change_NavigationMesh();
}

const _vec3* CPlayer_Manager::Get_Camera_Eye_Position()
{
	return m_arrPlayer[m_eCurrentPlayer]->Get_Camera_Eye_Position(m_eCurrentCamera);
}

const _vec3 * CPlayer_Manager::Get_Camera_At_Position()
{
	return m_arrPlayer[m_eCurrentPlayer]->Get_Camera_At_Position();
}

const _vec3 * CPlayer_Manager::Get_Player_Position()
{
	return  m_arrPlayer[m_eCurrentPlayer]->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
}

const _vec3 * CPlayer_Manager::Get_Player_Position(const PLAYERTYPE & eType)
{
	return  m_arrPlayer[eType]->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
}

COMBATSTATE CPlayer_Manager::Get_CombatState()
{
	return m_eCurrentCombatState;
}

CGameObject_Player * CPlayer_Manager::Get_Player(const PLAYERTYPE & eType)
{
	return m_arrPlayer[eType];
}

CGameObject_Player * CPlayer_Manager::Get_CurrentPlayer()
{
	return m_arrPlayer[m_eCurrentPlayer];
}

const PLAYERTYPE & CPlayer_Manager::Get_PlayerType()
{
	// TODO: insert return statement here
	return m_eCurrentPlayer;
}


_float CPlayer_Manager::Get_Hp_Radio(const PLAYERTYPE& eType)
{
	return m_arrPlayer[eType]->Get_Hp_Radio();
}

void CPlayer_Manager::Free()
{
}
