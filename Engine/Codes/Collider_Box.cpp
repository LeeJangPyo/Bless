#include "Collider_Box.h"


#include "Transform.h"
#include "Texture.h"

Engine::CCollider_Box::CCollider_Box(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
{
	
}

CCollider_Box::CCollider_Box(const CCollider_Box & rhs)
	:CComponent(rhs)

{
	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VTXCUBE) * 8, 0,
		VTXFVF_CUBE, D3DPOOL_MANAGED, &m_pVB, NULL)))
	{
		_TAGMSG_BOX(L"VB Creation Failed, in Ready_Collider_Box", L"Collider_Box");
	}
	if (FAILED(m_pDevice->CreateIndexBuffer(sizeof(INDEX32) * 12,
		0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, NULL)))
	{
		_TAGMSG_BOX(L"IB Creation Failed, in Ready_Collider_Box", L"Collider_Box");
	}


	m_pTransformCom = CTransform::Create(m_pDevice);
	m_pTextureCom = CTexture::Create(m_pDevice);
}

const _matrix * CCollider_Box::Get_WorldMatrix(void) const
{
	return m_pTransformCom->Get_WorldMatrix();
}

HRESULT Engine::CCollider_Box::Ready_Collider_Box()//const _vec3* pPosition, const _ulong& dwNumVertices, const _ulong& dwStride)
{

	if(FAILED(m_pDevice->CreateVertexBuffer(sizeof(VTXCUBE) * 8, 0,
		VTXFVF_CUBE, D3DPOOL_MANAGED, &m_pVB, NULL)))
	{
		_TAGMSG_BOX(L"VB Creation Failed, in Ready_Collider_Box", L"Collider_Box");
		return E_FAIL;
	}
	if(FAILED(m_pDevice->CreateIndexBuffer(sizeof(INDEX32) * 12,
		0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, NULL)))
	{
		_TAGMSG_BOX(L"IB Creation Failed, in Ready_Collider_Box", L"Collider_Box");
		return E_FAIL;
	}

	m_pTextureCom = CTexture::Create(m_pDevice);
	m_pTransformCom = CTransform::Create(m_pDevice);

	return S_OK;
}

HRESULT CCollider_Box::SetUp_Collider_Box(const _vec3 * pPosition, const _ulong & dwNumVertices, const _ulong & dwStride)
{
	if (FAILED(D3DXComputeBoundingBox(pPosition, dwNumVertices, sizeof(_vec3), &m_vMin, &m_vMax)))
	{
		_TAGMSG_BOX(L"Compute Bouding Box is Failed, in Ready_Collider_Box", L"CCollider_Box");
		return E_FAIL;
	}


	VTXCUBE*		pVtxCube = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	pVtxCube[0].vPos = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVtxCube[0].vTex = pVtxCube[0].vPos;

	pVtxCube[1].vPos = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVtxCube[1].vTex = pVtxCube[1].vPos;

	pVtxCube[2].vPos = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVtxCube[2].vTex = pVtxCube[2].vPos;

	pVtxCube[3].vPos = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVtxCube[3].vTex = pVtxCube[3].vPos;


	pVtxCube[4].vPos = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVtxCube[4].vTex = pVtxCube[4].vPos;

	pVtxCube[5].vPos = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVtxCube[5].vTex = pVtxCube[5].vPos;

	pVtxCube[6].vPos = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVtxCube[6].vTex = pVtxCube[6].vPos;

	pVtxCube[7].vPos = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMax.z);
	pVtxCube[7].vTex = pVtxCube[7].vPos;


	m_pVB->Unlock();

	INDEX32*	pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// -x
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;


	// +y
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// -y	 
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;
	pIndex[6]._3 = 6;

	pIndex[7]._1 = 3;
	pIndex[7]._2 = 6;
	pIndex[7]._3 = 7;

	// +z
	pIndex[8]._1 = 7;
	pIndex[8]._2 = 6;
	pIndex[8]._3 = 5;

	pIndex[9]._1 = 7;
	pIndex[9]._2 = 5;
	pIndex[9]._3 = 4;

	// -z
	pIndex[10]._1 = 0;
	pIndex[10]._2 = 1;
	pIndex[10]._3 = 2;

	pIndex[11]._1 = 0;
	pIndex[11]._2 = 2;
	pIndex[11]._3 = 3;

	m_pIB->Unlock();

	
	return S_OK;
}

HRESULT CCollider_Box::SetUp_Collider_Box()
{

	m_vMin = _vec3(-0.5f, -0.5f, -0.5f);
	m_vMax = _vec3(0.5f, 0.5f, 0.5f);

	VTXCUBE*		pVtxCube = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	// 전면부

	pVtxCube[0].vPos = _vec3(-0.5f, 0.5f, -0.5f);
	pVtxCube[0].vTex = pVtxCube[0].vPos;

	pVtxCube[1].vPos = _vec3(0.5f, 0.5f, -0.5f);
	pVtxCube[1].vTex = pVtxCube[1].vPos;

	pVtxCube[2].vPos = _vec3(0.5f, -0.5f, -0.5f);
	pVtxCube[2].vTex = pVtxCube[2].vPos;

	pVtxCube[3].vPos = _vec3(-0.5f, -0.5f, -0.5f);
	pVtxCube[3].vTex = pVtxCube[3].vPos;

	// 후면부

	pVtxCube[4].vPos = _vec3(-0.5f, 0.5f, 0.5f);
	pVtxCube[4].vTex = pVtxCube[4].vPos;

	pVtxCube[5].vPos = _vec3(0.5f, 0.5f, 0.5f);
	pVtxCube[5].vTex = pVtxCube[5].vPos;

	pVtxCube[6].vPos = _vec3(0.5f, -0.5f, 0.5f);
	pVtxCube[6].vTex = pVtxCube[6].vPos;

	pVtxCube[7].vPos = _vec3(-0.5f, -0.5f, 0.5f);
	pVtxCube[7].vTex = pVtxCube[7].vPos;

	m_pVB->Unlock();

	INDEX32*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// x+
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;


	// x-
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// y+
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;


	// y-
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;
	pIndex[6]._3 = 6;

	pIndex[7]._1 = 3;
	pIndex[7]._2 = 6;
	pIndex[7]._3 = 7;

	// z+
	pIndex[8]._1 = 7;
	pIndex[8]._2 = 6;
	pIndex[8]._3 = 5;

	pIndex[9]._1 = 7;
	pIndex[9]._2 = 5;
	pIndex[9]._3 = 4;


	// z-
	pIndex[10]._1 = 0;
	pIndex[10]._2 = 1;
	pIndex[10]._3 = 2;

	pIndex[11]._1 = 0;
	pIndex[11]._2 = 2;
	pIndex[11]._3 = 3;

	m_pIB->Unlock();


	return S_OK;
}

void Engine::CCollider_Box::Render_Collider_Box()
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBE));
	m_pDevice->SetFVF(VTXFVF_CUBE);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

}

HRESULT CCollider_Box::SetUp_ConstantTable(LPD3DXEFFECT pEffect, COLORTYPE eType, const _matrix* pmatWorld)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", pmatWorld);

	_matrix matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", eType);
	

	Safe_Release(pEffect);

	return S_OK;
}

Engine::CCollider_Box* Engine::CCollider_Box::Create(LPDIRECT3DDEVICE9 pDevice)//, const _vec3* pPosition, const _ulong& dwNumVertices, const _ulong& dwStride)
{
	CCollider_Box*	pInstance = new CCollider_Box(pDevice);

	if (FAILED(pInstance->Ready_Collider_Box()))//pPosition, dwNumVertices, dwStride)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CComponent * CCollider_Box::Clone_Component()
{
	return new CCollider_Box(*this);
}

void Engine::CCollider_Box::Free(void)
{

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CComponent::Free();

}

