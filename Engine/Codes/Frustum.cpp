#include "Frustum.h"


CFrustum::CFrustum(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

HRESULT Engine::CFrustum::Ready_Frustum(void)
{
	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);
	
	return S_OK;
}

_bool CFrustum::IsIn_Frustum(const _vec3* pPosition)
{
	_float	fDistance = 0.f;

	for (_int i = 0; i < 6; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPosition);

		if (fDistance > 0.f)
			return false;
	}

	return true;
}

_bool CFrustum::IsIn_Frustum(const _vec3* pPosition, const _float& fRadius)
{
	_float	fDistance = 0.f;

	for (_int i = 0; i < 6; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPosition);

		if (fDistance > fRadius)
			return false;
	}

	return true;
}

_bool CFrustum::Is_InFrustum_ForObject(const _vec3* pPosInWorld, 
												const _float fRadius)
{
	Ready_Frustum();

	_matrix			matProj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	for (_int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);

	_matrix			matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);

	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// z+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	return IsIn_Frustum(pPosInWorld, fRadius);
}

void CFrustum::Is_InFrustum_ForTerrain(const _vec3* pVertexPos, 
	const _ulong& dwCntX, const _ulong& dwCntZ, 
	Engine::INDEX32* pIndex, _ulong* pTriCnt, 
	CQuadTree* pQuadTree)
{
	Ready_Frustum();
	ZeroMemory(pIndex, sizeof(Engine::INDEX32) * (dwCntX - 1) * (dwCntZ - 1) * 2);


	_matrix			matProj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	for (_int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);

	_matrix			matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);

	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// z+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	*pTriCnt = 0;
	
	if (nullptr != pQuadTree)
		pQuadTree->CullingForTerrain(this, pVertexPos, pIndex, pTriCnt);

	else
	{
		_bool	bIsin[3]{ false };
		_ulong	dwTriCnt = 0;

		for (_ulong i = 0; i < dwCntZ - 1; ++i)
		{
			for (_ulong j = 0; j < dwCntX - 1; ++j)
			{
				_ulong dwIndex = i * dwCntX + j;

				// 오른쪽 위
				bIsin[0] = IsIn_Frustum(&pVertexPos[dwIndex + dwCntX]);
				bIsin[1] = IsIn_Frustum(&pVertexPos[dwIndex + dwCntX + 1]);
				bIsin[2] = IsIn_Frustum(&pVertexPos[dwIndex + 1]);

				if (true == bIsin[0] || true == bIsin[1] || true == bIsin[2])
				{
					pIndex[dwTriCnt]._1 = dwIndex + dwCntX;
					pIndex[dwTriCnt]._2 = dwIndex + dwCntX + 1;
					pIndex[dwTriCnt]._3 = dwIndex + 1;
					++dwTriCnt;
				}

				// 왼쪽 아래
				bIsin[0] = IsIn_Frustum(&pVertexPos[dwIndex + dwCntX]);
				bIsin[1] = IsIn_Frustum(&pVertexPos[dwIndex + 1]);
				bIsin[2] = IsIn_Frustum(&pVertexPos[dwIndex]);

				if (true == bIsin[0] || true == bIsin[1] || true == bIsin[2])
				{
					pIndex[dwTriCnt]._1 = dwIndex + dwCntX;
					pIndex[dwTriCnt]._2 = dwIndex + 1;
					pIndex[dwTriCnt]._3 = dwIndex;
					++dwTriCnt;
				}
			}
		}

		*pTriCnt = dwTriCnt;
	}

}

Engine::CFrustum* Engine::CFrustum::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFrustum*		pInstance = new CFrustum(pDevice);

	if (FAILED(pInstance->Ready_Frustum()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CFrustum::Free(void)
{
	Safe_Release(m_pDevice);
}

