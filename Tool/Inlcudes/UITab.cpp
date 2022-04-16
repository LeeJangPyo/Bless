// UITab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UITab.h"
#include "afxdialogex.h"

#include <io.h>

#include "Management.h"

#include "UI_Resources_Loader.h"
#include "UI_Frame.h"




// CUITab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUITab, CDialogEx)

CUITab::CUITab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UI, pParent)
	, m_fScaleX(0)
	, m_fScaleY(0)
	, m_hItemCurrent(NULL)
	, m_bIsLButtonDown(false)
	, m_pUIFrame(nullptr)
	, m_pUI(nullptr)
	, m_fPositionX(0)
	, m_fPositionY(0)

	, m_fDepthZ(0)
{
	
}

CUITab::~CUITab()
{
}

void CUITab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UI_FRAME_EDIT, m_editFrameName);
	DDX_Control(pDX, IDC_UI_EDIT, m_editUIName);
	DDX_Control(pDX, IDC_TREE_UI_List, m_treeUIList);
	DDX_Control(pDX, IDC_TREE_UI_TEXTURE_LIST, m_treeTextureList);
	DDX_Text(pDX, IDC_UI_SCALEX_EDIT, m_fScaleX);
	DDX_Text(pDX, IDC_UI_SCALEY_EDIT, m_fScaleY);
	DDX_Text(pDX, IDC_UI_POSITIONX_EDIT, m_fPositionX);
	DDX_Text(pDX, IDC_UI_POSITIONY_EDIT, m_fPositionY);
	DDX_Control(pDX, IDC_SLIDER_SCALE_X, m_sliderScaleX);
	DDX_Control(pDX, IDC_SLIDER_SCALE_Y, m_sliderScaleY);
	DDX_Text(pDX, IDC_UI_DEPTHZ, m_fDepthZ);
}


BEGIN_MESSAGE_MAP(CUITab, CDialogEx)
	ON_BN_CLICKED(IDC_UI_FRAME_CREATE, &CUITab::OnBnClickedUiFrameCreate)
	ON_BN_CLICKED(IDC_UI_CREATE, &CUITab::OnBnClickedUiCreate)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_UI_List, &CUITab::OnTvnSelchangedTreeUiList)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_UI_TEXTURE_LIST, &CUITab::OnNMDblclkTreeUiTextureList)
	ON_BN_CLICKED(IDC_UI_SAVE, &CUITab::OnBnClickedUiSave)
	ON_BN_CLICKED(IDC_UI_LOAD, &CUITab::OnBnClickedUiLoad)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SCALE_X, &CUITab::OnNMReleasedcaptureSliderScaleX)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SCALE_Y, &CUITab::OnNMReleasedcaptureSliderScaleY)
	ON_BN_CLICKED(IDC_BT_TRANSFORM_APPLY, &CUITab::OnBnClickedBtTransformApply)
	ON_BN_CLICKED(IDC_BT_PAREENT_APPLY, &CUITab::OnBnClickedBtPareentApply)
	ON_BN_CLICKED(IDC_UI_DELETE, &CUITab::OnBnClickedUiDelete)
	ON_BN_CLICKED(IDC_UIFrame_CHANGE, &CUITab::OnBnClickedUiframeChange)
END_MESSAGE_MAP()


// CUITab 메시지 처리기입니다.


void CUITab::OnBnClickedUiFrameCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString cstrFrameName;

	m_editFrameName.GetWindowTextW(cstrFrameName);
	if (cstrFrameName == L"")
		return;

	m_treeUIList.InsertItem(cstrFrameName);

	//Add Layer
	CManagement* pManagement = CManagement::GetInstance();
	pManagement->AddRef();

	CUI_Frame*	pUIFrame;

	pManagement->Add_Layer(SCENE_EDIT, L"Layer_UI_Frame", L"GameObject_UI_Frame", (CGameObject**)&pUIFrame);
	pUIFrame->Set_Tag(LPCTSTR(cstrFrameName));
	pUIFrame->Set_UITab(this);

	Safe_Release(pManagement);
}


void CUITab::OnBnClickedUiCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_hItemCurrent == NULL || m_pUIFrame == nullptr)
		return;

	CString cstrUIName;

	m_editUIName.GetWindowTextW(cstrUIName);

	//Insert UI Item
	m_treeUIList.InsertItem(cstrUIName, m_hItemCurrent);

	//Add UI to Frame
	m_pUIFrame->Add_UI((LPCWSTR)cstrUIName);
}


void CUITab::OnTvnSelchangedTreeUiList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;


	HTREEITEM	hItemParent;
	CString		cstrCur;
	CString		cstrParent;

	m_hItemCurrent = pNMTreeView->itemNew.hItem;


	//first Init Pointer
	if(m_pUIFrame != nullptr)
		m_pUIFrame->Set_Select(false);
	//m_pUIFrame = nullptr;
	if(m_pUI != nullptr)
		m_pUI->Set_Select(false);
//	m_pUI = nullptr;
	


	hItemParent = m_treeUIList.GetParentItem(m_hItemCurrent);
	//부모의 아이템이 없으면 프레임을 선택한것
	if(hItemParent == NULL)
	{
		cstrCur = m_treeUIList.GetItemText(m_hItemCurrent);

		const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_EDIT, L"Layer_UI_Frame");
		if (pGameObjectList == nullptr)
			return;

		for(auto& pUIFrame : *pGameObjectList)
		{
			m_pUIFrame = dynamic_cast<CUI_Frame*>(pUIFrame);
			if (m_pUIFrame == nullptr)
				return;

			if (m_pUIFrame->Compare_Tag((LPCWSTR)cstrCur))
			{

				if (m_pUI != nullptr)
					m_pUI->Set_Select(false);

				m_pUIFrame->Set_Select(true);

				const _vec3* pFramePosition = m_pUIFrame->Get_Position();
				Update_EditBox(0.f, 0.f, pFramePosition->x, pFramePosition->y,0.f);
				break;
			}

			m_pUIFrame = nullptr;
		}
	}
	else // UI아이템을 선택한것
	{
		cstrParent = m_treeUIList.GetItemText(hItemParent);

		const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_EDIT, L"Layer_UI_Frame");

		for (auto& pUIFrame : *pGameObjectList)
		{
			m_pUIFrame = dynamic_cast<CUI_Frame*>(pUIFrame);
			if (m_pUIFrame == nullptr)
				return;

			if (m_pUIFrame->Compare_Tag((LPCWSTR)cstrParent))
				break;

			m_pUIFrame = nullptr;
		}
		if (m_pUIFrame == nullptr)
			return;

		cstrCur = m_treeUIList.GetItemText(m_hItemCurrent);

		m_pUIFrame->Set_Select(false);
		m_pUI = m_pUIFrame->Get_UI(LPCWSTR(cstrCur));
		m_pUI->Set_Select(true);

		const _vec3* pUIPosition = m_pUI->Get_Position();
		Update_EditBox(m_pUI->Get_Scale_X(), m_pUI->Get_Scale_Y(), pUIPosition->x, pUIPosition->y,pUIPosition->z);
	}
}



BOOL CUITab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	int * a = new int;

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	pDevice->AddRef();

	CUI_Resources_Loader UI_Loader(pDevice);
	UI_Loader.UI_Resources_Load(L"../Bin/Resources/Texture/UI/*.*");

	Safe_Release(pDevice);

	HTREEITEM hRoot;
	Create_Texture_ItemList(L"../Bin/Resources/Texture/UI/*.*", hRoot);


	//SliderBar Init
	m_sliderScaleX.SetRange(0, 800);
	m_sliderScaleX.SetPos(400);
	m_sliderScaleX.SetTic(10);

	m_sliderScaleY.SetRange(0, 600);	
	m_sliderScaleY.SetPos(300);	
	m_sliderScaleY.SetTic(10);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CUITab::Create_Texture_ItemList(const wstring & wstrFilePath, HTREEITEM& hItem)
{
	_wfinddata_t		FindData;

	wstring		wstrCombinedPath;
	long		handle;
	int			iContinue = 1;

	//wstrPath = L"C:\\Users\\Administrator\\Desktop\\FrmaeWork85_D2D\\Texture\\*.*"
	handle = _wfindfirst(wstrFilePath.c_str(), &FindData);
	if (handle == -1)
		return;

	while (iContinue != -1)
	{
		iContinue = _wfindnext(handle, &FindData);

		if (!lstrcmp(FindData.name, L".") || !lstrcmp(FindData.name, L"..") || iContinue == -1)
			continue;

		wstrCombinedPath = wstrFilePath.substr(0, wstrFilePath.length() - 3) + FindData.name;

		//파일이 있으면


		if (FindData.size != 0)
		{
			_tchar szPathName[MIN_PATH];
			lstrcpy(szPathName, FindData.name);
			PathRemoveExtension(szPathName);
			m_treeTextureList.InsertItem(szPathName, hItem);
		}
		else
		{
			wstring		wstrTag = wstrCombinedPath;
			wstrTag = PathFindFileName(wstrTag.c_str());

			hItem = m_treeTextureList.InsertItem(wstrTag.c_str());
			Create_Texture_ItemList(wstrCombinedPath + L"\\*.*", hItem);
		}
	}

	_findclose(handle);
}


void CUITab::OnNMDblclkTreeUiTextureList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if (m_pUI == nullptr)
		return;

	CPoint tPoint;
	GetCursorPos(&tPoint);
	UINT flag;
	m_treeTextureList.ScreenToClient(&tPoint);

	HTREEITEM hItemCur = m_treeTextureList.HitTest(tPoint, &flag);
	HTREEITEM hItemParent = m_treeTextureList.GetParentItem(hItemCur);
	if (hItemParent == NULL)
		return;

	CString strCur = m_treeTextureList.GetItemText(hItemCur);
	CString strParent = m_treeTextureList.GetItemText(hItemParent);


	_char	szBuffer[MIN_PATH];
	string strNumber;

	WideCharToMultiByte(CP_ACP, 0, strCur, -1, szBuffer, MIN_PATH, NULL, NULL);

	for (_ulong i = 0; i < strlen(szBuffer); i++)
	{
		_char ch = szBuffer[i];
		if (ch >= '0' && ch <= '9')
			strNumber += ch;
	}
	_int number = atoi(strNumber.c_str());

	wstring wstrComponentTag = L"Component_Texture_";
	wstrComponentTag += (LPCWSTR)strParent;

	m_pUI->Replace_Texture_Component(wstrComponentTag.c_str());
	m_pUI->Set_TextureIndex(number);
}


void CUITab::OnBnClickedUiSave()
{
	CFileDialog		Dlg(FALSE, L"ui", L"*.ui"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT 
		, L"*.ui", this,0, FALSE);
	
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
	wstring wstr = Dlg.GetPathName();
	HANDLE		hFile = CreateFile(Dlg.GetPathName()
		, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD	dwByte = 0;

	const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_EDIT, L"Layer_UI_Frame");
	if (pGameObjectList == nullptr)
		return;

	//Size
	_int	iListSize = pGameObjectList->size();
	WriteFile(hFile, &iListSize, sizeof(_int), &dwByte, nullptr);

	for(auto& pGameObject : *pGameObjectList)
	{
		CUI_Frame*	pUIFrame = dynamic_cast<CUI_Frame*>(pGameObject);
		if (pUIFrame == nullptr)
			return;

		pUIFrame->Save_UIFrame_Info(hFile, dwByte);
	}

	CloseHandle(hFile);
}


void CUITab::OnBnClickedUiLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(TRUE, L"ui", L"*.ui"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.ui", this, 0, FALSE);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Data\\", szFullPath);

	Dlg.m_pOFN->lpstrInitialDir = szFullPath;

	if (Dlg.DoModal() == IDCANCEL)
		return;

	HANDLE		hFile = CreateFile(Dlg.GetPathName()
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD	dwByte = 0;
	_ulong	dwIndex = 0;


	//Frame Create
	//Add Layer
	CManagement* pManagement = CManagement::GetInstance();
	pManagement->AddRef();

	//Size
	_int	iListSize;
	ReadFile(hFile, &iListSize, sizeof(_int), &dwByte, nullptr);

	for(_int i =0; i<iListSize; ++i)
	{
		CUI_Frame*	pUIFrame;

		pManagement->Add_Layer(SCENE_EDIT, L"Layer_UI_Frame", L"GameObject_UI_Frame", (CGameObject**)&pUIFrame);
		pUIFrame->Set_UITab(this);

		pUIFrame->Load_UIFrame_Info(hFile, dwByte, m_treeUIList);
	}

	Safe_Release(pManagement);

	CloseHandle(hFile);
}

void CUITab::Update_EditBox(_float fScaleX, _float fScaleY, _float fPositionX, _float fPositionY, _float fDepthZ)
{
	UpdateData(TRUE);

	m_fScaleX = fScaleX;
	m_fScaleY = fScaleY;
	m_fPositionX = fPositionX;
	m_fPositionY = fPositionY;
	m_fDepthZ = fDepthZ;

	UpdateData(FALSE);
}


void CUITab::OnNMReleasedcaptureSliderScaleX(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);
	
	m_fScaleX = (_float)m_sliderScaleX.GetPos();
	m_fScaleY = (_float)m_sliderScaleY.GetPos();
	if (m_pUI != nullptr)
		m_pUI->Set_Scale(m_fScaleX, m_fScaleY);

	UpdateData(FALSE);
}


void CUITab::OnNMReleasedcaptureSliderScaleY(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);

	m_fScaleX = (_float)m_sliderScaleX.GetPos();
	m_fScaleY = (_float)m_sliderScaleY.GetPos();
	if (m_pUI != nullptr)
		m_pUI->Set_Scale(m_fScaleX, m_fScaleY);

	UpdateData(FALSE);
}


void CUITab::OnBnClickedBtTransformApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_sliderScaleX.SetPos((_int)m_fScaleX);
	m_sliderScaleY.SetPos((_int)m_fScaleY);

	if (m_pUI != nullptr)
	{
		m_pUI->Set_Scale(m_fScaleX, m_fScaleY);
		m_pUI->Set_Position(m_fPositionX, m_fPositionY, m_fDepthZ);
	}


	UpdateData(FALSE);
}


void CUITab::OnBnClickedBtPareentApply()
{
	UpdateData(TRUE);

	if (m_pUIFrame != nullptr)
	{
		m_pUIFrame->Set_Position(m_fPositionX, m_fPositionY);
		return;
	}
	UpdateData(FALSE);
}


void CUITab::OnBnClickedUiDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_pUIFrame != nullptr)
	{
		CManagement::GetInstance()->Remove_Layer_GameObject(SCENE_EDIT, L"Layer_UI_Frame", m_pUIFrame);

		m_treeUIList.DeleteItem(m_hItemCurrent);
	}
}


void CUITab::OnBnClickedUiframeChange()
{
	// TODO: Add your control notification handler code here

	if (m_pUIFrame != nullptr)
	{
		CString cstrFrameName;

		m_editFrameName.GetWindowTextW(cstrFrameName);
		if (cstrFrameName == L"")
			return;

		m_pUIFrame->Set_Tag(cstrFrameName);

		HTREEITEM treeItem = m_treeUIList.GetSelectedItem();
		m_treeUIList.SetItemText(treeItem, cstrFrameName);
	}
}
