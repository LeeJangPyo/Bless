#include "stdafx.h"

#include "Object_Mesh.h"
#include "MeshTab.h"


_USING(Client)

CObject_Mesh::CObject_Mesh(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_bIsPick(false)
	, m_bIsMouseDown(false)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

CObject_Mesh::CObject_Mesh(const CObject_Mesh & rhs)
	:CGameObject(rhs)
	, m_bIsPick(false)
	, m_bIsMouseDown(false)
	, m_pShaderCom(nullptr)
	, m_pInput_Device(rhs.m_pInput_Device)
{
	m_pInput_Device->AddRef();
}


void CObject_Mesh::Set_Position(const _vec3 * pPosition)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);
}

void CObject_Mesh::Set_Rotation(const _vec3 * pRotation)
{
	m_pTransformCom->Set_RotationY(pRotation->y);
}

void CObject_Mesh::Set_Scale(const _vec3 * pScale)
{
	m_pTransformCom->Set_ScaleInfo(pScale);
}
void CObject_Mesh::Set_Position(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION,fX, fY, fZ);
}
void CObject_Mesh::Set_Rotation(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Rotation_Y(fY, 1.f);
}
void CObject_Mesh::Set_Scale(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_ScaleInfo(fX, fY, fZ);
}
HRESULT CObject_Mesh::SetUp_FileInfo(const _tchar * pFilePath, const _tchar * pFileName, const _tchar * pGameObjectTag, const _tchar * pComponentMeshTag)
{
	lstrcpy(m_szFilePath, pFilePath);
	lstrcpy(m_szFileName, pFileName);
	lstrcpy(m_szGameObjectTag,pGameObjectTag);
	lstrcpy(m_szComponentMeshTag, pComponentMeshTag);
	return S_OK;

}
HRESULT CObject_Mesh::Ready_Component()
{
	
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//Static Component
	m_pRendererCom = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	m_pCalculatorCom = dynamic_cast<CCalculator*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Calculator"));
	if (FAILED(Add_Component(L"Com_Calculator", m_pCalculatorCom)))
		return E_FAIL;


	Safe_Release(pManagement);

	return S_OK;
}
void CObject_Mesh::Free(void)
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pCalculatorCom);

	Safe_Release(m_pInput_Device);

	CGameObject::Free();
}

