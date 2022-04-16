#include "stdafx.h"
#include "Sire_Hit.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Sire.h"

_USING(Client)

CSire_Hit::CSire_Hit()
	:m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

HRESULT CSire_Hit::Begin()
{
	m_pSire->Set_AnimationSet(59);

	return S_OK;
}

_int CSire_Hit::Update(const _float & fTimeDelta)
{
	m_pSire->Check_KeyInput_Move(fTimeDelta);

	return 0;
}

_int CSire_Hit::LastUpdate(const _float & fTimeDelta)
{
	if (m_pSire->Get_IsAnimationSetEnd())
	{
		m_pSire->Change_State(L"Idle");
	}
	return 0;
}

HRESULT CSire_Hit::End()
{
	return S_OK;
}

void CSire_Hit::Check_KeyInput(const _float & fTimeDelta)
{
	if (m_pSire->Get_ControlType() != CONTROL_PLAYER)
		return;

	if (m_pInput_Device->Get_KeyDown(DIK_1))
		m_pSire->Change_State(L"FireShock");

	if (m_pInput_Device->Get_KeyDown(DIK_2))
		m_pSire->Change_State(L"Ready_FireBall");

	if (m_pInput_Device->Get_KeyDown(DIK_3))
		m_pSire->Change_State(L"Meteo_Target");

}


HRESULT CSire_Hit::Ready_State(CSire* pSire)
{
	m_pSire = pSire;


	return S_OK;;
}

CSire_Hit * CSire_Hit::Create(CSire* pSire)
{
	CSire_Hit*		pInstance = new CSire_Hit();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_Hit Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_Hit::Free()
{
	Safe_Release(m_pInput_Device);
}


