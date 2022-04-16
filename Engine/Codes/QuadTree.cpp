#include "QuadTree.h"
#include "Frustum.h"


CQuadTree::CQuadTree(void)
{
	ZeroMemory(m_pChild, sizeof(CQuadTree*) * CHILD_END);
	ZeroMemory(m_dwCorner, sizeof(_ulong) * CORNER_END);
	ZeroMemory(m_pNeighbor, sizeof(CQuadTree*) * NEIGHBOR_END);
}


HRESULT CQuadTree::Ready_QuadTree(const _ulong& dwCntX, const _ulong& dwCntZ)
{
	m_dwCorner[CORNER_LT] = dwCntX * dwCntZ - dwCntX;
	m_dwCorner[CORNER_RT] = dwCntX * dwCntZ - 1;
	m_dwCorner[CORNER_LB] = 0;
	m_dwCorner[CORNER_RB] = dwCntX - 1;

	m_dwCenter = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 2;
	
	Make_ChildQuadTree();

	return S_OK;
}

void CQuadTree::CullingForTerrain(Engine::CFrustum* pFrustum, 
											const _vec3* pVertex, 
											INDEX32* pIndex, 
											_ulong* pTrinCnt)
{
	m_fRadius = D3DXVec3Length(&(pVertex[m_dwCorner[CORNER_LT]] - pVertex[m_dwCenter]));

	_bool bIsIn = pFrustum->IsIn_Frustum(&pVertex[m_dwCenter], m_fRadius);

	if (true == bIsIn)
	{
		//if (nullptr == m_pChild[0])
		if(true == LevelOfDetail(pFrustum->GetDevice(), pVertex))
		{
			_bool	bIsNeighbor[NEIGHBOR_END] = { true, true, true, true };


			for (_ulong i = 0; i < NEIGHBOR_END; ++i)
			{
				// 이웃이 있다면 이웃의 lod를 적용하겠다는 뜻, 
				// 위에서 true로 초기화한 이유는 이웃이 없는 경우는 새로운 lod를 적용하기 위함
				if (nullptr != m_pNeighbor[i])
					bIsNeighbor[i] = m_pNeighbor[i]->LevelOfDetail(pFrustum->GetDevice(), pVertex);
			}
			
			_bool		bIsInVertex[3] = { false };

			// 오른쪽 위
			bIsInVertex[0] = pFrustum->IsIn_Frustum(&pVertex[m_dwCorner[CORNER_LT]], m_fRadius);
			bIsInVertex[1] = pFrustum->IsIn_Frustum(&pVertex[m_dwCorner[CORNER_RT]], m_fRadius);
			bIsInVertex[2] = pFrustum->IsIn_Frustum(&pVertex[m_dwCorner[CORNER_RB]], m_fRadius);

			if (true == bIsInVertex[0] || true == bIsInVertex[1] || true == bIsInVertex[2])
			{
				if (bIsNeighbor[NEIGHBOR_TOP] && bIsNeighbor[NEIGHBOR_RIGHT])
				{
					if (nullptr == m_pChild[0])
					{
						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_LT];
						pIndex[*pTrinCnt]._2 = m_dwCorner[CORNER_RT];
						pIndex[*pTrinCnt]._3 = m_dwCorner[CORNER_RB];
						++(*pTrinCnt);
					}

					else
					{
						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_LT];
						pIndex[*pTrinCnt]._2 = m_dwCorner[CORNER_RT];
						pIndex[*pTrinCnt]._3 = m_dwCenter;
						++(*pTrinCnt);

						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_RT];
						pIndex[*pTrinCnt]._2 = m_dwCorner[CORNER_RB];
						pIndex[*pTrinCnt]._3 = m_dwCenter;
						++(*pTrinCnt);
					}
				}

				else
				{
					if (false == bIsNeighbor[NEIGHBOR_TOP])
					{
						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_LT];
						pIndex[*pTrinCnt]._2 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT]) >> 1;
						pIndex[*pTrinCnt]._3 = m_dwCenter;
						++(*pTrinCnt);

						pIndex[*pTrinCnt]._1 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT]) >> 1;
						pIndex[*pTrinCnt]._2 = m_dwCorner[CORNER_RT];
						pIndex[*pTrinCnt]._3 = m_dwCenter;
						++(*pTrinCnt);
					}
					else
					{
						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_LT];
						pIndex[*pTrinCnt]._2 = m_dwCorner[CORNER_RT];
						pIndex[*pTrinCnt]._3 = m_dwCenter;
						++(*pTrinCnt);
					}

					if (false == bIsNeighbor[NEIGHBOR_RIGHT])
					{
						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_RT];
						pIndex[*pTrinCnt]._2 = (m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_RB]) >> 1;
						pIndex[*pTrinCnt]._3 = m_dwCenter;
						++(*pTrinCnt);

						pIndex[*pTrinCnt]._1 = (m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_RB]) >> 1;
						pIndex[*pTrinCnt]._2 = m_dwCorner[CORNER_RB];
						pIndex[*pTrinCnt]._3 = m_dwCenter;
						++(*pTrinCnt);
					}
					else
					{
						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_RT];
						pIndex[*pTrinCnt]._2 = m_dwCorner[CORNER_RB];
						pIndex[*pTrinCnt]._3 = m_dwCenter;
						++(*pTrinCnt);
					}
				}				
			}

			// 왼쪽 아래
			bIsInVertex[0] = pFrustum->IsIn_Frustum(&pVertex[m_dwCorner[CORNER_LT]], m_fRadius);
			bIsInVertex[1] = pFrustum->IsIn_Frustum(&pVertex[m_dwCorner[CORNER_RB]], m_fRadius);
			bIsInVertex[2] = pFrustum->IsIn_Frustum(&pVertex[m_dwCorner[CORNER_LB]], m_fRadius);

			if (true == bIsInVertex[0] || true == bIsInVertex[1] || true == bIsInVertex[2])
			{
				if (bIsNeighbor[NEIGHBOR_LEFT] && bIsNeighbor[NEIGHBOR_BOTTOM])
				{
					if (nullptr == m_pChild[0])
					{
						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_LT];
						pIndex[*pTrinCnt]._2 = m_dwCorner[CORNER_RB];
						pIndex[*pTrinCnt]._3 = m_dwCorner[CORNER_LB];
						++(*pTrinCnt);
					}

					else
					{
						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_LT];
						pIndex[*pTrinCnt]._2 = m_dwCenter;
						pIndex[*pTrinCnt]._3 = m_dwCorner[CORNER_LB];
						++(*pTrinCnt);

						pIndex[*pTrinCnt]._1 = m_dwCenter;
						pIndex[*pTrinCnt]._2 = m_dwCorner[CORNER_RB];
						pIndex[*pTrinCnt]._3 = m_dwCorner[CORNER_LB];
						++(*pTrinCnt);
					}
				}

				else
				{
					if (false == bIsNeighbor[NEIGHBOR_BOTTOM])
					{
						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_LB];
						pIndex[*pTrinCnt]._2 = m_dwCenter;
						pIndex[*pTrinCnt]._3 = (m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 1;
						++(*pTrinCnt);

						pIndex[*pTrinCnt]._1 = m_dwCenter;
						pIndex[*pTrinCnt]._2 = m_dwCorner[CORNER_RB];
						pIndex[*pTrinCnt]._3 = (m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 1;
						++(*pTrinCnt);
					}
					else
					{
						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_LB];
						pIndex[*pTrinCnt]._2 = m_dwCenter;
						pIndex[*pTrinCnt]._3 = m_dwCorner[CORNER_RB];
						++(*pTrinCnt);
					}

					if (false == bIsNeighbor[NEIGHBOR_LEFT])
					{
						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_LT];
						pIndex[*pTrinCnt]._2 = m_dwCenter;
						pIndex[*pTrinCnt]._3 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_LB]) >> 1;
						++(*pTrinCnt);

						pIndex[*pTrinCnt]._1 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_LB]) >> 1;
						pIndex[*pTrinCnt]._2 = m_dwCenter;
						pIndex[*pTrinCnt]._3 = m_dwCorner[CORNER_LB];
						++(*pTrinCnt);
					}
					else
					{
						pIndex[*pTrinCnt]._1 = m_dwCorner[CORNER_LT];
						pIndex[*pTrinCnt]._2 = m_dwCenter;
						pIndex[*pTrinCnt]._3 = m_dwCorner[CORNER_LB];
						++(*pTrinCnt);
					}
				}
					
			}

			return;
		}

		for (_ulong i = 0; i < CHILD_END; ++i)
		{
			if (nullptr != m_pChild[i])
				m_pChild[i]->CullingForTerrain(pFrustum, pVertex, pIndex, pTrinCnt);
		}

	}
}

void CQuadTree::Make_ChildCorner(CHILD eType, 
										const _ulong& dwPLT, 
										const _ulong& dwPRT, 
										const _ulong& dwPLB, 
										const _ulong& dwPRB, 
										const _ulong& dwPCenter, 
										_ulong* pLT, 
										_ulong* pRT, 
										_ulong* pLB, 
										_ulong* pRB)
{
	_ulong	dwLT, dwRT, dwLB, dwRB;
	_ulong	dwLC, dwRC, dwTC, dwBC;

	dwLC = (dwPLT + dwPLB) >> 1;
	dwRC = (dwPRT + dwPRB) >> 1;
	dwTC = (dwPLT + dwPRT) >> 1;
	dwBC = (dwPLB + dwPRB) >> 1;

	switch (eType)
	{
	case CHILD_LT:
		dwLT = dwPLT;
		dwRT = dwTC;
		dwLB = dwLC;
		dwRB = dwPCenter;

		break;

	case CHILD_RT:
		dwLT = dwTC;
		dwRT = dwPRT;
		dwLB = dwPCenter;
		dwRB = dwRC;
		break;

	case CHILD_LB:
		dwLT = dwLC;
		dwRT = dwPCenter;
		dwLB = dwPLB;
		dwRB = dwBC;
		break;

	case CHILD_RB:
		dwLT = dwPCenter;
		dwRT = dwRC;
		dwLB = dwBC;
		dwRB = dwPRB;
		break;
	}

	*pLT = dwLT;
	*pRT = dwRT;
	*pLB = dwLB;
	*pRB = dwRB;
}

void CQuadTree::Make_ChildQuadTree(void)
{
	if (1 == m_dwCorner[CORNER_RT] - m_dwCorner[CORNER_LT])
		return;

	for (_ulong i = 0; i < CHILD_END; ++i)
	{
		_ulong dwLT, dwRT, dwLB, dwRB;

		Make_ChildCorner(CHILD(i),
			m_dwCorner[CORNER_LT],
			m_dwCorner[CORNER_RT],
			m_dwCorner[CORNER_LB],
			m_dwCorner[CORNER_RB],
			m_dwCenter, &dwLT, &dwRT, &dwLB, &dwRB);

		m_pChild[i] = Create(dwLT, dwRT, dwLB, dwRB);

		if (nullptr != m_pChild[i])
			m_pChild[i]->Make_ChildQuadTree();
	}
}

void CQuadTree::Ready_Child(const _ulong& dwLT, const _ulong& dwRT, const _ulong& dwLB, const _ulong& dwRB)
{
	m_dwCorner[CORNER_LT] = dwLT;
	m_dwCorner[CORNER_RT] = dwRT;
	m_dwCorner[CORNER_LB] = dwLB;
	m_dwCorner[CORNER_RB] = dwRB;

	m_dwCenter = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 2;
}

CQuadTree* CQuadTree::Create(const _ulong& dwCntX, const _ulong& dwCntZ)
{
	CQuadTree*	pInstance = new CQuadTree;

	if (FAILED(pInstance->Ready_QuadTree(dwCntX, dwCntZ)))
		Engine::Safe_Release(pInstance);

	if (FAILED(pInstance->Ready_Neighbor()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CQuadTree* CQuadTree::Create(const _ulong& dwLT, const _ulong& dwRT, const _ulong& dwLB, const _ulong& dwRB)
{
	CQuadTree*		pInstance = new CQuadTree;

	pInstance->Ready_Child(dwLT, dwRT, dwLB, dwRB);

	return pInstance;
}

void CQuadTree::Free(void)
{
	for (_ulong i = 0; i < CHILD_END; ++i)
		Safe_Release(m_pChild[i]);
}

_bool Engine::CQuadTree::LevelOfDetail(LPDIRECT3DDEVICE9 pGraphicDev, 
										const _vec3* pVertex)
{
	_matrix		matView;

	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3 vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));

	_float	fDistance = D3DXVec3Length(&(vCamPos - pVertex[m_dwCenter]));

	_float	fWidth = D3DXVec3Length(&(pVertex[m_dwCorner[CORNER_RT]] - pVertex[m_dwCorner[CORNER_LT]]));

	return fDistance * 0.1f > fWidth;
}

HRESULT Engine::CQuadTree::Ready_Neighbor(void)
{
	Make_Neighbor();

	return S_OK;
}

void Engine::CQuadTree::Make_Neighbor(void)
{
	if (nullptr == m_pChild[CHILD_LT])
		return;

	m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_RIGHT]		= m_pChild[CHILD_RT];
	m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_BOTTOM]	= m_pChild[CHILD_LB];

	m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_LEFT]		= m_pChild[CHILD_LT];
	m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_BOTTOM]	= m_pChild[CHILD_RB];

	m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_RIGHT]		= m_pChild[CHILD_RB];
	m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_TOP]		= m_pChild[CHILD_LT];

	m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_LEFT]		= m_pChild[CHILD_LB];
	m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_TOP]		= m_pChild[CHILD_RT];

	if (nullptr != m_pNeighbor[NEIGHBOR_LEFT])
	{
		m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RT];
		m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
	{
		m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_LB];
		m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_RB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
	{
		m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LT];
		m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
	{
		m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_LT];
		m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_RT];
	}

	for (_ulong i = 0; i < CHILD_END; ++i)
		m_pChild[i]->Make_Neighbor();

}

//void Engine::CQuadTree::CullingForTerrain(Engine::CFrustum* pFrustum,
//	const _vec3* pVerticesPos,
//	INDEX32* pIndex,
//	_ulong* pTriCnt)
//{
//	m_fRadius = D3DXVec3Length(&(pVerticesPos[m_dwCorner[CORNER_LT]] - pVerticesPos[m_dwCenter]));
//
//	_bool bIsIn = pFrustum->Is_InFrustum(&pVerticesPos[m_dwCenter], m_fRadius);
//
//
//	if (true == bIsIn)
//	{
//		//if(nullptr == m_pChild[0])
//		if (true == LevelOfDetail(pFrustum->Get_GraphicDev(), pVerticesPos))
//		{
//			_bool		bNeighborDraw[NEIGHBOR_END] = { true, true, true, true };
//
//			for (_ulong i = 0; i < NEIGHBOR_END; ++i)
//			{
//				if (nullptr != m_pNeighbor[i])
//					bNeighborDraw[i] = m_pNeighbor[i]->LevelOfDetail(pFrustum->Get_GraphicDev(), pVerticesPos);
//			}
//
//
//			_bool bCorner[3] = { false };
//
//			bCorner[0] = pFrustum->Is_InFrustum(&pVerticesPos[m_dwCorner[CORNER_LT]]);
//			bCorner[1] = pFrustum->Is_InFrustum(&pVerticesPos[m_dwCorner[CORNER_RT]]);
//			bCorner[2] = pFrustum->Is_InFrustum(&pVerticesPos[m_dwCorner[CORNER_RB]]);
//
//			if (true == bCorner[0] || true == bCorner[1] || true == bCorner[2])
//			{
//				if (bNeighborDraw[NEIGHBOR_TOP] && bNeighborDraw[NEIGHBOR_RIGHT])
//				{
//					if (nullptr == m_pChild[CHILD_LT])
//					{
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LT];
//						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RT];
//						pIndex[*pTriCnt]._3 = m_dwCorner[CORNER_RB];
//						++(*pTriCnt);
//					}
//					else
//					{
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LT];
//						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RT];
//						pIndex[*pTriCnt]._3 = m_dwCenter;
//						++(*pTriCnt);
//
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
//						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RB];
//						pIndex[*pTriCnt]._3 = m_dwCenter;
//						++(*pTriCnt);
//					}
//				}
//
//				else
//				{
//					if (false == bNeighborDraw[NEIGHBOR_TOP])
//					{
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LT];
//						pIndex[*pTriCnt]._2 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT]) >> 1;
//						pIndex[*pTriCnt]._3 = m_dwCenter;
//						++(*pTriCnt);
//
//						pIndex[*pTriCnt]._1 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT]) >> 1;
//						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RT];
//						pIndex[*pTriCnt]._3 = m_dwCenter;
//						++(*pTriCnt);
//					}
//					else
//					{
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LT];
//						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RT];
//						pIndex[*pTriCnt]._3 = m_dwCenter;
//						++(*pTriCnt);
//					}
//
//					if (false == bNeighborDraw[NEIGHBOR_RIGHT])
//					{
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
//						pIndex[*pTriCnt]._2 = (m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_RB]) >> 1;
//						pIndex[*pTriCnt]._3 = m_dwCenter;
//						++(*pTriCnt);
//
//						pIndex[*pTriCnt]._1 = m_dwCenter;
//						pIndex[*pTriCnt]._2 = (m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_RB]) >> 1;
//						pIndex[*pTriCnt]._3 = m_dwCorner[CORNER_RB];
//						++(*pTriCnt);
//					}
//					else
//					{
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
//						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RB];
//						pIndex[*pTriCnt]._3 = m_dwCenter;
//						++(*pTriCnt);
//					}
//				}
//
//			}
//
//			// 왼쪽 하단 폴리곤
//			bCorner[0] = pFrustum->Is_InFrustum(&pVerticesPos[m_dwCorner[CORNER_LT]]);
//			bCorner[1] = pFrustum->Is_InFrustum(&pVerticesPos[m_dwCorner[CORNER_RB]]);
//			bCorner[2] = pFrustum->Is_InFrustum(&pVerticesPos[m_dwCorner[CORNER_LB]]);
//
//			if (true == bCorner[0] || true == bCorner[1] || true == bCorner[2])
//			{
//
//				if (bNeighborDraw[NEIGHBOR_BOTTOM] && bNeighborDraw[NEIGHBOR_LEFT])
//				{
//					if (nullptr == m_pChild[CHILD_LT])
//					{
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LT];
//						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RB];
//						pIndex[*pTriCnt]._3 = m_dwCorner[CORNER_LB];
//						++(*pTriCnt);
//					}
//					else
//					{
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LT];
//						pIndex[*pTriCnt]._2 = m_dwCenter;
//						pIndex[*pTriCnt]._3 = m_dwCorner[CORNER_LB];
//						++(*pTriCnt);
//
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LB];
//						pIndex[*pTriCnt]._2 = m_dwCenter;
//						pIndex[*pTriCnt]._3 = m_dwCorner[CORNER_RB];
//						++(*pTriCnt);
//					}
//
//				}
//
//				else
//				{
//					if (false == bNeighborDraw[NEIGHBOR_BOTTOM])
//					{
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LB];
//						pIndex[*pTriCnt]._2 = m_dwCenter;
//						pIndex[*pTriCnt]._3 = (m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 1;
//						++(*pTriCnt);
//
//						pIndex[*pTriCnt]._1 = (m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 1;
//						pIndex[*pTriCnt]._2 = m_dwCenter;
//						pIndex[*pTriCnt]._3 = m_dwCorner[CORNER_RB];
//						++(*pTriCnt);
//					}
//					else
//					{
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LB];
//						pIndex[*pTriCnt]._2 = m_dwCenter;
//						pIndex[*pTriCnt]._3 = m_dwCorner[CORNER_RB];
//						++(*pTriCnt);
//					}
//
//					if (false == bNeighborDraw[NEIGHBOR_LEFT])
//					{
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LT];
//						pIndex[*pTriCnt]._2 = m_dwCenter;
//						pIndex[*pTriCnt]._3 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_LB]) >> 1;
//						++(*pTriCnt);
//
//						pIndex[*pTriCnt]._1 = m_dwCenter;
//						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LB];
//						pIndex[*pTriCnt]._3 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_LB]) >> 1;
//						++(*pTriCnt);
//					}
//					else
//					{
//						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LT];
//						pIndex[*pTriCnt]._2 = m_dwCenter;
//						pIndex[*pTriCnt]._3 = m_dwCorner[CORNER_LB];
//						++(*pTriCnt);
//					}
//				}
//
//			}
//
//			return;
//		}
//
//		for (_ulong i = 0; i < CHILD_END; ++i)
//		{
//			if (nullptr != m_pChild[i])
//				m_pChild[i]->CullingForTerrain(pFrustum, pVerticesPos, pIndex, pTriCnt);
//		}
//	}
//
//
//}