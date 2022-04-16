#include "stdafx.h"
#include "Ghost_Mage_Ready_Combat.h"

#include "Ghost_Mage.h"

_USING(Client)

CGhost_Mage_Ready_Combat::CGhost_Mage_Ready_Combat()
{
}

HRESULT CGhost_Mage_Ready_Combat::Begin()
{
	m_pGhost_Mage->Set_AnimationSet(12);

	return S_OK;
}

_int CGhost_Mage_Ready_Combat::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CGhost_Mage_Ready_Combat::LastUpdate(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CGhost_Mage_Ready_Combat::End()
{
	return S_OK;
}

HRESULT CGhost_Mage_Ready_Combat::Ready_State(CGhost_Mage * pGhost_Mage)
{
	m_pGhost_Mage  = pGhost_Mage;
	return S_OK;
}

CGhost_Mage_Ready_Combat * CGhost_Mage_Ready_Combat::Create(CGhost_Mage* pGhost_Mage)
{
	CGhost_Mage_Ready_Combat*		pInstance = new CGhost_Mage_Ready_Combat();

	if (FAILED(pInstance->Ready_State(pGhost_Mage)))
	{
		_MSG_BOX(L"CGhost_Mage_Ready_Combat Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Mage_Ready_Combat::Free()
{
}


