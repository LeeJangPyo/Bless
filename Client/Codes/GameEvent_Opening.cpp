#include "stdafx.h"
#include "..\Headers\GameEvent_Opening.h"

#include "Management.h"

#include "Camera_Dynamic.h"
#include "Camera_CutScene.h"

_USING(Client)

CGameEvent_Opening::CGameEvent_Opening()
	:m_pCamera_CutScene(nullptr)
{
}

HRESULT CGameEvent_Opening::Ready_GameEvent(const _tchar* pEventTag)
{
	m_wstrEventTag = pEventTag;
	return S_OK;
}



void CGameEvent_Opening::Start_Event()
{
	CManagement::GetInstance()->Set_CameraMain(CAMERA_CUTSCENE, L"Tutorial_Opening", (CCamera**)&m_pCamera_CutScene);

	m_bIsUsed = true;
}

_int CGameEvent_Opening::Update_Event()
{
	if (m_pCamera_CutScene->Get_IsSceneEnd())
	{
		End_Event();
		return -1;
	}
	return 0;
}



void CGameEvent_Opening::End_Event()
{
	CManagement::GetInstance()->Set_CameraMain(CAMERA_STATIC, L"Camera_Static");
}

CGameEvent * CGameEvent_Opening::Create(const _tchar* pEventTag)
{
	CGameEvent_Opening*	pInstance = new CGameEvent_Opening();

	if (FAILED(pInstance->Ready_GameEvent(pEventTag)))
	{
		_MSG_BOX(L"CGameEvent_Opening Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameEvent_Opening::Free()
{
	

}

