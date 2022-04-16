#include "stdafx.h"
#include "Flogas_Idle.h"

#include "Flogas.h"

#include "Management.h"
#include "Collision_Manager.h"

_USING(Client)

CFlogas_Idle::CFlogas_Idle()
	:m_fIdleTime(0.f)
{
}

HRESULT CFlogas_Idle::Begin()
{

	m_fIdleTime = 0.f;

	m_pFlogas->Set_AnimationSet(14);


	return S_OK;
}

_int CFlogas_Idle::Update(const _float & fTimeDelta)
{
	m_fIdleTime += fTimeDelta;

	if (m_pFlogas->Get_CurrentHp() < 0)
		m_pFlogas->Change_State(L"Dead");

	_vec3 vPlayerPos = *m_pPlayerTransform->Get_StateInfo(CTransform::STATE_POSITION);
	m_pFlogasTransform->LookAt_Target(&vPlayerPos, fTimeDelta);

	return 0;
}

_int CFlogas_Idle::LastUpdate(const _float & fTimeDelta)
{
	if (m_fIdleTime < 0.5f)
		return 0;

	//상태 분기
	_vec3 vPos = *m_pFlogasTransform->Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vPlayerPos = *m_pPlayerTransform->Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vDir = vPlayerPos - vPos;
	_float fDistance = D3DXVec3Length(&vDir);

	// Target Change
	_int iTargetChangeRand = rand() % 5;
	if (iTargetChangeRand == 0)
	{
		PLAYERTYPE eType = m_pFlogas->Get_TargetPlayerType();
		if (eType == PLAYER_LUPS)
			m_pFlogas->Set_TargetPlayerType(PLAYER_SIRE);
		else
			m_pFlogas->Set_TargetPlayerType(PLAYER_LUPS);
	}
	
	if (fDistance < m_pFlogas->Get_AttackRage())
	{
		m_pFlogas->Change_State(L"Attack");// Attack
	}
	else if (fDistance < m_pFlogas->Get_AttackRage() + 3.5f)
	{
		_int iRand = rand() % 3;

		if (iRand == 0)
			m_pFlogas->Change_State(L"Stab");// Attack
		else if (iRand == 1)
			m_pFlogas->Change_State(L"Earthquake");
		else
			m_pFlogas->Change_State(L"FireSpurt");
	}
	else
	{
		_int iRand = rand() % 2;
		if(iRand == 0)
			m_pFlogas->Change_State(L"Chase");
		else
			m_pFlogas->Change_State(L"Summon");
	}
//	m_pFlogas->Change_State(L"FireSpurt");

	return _int();
}

HRESULT CFlogas_Idle::End()
{
	return S_OK;
}

HRESULT CFlogas_Idle::Ready_State(CFlogas * pFlogas)
{
	m_pFlogas  = pFlogas;

	//Get Player's Transform;
	m_pPlayerTransform = dynamic_cast<CTransform*>
		(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Lups", L"Com_Transform"));
	if (m_pPlayerTransform == nullptr)
		return E_FAIL;

	m_pFlogasTransform = m_pFlogas->Get_Transform();
	return S_OK;
}

CFlogas_Idle * CFlogas_Idle::Create(CFlogas* pFlogas)
{
	CFlogas_Idle*		pInstance = new CFlogas_Idle();

	if (FAILED(pInstance->Ready_State(pFlogas)))
	{
		_MSG_BOX(L"CFlogas_Idle Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlogas_Idle::Free()
{
}


