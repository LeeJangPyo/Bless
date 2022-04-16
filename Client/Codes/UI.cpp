#include "stdafx.h"
#include "..\Headers\UI.h"

#include "Management.h"

_USING(Client)

CUI::CUI(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
	, m_pTextureCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pInput_Device(CInput_Device::GetInstance())
{
	m_pDevice->AddRef();
	m_pInput_Device->AddRef();

}
void CUI::SetUp_LoadInfo(const _matrix * pmatWorld, const _tchar * pTextureComponentTag)
{
	m_pTransformCom->Set_WorldMatrix(pmatWorld);
	m_wstrTextureComponentTag = pTextureComponentTag;

	Replace_Texture_Component(pTextureComponentTag);
}
HRESULT CUI::Replace_Texture_Component(const _tchar* pComponentProtoTag)
{
	m_wstrTextureComponentTag = pComponentProtoTag;

	//Release Current Texture;
	Safe_Release(m_pTextureCom);

	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Texutre
	m_pTextureCom = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_STAGE, pComponentProtoTag));
	if (m_pTextureCom == nullptr)
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CUI::Ready_UI()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUI::Update_UI(const _float& fTimeDelta,const _matrix * pmatParent)
{
	
	return 0;
}

void CUI::Render_UI(LPD3DXEFFECT pEffect, const _matrix * pmatProj)
{
	if (pEffect == nullptr)
		return;
	pEffect->AddRef();
	{
		pEffect->BeginPass(0);

		if (FAILED(SetUp_ConstantTable(pEffect, pmatProj)))
			return;

		m_pBufferCom->Render_Buffer();


		pEffect->End();
	}
	Safe_Release(pEffect);
}


HRESULT CUI::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Transfrom
	m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
//	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
	if (m_pTransformCom == nullptr)
		return E_FAIL;

	//For. Buffer
	m_pBufferCom = dynamic_cast<CBuffer_RcTex*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex"));
//	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
	if (m_pBufferCom == nullptr)
		return E_FAIL;



	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix * pmatProj)
{

	return S_OK;
}

void Client::CUI::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pInput_Device);

	Safe_Release(m_pDevice);
}
