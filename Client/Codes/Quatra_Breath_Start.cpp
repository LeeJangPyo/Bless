#include "stdafx.h"
#include "Quatra_Breath_Start.h"

#include "Quatra.h"

#include "Management.h"
#include "EffectGroup.h"
#include "Sound_Manager.h"

_USING(Client)

CQuatra_Breath_Start::CQuatra_Breath_Start()
{
}

HRESULT CQuatra_Breath_Start::Begin()
{
	m_pQuatra->Set_AnimationSet(2);
/*
	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
	CSound_Manager::GetInstance()->Play_Sound(L"Quatra_Fly.wav", CSound_Manager::BOSS);*/

	return S_OK;
}

_int CQuatra_Breath_Start::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CQuatra_Breath_Start::LastUpdate(const _float & fTimeDelta)
{
	if (m_pQuatra->Get_IsAnimationSetEnd())
	{
		m_pQuatra->Change_State(L"Breath");
	}


	return _int();
}

HRESULT CQuatra_Breath_Start::End()
{
	return S_OK;
}

HRESULT CQuatra_Breath_Start::Ready_State(CQuatra * pQuatra)
{
	m_pQuatra = pQuatra;

	m_pQuatraTransform = m_pQuatra->Get_Transform();

	return S_OK;
}

CQuatra_Breath_Start * CQuatra_Breath_Start::Create(CQuatra* pQuatra)
{
	CQuatra_Breath_Start*		pInstance = new CQuatra_Breath_Start();

	if (FAILED(pInstance->Ready_State(pQuatra)))
	{
		_MSG_BOX(L"CQuatra_Breath_Start Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuatra_Breath_Start::Free()
{
}


