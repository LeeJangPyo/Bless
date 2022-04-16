#include "stdafx.h"
#include "..\Headers\UI_IconMask.h"

#include "Management.h"

_USING(Client)

CUI_IconMask::CUI_IconMask(LPDIRECT3DDEVICE9 pDevice)
	:CUI(pDevice)
	, m_fRadio(1.f)
	, m_bIsCoolDown(false)
	, m_fTimeAcc(0.f)
	, m_fCoolDown(0.f)
{
}


HRESULT CUI_IconMask::Ready_UI()
{
	if (FAILED(CUI::Ready_UI()))
		return E_FAIL;


	return S_OK;
}

_int CUI_IconMask::Update_UI(const _float & fTimeDelta, const _matrix * pmatParent)
{

	m_matCombinedWorld = (*m_pTransformCom->Get_WorldMatrix()) * (*pmatParent);

	if (m_bIsCoolDown)	//재사용대기중이면
	{
		m_fTimeAcc += fTimeDelta;
		m_fRadio = m_fTimeAcc / m_fCoolDown;
		if (m_fTimeAcc > m_fCoolDown)
		{
			m_fTimeAcc = 0.f;
			m_bIsCoolDown = false;

			return COOLDOWN_END;
		}
	}
	
	//if (GetAsyncKeyState(VK_F4))
	//	m_bIsCoolDown = true;


	return 0;
}

void CUI_IconMask::Render_UI(LPD3DXEFFECT pEffect,const _matrix * pmatProj)
{
	if (pEffect == nullptr)
		return;

	_int iPassIndex;
	if (!m_bIsCoolDown)	//재사용중이 아니면
		iPassIndex = SHADER_PATH::ICONMASK;
	else
		iPassIndex = SHADER_PATH::ICONCOOLDOWN;

	pEffect->AddRef();
	{
		pEffect->BeginPass(iPassIndex);

		if (FAILED(SetUp_ConstantTable(pEffect, pmatProj)))
			return;
		pEffect->CommitChanges();

		m_pBufferCom->Render_Buffer();


		pEffect->EndPass();
	}
	Safe_Release(pEffect);
}

HRESULT CUI_IconMask::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix * pmatProj)
{
	if (pEffect == nullptr)
		return E_FAIL;
	pEffect->AddRef();

	_matrix matView, matProj;
	D3DXMatrixIdentity(&matView);

	pEffect->SetMatrix("g_matWorld", &m_matCombinedWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", pmatProj);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", 2);

	pEffect->SetFloat("g_fRadio", m_fRadio);


	Safe_Release(pEffect);

	return S_OK;
}

CUI_IconMask * CUI_IconMask::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CUI_IconMask*	pInstance = new CUI_IconMask(pDevice);

	if (FAILED(pInstance->Ready_UI()))
	{
		_MSG_BOX(L"CUI_IconMask Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IconMask::Free()
{
	CUI::Free();
}
