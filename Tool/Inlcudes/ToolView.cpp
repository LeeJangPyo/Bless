
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	m_pMainApp->Render_MainApp();

}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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



	//�� ����
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	Safe_Release(m_pCalculator);
	
	CView::PostNcDestroy();
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
