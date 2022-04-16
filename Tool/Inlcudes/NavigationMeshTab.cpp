// NavigationMeshTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "NavigationMeshTab.h"
#include "afxdialogex.h"
#include "Management.h"

#include "Mesh_Navigation_Controller.h"

// CNavigationMeshTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNavigationMeshTab, CDialogEx)

CNavigationMeshTab::CNavigationMeshTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NAVIMESH, pParent)
	, m_RadioMosueType(0)
	, m_dwOldIndex(0)
{

}

CNavigationMeshTab::~CNavigationMeshTab()
{
}

void CNavigationMeshTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE3, m_TreeNavigationMeshList);
	DDX_Radio(pDX, IDC_RADIO3, m_RadioMosueType);
	

	DDX_Text(pDX, IDC_SCALE_X, m_fSacle[0]);
	DDX_Text(pDX, IDC_SCALE_Y, m_fSacle[1]);
	DDX_Text(pDX, IDC_SCALE_Z, m_fSacle[2]);

	DDX_Text(pDX, IDC_ROTATION_X, m_fRotation[0]);
	DDX_Text(pDX, IDC_ROTATION_Y, m_fRotation[1]);
	DDX_Text(pDX, IDC_ROTATION_Z, m_fRotation[2]);

	DDX_Text(pDX, IDC_POSITION_X, m_fPosition[0]);
	DDX_Text(pDX, IDC_POSITION_Y, m_fPosition[1]);
	DDX_Text(pDX, IDC_POSITION_Z, m_fPosition[2]);
}


BEGIN_MESSAGE_MAP(CNavigationMeshTab, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE3, &CNavigationMeshTab::OnTvnSelchangedNavigationMeshList)
	ON_BN_CLICKED(IDC_BUTTON1, &CNavigationMeshTab::OnBnClickedBtNavigationMeshSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CNavigationMeshTab::OnBnClickedBtNavigationMeshLoad)
	ON_BN_CLICKED(IDC_RADIO3, &CNavigationMeshTab::OnBnClickedInsertPoint)
	ON_BN_CLICKED(IDC_RADIO4, &CNavigationMeshTab::OnBnClickedSelectPoint)
	ON_BN_CLICKED(IDC_RADIO9, &CNavigationMeshTab::OnBnClickedSelectCell)
//	ON_BN_CLICKED(IDC_BT_REMOVE_CELL, &CNavigationMeshTab::OnBnClickedBtRemoveCell)
END_MESSAGE_MAP()


// CNavigationMeshTab 메시지 처리기입니다.


void CNavigationMeshTab::OnTvnSelchangedNavigationMeshList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;


	HTREEITEM hItemParent;
	HTREEITEM hItemCur;

	CString strParent;
	CString strCur;


	hItemCur = pNMTreeView->itemNew.hItem;
	hItemParent = this->m_TreeNavigationMeshList.GetParentItem(hItemCur);

	strParent = m_TreeNavigationMeshList.GetItemText(hItemParent);
	strCur = m_TreeNavigationMeshList.GetItemText(hItemCur);

	//부모가 NULL이면 루트
	if (hItemParent == NULL)
	{
		m_pNaviMesh_Controller->Set_PickCellPoints(false, m_dwOldIndex);

		_ulong dwIndex;
		m_dwOldIndex = dwIndex = wcstoul(LPCWSTR(strCur), nullptr, 10);

		//모든 포인트 선택
		m_pNaviMesh_Controller->Set_PickCellPoints(true, dwIndex);

	}
	else//NULL이 아니면 리프
	{
		m_pNaviMesh_Controller->Set_PickCellPoints(false, m_dwOldIndex);

		//하나 선택
		_ulong dwIndex;
		_ulong dwPoint;

		dwPoint = wcstoul(LPCWSTR(strCur), nullptr, 10);
		m_dwOldIndex = dwIndex = wcstoul(LPCWSTR(strParent), nullptr, 10);


		//모든 포인트 선택
		m_pNaviMesh_Controller->Set_PickCellPoint(true, dwIndex, dwPoint);
	}

}

void CNavigationMeshTab::Add_NaviMeshCell(const Engine::CCell* pCell, unsigned long dwCellIndex)
{
	wchar_t	szCellIndex[MAX_PATH];
	_ultow_s(dwCellIndex, szCellIndex, 10);


	HTREEITEM parent = m_TreeNavigationMeshList.InsertItem(szCellIndex);
	m_TreeNavigationMeshList.InsertItem(L"0", parent);
	m_TreeNavigationMeshList.InsertItem(L"1", parent);
	m_TreeNavigationMeshList.InsertItem(L"2", parent);
}


void CNavigationMeshTab::OnBnClickedBtNavigationMeshSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(FALSE, L"navi", L"*.navi"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.navi", this, 0 , false);

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

	const vector<Engine::CCell*>* pvecCell = m_pNaviMesh_Controller->Get_VectorCell();

	HANDLE		hFile = CreateFile(Dlg.GetPathName()
		, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD	dwByte = 0;

	_int	iSize = pvecCell->size();
	WriteFile(hFile, &iSize, sizeof(_int), &dwByte, nullptr);

	for (auto& iter : *pvecCell)
	{
		for (_ulong i = 0; i < CCell::POINT::POINT_END; ++i)
		{
			//포인터 3개 저장
			_vec3* pPoint = iter->Get_Point(CCell::POINT(i));
			WriteFile(hFile, pPoint, sizeof(_vec3), &dwByte, nullptr);
		}
		const _ulong* pIndex = iter->Get_Index();
		WriteFile(hFile, pIndex, sizeof(_ulong), &dwByte, nullptr);
	}

	CloseHandle(hFile);
}


void CNavigationMeshTab::OnBnClickedBtNavigationMeshLoad()
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CFileDialog		Dlg(TRUE, L"navi", L"*.navi"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.navi", this,0,FALSE);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Data\\", szFullPath);

	Dlg.m_pOFN->lpstrInitialDir = szFullPath;


	if (Dlg.DoModal() == IDCANCEL)
		return;

	//삽입하기전에 초기화
	m_TreeNavigationMeshList.DeleteAllItems();
	//기존 네비메쉬의 데이터도 삭제해야 할 수 도 있음


	HANDLE		hFile = CreateFile(Dlg.GetPathName()
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD	dwByte = 0;

	_int	iSize;
	ReadFile(hFile, &iSize, sizeof(_int), &dwByte, nullptr);

	for(_int i = 0 ; i < iSize; ++i)
	{
		//포인터 3개 저장 배열
		_vec3 vPoint[Engine::CCell::POINT_END];

		ReadFile(hFile, &vPoint[Engine::CCell::POINT_A], sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vPoint[Engine::CCell::POINT_B], sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vPoint[Engine::CCell::POINT_C], sizeof(_vec3), &dwByte, nullptr);

		_ulong dwIndex;
		ReadFile(hFile, &dwIndex, sizeof(_ulong), &dwByte, nullptr);

		m_pNaviMesh_Controller->Add_Cell(vPoint);
	}

	//셀리스트 추가
	const vector<Engine::CCell*>* pvecCell = m_pNaviMesh_Controller->Get_VectorCell();
	for (auto& iter : *pvecCell)
	{
		Add_NaviMeshCell(iter, *iter->Get_Index());
	}

	CloseHandle(hFile);
	UpdateData(FALSE);
}


BOOL CNavigationMeshTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//네비메시 컴포넌트 
	m_pNaviMesh_Controller = dynamic_cast<CMesh_Navigation_Controller*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_EDIT, L"Layer_NaviMesh_Controller"));
	if (m_pNaviMesh_Controller == nullptr)
		return FALSE;

	m_pNaviMesh_Controller->Set_MeshTap(this);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CNavigationMeshTab::OnBnClickedInsertPoint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pNaviMesh_Controller->Set_MouseSelectType(CMesh_Navigation_Controller::INSERT_POINT);
}


void CNavigationMeshTab::OnBnClickedSelectPoint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pNaviMesh_Controller->Set_MouseSelectType(CMesh_Navigation_Controller::SELECT_POINT);
}


void CNavigationMeshTab::OnBnClickedSelectCell()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pNaviMesh_Controller->Set_MouseSelectType(CMesh_Navigation_Controller::SELECT_CELL);
}

void CNavigationMeshTab::Update_Transform(DATATYPE eDataType, const D3DXVECTOR3* pTransform)
{
	UpdateData(TRUE);

	switch (eDataType)
	{
	case CNavigationMeshTab::DATA_SCALE:
		m_fSacle[0] = pTransform->x;
		m_fSacle[1] = pTransform->y;
		m_fSacle[2] = pTransform->z;
		break;
	case CNavigationMeshTab::DATA_POSITION:
		m_fPosition[0] = pTransform->x;
		m_fPosition[1] = pTransform->y;
		m_fPosition[2] = pTransform->z;
		break;
	case CNavigationMeshTab::DATA_ROTATION:
		m_fRotation[0] = pTransform->x;
		m_fRotation[1] = pTransform->y;
		m_fRotation[2] = pTransform->z;
		break;
	default:
		break;
	}

	UpdateData(FALSE);
}

void CNavigationMeshTab::Get_Trasnform(DATATYPE eDataTyope, D3DXVECTOR3* pTransform)
{
	switch (eDataTyope)
	{
	case CNavigationMeshTab::DATA_SCALE:
		pTransform->x = m_fSacle[0];
		pTransform->y = m_fSacle[1];
		pTransform->z = m_fSacle[2];
		break;
	case CNavigationMeshTab::DATA_POSITION:
		pTransform->x = m_fPosition[0];
		pTransform->y = m_fPosition[1];
		pTransform->z = m_fPosition[2];
		break;
	case CNavigationMeshTab::DATA_ROTATION:
		pTransform->x = m_fRotation[0];
		pTransform->y = m_fRotation[1];
		pTransform->z = m_fRotation[2];
		break;
	default:
		break;
	}
}


void CNavigationMeshTab::OnBnClickedBtRemoveCell()
{
	
}
