#include "stdafx.h"
#include "Ghost_Mage_Idle.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Ghost_Mage.h"

_USING(Client)

CGhost_Mage_Idle::CGhost_Mage_Idle()
	:m_fIdleTime(0.f)
{

}

HRESULT CGhost_Mage_Idle::Begin()
{
	m_fIdleTime = 0.f;

	m_pGhost_Mage->Set_AnimationSet(11);



	return S_OK;
}

_int CGhost_Mage_Idle::Update(const _float & fTimeDelta)
{
	m_fIdleTime += fTimeDelta;


	if (m_pGhost_Mage->Get_CurrentHp() < 0)
		m_pGhost_Mage->Change_State(L"Dead");


	return 0;
}

_int CGhost_Mage_Idle::LastUpdate(const _float & fTimeDelta)
{

	if (m_fIdleTime < 0.5f)
		return 0;

	//상태 분기
	_float fDistance = m_pTransform->Get_Distance(m_pPlayerTransform->Get_StateInfo(CTransform::STATE_POSITION));
	if (CCollision_Manager::GetInstance()->Check_Collision(COL_PLAYER, m_pGhost_Mage))
	{
		// Attack
		m_pGhost_Mage->Change_State(L"Attack");
		return 0;
	}
	
	if (fDistance < m_pGhost_Mage->Get_AttackRage())
	{
		m_pGhost_Mage->Change_State(L"IceBolt");
	}
	else
	{
		m_pGhost_Mage->Change_State(L"Chase");
	}
	return 0;
}

HRESULT CGhost_Mage_Idle::End()
{
	return S_OK;
}

HRESULT CGhost_Mage_Idle::Ready_State(CGhost_Mage * pGhost_Mage)
{
	m_pGhost_Mage = pGhost_Mage;


	//Get Player's Transform;
	m_pPlayerTransform = dynamic_cast<CTransform*>
		(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Lups", L"Com_Transform"));
	if (m_pPlayerTransform == nullptr)
		return E_FAIL;

	m_pTransform = m_pGhost_Mage->Get_Transform();
	return S_OK;;
}

CGhost_Mage_Idle * CGhost_Mage_Idle::Create(CGhost_Mage * pGhost_Mage)
{
	CGhost_Mage_Idle*		pInstance = new CGhost_Mage_Idle();

	if (FAILED(pInstance->Ready_State(pGhost_Mage)))
	{
		_MSG_BOX(L"CGhost_Mage_Idle Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Mage_Idle::Free()
{
}


