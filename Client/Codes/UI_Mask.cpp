#include "stdafx.h"
#include "..\Headers\UI_Mask.h"

#include "Management.h"

_USING(Client)

CUI_Mask::CUI_Mask(LPDIRECT3DDEVICE9 pDevice)
	:CUI(pDevice)
	, m_fRadio(1.f)
{
}


HRESULT CUI_Mask::Ready_UI()
{
	if (FAILED(CUI::Ready_UI()))
		return E_FAIL;

	m_fRadio = 1.f;
	return S_OK;
}

_int CUI_Mask::Update_UI(const _float & fTimeDelta, const _matrix * pmatParent)
{
	m_matCombinedWorld = (*m_pTransformCom->Get_WorldMatrix()) * (*pmatParent);


	return _int();
}

void CUI_Mask::Render_UI(LPD3DXEFFECT pEffect, const _matrix * pmatProj)
{
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();
	{
		pEffect->BeginPass(SHADER_PATH::BARMASK);

		if (FAILED(SetUp_ConstantTable(pEffect, pmatProj)))
			return;
		pEffect->CommitChanges();
		m_pBufferCom->Render_Buffer();


		pEffect->EndPass();
	}
	Safe_Release(pEffect);
}

HRESULT CUI_Mask::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix * pmatProj)
{
	if (pEffect == nullptr)
		return E_FAIL;
	pEffect->AddRef();

	_matrix matView, matProj;
	D3DXMatrixIdentity(&matView);

	pEffect->SetMatrix("g_matWorld", &m_matCombinedWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", pmatProj);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", 12);

	pEffect->SetFloat("g_fRadio", m_fRadio);


	Safe_Release(pEffect);
	
	return S_OK;
}

CUI_Mask * CUI_Mask::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CUI_Mask*	pInstance = new CUI_Mask(pDevice);

	if (FAILED(pInstance->Ready_UI()))
	{
		_MSG_BOX(L"CUI_Mask Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Mask::Free()
{
	CUI::Free();
}
