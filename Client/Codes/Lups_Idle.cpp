#include "stdafx.h"
#include "Lups_Idle.h"

#include "Management.h"
#include "Collision_Manager.h"


//Object
#include "Lups.h"
#include "Sword.h"
#include "Shield.h"


_USING(Client)

CLups_Idle::CLups_Idle()
	:m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

HRESULT CLups_Idle::Begin()
{
	m_pLups->Set_AnimationSet(40);
	
	return S_OK;
}

_int CLups_Idle::Update(const _float & fTimeDelta)
{

	m_pLups->Check_KeyInput_Move(fTimeDelta);
	//m_pLups->Check_KeyInput_Combat(fTimeDelta);
	return 0;
}

_int CLups_Idle::LastUpdate(const _float & fTimeDelta)
{
	Check_KeyInput(fTimeDelta);

	return 0;
}

HRESULT CLups_Idle::End()
{
	return S_OK;
}

void CLups_Idle::Check_KeyInput(const _float & fTimeDelta)
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

HRESULT CLups_Idle::Ready_State(CLups* pLups)
{
	m_pLups = pLups;


	return S_OK;;
}

CLups_Idle * CLups_Idle::Create(CLups* pLups)
{
	CLups_Idle*		pInstance = new CLups_Idle();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_Idle Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_Idle::Free()
{
	Safe_Release(m_pInput_Device);
}


