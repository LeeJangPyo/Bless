#include "stdafx.h"
#include "Ghost_Mage_Hit.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Ghost_Mage.h"
#include "Damage.h"

_USING(Client)

CGhost_Mage_Hit::CGhost_Mage_Hit()
{

}

HRESULT CGhost_Mage_Hit::Begin()
{
	m_pGhost_Mage->Set_AnimationSet(16);

	m_pGhost_Mage->Set_FrameConst(2.5f);
	m_pGhost_Mage->Set_RimLight(1.f);


	return S_OK;
}

_int CGhost_Mage_Hit::Update(const _float & fTimeDelta)
{
	if (m_pGhost_Mage->Get_CurrentHp() < 0)
		m_pGhost_Mage->Change_State(L"Dead");

	return 0;
}

_int CGhost_Mage_Hit::LastUpdate(const _float & fTimeDelta)
{
	if (m_pGhost_Mage->Get_IsAnimationSetEnd())
	{
		m_pGhost_Mage->Change_State(L"Idle");
	}


	return 0;
}

HRESULT CGhost_Mage_Hit::End()
{
	m_pGhost_Mage->Set_FrameConst(1.f);
	m_pGhost_Mage->Set_RimLight(g_fDefaultRimLight);
	return S_OK;
}

HRESULT CGhost_Mage_Hit::Ready_State(CGhost_Mage * pGhost_Mage)
{
	m_pGhost_Mage = pGhost_Mage;


	m_pTransform = m_pGhost_Mage->Get_Transform();

	return S_OK;;
}

CGhost_Mage_Hit * CGhost_Mage_Hit::Create(CGhost_Mage * pGhost_Mage)
{
	CGhost_Mage_Hit*		pInstance = new CGhost_Mage_Hit();

	if (FAILED(pInstance->Ready_State(pGhost_Mage)))
	{
		_MSG_BOX(L"CGhost_Mage_Hit Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Mage_Hit::Free()
{
}


