#pragma once
#include "afxcmn.h"


// CNavigationMeshTab 대화 상자입니다.

_BEGIN(Engine)
	class CCell;
_END

_BEGIN(Client)
class CMesh_Navigation_Controller;
_END

class CNavigationMeshTab : public CDialogEx
{
	DECLARE_DYNAMIC(CNavigationMeshTab)


public:
	enum DATATYPE
	{
		DATA_SCALE, DATA_POSITION, DATA_ROTATION
	};


	CNavigationMeshTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNavigationMeshTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVIMESH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	Client::CMesh_Navigation_Controller*		m_pNaviMesh_Controller = nullptr;
	CTreeCtrl m_TreeNavigationMeshList;
	afx_msg void OnTvnSelchangedNavigationMeshList(NMHDR *pNMHDR, LRESULT *pResult);
	void	Add_NaviMeshCell(const CCell* pCell, unsigned long dwCellIndex);
	afx_msg void OnBnClickedBtNavigationMeshSave();
	afx_msg void OnBnClickedBtNavigationMeshLoad();
	virtual BOOL OnInitDialog();

	unsigned long					m_dwOldIndex;

	int m_RadioMosueType;
	afx_msg void OnBnClickedInsertPoint();
	afx_msg void OnBnClickedSelectPoint();
	afx_msg void OnBnClickedSelectCell();


	void			Update_Transform(DATATYPE eDataType, const D3DXVECTOR3* pTransform);
	void			Get_Trasnform(DATATYPE eDataTyope, D3DXVECTOR3* pTransform);
	
	float			 m_fSacle[3];
	float			 m_fRotation[3];
	float			 m_fPosition[3];
	afx_msg void OnBnClickedBtRemoveCell();
};
