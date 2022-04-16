#include "stdafx.h"
#include "Object_Equipment.h"

#include "MeshTab.h"
#include "Camera_Dynamic.h"

_USING(Client)

CObject_Equipment::CObject_Equipment(LPDIRECT3DDEVICE9 pDevice)
	: CObject_Mesh(pDevice)
	, m_pParentBoneMatrix(nullptr)
	, m_pParentWorldMatrix(nullptr)
	, m_pMeshCom(nullptr)
{
	D3DXMatrixIdentity(&m_matCombineWorld);
}
CObject_Equipment::CObject_Equipment(const CObject_Equipment & rhs)
	:CObject_Mesh(rhs)
	, m_pParentBoneMatrix(nullptr)
	, m_pParentWorldMatrix(nullptr)
{
	D3DXMatrixIdentity(&m_matCombineWorld);
}

HRESULT Client::CObject_Equipment::Ready_Prototype()
{
	
	return S_OK;
}

HRESULT Client::CObject_Equipment::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;



	return S_OK;
}


HRESULT Client::CObject_Equipment::Ready_Component()
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

Engine::_int Client::CObject_Equipment::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pParentBoneMatrix != nullptr || m_pParentWorldMatrix != nullptr)
	{
		_matrix matParent = (*m_pParentBoneMatrix) * (*m_pParentWorldMatrix);
		m_matCombineWorld = *m_pTransformCom->Get_WorldMatrix() * matParent;
	}

	return 0;
}
Engine::_int Client::CObject_Equipment::LastUpdate_GameObject(const _float& fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this);

	return 0;
}

void Client::CObject_Equipment::Render_GameObject()
{
	if (m_pShaderCom == nullptr ||
		m_pMeshCom == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh();

	pEffect->EndPass();
	pEffect->End();

}



HRESULT CObject_Equipment::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &m_matCombineWorld);

	//View Proejction Matrix
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	pManagement->AddRef();

	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(pManagement->Get_GameObject(SCENE_STATIC, L"Layer_Camera_Dynamic"));
	if (pCamera == nullptr)
	{
		Safe_Release(pManagement);
		Safe_Release(pEffect);
		return E_FAIL;
	}

	pCamera->SetUp_OnShader_View(pEffect, "g_matView");
	pCamera->SetUp_OnShader_Projection(pEffect, "g_matProj");

	Safe_Release(pManagement);
	Safe_Release(pEffect);

	return S_OK;
}


CObject_Equipment* CObject_Equipment::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CObject_Equipment*	pInstance = new CObject_Equipment(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CObject_Equipment Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CObject_Equipment::Clone_GameObject()
{
	CObject_Equipment*	pInstance = new CObject_Equipment(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CObject_Equipment Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Equipment::Free(void)
{
	Safe_Release(m_pMeshCom);
	
	CObject_Mesh::Free();
}



HRESULT CObject_Equipment::Set_ParentMatrix(const _tchar* pLayerTag, const _tchar* FrameTag)
{
	if (this == nullptr)
		return E_FAIL;

	CMesh_Dynamic*	pPlayerMeshCom = dynamic_cast<CMesh_Dynamic*>(CManagement::GetInstance()->Get_Component(SCENE_EDIT, pLayerTag,L"Com_Mesh"));
	if (pPlayerMeshCom == nullptr)
		return E_FAIL;

	_char	szFrameTag[MIN_PATH];

	WideCharToMultiByte(CP_ACP, 0, FrameTag, -1, szFrameTag, MIN_PATH, NULL, NULL);

	const Engine::D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName(szFrameTag);

	m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

	CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(CManagement::GetInstance()->Get_Component(SCENE_EDIT, pLayerTag, L"Com_Transform"));
	if (pPlayerTransCom == nullptr)
		return E_FAIL;

	m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();

	return S_OK;

}

HRESULT CObject_Equipment::SetUp_ObjectInfo(const _tchar * pComponentMeshTag)
{
	// Set Component
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
	//m_pColliderCom = dynamic_cast<CCollider_Sphere*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Sphere"));
	//if (m_pColliderCom == nullptr)
	//	return E_FAIL;

	//m_pColliderCom->SetUp_Collider_Spehre(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), 3.f);


	Safe_Release(pManagement);

	return S_OK;
}
