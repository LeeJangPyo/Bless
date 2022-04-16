#pragma once
#include "afxwin.h"


_BEGIN(Client)
class CDebug_Spehre;
_END

// CLightTab ��ȭ �����Դϴ�.


class CLightTab : public CDialogEx
{
	DECLARE_DYNAMIC(CLightTab)

public:
	CLightTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLightTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIGHT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void	Set_Position(const _vec3* vPosition);
	vector<D3DLIGHT9>	m_vecLightInfo;
	_uint				m_iLightInfoSize;
	_uint				m_iCurrentIndex;
	CDebug_Spehre*		m_pDebug_Sphere;

	CListBox m_LightList;
	float m_fPositionX;
	float m_fPositionY;
	float m_fPositionZ;
	float m_fDiffuseR;
	float m_fDiffuseG;
	float m_fDiffuseB;
	float m_fDiffuseA;
	float m_fAmbientR;
	float m_fAmbientG;
	float m_fAmbientB;
	float m_fAmbientA;
	float m_fSpecularR;
	float m_fSpecularG;
	float m_fSpecularB;
	float m_fSpecularA;
	float m_fRange;
	afx_msg void OnBnClickedBtLightApply();
	afx_msg void OnBnClickedLightSave();
	afx_msg void OnBnClickedLightLoad();
	afx_msg void OnLbnSelchangeLightList();
	afx_msg void OnBnClickedBtLightCreate();
	afx_msg void OnBnClickedBtLightDelete();
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
};
