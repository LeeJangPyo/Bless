
// Tool.h : Tool 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

#include "MainApp.h"

// CToolApp:
// 이 클래스의 구현에 대해서는 Tool.cpp을 참조하십시오.
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


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
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
