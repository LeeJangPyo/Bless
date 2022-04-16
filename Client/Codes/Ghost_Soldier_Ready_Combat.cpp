#include "stdafx.h"
#include "Ghost_Soldier_Ready_Combat.h"



#include "Ghost_Soldier.h"

_USING(Client)

CGhost_Soldier_Ready_Combat::CGhost_Soldier_Ready_Combat()
{
}

HRESULT CGhost_Soldier_Ready_Combat::Begin()
{
	m_pGhost_Solider->Set_AnimationSet(16);

	return S_OK;
}

_int CGhost_Soldier_Ready_Combat::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CGhost_Soldier_Ready_Combat::LastUpdate(const _float & fTimeDelta)
{
	return 0;
}

HRESULT CGhost_Soldier_Ready_Combat::End()
{
	return S_OK;
}

HRESULT CGhost_Soldier_Ready_Combat::Ready_State(CGhost_Soldier * pGhost_Soldier)
{
	m_pGhost_Solider = pGhost_Soldier;
	return S_OK;;
}

CGhost_Soldier_Ready_Combat * CGhost_Soldier_Ready_Combat::Create(CGhost_Soldier * pGhost_Soldier)
{
	CGhost_Soldier_Ready_Combat*		pInstance = new CGhost_Soldier_Ready_Combat();

	if (FAILED(pInstance->Ready_State(pGhost_Soldier)))
	{
		_MSG_BOX(L"CGhost_Soldier_Ready_Combat Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Soldier_Ready_Combat::Free()
{
}


