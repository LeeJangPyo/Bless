
#ifndef CGhost_Mage_h__
#define CGhost_Mage_h__

#include "GameObject_Monster.h"

_BEGIN(Engine)
class CMesh_Navigation;
class CCollider_Sphere;
class CState;
_END

_BEGIN(Client)
class CMonster_HpBar;
class CGhost_Mage final	: public CGameObject_Monster
{

public:
	explicit CGhost_Mage(LPDIRECT3DDEVICE9 pDevice);
	explicit CGhost_Mage(const CGhost_Mage& rhs);
	virtual ~CGhost_Mage() = default;

public:
	const _float&	Get_AttackRage() { return m_fAttackRange; }

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
	HRESULT Ready_HpBar();
private:
	_float				m_fAttackRange;

	CMonster_HpBar*		m_pMonster_HpBar = nullptr;
public:
	static	CGhost_Mage* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;

protected:
	virtual void Free() override;



};

_END

#endif // CGhost_Mage_h__
