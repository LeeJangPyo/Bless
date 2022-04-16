#pragma once
#include "afxwin.h"
#include "afxcmn.h"


#include "UnitTool.h"
// CAnimationTab 대화 상자입니다.

_BEGIN(Engine)
class CCollider_Sphere;
_END

_BEGIN(Client)
class CObject_Dynamic;
class CObject_Equipment;

_END

class CAnimationTab : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimationTab)

public:
	CAnimationTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATION
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CUnitTool			m_UnitTool;

	CObject_Dynamic*	m_pObject_Dynamic;
	CObject_Equipment*	m_pObject_Equipment;
	wstring				m_wstrDynamicMeshTag;
	wstring				m_wstrEquipmentMeshTag;
	unsigned int		m_iMaxAnimation;

	afx_msg void OnBnClickedLoadDynamicMesh();
	afx_msg void OnBnClickedDeleteDynamicMesh();
	afx_msg void OnBnClickedLoadEquipmentObject();
	CListBox m_AnimationListBox;
	afx_msg void OnLbnSelchangePlayAnimationList();
	CTreeCtrl m_TreeMeshFrameList;

	void	InsertMeshFrameList(HTREEITEM hParentItem, D3DXFRAME* pFrame);
//	afx_msg void OnNMDblclkFrameList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOpenUnitTool();
	afx_msg void OnBnClickedDeleteEquipmentObject();

	CString	m_cstrCurrentFrame;

	float m_fColliderX;
	float m_fColliderY;
	float m_fColliderZ;
	float m_fRadius;
	afx_msg void OnBnClickedBtCreateCol();
	afx_msg void OnBnClickedBtDeleteCol();
	CListBox m_ColliderList;

	map<wstring, CCollider_Sphere*>	m_mapCollider;
	CEdit m_editColliderTag;

	CCollider_Sphere*	m_pCurrentCollider = nullptr;
	CString m_cstrCurrentColliderTag;
	afx_msg void OnDeltaposSpinColPosX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinColPosY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinColPosZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinColRadius(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedMeshFrameList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtSave();
	afx_msg void OnBnClickedBtCreateLoad();
};
