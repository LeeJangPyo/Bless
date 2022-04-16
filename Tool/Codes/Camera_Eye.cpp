#include "stdafx.h"
#include "..\Headers\Camera_Eye.h"

#include "Management.h"

#include "Input_Device.h"
#include "CameraTab.h"

_USING(Client)

Client::CCamera_Eye::CCamera_Eye(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
	, m_bIsPick(false)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pCalculatorCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_pCameraTab(nullptr)
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
	Check_KeyInput(fTimeDelta);

//	D3DXMatrixTranslation(&m_matWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_pColliderCom->Set_Center_Position(&m_vPosition);
	m_pColliderCom->Update_Collider_Sphere();

	return 0;
}

void Client::CCamera_Eye::Render()
{
	m_pColliderCom->Render_Collider_Sphere();
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

void CCamera_Eye::Check_KeyInput(const _float & fTimeDelta)
{
	//wheel
	if (m_bIsPick)
	{
		_float fMouseMove = 0.f;

		if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Z))
		{
			m_vPosition.y += fMouseMove * 0.01f;
			m_pCameraTab->Update_CameraPosition(&m_vPosition);
		}

		//m_pMeshTab->Update_Transform(CMeshTab::DATA_POSITION, &vPosition);
	}

	if (m_pInput_Device->Get_MouseDown(CInput_Device::DIM_LB))
	{
		//Transform MosuePos to World Ray
		RAY	tRay = m_pCalculatorCom->Transform_ScreenPosToRay(g_hWnd);


		if (m_pCalculatorCom->Picking_Sphere(tRay, m_pColliderCom))
		{
			m_bIsPick = true;
			m_bIsMouseDown = true;
//			m_pMeshTab->m_pPickedMeshObject = this;
			m_pCameraTab->Update_CameraPosition(&m_vPosition);
		}
		else
		{
			m_bIsPick = false;

		}
	}
	if (m_pInput_Device->Get_MousePressing(CInput_Device::DIM_LB))
	{
		//
		if (m_bIsMouseDown)
		{
		
			_float fMouseMove = 0.f;
			if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Y))
			{
				m_vPosition.z -= fMouseMove * 0.01f;
				m_pCameraTab->Update_CameraPosition(&m_vPosition);
			}

			fMouseMove = 0.f;
			if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_X))
			{
				m_vPosition.x += fMouseMove * 0.01f;
				m_pCameraTab->Update_CameraPosition(&m_vPosition);
			}


			//Update MFC Data
//			m_pMeshTab->Update_Transform(CMeshTab::DATA_POSITION, &vPosition);
		}
	}

	if (m_pInput_Device->Get_MouseUp(CInput_Device::DIM_LB))
	{
		m_bIsMouseDown = false;
	//	m_pMeshTab->Update_Transform(CMeshTab::DATA_POSITION, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));
		m_pCameraTab->Update_CameraPosition(&m_vPosition);
	}
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
	//return (_vec3*)&m_matWorld._41;
	return &m_vPosition;
}

void CCamera_Eye::Set_CameraTab(CCameraTab * pCameraTab)
{
	m_pCameraTab = pCameraTab;
}
