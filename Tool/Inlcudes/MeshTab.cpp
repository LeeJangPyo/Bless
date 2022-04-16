// MeshTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MeshTab.h"
#include "afxdialogex.h"

#include "Management.h"
#include "Camera_Dynamic.h"
#include "Mesh_Navigation_Controller.h"

#include "Graphic_Device.h"

#include <io.h>

#include "Object_Static.h"
#include "Object_Dynamic.h"


// CMeshTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMeshTab, CDialogEx)

CMeshTab::CMeshTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MESH, pParent)
	, m_pPickedMeshObject(nullptr)
	, m_fPositionX(0)
	, m_fPositionY(0)
	, m_fPositionZ(0)
	, m_fScaleX(0)
	, m_fScaleY(0)
	, m_fScaleZ(0)
	, m_strDynamic_LayerTag(_T(""))
{
	for(int i = 0; i < 4; ++i)
	{

		m_fRotation[i] = 0.f;
	}
	m_eObjectType = STATIC_OBJECT;
	
}

CMeshTab::~CMeshTab()
{
	Engine::Safe_Release(m_pCalculatorCom);
}

void CMeshTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RADIO1, m_btRenderState[0]);
	DDX_Control(pDX, IDC_RADIO2, m_btRenderState[1]);
	DDX_Control(pDX, IDC_RADIO5, m_RbObjectSelect[0]);
	DDX_Control(pDX, IDC_RADIO6, m_RbObjectSelect[1]);


	DDX_Text(pDX, IDC_ROTATION_X, m_fRotation[0]);
	DDX_Text(pDX, IDC_ROTATION_Y, m_fRotation[1]);
	DDX_Text(pDX, IDC_ROTATION_Z, m_fRotation[2]);

	DDX_Control(pDX, IDC_OBJECTTREE, m_TreeStaticMeshList);
	DDX_Control(pDX, IDC_OBJECTTREE2, m_TreeDynamicMeshList);

	DDX_Text(pDX, IDC_POSITION_X, m_fPositionX);
	DDX_Text(pDX, IDC_POSITION_Y, m_fPositionY);
	DDX_Text(pDX, IDC_POSITION_Z, m_fPositionZ);
	DDX_Text(pDX, IDC_SCALE_X, m_fScaleX);
	DDX_Text(pDX, IDC_SCALE_Y, m_fScaleY);
	DDX_Text(pDX, IDC_SCALE_Z, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT_DYANMIC_LAYER, m_strDynamic_LayerTag);
}


BEGIN_MESSAGE_MAP(CMeshTab, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CMeshTab::OnBnClickedRbSolid)
	ON_BN_CLICKED(IDC_RADIO2, &CMeshTab::OnBnClickedRbWire)
	ON_BN_CLICKED(IDC_RADIO5, &CMeshTab::OnBnClickedRbStatic)
	ON_BN_CLICKED(IDC_RADIO6, &CMeshTab::OnBnClickedRbDynamic)
	ON_NOTIFY(NM_DBLCLK, IDC_OBJECTTREE, &CMeshTab::OnNMDblclkStaticObjectList)
	ON_NOTIFY(NM_DBLCLK, IDC_OBJECTTREE2, &CMeshTab::OnNMDblclkDynamicObjectList)
	ON_BN_CLICKED(IDC_BUTTON1, &CMeshTab::OnBnClickedBtApply)
	ON_BN_CLICKED(IDC_BUTTON2, &CMeshTab::OnBnClickedSaveStaticMeshList)
	ON_BN_CLICKED(IDC_BUTTON7, &CMeshTab::OnBnClickedLoadStaticMesh)
//	ON_BN_CLICKED(IDC_BUTTON8, &CMeshTab::OnBnClickedSaveDynamicMesh)
//	ON_BN_CLICKED(IDC_BUTTON9, &CMeshTab::OnBnClickedLoadDynamicMesh)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCALE_X, &CMeshTab::OnDeltaposSpinScaleX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCALE_Y, &CMeshTab::OnDeltaposSpinScaleY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCALE_Z, &CMeshTab::OnDeltaposSpinScaleZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ROTATION_Y, &CMeshTab::OnDeltaposSpinRotationY)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POSITION_X, &CMeshTab::OnDeltaposSpinPositionX)
//	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POSITION_Y, &CMeshTab::OnDeltaposSpinPositionY)
//	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POSITION_Z2, &CMeshTab::OnDeltaposSpinPositionZ2)
	ON_BN_CLICKED(IDC_BT_TRANSFORM_APPLY, &CMeshTab::OnBnClickedBtTransformApply)
	ON_BN_CLICKED(IDC_BT_DELETE, &CMeshTab::OnBnClickedBtDelete)
	ON_BN_CLICKED(IDC_BT_SAVE_DYNAMIC, &CMeshTab::OnBnClickedBtSaveDynamic)
	ON_BN_CLICKED(IDC_BT_LOAD_DYANMIC, &CMeshTab::OnBnClickedBtLoadDyanmic)
	ON_BN_CLICKED(IDC_BT_LAYERTAG, &CMeshTab::OnBnClickedBtLayertag)
END_MESSAGE_MAP()

HRESULT CMeshTab::Ready_Component()
{
	
	m_pCalculatorCom = dynamic_cast<CCalculator*>(CManagement::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Calculator"));
	if (m_pCalculatorCom == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshTab::Set_ObjectList(void)
{
	//파일 읽어옴
	HMODULE	hMoudle = GetModuleHandle(nullptr);
	ASSERT(hMoudle != 0);

	TCHAR		szFullPath[MAX_PATH] = L"";
	VERIFY(GetModuleFileNameW(hMoudle, szFullPath, MAX_PATH));
	PathRemoveFileSpecW(szFullPath);
	lstrcat(szFullPath, L"\\Resources\\Mesh\\StaticMesh\\*.*");

	//LoaD Object List
	HTREEITEM StaticMeshRoot = m_TreeStaticMeshList.InsertItem(L"Static Mesh");
	FindDirAddMeshList(szFullPath, m_TreeStaticMeshList, StaticMeshRoot);

	VERIFY(GetModuleFileNameW(hMoudle, szFullPath, MAX_PATH));
	PathRemoveFileSpecW(szFullPath);
	lstrcat(szFullPath, L"\\Resources\\Mesh\\DynamicMesh\\*.*");

	HTREEITEM DynamicMeshRoot = m_TreeDynamicMeshList.InsertItem(L"Dynamic Mesh");
	FindDirAddMeshList(szFullPath, m_TreeDynamicMeshList, DynamicMeshRoot);


	return S_OK;
}


HRESULT CMeshTab::FindDirAddMeshList(const wstring& wstrPath,
									CTreeCtrl&	rTreeCtrl,
									HTREEITEM hTreeItem)
{
	_wfinddata_t		FindData;

	wstring		wstrCombinedPath = wstrPath;
	_long		handle;
	_int		iContinue = 1;

	handle = _wfindfirst(wstrPath.c_str(), &FindData);

	if (handle == -1)
		return E_FAIL;

	while (iContinue != -1)
	{
		iContinue = _wfindnext(handle, &FindData);
		if (!lstrcmp(FindData.name, L".") || !lstrcmp(FindData.name, L"..") || iContinue == -1)
			continue;

		wstrCombinedPath = wstrPath.substr(0, wstrPath.length() - 3) + FindData.name;

		//파일이 있으면
		if (FindData.size != 0)
		{
			_tchar*		szExtension;
			//엑스파일만 검사
			szExtension = PathFindExtension(FindData.name);

			if (lstrcmp(szExtension, L".X") != 0)
				continue;

			rTreeCtrl.InsertItem(FindData.name, hTreeItem);
		}
		else//폴더면
		{
			HTREEITEM parentItem = rTreeCtrl.InsertItem(FindData.name, hTreeItem);
			FindDirAddMeshList(wstrCombinedPath + L"\\*.*", rTreeCtrl, parentItem);

		}

		
	}
	_findclose(handle);

	return S_OK;
}



void CMeshTab::OnBnClickedRbSolid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_btRenderState[0].SetCheck(TRUE);
	m_btRenderState[1].SetCheck(FALSE);

	AfxMessageBox(L"Solid 클릭");


	UpdateData(FALSE);
}


void CMeshTab::OnBnClickedRbWire()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_btRenderState[0].SetCheck(FALSE);
	m_btRenderState[1].SetCheck(TRUE);

	AfxMessageBox(L"Wire 클릭");


	UpdateData(FALSE);
}

void CMeshTab::OnBnClickedRbStatic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_RbObjectSelect[0].SetCheck(TRUE);
	m_RbObjectSelect[1].SetCheck(FALSE);

	//AfxMessageBox(L"Static 클릭");
	m_eObjectType = STATIC_OBJECT;


	UpdateData(FALSE);
}

void CMeshTab::OnBnClickedRbDynamic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_RbObjectSelect[0].SetCheck(FALSE);
	m_RbObjectSelect[1].SetCheck(TRUE);

	m_eObjectType = DYNAMIC_OBJECT;


	UpdateData(FALSE);
}

BOOL CMeshTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Set_ObjectList();

	Ready_Component();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

_vec3 CMeshTab::PickUp_OnTerrain(void)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return _vec3();

	pManagement->AddRef();


	CBuffer_TerrainTex*	pTerrainBufferCom = dynamic_cast<CBuffer_TerrainTex*>
		(pManagement->Get_Component(SCENE_EDIT,L"Layer_Terrain",L"Com_Buffer"));
	if(pTerrainBufferCom == nullptr)
	{
		Safe_Release(pManagement);
		return _vec3();
	}

	CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>
		(pManagement->Get_Component(SCENE_EDIT,L"Layer_Terrain", L"Com_Transform"));
	if (pTerrainBufferCom == nullptr)
	{
		Safe_Release(pManagement);
		return _vec3();
	}

	Safe_Release(pManagement);

	RAY tRay = m_pCalculatorCom->Transform_ScreenPosToRay(&_vec2(400.f, 300.f));

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

void CMeshTab::SetUp_OnTerrain(D3DXVECTOR3& vPos)
{
	g_hWnd;

	CBuffer_TerrainTex*	pTerrainBufferCom = dynamic_cast<CBuffer_TerrainTex*>
		(CManagement::GetInstance()->Get_Component(SCENE_STATIC,L"Layer_Terrain",L"Com_Buffer"));
	if (pTerrainBufferCom == nullptr)
		return;

	float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ, _float(VTXITV));

	vPos.y = fHeight;
}

void CMeshTab::OnNMDblclkStaticObjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CPoint tPoint;
	GetCursorPos(&tPoint);
	UINT flag;
	m_TreeStaticMeshList.ScreenToClient(&tPoint);

	HTREEITEM hItemCur = m_TreeStaticMeshList.HitTest(tPoint, &flag);
	HTREEITEM hItemParent = m_TreeStaticMeshList.GetParentItem(hItemCur);

	CString strCur = m_TreeStaticMeshList.GetItemText(hItemCur);
	CString strParent = m_TreeStaticMeshList.GetItemText(hItemParent);

	wstring wstrComponentMeshTag = L"Component_Mesh_Static_";
	wstring wstrGameObjectTag = L"GameObject_Static_";
	wstring wstrFilePath = L"../Bin/Resources/Mesh/StaticMesh/";
	wstring wstrFileName = (LPCWSTR)strCur;

	wstrComponentMeshTag += wstrFileName.substr(0, wstrFileName.length() - 2);
	wstrGameObjectTag += wstrFileName.substr(0, wstrFileName.length() - 2);
	wstrFilePath += (LPCWSTR)strParent;
	wstrFilePath += L"/";

	if(hItemCur == NULL)
	{
		AfxMessageBox(L"Don't Clicked Item");
		return;
	}

	if (strCur == L"Static Mesh")
		return;

	//////////////////////////////////////////////////////////////////////////
	//Picked Position
	_vec3 vPickingPosition;
	vPickingPosition = PickUp_OnTerrain();
	if (vPickingPosition == _vec3())
		return;

	//////////////////////////////////////////////////////////////////////////
	//Create Static Mesh

	CObject_Static* pObject_Static;

	Create_Prototype_Component_Mesh(MESH_STATIC, wstrFilePath.c_str(), wstrFileName.c_str(), wstrComponentMeshTag.c_str());
	Create_Prototype_GameObject(MESH_STATIC, wstrGameObjectTag.c_str());
	if (FAILED(Add_Layer_GameObject(L"Layer_Static",wstrGameObjectTag.c_str(), (CGameObject**)&pObject_Static)))
		return;

	//Late SetUp Staic Object
	pObject_Static->SetUp_ObjectInfo(&vPickingPosition, wstrComponentMeshTag.c_str(), this);
	pObject_Static->SetUp_FileInfo(wstrFilePath.c_str(), wstrFileName.c_str(), wstrGameObjectTag.c_str(), wstrComponentMeshTag.c_str());

	//////////////////////////////////////////////////////////////////////////
	//SetUp EditBox matrix Info;
	SetUp_EditBox_DefaultInfo(&vPickingPosition);
}


void CMeshTab::OnNMDblclkDynamicObjectList(NMHDR *pNMHDR, LRESULT *pResult)
{ 

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if (m_strDynamic_LayerTag == L"")
		return;

	CPoint tPoint;
	GetCursorPos(&tPoint);
	UINT flag;
	m_TreeDynamicMeshList.ScreenToClient(&tPoint);

	HTREEITEM hItemCur = m_TreeDynamicMeshList.HitTest(tPoint, &flag);
	HTREEITEM hItemParent = m_TreeDynamicMeshList.GetParentItem(hItemCur);

	CString strCur = m_TreeDynamicMeshList.GetItemText(hItemCur);
	CString strParent = m_TreeDynamicMeshList.GetItemText(hItemParent);

	wstring wstrComponentMeshTag = L"Component_Mesh_";
	wstring wstrGameObjectTag = L"GameObject_";
	wstring wstrFilePath = L"../Bin/Resources/Mesh/DynamicMesh/";
	wstring wstrFileName = (LPCWSTR)strCur;

	wstrComponentMeshTag += wstrFileName.substr(0, wstrFileName.length() - 2);
	wstrGameObjectTag += wstrFileName.substr(0, wstrFileName.length() - 2);
	wstrFilePath += (LPCWSTR)strParent;
	wstrFilePath += L"/";

	if (hItemCur == NULL || strParent == L"Dynamic Mesh")
	{
		return;
	}
	if (strCur == L"Dynamic Mesh")
		return;

	//////////////////////////////////////////////////////////////////////////
	//Picked Position
	_vec3 vPickingPosition;
	vPickingPosition = PickUp_OnTerrain();
	if (vPickingPosition == _vec3())
		return;

	//////////////////////////////////////////////////////////////////////////
	//Create Dynamic Object

	CObject_Dynamic* pObject_Dynamic;


	Create_Prototype_Component_Mesh(MESH_DYNAMIC,wstrFilePath.c_str(),wstrFileName.c_str(),wstrComponentMeshTag.c_str());
	Create_Prototype_GameObject(MESH_DYNAMIC, wstrGameObjectTag.c_str());
	if (FAILED(Add_Layer_GameObject(m_strDynamic_LayerTag, wstrGameObjectTag.c_str(), (CGameObject**)&pObject_Dynamic)))
		return;
	pObject_Dynamic->SetUp_ObjectInfo(&vPickingPosition, wstrComponentMeshTag.c_str(), this);
	pObject_Dynamic->SetUp_FileInfo(wstrFilePath.c_str(), wstrFileName.c_str(), wstrGameObjectTag.c_str(), wstrComponentMeshTag.c_str());

	//////////////////////////////////////////////////////////////////////////
	//SetUp EditBox matrix Info;
	SetUp_EditBox_DefaultInfo(&vPickingPosition);
}



HRESULT CMeshTab::Create_Prototype_Component_Mesh(MeshType eType, const _tchar * pFilePath, const _tchar * pFileName, const _tchar * pComponentMeshTag)
{
	//////////////////////////////////////////////////////////////////////////
	//Create Dynamic Mesh
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	pManagement->AddRef();

	LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	if (pDevice == nullptr)
	{
		Safe_Release(pManagement);
		return E_FAIL;
	}
	pDevice->AddRef();

	if (FAILED(pManagement->Find_Prototype_Component(SCENE_EDIT, pComponentMeshTag)))
	{
		Safe_Release(pDevice);
		Safe_Release(pManagement);
		return E_FAIL;
	}

	switch (eType)
	{
	case CMeshTab::MESH_DYNAMIC:
		//Dynamic Mesh Component Load
		pManagement->Add_Prototype_Component(SCENE_EDIT, pComponentMeshTag,
			CMesh_Dynamic::Create(pDevice, pFilePath, pFileName));
		break;
	case CMeshTab::MESH_STATIC:
		pManagement->Add_Prototype_Component(SCENE_EDIT, pComponentMeshTag,
			CMesh_Static::Create(pDevice, pFilePath, pFileName));
		break;
	}

	Safe_Release(pManagement);
	Safe_Release(pDevice);

	return S_OK;
}

HRESULT CMeshTab::Create_Prototype_GameObject(MeshType eType, const _tchar * pGameObjectTag)
{

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}
	pManagement->AddRef();

	LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	if (pDevice == nullptr)
	{
		Safe_Release(pManagement);
		return E_FAIL;
	}
	pDevice->AddRef();

	if (FAILED(pManagement->Find_Prototype_GameObject(pGameObjectTag)))
	{
		Safe_Release(pDevice);
		Safe_Release(pManagement);
		return E_FAIL;
	}

	switch (eType)
	{
	case CMeshTab::MESH_DYNAMIC:
		if (FAILED(pManagement->Add_Prototype_GameObject(pGameObjectTag, CObject_Dynamic::Create(pDevice))))
		{
			Safe_Release(pDevice);
			Safe_Release(pManagement);
			return E_FAIL;
		}
		break;
	case CMeshTab::MESH_STATIC:
		if (FAILED(pManagement->Add_Prototype_GameObject(pGameObjectTag, CObject_Static::Create(pDevice))))
		{
			Safe_Release(pDevice);
			Safe_Release(pManagement);
			return E_FAIL;
		}
		break;
	}
	Safe_Release(pDevice);
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CMeshTab::Add_Layer_GameObject(const _tchar* pLayerTag, const _tchar * pGameObjectTag, CGameObject** ppGameObject)
{

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		if (FAILED(pManagement->Add_Layer(SCENE_EDIT, pLayerTag, pGameObjectTag, ppGameObject)))
		{
			Safe_Release(pManagement);
			return E_FAIL;
		}
	}
	Safe_Release(pManagement);

	return S_OK;
}

void CMeshTab::SetUp_EditBox_DefaultInfo(const _vec3* pPosition)
{
	UpdateData(TRUE);
	m_fScaleX = g_fDefault_Sacle;
	m_fScaleY = g_fDefault_Sacle;
	m_fScaleZ = g_fDefault_Sacle;

	m_fRotation[0] = 0.f;
	m_fRotation[1] = 0.f;
	m_fRotation[2] = 0.f;

	m_fPositionX = pPosition->x;
	m_fPositionY = pPosition->y;
	m_fPositionZ = pPosition->z;
	UpdateData(FALSE);
}



void CMeshTab::Update_Transform(DATATYPE eDataType, const D3DXVECTOR3* pTransform)
{
	UpdateData(TRUE);

	switch (eDataType)
	{
	case CMeshTab::DATA_SCALE:
		m_fScaleX = pTransform->x;
		m_fScaleY = pTransform->y;
		m_fScaleZ = pTransform->z;
		break;
	case CMeshTab::DATA_POSITION:
		m_fPositionX = pTransform->x;
		m_fPositionY = pTransform->y;
		m_fPositionZ = pTransform->z;
		break;
	case CMeshTab::DATA_ROTATION:
		m_fRotation[0] = pTransform->x;
		m_fRotation[1] = pTransform->y;
		m_fRotation[2] = pTransform->z;
		break;
	default:
		break;
	}

	UpdateData(FALSE);
}

void CMeshTab::Get_Trasnform(DATATYPE eDataTyope, D3DXVECTOR3* pTransform)
{
	switch (eDataTyope)
	{
	case CMeshTab::DATA_SCALE:
		pTransform->x = m_fScaleX;
		pTransform->y = m_fScaleY;
		pTransform->z = m_fScaleZ;
		break;
	case CMeshTab::DATA_POSITION:
		pTransform->x = m_fPositionX;
		pTransform->y = m_fPositionY;
		pTransform->z = m_fPositionZ;
		break;
	case CMeshTab::DATA_ROTATION:
		pTransform->x = m_fRotation[0];
		pTransform->y = m_fRotation[1];
		pTransform->z = m_fRotation[2];
		break;
	default:
		break;
	}
}


void CMeshTab::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.


	CDialogEx::PostNcDestroy();
}


void CMeshTab::OnBnClickedBtApply()
{
	if (m_pPickedMeshObject == nullptr)
		return;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_pPickedMeshObject->Set_Position(&_vec3(m_fPositionX, m_fPositionY, m_fPositionZ));
	//m_pPickedMeshObject->Set_Rotation(&_vec3(m_fRotation[0], m_fRotation[1], m_fRotation[2]));
	m_pPickedMeshObject->Set_Scale(&_vec3(m_fScaleX, m_fScaleY, m_fScaleZ));
	

	UpdateData(FALSE);

}


void CMeshTab::OnBnClickedSaveStaticMeshList()
{
	CFileDialog		Dlg(FALSE, L"smesh", L"*.smesh"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.smesh", this);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);
	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Data\\", szFullPath);

	Dlg.m_ofn.lpstrInitialDir = szFullPath;

	if (Dlg.DoModal() == IDCANCEL)
		return;

	//Get Static Mesh List
	CManagement *pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();

	const list<CGameObject*>* pStatic_ObjectList = pManagement->Get_Layer_ObjectList(SCENE_EDIT, L"Layer_Static");
	if(pStatic_ObjectList == nullptr)
	{
		Safe_Release(pManagement);
		return;
	}
	Safe_Release(pManagement);

	//Create File
	HANDLE		hFile = CreateFile(Dlg.GetPathName()
		, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD	dwByte = 0;

	for (auto& pObject : *pStatic_ObjectList)
	{
		CTransform* pTransform;
		//월드매트릭스 저장
		pTransform = dynamic_cast<CTransform*>(pObject->Get_Component(L"Com_Transform"));
		if (pObject == nullptr)
			break;
		WriteFile(hFile, pTransform->Get_WorldMatrix(), sizeof(_matrix), &dwByte, nullptr);

		//컴포넌트 태그 저장해야함
		
		CObject_Static*	pObject_Static;
		pObject_Static = dynamic_cast<CObject_Static*>(pObject);
		if (pObject_Static == nullptr)
			break;

		//파일경로
		WriteFile(hFile, pObject_Static->Get_FilePath(), sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//파일이름
		WriteFile(hFile, pObject_Static->Get_FileName(), sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//오브젝트 테그
		WriteFile(hFile, pObject_Static->Get_GameObjecTag(), sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//메쉬컴포넌트 테그
		WriteFile(hFile, pObject_Static->Get_ComponentMeshTag(), sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
	}

	CloseHandle(hFile);
}


void CMeshTab::OnBnClickedLoadStaticMesh()
{
	CFileDialog		Dlg(TRUE, L"smesh", L"*.smesh"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.smesh", this);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Data\\", szFullPath);

	Dlg.m_pOFN->lpstrInitialDir = szFullPath;

	if (Dlg.DoModal() == IDCANCEL)
		return;


	//기존 네비메쉬의 데이터도 삭제해야 할 수 도 있음
	HANDLE		hFile = CreateFile(Dlg.GetPathName()
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD	dwByte = 0;
	_ulong	dwIndex = 0;

	while (true)
	{
		//월드 매트릭스 로드
		_matrix matWorld;
		ReadFile(hFile, &matWorld, sizeof(_matrix), &dwByte, nullptr);


		//파일경로
		_tchar szFilePath[MIN_PATH] = L"";
		ReadFile(hFile, szFilePath, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//파일이름
		_tchar szFileName[MIN_PATH] = L"";
		ReadFile(hFile, szFileName, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//오브젝트 테그
		_tchar szGameObjectTag[MIN_PATH] = L"";
		ReadFile(hFile, szGameObjectTag, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//컴포넌트매쉬태그 읽기
		_tchar szComponentMeshTag[MIN_PATH] = L"";
		ReadFile(hFile, szComponentMeshTag, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);

		if (dwByte == 0)
		{
			break;
		}
	
		wstring wstrComponentMeshTag = szComponentMeshTag;
		wstring wstrGameObjectTag = szGameObjectTag;
		wstring wstrFileName = szFileName;
		wstring wstrFilePath = szFilePath;
		
	//	wstrFileName = wstrFileName.sub
		//////////////////////////////////////////////////////////////////////////
		//Create Static Mesh
		CObject_Static* pObject_Static;
		Create_Prototype_Component_Mesh(MESH_STATIC, wstrFilePath.c_str(), wstrFileName.c_str(), wstrComponentMeshTag.c_str());
		Create_Prototype_GameObject(MESH_STATIC, wstrGameObjectTag.c_str());
		if (FAILED(Add_Layer_GameObject(L"Layer_Static", wstrGameObjectTag.c_str(), (CGameObject**)&pObject_Static)))
			return;
		//Late SetUp Staic Object
		pObject_Static->SetUp_ObjectInfo(&matWorld, wstrComponentMeshTag.c_str(), this);
		pObject_Static->SetUp_FileInfo(wstrFilePath.c_str(), wstrFileName.c_str(), wstrGameObjectTag.c_str(), wstrComponentMeshTag.c_str());

	}
	CloseHandle(hFile);
}
void CMeshTab::OnBnClickedBtSaveDynamic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(FALSE, L"dmesh", L"*.dmesh"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dmesh", this, 0, false);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Data\\", szFullPath);

	Dlg.m_pOFN->lpstrInitialDir = szFullPath;

	if (Dlg.DoModal() == IDCANCEL)
		return;



	//Get Dynamic Mesh List
	CManagement *pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();

	UpdateData(TRUE);
	const list<CGameObject*>* pDynamic_ObjectList = pManagement->Get_Layer_ObjectList(SCENE_EDIT, m_strDynamic_LayerTag);
	UpdateData(FALSE);

	if (pDynamic_ObjectList == nullptr)
	{
		Safe_Release(pManagement);
		return;
	}
	Safe_Release(pManagement);

	//Create File
	HANDLE		hFile = CreateFile(Dlg.GetPathName()
		, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD	dwByte = 0;

	for (auto& pObject : *pDynamic_ObjectList)
	{
		CTransform* pTransform;
		//월드매트릭스 저장
		pTransform = dynamic_cast<CTransform*>(pObject->Get_Component(L"Com_Transform"));
		if (pObject == nullptr)
			break;
		WriteFile(hFile, pTransform->Get_WorldMatrix(), sizeof(_matrix), &dwByte, nullptr);

		//컴포넌트 태그 저장해야함

		CObject_Dynamic*	pObject_Dynamic;
		pObject_Dynamic = dynamic_cast<CObject_Dynamic*>(pObject);
		if (pObject_Dynamic == nullptr)
			break;

		//파일경로
		WriteFile(hFile, pObject_Dynamic->Get_FilePath(), sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//파일이름
		WriteFile(hFile, pObject_Dynamic->Get_FileName(), sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//오브젝트 테그
		WriteFile(hFile, pObject_Dynamic->Get_GameObjecTag(), sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//메쉬컴포넌트 테그
		WriteFile(hFile, pObject_Dynamic->Get_ComponentMeshTag(), sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
	}

	CloseHandle(hFile);
}

void CMeshTab::OnBnClickedBtLoadDyanmic()
{
	CFileDialog		Dlg(TRUE, L"dmesh", L"*.dmesh"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dmesh", this);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Data\\", szFullPath);

	Dlg.m_pOFN->lpstrInitialDir = szFullPath;


	if (Dlg.DoModal() == IDCANCEL)
		return;


	//기존 네비메쉬의 데이터도 삭제해야 할 수 도 있음
	HANDLE		hFile = CreateFile(Dlg.GetPathName()
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	CString FileName = Dlg.GetFileName();
	PathRemoveExtension((LPWSTR)FileName.operator LPCWSTR());
	UpdateData(TRUE);
	m_strDynamic_LayerTag = /*L"Layer_"*/ FileName;
	UpdateData(FALSE);

	DWORD	dwByte = 0;

	while (true)
	{
		//월드 매트릭스 로드
		_matrix matWorld;
		ReadFile(hFile, &matWorld, sizeof(_matrix), &dwByte, nullptr);


		//파일경로
		_tchar szFilePath[MIN_PATH] = L"";
		ReadFile(hFile, szFilePath, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//파일이름
		_tchar szFileName[MIN_PATH] = L"";
		ReadFile(hFile, szFileName, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//오브젝트 테그
		_tchar szGameObjectTag[MIN_PATH] = L"";
		ReadFile(hFile, szGameObjectTag, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//컴포넌트매쉬태그 읽기
		_tchar szComponentMeshTag[MIN_PATH] = L"";
		ReadFile(hFile, szComponentMeshTag, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);

		if (dwByte == 0)
		{
			break;
		}

		wstring wstrComponentMeshTag = szComponentMeshTag;
		wstring wstrGameObjectTag = szGameObjectTag;
		wstring wstrFileName = szFileName;
		wstring wstrFilePath = szFilePath;

		//	wstrFileName = wstrFileName.sub
		//////////////////////////////////////////////////////////////////////////
		//Create Dynamic Mesh
		CObject_Dynamic* pObject_Dynamic;
		Create_Prototype_Component_Mesh(MESH_DYNAMIC, wstrFilePath.c_str(), wstrFileName.c_str(), wstrComponentMeshTag.c_str());
		Create_Prototype_GameObject(MESH_DYNAMIC, wstrGameObjectTag.c_str());
		if (FAILED(Add_Layer_GameObject(m_strDynamic_LayerTag, wstrGameObjectTag.c_str(), (CGameObject**)&pObject_Dynamic)))
			return;
		//Late SetUp Staic Object
		pObject_Dynamic->SetUp_ObjectInfo(&matWorld, wstrComponentMeshTag.c_str(), this);
		pObject_Dynamic->SetUp_FileInfo(wstrFilePath.c_str(), wstrFileName.c_str(), wstrGameObjectTag.c_str(), wstrComponentMeshTag.c_str());

	}
	CloseHandle(hFile);
}





void CMeshTab::OnBnClickedRadioScale()
{
	UpdateData(TRUE);

	for(int i = 0; i < 3; ++i)
		m_btTransformType[i].SetCheck(FALSE);

	m_btTransformType[0].SetCheck(TRUE);

	UpdateData(FALSE);
}


void CMeshTab::OnBnClickedRadioRotation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	for (int i = 0; i < 3; ++i)
		m_btTransformType[i].SetCheck(FALSE);

	m_btTransformType[1].SetCheck(TRUE);

	UpdateData(FALSE);

}


void CMeshTab::OnBnClickedRadioPosition()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	for (int i = 0; i < 3; ++i)
		m_btTransformType[i].SetCheck(FALSE);

	m_btTransformType[2].SetCheck(TRUE);

	UpdateData(FALSE);

}


void CMeshTab::OnDeltaposSpinScaleX(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pPickedMeshObject == nullptr)
		return;

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);
	if(pNMUpDown->iDelta < 0)
	{
		m_fScaleX += 0.01f;
	}
	else
	{
		m_fScaleX -= 0.01f;
		if (m_fScaleX <= 0.f)
			m_fScaleX = 0.01f;
	}
	UpdateData(FALSE);

	m_pPickedMeshObject->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
}


void CMeshTab::OnDeltaposSpinScaleY(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pPickedMeshObject == nullptr)
		return;

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)
	{
		m_fScaleY += 0.01f;
	}
	else
	{
		m_fScaleY -= 0.01f;
		if (m_fScaleY <= 0.f)
			m_fScaleY = 0.01f;
	}
	UpdateData(FALSE);
	m_pPickedMeshObject->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
}


void CMeshTab::OnDeltaposSpinScaleZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pPickedMeshObject == nullptr)
		return;

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)
	{
		m_fScaleZ += 0.01f;
	}
	else
	{
		m_fScaleZ -= 0.01f;
		if (m_fScaleZ <= 0.f)
			m_fScaleZ = 0.01f;
	}
	UpdateData(FALSE);
	m_pPickedMeshObject->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
}


void CMeshTab::OnDeltaposSpinRotationY(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pPickedMeshObject == nullptr)
		return;

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)
	{
		m_fRotation[1] += D3DXToRadian(30);
		m_pPickedMeshObject->Set_Rotation(0.f, D3DXToRadian(30), 0.f);
	}
	else
	{
		m_fRotation[1] -= D3DXToRadian(30);
		m_pPickedMeshObject->Set_Rotation(0.f, -D3DXToRadian(30), 0.f);
	}
	UpdateData(FALSE);

}


void CMeshTab::OnBnClickedBtTransformApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pPickedMeshObject == nullptr)
		return;

	UpdateData(TRUE);

	m_pPickedMeshObject->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pPickedMeshObject->Set_Position(m_fPositionX, m_fPositionY, m_fPositionZ);

	UpdateData(FALSE);
	
	
}								


void CMeshTab::OnBnClickedBtDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_pPickedMeshObject == nullptr)
		return;

	CManagement::GetInstance()->Remove_Layer_GameObject(SCENE_EDIT, L"Layer_Static", m_pPickedMeshObject);
}





void CMeshTab::OnBnClickedBtLayertag()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_strDynamic_LayerTag == L"")
		return;
	m_strDynamic_LayerTag = L"Layer_" + m_strDynamic_LayerTag;
	UpdateData(FALSE);
}
