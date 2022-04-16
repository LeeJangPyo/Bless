#include "stdafx.h"
#include "Flogas_Stab.h"

#include "Flogas.h"

#include "Management.h"

#include "Collision_Manager.h"
#include "Sound_Manager.h"
#include "Player_Manager.h"

_USING(Client)

CFlogas_Stab::CFlogas_Stab()
{
}

HRESULT CFlogas_Stab::Begin()
{
	//19~20
	m_pFlogas->Set_AnimationSet(2);

	m_pFlogas->Set_FrameConst(0.75f);

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
	CSound_Manager::GetInstance()->Play_Sound(L"Lups_SwordThrowing1.mp3", CSound_Manager::BOSS);

	m_pTarget_Position = CPlayer_Manager::GetInstance()->Get_Player_Position(m_pFlogas->Get_TargetPlayerType());
	return S_OK;
}

_int CFlogas_Stab::Update(const _float & fTimeDelta)
{
	if (m_pFlogas->Get_CurrentHp() < 0)
		m_pFlogas->Change_State(L"Dead");

	m_pFlogasTransform->LookAt_Target(m_pTarget_Position, fTimeDelta);

	if (m_pFlogas->Get_IsAnimationRange(0.4f, 0.5f))
	{
		//	_MSG_BOX(L"Check 7");
		const DAMAGE*	pDemage = m_pFlogas->Get_Damage();
		CCollision_Manager::GetInstance()->
			Check_CollisionAndHit(COL_PLAYER, m_pFlogas->Get_Collider_Weapon(), pDemage->iDamage + rand() % pDemage->iDamage_Range, EFFECT_BLOOD);
	}


	return 0;
}

_int CFlogas_Stab::LastUpdate(const _float & fTimeDelta)
{
	if (m_pFlogas->Get_IsAnimationSetEnd())
	{
		m_pFlogas->Change_State(L"Idle");
	}
	return _int();
}

HRESULT CFlogas_Stab::End()
{
	m_pFlogas->Set_FrameConst(1.f);
	return S_OK;
}

HRESULT CFlogas_Stab::Ready_State(CFlogas * pFlogas)
{
	m_pFlogas  = pFlogas;

	m_pFlogas_Damage = m_pFlogas->Get_Damage();

	m_pFlogasTransform = m_pFlogas->Get_Transform();

	return S_OK;;
}

CFlogas_Stab * CFlogas_Stab::Create(CFlogas* pFlogas)
{
	CFlogas_Stab*		pInstance = new CFlogas_Stab();

	if (FAILED(pInstance->Ready_State(pFlogas)))
	{
		_MSG_BOX(L"CFlogas_Stab Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlogas_Stab::Free()
{
}


