#include "VIBuffer.h"


CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwVtxFVF(0)
	, m_dwTriCnt(0)
	, m_dwIdxSize(0)
{
	ZeroMemory(&m_IdxFmt, sizeof(D3DFORMAT));
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
	, m_IdxFmt(rhs.m_IdxFmt)
	, m_dwVtxSize(rhs.m_dwVtxSize)
{

	m_pVB->AddRef();
	m_pIB->AddRef();
}


HRESULT CVIBuffer::Ready_Buffer(void)
{
	m_pDevice->CreateVertexBuffer(
		m_dwVtxSize * m_dwVtxCnt,		// 정점 버퍼의 사이즈(바이트 단위)
		0,						// 정점 사용방식 : 버퍼를 만들 때 어떤 버퍼를 만들 것인가?(0인 경우 정적 버퍼)
		m_dwVtxFVF,				// 사용하는 버퍼 스타일
		D3DPOOL_MANAGED,		// 메모리 풀의 사용 방식
		&m_pVB,					// 버텍스 버퍼의 접근권한을 갖고 있는 객체 주소값
		NULL);					// 버퍼 만들 때 예약상태


	m_pDevice->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
		0,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL);

	return S_OK;
}

void Engine::CVIBuffer::Render_Buffer(void)
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetFVF(m_dwVtxFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}



void Engine::CVIBuffer::Free(void)
{

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	
	CComponent::Free();

}

