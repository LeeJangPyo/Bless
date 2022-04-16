#include "stdafx.h"
#include "Object_Static.h"


#include "MainFrm.h"
#include "ControlForm.h"
#include "MeshTab.h"

#include "Camera_Dynamic.h"

_USING(Client)

CObject_Static::CObject_Static(LPDIRECT3DDEVICE9 pDevice)
	: CObject_Mesh(pDevice)
	, m_pMeshCom(nullptr)
	, m_pColliderCom(nullptr)
{
}

CObject_Static::CObject_Static(const CObject_Static & rhs)
	: CObject_Mesh(rhs)
{
}

HRESULT Client::CObject_Static::Ready_Prototype()
{
	return S_OK;
}

HRESULT Client::CObject_Static::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_ScaleInfo(g_fDefault_Sacle, g_fDefault_Sacle, g_fDefault_Sacle);

	return S_OK;
}



Engine::_int Client::CObject_Static::Update_GameObject(const _float& fTimeDelta)
{
	if (((CMainFrame*)AfxGetMainWnd())->m_pControlForm->m_eTabType == CControlForm::TAB_MESH)
		Key_Input(fTimeDelta);


	return 0;
}

Engine::_int Client::CObject_Static::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this);

	return 0;
}

void Client::CObject_Static::Render_GameObject()
{
	if (m_pShaderCom == nullptr ||
		m_pMeshCom == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	pEffect->AddRef();

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(2);

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();

	//Pass 1
	pEffect->BeginPass(1);
	if (m_bIsPick)
	{
		m_pColliderCom->SetUp_ConstantTable(pEffect, CCollider_Box::COLOR_GREEN, m_pTransformCom->Get_WorldMatrix());	
	}
	else
	{
		m_pColliderCom->SetUp_ConstantTable(pEffect, CCollider_Box::COLOR_RED, m_pTransformCom->Get_WorldMatrix());
	}
	pEffect->CommitChanges();
	m_pColliderCom->Render_Collider_Box();

	pEffect->EndPass();
	
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CObject_Static::Ready_Component()
{
	if (CObject_Mesh::Ready_Component())
		return E_FAIL;

	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Shader
	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Default"));
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CObject_Static::SetUp_ObjectInfo(const _tchar * pComponentMeshTag)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Mesh
	m_pMeshCom = dynamic_cast<CMesh_Static*>(pManagement->Clone_Component(SCENE_EDIT, pComponentMeshTag));
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	////For. Collider
	////Collider Clone Ready is Need Mesh Information 
	//m_pColliderCom = dynamic_cast<CCollider_Box*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box"));
	//if (m_pColliderCom == nullptr)
	//	return E_FAIL;
	//m_pColliderCom->SetUp_Collider_Box(m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVertex(), m_pMeshCom->Get_Stride());


	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CObject_Static::SetUp_ObjectInfo(const _matrix * pMatrix, const _tchar * pComponentMeshTag, CMeshTab * pMeshTab)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	// Set MeshTab Pointer;
	m_pMeshTab = pMeshTab;

	//For. Mesh
	m_pMeshCom = dynamic_cast<CMesh_Static*>(pManagement->Clone_Component(SCENE_EDIT, pComponentMeshTag));
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;
	

	//Set World Matrix
	m_pTransformCom->Set_WorldMatrix(pMatrix);


	//For. Collider
	//Collider Clone Ready is Need Mesh Information 
	m_pColliderCom = dynamic_cast<CCollider_Box*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box"));
	if (m_pColliderCom == nullptr)
		return E_FAIL;
	m_pColliderCom->SetUp_Collider_Box(m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVertex(), m_pMeshCom->Get_Stride());

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CObject_Static::SetUp_ObjectInfo(const _vec3 * pPosition, const _tchar * pComponentMeshTag, CMeshTab * pMeshTab)
{
	// Set Position
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);

	// Set MeshTab Pointer;
	m_pMeshTab = pMeshTab;


	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();


	//For. Mesh
	m_pMeshCom = dynamic_cast<CMesh_Static*>(pManagement->Clone_Component(SCENE_EDIT, pComponentMeshTag));
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	//For. Collider
	//Collider Clone Ready is Need Mesh Information 
	m_pColliderCom = dynamic_cast<CCollider_Box*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Box"));
	if (FAILED(Add_Component(L"m_pColliderCom", m_pColliderCom)))
		return E_FAIL;
	m_pColliderCom->SetUp_Collider_Box(m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVertex(), m_pMeshCom->Get_Stride());


	Safe_Release(pManagement);

	return S_OK;
}

//_bool CObject_Static::Intersect_Static_Mesh(const RAY& rRay, _ulong * pFaceIndex, _float * pU, _float * pV, _float * pDist, LPD3DXBUFFER * ppAllHits, _ulong * pCountOfHits)
_bool			CObject_Static::Intersect_Static_Mesh(const RAY& rRay, _vec3* pOutPosition)
{
	BOOL bHit;
	_ulong	dwFaceIndex;
	_float	fU, fV, fDist;
	LPD3DXBUFFER pAllHits;
	_ulong  dwCountOfHits;

	LPD3DXMESH pMesh = m_pMeshCom->Get_Mesh();

	//Ray is transfrom to local Space;

	_matrix matWorld = *m_pTransformCom->Get_WorldMatrix();
	RAY Ray = rRay;
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&Ray.vRayPos, &Ray.vRayPos, &matWorld);
	D3DXVec3TransformNormal(&Ray.vRayDir, &Ray.vRayDir, &matWorld);

	D3DXVec3Normalize(&Ray.vRayDir, &Ray.vRayDir);

	//Interesect Mesh
	D3DXIntersect(pMesh, &Ray.vRayPos, &Ray.vRayDir, &bHit, &dwFaceIndex, &fU, &fV, &fDist, &pAllHits, &dwCountOfHits);

	if (bHit == TRUE)
	{
		*pOutPosition = Ray.vRayPos + (Ray.vRayDir * fDist);
		D3DXVec3TransformCoord(pOutPosition, pOutPosition, m_pTransformCom->Get_WorldMatrix());
		return true;
	}
	else
	{
		pOutPosition = nullptr;
		return false;
	}
}

CObject_Static * CObject_Static::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CObject_Static*	pInstance = new CObject_Static(pDevice);
	
	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CObject_Static Create Failed");
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

CGameObject * CObject_Static::Clone_GameObject()
{
	CObject_Static*	pInstance = new CObject_Static(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CObject_Static Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Static::Free(void)
{
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pColliderCom);

	CObject_Mesh::Free();
}


HRESULT CObject_Static::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld")))
		return E_FAIL;


	//View Proejction Matrix
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	pManagement->AddRef();

	//CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(pManagement->Get_GameObject(SCENE_STATIC, L"Layer_Camera_Dynamic"));
	//if (pCamera == nullptr)
	//{
	//	Safe_Release(pManagement);
	//	Safe_Release(pEffect);
	//	return E_FAIL;
	//}

	//pCamera->SetUp_OnShader_View(pEffect, "g_matView");
	//pCamera->SetUp_OnShader_Projection(pEffect, "g_matProj");

	//Inv
	_matrix			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	Safe_Release(pManagement);
	Safe_Release(pEffect);

	return S_OK;
}

void CObject_Static::Key_Input(const _float fTimeDelta)
{
	//Exception
	HWND	hWnd = ::GetFocus();

	if (hWnd != g_hWnd)
		return;

	POINT	ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (ptMouse.x < 0 || ptMouse.x > g_nBackCX || ptMouse.y < 0 || ptMouse.y > g_nBackCY)
		return;

	if (m_pMeshTab->Get_ObjectType() != CMeshTab::STATIC_OBJECT)
		return;

	//wheel
	if (m_bIsPick)
	{
		_vec3 vPosition = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);

		_float fMouseMove = 0.f;

		if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Z))
		{
			vPosition.y += fMouseMove * 0.001f;
		}
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);

		m_pMeshTab->Update_Transform(CMeshTab::DATA_POSITION, &vPosition);
	}

	if (m_pInput_Device->Get_MouseDown(CInput_Device::DIM_LB))
	{
		//Transform MosuePos to World Ray
		RAY	tRay = m_pCalculatorCom->Transform_ScreenPosToRayLocal(g_hWnd, m_pTransformCom->Get_WorldMatrix());

		const _vec3* pMin = m_pColliderCom->Get_Min();
		const _vec3* pMax = m_pColliderCom->Get_Max();

		_vec3 vIntersectPoint;
		if (m_pCalculatorCom->Picking_AABB(pMin, pMax, tRay, &vIntersectPoint))
		{
			m_bIsPick = true;
			m_bIsMouseDown = true;
			m_pMeshTab->m_pPickedMeshObject = this;
			m_pMeshTab->Update_Transform(CMeshTab::DATA_POSITION, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));
		
			_vec3 vScale = _vec3(m_pTransformCom->Get_ScaleInfo(CTransform::STATE_RIGHT),
									m_pTransformCom->Get_ScaleInfo(CTransform::STATE_UP),
									m_pTransformCom->Get_ScaleInfo(CTransform::STATE_LOOK));
			m_pMeshTab->Update_Transform(CMeshTab::DATA_SCALE, &vScale);
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
			_vec3 vPosition = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);

			_float fMouseMove = 0.f;
			if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Y))
			{
				vPosition.z -= fMouseMove * 0.01f;
			}

			fMouseMove = 0.f;
			if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_X))
			{
				vPosition.x += fMouseMove * 0.01f;
			}

			m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);

			//Update MFC Data
			m_pMeshTab->Update_Transform(CMeshTab::DATA_POSITION, &vPosition);
		}
	}

	if (m_pInput_Device->Get_MouseUp(CInput_Device::DIM_LB))
	{
		m_bIsMouseDown = false;
		m_pMeshTab->Update_Transform(CMeshTab::DATA_POSITION, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));
	}
}
