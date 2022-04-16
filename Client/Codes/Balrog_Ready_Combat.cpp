#include "stdafx.h"
#include "Balrog_Ready_Combat.h"

#include "Balrog.h"

_USING(Client)

CBalrog_Ready_Combat::CBalrog_Ready_Combat()
{
}

HRESULT CBalrog_Ready_Combat::Begin()
{
	m_pBalrog->Set_AnimationSet(2);

	return S_OK;
}

_int CBalrog_Ready_Combat::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CBalrog_Ready_Combat::LastUpdate(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CBalrog_Ready_Combat::End()
{
	return S_OK;
}

HRESULT CBalrog_Ready_Combat::Ready_State(CBalrog * pBalrog)
{
	m_pBalrog  = pBalrog;
	return S_OK;;
}

CBalrog_Ready_Combat * CBalrog_Ready_Combat::Create(CBalrog* pBalrog)
{
	CBalrog_Ready_Combat*		pInstance = new CBalrog_Ready_Combat();

	if (FAILED(pInstance->Ready_State(pBalrog)))
	{
		_MSG_BOX(L"CBalrog_Ready_Combat Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalrog_Ready_Combat::Free()
{
}


