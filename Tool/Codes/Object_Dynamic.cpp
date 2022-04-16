#include "stdafx.h"
#include "Object_Dynamic.h"

#include "Font_Manager.h"
#include "Camera_Dynamic.h"

#include "MainFrm.h"
#include "ControlForm.h"
#include "MeshTab.h"


_USING(Client)

CObject_Dynamic::CObject_Dynamic(LPDIRECT3DDEVICE9 pDevice)
	:CObject_Mesh(pDevice)
	,m_pMeshCom(nullptr)
	, m_pFont_Manager(CFont_Manager::GetInstance())
{
	m_pFont_Manager->AddRef();
}

CObject_Dynamic::CObject_Dynamic(const CObject_Dynamic & rhs)
	:CObject_Mesh(rhs)
	, m_pFont_Manager(rhs.m_pFont_Manager)
{
	m_pFont_Manager->AddRef();
}


_uint CObject_Dynamic::Get_MaxAnimation(void)
{
	return m_pMeshCom->Get_MaxAnimation();
}

D3DXFRAME* CObject_Dynamic::Get_RootFrame(void)
{
	return m_pMeshCom->Get_RootFrame();
}

void CObject_Dynamic::Set_Animation(const _int& iIndex)
{
	m_pMeshCom->Set_AnimationSet(iIndex);
}

HRESULT CObject_Dynamic::Set_ColliderInfo(const _tchar * pColliderTag, const _float & fX, const _float & fY, const _float & fZ, const _float & fRadius)
{
	auto& iter = find_if(m_mapCollider.begin(), m_mapCollider.end(), CCompare_String(pColliderTag));
	if (iter == m_mapCollider.end())
		return E_FAIL;

	
	iter->second->pCollider->Set_Center_Position(&_vec3(fX, fY, fZ));
	iter->second->pCollider->Set_Radius(fRadius);

	return S_OK;
}

HRESULT CObject_Dynamic::Create_Collider(const _tchar* pColliderTag, const _tchar * pParentBoneTag, const _float& fRadius, CCollider_Sphere** ppCollider)
{
	COLLIDERINFO*	pColliderInfo = new COLLIDERINFO;
	
	pColliderInfo->pCollider = dynamic_cast<CCollider_Sphere*>(CManagement::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Collider_Sphere"));
	if (pColliderInfo->pCollider == nullptr)
		return E_FAIL;

	_char	szBoneTag[MIN_PATH];

	WideCharToMultiByte(CP_ACP, 0, pParentBoneTag, -1, szBoneTag, MIN_PATH, NULL, NULL);

	const Engine::D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName(szBoneTag);
	if (pFrame == nullptr)
		return E_FAIL;

	pColliderInfo->strBoneTag = szBoneTag;

	pColliderInfo->pCollider->SetUp_Collider_Spehre(&pFrame->CombinedTransformMatrix, m_pTransformCom->Get_WorldMatrix(),&_vec3(0.f,0.f,0.f), fRadius);

	m_mapCollider.emplace(pColliderTag, pColliderInfo);

	*ppCollider = pColliderInfo->pCollider;

	return S_OK;
}

HRESULT CObject_Dynamic::Change_Collider_Parent(const _tchar * pColliderTag, const _tchar * pFrameTag)
{
	auto& iter = find_if(m_mapCollider.begin(), m_mapCollider.end(), CCompare_String(pColliderTag));
	if (iter == m_mapCollider.end())
		return E_FAIL;

	_char	szFrameTag[MIN_PATH];

	WideCharToMultiByte(CP_ACP, 0, pFrameTag, -1, szFrameTag, MIN_PATH, NULL, NULL);

	const Engine::D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName(szFrameTag);
	if (pFrame == nullptr)
		return E_FAIL;

	iter->second->pCollider->Set_Parnet_Bone_Matrix(&pFrame->CombinedTransformMatrix);

	return S_OK;
}

HRESULT CObject_Dynamic::Save_ColliderInfo(const HANDLE & hFile, _ulong & dwByte)
{
	_int	iColliderSize = m_mapCollider.size();
	_int	iStrSize;

	//Collider Info Save;
	WriteFile(hFile, &iColliderSize, sizeof(_int), &dwByte, nullptr);
	for(auto& Pair : m_mapCollider)
	{
		//Map Key Size;
		iStrSize = Pair.first.size() + 1;
		WriteFile(hFile, &iStrSize, sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, Pair.first.c_str(), sizeof(_tchar)* iStrSize, &dwByte, nullptr);

		//Collier Info Save
		//BoneString
		iStrSize = Pair.second->strBoneTag.size()+1;
		WriteFile(hFile, &iStrSize, sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, Pair.second->strBoneTag.c_str(), sizeof(_char)* iStrSize, &dwByte, nullptr);

		Pair.second->pCollider->Save_ColliderInfo(hFile, dwByte);
	}

	return S_OK;
}

HRESULT CObject_Dynamic::Load_ColliderInfo(const HANDLE & hFile, _ulong & dwByte)
{
	_int	iColliderSize;
	_int	iStrSize;
	_tchar	szKeyBuffer[MIN_PATH];
	_char	szBoneTagBuffer[MIN_PATH];

	//Collider Info Save;
	ReadFile(hFile, &iColliderSize, sizeof(_int), &dwByte, nullptr);
	for (_int i = 0; i < iColliderSize; ++i)
	{
		COLLIDERINFO*	pColliderInfo = new COLLIDERINFO;
		pColliderInfo->pCollider = dynamic_cast<CCollider_Sphere*>(CManagement::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Collider_Sphere"));
		if (pColliderInfo->pCollider == nullptr)
			return E_FAIL;

		//Map Key Size;
		ReadFile(hFile, &iStrSize, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, szKeyBuffer, sizeof(_tchar)* iStrSize, &dwByte, nullptr);


		//Collier Info Save
		//BoneString
		ReadFile(hFile, &iStrSize, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, szBoneTagBuffer, sizeof(_char)* iStrSize, &dwByte, nullptr);

		pColliderInfo->strBoneTag = szBoneTagBuffer;
		
		//Riadus
		_float	fRadius;
		ReadFile(hFile, &fRadius, sizeof(_float), &dwByte, nullptr);

		//vCenterPos;
		_vec3	vCenterPos;
		ReadFile(hFile, &vCenterPos, sizeof(_vec3), &dwByte, nullptr);

		const Engine::D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName(szBoneTagBuffer);
		if (pFrame == nullptr)
			return E_FAIL;

		pColliderInfo->pCollider->SetUp_Collider_Spehre(&pFrame->CombinedTransformMatrix, m_pTransformCom->Get_WorldMatrix(),&vCenterPos, fRadius);
		

		m_mapCollider.emplace(szKeyBuffer, pColliderInfo);
	}

	return S_OK;
}


HRESULT Client::CObject_Dynamic::Ready_Prototype()
{

	return S_OK;
}


HRESULT Client::CObject_Dynamic::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_ScaleInfo(0.15f, 0.15f, 0.15f);
	return S_OK;
}


HRESULT CObject_Dynamic::SetUp_ObjectInfo(const _matrix * pMatrix, const _tchar * pComponentMeshTag, CMeshTab * pMeshTab)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//Set World Matrix
	m_pTransformCom->Set_WorldMatrix(pMatrix);


	// Set MeshTab Pointer;
	m_pMeshTab = pMeshTab;

	//For. Mesh
	m_pMeshCom = dynamic_cast<CMesh_Dynamic*>(pManagement->Clone_Component(SCENE_EDIT, pComponentMeshTag));
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;



	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CObject_Dynamic::SetUp_ObjectInfo(const _vec3 * pPosition, const _tchar * pComponentMeshTag, CMeshTab * pMeshTab)
{
	// Set Position
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);

	// Set MeshTab Pointer;
	m_pMeshTab = pMeshTab;

	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Mesh
	m_pMeshCom = dynamic_cast<CMesh_Dynamic*>(pManagement->Clone_Component(SCENE_EDIT, pComponentMeshTag));
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;



	m_pMeshCom->Set_AnimationSet(7);

	Safe_Release(pManagement);

	return S_OK;
}
HRESULT CObject_Dynamic::SetUp_ObjectInfo(const _tchar * pComponentMeshTag)
{
	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Mesh
	m_pMeshCom = dynamic_cast<CMesh_Dynamic*>(pManagement->Clone_Component(SCENE_EDIT, pComponentMeshTag));
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pMeshCom->Set_AnimationSet(0);

	Safe_Release(pManagement);

	return S_OK;
}
HRESULT Client::CObject_Dynamic::Ready_Component()
{
	if (CObject_Mesh::Ready_Component())
		return E_FAIL;

	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Shader
	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Default"));
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}


Engine::_int Client::CObject_Dynamic::Update_GameObject(const _float& fTimeDelta)
{
	if (((CMainFrame*)AfxGetMainWnd())->m_pControlForm->m_eTabType == CControlForm::TAB_MESH)
		Check_KeyInput(fTimeDelta); 

	for (auto& Pair : m_mapCollider)
	{
		Pair.second->pCollider->Update_Collider_Sphere();
	}

	return 0;
}


Engine::_int Client::CObject_Dynamic::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pMeshCom->Play_AnimationSet(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this);
	return 0;
}


void Client::CObject_Dynamic::Render_GameObject()
{
	if (m_pShaderCom == nullptr ||
		m_pMeshCom == nullptr)
		return;


	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(2);

	m_pMeshCom->Render_Mesh(pEffect);
	
	pEffect->EndPass();

	pEffect->BeginPass(1);
	for (auto& Pair : m_mapCollider)
	{
		Pair.second->pCollider->SetUp_ConstantTable(pEffect,CCollider_Sphere::COLOR_GREEN);
		pEffect->CommitChanges();
		Pair.second->pCollider->Render_Collider_Sphere();
	}

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);


//	Render_Frame(Get_RootFrame());
}


void CObject_Dynamic::Render_Frame(D3DXFRAME* pFrame)
{
	if (pFrame->Name != NULL)
	{
		_tchar	szFrameName[MIN_PATH] = L"";
		_vec3	vPosition;

		//와이드 케릭터로 변환
		MultiByteToWideChar(CP_ACP, 0, pFrame->Name, strlen(pFrame->Name), szFrameName, MIN_PATH);

		memcpy(&vPosition, &((Engine::D3DXFRAME_DERIVED*)pFrame)->CombinedTransformMatrix._41, sizeof(_vec3));

		m_pFont_Manager->Render_Font(L"Font_Frame", szFrameName, &vPosition, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	}

	if (pFrame->pFrameSibling != nullptr)
		Render_Frame(pFrame->pFrameSibling);

	if (pFrame->pFrameFirstChild != nullptr)
		Render_Frame(pFrame->pFrameFirstChild);
}

_bool CObject_Dynamic::Intersect_Dynamic_Mesh(const RAY & rRay, _vec3 * pOutPosition)
{
	BOOL bHit;
	_ulong	dwFaceIndex;
	_float	fU, fV, fDist;
	LPD3DXBUFFER pAllHits;
	_ulong  dwCountOfHits;

	list<D3DXMESHCONTAINER_DERIVED*>* pMeshContainerList = m_pMeshCom->Get_MeshContainerList();

	//Ray is transfrom to local Space;
	for(auto& pMesh : *pMeshContainerList)
	{
		//Interesect Mesh
		D3DXIntersect(pMesh->pOriginMesh, &rRay.vRayPos, &rRay.vRayDir, &bHit, &dwFaceIndex, &fU, &fV, &fDist, &pAllHits, &dwCountOfHits);

		if (bHit == TRUE)
		{
			*pOutPosition = rRay.vRayPos + (rRay.vRayDir * fDist);
			D3DXVec3TransformCoord(pOutPosition, pOutPosition, m_pTransformCom->Get_WorldMatrix());
			return true;
		}

	}

	pOutPosition = nullptr;
	return false;
}



void CObject_Dynamic::Check_KeyInput(const _float& fTimeDelta)
{

	//Exception
	HWND	hWnd = ::GetFocus();

	if (hWnd != g_hWnd)
		return;

	POINT	ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (ptMouse.x < 0 || ptMouse.x > g_nBackCX || ptMouse.y < 0 || ptMouse.y > g_nBackCY)
		return;


	if (m_pMeshTab->Get_ObjectType() != CMeshTab::DYNAMIC_OBJECT)
		return;

	if (m_pInput_Device->Get_MouseDown(CInput_Device::DIM_LB))
	{
		//Transform MosuePos to World Ray
		RAY	tRay = m_pCalculatorCom->Transform_ScreenPosToRayLocal(g_hWnd, m_pTransformCom->Get_WorldMatrix());

		_vec3 vIntersectPoint;
		if (Intersect_Dynamic_Mesh(tRay, &vIntersectPoint))
		{
			m_bIsPick = true;
			m_bIsMouseDown = true;
			m_pMeshTab->m_pPickedMeshObject = this;
			m_pMeshTab->Update_Transform(CMeshTab::DATA_POSITION, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));

			_vec3 vScale = _vec3(m_pTransformCom->Get_ScaleInfo(CTransform::STATE_RIGHT),
				m_pTransformCom->Get_ScaleInfo(CTransform::STATE_UP),
				m_pTransformCom->Get_ScaleInfo(CTransform::STATE_LOOK));
			m_pMeshTab->Update_Transform(CMeshTab::DATA_SCALE, &vScale);
		}
		else
		{
			m_bIsPick = false;

		}
	}
	if (m_pInput_Device->Get_MousePressing(CInput_Device::DIM_LB))
	{
		//
		if (m_bIsMouseDown)
		{
			_vec3 vPosition = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);

			_float fMouseMove = 0.f;
			if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Y))
			{
				vPosition.z -= fMouseMove * 0.01f;
			}

			fMouseMove = 0.f;
			if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_X))
			{
				vPosition.x += fMouseMove * 0.01f;
			}

			m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);

			//Update MFC Data
			m_pMeshTab->Update_Transform(CMeshTab::DATA_POSITION, &vPosition);
		}
	}

	if (m_pInput_Device->Get_MouseUp(CInput_Device::DIM_LB))
	{
		m_bIsMouseDown = false;
		m_pMeshTab->Update_Transform(CMeshTab::DATA_POSITION, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));
	}
}

HRESULT CObject_Dynamic::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld")))
		return E_FAIL;


	//View Proejction Matrix
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	pManagement->AddRef();

	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(pManagement->Get_GameObject(SCENE_STATIC, L"Layer_Camera_Dynamic"));
	if (pCamera == nullptr)
	{
		Safe_Release(pManagement);
		Safe_Release(pEffect);
		return E_FAIL;
	}

	pCamera->SetUp_OnShader_View(pEffect, "g_matView");
	pCamera->SetUp_OnShader_Projection(pEffect, "g_matProj");

	Safe_Release(pManagement);
	Safe_Release(pEffect);

	return S_OK;
}


CObject_Dynamic * CObject_Dynamic::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CObject_Dynamic*	pInstance = new CObject_Dynamic(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CObject_Dynamic Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* Client::CObject_Dynamic::Clone_GameObject()
{
	CObject_Dynamic*	pInstance = new CObject_Dynamic(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CObject_Dynamic Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CObject_Dynamic::Free(void)
{
	for (auto& Colldier : m_mapCollider)
	{
		Safe_Release(Colldier.second->pCollider);
		delete Colldier.second;
	}
	m_mapCollider.clear();

	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pMeshCom);
//	Safe_Release(m_pColliderCom);

	CObject_Mesh::Free();
}
