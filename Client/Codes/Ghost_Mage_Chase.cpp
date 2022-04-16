#include "stdafx.h"
#include "Ghost_Mage_Chase.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Player_Manager.h"

#include "Ghost_Mage.h"

_USING(Client)

CGhost_Mage_Chase::CGhost_Mage_Chase()
{

}

HRESULT CGhost_Mage_Chase::Begin()
{
	m_pGhost_Mage->Set_AnimationSet(10);

	//Get
	m_pTarget_Position = CPlayer_Manager::GetInstance()->Get_Player_Position(m_pGhost_Mage->Get_TargetPlayerType());

	return S_OK;
}

_int CGhost_Mage_Chase::Update(const _float & fTimeDelta)
{
	if (m_pGhost_Mage->Get_CurrentHp() < 0)
		m_pGhost_Mage->Change_State(L"Dead");

	m_pTransform->LookAt_Target(m_pTarget_Position, fTimeDelta);

	//Monster Collision Check;
	CCollision_Manager::GetInstance()->Check_CollisionAndPush(COL_MONSTER, m_pGhost_Mage);

	_vec3 vMovePos = m_pGhost_Mage->Move_OnNaviMesh(m_pTarget_Position, fTimeDelta);
	m_pTransform->Set_StateInfo(CTransform::STATE_POSITION, &vMovePos);
	

	return 0;
}

_int CGhost_Mage_Chase::LastUpdate(const _float & fTimeDelta)
{

	_float fDistance = m_pTransform->Get_Distance(m_pTarget_Position);
	if (fDistance < m_pGhost_Mage->Get_AttackRage())
	{
		m_pGhost_Mage->Change_State(L"Idle");
	}
	return 0;
}

HRESULT CGhost_Mage_Chase::End()
{
	return S_OK;
}

HRESULT CGhost_Mage_Chase::Ready_State(CGhost_Mage * pGhost_Mage)
{
	m_pGhost_Mage = pGhost_Mage;

	m_pTransform = m_pGhost_Mage->Get_Transform();

	return S_OK;;
}

CGhost_Mage_Chase * CGhost_Mage_Chase::Create(CGhost_Mage * pGhost_Mage)
{
	CGhost_Mage_Chase*		pInstance = new CGhost_Mage_Chase();

	if (FAILED(pInstance->Ready_State(pGhost_Mage)))
	{
		_MSG_BOX(L"CGhost_Mage_Chase Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Mage_Chase::Free()
{
}


