#include "stdafx.h"
#include "Flogas_Dead.h"

#include "Flogas.h"
#include "Sound_Manager.h"

_USING(Client)

CFlogas_Dead::CFlogas_Dead()
	:m_pFlogas(nullptr)
	, m_fWaitTime(5.f)
	, m_fTimeAcc(0.f)
	, m_bIsAnimationEnd(false)
{
}

HRESULT CFlogas_Dead::Begin()
{
	m_pFlogas->Set_AnimationSet(18);

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
	CSound_Manager::GetInstance()->Play_Sound(L"Flogas_Die_1.mp3", CSound_Manager::BOSS);
	return S_OK;
}

_int CFlogas_Dead::Update(const _float & fTimeDelta)
{
	if (m_pFlogas->Get_IsAnimationSetEnd())
	{
		m_pFlogas->Set_AnimationSet(18);

		m_pFlogas->Set_IsDead();
	}
	return 0;
}

_int CFlogas_Dead::LastUpdate(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CFlogas_Dead::End()
{
	return S_OK;
}

HRESULT CFlogas_Dead::Ready_State(CFlogas * pFlogas)
{
	m_pFlogas  = pFlogas;
	return S_OK;;
}

CFlogas_Dead * CFlogas_Dead::Create(CFlogas* pFlogas)
{
	CFlogas_Dead*		pInstance = new CFlogas_Dead();

	if (FAILED(pInstance->Ready_State(pFlogas)))
	{
		_MSG_BOX(L"CFlogas_Dead Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlogas_Dead::Free()
{
}


