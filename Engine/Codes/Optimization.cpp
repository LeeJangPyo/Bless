#include "../Headers/Optimization.h"


COptimization::COptimization(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
	, m_pFrustum(nullptr)
	, m_pQuadTree(nullptr)
	, m_bClone(false)
{
}

COptimization::COptimization(const COptimization& rhs)
	: CComponent(rhs)
	, m_pFrustum(rhs.m_pFrustum)
	, m_pQuadTree(rhs.m_pQuadTree)
	, m_bClone(true)
{
}



HRESULT Engine::COptimization::Ready_Optimization(_bool bUsed, const _ulong& dwCntX, const _ulong& dwCntZ)
{
	m_pFrustum = Engine::CFrustum::Create(m_pDevice);
	if (m_pFrustum == nullptr)
		return E_FAIL;

	if (bUsed == false)
		return S_OK;

	m_pQuadTree = CQuadTree::Create(dwCntX, dwCntZ);
	if (m_pQuadTree == nullptr)
		return E_FAIL;

	return S_OK;
}

Engine::COptimization* Engine::COptimization::Create(LPDIRECT3DDEVICE9 pDevice,
													_bool bUsed, 
													const _ulong& dwCntX, 
													const _ulong& dwCntZ)
{
	COptimization* pInstance = new COptimization(pDevice);

	if (FAILED(pInstance->Ready_Optimization(bUsed, dwCntX, dwCntZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* Engine::COptimization::Clone_Component(void)
{
	return new COptimization(*this);
}

void Engine::COptimization::Free(void)
{
	if (m_bClone == false)
	{
		Safe_Release(m_pFrustum);
		Safe_Release(m_pQuadTree);
	}

	CComponent::Free();
}

void COptimization::Is_InFrustum_ForTerrain(const _vec3* pVertexPos, const _ulong& dwCntX, const _ulong& dwCntZ, Engine::INDEX32* pIndex, _ulong* pTriCnt)
{
	m_pFrustum->Is_InFrustum_ForTerrain(pVertexPos, dwCntX, dwCntZ, pIndex, pTriCnt, m_pQuadTree);
}

_bool COptimization::Is_InFrustum_ForObject(const _vec3* pPosInWorld, const _float fRadius)
{
	return m_pFrustum->Is_InFrustum_ForObject(pPosInWorld, fRadius);
}

