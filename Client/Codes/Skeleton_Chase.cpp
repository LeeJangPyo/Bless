#include "stdafx.h"
#include "Skeleton_Chase.h"

#include "Skeleton.h"

#include "Management.h"

#include "Collision_Manager.h"
#include "Player_Manager.h"

_USING(Client)

CSkeleton_Chase::CSkeleton_Chase()
{
}

HRESULT CSkeleton_Chase::Begin()
{
	m_pSkeleton->Set_AnimationSet(13);

	m_pTarget_Position = CPlayer_Manager::GetInstance()->Get_Player_Position(m_pSkeleton->Get_TargetPlayerType());

	return S_OK;
}

_int CSkeleton_Chase::Update(const _float & fTimeDelta)
{
	if (m_pSkeleton->Get_CurrentHp() < 0)
		m_pSkeleton->Change_State(L"Dead");

	m_pSkeletonTransform->LookAt_Target(m_pTarget_Position, fTimeDelta);
	m_pSkeleton->Go_Target(m_pTarget_Position, fTimeDelta);

	//Monster Collision Check;
	CCollision_Manager::GetInstance()->Check_CollisionAndPush(COL_MONSTER, m_pSkeleton);

	_vec3 vMovePos = m_pSkeleton->Move_OnNaviMesh(m_pTarget_Position, fTimeDelta);
	m_pSkeletonTransform->Set_StateInfo(CTransform::STATE_POSITION, &vMovePos);

	return 0;
}

_int CSkeleton_Chase::LastUpdate(const _float & fTimeDelta)
{
	//상태 분기
	_vec3 vPos = *m_pSkeletonTransform->Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vDir = *m_pTarget_Position - vPos;
	_float fDistance = D3DXVec3Length(&vDir);

	if (fDistance < m_pSkeleton->Get_AttackRage())
	{
		m_pSkeleton->Change_State(L"Idle");
	}

	return _int();
}

HRESULT CSkeleton_Chase::End()
{
	return S_OK;
}

HRESULT CSkeleton_Chase::Ready_State(CSkeleton * pSkeleton)
{
	m_pSkeleton  = pSkeleton;

	m_pSkeletonTransform = m_pSkeleton->Get_Transform();

	return S_OK;
}

CSkeleton_Chase * CSkeleton_Chase::Create(CSkeleton* pSkeleton)
{
	CSkeleton_Chase*		pInstance = new CSkeleton_Chase();

	if (FAILED(pInstance->Ready_State(pSkeleton)))
	{
		_MSG_BOX(L"CSkeleton_Chase Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeleton_Chase::Free()
{
}


