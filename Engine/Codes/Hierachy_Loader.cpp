#include "Hierachy_Loader.h"


Engine::CHierachy_Loader::CHierachy_Loader(LPDIRECT3DDEVICE9 pDevice,
										const _tchar* pFilePath)
	:	m_pDevice(pDevice)
	, m_pFilePath(pFilePath)
{
	m_pDevice->AddRef();
}

Engine::CHierachy_Loader::~CHierachy_Loader(void)
{

}

STDMETHODIMP Engine::CHierachy_Loader::Ready_Loader(void)
{
	return S_OK;
}


STDMETHODIMP Engine::CHierachy_Loader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_DERIVED*		pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));

	Allocate_Name(&pNewFrame->Name, Name);

	D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pNewFrame->CombinedTransformMatrix);

	pNewFrame->CombinedTransformMatrix = pNewFrame->TransformationMatrix;

	*ppNewFrame = pNewFrame;

	return S_OK;
}

STDMETHODIMP Engine::CHierachy_Loader::CreateMeshContainer(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*	pNewMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pNewMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	Allocate_Name(&pNewMeshContainer->Name, Name);

	pNewMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH	 pMesh = pMeshData->pMesh;

	_ulong		dwNumFace = pMesh->GetNumFaces();


	//�ﰢ���� ���������� ����, 3���� �������� �ִ�.
	pNewMeshContainer->pAdjacency = new _ulong[dwNumFace * 3];
	memcpy(pNewMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFace * 3);

	// D3DVERTEXELEMENT9 : ������ FVF���� �ϳ��� ������� ����ü.
	D3DVERTEXELEMENT9			Element[MAX_FVF_DECL_SIZE];
	ZeroMemory(&Element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);
	pMesh->GetDeclaration(Element);

	for (_int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Element[i].Type == D3DDECLTYPE_UNUSED)
		{
			Element[i].Stream = 0;

			switch (Element[i - 1].Type)
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

			Element[i + 1] = D3DDECL_END();
			break;
		}
	}

	if (FAILED(pMesh->CloneMesh(pMesh->GetOptions(), Element, m_pDevice, &pNewMeshContainer->MeshData.pMesh)))
		return E_FAIL;
	//_ulong		dwFVF = pMesh->GetFVF();

	//if (!(dwFVF & D3DFVF_NORMAL))
	//{
	//	pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pDevice, &pNewMeshContainer->MeshData.pMesh);
	//	D3DXComputeNormals(pNewMeshContainer->MeshData.pMesh, pNewMeshContainer->pAdjacency);
	//}
	//else
	//{
	//	pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pDevice, &pNewMeshContainer->MeshData.pMesh);
	//}

	pNewMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);
	// ����� ������ 0�� �Ǿ� ������ ��� �޽� �ؽ��İ� ������ ���� �ʾ� ������ �߻��Ѵ�. �� �� ������ ������� 1�� ���� ������ �������� �ڵ�

	
	//��������
	pNewMeshContainer->pMaterials = new D3DXMATERIAL[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);


	if(NumMaterials != 0)
	{
		memcpy(pNewMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

		//�ؽ�ó ����
		pNewMeshContainer->pTextures = new MESHTEXTURE[pNewMeshContainer->NumMaterials];
		ZeroMemory(pNewMeshContainer->pTextures, sizeof(MESHTEXTURE) * pNewMeshContainer->NumMaterials);

		for (_ulong i = 0; i < pNewMeshContainer->NumMaterials; ++i)
		{
			_tchar		szFullPath[MAX_PATH] = L"";
			_tchar		szFileName[MAX_PATH] = L"";

			MultiByteToWideChar(CP_ACP, 0, pNewMeshContainer->pMaterials[i].pTextureFilename
				, strlen(pNewMeshContainer->pMaterials[i].pTextureFilename), szFileName, 128);

			Load_Texture(szFileName, L"D", &pNewMeshContainer->pTextures[i].pDiffuse);
			Load_Texture(szFileName, L"N", &pNewMeshContainer->pTextures[i].pNormal);
			Load_Texture(szFileName, L"C", &pNewMeshContainer->pTextures[i].pSpecular);
		}
	}
	else
	{
		pNewMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Power = 0.f;
		pNewMeshContainer->pMaterials[0].pTextureFilename = nullptr;

	}

	if (nullptr == pSkinInfo)
		return S_OK;

	pNewMeshContainer->pSkinInfo = pSkinInfo;
	pNewMeshContainer->pSkinInfo->AddRef();
	pNewMeshContainer->pSkinInfo->SetDeclaration(Element);

	pNewMeshContainer->dwNumBones = pNewMeshContainer->pSkinInfo->GetNumBones();

	// �� �޽ÿ� ������ ��ġ�� ������ Combined����� �ּҸ� ������.
	pNewMeshContainer->ppFrameCombinedMatrix = new D3DXMATRIX*[pNewMeshContainer->dwNumBones];
	ZeroMemory(pNewMeshContainer->ppFrameCombinedMatrix, sizeof(D3DXMATRIX*) * pNewMeshContainer->dwNumBones);

	pNewMeshContainer->pFrameOffsetMatrix = new D3DXMATRIX[pNewMeshContainer->dwNumBones];
	pNewMeshContainer->pRenderingMatrix = new D3DXMATRIX[pNewMeshContainer->dwNumBones];


	for (size_t i = 0; i < pNewMeshContainer->dwNumBones; i++)
	{
		pNewMeshContainer->pFrameOffsetMatrix[i] = *pNewMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
		D3DXMatrixIdentity(&pNewMeshContainer->pRenderingMatrix[i]);
	}


	if (FAILED(pNewMeshContainer->MeshData.pMesh->CloneMesh(pNewMeshContainer->MeshData.pMesh->GetOptions(), Element, m_pDevice, &pNewMeshContainer->pOriginMesh)))
		return E_FAIL;

	if (FAILED(D3DXComputeTangent(pNewMeshContainer->pOriginMesh, 0, 0, D3DX_DEFAULT, TRUE, NULL)))
		return E_FAIL;


	*ppNewMeshContainer = pNewMeshContainer;
	
	return S_OK;
}

STDMETHODIMP Engine::CHierachy_Loader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	Engine::Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Engine::Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CHierachy_Loader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
	{
		Safe_Release(pMeshContainer->pTextures[i].pDiffuse);
		Safe_Release(pMeshContainer->pTextures[i].pNormal);
		Safe_Release(pMeshContainer->pTextures[i].pSpecular);
	}


	Safe_Delete_Array(pMeshContainer->pTextures);

	Safe_Release(pMeshContainer->pOriginMesh);

	Safe_Delete_Array(pMeshContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pMeshContainer->ppFrameCombinedMatrix);
	Safe_Delete_Array(pMeshContainer->pRenderingMatrix);

	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Release(pMeshContainer->MeshData.pMesh);

	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->pAdjacency);
	Safe_Release(pMeshContainer->pSkinInfo);

	Safe_Delete(pMeshContainer);


	return S_OK;
}

void Engine::CHierachy_Loader::Allocate_Name(char** ppName, const char* pFrameName)
{
	if (pFrameName == nullptr)
		return;

	_uint iLength = strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);

}

HRESULT CHierachy_Loader::Load_Texture(const _tchar * pFullName, const _tchar * pTextureKey, LPDIRECT3DTEXTURE9 * ppTexture)
{
	_tchar			szFullPath[MAX_PATH] = L"";
	ZeroMemory(szFullPath, sizeof(_tchar) * MAX_PATH);

	_tchar			szFileName[MAX_PATH] = L"";
	lstrcpy(szFileName, pFullName);

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

	lstrcpy(szFullPath, m_pFilePath);
	lstrcat(szFullPath, szFileName);

	if(FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, ppTexture)))
	{
		ppTexture = nullptr;
	}

	return S_OK;
}

Engine::CHierachy_Loader* Engine::CHierachy_Loader::Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pFilePath)
{
	CHierachy_Loader*	pInstance = new CHierachy_Loader(pDevice, pFilePath);

	if (FAILED(pInstance->Ready_Loader()))
	{
		_MSG_BOX(L"CHireachy_Loader Creation Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong Engine::CHierachy_Loader::Release(void)
{

	Engine::Safe_Release(m_pDevice);

	delete this;

	return 0;
}
