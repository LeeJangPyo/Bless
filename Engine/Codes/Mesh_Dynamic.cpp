#include "Mesh_Dynamic.h"


Engine::CMesh_Dynamic::CMesh_Dynamic(LPDIRECT3DDEVICE9 pDevice)
	:CMesh(pDevice)
	, m_pHierachy_Loader(nullptr)
	, m_pRootFrame(nullptr)
{

}

Engine::CMesh_Dynamic::CMesh_Dynamic(const CMesh_Dynamic& rhs)
	: CMesh(rhs)
	, m_pHierachy_Loader(rhs.m_pHierachy_Loader)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_MeshContainerList(rhs.m_MeshContainerList)
{
	//새거 생성
	m_pAnimation_Controller = CAnimation_Controller::Create(*rhs.m_pAnimation_Controller);
}

Engine::CMesh_Dynamic::~CMesh_Dynamic(void)
{

}

HRESULT Engine::CMesh_Dynamic::Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pHierachy_Loader = CHierachy_Loader::Create(m_pDevice, pFilePath);
	if (m_pHierachy_Loader == nullptr)
		return E_FAIL;


	LPD3DXANIMATIONCONTROLLER		pAnimation_Controller = nullptr;


	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pDevice,
		m_pHierachy_Loader,
		NULL,
		&m_pRootFrame,
		&pAnimation_Controller)))
	{
		_TAGMSG_BOX(L"D3DXLoadMeshHierarchyFromX Faieldm, In Ready_Mesh", L"CMesh_Dynamic");
		return E_FAIL;
	}
		
	m_pAnimation_Controller = CAnimation_Controller::Create(pAnimation_Controller);
	if (m_pAnimation_Controller == nullptr)
		return E_FAIL;

	Safe_Release(pAnimation_Controller);

	_matrix		matTemp;
	D3DXMatrixIdentity(&matTemp);
	//D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f));

	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

void Engine::CMesh_Dynamic::Render_Mesh(void)
{
	//for (auto& iter : m_MeshContainerList)
	//{
	//	D3DXMESHCONTAINER_DERIVED*		pMeshContainer = iter;

	//	for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
	//		pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * (*pMeshContainer->ppFrameCombinedMatrix[i]);


	//	void*		pSrcVertex = nullptr;
	//	void*		pDestVertex = nullptr;

	//	pMeshContainer->pOriginMesh->LockVertexBuffer(0, &pSrcVertex);
	//	pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVertex);

	//	// 소프트 웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈대들의 정점 정보들의 변경을 동시에 수행해주기도 한다)
	//	pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix,
	//		NULL,
	//		pSrcVertex,
	//		pDestVertex);
	//	// 1인자 : 뼈의 변환 상태
	//	// 2인자 : 원래로 돌리기 위한 상태 행렬(본래는 싹 다 역행렬을 구해줘서 넣어야하지만 굳이 안넣어줘도 된다)
	//	// 3인자 : 변하지 않는 원본 메쉬의 정점 정보
	//	// 4인자 : 변환된 정보를 담기 위한 정점 정보


	//	for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
	//	{
	//		m_pDevice->SetTexture(0, pMeshContainer->ppTexture[i]);
	//		pMeshContainer->MeshData.pMesh->DrawSubset(i);
	//	}

	//	pMeshContainer->pOriginMesh->UnlockVertexBuffer();
	//	pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	//}


}

void Engine::CMesh_Dynamic::Render_Mesh(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();
	{

	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = iter;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * (*pMeshContainer->ppFrameCombinedMatrix[i]);
		}

		void*		pSrcVertex = nullptr;
		void*		pDestVertex = nullptr;

		pMeshContainer->pOriginMesh->LockVertexBuffer(0, &pSrcVertex);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVertex);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix,
			NULL,
			pSrcVertex,
			pDestVertex);
			
		//D3DXComputeTangent(pMeshContainer->pOriginMesh, 0, 0, D3DX_DEFAULT, TRUE, NULL);
		
		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{

			if (pMeshContainer->pTextures[i].pDiffuse !=  nullptr  )
				pEffect->SetTexture("g_DiffuseTexture", pMeshContainer->pTextures[i].pDiffuse);

			if (pMeshContainer->pTextures[i].pNormal != nullptr )
				pEffect->SetTexture("g_NormalTexture", pMeshContainer->pTextures[i].pNormal);

			if (pMeshContainer->pTextures[i].pSpecular != nullptr )
				pEffect->SetTexture("g_SpecularTexture", pMeshContainer->pTextures[i].pSpecular);

			pEffect->CommitChanges();	// 변경된 사항을 다시 정리해주는 함수, 다이렉트 8이후에 등장한 함수

			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pMeshContainer->pOriginMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}

	}
	Safe_Release(pEffect);
}

void Engine::CMesh_Dynamic::Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix)
{
	if (pFrame == nullptr)
		return;

	pFrame->CombinedTransformMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformMatrix);
}

void Engine::CMesh_Dynamic::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*	pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			const char* pBoneName = pMeshContainer->pSkinInfo->GetBoneName(i);
			D3DXFRAME_DERIVED*	pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
			pMeshContainer->ppFrameCombinedMatrix[i] = &pFrame->CombinedTransformMatrix;
		}

		m_MeshContainerList.push_back(pMeshContainer);
	}
	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
}

void Engine::CMesh_Dynamic::Play_AnimationSet(const _float& fTimeDelta)
{
	m_pAnimation_Controller->Play_Animation(fTimeDelta);

	//회전
	_matrix		matTemp;
	D3DXMatrixIdentity(&matTemp);
	//D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f));

	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);
}

void CMesh_Dynamic::Reset_AnimationSet()
{
	m_pAnimation_Controller->Reset_AnimationSet();
}

void Engine::CMesh_Dynamic::Set_AnimationSet(const _uint& iIndex, _bool bResetAnimation)
{
	m_pAnimation_Controller->Set_AnimationSet(iIndex, bResetAnimation);
}

Engine::CMesh_Dynamic* Engine::CMesh_Dynamic::Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pFilePath, const _tchar* pFileName)
{
	CMesh_Dynamic*	pInstance = new CMesh_Dynamic(pDevice);

	if (FAILED(pInstance->Ready_Mesh(pFilePath, pFileName)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* Engine::CMesh_Dynamic::Clone_Component(void)
{
	return new CMesh_Dynamic(*this);
}

void Engine::CMesh_Dynamic::Free(void)
{
	CMesh::Free();

	Safe_Release(m_pAnimation_Controller);

	if (false == m_bClone)
	{
		m_pHierachy_Loader->DestroyFrame(m_pRootFrame);
		Engine::Safe_Release(m_pHierachy_Loader);
	}
	m_MeshContainerList.clear();
}

const Engine::D3DXFRAME_DERIVED* Engine::CMesh_Dynamic::Get_FrameByName(const char* pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

_bool Engine::CMesh_Dynamic::Is_AnimationSetEnd(void)
{
	return m_pAnimation_Controller->Is_AnimationSetEnd();
}

_bool CMesh_Dynamic::Is_AnimationPassRadio(const _float& fRadio)
{
	return m_pAnimation_Controller->Is_AnimationPassRadio(fRadio);
}

_bool CMesh_Dynamic::Is_AnimationInRange(const _float& fStart, const _float& fEnd)
{
	return m_pAnimation_Controller->Is_AnimationInRange(fStart, fEnd);
}

_uint Engine::CMesh_Dynamic::Get_MaxAnimation(void)
{
	return m_pAnimation_Controller->Get_MaxNumAnimation();
}
