#include "stdafx.h"
#include "Skeleton_Idle.h"

#include "Skeleton.h"

#include "Management.h"
#include "Collision_Manager.h"

_USING(Client)

CSkeleton_Idle::CSkeleton_Idle()
	:m_fIdelTime(0.f)
{
}

HRESULT CSkeleton_Idle::Begin()
{
	m_pSkeleton->Set_AnimationSet(14);

	m_fIdelTime = 0.f;

	return S_OK;
}

_int CSkeleton_Idle::Update(const _float & fTimeDelta)
{
	m_fIdelTime += fTimeDelta;
		 
	if (m_pSkeleton->Get_CurrentHp() < 0)
		m_pSkeleton->Change_State(L"Dead");

	_vec3 vPlayerPos = *m_pPlayerTransform->Get_StateInfo(CTransform::STATE_POSITION);
	m_pSkeletonTransform->LookAt_Target(&vPlayerPos, fTimeDelta);

	return 0;
}

_int CSkeleton_Idle::LastUpdate(const _float & fTimeDelta)
{

	if (m_fIdelTime < 0.5f)
		return 0;

	//상태 분기
	_vec3 vPos = *m_pSkeletonTransform->Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vPlayerPos = *m_pPlayerTransform->Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vDir = vPlayerPos - vPos;
	_float fDistance = D3DXVec3Length(&vDir);


	// Target Change
	_int iTargetChangeRand = rand() % 5;
	if (iTargetChangeRand == 0)
	{
		PLAYERTYPE eType = m_pSkeleton->Get_TargetPlayerType();
		if(eType == PLAYER_LUPS)
			m_pSkeleton->Set_TargetPlayerType(PLAYER_SIRE);
		else
			m_pSkeleton->Set_TargetPlayerType(PLAYER_LUPS);
	}
		 
	if (fDistance < m_pSkeleton->Get_AttackRage())
	{
		_int iRand = rand() % 2;
		
		if (iRand == 0)
			m_pSkeleton->Change_State(L"Attack");// Attack
		else
			m_pSkeleton->Change_State(L"Crack");

	}
	else
	{
		_int iRand = rand() % 2;
		if (iRand == 0)
			m_pSkeleton->Change_State(L"Chase");
		else
			m_pSkeleton->Change_State(L"Spell");
		
	}

	return _int();
}

HRESULT CSkeleton_Idle::End()
{
	return S_OK;
}

HRESULT CSkeleton_Idle::Ready_State(CSkeleton * pSkeleton)
{
	m_pSkeleton  = pSkeleton;


	//Get Player's Transform;
	m_pPlayerTransform = dynamic_cast<CTransform*>
		(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Lups", L"Com_Transform"));
	if (m_pPlayerTransform == nullptr)
		return E_FAIL;

	m_pSkeletonTransform = m_pSkeleton->Get_Transform();
	return S_OK;
}

CSkeleton_Idle * CSkeleton_Idle::Create(CSkeleton* pSkeleton)
{
	CSkeleton_Idle*		pInstance = new CSkeleton_Idle();

	if (FAILED(pInstance->Ready_State(pSkeleton)))
	{
		_MSG_BOX(L"CSkeleton_Idle Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeleton_Idle::Free()
{
}


