#include "..\Headers\Calculator.h"

#include "Collider_Sphere.h"
#include "Collider_Box.h"
#include "Buffer_TerrainTex.h"
#include "Transform.h"




CCalculator::CCalculator(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
{
}

CCalculator::CCalculator(const CCalculator & rhs)
	:CComponent(rhs)
{
}

HRESULT CCalculator::Ready_Calculator(void)
{
	return S_OK;
}

_float CCalculator::Compute_HeightOnTerrain(const _vec3 * pPosition, const _vec3 * pTerrainVtxPos, const _ulong & dwCntX, const _ulong & dwCntZ, const _float & fItv)
{
	_ulong	dwIndex = _ulong(pPosition->z / fItv) * _ulong(dwCntX) + _ulong(pPosition->x / fItv);


	_float fRatioX = (pPosition->x - pTerrainVtxPos[dwIndex + dwCntX].x) / fItv;
	_float fRatioZ = (pTerrainVtxPos[dwIndex + dwCntX].z - pPosition->z) / fItv;


	_float	fHeight[4] = {
		pTerrainVtxPos[dwIndex + dwCntX].y,
		pTerrainVtxPos[dwIndex + dwCntX + 1].y,
		pTerrainVtxPos[dwIndex + 1].y,
		pTerrainVtxPos[dwIndex].y
	};

	// 오른쪽 위
	if (fRatioX > fRatioZ)
	{
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}
	// 왼쪽 아래
	else
	{
		return fHeight[0] + (fHeight[3] - fHeight[0]) * fRatioZ + (fHeight[2] - fHeight[3]) * fRatioX;
	}
}

_vec3 CCalculator::Picking_OnTerrain(HWND hWnd, const CBuffer_TerrainTex * pTerrainBufferCom, const CTransform * pTransform)
{
	RAY tRay = Transform_ScreenPosToRayLocal(hWnd, pTransform->Get_WorldMatrix());

	_ulong dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();
	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();


	_ulong dwVtxIdx[3];
	_float fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwVtxCntX + j;

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			// 오른쪽 위
			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[0]],
				&tRay.vRayPos, &tRay.vRayDir, &fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);
			}

			// 왼쪽 아래

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&tRay.vRayPos, &tRay.vRayDir, &fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
			}
		}
	}

	return _vec3();
}

_vec3 CCalculator::Picking_OnTerrain(const RAY & Ray, const CBuffer_TerrainTex * pTerrainBufferCom, const CTransform * pTransform)
{
	//RAY tRay = Transform_ScreenPosToRayLocal(hWnd, pTransform->Get_WorldMatrix());
	RAY tRay = Transform_WorldToLocal(Ray, pTransform->Get_WorldMatrix());

	_ulong dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();
	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();


	_ulong dwVtxIdx[3];
	_float fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwVtxCntX + j;

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			// 오른쪽 위
			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[0]],
				&tRay.vRayPos, &tRay.vRayDir, &fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);
			}

			// 왼쪽 아래

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&tRay.vRayPos, &tRay.vRayDir, &fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
			}
		}
	}

	return _vec3();
}


_bool CCalculator::Check_CollisionAABB(const _vec3 * pDestMin, const _vec3 * pDestMax, const _matrix * pDestWorld, const _vec3 * pSrcMin, const _vec3 * pSrcMax, const _matrix * pSrcWorld)
{
	_vec3	vDestMin, vDestMax, vSrcMin, vSrcMax;
	_float	fMin, fMax;

	D3DXVec3TransformCoord(&vDestMin, pDestMin, pDestWorld);
	D3DXVec3TransformCoord(&vDestMax, pDestMax, pDestWorld);

	D3DXVec3TransformCoord(&vSrcMin, pSrcMin, pSrcWorld);
	D3DXVec3TransformCoord(&vSrcMax, pSrcMax, pSrcWorld);

	//X축 비교
	fMin = max(vDestMin.x, vSrcMin.x);
	fMax = min(vDestMax.x, vSrcMax.x);

	if (fMax < fMin)
		return false;

	//Y축 비교
	fMin = max(vDestMin.y, vSrcMin.y);
	fMax = min(vDestMax.y, vSrcMax.y);

	if (fMax < fMin)
		return false;

	//Z축 비교
	fMin = max(vDestMin.z, vSrcMin.z);
	fMax = min(vDestMax.z, vSrcMax.z);

	if (fMax < fMin)
		return false;

	return true;
}

_bool CCalculator::Picking_Sphere(const RAY & rRay, const CCollider_Sphere * pColliderCom)
{
	_vec3 v = rRay.vRayPos - *pColliderCom->Get_Center();

	_float b = 2.0f * D3DXVec3Dot(&rRay.vRayDir, &v);
	_float c = D3DXVec3Dot(&v, &v) - (*pColliderCom->Get_Radius() * *pColliderCom->Get_Radius());


	// find the discriminant
	float discriminant = (b * b) - (4.0f * c);

	// test for imaginary number
	if (discriminant < 0.0f)
		return false;

	discriminant = sqrtf(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	// if a solution is >= 0, then we intersected the sphere
	if (s0 >= 0.0f || s1 >= 0.0f)
		return true;

	return false;
}

_bool CCalculator::Picking_Triangle(RAY & tRay, const _vec3 * pPoint)
{
	_float fU, fV, fDist;

	if (D3DXIntersectTri(&pPoint[0],
		&pPoint[1],
		&pPoint[2],
		&tRay.vRayPos, &tRay.vRayDir, &fU, &fV, &fDist))
	{
		return true;
	}

	return false;
}

//WorldSpace Picking
_bool CCalculator::Picking_AABB(const _vec3 * pMin, const _vec3 * pMax, RAY & tRay, _vec3 * pIntersectPoint)
{
	_float tStart = 0.f;
	_float tEnd = 99999.f;


	//x aixs
	if (tRay.vRayDir.x == 0.f)
	{
		if (tRay.vRayPos.x < pMin->x || tRay.vRayPos.x > pMax->x)
			return false;
	}
	else
	{
		_float tStartX = (pMin->x - tRay.vRayPos.x) / tRay.vRayDir.x;
		_float tEndX = (pMax->x - tRay.vRayPos.x) / tRay.vRayDir.x;

		if (tStartX > tEndX)
		{
			_float temp;
			temp = tStartX;
			tStartX = tEndX;
			tEndX = temp;
		}

		if (tStart < tStartX)
			tStart = tStartX;

		if (tEnd > tEndX)
			tEnd = tEndX;

		if (tStart > tEnd)
			return false;
	}

	//y aixs
	if (tRay.vRayDir.y == 0.f)
	{
		if (tRay.vRayPos.y < pMin->y || tRay.vRayPos.y > pMax->y)
			return false;
	}
	else
	{
		_float tStartY = (pMin->y - tRay.vRayPos.y) / tRay.vRayDir.y;
		_float tEndY = (pMax->y - tRay.vRayPos.y) / tRay.vRayDir.y;

		if (tStartY > tEndY)
		{
			_float temp;
			temp = tStartY;
			tStartY = tEndY;
			tEndY = temp;
		}

		if (tStart < tStartY)
			tStart = tStartY;

		if (tEnd > tEndY)
			tEnd = tEndY;

		if (tStart > tEnd)
			return false;
	}


	//Z aixs
	if (tRay.vRayDir.z == 0.f)
	{
		if (tRay.vRayPos.z < pMin->z || tRay.vRayPos.z > pMax->z)
			return false;
	}
	else
	{
		_float tStartZ = (pMin->z - tRay.vRayPos.z) / tRay.vRayDir.z;
		_float tEndZ = (pMax->z - tRay.vRayPos.z) / tRay.vRayDir.z;

		if (tStartZ > tEndZ)
		{
			_float temp;
			temp = tStartZ;
			tStartZ = tEndZ;
			tEndZ = temp;
		}

		if (tStart < tStartZ)
			tStart = tStartZ;

		if (tEnd > tEndZ)
			tEnd = tEndZ;

		if (tStart > tEnd)
			return false;
	}

	return true;
}

RAY CCalculator::Transform_WorldToLocal(const RAY & rRay, const _matrix * pmatWorld)
{
	RAY tRay;

	_matrix matInvWorld;
	D3DXMatrixInverse(&matInvWorld, NULL, pmatWorld);

	D3DXVec3TransformCoord(&tRay.vRayPos, &rRay.vRayPos, &matInvWorld);
	D3DXVec3TransformNormal(&tRay.vRayDir, &rRay.vRayDir, &matInvWorld);

	D3DXVec3Normalize(&tRay.vRayDir, &tRay.vRayDir);

	return tRay;
}

RAY CCalculator::Transform_ScreenPosToRay(HWND hWnd)
{
	RAY	tRay;

	POINT	ptMouse = {};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vMousePos;

	D3DVIEWPORT9	ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pDevice->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix		matProj, matView;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	tRay.vRayPos = _vec3(0.f, 0.f, 0.f);
	tRay.vRayDir = vMousePos - tRay.vRayPos;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	D3DXVec3TransformCoord(&tRay.vRayPos, &tRay.vRayPos, &matView);
	D3DXVec3TransformNormal(&tRay.vRayDir, &tRay.vRayDir, &matView);

	D3DXVec3Normalize(&tRay.vRayDir, &tRay.vRayDir);

	return tRay;
}

RAY CCalculator::Transform_ScreenPosToRayLocal(HWND hWnd, const _matrix * pWorld)
{
	RAY tRay = Transform_ScreenPosToRay(hWnd);

	_matrix matWorld = *pWorld;
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&tRay.vRayPos, &tRay.vRayPos, &matWorld);
	D3DXVec3TransformNormal(&tRay.vRayDir, &tRay.vRayDir, &matWorld);

	D3DXVec3Normalize(&tRay.vRayDir, &tRay.vRayDir);


	return tRay;
}

RAY CCalculator::Transform_ScreenPosToRay(const _vec2 * pScreenPos)
{
	RAY	tRay;

	_vec3		vMousePos;

	D3DVIEWPORT9	ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pDevice->GetViewport(&ViewPort);

	vMousePos.x = pScreenPos->x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = pScreenPos->y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix		matProj, matView;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	tRay.vRayPos = _vec3(0.f, 0.f, 0.f);
	tRay.vRayDir = vMousePos - tRay.vRayPos;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	D3DXVec3TransformCoord(&tRay.vRayPos, &tRay.vRayPos, &matView);
	D3DXVec3TransformNormal(&tRay.vRayDir, &tRay.vRayDir, &matView);

	D3DXVec3Normalize(&tRay.vRayDir, &tRay.vRayDir);

	return tRay;
}

RAY CCalculator::Transform_ScreenPosToRayLocal(const _vec2 * pScreenPos, const _matrix * pWorld)
{
	RAY tRay = Transform_ScreenPosToRay(pScreenPos);

	_matrix matWorld = *pWorld;
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&tRay.vRayPos, &tRay.vRayPos, &matWorld);
	D3DXVec3TransformNormal(&tRay.vRayDir, &tRay.vRayDir, &matWorld);

	D3DXVec3Normalize(&tRay.vRayDir, &tRay.vRayDir);


	return tRay;
}
_vec3 CCalculator::SpringDamp(const _vec3 * pCurrentPos,
								const _vec3 * pTargetPos,
								const _vec3 * pPrevTargetPos, 
								const _float & fTimeDelta, 
								const _float & fSpringConst, 
								const _float & fDmapConst,
								const _float & fSpringLen)
{
	_vec3 vDisplacement;           // Displacement
	_vec3 vVelocity;				// Velocity   
	_float fForceMagnitude;         // Force Magnitude

	vDisplacement = *pCurrentPos - *pTargetPos;
	vVelocity = (*pPrevTargetPos - *pTargetPos) * fTimeDelta;
	fForceMagnitude = fSpringConst * (fSpringLen - D3DXVec3Length(&vDisplacement))
		+ fDmapConst * (D3DXVec3Dot(&vDisplacement, &vVelocity) / D3DXVec3Length(&vDisplacement));

	D3DXVec3Normalize(&vDisplacement, &vDisplacement);
	vDisplacement *= fForceMagnitude * fTimeDelta;

	_vec3 ResultPos = *pCurrentPos + vDisplacement;
	return ResultPos;

}
Engine::CComponent* CCalculator::Clone_Component()
{
	this->AddRef();
	return this;
}

CCalculator * CCalculator::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCalculator*	pInstance = new CCalculator(pDevice);

	if (FAILED(pInstance->Ready_Calculator()))
	{
		_MSG_BOX(L"Calculator COmponent Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CCalculator::Free()
{
	CComponent::Free();
}