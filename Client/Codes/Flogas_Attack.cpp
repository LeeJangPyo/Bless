#include "stdafx.h"
#include "Flogas_Attack.h"

#include "Flogas.h"

#include "Management.h"

#include "Collision_Manager.h"
#include "Sound_Manager.h"
#include "Player_Manager.h"

_USING(Client)

CFlogas_Attack::CFlogas_Attack()
{
}

HRESULT CFlogas_Attack::Begin()
{

	m_iAttackIndex = 6 + rand() % 2;
	m_pFlogas->Set_AnimationSet(m_iAttackIndex);

	if (m_iAttackIndex == 6)
	{
		CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
		CSound_Manager::GetInstance()->Play_Sound(L"Flogas_Attack_1.mp3", CSound_Manager::BOSS);
	}
	else if (m_iAttackIndex ==7)
	{
		CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
		CSound_Manager::GetInstance()->Play_Sound(L"Flogas_Attack_2.mp3", CSound_Manager::BOSS);
	}

	m_pFlogas->Set_FrameConst(1.f);

	m_pTarget_Position = CPlayer_Manager::GetInstance()->Get_Player_Position(m_pFlogas->Get_TargetPlayerType());

	return S_OK;
}

_int CFlogas_Attack::Update(const _float & fTimeDelta)
{
	if (m_pFlogas->Get_CurrentHp() < 0)
		m_pFlogas->Change_State(L"Dead");

	m_pFlogasTransform->LookAt_Target(m_pTarget_Position, fTimeDelta);

	if (m_iAttackIndex == 6)
	{
		if (m_pFlogas->Get_IsAnimationRange(0.425f, 0.5f)/* && m_bIsCollision == false*/)
		{
			//_MSG_BOX(L"Check 6");
			const DAMAGE*	pDemage = m_pFlogas->Get_Damage();
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_PLAYER, m_pFlogas->Get_Collider_Weapon(), pDemage->iDamage + rand() % pDemage->iDamage_Range, EFFECT_BLOOD);

		}

	}
	else if (m_iAttackIndex ==7)
	{
		if (m_pFlogas->Get_IsAnimationRange(0.415f, 0.5f)/* && m_bIsCollision == false*/)
		{
		//	_MSG_BOX(L"Check 7");
			const DAMAGE*	pDemage = m_pFlogas->Get_Damage();
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_PLAYER, m_pFlogas->Get_Collider_Weapon(), pDemage->iDamage + rand() % pDemage->iDamage_Range, EFFECT_BLOOD);
		}
	}
	return 0;
}

_int CFlogas_Attack::LastUpdate(const _float & fTimeDelta)
{
	if (m_pFlogas->Get_IsAnimationSetEnd())
	{
		m_pFlogas->Change_State(L"Idle");
	}
	return _int();
}

HRESULT CFlogas_Attack::End()
{
	m_pFlogas->Set_FrameConst(1.f);
	return S_OK;
}

HRESULT CFlogas_Attack::Ready_State(CFlogas * pFlogas)
{
	m_pFlogas  = pFlogas;

	m_pFlogas_Damage = m_pFlogas->Get_Damage();

	m_pFlogasTransform = m_pFlogas->Get_Transform();

	return S_OK;;
}

CFlogas_Attack * CFlogas_Attack::Create(CFlogas* pFlogas)
{
	CFlogas_Attack*		pInstance = new CFlogas_Attack();

	if (FAILED(pInstance->Ready_State(pFlogas)))
	{
		_MSG_BOX(L"CFlogas_Attack Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlogas_Attack::Free()
{
}


