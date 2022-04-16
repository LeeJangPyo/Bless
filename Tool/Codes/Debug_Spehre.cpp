#include "stdafx.h"
#include "..\Headers\Debug_Spehre.h"


#include "Management.h"
_USING(Client)

CDebug_Spehre::CDebug_Spehre(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_bActivate(false)
	, m_pTransform(nullptr)
	, m_pRenderer(nullptr)
	, m_pShader(nullptr)
	, m_pTexture(nullptr)
	, m_pMesh_Sphere(nullptr)
	, m_bPick(false)
{
}

CDebug_Spehre::CDebug_Spehre(const CDebug_Spehre & rhs)
	:CGameObject(rhs)
	, m_bActivate(false)
	, m_bPick(false)
{
}

HRESULT CDebug_Spehre::Ready_Prototype()
{
	return S_OK;
}

HRESULT CDebug_Spehre::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTexture = CTexture::Create(m_pDevice);
	if (m_pTexture == nullptr)
		return E_FAIL;

	D3DXCreateSphere(m_pDevice, 0.5f, 10, 10, &m_pMesh_Sphere, nullptr);

	return S_OK;
}

_int CDebug_Spehre::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CDebug_Spehre::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (!m_bActivate)
		return 0;

	if (m_pRenderer == nullptr)
		return 0;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this);
	return _int();
}

void CDebug_Spehre::Render_GameObject()
{
	if (m_pShader == nullptr||
		m_pTexture == nullptr || 
		m_pMesh_Sphere == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	if (FAILED(SetUp_ConstantTable(pEffect)))
	{
		Safe_Release(pEffect);
		return;
	}

	pEffect->CommitChanges();
	m_pMesh_Sphere->DrawSubset(0);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

}

void CDebug_Spehre::Set_Position(const _vec3 * pPosition)
{
	if (m_pTransform == nullptr)
		return;

	m_pTransform->Set_StateInfo(CTransform::STATE_POSITION, pPosition);	
}

HRESULT CDebug_Spehre::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//For. Renderer
		m_pRenderer = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
		if (FAILED(Add_Component(L"Com_Renderer", m_pRenderer)))
			return E_FAIL;

		//For. Shader;
		m_pShader = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Default"));
		if (FAILED(Add_Component(L"Com_Shader", m_pShader)))
			return E_FAIL;

		//For. Transform;
		m_pTransform = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
		if (FAILED(Add_Component(L"Com_Transform", m_pTransform)))
			return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CDebug_Spehre::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if(pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(m_pTransform->SetUp_OnShader(pEffect, "g_matWorld")))
		return E_FAIL;

	if (!m_bPick)
	{
		if (FAILED(m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", 0)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", 1)))
			return E_FAIL;
	}



	_matrix matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);


	Safe_Release(pEffect);

	return S_OK;
}

CGameObject * CDebug_Spehre::Clone_GameObject()
{
	CDebug_Spehre*	pInstance = new CDebug_Spehre(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CDebug_Spehre Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CDebug_Spehre * CDebug_Spehre::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDebug_Spehre*	pInstance = new CDebug_Spehre(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CDebug_Spehre Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDebug_Spehre::Free()
{
	Safe_Release(m_pMesh_Sphere);

	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);

	CGameObject::Free();
}
