#include "stdafx.h"
#include "Skeleton_Stab.h"

#include "Skeleton.h"

#include "Management.h"

#include "Collision_Manager.h"

_USING(Client)

CSkeleton_Stab::CSkeleton_Stab()
{
}

HRESULT CSkeleton_Stab::Begin()
{
	//19~20
	m_pSkeleton->Set_AnimationSet(12);

	m_pSkeleton->Set_FrameConst(0.75f);

	return S_OK;
}

_int CSkeleton_Stab::Update(const _float & fTimeDelta)
{
	if (m_pSkeleton->Get_CurrentHp() < 0)
		m_pSkeleton->Change_State(L"Dead");

	_vec3 vPlayerPos = *m_pPlayerTransform->Get_StateInfo(CTransform::STATE_POSITION);
	m_pSkeletonTransform->LookAt_Target(&vPlayerPos, fTimeDelta);

	//CCollision_Manager::GetInstance()->Check_CollisionAndPush(COL_MONSTER, m_pSkeleton);

	CCollision_Manager::GetInstance()->
		Check_CollisionAndHit(COL_PLAYER, m_pSkeleton->Get_Collider_Weapon(), m_pSkeleton_Damage->iDamage + rand() % m_pSkeleton_Damage->iDamage_Range);

	return 0;
}

_int CSkeleton_Stab::LastUpdate(const _float & fTimeDelta)
{
	if (m_pSkeleton->Get_IsAnimationSetEnd())
	{
		m_pSkeleton->Change_State(L"Idle");
	}
	return _int();
}

HRESULT CSkeleton_Stab::End()
{
	m_pSkeleton->Set_FrameConst(1.f);
	return S_OK;
}

HRESULT CSkeleton_Stab::Ready_State(CSkeleton * pSkeleton)
{
	m_pSkeleton  = pSkeleton;

	m_pSkeleton_Damage = m_pSkeleton->Get_Damage();

	//Get Player's Transform;
	m_pPlayerTransform = dynamic_cast<CTransform*>
		(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Lups", L"Com_Transform"));
	if (m_pPlayerTransform == nullptr)
		return E_FAIL;


	m_pSkeletonTransform = m_pSkeleton->Get_Transform();
	return S_OK;;
}

CSkeleton_Stab * CSkeleton_Stab::Create(CSkeleton* pSkeleton)
{
	CSkeleton_Stab*		pInstance = new CSkeleton_Stab();

	if (FAILED(pInstance->Ready_State(pSkeleton)))
	{
		_MSG_BOX(L"CSkeleton_Stab Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeleton_Stab::Free()
{
}


