#include "stdafx.h"
#include "Ghost_Mage_Wait.h"

#include "Ghost_Mage.h"

_USING(Client)

CGhost_Mage_Wait::CGhost_Mage_Wait()
{
}

HRESULT CGhost_Mage_Wait::Begin()
{
	//Wait animation
	m_pGhost_Mage->Set_AnimationSet(1);
	return S_OK;
}

_int CGhost_Mage_Wait::Update(const _float & fTimeDelta)
{

	return 0;
}

_int Client::CGhost_Mage_Wait::LastUpdate(const _float& fTimeDelta)
{
	return 0;
}
HRESULT CGhost_Mage_Wait::End()
{
	return S_OK;
}

HRESULT CGhost_Mage_Wait::Ready_State(CGhost_Mage * pGhost_Mage)
{
	m_pGhost_Mage = pGhost_Mage;
	return S_OK;;
}

CGhost_Mage_Wait * CGhost_Mage_Wait::Create(CGhost_Mage * pGhost_Mage)
{
	CGhost_Mage_Wait*		pInstance = new CGhost_Mage_Wait();

	if (FAILED(pInstance->Ready_State(pGhost_Mage)))
	{
		_MSG_BOX(L"CGhost_Mage_Wait Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Mage_Wait::Free()
{
}


