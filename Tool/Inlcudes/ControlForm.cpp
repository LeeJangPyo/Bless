// ControlForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ControlForm.h"

#include "TerrainTab.h"
#include "MeshTab.h"
#include "CameraTab.h"
#include "EffectTab.h"
#include "NavigationMeshTab.h"
#include "AnimationTab.h"
#include "UITab.h"
#include "LightTab.h"

// CControlForm
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CControlForm, CFormView)

CControlForm::CControlForm()
	: CFormView(IDD_CONTROLFORM)
{

}

CControlForm::~CControlForm()
{

	if (m_pTerrainTab != nullptr)
	{
		delete m_pTerrainTab;
		m_pTerrainTab = nullptr;
	}

	if (m_pMeshTab != nullptr)
	{
		delete m_pMeshTab;
		m_pMeshTab = nullptr;
	}

	if (m_pCameraTab != nullptr)
	{
		delete m_pCameraTab;
		m_pCameraTab = nullptr;
	}

	if (m_pEffectTab != nullptr)
	{
		delete m_pEffectTab;
		m_pEffectTab = nullptr;
	}

	if (m_pAnimationTab != nullptr)
	{
		delete m_pAnimationTab;
		m_pAnimationTab = nullptr;
	}


	if (m_pNavigationMeshTab != nullptr)
	{
		delete m_pNavigationMeshTab;
		m_pNavigationMeshTab = nullptr;
	}

	if (m_pUITab != nullptr)
	{
		delete m_pUITab;
		m_pUITab = nullptr;
	}


	if (m_pLightTab != nullptr)
	{
		delete m_pLightTab;
		m_pLightTab = nullptr;
	}
}

void CControlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CControlForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CControlForm::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CControlForm 진단입니다.

#ifdef _DEBUG
void CControlForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlForm 메시지 처리기입니다.


void CControlForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_Tab.InsertItem(0, L"Terrain");
	m_Tab.InsertItem(1, L"Mesh");
	m_Tab.InsertItem(2, L"NaviMesh");
	m_Tab.InsertItem(3, L"Camera");
	m_Tab.InsertItem(4, L"Effect");
	m_Tab.InsertItem(5, L"Animation");
	m_Tab.InsertItem(6, L"UI");
	m_Tab.InsertItem(7, L"Light");


	m_Tab.SetCurSel(0);

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	m_pTerrainTab = new CTerrainTab;
	m_pTerrainTab->Create(IDD_TERRAIN, &m_Tab);
	m_pTerrainTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pTerrainTab->ShowWindow(SW_SHOW);

	m_pMeshTab = new CMeshTab;
	m_pMeshTab->Create(IDD_MESH, &m_Tab);
	m_pMeshTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pMeshTab->ShowWindow(SW_HIDE);

	m_pNavigationMeshTab = new CNavigationMeshTab;
	m_pNavigationMeshTab->Create(IDD_NAVIMESH, &m_Tab);
	m_pNavigationMeshTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pNavigationMeshTab->ShowWindow(SW_HIDE);

	m_pCameraTab = new CCameraTab;
	m_pCameraTab->Create(IDD_CAMERA, &m_Tab);
	m_pCameraTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pCameraTab->ShowWindow(SW_HIDE);

	m_pEffectTab = new CEffectTab;
	m_pEffectTab->Create(IDD_EFFECT, &m_Tab);
	m_pEffectTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pEffectTab->ShowWindow(SW_HIDE);

	m_pAnimationTab = new CAnimationTab;
	m_pAnimationTab->Create(IDD_ANIMATION, &m_Tab);
	m_pAnimationTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pAnimationTab->ShowWindow(SW_HIDE);

	m_pUITab = new CUITab;
	m_pUITab->Create(IDD_UI, &m_Tab);
	m_pUITab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pUITab->ShowWindow(SW_HIDE);

	m_pLightTab = new CLightTab;
	m_pLightTab->Create(IDD_LIGHT, &m_Tab);
	m_pLightTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pLightTab->ShowWindow(SW_HIDE);

}


void CControlForm::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSelect = m_Tab.GetCurSel();

	switch(iSelect)
	{
	case 0:
		m_pTerrainTab->ShowWindow(SW_SHOW);
		m_pMeshTab->ShowWindow(SW_HIDE);
		m_pNavigationMeshTab->ShowWindow(SW_HIDE);
		m_pCameraTab->ShowWindow(SW_HIDE);
		m_pEffectTab->ShowWindow(SW_HIDE);
		m_pAnimationTab->ShowWindow(SW_HIDE);
		m_pUITab->ShowWindow(SW_HIDE);
		m_pLightTab->ShowWindow(SW_HIDE);

		m_eTabType = TAB_TERRAIN;
		break;
	case 1:
		m_pTerrainTab->ShowWindow(SW_HIDE);
		m_pMeshTab->ShowWindow(SW_SHOW);
		m_pNavigationMeshTab->ShowWindow(SW_HIDE);
		m_pCameraTab->ShowWindow(SW_HIDE);
		m_pEffectTab->ShowWindow(SW_HIDE);
		m_pAnimationTab->ShowWindow(SW_HIDE);
		m_pUITab->ShowWindow(SW_HIDE);
		m_pLightTab->ShowWindow(SW_HIDE);

		m_eTabType = TAB_MESH;
		break;
	case 2:
		m_pTerrainTab->ShowWindow(SW_HIDE);
		m_pMeshTab->ShowWindow(SW_HIDE);
		m_pNavigationMeshTab->ShowWindow(SW_SHOW);
		m_pCameraTab->ShowWindow(SW_HIDE);
		m_pEffectTab->ShowWindow(SW_HIDE);
		m_pAnimationTab->ShowWindow(SW_HIDE);
		m_pUITab->ShowWindow(SW_HIDE);
		m_pLightTab->ShowWindow(SW_HIDE);

		m_eTabType = TAB_NAVIMESH;
		break;
	case 3:
		m_pTerrainTab->ShowWindow(SW_HIDE);
		m_pMeshTab->ShowWindow(SW_HIDE);
		m_pNavigationMeshTab->ShowWindow(SW_HIDE);
		m_pCameraTab->ShowWindow(SW_SHOW);
		m_pEffectTab->ShowWindow(SW_HIDE);
		m_pAnimationTab->ShowWindow(SW_HIDE);
		m_pUITab->ShowWindow(SW_HIDE);
		m_pLightTab->ShowWindow(SW_HIDE);

		m_eTabType = TAB_CAMERA;
		break;
	case 4:
		m_pTerrainTab->ShowWindow(SW_HIDE);
		m_pMeshTab->ShowWindow(SW_HIDE);
		m_pNavigationMeshTab->ShowWindow(SW_HIDE);
		m_pCameraTab->ShowWindow(SW_HIDE);
		m_pEffectTab->ShowWindow(SW_SHOW);
		m_pAnimationTab->ShowWindow(SW_HIDE);
		m_pUITab->ShowWindow(SW_HIDE);
		m_pLightTab->ShowWindow(SW_HIDE);

		m_eTabType = TAB_EFFECT;
		break;
	case 5:
		m_pTerrainTab->ShowWindow(SW_HIDE);
		m_pMeshTab->ShowWindow(SW_HIDE);
		m_pNavigationMeshTab->ShowWindow(SW_HIDE);
		m_pCameraTab->ShowWindow(SW_HIDE);
		m_pEffectTab->ShowWindow(SW_HIDE);
		m_pAnimationTab->ShowWindow(SW_SHOW);
		m_pUITab->ShowWindow(SW_HIDE);
		m_pLightTab->ShowWindow(SW_HIDE);

		m_eTabType = TAB_ANIMATION;
		break;
	case 6:
		m_pTerrainTab->ShowWindow(SW_HIDE);
		m_pMeshTab->ShowWindow(SW_HIDE);
		m_pNavigationMeshTab->ShowWindow(SW_HIDE);
		m_pCameraTab->ShowWindow(SW_HIDE);
		m_pEffectTab->ShowWindow(SW_HIDE);
		m_pAnimationTab->ShowWindow(SW_HIDE);
		m_pUITab->ShowWindow(SW_SHOW);
		m_pLightTab->ShowWindow(SW_HIDE);

		m_eTabType = TAB_UI;
		break;
	case 7:
		m_pTerrainTab->ShowWindow(SW_HIDE);
		m_pMeshTab->ShowWindow(SW_HIDE);
		m_pNavigationMeshTab->ShowWindow(SW_HIDE);
		m_pCameraTab->ShowWindow(SW_HIDE);
		m_pEffectTab->ShowWindow(SW_HIDE);
		m_pAnimationTab->ShowWindow(SW_HIDE);
		m_pUITab->ShowWindow(SW_HIDE);
		m_pLightTab->ShowWindow(SW_SHOW);

		m_eTabType = TAB_LIGHT;
		break;
	}


	*pResult = 0;
}


void CControlForm::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.



	CFormView::PostNcDestroy();
}
