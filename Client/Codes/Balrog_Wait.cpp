#include "stdafx.h"
#include "Balrog_Wait.h"

#include "Balrog.h"

_USING(Client)

CBalrog_Wait::CBalrog_Wait()
{
}

HRESULT CBalrog_Wait::Begin()
{
	//Wait animation
	m_pBarlog->Set_AnimationSet(10);
	return S_OK;
}

_int CBalrog_Wait::Update(const _float & fTimeDelta)
{

	return 0;
}

_int Client::CBalrog_Wait::LastUpdate(const _float& fTimeDelta)
{
	return 0;
}
HRESULT CBalrog_Wait::End()
{
	return S_OK;
}

HRESULT CBalrog_Wait::Ready_State(CBalrog * pBarlog)
{
	m_pBarlog = pBarlog;
	return S_OK;;
}

CBalrog_Wait * CBalrog_Wait::Create(CBalrog * pBarlog)
{
	CBalrog_Wait*		pInstance = new CBalrog_Wait();

	if (FAILED(pInstance->Ready_State(pBarlog)))
	{
		_MSG_BOX(L"CBalrog_Wait Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalrog_Wait::Free()
{
}


