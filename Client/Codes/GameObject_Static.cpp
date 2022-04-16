#include "stdafx.h"
#include "..\Headers\GameObject_Static.h"

#include "Management.h"
#include "Camera_Dynamic.h"

_USING(Client)

CGameObject_Static::CGameObject_Static(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_pMeshCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pOptimizationCom(nullptr)
{
}

CGameObject_Static::CGameObject_Static(const CGameObject_Static & rhs)
	:CGameObject(rhs)
{
}

HRESULT Client::CGameObject_Static::Ready_Prototype()
{
	return S_OK;
}

HRESULT Client::CGameObject_Static::Ready_GameObject()
{
	
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, 0.f, 0.f, 0.f);

	return S_OK;
}

Engine::_int Client::CGameObject_Static::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

Engine::_int Client::CGameObject_Static::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_pRendererCom == nullptr || m_pOptimizationCom == nullptr)
		return 0;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this);

	//m_bDraw = m_pOptimizationCom->Is_InFrustum_ForObject
	//(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), 30.f);

	return 0;
}

void Client::CGameObject_Static::Render_GameObject()
{
	if (m_bDraw == false ||
		m_pMeshCom == nullptr||
		m_pShaderCom == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
	{
		Safe_Release(pEffect);
		return;
	}

	pEffect->Begin(nullptr, 0);
	
	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->End();

	Safe_Release(pEffect);


	//m_pColliderCom->Render_Collider_Box(CCollider_Box::COL_TRUE, m_pTransformCom->Get_WorldMatrix());
}

HRESULT CGameObject_Static::SetUp_GameObjectInfo(const _matrix* pMatrix, const _tchar * pComponentMeshTag)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Mesh
	m_pMeshCom = dynamic_cast<CMesh_Static*>(pManagement->Clone_Component(SCENE_STAGE, pComponentMeshTag));
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;


	//Set World Matrix
	m_pTransformCom->Set_WorldMatrix(pMatrix);


	Safe_Release(pManagement);

	return S_OK;
}

CGameObject * CGameObject_Static::Clone_GameObject()
{
	CGameObject_Static*		pInstance = new CGameObject_Static(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CGameObject_Static Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGameObject_Static::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CGameObject_Static*		pInstance = new CGameObject_Static(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CGameObject_Static Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CGameObject_Static::Ready_Component()
{
	
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	// For. Static Component
	m_pRendererCom = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	m_pCalculatorCom = dynamic_cast<CCalculator*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Calculator"));
	if (FAILED(Add_Component(L"Com_Calculator", m_pCalculatorCom)))
		return E_FAIL;

	m_pOptimizationCom = dynamic_cast<COptimization*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Optimization"));
	if (FAILED(Add_Component(L"Com_Optimization", m_pOptimizationCom)))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////
	// Stage Component
	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh"));
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	////For .Stage Component
	//m_pMeshCom = dynamic_cast<CMesh_Static*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Mesh_Stone"));
	//if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
	//	return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;

}

HRESULT CGameObject_Static::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld")))
		return E_FAIL;

	CCamera* pCamera = CManagement::GetInstance()->Get_CameraMain();
	if (pCamera == nullptr)
	{
		Safe_Release(pEffect);
		return E_FAIL;
	}
	pCamera->SetUp_OnShader_View(pEffect, "g_matView");
	pCamera->SetUp_OnShader_Projection(pEffect, "g_matProj");
	pCamera->SetUp_OnShader_FarPlane(pEffect, "g_fFarPlane");

	Safe_Release(pEffect);

	return S_OK;
}


void Client::CGameObject_Static::Free()
{

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pCalculatorCom);
	Safe_Release(m_pMeshCom);
	//Safe_Release(m_pColliderCom);
	Safe_Release(m_pOptimizationCom);

	CGameObject::Free();
}
