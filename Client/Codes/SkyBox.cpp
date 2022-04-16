#include "stdafx.h"
#include "..\Headers\SkyBox.h"

#include "Management.h"


_USING(Client)

Client::CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pShaderCom(nullptr)
{

}

Client::CSkyBox::CSkyBox(const CSkyBox& rhs)
	: CGameObject(rhs)
{

}

HRESULT Client::CSkyBox::Ready_Prototype()
{

	return S_OK;
}

HRESULT Client::CSkyBox::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_ScaleInfo(50.f, 50.f, 50.f);

	return S_OK;
}

Engine::_int Client::CSkyBox::Update_GameObject(const _float& fTimeDelta)
{


	return 0;
}

Engine::_int Client::CSkyBox::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	_matrix		matView;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(matView._41, matView._42 + 3.f, matView._43));

	return 0;
}

void Client::CSkyBox::Render_GameObject()
{
	if (m_pBufferCom == nullptr || m_pTextureCom == nullptr || m_pShaderCom == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	
	pEffect->AddRef();
	
	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	
	m_pBufferCom->Render_Buffer();

	pEffect->End();
	pEffect->EndPass();

	Safe_Release(pEffect);
}

HRESULT Client::CSkyBox::Ready_Component(void)
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

	m_pBufferCom = dynamic_cast<CBuffer_CubeTex*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Buffer_CubeTex"));
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Texture_SkyBox"));
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Shader_SkyBox"));
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;


	Safe_Release(pManagement);

	return S_OK;

}

HRESULT CSkyBox::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;
	
	pEffect->AddRef();

	if(FAILED(m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld")))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture")))
		return E_FAIL;

	CCamera* pCamera = CManagement::GetInstance()->Get_CameraMain();
	if(pCamera == nullptr)
		return E_FAIL;
	if(FAILED(pCamera->SetUp_OnShader_View(pEffect, "g_matView")))
		return E_FAIL;
	if (FAILED(pCamera->SetUp_OnShader_Projection(pEffect, "g_matProj")))
		return E_FAIL;


	Safe_Release(pEffect);
	
	return S_OK;
}

Client::CSkyBox* Client::CSkyBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSkyBox*		pInstance = new CSkyBox(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CSkyBox Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkyBox::Clone_GameObject()
{
	CSkyBox*		pInstance = new CSkyBox(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CSkyBox Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void Client::CSkyBox::Free(void)
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);

	CGameObject::Free();
}

