#ifndef CBalrog_h__
#define CBalrog_h__

#include "GameObject_Dynamic.h"

_BEGIN(Engine)
class CMesh_Navigation;
class CCollider_Sphere;
class CState;
_END

_BEGIN(Client)

class CBalrog final : public CGameObject_Dynamic
{

public:
	explicit CBalrog(LPDIRECT3DDEVICE9 pDevice);
	explicit CBalrog(const CBalrog& rhs);
	virtual ~CBalrog() = default;

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
	void	Ready_Stats();
private:
	_float				m_fAttackRange;

public:
	static	CBalrog* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free() override;



};

_END

#endif // CBalrog_h__
