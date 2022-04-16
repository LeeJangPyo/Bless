#include "stdafx.h"
#include "..\Headers\Blood.h"

#include "Management.h"

_USING(Client)

CBlood::CBlood(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_iWidth(0)
	, m_iHeight(0)
	, m_iAnimationX(0)
	, m_iAnimationY(0)
	, m_fFrame(0.f)
	, m_fTimeAcc(0.f)
{
}

CBlood::CBlood(const CBlood & rhs)
	: CGameObject(rhs)
	, m_iWidth(0)
	, m_iHeight(0)
	, m_iAnimationX(0)
	, m_iAnimationY(0)
	, m_fFrame(0.f)
	, m_fTimeAcc(0.f)
{

}

HRESULT CBlood::Ready_Prototype()
{
	return S_OK;
}

HRESULT CBlood::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_ScaleInfo(3.f, 3.f, 1.f);

	m_iWidth = 4;
	m_iHeight = 4;


	return S_OK;
}

_int CBlood::Update_GameObject(const _float & fTimeDelta)
{
	m_fFrame += m_iWidth * m_iHeight * fTimeDelta * 0.5f;
	if (m_fFrame > m_iWidth * m_iHeight)
	{
		m_bIsDead = true;
		m_fFrame = 0.f;
	}

	if (m_bIsDead)
		return DEAD_OBJ;


		
	m_iAnimationX = (_int)m_fFrame % m_iWidth;
	m_iAnimationY = (_int)m_fFrame / m_iWidth;

	return _int();
}

_int CBlood::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (m_pRendererCom == nullptr)
		return 0;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);


	return _int();
}

void CBlood::Render_GameObject()
{
	if (m_bDraw == false ||
		m_pBufferCom == nullptr ||
		m_pShaderCom == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
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

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);
}

void CBlood::Set_Position(const _vec3 * pPosition)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);
}

HRESULT CBlood::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		// For. Static Component
		m_pRendererCom = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
		if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
			return E_FAIL;

		m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
		if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
			return E_FAIL;

		m_pBufferCom = dynamic_cast<CBuffer_RcTex*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex"));
		if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
			return E_FAIL;

		//////////////////////////////////////////////////////////////////////////
		// Stage Component
		m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Shader_Effect"));
		if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
			return E_FAIL;

		m_pTextureCom = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Texture_Blood_anim_00_DEF"));
		if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
			return E_FAIL;


	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CBlood::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetFloat("g_fTimeAcc", 0.f);
	pEffect->SetFloat("g_fUSpeed", 0.f);
	pEffect->SetFloat("g_fVSpeed", 0.f);


	pEffect->SetInt("g_iWidth", m_iWidth);
	pEffect->SetInt("g_iHeight", m_iHeight);
	pEffect->SetInt("g_iAnimationX", m_iAnimationX);
	pEffect->SetInt("g_iAnimationY", m_iAnimationY);

	pEffect->SetVector("g_vAlpha", &_vec4(1.f, 1.f, 1.f, 1.f));

	if (FAILED(m_pTransformCom->SetUp_OnShader_BillBoardMatrix(pEffect, "g_matWorld")))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_AlphaTexture")))
		return E_FAIL;

	pEffect->SetVector("g_vMaterial", &_vec4(1.f, 1.f, 1.f, 1.f));

	_matrix matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);


	Safe_Release(pEffect);


	return S_OK;
}

CGameObject * CBlood::Clone_GameObject()
{
	CBlood*		pInstance = new CBlood(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CBlood Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBlood * CBlood::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBlood*		pInstance = new CBlood(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CBlood Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlood::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	CGameObject::Free();
}
