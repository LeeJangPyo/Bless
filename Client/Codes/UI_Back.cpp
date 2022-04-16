#include "stdafx.h"
#include "..\Headers\UI_Back.h"

#include "Management.h"

_USING(Client)

CUI_Back::CUI_Back(LPDIRECT3DDEVICE9 pDevice)
	:CUI(pDevice)
{
}


HRESULT CUI_Back::Ready_UI()
{
	if (FAILED(CUI::Ready_UI()))
		return E_FAIL;

	return S_OK;
}

_int CUI_Back::Update_UI(const _float & fTimeDelta, const _matrix * pmatParent)
{
	m_matCombinedWorld = (*m_pTransformCom->Get_WorldMatrix()) * (*pmatParent);


	return _int();
}

void CUI_Back::Render_UI(LPD3DXEFFECT pEffect, const _matrix * pmatProj)
{
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();
	{
		pEffect->BeginPass(SHADER_PATH::DEFAULT);

		if (FAILED(SetUp_ConstantTable(pEffect, pmatProj)))
			return;
		pEffect->CommitChanges();
		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
	}
	Safe_Release(pEffect);
}

HRESULT CUI_Back::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix * pmatProj)
{
	if (pEffect == nullptr)
		return E_FAIL;
	pEffect->AddRef();

	_matrix matView, matProj;
	D3DXMatrixIdentity(&matView);

	pEffect->SetMatrix("g_matWorld", &m_matCombinedWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", pmatProj);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iTextureIndex);


	Safe_Release(pEffect);

	return S_OK;
}

CUI_Back * CUI_Back::Create(LPDIRECT3DDEVICE9 pDevice)
{
	
	CUI_Back*	pInstance = new CUI_Back(pDevice);

	if (FAILED(pInstance->Ready_UI()))
	{
		_MSG_BOX(L"CUI_Back Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CUI_Back::Free()
{
	CUI::Free();
}
