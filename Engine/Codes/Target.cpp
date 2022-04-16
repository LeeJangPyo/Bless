#include "..\Headers\Target.h"


#include "Buffer_Screen.h"

CTarget::CTarget(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
	, m_pTarget_Texture(nullptr)
	, m_pTarget_Surface(nullptr)
	, m_pOld_Surface(nullptr)
	, m_pBuffer_Screen(nullptr)
	, m_pShadowDepth_Surface(nullptr)
	, m_pOld_Dpeth_Surface(nullptr)
{
	m_pDevice->AddRef();
}

HRESULT CTarget::Ready_Target(const _float & fSizeX, const _float & fSizeY, D3DFORMAT Format, D3DXCOLOR Clear_Color, TARGETTYPE eType)
{
	m_Clear_Color = Clear_Color;

	if (eType == TARGET_NORMAL)
	{
		if (FAILED(D3DXCreateTexture(m_pDevice, (_uint)fSizeX, (_uint)fSizeY, 1, D3DUSAGE_RENDERTARGET, Format, D3DPOOL_DEFAULT, &m_pTarget_Texture)))
			return E_FAIL;

		if (FAILED(m_pTarget_Texture->GetSurfaceLevel(0, &m_pTarget_Surface)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(D3DXCreateTexture(m_pDevice, (_uint)fSizeX, (_uint)fSizeY, 1, D3DUSAGE_RENDERTARGET, Format, D3DPOOL_DEFAULT, &m_pTarget_Texture)))
			return E_FAIL;

		if (FAILED(m_pTarget_Texture->GetSurfaceLevel(0, &m_pTarget_Surface)))
			return E_FAIL;

		if (FAILED(m_pDevice->CreateDepthStencilSurface((_uint)fSizeX, (_uint)fSizeY, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pShadowDepth_Surface, NULL)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CTarget::Ready_DebugBuffer(const _float & fStartX, const _float & fStartY, const _float & fSizeX, const _float & fSizeY)
{
	
	m_pBuffer_Screen = CBuffer_Screen::Create(m_pDevice, fStartX, fStartY, fSizeX, fSizeY);

	return S_OK;
}

HRESULT CTarget::SetUp_OnGraphic_Device(const _uint & iIndex)
{
	if (m_pDevice == nullptr ||
		m_pTarget_Surface == nullptr)
		return E_FAIL;


	//Backup Back Buffer
	m_pDevice->GetRenderTarget(iIndex, &m_pOld_Surface);

	//Target Set to Device
	if (FAILED(m_pDevice->SetRenderTarget(iIndex, m_pTarget_Surface)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTarget::Release_OnGraphic_Device(_uint iIndex)
{
	if (m_pDevice == nullptr ||
		m_pTarget_Surface == nullptr)
		return E_FAIL;

	//Recover Back Buffer
	if (FAILED(m_pDevice->SetRenderTarget(iIndex, m_pOld_Surface)))
		return E_FAIL;

	Safe_Release(m_pOld_Surface);
		
	return S_OK;
}

HRESULT CTarget::SetUp_OnGraphic_Device_Shadow(const _uint & iIndex)
{
	if (m_pDevice == nullptr ||
		m_pTarget_Surface == nullptr)
		return E_FAIL;


	//Backup Back Buffer
	m_pDevice->GetRenderTarget(iIndex, &m_pOld_Surface);

	//Backup Depth Buffer
	m_pDevice->GetDepthStencilSurface(&m_pOld_Dpeth_Surface);
	//Safe_Release(m_pOld_Dpeth_Surface);

	//Target Set to Device
	if (FAILED(m_pDevice->SetRenderTarget(iIndex, m_pTarget_Surface)))
		return E_FAIL;

	//DepthBUffer Set to Device
	if (FAILED(m_pDevice->SetDepthStencilSurface(m_pShadowDepth_Surface)))
		return E_FAIL;

//	Safe_Release(m_pShadowDepth_Surface);

	return S_OK;
}

HRESULT CTarget::Release_OnGraphic_Device_Shadow(_uint iIndex)
{
	if (m_pDevice == nullptr ||
		m_pTarget_Surface == nullptr)
		return E_FAIL;

	//Recover Back Buffer
	if (FAILED(m_pDevice->SetRenderTarget(iIndex, m_pOld_Surface)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetDepthStencilSurface(m_pOld_Dpeth_Surface)))
		return E_FAIL;

	Safe_Release(m_pOld_Surface);
	Safe_Release(m_pOld_Dpeth_Surface);

	return S_OK;
}

HRESULT CTarget::Clear_Target()
{
	if (m_pDevice  == nullptr ||
		m_pTarget_Surface == nullptr)
		return E_FAIL;

	//Backup Back Buffer
	m_pDevice->GetRenderTarget(0, &m_pOld_Surface);

	//Target Set to Device
	if (FAILED(m_pDevice->SetRenderTarget(0, m_pTarget_Surface)))
		return E_FAIL;

	//Initalize Target Buffer
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, m_Clear_Color, 1.f, 0);

	//Recover Back Buffer
	if (FAILED(m_pDevice->SetRenderTarget(0, m_pOld_Surface)))
		return E_FAIL;

	Safe_Release(m_pOld_Surface);

	return S_OK;
}

HRESULT CTarget::Clear_Shadow_Target()
{
	if (m_pDevice == nullptr ||
		m_pTarget_Surface == nullptr)
		return E_FAIL;

	//Backup Back Buffer
	m_pDevice->GetRenderTarget(0, &m_pOld_Surface);

	//Backup Back Buffer
	m_pDevice->GetDepthStencilSurface(&m_pOld_Dpeth_Surface);

	//Target Set to Device
	if (FAILED(m_pDevice->SetRenderTarget(0, m_pTarget_Surface)))
		return E_FAIL;

	//Set Buffer
	if (FAILED(m_pDevice->SetDepthStencilSurface(m_pShadowDepth_Surface)))
		return E_FAIL;
	//Safe_Release(m_pShadowDepth_Surface);

	//Initalize Target Buffer
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, m_Clear_Color, 1.f, 0);

	//Recover Back Buffer
	if (FAILED(m_pDevice->SetRenderTarget(0, m_pOld_Surface)))
		return E_FAIL;

	//Recover Depth Buffer
	if (FAILED(m_pDevice->SetDepthStencilSurface(m_pOld_Dpeth_Surface)))
		return E_FAIL;

	Safe_Release(m_pOld_Surface);
	Safe_Release(m_pOld_Dpeth_Surface);

	return S_OK;
}

void CTarget::Render_DebugBuffer()
{
	m_pDevice->SetTexture(0, m_pTarget_Texture);

	m_pBuffer_Screen->Render_Buffer();
}

HRESULT CTarget::SetUp_Texture_OnShader(LPD3DXEFFECT pEffect, const _char * pConstantName)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetTexture(pConstantName, m_pTarget_Texture);

	Safe_Release(pEffect);

	return S_OK;
}

CTarget * CTarget::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _float & fSizeX, const _float & fSizeY, D3DFORMAT Format,D3DXCOLOR Clear_Color, TARGETTYPE eType)
{
	CTarget*		pInstance = new CTarget(pGraphic_Device);

	if (FAILED(pInstance->Ready_Target(fSizeX, fSizeY, Format, Clear_Color, eType)))
	{
		_MSG_BOX(L"CTarget Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTarget::Free()
{
	_ulong dwfree = 0;
	Safe_Release(m_pBuffer_Screen);

	Safe_Release(m_pTarget_Surface);
	Safe_Release(m_pTarget_Texture);
	dwfree = Safe_Release(m_pShadowDepth_Surface);
	dwfree = 0;
	dwfree = Safe_Release(m_pOld_Dpeth_Surface);

	Safe_Release(m_pDevice);
}
