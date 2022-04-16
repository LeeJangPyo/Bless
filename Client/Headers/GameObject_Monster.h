
#ifndef GameObject_Monster_h__
#define GameObject_Monster_h__

#include "GameObject_Dynamic.h"


_BEGIN(Engine)
_END

_USING(Client)

class CGameObject_Monster abstract : public CGameObject_Dynamic
{
public:
	explicit	CGameObject_Monster(LPDIRECT3DDEVICE9 pDevice);
	explicit	CGameObject_Monster(const CGameObject_Monster& rhs);
	virtual		~CGameObject_Monster() = default;

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

	virtual HRESULT SetUp_GameObjectInfo(const _matrix* pMatrix) override;
	virtual	void Damaged(const _int& iDamage, const _uint& iEffect_Type, const _vec3* vEffect_Position) = 0;

public:
	const PLAYERTYPE&	Get_TargetPlayerType() {
		return m_eTargetPlayerType;	}
	void Set_TargetPlayerType(const PLAYERTYPE& eType){
		m_eTargetPlayerType = eType;	}
	CCollider_Sphere*	Get_Collider_Weapon() {
		if (m_pCollider_Weapon == nullptr)
			return nullptr;
		return m_pCollider_Weapon;
	}	
	const _vec3*	Get_WeaponCollider_Position();

protected:
	HRESULT Load_WeaponCollier_FromFile(const _tchar* pFilePath);
protected:
	_float				m_fTimeDelta;

	PLAYERTYPE			m_eTargetPlayerType;

	CCollider_Sphere*	m_pCollider_Weapon = nullptr;
public:
	virtual CGameObject* Clone_GameObject() = 0;

protected:
	virtual void Free() override;
};	



#endif // GameObject_Monster_h__
