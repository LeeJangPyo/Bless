#include "stdafx.h"
#include "Ghost_Soldier_Wait.h"

#include "Ghost_Soldier.h"

_USING(Client)

CGhost_Soldier_Wait::CGhost_Soldier_Wait()
{
}

HRESULT CGhost_Soldier_Wait::Begin()
{
	m_pGhost_Solider->Set_AnimationSet(3);
	return S_OK;
}

_int CGhost_Soldier_Wait::Update(const _float & fTimeDelta)
{

	return 0;
}

_int Client::CGhost_Soldier_Wait::LastUpdate(const _float& fTimeDelta)
{
	return 0;
}
HRESULT CGhost_Soldier_Wait::End()
{
	return S_OK;
}

HRESULT CGhost_Soldier_Wait::Ready_State(CGhost_Soldier * pGhost_Soldier)
{
	m_pGhost_Solider = pGhost_Soldier;
	return S_OK;;
}

CGhost_Soldier_Wait * CGhost_Soldier_Wait::Create(CGhost_Soldier * pGhost_Soldier)
{
	CGhost_Soldier_Wait*		pInstance = new CGhost_Soldier_Wait();

	if (FAILED(pInstance->Ready_State(pGhost_Soldier)))
	{
		_MSG_BOX(L"CGhost_Soldier_Wait Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Soldier_Wait::Free()
{
}


