#pragma once
#include "afxcmn.h"



// CControlForm �� ���Դϴ�.

class CTerrainTab;
class CMeshTab;
class CCameraTab;
class CEffectTab;
class CNavigationMeshTab;
class CAnimationTab;
class CUITab;
class CLightTab;
class CControlForm : public CFormView
{
	DECLARE_DYNCREATE(CControlForm)

protected:
	CControlForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CControlForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROLFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	enum TABTYPE
	{
		TAB_TERRAIN, TAB_MESH,TAB_NAVIMESH, TAB_CAMERA, TAB_EFFECT, TAB_ANIMATION,TAB_UI,TAB_LIGHT, TAB_END
	};
	TABTYPE					m_eTabType;

	CTabCtrl				m_Tab;
	CTerrainTab*			m_pTerrainTab;
	CMeshTab*				m_pMeshTab;
	CNavigationMeshTab*		m_pNavigationMeshTab;
	CCameraTab*				m_pCameraTab;
	CEffectTab*				m_pEffectTab;
	CAnimationTab*			m_pAnimationTab;
	CUITab*					m_pUITab;
	CLightTab*				m_pLightTab;

	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void PostNcDestroy();
};


