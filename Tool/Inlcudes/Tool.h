
// Tool.h : Tool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include "MainApp.h"

// CToolApp:
// �� Ŭ������ ������ ���ؼ��� Tool.cpp�� �����Ͻʽÿ�.
//
_BEGIN(Engine)
class CFrame_Manager;
class CTimer_Manager;

_END

_USING(Client)

class CToolApp : public CWinAppEx
{
public:
	CToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL InitApplication();

	CMainApp*			m_pMainApp = nullptr;
	CFrame_Manager*		m_pFrame_Manager = nullptr;
	CTimer_Manager*		m_pTimer_Manager = nullptr;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CToolApp theApp;
