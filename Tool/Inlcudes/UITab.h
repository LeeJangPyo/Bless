#pragma once
#include "afxwin.h"

#include "afxcmn.h"

// CUITab 대화 상자입니다.

_BEGIN(Client)
class CUI_Frame;
class CUI;
_END


class CUITab : public CDialogEx
{
	DECLARE_DYNAMIC(CUITab)

public:
	CUITab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUITab();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editFrameName;
	CEdit m_editUIName;


	afx_msg void OnBnClickedUiFrameCreate();
	afx_msg void OnBnClickedUiCreate();
	CTreeCtrl m_treeUIList;
	afx_msg void OnTvnSelchangedTreeUiList(NMHDR *pNMHDR, LRESULT *pResult);

	Client::CUI_Frame*	m_pUIFrame;
	Client::CUI*		m_pUI;
	HTREEITEM	m_hItemCurrent;


	_bool		m_bIsLButtonDown;
	CPoint		m_PreviousMousePos;

	virtual BOOL OnInitDialog();
	void	Create_Texture_ItemList(const wstring& wstrFilePath, HTREEITEM& hItem);
	CTreeCtrl m_treeTextureList;
	afx_msg void OnNMDblclkTreeUiTextureList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedUiSave();
	afx_msg void OnBnClickedUiLoad();


	void Update_EditBox(_float fScaleX, _float fScaleY, _float fPositionX, _float fPositionY, _float fDepthZ);
	float m_fScaleX;
	float m_fScaleY;
	float m_fPositionX;
	float m_fPositionY;
	float m_fDepthZ;
	CSliderCtrl m_sliderScaleX;
	CSliderCtrl m_sliderScaleY;
	afx_msg void OnNMReleasedcaptureSliderScaleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderScaleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtTransformApply();
	afx_msg void OnBnClickedBtPareentApply();

	afx_msg void OnBnClickedUiDelete();
	afx_msg void OnBnClickedUiframeChange();
};