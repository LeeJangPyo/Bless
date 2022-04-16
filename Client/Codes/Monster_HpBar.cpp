#include "stdafx.h"
#include "..\Headers\Monster_HpBar.h"


#include "Management.h"

_USING(Client)

CMonster_HpBar::CMonster_HpBar(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fHpRadio(0.f)
{
}

CMonster_HpBar::CMonster_HpBar(const CMonster_HpBar & rhs)
	:CGameObject(rhs)
	, m_fHpRadio(0.f)
{
}

HRESULT CMonster_HpBar::Ready_Prototype()
{
	return S_OK;
}

HRESULT CMonster_HpBar::Ready_GameObject()
{

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransform->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0.f, 5.f,0.f));
	m_pTransform->Set_ScaleInfo(&_vec3(1.5f, 0.15f, 1.f));

	return S_OK;
}

_int CMonster_HpBar::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return DEAD_OBJ;
	return _int();
}

_int CMonster_HpBar::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (m_pRenderer == nullptr || m_pOptimization == nullptr)
		return 0;

	m_bDraw = m_pOptimization->Is_InFrustum_ForObject(m_pTransform->Get_StateInfo(CTransform::STATE_POSITION), 0.f);
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA,this);

	return _int();
}

void CMonster_HpBar::Render_GameObject()
{
	if (m_bDraw == false)
		return;

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	pEffect->AddRef();


	pEffect->Begin(nullptr, 0);
	{
	
		pEffect->BeginPass(0);
		{	
			//Render Back
			if (FAILED(SetUp_ConstantTable_Back(pEffect)))
			{
				Safe_Release(pEffect);
				return;
			}
			pEffect->CommitChanges();

			m_pBuffer_Back->Render_Buffer();


			//Render Bar
			if (FAILED(SetUp_ConstantTable_Bar(pEffect)))
			{
				Safe_Release(pEffect);
				return;
			}
			pEffect->CommitChanges();

			m_pBuffer_Bar->Render_Buffer();

		}
		pEffect->EndPass();

		pEffect->BeginPass(2);
		{
			//Render Mask
			if (FAILED(SetUp_ConstantTable_Mask(pEffect)))
			{
				Safe_Release(pEffect);
				return;
			}
			pEffect->CommitChanges();

			m_pBuffer_Mask->Render_Buffer();
		}
		pEffect->EndPass();



	}
	pEffect->End();

	Safe_Release(pEffect);
}

void CMonster_HpBar::Set_Position(const _vec3 * pPosition)
{
	_vec3 vPosition = *pPosition;
	vPosition.y += 3.5f;
	m_pTransform->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);
	
}

HRESULT CMonster_HpBar::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//////////////////////////////////////////////////////////////////////////
	//Static Component
	m_pRenderer = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (FAILED(Add_Component(L"Com_Renderer", m_pRenderer)))
		return E_FAIL;

	m_pTransform = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (FAILED(Add_Component(L"Com_Transform", m_pTransform)))
		return E_FAIL;

	m_pOptimization = dynamic_cast<COptimization*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Optimization"));
	if (FAILED(Add_Component(L"Com_Optimization", m_pOptimization)))
		return E_FAIL;


	//////////////////////////////////////////////////////////////////////////
	//Stage Component
	//For. Shader
	m_pShader = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Shader_Monster_UI"));
	if (FAILED(Add_Component(L"Com_Shader", m_pShader)))
		return E_FAIL;

	//For. Texutre
	m_pTexture = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Texture_HPGauge"));
	if (FAILED(Add_Component(L"Com_Texture", m_pTexture)))
		return E_FAIL;

	//For. Buffer
	m_pBuffer_Back = dynamic_cast<CBuffer_RcTex*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex"));
	if (FAILED(Add_Component(L"Com_Buffer_Back", m_pBuffer_Back)))
		return E_FAIL;

	m_pBuffer_Bar = dynamic_cast<CBuffer_RcTex*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex"));
	if (FAILED(Add_Component(L"Com_Buffer_Bar", m_pBuffer_Bar)))
		return E_FAIL;

	m_pBuffer_Mask = dynamic_cast<CBuffer_RcTex*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex"));
	if (FAILED(Add_Component(L"Com_Buffer_Mask", m_pBuffer_Bar)))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CMonster_HpBar::SetUp_ConstantTable_Back(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();
	{
		if (FAILED(m_pTransform->SetUp_OnShader_BillBoardMatrix(pEffect, "g_matWorld")))
			return E_FAIL;

		//if (FAILED(m_pTransform->SetUp_OnShader_MultiplyParnetMatrix_BillBoadMatrix(pEffect,m_pParentMatrix, "g_matWorld")))
		//	return E_FAIL;
		

		if (FAILED(m_pTexture->SetUp_OnShader(pEffect,"g_BaseTexture", 0)))
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
	}

	return S_OK;
}

HRESULT CMonster_HpBar::SetUp_ConstantTable_Bar(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();
	{

		if (FAILED(m_pTransform->SetUp_OnShader_BillBoardMatrix(pEffect, "g_matWorld")))
			return E_FAIL;
		//if (FAILED(m_pTransform->SetUp_OnShader_MultiplyParnetMatrix_BillBoadMatrix(pEffect, m_pParentMatrix, "g_matWorld")))
		//	return E_FAIL;

						
		if (FAILED(m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", 1)))
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
	}

	return S_OK;
}

HRESULT CMonster_HpBar::SetUp_ConstantTable_Mask(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();
	{
		pEffect->SetFloat("g_fRadio", m_fHpRadio);

		if (FAILED(m_pTransform->SetUp_OnShader_BillBoardMatrix(pEffect, "g_matWorld")))
			return E_FAIL;
		/*if (FAILED(m_pTransform->SetUp_OnShader_MultiplyParnetMatrix_BillBoadMatrix(pEffect, m_pParentMatrix, "g_matWorld")))
			return E_FAIL;*/

		if (FAILED(m_pTexture->SetUp_OnShader(pEffect, "g_BaseTexture", 2)))
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
	}

	return S_OK;
}

CGameObject * CMonster_HpBar::Clone_GameObject()
{
	CMonster_HpBar*		pInstance = new CMonster_HpBar(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CGameObject Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMonster_HpBar * CMonster_HpBar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonster_HpBar*		pInstance = new CMonster_HpBar(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CMonster_HpBar Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_HpBar::Free()
{
	Safe_Release(m_pBuffer_Mask);
	Safe_Release(m_pBuffer_Back);
	Safe_Release(m_pBuffer_Bar);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);
	Safe_Release(m_pShader);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pOptimization);

	CGameObject::Free();
}
