
#ifndef Ghost_Soldier_h__
#define Ghost_Soldier_h__

#include "GameObject_Monster.h"


_BEGIN(Engine)
class CMesh_Navigation;
class CCollider_Sphere;
class CState;
_END

_BEGIN(Client)
class CMonster_HpBar;
class CGhost_Soldier final	: public CGameObject_Monster
{

public:
	explicit CGhost_Soldier(LPDIRECT3DDEVICE9 pDevice);
	explicit CGhost_Soldier(const CGhost_Soldier& rhs);
	virtual ~CGhost_Soldier() = default;

public:

	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

	virtual HRESULT SetUp_GameObjectInfo(const _matrix* pMatrix) override;

	virtual	void Damaged(const _int& iDamage, const _uint& iEffect_Type, const _vec3* pEffect_Position) override;
private:

	HRESULT	Ready_Component();
	HRESULT	Ready_State();
	void	Ready_StatsInfo();
	HRESULT	Ready_HpBar();

private:
	CMonster_HpBar*		m_pMonster_HpBar = nullptr;

	_bool				m_bTauntOnce = false;
public:
	static	CGhost_Soldier* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;

protected:
	virtual void Free() override;

};

_END

#endif // Ghost_Soldier_h__
