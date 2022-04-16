#include "stdafx.h"
#include "..\Headers\Camera_Eye.h"

#include "Management.h"

#include "Input_Device.h"


_USING(Client)

Client::CCamera_Eye::CCamera_Eye(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
	, m_bIsPick(false)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pCalculatorCom(nullptr)
	, m_pColliderCom(nullptr)
{
	m_pDevice->AddRef();
	m_pInput_Device->AddRef();
}

HRESULT Client::CCamera_Eye::Ready_Eye(const _vec3& vPosition)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_vPosition = vPosition;
	m_pColliderCom->SetUp_Collider_Spehre(&m_vPosition, 1.f);

	return S_OK;
}

_int Client::CCamera_Eye::Update(const _float& fTimeDelta)
{

	m_pColliderCom->Update_Collider_Sphere();

	return 0;
}

void Client::CCamera_Eye::Render()
{
	//m_pColliderCom->Render_Collider_Sphere();
}

HRESULT CCamera_Eye::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (m_bIsPick)
	{
		if (FAILED(m_pColliderCom->SetUp_ConstantTable(pEffect,CCollider_Sphere::COLOR_GREEN)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pColliderCom->SetUp_ConstantTable(pEffect, CCollider_Sphere::COLOR_RED)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT Client::CCamera_Eye::Ready_Component()
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Calculator
	m_pCalculatorCom = dynamic_cast<CCalculator*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Calculator"));
	if (m_pCalculatorCom == nullptr)
		return E_FAIL;

	//For. Shader;
	m_pColliderCom = dynamic_cast<CCollider_Sphere*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Sphere"));
	if(m_pColliderCom == nullptr)
		return E_FAIL;


	Safe_Release(pManagement);
	return S_OK;
}



Client::CCamera_Eye* Client::CCamera_Eye::Create(LPDIRECT3DDEVICE9 pDevice, const _vec3& vPosition)
{
	CCamera_Eye*	pInstance = new CCamera_Eye(pDevice);

	if (FAILED(pInstance->Ready_Eye(vPosition)))
	{
		_MSG_BOX(L"CCamera_Eye Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CCamera_Eye::Free()
{
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pCalculatorCom);
	Safe_Release(m_pDevice);
}

const Engine::_vec3* Client::CCamera_Eye::Get_Position()
{
	return &m_vPosition;
}
