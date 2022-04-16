#ifndef Collision_h__
#define Collision_h__

#include "Component.h"

_BEGIN(Engine)

//class CGameObject;
class CCollider_Sphere;
class _ENGINE_DLL CCollision : public CComponent
{
public:
	enum COLLISION_TARGET
	{
		COL_PLAYER, COL_MONSTER, COL_END
	};
public:
	explicit CCollision(LPDIRECT3DDEVICE9	pDevice);
	explicit CCollision(const CCollision&	rhs);
	virtual ~CCollision() =default;
public:
	HRESULT			SetUp_PlayerList(CCollider_Sphere*	pCollider);
	HRESULT			SetUp_MonsterList(list<CCollider_Sphere*>*	pColliderList);

public:
	_bool			Check_Collision(COLLISION_TARGET eType,CCollider_Sphere*	pCollider);

private:
	HRESULT			Ready_Collision();

private:
	list<CCollider_Sphere*> m_PlayerCollierList;
	list<list<CCollider_Sphere*>*> m_MonsterColliderList;

public:
	static CCollision* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone_Component() override;

protected:
	virtual void Free() override;
};

_END
#endif // Collision_h__