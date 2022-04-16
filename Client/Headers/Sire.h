#ifndef Sire_h__
#define Sire_h__

#include "GameObject_Player.h"


_BEGIN(Client)
class CGameEvent_Manager;
class CUI_Frame;
class CSire final	: public CGameObject_Player
{
public:
	explicit CSire(LPDIRECT3DDEVICE9 pDevice);
	explicit CSire(const CSire& rhs);
	virtual ~CSire() = default;

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

	virtual	HRESULT	SetUp_GameObjectInfo(const _matrix* pMatrix) override;
	virtual	void	Damaged(const _int& iDamage, const _uint& iEffect_Type, const _vec3* vEffect_Position) override;

public:
	virtual void	Check_KeyInput_Move(const _float& fTimeDelta) override;
	virtual void	Check_KeyInput(const _float& fTimeDelta) override;
public:
	void	Set_Spell_Target(const _vec3* pPosition) {
		m_vSpell_Target = *pPosition;	}	
	_vec3	Get_Spell_Target() {
		return m_vSpell_Target;	}

private:
	HRESULT	Ready_Component();
	HRESULT	Ready_State();
	void	Ready_StatsInfo();
private:
	_bool				m_bIsRButtonDown;

	_vec3				m_vSpell_Target;

	CUI_Frame*			m_pUI_HP = nullptr;
	CUI_Frame*			m_pUI_MP = nullptr;
	CUI_Frame*			m_pUI_Lups_HP = nullptr;
public:
	static	CSire* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free() override;



};


_END
#endif // Sire_h__