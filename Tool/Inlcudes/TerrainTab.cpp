// TerrainTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "TerrainTab.h"
#include "afxdialogex.h"


// CTerrainTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTerrainTab, CDialogEx)

CTerrainTab::CTerrainTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TERRAIN, pParent)
{

}

CTerrainTab::~CTerrainTab()
{
}

void CTerrainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_TerrainTextureListBox);
}


BEGIN_MESSAGE_MAP(CTerrainTab, CDialogEx)

	ON_LBN_SELCHANGE(IDC_LIST1, &CTerrainTab::OnLbnSelchangeTerrainTextureListBox)
END_MESSAGE_MAP()


// CTerrainTab 메시지 처리기입니다.





BOOL CTerrainTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_TerrainTextureListBox.AddString(L"Tile_00");
	m_TerrainTextureListBox.AddString(L"Tile_01");
	m_TerrainTextureListBox.AddString(L"Tile_02");
	m_TerrainTextureListBox.AddString(L"Tile_03");
	m_TerrainTextureListBox.AddString(L"Tile_04");
	m_TerrainTextureListBox.AddString(L"Tile_05");
	m_TerrainTextureListBox.AddString(L"Tile_06");
	m_TerrainTextureListBox.AddString(L"Tile_07");


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTerrainTab::OnLbnSelchangeTerrainTextureListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int index = m_TerrainTextureListBox.GetCurSel();

	
}
