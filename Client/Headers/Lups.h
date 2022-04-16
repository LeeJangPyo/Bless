#ifndef Lups_h__
#define Lups_h__

#include "GameObject_Player.h"


_BEGIN(Client)
class CUI_Frame;
class CGameEvent_Manager;
class CLups final	: public CGameObject_Player
{
public:
	explicit CLups(LPDIRECT3DDEVICE9 pDevice);
	explicit CLups(const CLups& rhs);
	virtual ~CLups() = default;

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

	virtual	HRESULT	SetUp_GameObjectInfo(const _matrix* pMatrix) override;
	virtual	void	Damaged(const _int& iDamage,const _uint& iEffect_Type, const _vec3* vEffect_Position) override;

	virtual void	Check_KeyInput_Move(const _float& fTimeDelta);
	virtual void	Check_KeyInput(const _float& fTimeDelta);

private:
	HRESULT	Ready_Component();
	HRESULT	Ready_State();
	void	Ready_StatsInfo();

private:
	_bool				m_bIsRButtonDown;

	CUI_Frame*			m_pUI_HP = nullptr;
	CUI_Frame*			m_pUI_MP = nullptr;
	CUI_Frame*			m_pUI_Sire_HP = nullptr;
public:
	static	CLups* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free() override;
};
_END
#endif // Lups_h__