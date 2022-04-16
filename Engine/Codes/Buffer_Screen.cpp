#include "..\Headers\Buffer_Screen.h"


CBuffer_Screen::CBuffer_Screen(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CBuffer_Screen::CBuffer_Screen(const CBuffer_Screen & rhs)
	:CVIBuffer(rhs)
{
}


HRESULT CBuffer_Screen::Ready_Buffer(void)
{
	m_dwVtxSize = sizeof(VTXVIEWPORT);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;
	m_dwVtxFVF = VTXFVF_VIEWPORT;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Buffer()))
	{
		_TAGMSG_BOX(L"VIBuffer Ready Failed, in ReadyBuffer", L"CBuffer_RcTex");
	}

	D3DVIEWPORT9			ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pDevice->GetViewport(&ViewPort);

	VTXVIEWPORT*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec4(0.0f - 0.5f, 0.0f - 0.5f, 0.f, 1.f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPosition = _vec4(ViewPort.Width - 0.5f, 0.0f - 0.5f, 0.f, 1.f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPosition = _vec4(ViewPort.Width - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPosition = _vec4(0.0f - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CBuffer_Screen::Ready_Buffer(const _float & fStartX, const _float & fStartY, const _float & fSizeX, const _float & fSizeY)
{
	m_dwVtxSize = sizeof(VTXVIEWPORT);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;
	m_dwVtxFVF = VTXFVF_VIEWPORT;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Buffer()))
	{
		_TAGMSG_BOX(L"VIBuffer Ready Failed, in ReadyBuffer", L"CBuffer_RcTex");
	}

	VTXVIEWPORT*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec4(fStartX, fStartY, 0.f, 1.f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPosition = _vec4(fStartX + fSizeX, fStartY, 0.f, 1.f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPosition = _vec4(fStartX + fSizeX, fStartY + fSizeY, 0.f, 1.f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPosition = _vec4(fStartX, fStartY + fSizeY, 0.f, 1.f);
	pVertices[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

CBuffer_Screen * CBuffer_Screen::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBuffer_Screen*		pInstance = new CBuffer_Screen(pDevice);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		_MSG_BOX(L"CBuffer_Screen Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CBuffer_Screen * CBuffer_Screen::Create(LPDIRECT3DDEVICE9 pDevice, const _float & fStartX, const _float &fStartY, const _float &fSizeX, const _float &fSizeY)
{
	CBuffer_Screen*		pInstance = new CBuffer_Screen(pDevice);

	if (FAILED(pInstance->Ready_Buffer(fStartX, fStartY, fSizeX, fSizeY)))
	{
		_MSG_BOX(L"CBuffer_Screen Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CBuffer_Screen::Clone_Component()
{
	return new CBuffer_Screen(*this);
}

void CBuffer_Screen::Free()
{
	CVIBuffer::Free();
}
