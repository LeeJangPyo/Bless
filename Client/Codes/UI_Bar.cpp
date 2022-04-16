#include "stdafx.h"
#include "..\Headers\UI_Bar.h"

#include "Management.h"

_USING(Client)

CUI_Bar::CUI_Bar(LPDIRECT3DDEVICE9 pDevice)
	:CUI(pDevice)
	, m_fTimeAcc(0.f)
{
}


HRESULT CUI_Bar::Ready_UI()
{
	if (FAILED(CUI::Ready_UI()))
		return E_FAIL;

	return S_OK;
}

_int CUI_Bar::Update_UI(const _float & fTimeDelta, const _matrix * pmatParent)
{
	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc > 1.f)
		m_fTimeAcc = 0.f;


	m_matCombinedWorld = (*m_pTransformCom->Get_WorldMatrix()) * (*pmatParent);


	return _int();
}

void CUI_Bar::Render_UI(LPD3DXEFFECT pEffect,const _matrix * pmatProj)
{
	if (pEffect == nullptr)
		return;
	pEffect->AddRef();
	{
		pEffect->BeginPass(SHADER_PATH::BAR);

		if (FAILED(SetUp_ConstantTable(pEffect, pmatProj)))
			return; 
		pEffect->CommitChanges();
		m_pBufferCom->Render_Buffer();


		pEffect->EndPass();
	}
	Safe_Release(pEffect);
}

HRESULT CUI_Bar::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix * pmatProj)
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
	m_pTextureCom->SetUp_OnShader(pEffect, "g_ShadeTexture", 7);

	pEffect->SetFloat("g_fTimeAcc", m_fTimeAcc);


	Safe_Release(pEffect);	

	return S_OK;
}

CUI_Bar * CUI_Bar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CUI_Bar*	pInstance = new CUI_Bar(pDevice);

	if (FAILED(pInstance->Ready_UI()))
	{
		_MSG_BOX(L"CUI_Bar Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Bar::Free()
{
	CUI::Free();
}
