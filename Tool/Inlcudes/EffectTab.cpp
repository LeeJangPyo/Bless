// EffectTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "EffectTab.h"
#include "afxdialogex.h"

#include "Management.h"


#include "Effect.h"
#include "EffectGroup.h"


#include "Mesh_Static.h"

#include "UI_Resources_Loader.h"


// CEffectTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEffectTab, CDialogEx)

CEffectTab::CEffectTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EFFECT, pParent)
	, m_fPositionX(0)
	, m_fPositionY(0)
	, m_fPositionZ(0)
	, m_fScaleX(1)
	, m_fScaleY(1)
	, m_fScaleZ(1)
	, m_fRotationX(0)
	, m_fRotationY(0)
	, m_fRotationZ(0)
	, m_fUSpeed(0)
	, m_fVSpeed(0)
	, m_fStart_Position_X(0)
	, m_fStart_Position_Y(0)
	, m_fStart_Position_Z(0)
	, m_fEnd_Position_X(0)
	, m_fEnd_Position_Y(0)
	, m_fEnd_Position_Z(0)
	, m_fRotation_Start_X(0)
	, m_fRotation_Start_Y(0)
	, m_fRotation_Start_Z(0)
	, m_fRotation_End_X(0)
	, m_fRotation_End_Y(0)
	, m_fRotation_End_Z(0)
	, m_fScale_Start_X(1)
	, m_fScale_Start_Y(1)
	, m_fScale_Start_Z(1)
	, m_fScale_End_X(1)
	, m_fScale_End_Y(1)
	, m_fScale_End_Z(1)
	, m_strTag(_T(""))
	, m_fFadeOut(0)
	, m_fFadeIn(0)
	, m_iParticleNum(100)
	, m_strGroupName(_T(""))
	, m_fGroupEffect_LifeTime(1)
	, m_fStartTime(0)
	, m_fEndTime(0)
	, m_fMaxTheta(0)
	, m_fMinTheta(0)
	, m_fMaxPhi(0)
	, m_fMinPhi(0)
	, m_fInit_Speed(0)
	, m_fSpeed_Range(0)
	, m_fInit_LifeTime(0)
	, m_fLifeTime_Range(0)
	, m_fInit_Acc_X(0)
	, m_fInit_Acc_Y(0)
	, m_fInit_Acc_Z(0)
	, m_fFrame_Const(0)
	, m_fAlpha(1)
	, m_iAlpha_Width(1)
	, m_iAlpha_Height(1)
	, m_iBase_Width(1)
	, m_iBase_Height(1)
	, m_fInit_Alpha(0)
	, m_strMeshTag(_T(""))
{
		
}

CEffectTab::~CEffectTab()
{
}

void CEffectTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POSITION_X, m_fPositionX);
	DDX_Text(pDX, IDC_POSITION_Y, m_fPositionY);
	DDX_Text(pDX, IDC_POSITION_Z, m_fPositionZ);
	DDX_Text(pDX, IDC_SCALE_X, m_fScaleX);
	DDX_Text(pDX, IDC_SCALE_Y, m_fScaleY);
	DDX_Text(pDX, IDC_SCALE_Z, m_fScaleZ);
	DDX_Text(pDX, IDC_ROTATION_X, m_fRotationX);
	DDX_Text(pDX, IDC_ROTATION_Y, m_fRotationY);
	DDX_Text(pDX, IDC_ROTATION_Z, m_fRotationZ);
	DDX_Control(pDX, IDC_CHECK_PASS0, m_bt_Default);
	DDX_Control(pDX, IDC_CHECK_PASS1, m_bt_Alpha);
	DDX_Control(pDX, IDC_CHECK_PASS2, m_bt_TwoTexutre);
	DDX_Text(pDX, IDC_EDIT_USPEED, m_fUSpeed);
	DDX_Text(pDX, IDC_EDIT_VSPEED, m_fVSpeed);

	DDX_Text(pDX, IDC_EDIT_THETA, m_fMaxTheta);
	DDX_Text(pDX, IDC_EDIT_MIN_THETA, m_fMinTheta);
	DDX_Text(pDX, IDC_EDIT_PHI, m_fMaxPhi);
	DDX_Text(pDX, IDC_EDIT_MIN_PHI, m_fMinPhi);
	DDX_Text(pDX, IDC_EDIT_INIT_SPEED, m_fInit_Speed);
	DDX_Text(pDX, IDC_EDIT_SPEED_RANGE, m_fSpeed_Range);
	DDX_Text(pDX, IDC_EDIT_INIT_LIFETIME, m_fInit_LifeTime);
	DDX_Text(pDX, IDC_EDIT_LIFETIME_RANGE, m_fLifeTime_Range);
	DDX_Text(pDX, IDC_EDIT_INIT_ACC_X, m_fInit_Acc_X);
	DDX_Text(pDX, IDC_EDIT_INIT_ACC_Y, m_fInit_Acc_Y);
	DDX_Text(pDX, IDC_EDIT_INIT_ACC_Z, m_fInit_Acc_Z);
	DDX_Text(pDX, IDC_EDIT_FRAME_CONST, m_fFrame_Const);

	DDX_Control(pDX, IDC_LIST_MESH, m_Mesh_List);
	DDX_Control(pDX, IDC_LIST_RECT, m_Rect_List);
	DDX_Control(pDX, IDC_LIST_PARTICLE, m_Particle_List);

	DDX_Text(pDX, IDC_EDIT_POSITION_START_POSITION_X, m_fStart_Position_X);
	DDX_Text(pDX, IDC_EDIT_POSITION_START_POSITION_Y, m_fStart_Position_Y);
	DDX_Text(pDX, IDC_EDIT_POSITION_START_POSITION_Z, m_fStart_Position_Z);
	DDX_Text(pDX, IDC_EDIT_POSITION_END_POSITION_X, m_fEnd_Position_X);
	DDX_Text(pDX, IDC_EDIT_POSITION_END_POSITION_Y, m_fEnd_Position_Y);
	DDX_Text(pDX, IDC_EDIT_POSITION_END_POSITION_Z, m_fEnd_Position_Z);

	DDX_Text(pDX, IDC_EDIT_ROTATION_START_POSITION_X, m_fRotation_Start_X);
	DDX_Text(pDX, IDC_EDIT_ROTATION_START_POSITION_Y, m_fRotation_Start_Y);
	DDX_Text(pDX, IDC_EDIT_ROTATION_START_POSITION_Z, m_fRotation_Start_Z);
	DDX_Text(pDX, IDC_EDIT_ROTATION_END_POSITION_X, m_fRotation_End_X);
	DDX_Text(pDX, IDC_EDIT_ROTATION_END_POSITION_Y, m_fRotation_End_Y);
	DDX_Text(pDX, IDC_EDIT_ROTATION_END_POSITION_Z, m_fRotation_End_Z);

	DDX_Text(pDX, IDC_EDIT_SCALE_START_POSITION_X, m_fScale_Start_X);
	DDX_Text(pDX, IDC_EDIT_SCALE_START_POSITION_Y, m_fScale_Start_Y);
	DDX_Text(pDX, IDC_EDIT_SCALE_START_POSITION_Z, m_fScale_Start_Z);
	DDX_Text(pDX, IDC_EDIT_SCALE_END_POSITION_X, m_fScale_End_X);
	DDX_Text(pDX, IDC_EDIT_SCALE_END_POSITION_Y, m_fScale_End_Y);
	DDX_Text(pDX, IDC_EDIT_SCALE_END_POSITION_Z, m_fScale_End_Z);
	DDX_Text(pDX, IDC_EDIT_MIN_THETA, m_fMinTheta);
	DDX_Text(pDX, IDC_EDIT_TAG, m_strTag);
	DDX_Text(pDX, IDC_EDIT_FADEOUT, m_fFadeOut);
	DDX_Text(pDX, IDC_EDIT_FADEIN, m_fFadeIn);
	DDX_Control(pDX, IDC_CHECK_PASS3, m_bt_NoisePass);
	DDX_Text(pDX, IDC_EDIT_PARTICLENUM, m_iParticleNum);
	DDX_Text(pDX, IDC_EDIT_EFFECT_GROUP, m_strGroupName);
	DDX_Text(pDX, IDC_EDIT_GROUPEFFECT_LIEFTIME, m_fGroupEffect_LifeTime);
	DDX_Text(pDX, IDC_EDIT_STARTTIME, m_fStartTime);
	DDX_Text(pDX, IDC_EDIT_ENTIME, m_fEndTime);
	DDX_Control(pDX, IDC_CHECK_RECYCLE, m_bt_Recycle);
	DDX_Text(pDX, IDC_EDIT_ALPHA, m_fAlpha);
	DDX_Text(pDX, IDC_EDIT_ALPHA_WIDTH, m_iAlpha_Width);
	DDX_Text(pDX, IDC_EDIT_ALPHA_HEIGHT, m_iAlpha_Height);
	DDX_Text(pDX, IDC_EDIT_BASE_WIDTH, m_iBase_Width);
	DDX_Text(pDX, IDC_EDIT_BASE_HEIGHT, m_iBase_Height);
	DDX_Text(pDX, IDC_EDIT_INIT_ALPHA, m_fInit_Alpha);
	DDX_Control(pDX, IDC_CHECK_PASS4, m_bt_NoneAlhpa);
	DDX_Text(pDX, IDC_EDIT_MESH_TAG, m_strMeshTag);
}


BEGIN_MESSAGE_MAP(CEffectTab, CDialogEx)
	ON_BN_CLICKED(IDC_CREATE_MESH, &CEffectTab::OnBnClickedCreateMesh)
	ON_BN_CLICKED(IDC_CREATE_RECT, &CEffectTab::OnBnClickedCreateRect)
	ON_BN_CLICKED(IDC_CHECK_PASS0, &CEffectTab::OnBnClickedCheckPass0)
	ON_BN_CLICKED(IDC_CHECK_PASS1, &CEffectTab::OnBnClickedCheckPass1)
	ON_BN_CLICKED(IDC_CHECK_PASS2, &CEffectTab::OnBnClickedCheckPass2)
	ON_BN_CLICKED(IDC_BT_UVANIMATION_APPLY, &CEffectTab::OnBnClickedBtUvanimationApply)
	ON_BN_CLICKED(IDC_CREATE_PARTICLE, &CEffectTab::OnBnClickedCreateParticle)
	ON_BN_CLICKED(IDC_BT_PARTICLE_INFO_ALLPY, &CEffectTab::OnBnClickedBtParticleInfoAllpy)
	ON_LBN_SELCHANGE(IDC_LIST_MESH, &CEffectTab::OnLbnSelchangeListMesh)
	ON_LBN_SELCHANGE(IDC_LIST_RECT, &CEffectTab::OnLbnSelchangeListRect)
	ON_LBN_SELCHANGE(IDC_LIST_PARTICLE, &CEffectTab::OnLbnSelchangeListParticle)
	ON_BN_CLICKED(IDC_BT_TRANSFORM_APPLY, &CEffectTab::OnBnClickedBtTransformApply)
	ON_BN_CLICKED(IDC_BT_PLAY, &CEffectTab::OnBnClickedBtPlay)
	ON_BN_CLICKED(IDC_BT_S_POSITION_SET, &CEffectTab::OnBnClickedBtSPositionSet)
	ON_BN_CLICKED(IDC_BT_E_POSITION_SET, &CEffectTab::OnBnClickedBtEPositionSet)
	ON_BN_CLICKED(IDC_BT_S_ROTATION_SET, &CEffectTab::OnBnClickedBtSRotationSet)
	ON_BN_CLICKED(IDC_BT_E_ROTATION_SET, &CEffectTab::OnBnClickedBtERotationSet)
	ON_BN_CLICKED(IDC_BT_S_SCALE_SET, &CEffectTab::OnBnClickedBtSScaleSet)
	ON_BN_CLICKED(IDC_BT_E_SCALE_SET, &CEffectTab::OnBnClickedBtEScaleSet)
	ON_BN_CLICKED(IDC_EFFECT_SAVE, &CEffectTab::OnBnClickedEffectSave)
	ON_BN_CLICKED(IDC_EFFECT_Load, &CEffectTab::OnBnClickedEffectLoad)
	ON_BN_CLICKED(IDC_CREATE_GROUP, &CEffectTab::OnBnClickedCreateGroup)
	ON_BN_CLICKED(IDC_CHECK_PASS3, &CEffectTab::OnBnClickedCheckPass3)
	ON_BN_CLICKED(IDC_BT_GROUPEFFECT_LIFETIME_APPLY, &CEffectTab::OnBnClickedBtGroupeffectLifetimeApply)
	ON_BN_CLICKED(IDC_BT_EFFECT_DELETE_PARTICLE, &CEffectTab::OnBnClickedBtEffectDeleteParticle)
	ON_BN_CLICKED(IDC_BT_EFFECT_DELETE_RECT, &CEffectTab::OnBnClickedBtEffectDeleteRect)
	ON_BN_CLICKED(IDC_BT_EFFECT_DELETE_MESH, &CEffectTab::OnBnClickedBtEffectDeleteMesh)
	ON_BN_CLICKED(IDC_BT_SELECT_CLEAR, &CEffectTab::OnBnClickedBtSelectClear)
	ON_BN_CLICKED(IDC_CHECK_RECYCLE, &CEffectTab::OnBnClickedCheckRecycle)
	ON_BN_CLICKED(IDC_BT_SET_TAG, &CEffectTab::OnBnClickedBtSetTag)
	ON_BN_CLICKED(IDC_CHECK_PASS4, &CEffectTab::OnBnClickedCheckPass4)
	ON_BN_CLICKED(IDC_MESH_TAG_BT, &CEffectTab::OnBnClickedMeshTagBt)
END_MESSAGE_MAP()


// CEffectTab 메시지 처리기입니다.


void CEffectTab::OnBnClickedCreateGroup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_strTag == L"")
		return;
	UpdateData(FALSE);

	if (m_pEffectGroup != nullptr)
		Safe_Release(m_pEffectGroup);

	wstring wstrGameObjectTag = L"GameObject_Effect_";
	wstrGameObjectTag += m_strTag;

	Create_Prototype_GameObject(wstrGameObjectTag.c_str());
	if (FAILED(Add_Layer_GameObject(L"Layer_Effect", wstrGameObjectTag.c_str(), (CGameObject**)&m_pEffectGroup)))
		return;

	UpdateData(TRUE);
	m_strGroupName = wstrGameObjectTag.c_str();
	m_pEffectGroup->Set_Tag(wstrGameObjectTag.c_str());
	UpdateData(FALSE);
}

void CEffectTab::Add_EffectMesh_List()
{
	m_Mesh_List.AddString(L"Mesh");
}

void CEffectTab::Add_EffectRect_List()
{
	m_Rect_List.AddString(L"Rect");
}

void CEffectTab::Add_EffectParticle_List()
{
	m_Particle_List.AddString(L"Particle");
}

void CEffectTab::OnBnClickedCreateMesh()
{
	if (m_pEffectGroup == nullptr)
		return;

	UpdateData(TRUE);
	if (m_strTag == L"")
		return;
	UpdateData(FALSE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(TRUE, L"X", L"*.X"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.X", this,0,0);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Effect\\EffectMesh\\", szFullPath);

	Dlg.m_pOFN->lpstrInitialDir = szFullPath;

	if (Dlg.DoModal() == IDCANCEL)
		return;
	CString strFilePath= Dlg.GetPathName();
	PathRemoveFileSpec((LPWSTR)strFilePath.operator LPCWSTR());


	wstring wstrComponentMeshTag = L"Component_Mesh_Static_";
	wstring wstrFileName = Dlg.GetFileName();

	wstrComponentMeshTag += wstrFileName.substr(0, wstrFileName.length() - 2);

	//Create EffectMesh
	LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	if (pDevice == nullptr)
		return;
	pDevice->AddRef();
	{
		CEffect_Mesh* pEffect_Mesh = CEffect_Mesh::Create(pDevice);
		m_pEffectGroup->Add_Effect_Mesh(pEffect_Mesh);
		pEffect_Mesh->SetUp_EffectInfo(wstrComponentMeshTag.c_str(), this);
		m_pEffect = pEffect_Mesh;
	}
	Safe_Release(pDevice);
	//Late SetUp Staic Object


	//ListBox
	m_Mesh_List.AddString(m_strTag);
}

HRESULT CEffectTab::Create_Prototype_GameObject(const _tchar * pGameObjectTag)
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

	if (FAILED(pManagement->Add_Prototype_GameObject(pGameObjectTag, CEffectGroup::Create(pDevice))))
	{
		Safe_Release(pDevice);
		Safe_Release(pManagement);
		return E_FAIL;
	}

	
	Safe_Release(pDevice);
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CEffectTab::Add_Layer_GameObject(const _tchar* pLayerTag, const _tchar * pGameObjectTag, CGameObject** ppGameObject)
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

void CEffectTab::OnBnClickedCreateRect()
{
	UpdateData(TRUE);
	if (m_strTag == L"")
		return;
	UpdateData(FALSE);

	
	//Create EffectRect
	LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	if (pDevice == nullptr)
		return;
	pDevice->AddRef();
	{
		CEffect_Rect* pEffect_Rect = CEffect_Rect::Create(pDevice);
		m_pEffectGroup->Add_Effect_Rect(pEffect_Rect);
		m_pEffect = pEffect_Rect;
	}
	Safe_Release(pDevice);
// 
// 	//Late SetUp Staic Object
// 	if (FAILED(m_pEffect_Rect->Set_TextureCom(wstrComponentTextureTag.c_str())))
// 		return;

	//List Box 추가
	m_Rect_List.AddString(m_strTag);
}



void CEffectTab::OnBnClickedCheckPass0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bt_Default.SetCheck(1);
	m_bt_Alpha.SetCheck(0);
	m_bt_TwoTexutre.SetCheck(0);
	m_bt_NoisePass.SetCheck(0);
	m_bt_NoneAlhpa.SetCheck(0);

	m_pEffect->Set_PassIndex(0);
}


void CEffectTab::OnBnClickedCheckPass1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bt_Default.SetCheck(0);
	m_bt_Alpha.SetCheck(1);
	m_bt_TwoTexutre.SetCheck(0);
	m_bt_NoisePass.SetCheck(0);
	m_bt_NoneAlhpa.SetCheck(0);


	CFileDialog		Dlg(TRUE, 0, 0
		, OFN_HIDEREADONLY
		, 0, this);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Effect\\", szFullPath);

	Dlg.m_pOFN->lpstrInitialDir = szFullPath;
	
	if (Dlg.DoModal() == IDCANCEL)
		return;

	wstring wstrComponentTextureTag = L"Component_Texture_";
	wstring wstrFileName = Dlg.GetFileName();
	wstrComponentTextureTag += wstrFileName.substr(0, wstrFileName.length() - 4);

	m_pEffect->SetUp_AlphaTexutre(wstrComponentTextureTag.c_str());
	m_pEffect->Set_PassIndex(1);
}


void CEffectTab::OnBnClickedCheckPass2()
{
	if(m_pEffect == nullptr)
		return;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bt_Default.SetCheck(0);
	m_bt_Alpha.SetCheck(0);
	m_bt_TwoTexutre.SetCheck(1);
	m_bt_NoisePass.SetCheck(0);
	m_bt_NoneAlhpa.SetCheck(0);

	CFileDialog		Dlg(TRUE, 0, 0
		, OFN_HIDEREADONLY
		, 0, this);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Effect\\", szFullPath);

	Dlg.m_pOFN->lpstrInitialDir = szFullPath;

	if (Dlg.DoModal() == IDCANCEL)
		return;

	wstring wstrComponentTextureTag = L"Component_Texture_";
	wstring wstrFileName = Dlg.GetFileName();
	wstrComponentTextureTag += wstrFileName.substr(0, wstrFileName.length() - 4);


	m_pEffect->SetUp_BaseTexture(wstrComponentTextureTag.c_str());
	m_pEffect->Set_PassIndex(2);
}



void CEffectTab::OnBnClickedCheckPass3()
{
	if (m_pEffect == nullptr)
		return;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bt_Default.SetCheck(0);
	m_bt_Alpha.SetCheck(0);
	m_bt_TwoTexutre.SetCheck(0);
	m_bt_NoisePass.SetCheck(1);
	m_bt_NoneAlhpa.SetCheck(0);

	CFileDialog		Dlg(TRUE, 0, 0
		, OFN_HIDEREADONLY
		, 0, this);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Effect\\", szFullPath);

	Dlg.m_pOFN->lpstrInitialDir = szFullPath;

	if (Dlg.DoModal() == IDCANCEL)
		return;

	wstring wstrComponentTextureTag = L"Component_Texture_";
	wstring wstrFileName = Dlg.GetFileName();
	wstrComponentTextureTag += wstrFileName.substr(0, wstrFileName.length() - 4);


	m_pEffect->SetUp_NoiseTexture(wstrComponentTextureTag.c_str());
	m_pEffect->Set_PassIndex(3);
}

void CEffectTab::OnBnClickedCheckPass4()
{
	if (m_pEffect == nullptr)
		return;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bt_Default.SetCheck(0);
	m_bt_Alpha.SetCheck(0);
	m_bt_TwoTexutre.SetCheck(0);
	m_bt_NoisePass.SetCheck(0);
	m_bt_NoneAlhpa.SetCheck(1);

	CFileDialog		Dlg(TRUE, 0, 0
		, OFN_HIDEREADONLY
		, 0, this);

	HMODULE	hModule = GetModuleHandle(nullptr);
	ASSERT(hModule != 0);

	_tchar		szFullPath[MAX_PATH] = L"";
	GetModuleFileNameW(hModule, szFullPath, MAX_PATH);

	PathRemoveFileSpecW(szFullPath);

	swprintf_s(szFullPath, L"%s\\Resources\\Effect\\", szFullPath);

	Dlg.m_pOFN->lpstrInitialDir = szFullPath;

	if (Dlg.DoModal() == IDCANCEL)
		return;

	wstring wstrComponentTextureTag = L"Component_Texture_";
	wstring wstrFileName = Dlg.GetFileName();
	wstrComponentTextureTag += wstrFileName.substr(0, wstrFileName.length() - 4);


	m_pEffect->SetUp_AlphaTexutre(wstrComponentTextureTag.c_str());
	m_pEffect->Set_PassIndex(3);
}


void CEffectTab::OnBnClickedBtUvanimationApply()
{
	if (m_pEffect == nullptr)
		return;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_pEffect->Set_USpeed(m_fUSpeed);
	m_pEffect->Set_VSpeed(m_fVSpeed);

	m_pEffect->Set_Alpha_Width(m_iAlpha_Width);
	m_pEffect->Set_Alpha_Height(m_iAlpha_Height);
	m_pEffect->Set_Base_Width(m_iBase_Width);
	m_pEffect->Set_Base_Height(m_iBase_Height);

	UpdateData(FALSE);
}


void CEffectTab::OnBnClickedCreateParticle()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strTag == L"")
		return;
	UpdateData(FALSE);


	//Create EffectParticle
	LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	if (pDevice == nullptr)
		return;
	pDevice->AddRef();
	{
		CEffect_Particle* pEffect_Particle = CEffect_Particle::Create(pDevice);
		m_pEffectGroup->Add_Effect_Paritcle(pEffect_Particle);

		UpdateData(TRUE);
		pEffect_Particle->SetUp_ParticleInfo(m_iParticleNum);

		
		EFFECTPARTICLE_DATA EffectData;
		pEffect_Particle->Get_Inital_ParticleInfo(EffectData);


		 m_fMaxTheta			= EffectData.fMax_Theta;
		 m_fMinTheta			= EffectData.fMin_Theta;
		 m_fMinPhi				= EffectData.fMin_Phi;
		 m_fMaxPhi				= EffectData.fMax_Phi;
		 m_fInit_Speed			= EffectData.fInit_Speed;
		 m_fSpeed_Range			= EffectData.fSpeed_Range;
		 m_fInit_LifeTime		= EffectData.fInit_LifeTime;
		 m_fLifeTime_Range		= EffectData.fLifeTime_Range ;
		 m_fFrame_Const			= EffectData.fFrame_Const;
		 m_fInit_Acc_X			= EffectData.vInital_Acceleration.x;
		 m_fInit_Acc_Y			= EffectData.vInital_Acceleration.y;
		 m_fInit_Acc_Z			= EffectData.vInital_Acceleration.z;


		UpdateData(FALSE);

		m_pEffect = pEffect_Particle;
	}
	Safe_Release(pDevice);


	//List Box 추가
	m_Particle_List.AddString(m_strTag);

}


void CEffectTab::OnBnClickedBtParticleInfoAllpy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEffect_Particle* pEffect_Particle = dynamic_cast<CEffect_Particle*>(m_pEffect);
	if (pEffect_Particle == nullptr)
		return;

	UpdateData(TRUE);
	EFFECTPARTICLE_DATA EffectData;
	_vec3	m_vInital_Accelation = _vec3(m_fInit_Acc_X, m_fInit_Acc_Y, m_fInit_Acc_Z);

	pEffect_Particle->Set_ParticleNum(m_iParticleNum);

	EffectData.fMax_Theta = m_fMaxTheta;
	EffectData.fMin_Theta = m_fMinTheta;
	EffectData.fMin_Phi = m_fMinPhi;
	EffectData.fMax_Phi = m_fMaxPhi;
	EffectData.fInit_Speed = m_fInit_Speed;
	EffectData.fSpeed_Range = m_fSpeed_Range;
	EffectData.fInit_LifeTime = m_fInit_LifeTime;
	EffectData.fLifeTime_Range = m_fLifeTime_Range;
	EffectData.fFrame_Const = m_fFrame_Const;
	EffectData.vInital_Acceleration.x = m_fInit_Acc_X;
	EffectData.vInital_Acceleration.y = m_fInit_Acc_Y;
	EffectData.vInital_Acceleration.z = m_fInit_Acc_Z;

	pEffect_Particle->Set_Inital_ParticleInfo(EffectData);

	UpdateData(FALSE);
}


void CEffectTab::OnLbnSelchangeListMesh()
{
	if (m_pEffect != nullptr)
	{
		m_pEffect->Set_Selected(false);
		m_pEffect = nullptr;
	}

	_int iIndex = m_Mesh_List.GetCurSel();

	m_pEffect = m_pEffectGroup->Get_Effect_Mesh(iIndex);
	if (m_pEffect == nullptr)
	{
		return;
	}
	m_pEffect->Set_Selected(true);


	UpdateData(TRUE);
	m_pEffect->Get_Position(m_fPositionX, m_fPositionY, m_fPositionZ);
	m_pEffect->Get_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pEffect->Get_Rotation(m_fRotationX, m_fRotationY, m_fRotationZ);
	m_pEffect->Get_USpeed(m_fUSpeed);
	m_pEffect->Get_VSpeed(m_fVSpeed);
	m_pEffect->Get_Alpha(m_fAlpha);
	m_pEffect->Get_Init_Alpha(m_fInit_Alpha);
	m_pEffect->Get_Alpha_Width(m_iAlpha_Width);
	m_pEffect->Get_Alpha_Height(m_iAlpha_Height);
	m_pEffect->Get_Base_Width(m_iBase_Width);
	m_pEffect->Get_Base_Height(m_iBase_Height);
	

	EFFECTDATA	EffectData;
	m_pEffect->Get_EffectData(EffectData);
	m_fStart_Position_X = EffectData.vStart_Position.x;
	m_fStart_Position_Y = EffectData.vStart_Position.y;
	m_fStart_Position_Z = EffectData.vStart_Position.z;
	m_fEnd_Position_X = EffectData.vEnd_Position.x;
	m_fEnd_Position_Y = EffectData.vEnd_Position.y;
	m_fEnd_Position_Z = EffectData.vEnd_Position.z;

	m_fRotation_Start_X = EffectData.vStart_Rotation.x;
	m_fRotation_Start_Y = EffectData.vStart_Rotation.y;
	m_fRotation_Start_Z = EffectData.vStart_Rotation.z;
	m_fRotation_End_X   = EffectData.vEnd_Rotation.x;
	m_fRotation_End_Y   = EffectData.vEnd_Rotation.y;
	m_fRotation_End_Z   = EffectData.vEnd_Rotation.z;

	m_fScale_Start_X = EffectData.vStart_Scale.x;
	m_fScale_Start_Y = EffectData.vStart_Scale.y;
	m_fScale_Start_Z = EffectData.vStart_Scale.z;
	m_fScale_End_X = EffectData.vEnd_Scale.x;
	m_fScale_End_Y = EffectData.vEnd_Scale.y;
	m_fScale_End_Z = EffectData.vEnd_Scale.z;

	m_fFadeIn = EffectData.fFadeIn;
	m_fFadeOut = EffectData.fFadeOut;
	m_fStartTime = EffectData.fStartTime;
	m_fEndTime = EffectData.fEndTime;

	UpdateData(FALSE);
}


void CEffectTab::OnLbnSelchangeListRect()
{
	if (m_pEffect != nullptr)
	{
		m_pEffect->Set_Selected(false);
		m_pEffect = nullptr;
	}

	_int iIndex = m_Rect_List.GetCurSel();

	m_pEffect = m_pEffectGroup->Get_Effect_Rect(iIndex);
	if (m_pEffect == nullptr)
	{
		return;
	}
	m_pEffect->Set_Selected(true);


	UpdateData(TRUE);
	m_pEffect->Get_Position(m_fPositionX, m_fPositionY, m_fPositionZ);
	m_pEffect->Get_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pEffect->Get_Rotation(m_fRotationX, m_fRotationY, m_fRotationZ);
	m_pEffect->Get_USpeed(m_fUSpeed);
	m_pEffect->Get_VSpeed(m_fVSpeed);
	m_pEffect->Get_Alpha(m_fAlpha);
	m_pEffect->Get_Init_Alpha(m_fInit_Alpha);
	m_pEffect->Get_Alpha_Width(m_iAlpha_Width);
	m_pEffect->Get_Alpha_Height(m_iAlpha_Height);
	m_pEffect->Get_Base_Width(m_iBase_Width);
	m_pEffect->Get_Base_Height(m_iBase_Height);

	EFFECTDATA	EffectData;
	m_pEffect->Get_EffectData(EffectData);
	m_fStart_Position_X = EffectData.vStart_Position.x;
	m_fStart_Position_Y = EffectData.vStart_Position.y;
	m_fStart_Position_Z = EffectData.vStart_Position.z;
	m_fEnd_Position_X = EffectData.vEnd_Position.x;
	m_fEnd_Position_Y = EffectData.vEnd_Position.y;
	m_fEnd_Position_Z = EffectData.vEnd_Position.z;

	m_fRotation_Start_X = EffectData.vStart_Rotation.x;
	m_fRotation_Start_Y = EffectData.vStart_Rotation.y;
	m_fRotation_Start_Z = EffectData.vStart_Rotation.z;
	m_fRotation_End_X = EffectData.vEnd_Rotation.x;
	m_fRotation_End_Y = EffectData.vEnd_Rotation.y;
	m_fRotation_End_Z = EffectData.vEnd_Rotation.z;

	m_fScale_Start_X = EffectData.vStart_Scale.x;
	m_fScale_Start_Y = EffectData.vStart_Scale.y;
	m_fScale_Start_Z = EffectData.vStart_Scale.z;
	m_fScale_End_X = EffectData.vEnd_Scale.x;
	m_fScale_End_Y = EffectData.vEnd_Scale.y;
	m_fScale_End_Z = EffectData.vEnd_Scale.z;

	m_fFadeIn = EffectData.fFadeIn;
	m_fFadeOut = EffectData.fFadeOut;
	m_fStartTime = EffectData.fStartTime;
	m_fEndTime = EffectData.fEndTime;
	UpdateData(FALSE);
}


void CEffectTab::OnLbnSelchangeListParticle()
{
	if (m_pEffect != nullptr)
	{
		m_pEffect->Set_Selected(false);
		m_pEffect = nullptr;
	}

	_int iIndex = m_Particle_List.GetCurSel();

	m_pEffect = m_pEffectGroup->Get_Effect_Particle(iIndex);
	if (m_pEffect == nullptr)
	{
		return;
	}
	m_pEffect->Set_Selected(true);

	UpdateData(TRUE);
	m_pEffect->Get_Position(m_fPositionX, m_fPositionY, m_fPositionZ);
	m_pEffect->Get_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pEffect->Get_Rotation(m_fRotationX, m_fRotationY, m_fRotationZ);
	m_pEffect->Get_FadeIn(m_fFadeIn);
	m_pEffect->Get_FadeOut(m_fFadeOut);
	m_pEffect->Get_StartTime(m_fStartTime);
	m_pEffect->Get_EndTime(m_fEndTime);
	m_pEffect->Get_Alpha(m_fAlpha);
	m_pEffect->Get_Init_Alpha(m_fInit_Alpha);
	m_pEffect->Get_Alpha_Width(m_iAlpha_Width);
	m_pEffect->Get_Alpha_Height(m_iAlpha_Height);
	m_pEffect->Get_Base_Width(m_iBase_Width);
	m_pEffect->Get_Base_Height(m_iBase_Height);

	CEffect_Particle*	pEffect_Particle = dynamic_cast<CEffect_Particle*>(m_pEffect);

	EFFECTPARTICLE_DATA EffectData;
	pEffect_Particle->Get_Inital_ParticleInfo(EffectData);

		 m_fMaxTheta			= EffectData.fMax_Theta;
		 m_fMinTheta			= EffectData.fMin_Theta;
		 m_fMinPhi				= EffectData.fMin_Phi;
		 m_fMaxPhi				= EffectData.fMax_Phi;
		 m_fInit_Speed			= EffectData.fInit_Speed;
		 m_fSpeed_Range			= EffectData.fSpeed_Range;
		 m_fInit_LifeTime		= EffectData.fInit_LifeTime;
		 m_fLifeTime_Range		= EffectData.fLifeTime_Range ;
		 m_fFrame_Const			= EffectData.fFrame_Const;
		 m_fInit_Acc_X			= EffectData.vInital_Acceleration.x;
		 m_fInit_Acc_Y			= EffectData.vInital_Acceleration.y;
		 m_fInit_Acc_Z			= EffectData.vInital_Acceleration.z;


	UpdateData(FALSE);
}


void CEffectTab::OnBnClickedBtTransformApply()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pEffect != nullptr)
	{
		_vec3 vPosition = _vec3(m_fPositionX, m_fPositionY, m_fPositionZ);
		m_pEffect->Set_Position(&vPosition);
		m_pEffect->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
		m_pEffect->Set_Rotation(m_fRotationX, m_fRotationY, m_fRotationZ);    

		m_pEffect->Set_FadeIn(m_fFadeIn);
		m_pEffect->Set_FadeOut(m_fFadeOut);
		m_pEffect->Set_StartTime(m_fStartTime);
		m_pEffect->Set_EndTime(m_fEndTime);

		m_pEffect->Set_Alpha(m_fAlpha);
		m_pEffect->Set_Init_Alpha(m_fInit_Alpha);
	}
	m_fRotationX = 0.f;
	m_fRotationY = 0.f;
	m_fRotationZ = 0.f;
	UpdateData(FALSE);



}


void CEffectTab::OnBnClickedBtPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pEffectGroup->Set_Activate(true);

}


void CEffectTab::OnBnClickedBtSPositionSet()
{
	UpdateData(TRUE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pEffect != nullptr)
	{
		_vec3 vStartPos = m_pEffect->Set_StartPosition();

		m_fStart_Position_X = vStartPos.x;
		m_fStart_Position_Y = vStartPos.y;
		m_fStart_Position_Z = vStartPos.z;
	}

	UpdateData(FALSE);
}


void CEffectTab::OnBnClickedBtEPositionSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (m_pEffect != nullptr)
	{
		_vec3 vEndPos = m_pEffect->Set_EndPosition();
		m_fEnd_Position_X = vEndPos.x;
		m_fEnd_Position_Y = vEndPos.y;
		m_fEnd_Position_Z = vEndPos.z;
	}
	UpdateData(FALSE);
}


void CEffectTab::OnBnClickedBtSRotationSet()
{
	UpdateData(TRUE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pEffect != nullptr)
	{
		_vec3 vStartRotation = m_pEffect->Set_StartRotation();

		m_fRotation_Start_X = vStartRotation.x;
		m_fRotation_Start_Y = vStartRotation.y;
		m_fRotation_Start_Z = vStartRotation.z;
	}

	UpdateData(FALSE);
}


void CEffectTab::OnBnClickedBtERotationSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_pEffect != nullptr)
	{
		_vec3 vEndRotation = m_pEffect->Set_EndRotation();
		m_fRotation_End_X = vEndRotation.x;
		m_fRotation_End_Y = vEndRotation.y;
		m_fRotation_End_Z = vEndRotation.z;
	}

	UpdateData(FALSE);
}



void CEffectTab::OnBnClickedBtSScaleSet()
{
	UpdateData(TRUE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pEffect != nullptr)
	{
		_vec3 vStartScale = m_pEffect->Set_StartScale();

		m_fScale_Start_X = vStartScale.x;
		m_fScale_Start_Y = vStartScale.y;
		m_fScale_Start_Z = vStartScale.z;
	}

	UpdateData(FALSE);
}


void CEffectTab::OnBnClickedBtEScaleSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_pEffect != nullptr)
	{
		_vec3 vEndScale = m_pEffect->Set_EndScale();

		m_fScale_End_X = vEndScale.x;
		m_fScale_End_Y = vEndScale.y;
		m_fScale_End_Z = vEndScale.z;
	}
	UpdateData(FALSE);
}


void CEffectTab::OnBnClickedEffectSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(FALSE, L"eff", L"*.eff"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.eff", this, 0, FALSE);

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

	m_pEffectGroup->Save_EffectData(hFile, dwByte);

	CloseHandle(hFile);

}


void CEffectTab::OnBnClickedEffectLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(TRUE, L"eff", L"*.eff"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.eff", this, 0, FALSE);

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


	CEffectGroup::EFFECTGROUP_DATA	EffectGroyp_Data;

	//테그
	ReadFile(hFile, &EffectGroyp_Data, sizeof(CEffectGroup::EFFECTGROUP_DATA), &dwByte, nullptr);
	UpdateData(TRUE);
	m_strGroupName = EffectGroyp_Data.szTag;
	m_fGroupEffect_LifeTime = EffectGroyp_Data.fLifeTime;
	UpdateData(FALSE);

	//EffectGroup Create
	Create_Prototype_GameObject(EffectGroyp_Data.szTag);
	if (FAILED(Add_Layer_GameObject(L"Layer_Effect", EffectGroyp_Data.szTag, (CGameObject**)&m_pEffectGroup)))
		return;

	m_pEffectGroup->Load_EffectDataFromFile(hFile, dwByte,this);
	m_pEffectGroup->Set_LifeTime(EffectGroyp_Data.fLifeTime);

	
	m_pEffectGroup->Set_Tag(EffectGroyp_Data.szTag);
			

	CloseHandle(hFile);
}


BOOL CEffectTab::OnInitDialog()
{
 	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	pDevice->AddRef();

	CUI_Resources_Loader UI_Loader(pDevice);
	if (FAILED(UI_Loader.Effect_Mesh_Load(L"../Bin/Resources/Effect/EffectMesh/*.*")))
	{
		Safe_Release(pDevice);
		return E_FAIL;
	}

	if (FAILED(UI_Loader.Effect_Texture_Load(L"../Bin/Resources/Effect/SingleTexture/*.*")))
	{
		Safe_Release(pDevice);
		return E_FAIL;
	}

	UI_Loader.UI_Resources_Load(L"../Bin/Resources/Effect/Texture/*.*");
	

	Safe_Release(pDevice);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CEffectTab::OnBnClickedBtGroupeffectLifetimeApply()
{
	if (m_pEffectGroup == nullptr)
		return;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_pEffectGroup->Set_LifeTime(m_fGroupEffect_LifeTime);
	UpdateData(FALSE);
}


void CEffectTab::OnBnClickedBtEffectDeleteParticle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pEffectGroup == nullptr)
		return;

	if (m_Particle_List.GetCount())
		return;
	
	_int iIndex = m_Particle_List.GetCurSel();

	m_Particle_List.DeleteString(iIndex);
	
	_int iCount = m_Particle_List.GetCount();
	if (iCount)
		m_Particle_List.SetCurSel(iIndex - (iIndex == iCount));

	m_pEffectGroup->Remove_EffectParticle(iIndex);
}


void CEffectTab::OnBnClickedBtEffectDeleteRect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pEffectGroup == nullptr)
		return;

	if (m_Rect_List.GetCount())
		return;

	_int iIndex = m_Rect_List.GetCurSel();
	m_Rect_List.DeleteString(iIndex);


	_int iCount = m_Rect_List.GetCount();
	if (iCount)
		m_Rect_List.SetCurSel(iIndex - (iIndex == iCount));


	m_pEffectGroup->Remove_EffectRect(iIndex);
}


void CEffectTab::OnBnClickedBtEffectDeleteMesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pEffectGroup == nullptr)
		return;

	_int iCount = m_Mesh_List.GetCount();
	
	if (iCount <= 0)
		return;

	_int iIndex = m_Mesh_List.GetCurSel();

	m_Mesh_List.DeleteString(iIndex);

	iCount = m_Mesh_List.GetCount();
	if (iCount)
		m_Mesh_List.SetCurSel(iIndex - (iIndex == iCount));


	m_pEffectGroup->Remove_EffectMesh(iIndex);
}


void CEffectTab::OnBnClickedBtSelectClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pEffect == nullptr)
		return;

	m_pEffect->Set_Selected(false);

}


void CEffectTab::OnBnClickedCheckRecycle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEffect_Particle* pParticle= dynamic_cast<CEffect_Particle*>(m_pEffect);
	if (pParticle == nullptr)
		return;
	if(m_bt_Recycle.GetCheck() == TRUE)
		pParticle->Set_ReCycle(true);
	else
		pParticle->Set_ReCycle(false);
}


void CEffectTab::OnBnClickedBtSetTag()
{
	if (m_pEffectGroup == nullptr)
		return;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	wstring wstrGameObjectTag = L"GameObject_Effect_";
	wstrGameObjectTag += m_strTag;
	m_pEffectGroup->Set_Tag(wstrGameObjectTag.c_str());
	m_strGroupName = wstrGameObjectTag.c_str();
	UpdateData(FALSE);
}




void CEffectTab::OnBnClickedMeshTagBt()
{
	// TODO: Add your control notification handler code here
	//szMeshTag = 0x3626ae6c_Shild_001_SM_PHS"
	if (m_pEffect == nullptr)
		return;
	CEffect_Mesh* pEffectMesh = dynamic_cast<CEffect_Mesh*>(m_pEffect);

	CString	strTag = L"Component_Mesh_Static_" + m_strMeshTag;
	UpdateData(TRUE);
	pEffectMesh->Set_MeshTag(strTag);
	UpdateData(FALSE);
}
