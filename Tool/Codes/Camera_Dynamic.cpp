#include "stdafx.h"
#include "..\Headers\Camera_Dynamic.h"

#include "Management.h"
#include "Input_Device.h"

_USING(Client)

CCamera_Dynamic::CCamera_Dynamic(LPDIRECT3DDEVICE9 pDevice)
	:CCamera(pDevice)
	, m_fCamSpeed(0.f)
	, m_bIsActivate(false)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	:CCamera(rhs)
	, m_fCamSpeed(rhs.m_fCamSpeed)
	, m_bIsActivate(false)
{

}

HRESULT Client::CCamera_Dynamic::Ready_Prototype()
{
	if (FAILED(CCamera::Ready_Prototype()))
		return E_FAIL;

	Enable();

	return S_OK;
}


HRESULT Client::CCamera_Dynamic::Ready_GameObject()
{
	m_fCamSpeed = 10.f;

	Enable();

	return S_OK;
}


Engine::_int Client::CCamera_Dynamic::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActivate)
		return 0;

	Check_InputKey(fTimeDelta);


	return CCamera::Update_GameObject(fTimeDelta);
}

Engine::_int Client::CCamera_Dynamic::LastUpdate_GameObject(const _float& fTimeDelta)
{

	return 0;
}

void Client::CCamera_Dynamic::Render_GameObject()
{

}


HRESULT CCamera_Dynamic::SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc)
{
	Set_CameraDesc(CamDesc);
	Set_ProjectionDesc(ProjDesc);

	SetUp_Matrix();

	return S_OK;
}

void Client::CCamera_Dynamic::Check_InputKey(const _float& fTimeDelta)
{
	_matrix			matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (m_pInput_Device->Get_KeyPressing(DIK_W))
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fCamSpeed * fTimeDelta;

		m_Camera_Desc.vEye += vLength;
		m_Camera_Desc.vAt += vLength;
	}

	if (m_pInput_Device->Get_KeyPressing(DIK_S))
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fCamSpeed * fTimeDelta;

		m_Camera_Desc.vEye -= vLength;
		m_Camera_Desc.vAt -= vLength;
	}

	if (m_pInput_Device->Get_KeyPressing(DIK_A))
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fCamSpeed * fTimeDelta;

		m_Camera_Desc.vEye -= vLength;
		m_Camera_Desc.vAt -= vLength;
	}

	if (m_pInput_Device->Get_KeyPressing(DIK_D))
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fCamSpeed * fTimeDelta;

		m_Camera_Desc.vEye += vLength;
		m_Camera_Desc.vAt += vLength;
	}

	if (m_pInput_Device->Get_KeyPressing(DIK_Q))
	{
		_vec3		vUp;
		memcpy(&vUp, &matCamWorld.m[1][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vUp, &vUp) * m_fCamSpeed * fTimeDelta;

		m_Camera_Desc.vEye -= vLength;
		m_Camera_Desc.vAt -= vLength;
	}

	if (m_pInput_Device->Get_KeyPressing(DIK_E))
	{
		_vec3		vUp;
		memcpy(&vUp, &matCamWorld.m[1][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vUp, &vUp) * m_fCamSpeed * fTimeDelta;

		m_Camera_Desc.vEye += vLength;
		m_Camera_Desc.vAt += vLength;
	}

	if (m_pInput_Device->Get_MousePressing(CInput_Device::DIM_RB))
	{
		_float fMouseMove = 0;

		if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Y))
		{
			_vec3	vRight;
			memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

			_vec3	vLook = m_Camera_Desc.vAt - m_Camera_Desc.vEye;
			D3DXVec3Normalize(&vLook, &vLook);

			_matrix		matRot;

			D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(fMouseMove / 10.f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_Camera_Desc.vAt = m_Camera_Desc.vEye + vLook;
		}

		if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_X))
		{
			_vec3	vAxis = _vec3(0.f, 1.f, 0.f);
			_vec3	vLook = m_Camera_Desc.vAt - m_Camera_Desc.vEye;

			_matrix		matRot;

			D3DXMatrixRotationAxis(&matRot, &vAxis, D3DXToRadian(fMouseMove / 10.f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_Camera_Desc.vAt = m_Camera_Desc.vEye + vLook;
		}
	}
}


Engine::CGameObject* Client::CCamera_Dynamic::Clone_GameObject()
{
	CCamera_Dynamic*		pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CCamera_Dynamic Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCamera_Dynamic * CCamera_Dynamic::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCamera_Dynamic*		pInstance = new CCamera_Dynamic(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CCamera_Dynamic Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void Client::CCamera_Dynamic::Free()
{
	CCamera::Free();
}

