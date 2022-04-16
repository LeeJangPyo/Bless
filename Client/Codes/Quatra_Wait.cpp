#include "stdafx.h"
#include "Quatra_Wait.h"

#include "Quatra.h"

_USING(Client)

CQuatra_Wait::CQuatra_Wait()
{
}

HRESULT CQuatra_Wait::Begin()
{
	m_pQuatra->Set_AnimationSet(14);

	return S_OK;
}

_int CQuatra_Wait::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CQuatra_Wait::LastUpdate(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CQuatra_Wait::End()
{
	return S_OK;
}

HRESULT CQuatra_Wait::Ready_State(CQuatra * pQuatra)
{
	m_pQuatra  = pQuatra;
	return S_OK;;
}

CQuatra_Wait * CQuatra_Wait::Create(CQuatra* pQuatra)
{
	CQuatra_Wait*		pInstance = new CQuatra_Wait();

	if (FAILED(pInstance->Ready_State(pQuatra)))
	{
		_MSG_BOX(L"CQuatra_Wait Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuatra_Wait::Free()
{
}


