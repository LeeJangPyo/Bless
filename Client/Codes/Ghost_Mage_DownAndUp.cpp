#include "stdafx.h"
#include "Ghost_Mage_DownAndUp.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Ghost_Mage.h"
#include "Damage.h"

_USING(Client)

CGhost_Mage_DownAndUp::CGhost_Mage_DownAndUp()
	:m_iAnimationIndex(0)
{

}

HRESULT CGhost_Mage_DownAndUp::Begin()
{
	m_iAnimationIndex = 9;
	m_pGhost_Mage->Set_AnimationSet(m_iAnimationIndex,true);
	
	m_pGhost_Mage->Set_FrameConst(1.f);

	return S_OK;
}

_int CGhost_Mage_DownAndUp::Update(const _float & fTimeDelta)
{
	if (m_pGhost_Mage->Get_CurrentHp() < 0)
		m_pGhost_Mage->Change_State(L"Dead");

	return 0;
}

_int CGhost_Mage_DownAndUp::LastUpdate(const _float & fTimeDelta)
{
	if (m_pGhost_Mage->Get_IsAnimationSetEnd() && m_iAnimationIndex == 9)
	{
		m_iAnimationIndex = 6;
		m_pGhost_Mage->Set_AnimationSet(m_iAnimationIndex);
	}

	if (m_pGhost_Mage->Get_IsAnimationSetEnd() && m_iAnimationIndex == 6)
	{
		m_pGhost_Mage->Change_State(L"Idle");
	}


	return 0;
}

HRESULT CGhost_Mage_DownAndUp::End()
{
	m_pGhost_Mage->Set_FrameConst(1.f);

	return S_OK;
}

HRESULT CGhost_Mage_DownAndUp::Ready_State(CGhost_Mage * pGhost_Mage)
{
	m_pGhost_Mage = pGhost_Mage;


	m_pTransform = m_pGhost_Mage->Get_Transform();

	return S_OK;;
}

CGhost_Mage_DownAndUp * CGhost_Mage_DownAndUp::Create(CGhost_Mage * pGhost_Mage)
{
	CGhost_Mage_DownAndUp*		pInstance = new CGhost_Mage_DownAndUp();

	if (FAILED(pInstance->Ready_State(pGhost_Mage)))
	{
		_MSG_BOX(L"CGhost_Mage_DownAndUp Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Mage_DownAndUp::Free()
{
}


