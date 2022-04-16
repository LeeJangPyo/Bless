#ifndef CFlogas_h__
#define CFlogas_h__

#include "GameObject_Monster.h"

_BEGIN(Engine)
class CMesh_Navigation;
class CCollider_Sphere;
class CState;
_END

_BEGIN(Client)
class CUI_Frame;
class CFlogas final : public CGameObject_Monster
{

public:
	explicit CFlogas(LPDIRECT3DDEVICE9 pDevice);
	explicit CFlogas(const CFlogas& rhs);
	virtual ~CFlogas() = default;

public:
	void Set_HpBar_Activate(_bool bActivate);
	const _float&	Get_AttackRage() { return m_fAttackRange; }


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
	void	Ready_Stats();
private:
	_float				m_fAttackRange;

	CUI_Frame*			m_pUI_HpBar = nullptr;
	
public:
	static	CFlogas* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free() override;
};

_END

#endif // CFlogas_h__
