#include "stdafx.h"
#include "Collision_Manager.h"


#include "GameObject_Dynamic.h"
#include "Collider_Sphere.h"

#include "Management.h"
#include "Blood.h"

_USING(Client)

_IMPLEMENT_SINGLETON(CCollision_Manager)


Client::CCollision_Manager::CCollision_Manager()
{

}

HRESULT CCollision_Manager::SetUp_PlayerList(CGameObject_Dynamic * pGmaeObject_Dynamic)
{
	if (pGmaeObject_Dynamic == nullptr)
		return E_FAIL;

	m_PlayerList.push_back(pGmaeObject_Dynamic);
	pGmaeObject_Dynamic->AddRef();
	return S_OK;
}

HRESULT CCollision_Manager::SetUp_MonsterList(CGameObject_Dynamic * pGmaeObject_Dynamic)
{
	if (pGmaeObject_Dynamic == nullptr)
		return E_FAIL;

	m_MonsterList.push_back(pGmaeObject_Dynamic);
	pGmaeObject_Dynamic->AddRef();
	return S_OK;
}

//Target 누구를 맞출것인가
_bool CCollision_Manager::Check_CollisionAndHit(COLLISION_TARGET eType, CCollider_Sphere * pCollider, _int iDamag, _uint iEffect_Type)
{
	switch (eType)
	{
	case COL_PLAYER:
		for (auto& pPlayer : m_PlayerList)
		{
			if(pPlayer->Check_Collision(pCollider))
			{
				//맞은 효과
				pPlayer->Damaged(iDamag, iEffect_Type, nullptr);
				//return true;
			}
		}
		return false;
	case COL_MONSTER:
		for (auto& pMosnter : m_MonsterList)
		{
			_vec3 vCollision_Position(0.f,0.f,0.f);

			if (pMosnter->Check_Collision(pCollider,&vCollision_Position))
			{
				pMosnter->Damaged(iDamag, iEffect_Type, &vCollision_Position);

				//동시 피격
				//return true;
			}
		}
		return false;
	}
	return false;
}

_bool CCollision_Manager::Check_CollisionAndPush(COLLISION_TARGET eType, CGameObject_Dynamic * pObject)
{
	switch (eType)
	{
	case COL_PLAYER:
		for (auto& pPlayer : m_PlayerList)
		{
			if (pPlayer->Check_CollsionAndPush(pObject))
			{
				return true;
			}
		}
		return false;
	case COL_MONSTER:
		for (auto& pMosnter : m_MonsterList)
		{
			if (pMosnter == pObject)
				continue;

			if (pMosnter->Check_CollsionAndPush(pObject))
			{
				return true;
			}
			
		}
		return false;
	}
	return false;
}

_bool CCollision_Manager::Check_Collision(COLLISION_TARGET eType, CGameObject_Dynamic * pObject)
{
	switch (eType)
	{
	case COL_PLAYER:
		for (auto& pPlayer : m_PlayerList)
		{
			for (auto& pOtherCollider : *pObject->Get_ColliderList())
			{
				if (pPlayer->Check_Collision(pOtherCollider))
				{
					return true;
				}
			}
		}
		return false;
	case COL_MONSTER:
		for (auto& pMosnter : m_MonsterList)
		{
			if (pMosnter == pObject)
				continue;

			for (auto& pOtherCollider : *pObject->Get_ColliderList())
			{
				if (pMosnter->Check_Collision(pOtherCollider))
				{
					return true;
				}
			}
		}
		return false;
	}

	return false;
}



HRESULT CCollision_Manager::Ready_Collsion()
{
	return S_OK;
}


void CCollision_Manager::Free()
{
	for (auto& pPlayer : m_PlayerList)
		Safe_Release(pPlayer);
	m_PlayerList.clear();

	for (auto& pMonster : m_MonsterList)
		Safe_Release(pMonster);
	m_MonsterList.clear();
}
