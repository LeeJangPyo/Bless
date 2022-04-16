#include "stdafx.h"
#include "Flogas_Ready_Combat.h"

#include "Flogas.h"

_USING(Client)

CFlogas_Ready_Combat::CFlogas_Ready_Combat()
{
}

HRESULT CFlogas_Ready_Combat::Begin()
{
	m_pFlogas->Set_AnimationSet(13);

	return S_OK;
}

_int CFlogas_Ready_Combat::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CFlogas_Ready_Combat::LastUpdate(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CFlogas_Ready_Combat::End()
{
	m_pFlogas->Set_HpBar_Activate(true);
	return S_OK;
}

HRESULT CFlogas_Ready_Combat::Ready_State(CFlogas * pFlogas)
{
	m_pFlogas  = pFlogas;
	return S_OK;;
}

CFlogas_Ready_Combat * CFlogas_Ready_Combat::Create(CFlogas* pFlogas)
{
	CFlogas_Ready_Combat*		pInstance = new CFlogas_Ready_Combat();

	if (FAILED(pInstance->Ready_State(pFlogas)))
	{
		_MSG_BOX(L"CFlogas_Ready_Combat Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlogas_Ready_Combat::Free()
{
}


