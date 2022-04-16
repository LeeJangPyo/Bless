#ifndef Player_Manager_h__
#define Player_Manager_h__

#include "Base.h"

#include "Lups.h"
#include "Sire.h"

_BEGIN(Client)


class CPlayer_Manager final : public CBase
{
	_DECLARE_SINGLETON(CPlayer_Manager)


public:
	CPlayer_Manager();
	~CPlayer_Manager() = default;

public:
	void	Set_Lups(CLups* pLups) {
		m_arrPlayer[PLAYER_LUPS] = pLups;	}
	void	Set_Sire(CSire* pSire)	{
		m_arrPlayer[PLAYER_SIRE] = pSire;	}

	void	Set_Character(const PLAYERTYPE& eType);
	void	Set_Camera(PLAYERCAMERA_TYPE eType);
	void	Set_CombatState(COMBATSTATE eState);
	void	Set_CharacterState(const PLAYERTYPE& eType, const _tchar* pStateTag);


	void	Change_Player_NaviMesh();

	const _vec3*	Get_Camera_Eye_Position();
	const _vec3*	Get_Camera_At_Position();
	const _vec3*	Get_Player_Position();
	const _vec3*	Get_Player_Position(const PLAYERTYPE& eType);
	COMBATSTATE		Get_CombatState();
	CGameObject_Player*	Get_Player(const PLAYERTYPE& eType);
	CGameObject_Player* Get_CurrentPlayer();
	const PLAYERTYPE& Get_PlayerType();

	_float	Get_Hp_Radio(const PLAYERTYPE& eType);

private:
	CGameObject_Player*	m_arrPlayer[PLAYER_END];

	PLAYERTYPE			m_eCurrentPlayer;
	PLAYERCAMERA_TYPE	m_eCurrentCamera;
	COMBATSTATE			m_eCurrentCombatState;

protected:
	virtual void Free() override;

};


_END

#endif // Player_Manager_h__
