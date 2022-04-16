#include "stdafx.h"
#include "..\Headers\Terrain.h"

#include "Management.h"
#include "Camera_Dynamic.h"

_USING(Client)

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	,m_pTransformCom(nullptr)
	,m_pTextureCom(nullptr)
	,m_pRendererCom(nullptr)
	,m_pBufferCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pNaviMeshCom(nullptr)
	, m_pFilterTextureCom(nullptr)
	, m_pNormalTextureCom(nullptr)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
	, m_pTransformCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pNaviMeshCom(nullptr)
	, m_pFilterTextureCom(nullptr)
	, m_pNormalTextureCom(nullptr)
{
}

HRESULT Client::CTerrain::Ready_Prototype()
{
	return S_OK;
}

HRESULT Client::CTerrain::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

Engine::_int Client::CTerrain::Update_GameObject(const _float& fTimeDelta)
{	
	return 0;
}

Engine::_int Client::CTerrain::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this);
	return S_OK;
}

void Client::CTerrain::Render_GameObject()
{
	if (m_pBufferCom == nullptr  ||
		m_pShaderCom == nullptr  ||
		m_pTextureCom == nullptr  )
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
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	if(g_bDebug)
		m_pNaviMeshCom->Render_NaviCell();

}

HRESULT CTerrain::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(m_pBufferCom->SetUp_OnShader_Detail(pEffect, "g_fDetail")))
		return E_FAIL;

	if (FAILED(m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld")))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture",0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_SubTexture", 3)))
		return E_FAIL;

	if (FAILED(m_pNormalTextureCom->SetUp_OnShader(pEffect, "g_BaseNormalTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pNormalTextureCom->SetUp_OnShader(pEffect, "g_SubNormalTexture", 3)))
		return E_FAIL;

	if (FAILED(m_pFilterTextureCom->SetUp_OnShader(pEffect, "g_FilterTexture")))
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


HRESULT CTerrain::Ready_Component()
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



	m_pNaviMeshCom = dynamic_cast<CMesh_Navigation*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Mesh_Navigation"));
	if (FAILED(Add_Component(L"Com_Navi", m_pNaviMeshCom)))
		return E_FAIL;
	m_pNaviMeshCom->Load_InfoFromFile(L"../Bin/Resources/Data/stage1.navi");
	

	// For. Stage Component
	m_pBufferCom = dynamic_cast<CBuffer_TerrainTex*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Buffer_Terrain"));
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Texture_Terrain"));
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	m_pFilterTextureCom = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Texture_Terrain_Filter"));
	if (FAILED(Add_Component(L"Com_FilterTexture", m_pFilterTextureCom)))
		return E_FAIL;

	m_pNormalTextureCom = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Texture_Terrain_Normal"));
	if (FAILED(Add_Component(L"Com_TextureNormal", m_pNormalTextureCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Shader_Terrain"));
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;
	
	Safe_Release(pManagement);

	return S_OK;
}



CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTerrain*		pInstance = new CTerrain(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CTerrain Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* Client::CTerrain::Clone_GameObject()
{
	CTerrain*		pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CTerrain Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CTerrain::Free()
{
	Safe_Release(m_pNaviMeshCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pFilterTextureCom);
	Safe_Release(m_pNormalTextureCom);

	CGameObject::Free();
}
