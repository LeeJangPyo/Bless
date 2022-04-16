#include "stdafx.h"
#include "Ghost_Soldier_Hit.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Ghost_Soldier.h"
#include "Damage.h"

_USING(Client)

CGhost_Soldier_Hit::CGhost_Soldier_Hit()
{

}

HRESULT CGhost_Soldier_Hit::Begin()
{
	//m_pGhost_Solider->Reset_AnimationSet();
	m_pGhost_Solider->Set_AnimationSet(12,true);
	
	m_pGhost_Solider->Set_FrameConst(2.5f);
	m_pGhost_Solider->Set_RimLight(1.f);
	

	return S_OK;
}

_int CGhost_Soldier_Hit::Update(const _float & fTimeDelta)
{
	if (m_pGhost_Solider->Get_CurrentHp() < 0)
		m_pGhost_Solider->Change_State(L"Dead");

	return 0;
}

_int CGhost_Soldier_Hit::LastUpdate(const _float & fTimeDelta)
{
	if (m_pGhost_Solider->Get_IsAnimationSetEnd())
	{
		m_pGhost_Solider->Change_State(L"Idle");
	}


	return 0;
}

HRESULT CGhost_Soldier_Hit::End()
{
	m_pGhost_Solider->Set_FrameConst(1.f);

	m_pGhost_Solider->Set_RimLight(g_fDefaultRimLight);
	return S_OK;
}

HRESULT CGhost_Soldier_Hit::Ready_State(CGhost_Soldier * pGhost_Soldier)
{
	m_pGhost_Solider = pGhost_Soldier;


	m_pTransform = m_pGhost_Solider->Get_Transform();

	return S_OK;;
}

CGhost_Soldier_Hit * CGhost_Soldier_Hit::Create(CGhost_Soldier * pGhost_Soldier)
{
	CGhost_Soldier_Hit*		pInstance = new CGhost_Soldier_Hit();

	if (FAILED(pInstance->Ready_State(pGhost_Soldier)))
	{
		_MSG_BOX(L"CGhost_Soldier_Hit Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Soldier_Hit::Free()
{
}


