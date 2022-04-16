// LightTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "LightTab.h"
#include "afxdialogex.h"

#include "Management.h"
#include "Debug_Spehre.h"


// CLightTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLightTab, CDialogEx)

CLightTab::CLightTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LIGHT, pParent)
	, m_fPositionX(0)
	, m_fPositionY(0)
	, m_fPositionZ(0)
	, m_fDiffuseR(0.7f)
	, m_fDiffuseG(0.7f)
	, m_fDiffuseB(0.7f)
	, m_fDiffuseA(0.7f)
	, m_fAmbientR(0.4f)
	, m_fAmbientG(0.4f)
	, m_fAmbientB(0.4f)
	, m_fAmbientA(1.f)
	, m_fSpecularR(0.7f)
	, m_fSpecularG(0.7f)
	, m_fSpecularB(0.7f)
	, m_fSpecularA(0.7f)
	, m_fRange(5.f)
	, m_iCurrentIndex(0)
	, m_iLightInfoSize(0)
{

}

CLightTab::~CLightTab()
{
}

void CLightTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIGHT_LIST, m_LightList);
	DDX_Text(pDX, IDC_EDIT_LIGHT_POSITION_X, m_fPositionX);
	DDX_Text(pDX, IDC_EDIT_LIGHT_POSITION_Y, m_fPositionY);
	DDX_Text(pDX, IDC_EDIT_LIGHT_POSITION_Z, m_fPositionZ);
	DDX_Text(pDX, IDC_EDIT_LIGHT_DIFFUSE_R, m_fDiffuseR);
	DDX_Text(pDX, IDC_EDIT_LIGHT_DIFFUSE_G, m_fDiffuseG);
	DDX_Text(pDX, IDC_EDIT_LIGHT_DIFFUSE_B, m_fDiffuseB);
	DDX_Text(pDX, IDC_EDIT_LIGHT_DIFFUSE_A, m_fDiffuseA);
	DDX_Text(pDX, IDC_EDIT_LIGHT_AMBIENT_R, m_fAmbientR);
	DDX_Text(pDX, IDC_EDIT_LIGHT_AMBIENT_G, m_fAmbientG);
	DDX_Text(pDX, IDC_EDIT_LIGHT_AMBIENT_B, m_fAmbientB);
	DDX_Text(pDX, IDC_EDIT_LIGHT_AMBIENT_A, m_fAmbientA);
	DDX_Text(pDX, IDC_EDIT_LIGHT_SPECULAR_R, m_fSpecularR);
	DDX_Text(pDX, IDC_EDIT_LIGHT_SPECULAR_G, m_fSpecularG);
	DDX_Text(pDX, IDC_EDIT_LIGHT_SPECULAR_B, m_fSpecularB);
	DDX_Text(pDX, IDC_EDIT_LIGHT_SPECULAR_A, m_fSpecularA);
	DDX_Text(pDX, IDC_EDIT_LIGHT_RAGNE, m_fRange);
}

void CLightTab::Set_Position(const _vec3* vPosition)
{
	UpdateData(TRUE);
	m_fPositionX = vPosition->x;
	m_fPositionY = vPosition->y;
	m_fPositionZ = vPosition->z;
	UpdateData(FALSE);

	m_pDebug_Sphere->Set_Position(vPosition);
}


BEGIN_MESSAGE_MAP(CLightTab, CDialogEx)
	ON_BN_CLICKED(IDC_BT_LIGHT_APPLY, &CLightTab::OnBnClickedBtLightApply)
	ON_BN_CLICKED(IDC_LIGHT_SAVE, &CLightTab::OnBnClickedLightSave)
	ON_BN_CLICKED(IDC_LIGHT_LOAD, &CLightTab::OnBnClickedLightLoad)
	ON_LBN_SELCHANGE(IDC_LIGHT_LIST, &CLightTab::OnLbnSelchangeLightList)
	ON_BN_CLICKED(IDC_BT_LIGHT_CREATE, &CLightTab::OnBnClickedBtLightCreate)
	ON_BN_CLICKED(IDC_BT_LIGHT_DELETE, &CLightTab::OnBnClickedBtLightDelete)
END_MESSAGE_MAP()


// CLightTab 메시지 처리기입니다.



void CLightTab::OnBnClickedBtLightCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Position = _vec3(m_fPositionX, m_fPositionY, m_fPositionZ);
	LightInfo.Range = m_fRange;
	LightInfo.Diffuse = D3DXCOLOR(m_fDiffuseR, m_fDiffuseG, m_fDiffuseB, m_fDiffuseA);
	LightInfo.Ambient = D3DXCOLOR(m_fAmbientR, m_fAmbientG, m_fAmbientB, m_fAmbientA);
	LightInfo.Specular = D3DXCOLOR(m_fSpecularR, m_fSpecularG, m_fSpecularB, m_fSpecularA);


	if (FAILED(CManagement::GetInstance()->Add_Layer
			(SCENE_EDIT, L"Layer_Debug_Sphere", L"GameObject_Debug_Sphere",(CGameObject**)&m_pDebug_Sphere)))
		return;
	m_pDebug_Sphere->Set_Activate(true);
	m_pDebug_Sphere->Set_Pick(false);
	m_pDebug_Sphere->Set_Position(&_vec3(0.f, 0.f, 0.f));

	m_vecLightInfo.push_back(LightInfo);

	_tchar szNum[128];
	_itow_s(m_iLightInfoSize++, szNum, 10);
	m_LightList.AddString(szNum);
	
}


void CLightTab::OnBnClickedBtLightApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (m_iCurrentIndex > m_vecLightInfo.size() || m_iCurrentIndex < 0)
		return;

	UpdateData(TRUE);
	m_vecLightInfo[m_iCurrentIndex].Position = _vec3(m_fPositionX, m_fPositionY, m_fPositionZ);
	m_vecLightInfo[m_iCurrentIndex].Range = m_fRange;
	m_vecLightInfo[m_iCurrentIndex].Diffuse = D3DXCOLOR(m_fDiffuseR, m_fDiffuseG, m_fDiffuseB, m_fDiffuseA);
	m_vecLightInfo[m_iCurrentIndex].Ambient = D3DXCOLOR(m_fAmbientR, m_fAmbientG, m_fAmbientB, m_fAmbientA);
	m_vecLightInfo[m_iCurrentIndex].Specular = D3DXCOLOR(m_fSpecularR, m_fSpecularG, m_fSpecularB, m_fSpecularA);
	UpdateData(FALSE);
	
	m_pDebug_Sphere->Set_Position(&_vec3(m_fPositionX, m_fPositionY, m_fPositionZ));
}


void CLightTab::OnBnClickedLightSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(FALSE, L"light", L"*.light"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.light", this, 0, FALSE);

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

	for (auto& LightInfo : m_vecLightInfo)
	{
		WriteFile(hFile, &LightInfo, sizeof(D3DLIGHT9), &dwByte, nullptr);
	}

	CloseHandle(hFile);
}


void CLightTab::OnBnClickedLightLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(TRUE, L"light", L"*.light"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.light", this, 0, FALSE);

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


	while (true)
	{
		D3DLIGHT9 LightInfo;
		ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));
		ReadFile(hFile, &LightInfo, sizeof(D3DLIGHT9), &dwByte, nullptr);

	
		if (dwByte == 0)
			break;

		m_vecLightInfo.push_back(LightInfo);

		_tchar szNum[128];
		_itow_s(m_iLightInfoSize++, szNum, 10);
		m_LightList.AddString(szNum);

		if (FAILED(CManagement::GetInstance()->Add_Layer
		(SCENE_EDIT, L"Layer_Debug_Sphere", L"GameObject_Debug_Sphere", (CGameObject**)&m_pDebug_Sphere)))
			return;
		m_pDebug_Sphere->Set_Activate(true);
		m_pDebug_Sphere->Set_Pick(true);
		m_pDebug_Sphere->Set_Position(&_vec3(LightInfo.Position.x, LightInfo.Position.y, LightInfo.Position.z));
	}



	CloseHandle(hFile);

}


void CLightTab::OnLbnSelchangeLightList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_iCurrentIndex = m_LightList.GetCurSel();

	UpdateData(TRUE);
	m_fPositionX = m_vecLightInfo[m_iCurrentIndex].Position.x;
	m_fPositionY = m_vecLightInfo[m_iCurrentIndex].Position.y;
	m_fPositionZ = m_vecLightInfo[m_iCurrentIndex].Position.z;

	m_fRange = m_vecLightInfo[m_iCurrentIndex].Range;

	m_fDiffuseR = m_vecLightInfo[m_iCurrentIndex].Diffuse.r;
	m_fDiffuseG = m_vecLightInfo[m_iCurrentIndex].Diffuse.g;
	m_fDiffuseB = m_vecLightInfo[m_iCurrentIndex].Diffuse.b;
	m_fDiffuseA = m_vecLightInfo[m_iCurrentIndex].Diffuse.a;

	m_fAmbientR = m_vecLightInfo[m_iCurrentIndex].Ambient.r;
	m_fAmbientG = m_vecLightInfo[m_iCurrentIndex].Ambient.g;
	m_fAmbientB = m_vecLightInfo[m_iCurrentIndex].Ambient.b;
	m_fAmbientA = m_vecLightInfo[m_iCurrentIndex].Ambient.a;

	m_fSpecularR = m_vecLightInfo[m_iCurrentIndex].Specular.r;
	m_fSpecularG = m_vecLightInfo[m_iCurrentIndex].Specular.g;
	m_fSpecularB = m_vecLightInfo[m_iCurrentIndex].Specular.b;
	m_fSpecularA = m_vecLightInfo[m_iCurrentIndex].Specular.a;

	UpdateData(FALSE);
	m_pDebug_Sphere->Set_Pick(false);

	m_pDebug_Sphere = dynamic_cast<CDebug_Spehre*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_EDIT, L"Layer_Debug_Sphere", m_iCurrentIndex));
	m_pDebug_Sphere->Set_Position(&_vec3(m_fPositionX, m_fPositionY, m_fPositionZ));
	m_pDebug_Sphere->Set_Pick(true);
	m_pDebug_Sphere->Set_Activate(true);

}



void CLightTab::OnBnClickedBtLightDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_iCurrentIndex = m_LightList.GetCurSel();

	m_LightList.DeleteString(m_iCurrentIndex);

	_int iCount = m_LightList.GetCount();
	if (iCount)
		m_LightList.SetCurSel(m_iCurrentIndex - (m_iCurrentIndex == iCount));

	auto	iter = m_vecLightInfo.begin();

	for (size_t i = 0; i < m_iCurrentIndex; ++i)
		++iter;
	m_vecLightInfo.erase(iter);

}


BOOL CLightTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_pDebug_Sphere = dynamic_cast<CDebug_Spehre*>(CManagement::GetInstance()->Get_GameObject(SCENE_EDIT, L"Layer_Debug_Sphere"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLightTab::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_vecLightInfo.clear();

	CDialogEx::PostNcDestroy();
}
