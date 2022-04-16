#include "stdafx.h"
#include "Ghost_Soldier_DownAndUp.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Ghost_Soldier.h"
#include "Damage.h"

_USING(Client)

CGhost_Soldier_DownAndUp::CGhost_Soldier_DownAndUp()
	:m_iAnimationIndex(0)
{

}

HRESULT CGhost_Soldier_DownAndUp::Begin()
{
	m_iAnimationIndex = 11;
	m_pGhost_Solider->Set_AnimationSet(m_iAnimationIndex,true);
	
	m_pGhost_Solider->Set_FrameConst(1.f);

	return S_OK;
}

_int CGhost_Soldier_DownAndUp::Update(const _float & fTimeDelta)
{
	if (m_pGhost_Solider->Get_CurrentHp() < 0)
		m_pGhost_Solider->Change_State(L"Dead");

	return 0;
}

_int CGhost_Soldier_DownAndUp::LastUpdate(const _float & fTimeDelta)
{
	if (m_pGhost_Solider->Get_IsAnimationSetEnd() && m_iAnimationIndex == 11)
	{
		//m_pGhost_Solider->Change_State(L"Idle");
		m_iAnimationIndex = 8;
		m_pGhost_Solider->Set_AnimationSet(m_iAnimationIndex);
	}
	//if (m_pGhost_Solider->Get_IsAnimationSetEnd() && m_iAnimationIndex == 9)
	//{
	//	//m_pGhost_Solider->Change_State(L"Idle");
	//	m_iAnimationIndex = 8;
	//	m_pGhost_Solider->Set_AnimationSet(m_iAnimationIndex);
	//}
	if (m_pGhost_Solider->Get_IsAnimationSetEnd() && m_iAnimationIndex == 8)
	{
		m_pGhost_Solider->Change_State(L"Idle");
	}


	return 0;
}

HRESULT CGhost_Soldier_DownAndUp::End()
{
	m_pGhost_Solider->Set_FrameConst(1.f);

	return S_OK;
}

HRESULT CGhost_Soldier_DownAndUp::Ready_State(CGhost_Soldier * pGhost_Soldier)
{
	m_pGhost_Solider = pGhost_Soldier;


	m_pTransform = m_pGhost_Solider->Get_Transform();

	return S_OK;;
}

CGhost_Soldier_DownAndUp * CGhost_Soldier_DownAndUp::Create(CGhost_Soldier * pGhost_Soldier)
{
	CGhost_Soldier_DownAndUp*		pInstance = new CGhost_Soldier_DownAndUp();

	if (FAILED(pInstance->Ready_State(pGhost_Soldier)))
	{
		_MSG_BOX(L"CGhost_Soldier_DownAndUp Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Soldier_DownAndUp::Free()
{
}


