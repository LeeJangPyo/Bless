#include "Texture.h"


Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pDeivce)
	:CComponent(pDeivce)
{

}

Engine::CTexture::CTexture(const CTexture& rhs)
	:CComponent(rhs)
{
	_uint	iContainerSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iContainerSize);

	m_vecTexture = rhs.m_vecTexture;

	for (_uint i = 0; i < iContainerSize; ++i)
		m_vecTexture[i]->AddRef();

}


void Engine::CTexture::Set_Texture(const _uint& iIndex /*= 0*/)
{
	if (m_vecTexture.size() < iIndex)
		return;
	
	m_pDevice->SetTexture(0, m_vecTexture[iIndex]);
}

void Engine::CTexture::Set_Texture(LPD3DXEFFECT pEffect, const char* pShaderName, const _uint& iIndex /*= 0*/)
{
	if (m_vecTexture.size() < iIndex)
		return;


	pEffect->SetTexture(pShaderName, m_vecTexture[iIndex]);
}

_uint CTexture::Get_TextureSize()
{
	return m_vecTexture.size();
}

HRESULT Engine::CTexture::Ready_Texture(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCount /*= 1*/)
{
	m_vecTexture.reserve(iCount);

	IDirect3DBaseTexture9*				pTexture = nullptr;

	for (_uint i = 0; i < iCount; ++i)
	{
		TCHAR			szFileName[256] = L"";

		wsprintf(szFileName, pPath, i);

		switch (eType)
		{
		case TEX_NORMAL:
			if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;

		case TEX_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pDevice, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}
		m_vecTexture.push_back(pTexture);

	}
	return S_OK;

}

HRESULT CTexture::Ready_Texture()
{
	//Ready Texture
	LPDIRECT3DTEXTURE9 pTexture;

	// For . Red Texture  0
	m_pDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);

	D3DLOCKED_RECT			LockRect;
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	pTexture->LockRect(0, &LockRect, NULL, 0);

	*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pTexture->UnlockRect(0);

	m_vecTexture.push_back(pTexture);
	
	// For . Green Texture 1
	m_pDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);


	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	pTexture->LockRect(0, &LockRect, NULL, 0);

	*((_ulong*)LockRect.pBits) = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pTexture->UnlockRect(0);

	m_vecTexture.push_back(pTexture);

	// For . Yellow Texture 2
	m_pDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);

	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	pTexture->LockRect(0, &LockRect, NULL, 0);

	*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pTexture->UnlockRect(0);

	m_vecTexture.push_back(pTexture);

	// For . Blue Texture 3
	m_pDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);

	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	pTexture->LockRect(0, &LockRect, NULL, 0);

	*((_ulong*)LockRect.pBits) = D3DXCOLOR(0.f, 1.f, 1.f, 1.f);

	pTexture->UnlockRect(0);

	m_vecTexture.push_back(pTexture);

	return S_OK;
}

HRESULT CTexture::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, const _uint & iIndex)
{
	if (pEffect == nullptr)
		return E_FAIL;

	if (m_vecTexture.size() <= iIndex)
		return E_FAIL;

	pEffect->SetTexture(pConstantName, m_vecTexture[iIndex]);

	return S_OK;
}


Engine::CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCount /*= 1*/)
{
	CTexture*	pInstance = new CTexture(pDevice);

	if (FAILED(pInstance->Ready_Texture(pPath, eType, iCount)))
	{
		_MSG_BOX(L"Texture Creation Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTexture*	pInstance = new CTexture(pDevice);

	if (FAILED(pInstance->Ready_Texture()))
	{
		_MSG_BOX(L"Texture Creation Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTexture::Clone_Component(void)
{
	return new CTexture(*this);
}

void Engine::CTexture::Free(void)
{
	CComponent::Free();

	_uint	iContainerSize = m_vecTexture.size();

	for (_uint i = 0; i < iContainerSize; ++i)
	{
		Safe_Release(m_vecTexture[i]);
	}
	m_vecTexture.clear();
}

