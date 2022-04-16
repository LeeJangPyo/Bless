#include "Mesh_Static.h"

Engine::CMesh_Static::CMesh_Static(LPDIRECT3DDEVICE9 pDevice)
	:CMesh(pDevice)
{

}

Engine::CMesh_Static::CMesh_Static(const CMesh_Static& rhs)
	: CMesh(rhs),
	m_pMesh(rhs.m_pMesh),
	m_pAdjacency(rhs.m_pAdjacency),
	m_pSubsetBuffer(rhs.m_pSubsetBuffer),
	m_pSubsets(rhs.m_pSubsets),
	m_dwSubsetCnt(rhs.m_dwSubsetCnt),
	m_pOriMesh(rhs.m_pOriMesh),
	m_pVtxPos(rhs.m_pVtxPos),
	m_dwStride(rhs.m_dwStride),
	m_dwNumVertex(rhs.m_dwNumVertex)
{
	m_pTextures = new MESHTEXTURE[rhs.m_dwSubsetCnt];

	for(_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_pTextures[i].pDiffuse = rhs.m_pTextures[i].pDiffuse;
		m_pTextures[i].pNormal	= rhs.m_pTextures[i].pNormal;
		m_pTextures[i].pSpecular = rhs.m_pTextures[i].pSpecular;

		if (m_pTextures[i].pDiffuse != nullptr)
			m_pTextures[i].pDiffuse->AddRef();
		if (m_pTextures[i].pNormal != nullptr)
			m_pTextures[i].pNormal->AddRef();
		if (m_pTextures[i].pSpecular != nullptr)
			m_pTextures[i].pSpecular->AddRef();
	}
	
	m_pMesh->AddRef();
	m_pOriMesh->AddRef();
	m_pAdjacency->AddRef();
	m_pSubsetBuffer->AddRef();
}

Engine::CMesh_Static::~CMesh_Static(void)
{

}

HRESULT Engine::CMesh_Static::Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	//pFilePath = 0x0abe4b48 L"C:\\Users\\Administrator.JUSIN-20180617B\\Desktop\\MyWork\\D3D\\D3DFramework\\Tool\\Bin\\Resources\\Mesh\\DynamicMesh\\Archer\\Archer.X"
	if(FAILED(D3DXLoadMeshFromX(szFullPath,	//로드할 x파일
											D3DXMESH_MANAGED,	//메시를 생성하는 방법
											m_pDevice,			
											&m_pAdjacency,		//애니메이션 메쉬를 구동할 때 스키닝을 위한 인접 버퍼 정보를 저장해두는 포인터
											&m_pSubsetBuffer,	//서브셋에 대한 정보를 의미하는 포인터, 즉 메쉬의 재질 정보를 보관하는 포인터
											NULL,				//메쉬 자체가 지니고 있는 이펙트에 대한 정보, 우리가 추줄하는 메쉬에는 이 정보가 거의 없음 NULL
											&m_dwSubsetCnt,		//메테리얼 개수(서브셋개수)를 저장하기 위한 변수
											&m_pOriMesh)))			//로드된 메쉬 정보를 저장할 포인터
	{
		_TAGMSG_BOX(L"MeshObject Creation  Failed from File, in Ready_Mesh", L"CMesh_Static");
		return E_FAIL;
	}
											

	////노말 옵션이 없는 메쉬에 노말 값을 부여하기 위한 작업
	//_ulong	dwFVF = m_pOriMesh->GetFVF();

	//if( !(dwFVF & D3DFVF_NORMAL) )
	//{
	//	//기존의 로드한 메쉬의 fvf 정보를 복제해주는 함수
	//	m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pDevice, &m_pMesh);

	//	//정점의 노말을 계산해 주는 함수
	//	D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	//}
	//else
	//{
	//	m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pDevice, &m_pMesh);
	//}
	// D3DVERTEXELEMENT9 : 정점의 FVF정보 하나를 담기위한 구조체.
	D3DVERTEXELEMENT9			Element[MAX_FVF_DECL_SIZE];
	ZeroMemory(&Element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);
	m_pOriMesh->GetDeclaration(Element);
//	_ulong	dwFVF1 = m_pOriMesh->GetFVF();
//	m_dwStride = D3DXGetFVFVertexSize(dwFVF1);


	for (_int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Element[i].Type == D3DDECLTYPE_UNUSED)
		{
			Element[i].Stream = 0;

			switch(Element[i - 1].Type)
			{
			case D3DDECLTYPE_FLOAT3:
				Element[i].Offset = Element[i - 1].Offset + 12;
				break;
			case D3DDECLTYPE_FLOAT2:
				Element[i].Offset = Element[i - 1].Offset + 8;
				break;
			default:
				Element[i].Offset = Element[i - 1].Offset + 0;
				break;
			}

			Element[i].Type = D3DDECLTYPE_FLOAT3;
			Element[i].Method = D3DDECLMETHOD_DEFAULT;
			Element[i].Usage = D3DDECLUSAGE_TANGENT;
			Element[i].UsageIndex = 0;

			m_dwStride = Element[i].Offset + 12;

			Element[i + 1] = D3DDECL_END();
			break;
		}
	}

	if (FAILED(m_pOriMesh->CloneMesh(m_pOriMesh->GetOptions(), Element, m_pDevice, &m_pMesh)))
		return E_FAIL;

	//메시 사이즈에 맞는 바운딩 박스를 생성해본다.
	void* pVertices = nullptr;

	m_dwNumVertex = m_pMesh->GetNumVertices();
	m_pVtxPos = new _vec3[m_dwNumVertex];

	m_pMesh->LockVertexBuffer(0, &pVertices);

	// 정점의 fvf를 알고 있을 때, 정점의 크기를 얻어와 준다.
//	_ulong	dwFVF = m_pOriMesh->GetFVF();
//	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	_ubyte		byOffSet = 0;

	for (_ubyte i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Element[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffSet = (_ubyte)Element[i].Offset;
			break;
		}
	}


	for (_ulong i = 0; i < m_dwNumVertex; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertices) + (m_dwStride * i + byOffSet)));
	}



	m_pMesh->UnlockVertexBuffer();

	D3DXComputeTangent(m_pMesh, 0, 0, D3DX_DEFAULT, TRUE, NULL);
		

	//텍스처 생성
	m_pSubsets = (D3DXMATERIAL*)m_pSubsetBuffer->GetBufferPointer();//D3DXBuffer가 담고 있는 정보의 가장 앞 주소 리턴
																	//서브셋의 정보를 가져옴

	m_pTextures = new MESHTEXTURE[m_dwSubsetCnt];
	for (_uint i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pTextures[i].pDiffuse = nullptr;
		m_pTextures[i].pNormal = nullptr;
		m_pTextures[i].pSpecular = nullptr;
	}

	for(_uint i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar	szFileName[MAX_PATH] = L"";
		_tchar	szFilePath[MAX_PATH] = L"";
		
		lstrcpy(szFullPath, pFilePath);

		MultiByteToWideChar(CP_ACP, 0, m_pSubsets[i].pTextureFilename, strlen(m_pSubsets[i].pTextureFilename), szFileName, MAX_PATH);

		lstrcat(szFullPath, szFileName);
		lstrcat(szFullPath, pFilePath);

		//if (!lstrcmp(szFileName, L"LV_Hieracon_City_CastleFloor02_D_KJH.tga"))
		//	int a= 0;
	
		Load_Texture(szFileName,pFilePath, L"D", &m_pTextures[i].pDiffuse);
		Load_Texture(szFileName,pFilePath, L"N", &m_pTextures[i].pNormal);
		Load_Texture(szFileName,pFilePath, L"C", &m_pTextures[i].pSpecular);
	
		//if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppTexture[i])))
		//{
		//	_TAGMSG_BOX(L"Texture Object Creation Failed from file, in Ready_Mesh_Static", L"CMesh_Static");
		//	return E_FAIL;
		//}

	}

	return S_OK;
}

void Engine::CMesh_Static::Render_Mesh(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
	//	m_pDevice->SetTexture(0, m_ppTexture[i]);
		m_pMesh->DrawSubset(i);
	}
}

void Engine::CMesh_Static::Render_Mesh(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
//		_bool bAlpha = false;
		_uint iPassCnt = 0;

		//if (bAlpha = Find_Alpha(m_pSubsets[i].pTextureFilename))
		//	iPassCnt = 1;
		if (m_pTextures[i].pSpecular == nullptr)
			iPassCnt = 2;
		else
			iPassCnt = 3;

		pEffect->BeginPass(2);

		if (m_pTextures[i].pDiffuse != nullptr)
			pEffect->SetTexture("g_DiffuseTexture", m_pTextures[i].pDiffuse);

		if (m_pTextures[i].pNormal != nullptr)
			pEffect->SetTexture("g_NormalTexture", m_pTextures[i].pNormal);

		if (m_pTextures[i].pSpecular != nullptr)
			pEffect->SetTexture("g_SpecularTexture", m_pTextures[i].pSpecular);

		pEffect->CommitChanges();
		m_pMesh->DrawSubset(i);

		pEffect->EndPass();

	}


	Safe_Release(pEffect);
}

Engine::CMesh_Static* Engine::CMesh_Static::Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pFilePath, const _tchar* pFileName)
{
	CMesh_Static*	pInstance = new	CMesh_Static(pDevice);

	if (FAILED(pInstance->Ready_Mesh(pFilePath, pFileName)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}


Engine::CComponent* Engine::CMesh_Static::Clone_Component(void)
{
	return new CMesh_Static(*this);
}

void Engine::CMesh_Static::Free(void)
{
	CMesh::Free();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		Safe_Release(m_pTextures[i].pDiffuse);
		Safe_Release(m_pTextures[i].pSpecular);
		Safe_Release(m_pTextures[i].pNormal);
	}
	Safe_Delete_Array(m_pTextures);


	if (false == m_bClone)
		Engine::Safe_Delete_Array(m_pVtxPos);


	Safe_Release(m_pMesh);
	Safe_Release(m_pOriMesh);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubsetBuffer);
	
}

_bool Engine::CMesh_Static::Find_Alpha(const char* pFileName)
{
	_uint	iLength = strlen(pFileName);

	for(_uint i = 0 ; i < iLength; ++i)
	{
		if(pFileName[i] == '.')
		{
			if (pFileName[i - 1] == 'A')
				return true;
		}
	}

	return false;
}

HRESULT CMesh_Static::Load_Texture(const _tchar * pFileName, const _tchar * pFilePath,const _tchar * pTextureKey, LPDIRECT3DTEXTURE9 * ppTexture)
{
	_tchar			szFullPath[MAX_PATH] = L"";
	ZeroMemory(szFullPath, sizeof(_tchar) * MAX_PATH);

	_tchar			szFileName[MAX_PATH] = L"";
	lstrcpy(szFileName, pFileName);

	_int iLength = lstrlen(szFileName);

	for (_int i = 0; i < iLength + 1; i++)
	{
		if (szFileName[i] == L'.')
		{
			for (_int j = i; j >= 0; --j)
			{
				if (szFileName[j] == 'D')
				{
					szFileName[j] = *pTextureKey;

					break;
				}
			}
			break;
		}
	}

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, szFileName);
	if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, ppTexture)))
	{
		ppTexture = nullptr;

	}


	return S_OK;
}
