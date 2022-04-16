#ifndef CSkeleton_h__
#define CSkeleton_h__

#include "GameObject_Monster.h"

_BEGIN(Engine)
class CMesh_Navigation;
class CCollider_Sphere;
class CState;
_END

_BEGIN(Client)
class CUI_Frame;
class CSkeleton final : public CGameObject_Monster
{

public:
	explicit CSkeleton(LPDIRECT3DDEVICE9 pDevice);
	explicit CSkeleton(const CSkeleton& rhs);
	virtual ~CSkeleton() = default;

public:
	void Set_HpBar_Activate(_bool bActivate);
	const _float&	Get_AttackRage() { return m_fAttackRange; }
	const _vec3*	Get_WeaponCollider_Position();


public:

	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

	virtual HRESULT SetUp_GameObjectInfo(const _matrix* pMatrix) override;

	virtual	void Damaged(const _int& iDamage, const _uint& iEffect_Type, const _vec3* vEffect_Position) override;
private:
	HRESULT	Ready_Component();
	HRESULT	Ready_State();
	void	Ready_StatsInfo();
private:
	_float				m_fAttackRange;

	CUI_Frame*			m_pUI_HpBar = nullptr;
public:
	static	CSkeleton* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free() override;
};

_END

#endif // CSkeleton_h__
