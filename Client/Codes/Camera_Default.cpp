#include "stdafx.h"
#include "..\Headers\Camera_Default.h"

#include "Management.h"
#include "Input_Device.h"

_USING(Client)

CCamera_Default::CCamera_Default(LPDIRECT3DDEVICE9 pDevice)
	:CCamera(pDevice)
{
}

CCamera_Default::CCamera_Default(const CCamera_Default & rhs)
	: CCamera(rhs)
{

}

HRESULT Client::CCamera_Default::Ready_Prototype()
{
	if (FAILED(CCamera::Ready_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT Client::CCamera_Default::Ready_GameObject()
{
	return S_OK;
}


Engine::_int Client::CCamera_Default::Update_GameObject(const _float& fTimeDelta)
{
	return CCamera::Update_GameObject(fTimeDelta);
}

Engine::_int Client::CCamera_Default::LastUpdate_GameObject(const _float& fTimeDelta)
{

	return 0;
}

void Client::CCamera_Default::Render_GameObject()
{

}

HRESULT CCamera_Default::SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc)
{
	Set_CameraDesc(CamDesc);
	Set_ProjectionDesc(ProjDesc);

	SetUp_Matrix();

	return S_OK;
}



Engine::CGameObject* Client::CCamera_Default::Clone_GameObject()
{
	CCamera_Default*		pInstance = new CCamera_Default(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CCamera_Default Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCamera_Default * CCamera_Default::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCamera_Default*		pInstance = new CCamera_Default(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CCamera_Default Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void Client::CCamera_Default::Free()
{
	CCamera::Free();
}

