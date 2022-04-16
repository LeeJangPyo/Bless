
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"

#include "ControlForm.h"
#include "LightTab.h"

#include "Management.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	m_pMainApp->Render_MainApp();

}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();

	

	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
	RECT			rcFrameWindows;
	pMainFrame->GetWindowRect(&rcFrameWindows);

	SetRect(&rcFrameWindows, 0, 0
		, rcFrameWindows.right - rcFrameWindows.left
		, rcFrameWindows.bottom - rcFrameWindows.top);

	RECT	rcMainView;
	GetClientRect(&rcMainView);

	int		iRowFrm = rcFrameWindows.right - rcMainView.right;
	int		iColFrm = rcFrameWindows.bottom - rcMainView.bottom;

	pMainFrame->SetWindowPos(nullptr, 200, 200
		, int(g_nBackCX + iRowFrm)
		, int(g_nBackCY + iColFrm)
		, SWP_NOZORDER);



	//앱 생성
	((CToolApp*)AfxGetApp())->m_pMainApp = m_pMainApp = CMainApp::Create();
	if (nullptr == m_pMainApp)
		return;


	//Calcuatlor
	LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	m_pCalculator = CCalculator::Create(pDevice);
	if (m_pCalculator == nullptr)
		return;
}


void CToolView::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	Safe_Release(m_pCalculator);
	
	CView::PostNcDestroy();
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CMainFrame
	if (((CMainFrame*)AfxGetMainWnd())->m_pControlForm->m_eTabType == CControlForm::TAB_LIGHT)
	{
		CBuffer_TerrainTex*	pTerrainBufferCom = dynamic_cast<CBuffer_TerrainTex*>
			(CManagement::GetInstance()->Get_Component(SCENE_EDIT, L"Layer_Terrain", L"Com_Buffer"));

		if (pTerrainBufferCom == nullptr)
		{
			AfxMessageBox(L"TerrainBufferCom is Null");
		}

		CTransform*		pTerrainTransCom = dynamic_cast<Engine::CTransform*>
			(CManagement::GetInstance()->Get_Component(SCENE_EDIT, L"Layer_Terrain", L"Com_Transform"));
		if (pTerrainTransCom == nullptr)
		{
			AfxMessageBox(L"TerrainTransCom is Null");
		}

		_vec3 vPickingPosition = m_pCalculator->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);

		((CMainFrame*)AfxGetMainWnd())->m_pControlForm->m_pLightTab->Set_Position(&vPickingPosition);
	}
	

	CView::OnLButtonDown(nFlags, point);
}
