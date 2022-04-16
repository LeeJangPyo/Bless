#include "stdafx.h"
#include "..\Headers\Camera_Static.h"

#include "Management.h"

#include "Player_Manager.h"

_USING(Client)


CCamera_Static::CCamera_Static(LPDIRECT3DDEVICE9 pDevice)
	:CCamera(pDevice)
	, m_pCalculatorCom(nullptr)
	, m_bIsShakeing(false)
	, m_fShakingTime(0.f)
	, m_fTimeAcc(0.f)
{
}

CCamera_Static::CCamera_Static(const CCamera_Static & rhs)
	:CCamera(rhs)
	, m_bIsShakeing(false)
	, m_fShakingTime(0.f)
	, m_fTimeAcc(0.f)
{
}

HRESULT Client::CCamera_Static::Ready_Prototype()
{
	if (FAILED(CCamera::Ready_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Client::CCamera_Static::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	m_vPrevTarget_Pos = _vec3(0.f, 0.f, 0.f);
	m_vTarget_Pos = _vec3(0.f, 0.f, 0.f);

	return S_OK;
}


Engine::_int Client::CCamera_Static::Update_GameObject(const _float& fTimeDelta)
{
	Check_KeyInput(fTimeDelta);
	


	//0.5, 2.5,4.5;
	m_vPrevTarget_Pos = m_vTarget_Pos;

	m_vTarget_Pos = *CPlayer_Manager::GetInstance()->Get_Camera_Eye_Position();

	m_Camera_Desc.vAt = *CPlayer_Manager::GetInstance()->Get_Camera_At_Position();

	if (m_bIsShakeing)
	{
		_float fX = sinf(/*m_fShakingTime -*/ m_fTimeAcc *75.f)*0.25f * powf(0.5f, /*m_fShakingTime -*/ m_fTimeAcc);
		_float fY = cosf(/*m_fShakingTime -*/ m_fTimeAcc *75.f)*0.25f * powf(0.5f, /*m_fShakingTime -*/ m_fTimeAcc);

		m_Camera_Desc.vEye.x += fX;
		m_Camera_Desc.vEye.y += fY;

		m_Camera_Desc.vAt.x += fX;
		m_Camera_Desc.vAt.y += fY;

		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc > m_fShakingTime)
		{
			m_bIsShakeing = false;
			m_fTimeAcc = 0.f;
			m_fShakingTime = 0.f;
		}
	}

	m_Camera_Desc.vEye = m_pCalculatorCom->SpringDamp(&m_Camera_Desc.vEye, &m_vTarget_Pos, &m_vPrevTarget_Pos, fTimeDelta, 2.5f, 0.25f, 1.f);
	
	return CCamera::Update_GameObject(fTimeDelta);
}

Engine::_int Client::CCamera_Static::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void Client::CCamera_Static::Render_GameObject()
{
	
}

HRESULT CCamera_Static::SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc)
{
	Set_CameraDesc(CamDesc);
	Set_ProjectionDesc(ProjDesc);
	SetUp_Matrix();


	return S_OK;
}

void CCamera_Static::Set_CameraShaking(const _float & fShakeTime)
{
	m_fShakingTime = fShakeTime;
	m_bIsShakeing = true;
}

HRESULT CCamera_Static::Ready_Component()
{
	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Calculator
	m_pCalculatorCom = dynamic_cast<CCalculator*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Calculator"));
	if (FAILED(Add_Component(L"Com_Mesh", m_pCalculatorCom)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

void CCamera_Static::Check_KeyInput(const _float & fTimeDelta)
{
	if (m_pInput_Device->Get_MouseDown(CInput_Device::DIM_RB))
	{
		m_bIsRButtonDown = true;
	}
	if (m_pInput_Device->Get_MousePressing(CInput_Device::DIM_RB))
	{
		if (m_bIsRButtonDown)
		{
			_float fMouseMove = 0.f;
			if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Y))
			{
				//m_vPrevTarget_Pos.y += fMouseMove;
			}
			fMouseMove = 0.f;
			if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_X))
			{
				//m_Camera_Desc.vAt = *CPlayer_Manager::GetInstance()->Get_PlayerPosition();
				m_Camera_Desc.vAt.y += fMouseMove;
			}
		}
	}
	if (m_pInput_Device->Get_MouseUp(CInput_Device::DIM_RB))
	{
		m_bIsRButtonDown = false;
	}
}

Engine::CGameObject* Client::CCamera_Static::Clone_GameObject()
{
	CCamera_Static*		pInstance = new CCamera_Static(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CCamera_Static Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCamera_Static * CCamera_Static::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCamera_Static*		pInstance = new CCamera_Static(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CCamera_Static Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CCamera_Static::Free()
{
	Safe_Release(m_pCalculatorCom);

	CCamera::Free();
}
