#include "stdafx.h"
#include "Ghost_Mage_Attack.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Player_Manager.h"


#include "Ghost_Mage.h"

_USING(Client)

CGhost_Mage_Attack::CGhost_Mage_Attack()
{

}

HRESULT CGhost_Mage_Attack::Begin()
{
	m_pGhost_Mage->Set_AnimationSet(20);

	m_pTarget_Position = CPlayer_Manager::GetInstance()->Get_Player_Position(m_pGhost_Mage->Get_TargetPlayerType());

	return S_OK;
}

_int CGhost_Mage_Attack::Update(const _float & fTimeDelta)
{
	if (m_pGhost_Mage->Get_CurrentHp() < 0)
		m_pGhost_Mage->Change_State(L"Dead");

	m_pTransform->LookAt_Target(m_pTarget_Position, fTimeDelta);

	CCollision_Manager::GetInstance()->Check_CollisionAndPush(COL_PLAYER, m_pGhost_Mage);



	return 0;
}

_int CGhost_Mage_Attack::LastUpdate(const _float & fTimeDelta)
{

	if (m_pGhost_Mage->Get_IsAnimationSetEnd())
	{
		m_pGhost_Mage->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CGhost_Mage_Attack::End()
{
	return S_OK;
}

HRESULT CGhost_Mage_Attack::Ready_State(CGhost_Mage * pGhost_Mage)
{
	m_pGhost_Mage = pGhost_Mage;

	m_pTransform = m_pGhost_Mage->Get_Transform();

	return S_OK;;
}

CGhost_Mage_Attack * CGhost_Mage_Attack::Create(CGhost_Mage * pGhost_Mage)
{
	CGhost_Mage_Attack*		pInstance = new CGhost_Mage_Attack();

	if (FAILED(pInstance->Ready_State(pGhost_Mage)))
	{
		_MSG_BOX(L"CGhost_Mage_Attack Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Mage_Attack::Free()
{
}


