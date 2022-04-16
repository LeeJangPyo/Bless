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
		m_dwVtxSize * m_dwVtxCnt,		// ���� ������ ������(����Ʈ ����)
		0,						// ���� ����� : ���۸� ���� �� � ���۸� ���� ���ΰ�?(0�� ��� ���� ����)
		m_dwVtxFVF,				// ����ϴ� ���� ��Ÿ��
		D3DPOOL_MANAGED,		// �޸� Ǯ�� ��� ���
		&m_pVB,					// ���ؽ� ������ ���ٱ����� ���� �ִ� ��ü �ּҰ�
		NULL);					// ���� ���� �� �������


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

