// TerrainTab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "TerrainTab.h"
#include "afxdialogex.h"


// CTerrainTab ��ȭ �����Դϴ�.

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


// CTerrainTab �޽��� ó�����Դϴ�.





BOOL CTerrainTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_TerrainTextureListBox.AddString(L"Tile_00");
	m_TerrainTextureListBox.AddString(L"Tile_01");
	m_TerrainTextureListBox.AddString(L"Tile_02");
	m_TerrainTextureListBox.AddString(L"Tile_03");
	m_TerrainTextureListBox.AddString(L"Tile_04");
	m_TerrainTextureListBox.AddString(L"Tile_05");
	m_TerrainTextureListBox.AddString(L"Tile_06");
	m_TerrainTextureListBox.AddString(L"Tile_07");


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTerrainTab::OnLbnSelchangeTerrainTextureListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int index = m_TerrainTextureListBox.GetCurSel();

	
}
