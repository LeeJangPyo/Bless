#include "stdafx.h"
#include "Lups_AI.h"

#include "Management.h"
#include "Collision_Manager.h"


#include "Player_Manager.h"
//Object
#include "Lups.h"
#include "Sword.h"
#include "Shield.h"


_USING(Client)

CLups_AI::CLups_AI()
	:m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

HRESULT CLups_AI::Begin()
{
	m_pLups->Set_AnimationSet(40);
	
	return S_OK;
}

_int CLups_AI::Update(const _float & fTimeDelta)
{

	m_pLups_Transform->LookAt_Target(CPlayer_Manager::GetInstance()->Get_Player_Position(), fTimeDelta);
	//true is wait
	if (m_pLups_Transform->Go_Target(CPlayer_Manager::GetInstance()->Get_Player_Position(), m_pLups->Get_BaseStats()->fMoveSpeed, fTimeDelta, 5.0f))
	{
		m_pLups->Set_AnimationSet(40);
	}
	else
	{
		m_pLups->Set_AnimationSet(31);
	}


	return 0;
}

_int CLups_AI::LastUpdate(const _float & fTimeDelta)
{

	return 0;
}

HRESULT CLups_AI::End()
{
	return S_OK;
}


HRESULT CLups_AI::Ready_State(CLups* pLups)
{
	m_pLups = pLups;


	m_pLups_Transform = m_pLups->Get_Transform();

	return S_OK;;
}

CLups_AI * CLups_AI::Create(CLups* pLups)
{
	CLups_AI*		pInstance = new CLups_AI();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_AI Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_AI::Free()
{
	Safe_Release(m_pInput_Device);
}


