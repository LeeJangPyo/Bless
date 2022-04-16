// CameraTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "CameraTab.h"
#include "afxdialogex.h"

#include "Management.h"


#include "Camera_CutScene.h"
#include "Camera_Dynamic.h"

// CCameraTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCameraTab, CDialogEx)

CCameraTab::CCameraTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAMERA, pParent)
	, m_iCount(0)
	, m_iCurrentIndex(-1)
	, m_pCurrentCamera(nullptr)
	, m_editSpeed(0)
	, m_fPosition_X(0)
	, m_fPosition_Y(0)
	, m_fPosition_Z(0)
{
	m_wstrBaseTag = L"CutScene_";
}

CCameraTab::~CCameraTab()
{
}

void CCameraTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CUTSCENE, m_TreeCutSceneList);
	DDX_Control(pDX, IDC_CAMERA_TAG, m_EditCameraTag);
	DDX_Text(pDX, IDC_EDIT_CUTSCENE_SPEED, m_editSpeed);
	DDX_Text(pDX, IDC_EDIT_POSITION_X, m_fPosition_X);
	DDX_Text(pDX, IDC_EDIT_POSITION_Y, m_fPosition_Y);
	DDX_Text(pDX, IDC_EDIT_POSITION_Z, m_fPosition_Z);
}


BEGIN_MESSAGE_MAP(CCameraTab, CDialogEx)
	ON_BN_CLICKED(IDC_CAMERA_SAVE, &CCameraTab::OnBnClickedCameraSave)
	ON_BN_CLICKED(IDC_CAMERA_LOAD, &CCameraTab::OnBnClickedCameraLoad)
	ON_BN_CLICKED(IDC_CAMERA_CREATE, &CCameraTab::OnBnClickedCameraCreate)
	ON_BN_CLICKED(IDC_CAMERA_DELETE, &CCameraTab::OnBnClickedCameraDelete)
	ON_BN_CLICKED(IDC_CAMERA_ADDPOINT, &CCameraTab::OnBnClickedCameraAddpoint)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CUTSCENE, &CCameraTab::OnTvnSelchangedTreeCutscene)
	ON_BN_CLICKED(IDC_CAMERA_APPLY, &CCameraTab::OnBnClickedCameraApply)
	ON_BN_CLICKED(IDC_CANERA_PLAY, &CCameraTab::OnBnClickedCaneraPlay)
	ON_BN_CLICKED(IDC_CAMERA_STOP, &CCameraTab::OnBnClickedCameraStop)
	ON_BN_CLICKED(IDC_CAMERA_SPEED_APPLY, &CCameraTab::OnBnClickedCameraSpeedApply)
END_MESSAGE_MAP()


// CCameraTab 메시지 처리기입니다.


void CCameraTab::OnBnClickedCameraSave()
{
	CFileDialog		Dlg(FALSE, L"scene", L"*.scene"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.scene", this);

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

	CManagement* pManagement = CManagement::GetInstance();
	pManagement->AddRef();

	const list<CGameObject*>* pGameObjectList;
	pGameObjectList = pManagement->Get_Layer_ObjectList(SCENE_EDIT, L"Layer_Camera_CutScene");

	for (auto& pGameObject : *pGameObjectList)
	{
		CCamera_CutScene* pCutScene = dynamic_cast<CCamera_CutScene*>(pGameObject);
		if (pCutScene == nullptr)
			break;
		pCutScene->Save_CutScene(Dlg.GetPathName());
	}
	Safe_Release(pManagement);
}


void CCameraTab::OnBnClickedCameraLoad()
{
	// TODO: Add your control notification handler code here

	CFileDialog		Dlg(TRUE, L"scene", L"*.scene"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.scene", this);

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


	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();

	//Read File
	HANDLE		hFile = CreateFile(Dlg.GetPathName()
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD	dwByte = 0;
	_ulong	dwIndex = 0;

	//MFC Update
 	UpdateData(TRUE);

 	while (true)
	{
		vector<_vec3> vecCamera_Eye;
		vector<_vec3> vecCamera_At;

		_int	iTagLength;
		_tchar	szSceneTag[MIN_PATH];

		ReadFile(hFile, &iTagLength, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, szSceneTag, sizeof(_tchar)*iTagLength, &dwByte, nullptr);

		//Read Speed;
		_float	fSpeed;
		ReadFile(hFile, &fSpeed, sizeof(_float), &dwByte, nullptr);

		UpdateData(TRUE);
		m_editSpeed = fSpeed;
		UpdateData(FALSE);

		//Read Szie
		_int iSize;


 		ReadFile(hFile, &iSize, sizeof(_int), &dwByte, nullptr);
		
		//Read CameraEye
		_vec3 vPosition;
		for(_int i = 0; i<iSize; ++i)
		{
			ReadFile(hFile, &vPosition, sizeof(_vec3), &dwByte, nullptr);
			vecCamera_Eye.push_back(vPosition);
			ReadFile(hFile, &vPosition, sizeof(_vec3), &dwByte, nullptr);
			vecCamera_At.push_back(vPosition);
		}
		if (dwByte == 0)
			break;

		//Input Data;
		// For. CutScene Camera

		pManagement->Add_Layer(SCENE_EDIT, L"Layer_Camera_CutScene", L"GameObject_Camera_CutScene", (CGameObject**)&m_pCurrentCamera);

		//Key
		m_pCurrentCamera->Set_KeyTag(szSceneTag);
		m_pCurrentCamera->Set_Speed(fSpeed);
		//Tree Cotnrol Add Data 
		HTREEITEM hRoot = m_TreeCutSceneList.InsertItem(szSceneTag);

		_tchar szNum[MIN_PATH];

		for (_int i = 0; i < iSize; ++i)
		{
			_itow_s(i, szNum, 10);
			HTREEITEM hChild = m_TreeCutSceneList.InsertItem(szNum, hRoot);

			m_pCurrentCamera->Add_Eye(vecCamera_Eye[i]);
			m_pCurrentCamera->Add_At(vecCamera_At[i]);

			m_TreeCutSceneList.InsertItem(L"Eye", hChild);
			m_TreeCutSceneList.InsertItem(L"At", hChild);
		}

		m_pCurrentCamera->Caculate_Line();
		
		vecCamera_Eye.clear();
		vecCamera_At.clear();

		m_pCurrentCamera->Set_CameraTab(this);

	}

	UpdateData(FALSE);


	Safe_Release(pManagement);

	CloseHandle(hFile);
}


void CCameraTab::OnBnClickedCameraCreate()
{
	// TODO: Add your control notification handler code here
	//LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	//pDevice->AddRef();

	CManagement* pManagement = CManagement::GetInstance();
	pManagement->AddRef();

	pManagement->Add_Layer(SCENE_EDIT, L"Layer_Camera_CutScene", L"GameObject_Camera_CutScene",(CGameObject**)&m_pCurrentCamera);
	
	_tchar	szCount[MIN_PATH];
	_itow_s(m_iCount++, szCount,10);
	wstring wstrTag = m_wstrBaseTag + szCount;

	m_pCurrentCamera->Set_KeyTag(wstrTag);

	HTREEITEM hRoot = m_TreeCutSceneList.InsertItem(wstrTag.c_str());
	HTREEITEM hChild = m_TreeCutSceneList.InsertItem(L"0", hRoot);
	m_TreeCutSceneList.InsertItem(L"Eye", hChild);
	m_TreeCutSceneList.InsertItem(L"At", hChild);

	Safe_Release(pManagement);
}


void CCameraTab::OnBnClickedCameraDelete()
{
	// TODO: Add your control notification handler code here


}
void CCameraTab::OnBnClickedCameraAddpoint()
{
	// TODO: Add your control notification handler code here
	if (m_pCurrentCamera == nullptr)
		return;

	m_pCurrentCamera->Add_Point();
}


void CCameraTab::OnTvnSelchangedTreeCutscene(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	HTREEITEM hItemCur;
	HTREEITEM hItemParent;
	HTREEITEM hItemGrandParent;

	hItemCur = pNMTreeView->itemNew.hItem;
	hItemParent = m_TreeCutSceneList.GetParentItem(hItemCur);
	hItemGrandParent = m_TreeCutSceneList.GetParentItem(hItemParent);

	CString strCur;
	CString strParent;
	CString	strGrandParent;

	strCur = m_TreeCutSceneList.GetItemText(hItemCur);
	strParent = m_TreeCutSceneList.GetItemText(hItemParent);
	strGrandParent = m_TreeCutSceneList.GetItemText(hItemGrandParent);


	if(hItemParent == NULL)
	{
		m_hCurrentTreeItem = hItemCur;

		UpdateData(TRUE);
		m_EditCameraTag.SetWindowTextW(strCur);
		UpdateData(FALSE);
	}
	else
	{
		CManagement* pManagement = CManagement::GetInstance();
		if (pManagement == nullptr)
			return;

		pManagement->AddRef();

		const list<CGameObject*>* pGameObjectList = pManagement->Get_Layer_ObjectList(SCENE_EDIT, L"Layer_Camera_CutScene");

		for (auto& pGameObject : *pGameObjectList)
		{
			CCamera_CutScene* pCamera = dynamic_cast<CCamera_CutScene*>(pGameObject);
			if (strParent == pCamera->Get_Key().c_str())
			{
				if (m_iCurrentIndex != -1)
				{
					m_pCurrentCamera->Set_PickEye(m_iCurrentIndex, false);
					m_pCurrentCamera->Set_PickAt(m_iCurrentIndex, false);
				}

				m_iCurrentIndex = _wtoi((LPCWSTR)strCur);
				m_pCurrentCamera = pCamera;

				m_pCurrentCamera->Set_PickEye(m_iCurrentIndex, true);
				m_pCurrentCamera->Set_PickAt(m_iCurrentIndex, true);
			}

			if (strGrandParent == pCamera->Get_Key().c_str())
			{
				if (m_iCurrentIndex != -1)
				{
					m_pCurrentCamera->Set_PickEye(m_iCurrentIndex, false);
					m_pCurrentCamera->Set_PickAt(m_iCurrentIndex, false);
				}

				m_iCurrentIndex = _wtoi((LPCWSTR)strParent);
				m_pCurrentCamera = pCamera;

				if (strCur == L"Eye")
				{
					m_pCurrentCamera->Set_PickEye(m_iCurrentIndex, true);
				}
				else
				{
					m_pCurrentCamera->Set_PickAt(m_iCurrentIndex, true);
				}
			}
		}

		Safe_Release(pManagement);
	}



}

void CCameraTab::OnBnClickedCameraApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString cstrText;

	UpdateData(TRUE);

	m_EditCameraTag.GetWindowTextW(cstrText);

	UpdateData(FALSE);
	if (cstrText == L"")
		return;

	m_pCurrentCamera->Set_KeyTag(LPCWSTR(cstrText));
	m_TreeCutSceneList.SetItemText(m_hCurrentTreeItem, LPCWSTR(cstrText));
}


void CCameraTab::OnBnClickedCaneraPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();

	CCamera_Dynamic* pCamera_Dynamic = dynamic_cast<CCamera_Dynamic*>(pManagement->Get_GameObject(SCENE_STATIC, L"Layer_Camera_Dynamic"));
	pCamera_Dynamic->Disable();

	m_pCurrentCamera->Enable();

	Safe_Release(pManagement);
}


void CCameraTab::OnBnClickedCameraStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();

	CCamera_Dynamic* pCamera_Dynamic = dynamic_cast<CCamera_Dynamic*>(pManagement->Get_GameObject(SCENE_STATIC, L"Layer_Camera_Dynamic"));
		
	pCamera_Dynamic->Enable();

	m_pCurrentCamera->Disable();

	Safe_Release(pManagement);
}


void CCameraTab::OnBnClickedCameraSpeedApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	UpdateData(TRUE);

	m_pCurrentCamera->Set_Speed(m_editSpeed);

	UpdateData(FALSE);
}

void CCameraTab::Update_CameraPosition(const _vec3 * pPosition)
{
	UpdateData(TRUE);
	m_fPosition_X = pPosition->x;
	m_fPosition_Y = pPosition->y;
	m_fPosition_Z = pPosition->z;
	UpdateData(FALSE);
}
