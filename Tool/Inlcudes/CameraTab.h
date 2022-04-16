#pragma once
#include "afxcmn.h"
#include "afxwin.h"


_BEGIN(Client)
class CCamera_CutScene;
_END

// CCameraTab 대화 상자입니다.

class CCameraTab : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraTab)

public:
	CCameraTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCameraTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	wstring		m_wstrBaseTag;
	_uint		m_iCount;

	Client::CCamera_CutScene*	m_pCurrentCamera;
	_int				m_iCurrentIndex;
	HTREEITEM			m_hCurrentTreeItem;

	afx_msg void OnBnClickedCameraSave();
	afx_msg void OnBnClickedCameraLoad();
	afx_msg void OnBnClickedCameraCreate();
	afx_msg void OnBnClickedCameraDelete();
	CTreeCtrl m_TreeCutSceneList;
	afx_msg void OnBnClickedCameraAddpoint();
	afx_msg void OnTvnSelchangedTreeCutscene(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_EditCameraTag;
	afx_msg void OnBnClickedCameraApply();
	afx_msg void OnBnClickedCaneraPlay();
	afx_msg void OnBnClickedCameraStop();
	float m_editSpeed;
	afx_msg void OnBnClickedCameraSpeedApply();
	float m_fPosition_X;
	float m_fPosition_Y;
	float m_fPosition_Z;

	void	Update_CameraPosition(const _vec3* pPosition);
};
