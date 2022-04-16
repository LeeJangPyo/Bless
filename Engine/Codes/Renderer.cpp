#include "..\Headers\Renderer.h"

#include "GameObject.h"
#include "Management.h"
#include "Target_Manager.h"


CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
	,m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pShader_LightAcc(nullptr)
	, m_pShader_Blend(nullptr)
	, m_pBuffer_Screen(nullptr)
	, m_pShader_Bloom(nullptr)
	, m_eCurrent_RenderGroup(RENDER_END)
{
	m_pTarget_Manager->AddRef();
}

CRenderer::CRenderer(const CRenderer & rhs)
	:CComponent(rhs)
	,m_pTarget_Manager(rhs.m_pTarget_Manager)
{
	m_pTarget_Manager->AddRef();
}

HRESULT CRenderer::Ready_Renderer(void)
{

	D3DVIEWPORT9	ViewPort;
	m_pDevice->GetViewport(&ViewPort);

	//////////////////////////////////////////////////////////////////////////
	// Create Target

	// For. Target_Diffuse
	if (FAILED(m_pTarget_Manager->Ready_Target(m_pDevice, 
												L"Target_Diffuse", 
												(_float)ViewPort.Width,
												(_float)ViewPort.Height, 
												D3DFMT_A8B8G8R8,
												D3DXCOLOR(0.f,0.f,0.f,0.f),
												CTarget::TARGET_NORMAL)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Diffuse", 0.f, 0.f, 150.f, 150.f)))
		return E_FAIL;


	// For. Target_Normal
	if (FAILED(m_pTarget_Manager->Ready_Target(m_pDevice, 
												L"Target_Normal", 
												(_float)ViewPort.Width, 
												(_float)ViewPort.Height, 
												D3DFMT_A16B16G16R16F,
												D3DXCOLOR(0.f, 0.f, 0.f, 1.f),
												CTarget::TARGET_NORMAL)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Normal", 0.f, 150.f, 150.f, 150.f)))
		return E_FAIL;

	// For. Target_Depth
	if (FAILED(m_pTarget_Manager->Ready_Target(m_pDevice,
												L"Target_Depth",
												(_float)ViewPort.Width,
												(_float)ViewPort.Height,
												D3DFMT_A32B32G32R32F,
												D3DXCOLOR(0.f, 1.f, 0.f, 1.f),
												CTarget::TARGET_NORMAL)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Depth", 0.f, 300.f, 150.f, 150.f)))
		return E_FAIL;

	// For. Target_Specular
	if (FAILED(m_pTarget_Manager->Ready_Target(m_pDevice,
												L"Target_Specular",
												(_float)ViewPort.Width,
												(_float)ViewPort.Height,
												D3DFMT_A16B16G16R16F,
												D3DXCOLOR(0.f, 0.f, 0.f, 1.f),
												CTarget::TARGET_NORMAL)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Specular", 0.f, 450.f, 150.f, 150.f)))
		return E_FAIL;

	// For. Target_Shade
	if (FAILED(m_pTarget_Manager->Ready_Target(m_pDevice, 
												L"Target_Shade", 
												(_float)ViewPort.Width, 
												(_float)ViewPort.Height,
												D3DFMT_A16B16G16R16F,
												D3DXCOLOR(0.f, 0.f, 0.f, 1.f),
												CTarget::TARGET_NORMAL)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Shade", 150.f, 0.0f, 150.f, 150.f)))
		return E_FAIL;

	// For. Target_Specular_Shade
	if (FAILED(m_pTarget_Manager->Ready_Target(m_pDevice,
												L"Target_Specular_Shade",
												(_float)ViewPort.Width,
												(_float)ViewPort.Height,
												D3DFMT_A16B16G16R16F,
												D3DXCOLOR(0.f, 0.f, 0.f, 1.f),
												CTarget::TARGET_NORMAL)))	
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Specular_Shade", 150.f, 150.f, 150.f, 150.f)))
		return E_FAIL;
	

	// For. Target_Shadow
	if (FAILED(m_pTarget_Manager->Ready_Target(m_pDevice,
													L"Target_Shadow",
													(_float)ViewPort.Width*4,
													(_float)ViewPort.Height*4,
													D3DFMT_A32B32G32R32F,
													D3DXCOLOR(1.f, 1.f, 1.f, 0.f),
													CTarget::TARGET_SHADOW)))

		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Shadow", 300.f, 0.f, 150.f, 150.f)))
		return E_FAIL;

	// For. Target_Blend
	if (FAILED(m_pTarget_Manager->Ready_Target(m_pDevice,
									L"Target_Blend",
									(_float)ViewPort.Width,
									(_float)ViewPort.Height,
									D3DFMT_A16B16G16R16F,
									D3DXCOLOR(0.f, 0.f, 0.f, 0.f),
									CTarget::TARGET_NORMAL)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Blend", 450.f, 0.f, 150.f, 150.f)))
		return E_FAIL;


	//////////////////////////////////////////////////////////////////////////
	// For.MRT_Shadow
	if (FAILED(m_pTarget_Manager->Ready_MRT(L"MRT_Shadow", L"Target_Shadow")))
		return E_FAIL;
	
	
	// For.MRT_Deferred
	if (FAILED(m_pTarget_Manager->Ready_MRT(L"MRT_Deferred", L"Target_Diffuse")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_MRT(L"MRT_Deferred", L"Target_Normal")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_MRT(L"MRT_Deferred", L"Target_Depth")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_MRT(L"MRT_Deferred", L"Target_Specular")))
		return E_FAIL;

	// For.MRT_LightAcc
	if (FAILED(m_pTarget_Manager->Ready_MRT(L"MRT_LightAcc", L"Target_Shade")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_MRT(L"MRT_LightAcc", L"Target_Specular_Shade")))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////
	// For.MRT_Blend
	if (FAILED(m_pTarget_Manager->Ready_MRT(L"MRT_Blend", L"Target_Blend")))
		return E_FAIL;


	//////////////////////////////////////////////////////////////////////////
	// Ready_Component

	// For. Shader_LightAcc
	m_pShader_LightAcc = CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_LightAcc.fx");
	if (m_pShader_LightAcc == nullptr)
		return E_FAIL;

	// For. Shader_Blend
	m_pShader_Blend = CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_Blend.fx");
	if (m_pShader_Blend == nullptr)
		return E_FAIL;

	// For. Bloom_Shader
	m_pShader_Bloom = CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_Bloom.fx");
	if (m_pShader_Bloom == nullptr)
		return E_FAIL;

	// For. ScreenBuffer
	m_pBuffer_Screen = CBuffer_Screen::Create(m_pDevice);
	if (m_pBuffer_Screen == nullptr)
		return E_FAIL;

	return S_OK;
}

void CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (RENDER_END <= eGroup)
		return;

	m_RenderList[eGroup].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer::Render_ObjectGroup(_bool bDebug, _bool bDeferred)
{
	Render_Priority();
	
	//
	if (bDeferred)
	{
		Render_NoneAlpha();
	}
	else
	{
		Render_Shadow();
		Render_Deferred();
		Render_LightAcc();
		Render_Blend();
		Render_Bloom();
	}	
		
	Render_Alpha();
	Render_UI();

	Clear_RenderGroup();


	if(bDebug)
	{
		m_pTarget_Manager->Render_DebugBuffer(L"MRT_Deferred");
		m_pTarget_Manager->Render_DebugBuffer(L"MRT_LightAcc");
		m_pTarget_Manager->Render_DebugBuffer(L"MRT_Shadow");
		m_pTarget_Manager->Render_DebugBuffer(L"MRT_Blend");
	}
	
}

void CRenderer::Clear_RenderGroup(void)
{
	for (_uint i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderList[i])
			Safe_Release(pGameObject);
		m_RenderList[i].clear();
	}
}

void CRenderer::Render_Priority( )
{
	m_eCurrent_RenderGroup = RENDER_PRIORITY;

	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();

		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_PRIORITY].clear();

}

void CRenderer::Render_NoneAlpha()
{
	m_eCurrent_RenderGroup = RENDER_NONEALPHA;

	for (auto& pGameObject : m_RenderList[RENDER_NONEALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();

		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_NONEALPHA].clear();

}
_bool Compare(CGameObject* pSour, CGameObject* pDest)
{
	return pSour->Get_ViewZ() > pDest->Get_ViewZ();
}

void CRenderer::Render_Alpha( )
{
	m_eCurrent_RenderGroup = RENDER_ALPHA;

	//m_RenderList[RENDER_ALPHA].sort(Compare);

	for (auto& pGameObject : m_RenderList[RENDER_ALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();

		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_ALPHA].clear();
}

void CRenderer::Render_UI( )
{
	m_eCurrent_RenderGroup = RENDER_UI;

	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();

		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_UI].clear();
}

void CRenderer::Render_Deferred()
{
	m_eCurrent_RenderGroup = RENDER_NONEALPHA;

	m_pTarget_Manager->Begin_MRT(L"MRT_Deferred");

	Render_NoneAlpha();

	m_pTarget_Manager->End_MRT(L"MRT_Deferred");
}

void CRenderer::Render_LightAcc()
{
	LPD3DXEFFECT	pEffect = m_pShader_LightAcc->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	pEffect->AddRef();

	//Normal Texture Setting;
	m_pTarget_Manager->SetUp_Texture_OnShader(pEffect, L"Target_Normal", "g_NormalTexture");
	m_pTarget_Manager->SetUp_Texture_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	m_pTarget_Manager->SetUp_Texture_OnShader(pEffect, L"Target_Specular", "g_SpecularTexture");

	pEffect->Begin(nullptr, 0);

	m_pTarget_Manager->Begin_MRT(L"MRT_LightAcc");

	CLight_Manager::GetInstance()->Render_Light(pEffect);

	m_pTarget_Manager->End_MRT(L"MRT_LightAcc");

	pEffect->End();

	Safe_Release(pEffect);
}

void CRenderer::Render_Blend()
{
	LPD3DXEFFECT	pEffect = m_pShader_Blend->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	pEffect->AddRef();

	if (FAILED(m_pTarget_Manager->SetUp_Texture_OnShader(pEffect, L"Target_Diffuse", "g_DiffuseTexture")))
		return;
	if (FAILED(m_pTarget_Manager->SetUp_Texture_OnShader(pEffect, L"Target_Shade", "g_ShadeTexture")))
		return;
	if (FAILED(m_pTarget_Manager->SetUp_Texture_OnShader(pEffect, L"Target_Specular_Shade", "g_SpecularTexture")))
		return;
	if (FAILED(m_pTarget_Manager->SetUp_Texture_OnShader(pEffect, L"Target_Shadow", "g_ShadowTexture")))
		return;
	if (FAILED(m_pTarget_Manager->SetUp_Texture_OnShader(pEffect, L"Target_Depth", "g_DepthTexture")))
		return;

	//Inv
	_matrix			matView, matProj, matViewInv, matProjInv;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matViewInv, nullptr, &matView);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProjInv, nullptr, &matProj);

	//LightView
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetMatrix("g_matLightView", CLight_Manager::GetInstance()->Get_LightViewMatrix());

	pEffect->SetMatrix("g_matViewInv", &matViewInv);
	pEffect->SetMatrix("g_matProjInv", &matProjInv);
	pEffect->SetFloat("g_fFarPlane", 1000.f);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pTarget_Manager->Begin_MRT(L"MRT_Blend");

	m_pBuffer_Screen->Render_Buffer();

	m_pTarget_Manager->End_MRT(L"MRT_Blend");

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void CRenderer::Render_Shadow()
{
	m_eCurrent_RenderGroup = RENDER_SHADOW;

	m_pTarget_Manager->Begin_Shadow_MRT(L"MRT_Shadow");

	for (auto& pGameObject : m_RenderList[RENDER_SHADOW])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();

		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_SHADOW].clear();

	m_pTarget_Manager->End_Shadow_MRT(L"MRT_Shadow");
}

void CRenderer::Render_Bloom()
{
	LPD3DXEFFECT	pEffect = m_pShader_Bloom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	pEffect->AddRef();

	if (FAILED(m_pTarget_Manager->SetUp_Texture_OnShader(pEffect, L"Target_Blend", "g_BlendedTexture")))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBuffer_Screen->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void CRenderer::Free(void)
{
	Clear_RenderGroup();
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pShader_Blend);
	Safe_Release(m_pShader_Bloom);
	Safe_Release(m_pShader_LightAcc);
	Safe_Release(m_pBuffer_Screen);
	

	CComponent::Free();
}

CComponent * CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderer*		pInstance = new CRenderer(pDevice);

	if (FAILED(pInstance->Ready_Renderer()))
	{
		_MSG_BOX(L"CRenderer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CComponent* Engine::CRenderer::Clone_Component()
{
	this->AddRef();
	return this;
}
