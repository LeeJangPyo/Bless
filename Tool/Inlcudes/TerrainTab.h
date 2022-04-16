#pragma once
#include "afxwin.h"


// CTerrainTab ��ȭ �����Դϴ�.

class CTerrainTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTerrainTab)

public:
	CTerrainTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTerrainTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnDblclkTerrainTextureList();
	virtual BOOL OnInitDialog();
	CListBox m_TerrainTextureListBox;
	afx_msg void OnLbnSelchangeTerrainTextureListBox();
};
