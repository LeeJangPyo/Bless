#ifndef Collision_Manager_h__
#define Collision_Manager_h__

#include "Base.h"
#include "Defines.h"

_BEGIN(Engine)
class CCollider_Sphere;
_END

_BEGIN(Client)
class CGameObject_Dynamic;
class CCollision_Manager : public CBase
{
	_DECLARE_SINGLETON(CCollision_Manager)
public:
	explicit CCollision_Manager();
	virtual ~CCollision_Manager() =default;
public:
	HRESULT			SetUp_PlayerList(CGameObject_Dynamic*	pGmaeObject_Dynamic);
	HRESULT			SetUp_MonsterList(CGameObject_Dynamic*	pGmaeObject_Dynamic);

public:
	_bool			Check_CollisionAndHit(COLLISION_TARGET eType, CCollider_Sphere*	pCollider, _int iDamage, _uint iEffect_Type = 0);
	_bool			Check_CollisionAndPush(COLLISION_TARGET eType, CGameObject_Dynamic*	pObject);
	_bool			Check_Collision(COLLISION_TARGET eType, CGameObject_Dynamic * pObject);

private:
	HRESULT			Ready_Collsion();

private:
	list<CGameObject_Dynamic*> m_PlayerList;
	list<CGameObject_Dynamic*> m_MonsterList;

protected:
	virtual void Free() override;
};

_END
#endif // Collision_Manager_h__