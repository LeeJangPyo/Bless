#include "stdafx.h"
#include "Balrog_Dead.h"

#include "Management.h"


#include "Balrog.h"

_USING(Client)

CBalrog_Dead::CBalrog_Dead()
{

}

HRESULT CBalrog_Dead::Begin()
{

	m_pBalrog->Set_AnimationSet(26);
	return S_OK;
}

_int CBalrog_Dead::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CBalrog_Dead::LastUpdate(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CBalrog_Dead::End()
{
	return S_OK;
}

HRESULT CBalrog_Dead::Ready_State(CBalrog * pBalrog)
{
	m_pBalrog = pBalrog;

	m_pTransform = m_pBalrog->Get_Transform();

	return S_OK;;
}

CBalrog_Dead * CBalrog_Dead::Create(CBalrog * pBalrog)
{
	CBalrog_Dead*		pInstance = new CBalrog_Dead();

	if (FAILED(pInstance->Ready_State(pBalrog)))
	{
		_MSG_BOX(L"CBalrog_Dead Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalrog_Dead::Free()
{
}


