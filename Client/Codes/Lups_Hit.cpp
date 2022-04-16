#include "stdafx.h"
#include "Lups_Hit.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Lups.h"

_USING(Client)

CLups_Hit::CLups_Hit()
	:m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

HRESULT CLups_Hit::Begin()
{
	m_pLups->Set_AnimationSet(2);

	return S_OK;
}

_int CLups_Hit::Update(const _float & fTimeDelta)
{
	m_pLups->Check_KeyInput_Move(fTimeDelta);
	Check_KeyInput(fTimeDelta);


	return 0;
}

_int CLups_Hit::LastUpdate(const _float & fTimeDelta)
{
	if (m_pLups->Get_IsAnimationSetEnd())
	{
		m_pLups->Change_State(L"Idle");
	}
	return 0;
}

HRESULT CLups_Hit::End()
{
	return S_OK;
}

void CLups_Hit::Check_KeyInput(const _float & fTimeDelta)
{
	if (m_pLups->Get_ControlType() != CONTROL_PLAYER)
		return;

	if (m_pInput_Device->Get_MouseUp(CInput_Device::DIM_LB))
		m_pLups->Change_State(L"Attack");


	if (m_pInput_Device->Get_KeyDown(DIK_1))
		m_pLups->Change_State(L"Shield_Smash");

	if (m_pInput_Device->Get_KeyDown(DIK_2))
		m_pLups->Change_State(L"Spin_Attack");

	if (m_pInput_Device->Get_KeyDown(DIK_3))
		m_pLups->Change_State(L"Guard");

	if (m_pInput_Device->Get_KeyDown(DIK_4))
		m_pLups->Change_State(L"Taunt");

}


HRESULT CLups_Hit::Ready_State(CLups* pLups)
{
	m_pLups = pLups;


	return S_OK;;
}

CLups_Hit * CLups_Hit::Create(CLups* pLups)
{
	CLups_Hit*		pInstance = new CLups_Hit();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_Hit Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_Hit::Free()
{
	Safe_Release(m_pInput_Device);
}


