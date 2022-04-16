#include "..\Headers\Camera.h"



CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	,m_pInput_Device(CInput_Device::GetInstance())
	//, m_bIsActivate(false)
{
	m_pInput_Device->AddRef();

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	ZeroMemory(&m_Camera_Desc, sizeof(CAMERADESC));
	ZeroMemory(&m_Projection_Desc, sizeof(PROJDESC));
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, m_matView(rhs.m_matView)
	, m_Camera_Desc(rhs.m_Camera_Desc)
	, m_matProj(rhs.m_matProj)
	, m_Projection_Desc(rhs.m_Projection_Desc)
	, m_pInput_Device(rhs.m_pInput_Device)
	//, m_bIsActivate(rhs.m_bIsActivate)
{
	m_pInput_Device->AddRef();
}

HRESULT CCamera::Ready_Prototype()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	ZeroMemory(&m_Camera_Desc, sizeof(CAMERADESC));
	ZeroMemory(&m_Projection_Desc, sizeof(PROJDESC));

	return S_OK;
}

HRESULT CCamera::Ready_GameObject()
{
	return S_OK;
}

Engine::_int CCamera::Update_GameObject(const _float& fTimeDelta)
{

	SetUp_Matrix();

	return 0;
}

Engine::_int CCamera::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CCamera::Render_GameObject()
{
	
}

HRESULT CCamera::SetUp_Matrix()
{
	//view Matrix
	_vec3		vRight, vUp, vLook, vPosition;

	vPosition = m_Camera_Desc.vEye;
	vLook = m_Camera_Desc.vAt - m_Camera_Desc.vEye;
	D3DXVec3Cross(&vRight, &m_Camera_Desc.vUp, &vLook);
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	D3DXMatrixIdentity(&m_matWorld);

	memcpy(&m_matWorld.m[0][0], D3DXVec3Normalize(&vRight, &vRight), sizeof(_vec3));
	memcpy(&m_matWorld.m[1][0], D3DXVec3Normalize(&vUp, &vUp), sizeof(_vec3));
	memcpy(&m_matWorld.m[2][0], D3DXVec3Normalize(&vLook, &vLook), sizeof(_vec3));
	memcpy(&m_matWorld.m[3][0], &vPosition, sizeof(_vec3));

	D3DXMatrixInverse(&m_matView, nullptr, &m_matWorld);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);


	//Proejction Matrix
	D3DXMatrixIdentity(&m_matProj);

	m_matProj._11 = (1.f / tanf(m_Projection_Desc.fFovy * 0.5f)) / m_Projection_Desc.fAspect;
	m_matProj._22 = 1.f / tanf(m_Projection_Desc.fFovy * 0.5f);

	m_matProj._33 = m_Projection_Desc.fFar / (m_Projection_Desc.fFar - m_Projection_Desc.fNear);
	m_matProj._43 = (m_Projection_Desc.fFar * m_Projection_Desc.fNear) / (m_Projection_Desc.fFar - m_Projection_Desc.fNear) * -1.f;

	m_matProj._34 = 1.f;
	m_matProj._44 = 0.f;

	//D3DXMatrixPerspectiveFovLH(m_matProj, m_Projection_Desc.fFovy, m_Projection_Desc.fAspect, m_Projection_Desc.fNear, m_Projection_Desc.fFar);

	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}


void CCamera::Set_Fovy(const _float & fFovy)
{
	m_Projection_Desc.fFovy = fFovy;
}

_matrix CCamera::Get_BillBoardMatrix()
{
	_matrix matBill;
	D3DXMatrixIdentity(&matBill);

	matBill._11 = m_matWorld._11;
	matBill._13 = m_matWorld._13;
	matBill._31 = m_matWorld._31;
	matBill._33 = m_matWorld._33;

	return matBill;
}

HRESULT CCamera::SetUp_OnShader_FarPlane(LPD3DXEFFECT pEffect, const char * pConstantName)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetFloat(pConstantName, m_Projection_Desc.fFar);

	Safe_Release(pEffect);

	return S_OK;
}

HRESULT CCamera::SetUp_OnShader_View(LPD3DXEFFECT pEffect, const char * pConstantName)
{
	if (pEffect == nullptr)
		return E_FAIL;
	
	pEffect->AddRef();

	pEffect->SetMatrix(pConstantName, &m_matView);

	Safe_Release(pEffect);

	return S_OK;
}

HRESULT CCamera::SetUp_OnShader_Projection(LPD3DXEFFECT pEffect, const char * pConstantName)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix(pConstantName, &m_matProj);

	Safe_Release(pEffect);

	return S_OK;
}
HRESULT CCamera::SetUp_OnShader_WorldCameraPos(LPD3DXEFFECT pEffect, const char * pConstantName)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();


	pEffect->SetVector(pConstantName, &_vec4(m_Camera_Desc.vEye.x, m_Camera_Desc.vEye.y, m_Camera_Desc.vEye.z,1.f));

	Safe_Release(pEffect);

	return S_OK;
}

void CCamera::Free()
{
	Safe_Release(m_pInput_Device);
	CGameObject::Free();
}