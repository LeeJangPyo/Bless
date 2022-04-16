// AnimationTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "AnimationTab.h"
#include "afxdialogex.h"

#include "Management.h"

#include "Object_Equipment.h"
#include "Object_Dynamic.h"

// CAnimationTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimationTab, CDialogEx)

CAnimationTab::CAnimationTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANIMATION, pParent)
	, m_fColliderX(0)
	, m_fColliderY(0)
	, m_fColliderZ(0)
	, m_fRadius(3.f)
	, m_pObject_Dynamic(nullptr)
	, m_pObject_Equipment(nullptr)
{

}

CAnimationTab::~CAnimationTab()
{
}

void CAnimationTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_AnimationListBox);
	DDX_Control(pDX, IDC_MESH_FRAME_LIST, m_TreeMeshFrameList);
	DDX_Text(pDX, IDC_EDIT_COL_POS_X, m_fColliderX);
	DDX_Text(pDX, IDC_EDIT_COL_POS_Y, m_fColliderY);
	DDX_Text(pDX, IDC_EDIT_COL_POS_Z, m_fColliderZ);
	DDX_Text(pDX, IDC_EDIT_COL_RADIUS, m_fRadius);
	DDX_Control(pDX, IDC_COL_LIST, m_ColliderList);
	DDX_Control(pDX, IDC_EDIT_COLLIDER_TAG, m_editColliderTag);
}


BEGIN_MESSAGE_MAP(CAnimationTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimationTab::OnBnClickedLoadDynamicMesh)
	ON_BN_CLICKED(IDC_BUTTON6, &CAnimationTab::OnBnClickedDeleteDynamicMesh)
	ON_BN_CLICKED(IDC_BUTTON7, &CAnimationTab::OnBnClickedLoadEquipmentObject)
	ON_LBN_SELCHANGE(IDC_LIST1, &CAnimationTab::OnLbnSelchangePlayAnimationList)
	ON_BN_CLICKED(IDC_BUTTON9, &CAnimationTab::OnBnClickedOpenUnitTool)
	ON_BN_CLICKED(IDC_BUTTON8, &CAnimationTab::OnBnClickedDeleteEquipmentObject)
	ON_BN_CLICKED(IDC_BT_CREATE_COL, &CAnimationTab::OnBnClickedBtCreateCol)
	ON_BN_CLICKED(IDC_BT_DELETE_COL, &CAnimationTab::OnBnClickedBtDeleteCol)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COL_POS_X, &CAnimationTab::OnDeltaposSpinColPosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COL_POS_Y, &CAnimationTab::OnDeltaposSpinColPosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COL_POS_Z, &CAnimationTab::OnDeltaposSpinColPosZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COL_RADIUS, &CAnimationTab::OnDeltaposSpinColRadius)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MESH_FRAME_LIST, &CAnimationTab::OnTvnSelchangedMeshFrameList)
	ON_BN_CLICKED(IDC_BT_SAVE, &CAnimationTab::OnBnClickedBtSave)
	ON_BN_CLICKED(IDC_BT_CREATE_LOAD, &CAnimationTab::OnBnClickedBtCreateLoad)
END_MESSAGE_MAP()


// CAnimationTab 메시지 처리기입니다.

void CAnimationTab::InsertMeshFrameList(HTREEITEM hParentItem, D3DXFRAME* pFrame)
{

	if (nullptr != pFrame->pFrameSibling)
	{
		_tchar	szFrameName[MIN_PATH] = L"";

		//와이드 케릭터로 변환
		MultiByteToWideChar(CP_ACP, 0, pFrame->Name, strlen(pFrame->Name), szFrameName, MIN_PATH);

		HTREEITEM hParent = m_TreeMeshFrameList.InsertItem(szFrameName, hParentItem);
		InsertMeshFrameList(hParent, pFrame->pFrameSibling);
	}

	if (nullptr != pFrame->pFrameFirstChild)
	{
		_tchar	szFrameName[MIN_PATH] = L"";

		//와이드 케릭터로 변환
		MultiByteToWideChar(CP_ACP, 0, pFrame->Name, strlen(pFrame->Name), szFrameName, MIN_PATH);

		HTREEITEM hParent = m_TreeMeshFrameList.InsertItem(szFrameName, hParentItem);
		InsertMeshFrameList(hParent, pFrame->pFrameFirstChild);
	}
	m_TreeMeshFrameList.Expand(hParentItem, TVE_EXPAND);

}


void CAnimationTab::OnBnClickedDeleteDynamicMesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CManagement::GetInstance()->Remove_Layer_AllObject(SCENE_EDIT, L"Layer_Dynamic");
	m_TreeMeshFrameList.DeleteAllItems();
	m_AnimationListBox.ResetContent();
	
}


void CAnimationTab::OnBnClickedLoadEquipmentObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(TRUE, L"X", L"*.X"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.X", this);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Mesh\\StaticMesh\\", szFullPath);


	Dlg.m_pOFN->lpstrInitialDir = szFullPath;

	if (Dlg.DoModal() == IDCANCEL)
		return;

	lstrcpy(szFullPath, Dlg.GetPathName());
	PathRemoveFileSpecW(szFullPath);
	lstrcat(szFullPath, L"\\");

	wstring	wstrFileName = Dlg.GetFileName();
	wstrFileName = wstrFileName.substr(0, wstrFileName.length() - 2);
	m_wstrEquipmentMeshTag = L"Component_Mesh_Static_" + wstrFileName;
	wstring wstrGameObjectTag = L"GameObject_Object_Dynamic_" + wstrFileName;

	// Static Mesh Add to Layer
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();


	LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	if (pDevice == nullptr)
	{
		Safe_Release(pManagement);
		return;
	}
	pDevice->AddRef();
	//Static Mesh Component Load
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_EDIT, m_wstrEquipmentMeshTag.c_str(),
		CMesh_Static::Create(pDevice, szFullPath, Dlg.GetFileName()))))
	{

	}
	//Static Mesh Create GameObject Porotype
	if (FAILED(pManagement->Add_Prototype_GameObject(wstrGameObjectTag.c_str(), CObject_Equipment::Create(pDevice))))
	{
		Safe_Release(pDevice);
		Safe_Release(pManagement);
		return;
	}

	//Static Mesh Create 
	if (FAILED(pManagement->Add_Layer(SCENE_EDIT, L"Layer_Static", wstrGameObjectTag.c_str(), (CGameObject**)&m_pObject_Equipment)))
	{
		Safe_Release(pDevice);
		Safe_Release(pManagement);
		return;
	}
	m_pObject_Equipment->SetUp_ObjectInfo(m_wstrEquipmentMeshTag.c_str());


	Safe_Release(pDevice);
	Safe_Release(pManagement);
}



void CAnimationTab::OnBnClickedLoadDynamicMesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(TRUE, L"X", L"*.X"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.X", this,0,false);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Mesh\\DynamicMesh\\", szFullPath);

	Dlg.m_pOFN->lpstrInitialDir = szFullPath;

	if (Dlg.DoModal() == IDCANCEL)
		return;

	
	lstrcpy(szFullPath, Dlg.GetPathName());
	PathRemoveFileSpecW(szFullPath);
	lstrcat(szFullPath, L"\\");

	wstring	wstrFileName = Dlg.GetFileName();
	wstrFileName = wstrFileName.substr(0, wstrFileName.length() - 2);
	m_wstrDynamicMeshTag = L"Component_Mesh_Dynamic_" + wstrFileName;
	wstring wstrGameObjectTag = L"GameObject_Object_Dynamic_" + wstrFileName;

	// Dynamic Mesh Add to Layer
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();


	LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	if (pDevice == nullptr)
	{
		Safe_Release(pManagement);
		return;
	}
	pDevice->AddRef();
	

	//Dynamic Mesh Component Load
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_EDIT, m_wstrDynamicMeshTag.c_str(),
		CMesh_Dynamic::Create(pDevice, szFullPath, Dlg.GetFileName()))))
	{

	}
	//Dynamic Mesh Create GameObject Porotype
	if (FAILED(pManagement->Add_Prototype_GameObject(wstrGameObjectTag.c_str(), CObject_Dynamic::Create(pDevice))))
	{
		Safe_Release(pDevice);
		Safe_Release(pManagement);
		return;
	}

	//Dynamic Mesh Create 
	if (FAILED(pManagement->Add_Layer(SCENE_EDIT, L"Layer_Dynamic", wstrGameObjectTag.c_str(), (CGameObject**)&m_pObject_Dynamic)))
	{
		Safe_Release(pDevice);
		Safe_Release(pManagement);
		return;
	}
	m_pObject_Dynamic->SetUp_ObjectInfo(m_wstrDynamicMeshTag.c_str());


	Safe_Release(pDevice);
	Safe_Release(pManagement);



	//After Dyanmaic Mesh Load, Insert Animation Set List
	m_iMaxAnimation = m_pObject_Dynamic->Get_MaxAnimation();

	for (unsigned int i = 0; i < m_iMaxAnimation; ++i)
	{
		TCHAR	szIndex[128];
		_itot_s(i, szIndex, 10);
		m_AnimationListBox.InsertString(i, szIndex);
	}

	//After Dyanmaic Mesh Load, Insert Frame Set List
	D3DXFRAME*	pRootFrame = m_pObject_Dynamic->Get_RootFrame();


	HTREEITEM	hRootItem;
	hRootItem = m_TreeMeshFrameList.InsertItem(L"RootFrame");

	if (pRootFrame->pFrameSibling != nullptr)
		InsertMeshFrameList(hRootItem, pRootFrame->pFrameSibling);

	if (pRootFrame->pFrameFirstChild != nullptr)
		InsertMeshFrameList(hRootItem, pRootFrame->pFrameFirstChild);

	m_TreeMeshFrameList.Expand(hRootItem, TVE_EXPAND);



}
void CAnimationTab::OnLbnSelchangePlayAnimationList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iIndex = m_AnimationListBox.GetCurSel();
	m_pObject_Dynamic->Set_Animation(iIndex);
	
}


//
//void CAnimationTab::OnNMDblclkFrameList(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
////	*pResult = 0;
////
////	CPoint tPoint;
////	GetCursorPos(&tPoint);
////	UINT flag;
////	m_TreeMeshFrameList.ScreenToClient(&tPoint);
////
////	HTREEITEM hItemCur = m_TreeMeshFrameList.HitTest(tPoint, &flag);
////	
////	m_cstrCurrentFrame = m_TreeMeshFrameList.GetItemText(hItemCur);
//////	if (strCur.Compare(L"RootFrame"))
//////		return;
////
////	m_pObject_Equipment->Set_ParentMatrix(L"Layer_Dynamic", m_cstrCurrentFrame);
////
////	m_pObject_Dynamic->Change_Collider_Parent(m_cstrCurrentColliderTag, m_cstrCurrentFrame,m_fRadius);
//
//}


void CAnimationTab::OnTvnSelchangedMeshFrameList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	HTREEITEM hItemCur;

	hItemCur = pNMTreeView->itemNew.hItem;

	m_cstrCurrentFrame = m_TreeMeshFrameList.GetItemText(hItemCur);
	if (!m_cstrCurrentFrame.Compare(L"RootFrame"))
		return;

	//아이템 이동
	if(m_pObject_Equipment != nullptr)
		m_pObject_Equipment->Set_ParentMatrix(L"Layer_Dynamic", m_cstrCurrentFrame);

	//충돌체 이동
	m_pObject_Dynamic->Change_Collider_Parent(m_cstrCurrentColliderTag, m_cstrCurrentFrame);
}


void CAnimationTab::OnBnClickedOpenUnitTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_UnitTool.GetSafeHwnd() == NULL)
		m_UnitTool.Create(IDD_UNIT);

	m_UnitTool.ShowWindow(SW_SHOW);
}


void CAnimationTab::OnBnClickedDeleteEquipmentObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CManagement::GetInstance()->Remove_Layer_AllObject(SCENE_EDIT, L"Layer_Static");
	
}


void CAnimationTab::OnBnClickedBtCreateCol()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pObject_Dynamic == nullptr)
		return;
	
	m_editColliderTag.GetWindowTextW(m_cstrCurrentColliderTag);
	if (m_cstrCurrentColliderTag == L"")
		return;
	
	//콜라이더 생성
	m_pObject_Dynamic->Create_Collider(m_cstrCurrentColliderTag,m_cstrCurrentFrame, m_fRadius, &m_pCurrentCollider);

	//리스트 추가
	m_ColliderList.AddString(m_cstrCurrentColliderTag);
}


void CAnimationTab::OnBnClickedBtDeleteCol()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimationTab::OnDeltaposSpinColPosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		m_fColliderX += 0.5f;
	}
	else
	{
		m_fColliderX -= 0.5f;
	}
	UpdateData(FALSE);

	m_pObject_Dynamic->Set_ColliderInfo(m_cstrCurrentColliderTag, m_fColliderX, m_fColliderY, m_fColliderZ, m_fRadius);
}


void CAnimationTab::OnDeltaposSpinColPosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		m_fColliderY += 0.5f;
	}
	else
	{
		m_fColliderY -= 0.5f;
	}
	UpdateData(FALSE);
	m_pObject_Dynamic->Set_ColliderInfo(m_cstrCurrentColliderTag, m_fColliderX, m_fColliderY, m_fColliderZ, m_fRadius);
}


void CAnimationTab::OnDeltaposSpinColPosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;


	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		m_fColliderZ += 0.5f;
	}
	else
	{
		m_fColliderZ -= 0.5f;
	}
	UpdateData(FALSE);
	m_pObject_Dynamic->Set_ColliderInfo(m_cstrCurrentColliderTag, m_fColliderX, m_fColliderY, m_fColliderZ, m_fRadius);
}


void CAnimationTab::OnDeltaposSpinColRadius(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		m_fRadius += 0.5f;
	}
	else
	{
		m_fRadius -= 0.5f;
	}
	UpdateData(FALSE);
	m_pObject_Dynamic->Set_ColliderInfo(m_cstrCurrentColliderTag, m_fColliderX, m_fColliderY, m_fColliderZ, m_fRadius);
}



void CAnimationTab::OnBnClickedBtSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(FALSE, L"col", L"*.col"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.col", this, 0, FALSE);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Data\\", szFullPath);

	Dlg.m_pOFN->lpstrInitialDir = szFullPath;

	if (Dlg.DoModal() == IDCANCEL)
		return;

	//Create File
	HANDLE		hFile = CreateFile(Dlg.GetPathName()
		, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD	dwByte = 0;

	m_pObject_Dynamic->Save_ColliderInfo(hFile, dwByte);

	CloseHandle(hFile);
}


void CAnimationTab::OnBnClickedBtCreateLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(TRUE, L"col", L"*.col"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.col", this, 0, FALSE);

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

	m_pObject_Dynamic->Load_ColliderInfo(hFile, dwByte);
	
	CloseHandle(hFile);
}
