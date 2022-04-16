#include "stdafx.h"
#include "Ghost_Mage_Dead.h"

#include "Management.h"


#include "Ghost_Mage.h"

_USING(Client)

CGhost_Mage_Dead::CGhost_Mage_Dead()
	:m_fWaitTime(1.f),
	m_fTimeAcc(0.f),
	m_bIsAnimationEnd(false)
{


}

HRESULT CGhost_Mage_Dead::Begin()
{

	m_pGhost_Mage->Set_AnimationSet(15);



	return S_OK;
}

_int CGhost_Mage_Dead::Update(const _float & fTimeDelta)
{
	//애니메이션이 끝나면 검사
	if (m_pGhost_Mage->Get_IsAnimationSetEnd())
	{
		m_pGhost_Mage->Set_AnimationSet(18);
		m_bIsAnimationEnd = true;
	}

	if (m_bIsAnimationEnd)
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc > m_fWaitTime)
		{
			//사라짐
			m_pGhost_Mage->Set_IsDead();
		}
	}
	return 0;
}

_int CGhost_Mage_Dead::LastUpdate(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CGhost_Mage_Dead::End()
{
	return S_OK;
}

HRESULT CGhost_Mage_Dead::Ready_State(CGhost_Mage * pGhost_Mage)
{
	m_pGhost_Mage = pGhost_Mage;

	m_pTransform = m_pGhost_Mage->Get_Transform();

	m_fWaitTime = 1.f;
	m_bIsAnimationEnd = false;

	return S_OK;;
}

CGhost_Mage_Dead * CGhost_Mage_Dead::Create(CGhost_Mage * pGhost_Mage)
{
	CGhost_Mage_Dead*		pInstance = new CGhost_Mage_Dead();

	if (FAILED(pInstance->Ready_State(pGhost_Mage)))
	{
		_MSG_BOX(L"CGhost_Mage_Dead Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Mage_Dead::Free()
{
}


