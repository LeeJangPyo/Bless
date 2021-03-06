#pragma once
#include "afxwin.h"


// CTerrainTab 대화 상자입니다.

class CTerrainTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTerrainTab)

public:
	CTerrainTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTerrainTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnDblclkTerrainTextureList();
	virtual BOOL OnInitDialog();
	CListBox m_TerrainTextureListBox;
	afx_msg void OnLbnSelchangeTerrainTextureListBox();
};
