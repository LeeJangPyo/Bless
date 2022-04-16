#include "stdafx.h"
#include "Ghost_Soldier_Chase.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Player_Manager.h"


#include "Ghost_Soldier.h"

_USING(Client)

CGhost_Soldier_Chase::CGhost_Soldier_Chase()
{

}

HRESULT CGhost_Soldier_Chase::Begin()
{
	m_pGhost_Solider->Set_AnimationSet(4);

	m_pTarget_Position = CPlayer_Manager::GetInstance()->Get_Player_Position(m_pGhost_Solider->Get_TargetPlayerType());

	return S_OK;
}

_int CGhost_Soldier_Chase::Update(const _float & fTimeDelta)
{
	if (m_pGhost_Solider->Get_CurrentHp() < 0)
		m_pGhost_Solider->Change_State(L"Dead");

	m_pTransform->LookAt_Target(m_pTarget_Position, fTimeDelta);
	//m_pGhost_Solider->Go_Target(&vPlayerPos, fTimeDelta);

	//Monster Collision Check;
	CCollision_Manager::GetInstance()->Check_CollisionAndPush(COL_MONSTER, m_pGhost_Solider);

	_vec3 vMovePos = m_pGhost_Solider->Move_OnNaviMesh(m_pTarget_Position, fTimeDelta);
	m_pTransform->Set_StateInfo(CTransform::STATE_POSITION, &vMovePos);

	return 0;
}

_int CGhost_Soldier_Chase::LastUpdate(const _float & fTimeDelta)
{
	if (CCollision_Manager::GetInstance()->Check_Collision(COL_PLAYER, m_pGhost_Solider))
	{
		m_pGhost_Solider->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CGhost_Soldier_Chase::End()
{
	return S_OK;
}

HRESULT CGhost_Soldier_Chase::Ready_State(CGhost_Soldier * pGhost_Soldier)
{
	m_pGhost_Solider = pGhost_Soldier;

	m_pTransform = m_pGhost_Solider->Get_Transform();

	return S_OK;;
}

CGhost_Soldier_Chase * CGhost_Soldier_Chase::Create(CGhost_Soldier * pGhost_Soldier)
{
	CGhost_Soldier_Chase*		pInstance = new CGhost_Soldier_Chase();

	if (FAILED(pInstance->Ready_State(pGhost_Soldier)))
	{
		_MSG_BOX(L"CGhost_Soldier_Chase Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Soldier_Chase::Free()
{
}


