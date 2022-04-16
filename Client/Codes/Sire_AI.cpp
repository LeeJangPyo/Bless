#include "stdafx.h"
#include "Sire_AI.h"

#include "Management.h"
#include "Collision_Manager.h"


#include "Player_Manager.h"

//Object
#include "Sire.h"


_USING(Client)

CSire_AI::CSire_AI()
	:m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

HRESULT CSire_AI::Begin()
{
	m_pSire->Set_AnimationSet(42);
	
	return S_OK;
}

_int CSire_AI::Update(const _float & fTimeDelta)
{
	m_pSire_Transform->LookAt_Target(CPlayer_Manager::GetInstance()->Get_Player_Position(), fTimeDelta);
	//true is wait
	if (m_pSire_Transform->Go_Target(CPlayer_Manager::GetInstance()->Get_Player_Position(), m_pSire->Get_BaseStats()->fMoveSpeed, fTimeDelta, 5.0f))
	{
		m_pSire->Set_AnimationSet(50);
	}
	else
	{
		m_pSire->Set_AnimationSet(42);
	}

	return 0;
}

_int CSire_AI::LastUpdate(const _float & fTimeDelta)
{

	return 0;
}

HRESULT CSire_AI::End()
{
	return S_OK;
}


HRESULT CSire_AI::Ready_State(CSire* pSire)
{
	m_pSire = pSire;

	m_pSire_Transform = m_pSire->Get_Transform();

	return S_OK;;
}

CSire_AI * CSire_AI::Create(CSire* pSire)
{
	CSire_AI*		pInstance = new CSire_AI();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_AI Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_AI::Free()
{
	Safe_Release(m_pInput_Device);
}


