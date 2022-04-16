#include "Buffer_TerrainTex.h"


Engine::CBuffer_TerrainTex::CBuffer_TerrainTex(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
	, m_pPosition(nullptr)
	, m_bIsClone(false)
	, m_fDetail(0.f)
{

}

Engine::CBuffer_TerrainTex::CBuffer_TerrainTex(const CBuffer_TerrainTex& rhs)
	: CVIBuffer(rhs)
	, m_pPosition(rhs.m_pPosition)
	//, m_iH(rhs.m_iH)
	//, m_fH(rhs.m_fH)
	, m_bIsClone(true)
	,m_dwCntX(rhs.m_dwCntX)
	,m_dwCntZ(rhs.m_dwCntZ)
	, m_fDetail(rhs.m_fDetail)

{

}


HRESULT Engine::CBuffer_TerrainTex::Ready_Buffer(const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv,
	const _float& fDetail,
	const _float& fHeight)
{
	m_dwCntX = dwCntX;
	m_dwCntZ = dwCntZ;

	m_fDetail = fDetail;

	m_dwVtxSize = sizeof(VTXTANGENT);
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxFVF = VTXFVF_TANGENT;
	m_pPosition = new _vec3[m_dwVtxCnt];

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
	{
		_TAGMSG_BOX(L"VIBuffer Ready Failed, in ReadyBuffer", L"CBuffer_TerrainTex");
	}

	VTXTANGENT*		pVtxTangent = NULL;

	//Use HeightMap
	//_ulong*		pPixel = LoadImage();

	m_pVB->Lock(0, 0, (void**)&pVtxTangent, 0);

	_ulong	dwIndex = 0;

	for (_ulong z = 0; z < dwCntZ; ++z)
	{
		for (_ulong x = 0; x < dwCntX; ++x)
		{
			dwIndex = z * dwCntX + x;

			pVtxTangent[dwIndex].vPos = _vec3(_float(x) * dwVtxItv,
				//Use HeightMap
				//(pPixel[dwIndex] & 0x000000ff) / fHeight,
				 0.f,
				_float(z) * dwVtxItv);

			m_pPosition[dwIndex] = pVtxTangent[dwIndex].vPos;
			
			//노말
			pVtxTangent[dwIndex].vNormal = _vec3(0.f, 0.f, 0.f);

			pVtxTangent[dwIndex].vTex = _vec2(_float(x) / (dwCntX - 1) * fDetail, _float(z) / (dwCntZ - 1) * fDetail);
		}
	}

	
	//Use HeightMap
	//Engine::Safe_Delete_Array(pPixel);

	INDEX32*		pIndex = NULL;
	_int			iTriCnt = 0;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong z = 0; z < dwCntZ - 1; ++z)
	{
		for (_ulong x = 0; x < dwCntX - 1; ++x)
		{
			dwIndex = z * dwCntX + x;

			pIndex[iTriCnt]._1 = dwIndex + dwCntX;
			pIndex[iTriCnt]._2 = dwIndex + dwCntX + 1;
			pIndex[iTriCnt]._3 = dwIndex + 1;

			//오른쪽위
			_vec3	vDest, vSour, vNormal;

			vDest = pVtxTangent[pIndex[iTriCnt]._2].vPos - pVtxTangent[pIndex[iTriCnt]._1].vPos;
			vSour = pVtxTangent[pIndex[iTriCnt]._3].vPos - pVtxTangent[pIndex[iTriCnt]._2].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTangent[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTangent[pIndex[iTriCnt]._2].vNormal += vNormal;
			pVtxTangent[pIndex[iTriCnt]._3].vNormal += vNormal;

			++iTriCnt;

			// 왼쪽 아래
			pIndex[iTriCnt]._1 = dwIndex + dwCntX;
			pIndex[iTriCnt]._2 = dwIndex + 1;
			pIndex[iTriCnt]._3 = dwIndex;

			vDest = pVtxTangent[pIndex[iTriCnt]._2].vPos - pVtxTangent[pIndex[iTriCnt]._1].vPos;
			vSour = pVtxTangent[pIndex[iTriCnt]._3].vPos - pVtxTangent[pIndex[iTriCnt]._2].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTangent[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTangent[pIndex[iTriCnt]._2].vNormal += vNormal;
			pVtxTangent[pIndex[iTriCnt]._3].vNormal += vNormal;
			++iTriCnt;
		}
	}

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVtxTangent[i].vNormal, &pVtxTangent[i].vNormal);

	CalculateTangentArray(m_dwVtxCnt, pVtxTangent, m_dwTriCnt, pIndex);

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

void CBuffer_TerrainTex::CalculateTangentArray(_ulong dwVertexCount, VTXTANGENT* pVertexInfo, _ulong dwTriangleCount, const INDEX32 * pIndex)
{
	_vec3 *tan1 = new _vec3[dwVertexCount * 2];
	_vec3 *tan2 = tan1 + dwVertexCount;
	ZeroMemory(tan1, dwVertexCount * sizeof(_vec3) * 2);

	for (_ulong i = 0; i < dwTriangleCount; i++)
	{
		_ulong i1 = pIndex->_1;
		_ulong i2 = pIndex->_2;
		_ulong i3 = pIndex->_3;

		const _vec3& v1 = pVertexInfo[i1].vPos;
		const _vec3& v2 = pVertexInfo[i2].vPos;
		const _vec3& v3 = pVertexInfo[i3].vPos;

		const _vec2& w1 = pVertexInfo[i1].vTex;
		const _vec2& w2 = pVertexInfo[i2].vTex;
		const _vec2& w3 = pVertexInfo[i3].vTex;

		_float x1 = v2.x - v1.x;
		_float x2 = v3.x - v1.x;
		_float y1 = v2.y - v1.y;
		_float y2 = v3.y - v1.y;
		_float z1 = v2.z - v1.z;
		_float z2 = v3.z - v1.z;

		_float s1 = w2.x - w1.x;
		_float s2 = w3.x - w1.x;
		_float t1 = w2.y - w1.y;
		_float t2 = w3.y - w1.y;

		_float r = 1.0F / (s1 * t2 - s2 * t1);
		_vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		_vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;

		pIndex++;
	}

	for (_ulong i = 0; i < dwVertexCount; i++)
	{
		const _vec3& n = pVertexInfo[i].vNormal;
		const _vec3& t = tan1[i];

		// Gram-Schmidt orthogonalize
		_vec3 vNormalize;
		D3DXVec3Normalize(&vNormalize, &(t - n * D3DXVec3Dot(&n, &t)));
		pVertexInfo[i].vTangent.x = vNormalize.x;
		pVertexInfo[i].vTangent.y = vNormalize.y;
		pVertexInfo[i].vTangent.z = vNormalize.z;

		// Calculate handedness
		_vec3 vCross;
		D3DXVec3Cross(&vCross, &n, &t);
		pVertexInfo[i].vTangent.w = (D3DXVec3Dot(&vCross, &tan2[i]) < 0.0F) ? -1.0F : 1.0F;
	}

	delete[] tan1;
}

Engine::CBuffer_TerrainTex* Engine::CBuffer_TerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv,
	const _float& fDetail,
	const _float& fHeight)
{
	CBuffer_TerrainTex*		pInstance = new CBuffer_TerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv, fDetail, fHeight)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CBuffer_TerrainTex::Free(void)
{
	CVIBuffer::Free();

	if (false == m_bIsClone)
		Engine::Safe_Delete_Array(m_pPosition);
}

CComponent * CBuffer_TerrainTex::Clone_Component()
{
	return new CBuffer_TerrainTex(*this);
}



Engine::_ulong* Engine::CBuffer_TerrainTex::LoadImage(void)
{
	/*_ulong		dwByte = 0;

	m_hFile = CreateFile(L"../Bin/Resources/Texture/Terrain/Height.bmp",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	_ulong* pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biWidth * m_iH.biHeight, &dwByte, nullptr);

	CloseHandle(m_hFile);

	return pPixel;*/

	return nullptr;
}


void Engine::CBuffer_TerrainTex::Set_Index(INDEX32* pIndex, const _ulong& dwTriCnt)
{
	INDEX32*		pOriIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pOriIndex, 0);

	memcpy(pOriIndex, pIndex, sizeof(INDEX32) * dwTriCnt);

	m_pIB->Unlock();

	m_dwTriCnt = dwTriCnt;
}

HRESULT CBuffer_TerrainTex::SetUp_OnShader_Detail(LPD3DXEFFECT pEffect, const _char * pConstantTable)
{
	if (pEffect == nullptr)
		return E_FAIL;
	pEffect->AddRef();
	{
		pEffect->SetFloat(pConstantTable, m_fDetail);
	}
	Safe_Release(pEffect);

	return S_OK;
}

