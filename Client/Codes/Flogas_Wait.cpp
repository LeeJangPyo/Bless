#include "stdafx.h"
#include "Flogas_Wait.h"

#include "Flogas.h"

_USING(Client)

CFlogas_Wait::CFlogas_Wait()
{
}

HRESULT CFlogas_Wait::Begin()
{
	m_pFlogas->Set_AnimationSet(14);

	return S_OK;
}

_int CFlogas_Wait::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CFlogas_Wait::LastUpdate(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CFlogas_Wait::End()
{
	return S_OK;
}

HRESULT CFlogas_Wait::Ready_State(CFlogas * pFlogas)
{
	m_pFlogas  = pFlogas;
	return S_OK;;
}

CFlogas_Wait * CFlogas_Wait::Create(CFlogas* pFlogas)
{
	CFlogas_Wait*		pInstance = new CFlogas_Wait();

	if (FAILED(pInstance->Ready_State(pFlogas)))
	{
		_MSG_BOX(L"CFlogas_Wait Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlogas_Wait::Free()
{
}


