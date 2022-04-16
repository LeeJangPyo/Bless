#include "stdafx.h"
#include "Quatra_Ready_Combat.h"

#include "Quatra.h"
#include "Sound_Manager.h"

_USING(Client)

CQuatra_Ready_Combat::CQuatra_Ready_Combat()
	:m_bPlayOnce(false)
{
}

HRESULT CQuatra_Ready_Combat::Begin()
{

	m_bPlayOnce = false;
	m_pQuatra->Set_AnimationSet(5);

	return S_OK;
}

_int CQuatra_Ready_Combat::Update(const _float & fTimeDelta)
{
	if (m_pQuatra->Get_IsAnimationPassRadio(0.5f))
	{
		if (m_bPlayOnce == false)
		{
			CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
			CSound_Manager::GetInstance()->Play_Sound(L"Quatra_Ready_Combat.mp3", CSound_Manager::BOSS);
			m_bPlayOnce = true;
		}
	}

	return 0;
}

_int CQuatra_Ready_Combat::LastUpdate(const _float & fTimeDelta)
{
	if (m_pQuatra->Get_IsAnimationSetEnd())
	{
		m_pQuatra->Change_State(L"Breath_Start");
	}

	return _int();
}

HRESULT CQuatra_Ready_Combat::End()
{
	//CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
	//CSound_Manager::GetInstance()->Play_Sound(L"Quatra_Fly.wav", CSound_Manager::BOSS);
	return S_OK;
}

HRESULT CQuatra_Ready_Combat::Ready_State(CQuatra * pQuatra)
{
	m_pQuatra  = pQuatra;
	return S_OK;;
}

CQuatra_Ready_Combat * CQuatra_Ready_Combat::Create(CQuatra* pQuatra)
{
	CQuatra_Ready_Combat*		pInstance = new CQuatra_Ready_Combat();

	if (FAILED(pInstance->Ready_State(pQuatra)))
	{
		_MSG_BOX(L"CQuatra_Ready_Combat Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuatra_Ready_Combat::Free()
{
}


