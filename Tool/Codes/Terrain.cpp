#include "stdafx.h"
#include "..\Headers\Terrain.h"

#include "Management.h"
#include "Camera_Dynamic.h"

_USING(Client)

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
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

	if (nullptr == m_pBufferCom ||
		nullptr == m_pShaderCom ||
		nullptr == m_pTextureCom)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (FAILED(SetUp_ConstantTable(pEffect)))
	{
		Safe_Release(pEffect);
		return;
	}

	pEffect->CommitChanges();
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

}

HRESULT		CTerrain::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld")))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture")))
		return E_FAIL;


	_matrix matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView",&matView);
	pEffect->SetMatrix("g_matProj" ,&matProj);


	Safe_Release(pEffect);

	return S_OK;
}

HRESULT CTerrain::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	m_pRendererCom = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	m_pBufferCom = dynamic_cast<CBuffer_TerrainTex*>(pManagement->Clone_Component(SCENE_EDIT, L"Component_Buffer_Terrain"));
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_EDIT, L"Component_Texture_Terrain"));
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Default"));
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
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);

	CGameObject::Free();
}
