#ifndef GameObject_Player_h__
#define GameObject_Player_h__

#include "GameObject_Dynamic.h"

_BEGIN(Engine)
class CInput_Device;
_END

_BEGIN(Client)

class CGameObject_Player abstract : public CGameObject_Dynamic
{
public:
	explicit CGameObject_Player(LPDIRECT3DDEVICE9 pDevice);
	explicit CGameObject_Player(const CGameObject_Player& rhs);
	
	virtual ~CGameObject_Player() = default;

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

	virtual void Damaged(const _int& iDamage, const _uint& iEffect_Type, const _vec3* vEffect_Position) = 0;
public:
	const _vec3*	Get_Camera_Eye_Position(PLAYERCAMERA_TYPE eType){
		return &m_vCamera_Eye_Position[eType];	}
	const _vec3*	Get_Camera_At_Position() {
		return &m_vCamera_At_Position;
	}
	_float			Get_Hp_Radio();
	_float			Get_Mp_Radio();

	//False Not Enought Mana;
	_bool	Use_Mp(const _int& iMp);

	void	Set_UI_Activate(const PLAYERTYPE& ePlayerType,_bool bActivate);
	void	Set_ControlType(CONTROLTYPE eType);

	CONTROLTYPE		Get_ControlType() {
		return m_eControl;	}
protected:
	virtual void	Check_KeyInput_Move(const _float& fTimeDelta) = 0;
	virtual void	Check_KeyInput(const _float& fTimeDelta) = 0; 

	void	Update_CameraPosition();
protected:
	CInput_Device*		m_pInput_Device = nullptr;

	_vec3				m_vCamera_Eye_Position[PLAYERCAMERA_END];
	_vec3				m_vCamera_At_Position;
	_float				m_fCameraDistance[PLAYERCAMERA_END];

	_float				m_fCamera_At_Acc;

	CONTROLTYPE			m_eControl;
public:
	virtual CGameObject* Clone_GameObject() = 0;
	
protected:
	virtual void Free() override;

};


_END

#endif // GameObject_Player_h__
