#include "stdafx.h"
#include "MainApp.h"
#include "Graphic_Device.h"
#include "Management.h"
#include "Scene_Logo.h"
#include "Input_Device.h"
#include "GameEvent_Manager.h"
#include "Collision_Manager.h"
#include "Player_Manager.h"
#include "Sound_Manager.h"

#include "Camera_Static.h"
_USING(Client)

CMainApp::CMainApp()
	: m_pManagement(CManagement::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
{
	m_pManagement->AddRef();
	m_pInput_Device->AddRef();
}

HRESULT CMainApp::Ready_MainApp()
{

	//Hide Mouse Cursur
	//ShowCursor(false);

	if (FAILED(Ready_Default_Setting(CGraphic_Device::MODE_WIN, g_nBackCX, g_nBackCY)))
	{
		_TAGMSG_BOX(L"Graphic Device Setting is Failed, in Ready_MainApp", L"CMainApp");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_Component()))
	{
		_TAGMSG_BOX(L"Graphic Device Setting is Failed, in Ready_MainApp", L"CMainApp");
		return E_FAIL;
	}

	if (FAILED(Ready_Start_Scene(SCENE_LOGO)))
	{
		_TAGMSG_BOX(L"Scene Setting is Failed, in Ready_MainApp", L"CMainApp");
		return E_FAIL;
	}
		
	return S_OK;
}

_int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	if (m_pManagement == nullptr)
	{
		_TAGMSG_BOX(L"Management is invalid, in Update_MainApp", L"CMainApp");
		return -1;
	}
	if(m_pInput_Device == nullptr)
	{
		_TAGMSG_BOX(L"m_pInput_Device is invalid, in Update_MainApp", L"CMainApp");
		return -1;
	}

	//Keyboard Update
	m_pInput_Device->SetUp_InputState();
	//Sound Update
	CSound_Manager::GetInstance()->Update_Sound();

	m_fTimeAcc += fTimeDelta;

	if (m_pInput_Device->Get_KeyUp(DIK_F4))
		g_bDebug = !g_bDebug;

	if (m_pInput_Device->Get_KeyUp(DIK_F3))
		g_bDefferd = !g_bDefferd;


	return m_pManagement->Update_Management(fTimeDelta);
}

void CMainApp::Render_MainApp()
{
	if (m_pDevice == nullptr ||
		m_pManagement == nullptr)
	{
		_TAGMSG_BOX(L"Management or m_pDevice is not invalid, in Update_MainApp", L"CMainApp");
		return;
	}

	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 1.f, 0);

	m_pDevice->BeginScene();

	//Object Draw
	if(CGameEvent_Manager::GetInstance()->Get_Stage() == LOGO)
		m_pRenderer->Render_ObjectGroup(false, true);
	else
		m_pRenderer->Render_ObjectGroup(g_bDebug,g_bDefferd);
	
	//Scene Draw
	m_pManagement->Render_Management(m_pDevice);

	m_pDevice->EndScene();

	m_pDevice->Present(nullptr, nullptr, 0, nullptr);


//	++m_iNumRenderCall;
//
//	if (1.f <= m_fTimeAcc)
//	{
//		wsprintf(m_szTmp, L"FPS:%d", m_iNumRenderCall);
// 
//		m_iNumRenderCall = 0;
//		m_fTimeAcc = 0.f;
//	}
//
//	SetWindowText(g_hWnd, m_szTmp);
}

HRESULT CMainApp::Ready_Default_Setting(CGraphic_Device::WINMODE eMode, const _ushort& nSizeX, const _ushort& nSizeY)
{
	// For.Graphic_Device
	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(eMode, g_hWnd, nSizeX, nSizeY, &m_pDevice)))
	{
		_TAGMSG_BOX(L"GraphicDevice Creation is Failed in SetUp_DefaultSetting", L"CMainApp");
		return E_FAIL;
	}
	// Input_Device
	if (FAILED(m_pInput_Device->Ready_Input_Device(g_hInst,g_hWnd)))
	{
		_TAGMSG_BOX(L"CInput_Device Creation is Failed in SetUp_DefaultSetting", L"CMainApp");
		return E_FAIL;
	}

	// Ready_Sound_Manager
	CSound_Manager::GetInstance()->Ready_Sound();

	// Management 
	if (FAILED(m_pManagement->Ready_Management(SCENE_END,CAMERA_END)))
	{
		_TAGMSG_BOX(L"Management Creation is Failed in Ready_OpeningScene", L"CMainApp");
		return E_FAIL;
	}


	// Ready_Font
	if (FAILED(CFont_Manager::GetInstance()->Ready_Font(m_pDevice,
		L"Font_Default",
		L"¹ÙÅÁ",
		15,
		FW_HEAVY,
		20)))
		return E_FAIL;



	return S_OK;
}

HRESULT CMainApp::Ready_Start_Scene(SCENEID eSceneID)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	CScene*			pScene = nullptr;

	switch (eSceneID)
	{
	case SCENE_LOGO:
		pScene = CScene_Logo::Create(m_pDevice);
		break;
	case SCENE_STAGE:
		//pScene = CScene_Logo::Create(m_pDevice);
		break;
	}

	if (nullptr == pScene)
		return E_FAIL;

	if (FAILED(m_pManagement->SetUp_ScenePointer(pScene)))
		return E_FAIL;

	Safe_Release(pScene);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (m_pManagement == nullptr)
	{
		_TAGMSG_BOX(L"pManagement Is Nullptr, in Ready_Prototype_Component", L" CMainApp");
		return E_FAIL;
	}

	//Renderer Clone
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Renderer", m_pRenderer = dynamic_cast<CRenderer*>(CRenderer::Create(m_pDevice)))))
		return E_FAIL;
	m_pRenderer->AddRef();

	// For. Transform Porototype 
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pDevice))))
		return E_FAIL;

	// For. Calculator Porototype 
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Calculator", CCalculator::Create(m_pDevice))))
		return E_FAIL;

	// For.Box_Collider Prototype
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Collider_Box", CCollider_Box::Create(m_pDevice))))
		return E_FAIL;

	// For.Sphere_Collider Prototype
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Collider_Sphere", CCollider_Sphere::Create(m_pDevice))))
		return E_FAIL;

	// For. Default_Shader Prototype
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_Default", CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_Deafult.fx"))))
		return E_FAIL;

	// For. NaviGation Mesh
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Mesh_Navigation", CMesh_Navigation::Create(m_pDevice))))
		return E_FAIL;

	// For. Buffer_RcTex
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Buffer_RcTex", CBuffer_RcTex::Create(m_pDevice))))
		return E_FAIL;

	// For. Optimization
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Optimization", COptimization::Create(m_pDevice,false, STAGE1_VTXCNTX, STAGE1_VTXCNTZ))))
		return E_FAIL;


	return S_OK;
}


CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Ready_MainApp()))
	{
		_MSG_BOX(L"CMainApp Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pRenderer);

	_ulong		dwRefCnt = 0;


	//½Ì±ÛÅæ »èÁ¦
	CGameEvent_Manager::GetInstance()->DestroyInstance();
	CCollision_Manager::GetInstance()->DestroyInstance();
	CPlayer_Manager::GetInstance()->DestroyInstance();
	

	CManagement::Release_Engine();	


}



