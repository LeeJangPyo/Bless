#include "stdafx.h"
#include "..\Headers\Damage.h"

#include "Management.h"

_USING(Client)

CDamage::CDamage(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pShaderCom(nullptr)
{

}

CDamage::CDamage(const CDamage & rhs)
	:CGameObject(rhs)
{
}

HRESULT CDamage::Ready_Prototype()
{
	return S_OK;
}

HRESULT CDamage::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_iDamege = 0;
	m_iUnits = 0;
	m_iTens = 0;
	m_iHundreds = 0;
	m_iThousands = 0;


	m_bThousands = false;

	m_fTimeAcc = 0.f;

	
	m_pTransformCom->Set_ScaleInfo(1.5f, 0.5f, 1.f);

	return S_OK;
}

_int CDamage::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return DEAD_OBJ;

	m_fTimeAcc += fTimeDelta *0.5f;
	
	m_pTransformCom->Go_Direction(&_vec3(0.f, 1.f, 0.f), 2.5f, fTimeDelta);

	return 0;
}


_int CDamage::LastUpdate_GameObject(const _float & fTimeDelta)
{
	Compute_ViewZ(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));

 	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI,this);

	if (m_fTimeAcc > 1.f)
	{
		m_bIsDead = true;
	}

	return 0;
}

void CDamage::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if (pEffect == nullptr)
		return;
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	pEffect->CommitChanges();
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CDamage::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//For. Renderer
		m_pRendererCom = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
		if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
			return E_FAIL;

		// For. Transform
		m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
		if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
			return E_FAIL;

		//For. Shader
		m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Shader_Damage"));
		if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
			return E_FAIL;

		//For. Texture
		m_pTextureCom = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Texture_Damage"));
		if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
			return E_FAIL;

		//For. Buffer
		m_pBufferCom = dynamic_cast<CBuffer_RcTex*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex"));
		if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
			return E_FAIL;

	}

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CDamage::SetUp_ConstantTable(LPD3DXEFFECT pEffect )
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();
	{
		pEffect->SetFloat("g_fTimeAcc", m_fTimeAcc);
		pEffect->SetBool("g_bThousands", m_bThousands);

		switch (m_eColor)
		{
		case Client::CDamage::RED:
			pEffect->SetVector("g_vMaterial", &_vec4(1.f, 0.f, 0.f, 1.f));
			break;
		case Client::CDamage::GREEN:
			pEffect->SetVector("g_vMaterial", &_vec4(0.f, 1.f, 0.f, 1.f));
			break;
		case Client::CDamage::WHITE:
			pEffect->SetVector("g_vMaterial", &_vec4(1.f, 1.f, 1.f, 1.f));
			break;
		}
		

		if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_UnitsTexture", m_iUnits)))
			return E_FAIL;
		if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_TensTexture", m_iTens)))
			return E_FAIL;
		if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_HundredsTexture", m_iHundreds)))
			return E_FAIL;
		if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_ThousandsTexture", m_iThousands)))
			return E_FAIL;

		

		CCamera* pCamera = CManagement::GetInstance()->Get_CameraMain();
		if (pCamera == nullptr)
		{
			Safe_Release(pEffect);
			return E_FAIL;
		}

		pEffect->SetMatrix("g_matWorld", &_matrix(pCamera->Get_BillBoardMatrix()* *m_pTransformCom->Get_WorldMatrix()));


		pCamera->SetUp_OnShader_View(pEffect, "g_matView");
		pCamera->SetUp_OnShader_Projection(pEffect, "g_matProj");
		//	pCamera->SetUp_OnShader_FarPlane(pEffect, "g_fFarPlane");

		Safe_Release(pEffect);
	}

	return S_OK;
}

HRESULT CDamage::SetUp_DamageInfo(const _int & iDamage, const _vec3* pPosition, const DAMAGE_COLOR& eColor)
{
	m_iDamege = iDamage;

	m_iThousands = m_iDamege / 1000;
	m_iDamege = m_iDamege % 1000;	
	if (m_iThousands != 0)
		m_bThousands = true;

	m_iHundreds = m_iDamege / 100;
	m_iDamege = m_iDamege % 100;

	m_iTens = m_iDamege / 10;

	m_iUnits = m_iDamege % 10;

	m_vPosition = *pPosition;
	m_vPosition.y += 5.f;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &m_vPosition);


	m_eColor = eColor;

	return S_OK;
}

CDamage * CDamage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDamage*	pInstance = new CDamage(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CDamage Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDamage::Clone_GameObject()
{
	CDamage*	pInstance = new CDamage(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CDamage Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamage::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
