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
	if(FAILED(D3DXLoadMeshFromX(szFullPath,	//�ε��� x����
											D3DXMESH_MANAGED,	//�޽ø� �����ϴ� ���
											m_pDevice,			
											&m_pAdjacency,		//�ִϸ��̼� �޽��� ������ �� ��Ű���� ���� ���� ���� ������ �����صδ� ������
											&m_pSubsetBuffer,	//����¿� ���� ������ �ǹ��ϴ� ������, �� �޽��� ���� ������ �����ϴ� ������
											NULL,				//�޽� ��ü�� ���ϰ� �ִ� ����Ʈ�� ���� ����, �츮�� �����ϴ� �޽����� �� ������ ���� ���� NULL
											&m_dwSubsetCnt,		//���׸��� ����(����°���)�� �����ϱ� ���� ����
											&m_pOriMesh)))			//�ε�� �޽� ������ ������ ������
	{
		_TAGMSG_BOX(L"MeshObject Creation  Failed from File, in Ready_Mesh", L"CMesh_Static");
		return E_FAIL;
	}
											

	////�븻 �ɼ��� ���� �޽��� �븻 ���� �ο��ϱ� ���� �۾�
	//_ulong	dwFVF = m_pOriMesh->GetFVF();

	//if( !(dwFVF & D3DFVF_NORMAL) )
	//{
	//	//������ �ε��� �޽��� fvf ������ �������ִ� �Լ�
	//	m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pDevice, &m_pMesh);

	//	//������ �븻�� ����� �ִ� �Լ�
	//	D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	//}
	//else
	//{
	//	m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pDevice, &m_pMesh);
	//}
	// D3DVERTEXELEMENT9 : ������ FVF���� �ϳ��� ������� ����ü.
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

	//�޽� ����� �´� �ٿ�� �ڽ��� �����غ���.
	void* pVertices = nullptr;

	m_dwNumVertex = m_pMesh->GetNumVertices();
	m_pVtxPos = new _vec3[m_dwNumVertex];

	m_pMesh->LockVertexBuffer(0, &pVertices);

	// ������ fvf�� �˰� ���� ��, ������ ũ�⸦ ���� �ش�.
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
		

	//�ؽ�ó ����
	m_pSubsets = (D3DXMATERIAL*)m_pSubsetBuffer->GetBufferPointer();//D3DXBuffer�� ��� �ִ� ������ ���� �� �ּ� ����
																	//������� ������ ������

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
