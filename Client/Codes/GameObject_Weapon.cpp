#include "stdafx.h"
#include "..\Headers\GameObject_Weapon.h"

#include "Management.h"

_USING(Client)



CGameObject_Weapon::CGameObject_Weapon(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_bIsEquiped(false)
{
	D3DXMatrixIdentity(&m_matCombineWorld);
}

CGameObject_Weapon::CGameObject_Weapon(const CGameObject_Weapon & rhs)
	:CGameObject(rhs)
	, m_bIsEquiped(false)
{
	D3DXMatrixIdentity(&m_matCombineWorld);
}


HRESULT Client::CGameObject_Weapon::Ready_Prototype()
{
	
	return S_OK;
}

HRESULT Client::CGameObject_Weapon::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	return S_OK;
}

Engine::_int Client::CGameObject_Weapon::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}
Engine::_int Client::CGameObject_Weapon::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this);

	return 0;
}

void CGameObject_Weapon::Render_GameObject()
{
	
}

HRESULT CGameObject_Weapon::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CGameObject_Weapon::Ready_Component()
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


	//////////////////////////////////////////////////////////////////////////
	// Stage Component
	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh"));
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;




	Safe_Release(pManagement);

	return S_OK;
}



void Client::CGameObject_Weapon::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshCom);


	CGameObject::Free();
}

