#include "stdafx.h"
#include "Title_Logo.h"

//manager
#include "Management.h"
#include "Input_Device.h"
#include "Renderer.h"

//Component
#include "Transform.h"
#include "Texture.h"
#include "RcTex.h"

_USING(Client)

CTitle_Logo::CTitle_Logo(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pRendererCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_fFrame(0.f)
	, m_fMaxFrame(0.f)
	

{

}

CTitle_Logo::CTitle_Logo(const CTitle_Logo & rhs)
	: CGameObject(rhs)
	, m_fFrame(0.f)
	, m_fMaxFrame(0.f)
{

}

HRESULT CTitle_Logo::Ready_Prototype()
{
	return S_OK;

}

HRESULT CTitle_Logo::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
	{
		return E_FAIL;
	}

	m_pTransformCom->Set_ScaleInfo(0.5f, 0.35f, 1.f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, 0.f, -0.5f, 0.5f);

	m_fMaxFrame = (_float)m_pTextureCom->Get_TextureSize();

	return S_OK;
}

_int CTitle_Logo::Update_GameObject(const _float & fTimeDelta)
{
	m_fFrame += m_fMaxFrame * fTimeDelta;
	if (m_fFrame > m_fMaxFrame)
		m_fFrame = m_fMaxFrame-1.f;



	return _int();
}

_int CTitle_Logo::LastUpdate_GameObject(const _float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	return _int();
}

void CTitle_Logo::Render_GameObject()
{
	if (m_pBufferCom == nullptr || m_pTextureCom == nullptr || m_pShaderCom == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();


	if (FAILED(m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld")))
		return;

	if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture",_uint(m_fFrame))))
		return;

	_matrix matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);


	m_pBufferCom->Render_Buffer();

	pEffect->End();
	pEffect->EndPass();

	Safe_Release(pEffect);

}

CTitle_Logo * CTitle_Logo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTitle_Logo*		pInstance = new CTitle_Logo(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CTitle_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CTitle_Logo::Clone_GameObject()
{
	CTitle_Logo*		pInstance = new CTitle_Logo(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CTitle_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT Client::CTitle_Logo::Ready_Component()
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

	m_pBufferCom = dynamic_cast<CBuffer_RcTex*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex"));
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_LOGO, L"Component_Texture_TitleLogo"));
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_LOGO, L"Component_Shader_Logo"));
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

void Client::CTitle_Logo::Free(void)
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	//	Safe_Release(m_pInput_Device);

	CGameObject::Free();
}
