#include "stdafx.h"
#include "Skeleton_Attack.h"

#include "Skeleton.h"

#include "Management.h"

#include "Collision_Manager.h"
#include "Player_Manager.h"
#include "Sound_Manager.h"

_USING(Client)

CSkeleton_Attack::CSkeleton_Attack()
{
}

HRESULT CSkeleton_Attack::Begin()
{
	//19~20
	m_iAttackIndex = rand() % 2 + 19;
	m_pSkeleton->Set_AnimationSet(m_iAttackIndex);

	m_pSkeleton->Set_FrameConst(0.75f);

	m_pTarget_Position = CPlayer_Manager::GetInstance()->Get_Player_Position(m_pSkeleton->Get_TargetPlayerType());


	return S_OK;
}

_int CSkeleton_Attack::Update(const _float & fTimeDelta)
{
	if (m_pSkeleton->Get_CurrentHp() < 0)
		m_pSkeleton->Change_State(L"Dead");

	m_pSkeletonTransform->LookAt_Target(m_pTarget_Position, fTimeDelta);

	//CCollision_Manager::GetInstance()->Check_CollisionAndPush(COL_MONSTER, m_pSkeleton);

	if (m_iAttackIndex == 19)
	{
		if (m_pSkeleton->Get_IsAnimationRange(0.425f, 0.5f))
		{

			CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
			CSound_Manager::GetInstance()->Play_Sound(L"Skeleton_Attack.mp3", CSound_Manager::BOSS);
			//_MSG_BOX(L"Check 6");
			const DAMAGE*	pDemage = m_pSkeleton->Get_Damage();
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_PLAYER, m_pSkeleton->Get_Collider_Weapon(), pDemage->iDamage + rand() % pDemage->iDamage_Range, EFFECT_BLOOD);
		}

	}
	else if (m_iAttackIndex == 20)
	{
		if (m_pSkeleton->Get_IsAnimationRange(0.415f, 0.5f))
		{

			CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
			CSound_Manager::GetInstance()->Play_Sound(L"Skeleton_Attack.mp3", CSound_Manager::BOSS);
			//	_MSG_BOX(L"Check 7");
			const DAMAGE*	pDemage = m_pSkeleton->Get_Damage();
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_PLAYER, m_pSkeleton->Get_Collider_Weapon(), pDemage->iDamage + rand() % pDemage->iDamage_Range, EFFECT_BLOOD);
		}
	}

	return 0;
}

_int CSkeleton_Attack::LastUpdate(const _float & fTimeDelta)
{
	if (m_pSkeleton->Get_IsAnimationSetEnd())
	{
		m_pSkeleton->Change_State(L"Idle");
	}
	return _int();
}

HRESULT CSkeleton_Attack::End()
{
	m_pSkeleton->Set_FrameConst(1.f);
	return S_OK;
}

HRESULT CSkeleton_Attack::Ready_State(CSkeleton * pSkeleton)
{
	m_pSkeleton  = pSkeleton;

	m_pSkeleton_Damage = m_pSkeleton->Get_Damage();

	m_pSkeletonTransform = m_pSkeleton->Get_Transform();


	return S_OK;;
}

CSkeleton_Attack * CSkeleton_Attack::Create(CSkeleton* pSkeleton)
{
	CSkeleton_Attack*		pInstance = new CSkeleton_Attack();

	if (FAILED(pInstance->Ready_State(pSkeleton)))
	{
		_MSG_BOX(L"CSkeleton_Attack Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeleton_Attack::Free()
{
}


