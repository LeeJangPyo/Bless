#include "stdafx.h"
#include "..\Headers\Camera_At.h"

#include "Management.h"

#include "Input_Device.h"
_USING(Client)


CCamera_At::CCamera_At(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
	,m_bIsPick(false)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pCalculatorCom(nullptr)
	, m_pColliderCom(nullptr)
{
	m_pDevice->AddRef();
	m_pInput_Device->AddRef();
}

HRESULT CCamera_At::Ready_At(const _vec3 & vPosition)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_vPosition = vPosition;
	D3DXMatrixTranslation(&m_matWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_pColliderCom->SetUp_Collider_Box();

	return S_OK;
}

_int CCamera_At::Update(const _float & fTimeDelta)
{


	D3DXMatrixTranslation(&m_matWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	return 0;
}

void CCamera_At::Render()
{
	m_pColliderCom->Render_Collider_Box();
}

HRESULT CCamera_At::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (m_bIsPick)
	{
		if (FAILED(m_pColliderCom->SetUp_ConstantTable(pEffect, CCollider_Box::COLOR_GREEN, &m_matWorld)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pColliderCom->SetUp_ConstantTable(pEffect, CCollider_Box::COLOR_RED, &m_matWorld)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CCamera_At::Ready_Component()
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
	m_pColliderCom = dynamic_cast<CCollider_Box*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box"));
	if (m_pColliderCom == nullptr)
		return E_FAIL;


	Safe_Release(pManagement);
	return S_OK;
}

CCamera_At * CCamera_At::Create(LPDIRECT3DDEVICE9 pDevice, const _vec3 & vPosition)
{
	CCamera_At*	pInstance = new CCamera_At(pDevice);

	if (FAILED(pInstance->Ready_At(vPosition)))
	{
		_MSG_BOX(L"CCamera_Eye Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_At::Free()
{
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pCalculatorCom);
	Safe_Release(m_pDevice);
}
const Engine::_vec3* Client::CCamera_At::Get_Position()
{
	return (_vec3*)&m_matWorld._41;
}