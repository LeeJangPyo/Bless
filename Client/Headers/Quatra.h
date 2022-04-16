#ifndef CQuatra_h__
#define CQuatra_h__

#include "GameObject_Monster.h"

_BEGIN(Engine)
class CMesh_Navigation;
class CCollider_Sphere;
class CState;
_END

_BEGIN(Client)
class CQuatra final : public CGameObject_Monster
{

public:
	explicit CQuatra(LPDIRECT3DDEVICE9 pDevice);
	explicit CQuatra(const CQuatra& rhs);
	virtual ~CQuatra() = default;
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

public:
	static	CQuatra* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free() override;
};

_END

#endif // CQuatra_h__
