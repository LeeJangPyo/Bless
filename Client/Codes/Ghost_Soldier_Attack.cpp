#include "stdafx.h"
#include "Ghost_Soldier_Attack.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Player_Manager.h"

#include "Ghost_Soldier.h"

_USING(Client)

CGhost_Soldier_Attack::CGhost_Soldier_Attack()
	:m_iAttackIndex(0)
{

}

HRESULT CGhost_Soldier_Attack::Begin()
{
	//공격패턴은 16-17-18-19까지
	m_iAttackIndex = rand() % 4 + 16;
	m_pGhost_Solider->Set_AnimationSet(m_iAttackIndex);

	m_pGhost_Solider->Set_FrameConst(1.f);
	m_pTarget_Position = CPlayer_Manager::GetInstance()->Get_Player_Position(m_pGhost_Solider->Get_TargetPlayerType());

	return S_OK;
}

_int CGhost_Soldier_Attack::Update(const _float & fTimeDelta)
{
	if (m_pGhost_Solider->Get_CurrentHp() < 0)
		m_pGhost_Solider->Change_State(L"Dead");

	m_pTransform->LookAt_Target(m_pTarget_Position, fTimeDelta);

	if (m_iAttackIndex == 16)
	{
		if (m_pGhost_Solider->Get_IsAnimationRange(0.35f, 0.45f))
		{
			//_MSG_BOX(L"Check 16");
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_PLAYER, m_pGhost_Solider->Get_Collider_Weapon(), m_pGhost_Solider_Damage->iDamage + rand() % m_pGhost_Solider_Damage->iDamage_Range);

		}
	}
	else if (m_iAttackIndex == 17)
	{
		if (m_pGhost_Solider->Get_IsAnimationRange(0.25f, 0.35f))
		{
		//	_MSG_BOX(L"Check 17");
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_PLAYER, m_pGhost_Solider->Get_Collider_Weapon(), m_pGhost_Solider_Damage->iDamage + rand() % m_pGhost_Solider_Damage->iDamage_Range);
		}
		if (m_pGhost_Solider->Get_IsAnimationRange(0.675f, 0.775f))
		{
		//		_MSG_BOX(L"Check 17.5");
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_PLAYER, m_pGhost_Solider->Get_Collider_Weapon(), m_pGhost_Solider_Damage->iDamage + rand() % m_pGhost_Solider_Damage->iDamage_Range);
		}
	}
	else if (m_iAttackIndex == 18)
	{
		if (m_pGhost_Solider->Get_IsAnimationRange(0.45f, 0.55f))
		{
		//	_MSG_BOX(L"Check 18");
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_PLAYER, m_pGhost_Solider->Get_Collider_Weapon(), m_pGhost_Solider_Damage->iDamage + rand() % m_pGhost_Solider_Damage->iDamage_Range);
		}
	}
	else if (m_iAttackIndex == 19)
	{
		if (m_pGhost_Solider->Get_IsAnimationRange(0.2f, 0.3f))
		{
			//_MSG_BOX(L"Check 19 ");
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_PLAYER, m_pGhost_Solider->Get_Collider_Weapon(), m_pGhost_Solider_Damage->iDamage + rand() % m_pGhost_Solider_Damage->iDamage_Range);

		}
	}

	CCollision_Manager::GetInstance()->Check_CollisionAndPush(COL_MONSTER, m_pGhost_Solider);

	return 0;
}

_int CGhost_Soldier_Attack::LastUpdate(const _float & fTimeDelta)
{

	if (m_pGhost_Solider->Get_IsAnimationSetEnd())
	{
		m_pGhost_Solider->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CGhost_Soldier_Attack::End()
{
	m_pGhost_Solider->Set_FrameConst(1.f);

	return S_OK;
}

HRESULT CGhost_Soldier_Attack::Ready_State(CGhost_Soldier * pGhost_Soldier)
{
	m_pGhost_Solider = pGhost_Soldier;

	m_pGhost_Solider_Damage = m_pGhost_Solider->Get_Damage();

	m_pTransform = m_pGhost_Solider->Get_Transform();

	return S_OK;;
}

CGhost_Soldier_Attack * CGhost_Soldier_Attack::Create(CGhost_Soldier * pGhost_Soldier)
{
	CGhost_Soldier_Attack*		pInstance = new CGhost_Soldier_Attack();

	if (FAILED(pInstance->Ready_State(pGhost_Soldier)))
	{
		_MSG_BOX(L"CGhost_Soldier_Attack Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Soldier_Attack::Free()
{
}


