#include "stdafx.h"
#include "Sire_Idle.h"

#include "Management.h"
#include "Collision_Manager.h"


//Object
#include "Sire.h"


_USING(Client)

CSire_Idle::CSire_Idle()
	:m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

HRESULT CSire_Idle::Begin()
{
	m_pSire->Set_AnimationSet(50);
	
	return S_OK;
}

_int CSire_Idle::Update(const _float & fTimeDelta)
{
	m_pSire->Check_KeyInput_Move(fTimeDelta);

	Check_KeyInput(fTimeDelta);

	return 0;
}

_int CSire_Idle::LastUpdate(const _float & fTimeDelta)
{
	if (m_pSire->Get_IsAnimationSetEnd())
	{
		m_pSire->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CSire_Idle::End()
{
	return S_OK;
}

void CSire_Idle::Check_KeyInput(const _float & fTimeDelta)
{
	if (m_pSire->Get_ControlType() != CONTROL_PLAYER)
		return;

	if (m_pInput_Device->Get_KeyDown(DIK_1))
		m_pSire->Change_State(L"FireShock");

	if (m_pInput_Device->Get_KeyDown(DIK_2))
		m_pSire->Change_State(L"Ready_FireBall");

	if (m_pInput_Device->Get_KeyDown(DIK_3))
		m_pSire->Change_State(L"Meteo_Target");

	if (m_pInput_Device->Get_KeyDown(DIK_4))
		m_pSire->Change_State(L"Heal_Target");

}

HRESULT CSire_Idle::Ready_State(CSire* pSire)
{
	m_pSire = pSire;


	return S_OK;;
}

CSire_Idle * CSire_Idle::Create(CSire* pSire)
{
	CSire_Idle*		pInstance = new CSire_Idle();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_Idle Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_Idle::Free()
{
	Safe_Release(m_pInput_Device);
}


