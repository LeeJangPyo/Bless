#include "stdafx.h"
#include "MainApp.h"

#include "Management.h"
#include "Input_Device.h"

#include "Scene_MapEdit.h"

#include "Axis.h"
#include "Camera_Dynamic.h"
#include "Camera_CutScene.h"

_USING(Client)

CMainApp::CMainApp(void)
	: m_pManagement(CManagement::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pRenderer(nullptr)
	, m_pDevice(nullptr)
{
	m_pManagement->AddRef();
	m_pInput_Device->AddRef();
}



HRESULT CMainApp::Ready_MainApp(void)
{
	if (FAILED(Ready_Default_Setting(CGraphic_Device::MODE_WIN, g_nBackCX, g_nBackCY)))
	{
		_TAGMSG_BOX(L"Graphic Device Setting is Failed, in Ready_MainApp", L"CMainApp");
		return E_FAIL;
	}


	if (FAILED(Ready_Prototype_Component()))
	{
		_TAGMSG_BOX(L"Ready_Prototype_Component is Failed, in Ready_MainApp", L"CMainApp");
		return E_FAIL;
	}


	if (FAILED(Ready_Prototype_GameObject()))
	{
		_TAGMSG_BOX(L"Ready_Prototype_GameObject Failed, in Ready_MainApp", L"CMainApp");
		return E_FAIL;
	}

	if (FAILED(Ready_GameObject_Layer()))
	{
		_TAGMSG_BOX(L"Ready_GameObject_Layer Failed, in Ready_MainApp", L"CMainApp");
		return E_FAIL;
	}


	if (FAILED(Ready_Start_Scene(SCENE_STATIC)))
	{
		_TAGMSG_BOX(L"Scene Setting is Failed, in Ready_MainApp", L"CMainApp");
		return E_FAIL;
	}

	return S_OK;

}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	if (m_pManagement == nullptr)
	{
		_TAGMSG_BOX(L"Management is invalid, in Update_MainApp", L"CMainApp");
		return -1;
	}
	if (m_pInput_Device == nullptr)
	{
		_TAGMSG_BOX(L"m_pInput_Device is invalid, in Update_MainApp", L"CMainApp");
		return -1;
	}
	m_pInput_Device->SetUp_InputState();

	m_fTimeAcc += fTimeDelta;


	return m_pManagement->Update_Management(fTimeDelta);

}

void CMainApp::Render_MainApp(void)
{
	if (nullptr == m_pDevice ||
		nullptr == m_pManagement ||
		nullptr == m_pRenderer)
		return;

	//m_pDevice->SetRenderState(D3DRS_ZENABLE,)

	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);

	m_pDevice->BeginScene();

	// °´Ã¼µéÀ» ±×¸°´Ù.
	m_pRenderer->Render_ObjectGroup(false,true);

	// ¾ÀÀ»±×¸°´Ù.
	m_pManagement->Render_Management(m_pDevice);

	m_pDevice->EndScene();

	m_pDevice->Present(nullptr, nullptr, 0, nullptr); 

	++m_iNumRenderCall;

	if (1.f <= m_fTimeAcc)
	{
		wsprintf(m_szTmp, L"FPS:%d", m_iNumRenderCall);

		m_iNumRenderCall = 0;
		m_fTimeAcc = 0.f;
	}

	SetWindowText(g_hWnd, m_szTmp);
}

HRESULT CMainApp::Ready_Default_Setting(CGraphic_Device::WINMODE eMode, const _ushort& nSizeX, const _ushort& nSizeY)
{
	// For.Graphic_Device
	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(eMode, g_hWnd, nSizeX, nSizeY, &m_pDevice)))
	{
		_TAGMSG_BOX(L"GraphicDevice Creation is Failed in SetUp_DefaultSetting", L"CMainApp");
		return E_FAIL;
	}

	// For.Input_Device
	if (FAILED(m_pInput_Device->Ready_Input_Device(g_hInst, g_hWnd)))
	{
		_TAGMSG_BOX(L"CInput_Device Creation is Failed in SetUp_DefaultSetting", L"CMainApp");
		return E_FAIL;
	}

	// For.Management 
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

	if (FAILED(CFont_Manager::GetInstance()->Ready_Font(m_pDevice, L"Font_Frame", L"¹ÙÅÁ", 7, FW_NORMAL, 10)))
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
	case SCENE_STATIC:
		pScene = CScene_MapEdit::Create(m_pDevice);
		break;
	}

	if (nullptr == pScene)
		return E_FAIL;

	if (FAILED(m_pManagement->SetUp_ScenePointer(pScene)))
		return E_FAIL;

	Safe_Release(pScene);

	return NOERROR;
}

HRESULT	CMainApp::Ready_Prototype_Component()
{
	if (m_pManagement == nullptr)
	{
		_TAGMSG_BOX(L"pManagement Is Nullptr, in Ready_Prototype_Component", L" CMainApp");
		return E_FAIL;
	}

	//For Renderer Prototype
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Renderer", m_pRenderer = dynamic_cast<CRenderer*>(CRenderer::Create(m_pDevice)))))
		return E_FAIL;
	m_pRenderer->AddRef();

	//For. CubeBufferTex Porototype 
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Buffer_CubeTex", CBuffer_CubeTex::Create(m_pDevice))))
		return E_FAIL;

	//For. Buffer_RcTex Porototype 
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Buffer_RcTex", CBuffer_RcTex::Create(m_pDevice))))
		return E_FAIL;


	//Transform Porototype 
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pDevice))))
		return E_FAIL;

	//Calculator Porototype 
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Calculator", CCalculator::Create(m_pDevice))))
		return E_FAIL;

	//Box_Collider Prototype
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Collider_Box", CCollider_Box::Create(m_pDevice))))
		return E_FAIL;

	//Sphere_Collider Prototype
	if (FAILED(m_pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Collider_Sphere", CCollider_Sphere::Create(m_pDevice))))
		return E_FAIL;



	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	// for. Axis
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Axis", CAxis::Create(m_pDevice))))
		return E_FAIL;

	// For. Camera_Dynamic
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Camera_Dynamic", CCamera_Dynamic::Create(m_pDevice))))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CMainApp::Ready_GameObject_Layer()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	// For. Axis
	if (FAILED(pManagement->Add_Layer(SCENE_STATIC, L"Layer_Axis", L"GameObject_Axis")))
		return E_FAIL;

	// For. Camera
	CCamera_Dynamic*	pCamera;

	if (FAILED(pManagement->Add_Layer(SCENE_STATIC, L"Layer_Camera_Dynamic", L"GameObject_Camera_Dynamic",(CGameObject**)&pCamera)))
		return E_FAIL;

	CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CAMERADESC));

	CameraDesc.vEye = _vec3(0.f, 20.f, -15.f);
	CameraDesc.vAt = _vec3(0.f, 0.f, 0.f);
	CameraDesc.vUp = _vec3(0.f, 1.f, 0.f);

	PROJDESC		ProjectionDesc;
	ZeroMemory(&ProjectionDesc, sizeof(PROJDESC));

	ProjectionDesc.fFovy = D3DXToRadian(60.0f);
	ProjectionDesc.fAspect = _float(g_nBackCX) / g_nBackCY;
	ProjectionDesc.fNear = 0.1f;
	ProjectionDesc.fFar = 1000.0f;

	if (FAILED(pCamera->SetUp_CameraInfo(CameraDesc, ProjectionDesc)))
		return E_FAIL;




	Safe_Release(pManagement);

	return S_OK;
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Ready_MainApp()))
	{
		_MSG_BOX(L"CMainApp Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free(void)
{
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);

	CManagement::Release_Engine();
}

