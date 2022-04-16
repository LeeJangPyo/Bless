#include "Buffer_RcTex.h"

Engine::CBuffer_RcTex::CBuffer_RcTex(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{

}

Engine::CBuffer_RcTex::CBuffer_RcTex(const CBuffer_RcTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CBuffer_RcTex::~CBuffer_RcTex(void)
{

}

HRESULT Engine::CBuffer_RcTex::Ready_Buffer(void)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;
	m_dwVtxFVF = VTXFVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if(FAILED(CVIBuffer::Ready_Buffer()))
	{
		_TAGMSG_BOX(L"VIBuffer Ready Failed, in ReadyBuffer", L"CBuffer_RcTex");
	}

	VTXTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
	pVtxTex[0].vTex = _vec2(0.f, 0.f);

	pVtxTex[1].vPos = _vec3(0.5f, 0.5f, 0.f);
	pVtxTex[1].vTex = _vec2(1.f, 0.f);

	pVtxTex[2].vPos = _vec3(0.5f, -0.5f, 0.f);
	pVtxTex[2].vTex = _vec2(1.f, 1.f);

	pVtxTex[3].vPos = _vec3(-0.5f, -0.5f, 0.f);
	pVtxTex[3].vTex = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX32*		pIndex = NULL;

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



Engine::CBuffer_RcTex* Engine::CBuffer_RcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBuffer_RcTex*		pInstance = new CBuffer_RcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		_MSG_BOX(L"CBuffer_RcTex Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CBuffer_RcTex::Free(void)
{
	CVIBuffer::Free();
}

Engine::CComponent* Engine::CBuffer_RcTex::Clone_Component()
{
	return new CBuffer_RcTex(*this);
}


