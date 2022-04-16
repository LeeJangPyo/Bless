#include "stdafx.h"
#include "Scene_Logo.h"

#include "Management.h"
#include "GameEvent_Manager.h"
//Manager
#include "Font_Manager.h"


//Scene
#include "Scene_Stage.h"

//Loader
#include "Loading.h"

//Component
#include "Transform.h"
#include "RcTex.h"
#include "Texture.h"

//Object
#include "Back_Logo.h"
#include "Title_Logo.h"

//Camera
#include "Camera_Default.h"

//Sound
#include "Sound_Manager.h"

_USING(Client)

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
	, m_bLogoAddOnce(false)
	, m_pFont_Manager(CFont_Manager::GetInstance())
{
	m_pFont_Manager->AddRef();
}

HRESULT CScene_Logo::Ready_Scene()
{
	// Add All Prototype Component
	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	// 현재 씬에서 사용할 원형객체를 추가한다.
	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;


	// 현재 씬에서 사용할 객체들을 레이어별로 모아서 복제해놓자.
	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;


	// Loading Object
	m_pLoading = CLoading::Create(m_pDevice, CLoading::LOADING_STAGE);
	if (m_pLoading == nullptr)
		return E_FAIL;

	//Player BGM Sound
	CSound_Manager::GetInstance()->Play_BGM(L"Logo_BGM.mp3");

	CGameEvent_Manager::GetInstance()->Set_Stage(LOGO);

	return S_OK;
}

_int CScene_Logo::Update_Scene(const _float & fTimeDelta)
{
	_int iProcessCode = CScene::Update_Scene(SCENE_LOGO,fTimeDelta);


	return iProcessCode;
}

_int CScene_Logo::LastUpdate_Scene(const _float & fTimeDelta)
{
	if (m_pLoading->Get_IsFinish() == true)
	{
		if (m_bLogoAddOnce == false)
		{
 			// For.Back_Logo
 			if (FAILED(CManagement::GetInstance()->Add_Layer(SCENE_LOGO, L"Layer_BackGround", L"GameObject_Title_Logo")))
 				return E_FAIL;

			m_bLogoAddOnce = true;
		}
		if (m_pInput_Device->Get_KeyUp(DIK_RETURN))
		{
			CScene* pScene = nullptr;

			pScene = CScene_Stage::Create(m_pDevice);

			if (FAILED(CManagement::GetInstance()->SetUp_ScenePointer(pScene)))
				return -1;

			Safe_Release(pScene);

			return SCENE_CHANGE;
		}
	}

	return CScene::LastUpdate_Scene(SCENE_LOGO, fTimeDelta);
}

void CScene_Logo::Render_Scene()
{
	CScene::Render_Scene();


	m_pFont_Manager->Render_Font(L"Font_Default", m_pLoading->Get_String(), &_vec2(0.f, 0.f),D3DXCOLOR(1.f,0.f,0.f,1.f));

}

HRESULT CScene_Logo::Ready_Prototype_GameObject()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	// For.GameObject_Back_Logo
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Back_Logo", CBack_Logo::Create(m_pDevice))))
		return E_FAIL;

	// For.GameObject_Back_Logo
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Title_Logo", CTitle_Logo::Create(m_pDevice))))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CScene_Logo::Ready_Prototype_Component()
{

	CManagement*		pManagement = CManagement::GetInstance();
	
	if (pManagement == nullptr)
	{
		_TAGMSG_BOX(L"pManagement is nullptr, In Ready ProtoType_Component", L"CScene_Logo");
		return E_FAIL;
	}
	pManagement->AddRef();

	// Shader Porotype Create
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_LOGO, L"Component_Shader_Logo",
		CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_Logo.fx"))))
		return E_FAIL;

	////Texture Porototype Create;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_LOGO, L"Component_Texture_BackLogo",
		CTexture::Create(m_pDevice,L"../Bin/Resources/Texture/LogoBack/Back_%d.png",CTexture::TEXTURETYPE::TEX_NORMAL,5))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_Component(SCENE_LOGO, L"Component_Texture_TitleLogo",
		CTexture::Create(m_pDevice, L"../Bin/Resources/Texture/LogoBless/Bless_%d.png", CTexture::TEXTURETYPE::TEX_NORMAL, 88))))
		return E_FAIL;

	//Camera
	if (FAILED(pManagement->Add_Prototype_Camera(L"Camera_Default", CCamera_Default::Create(m_pDevice))))
		return E_FAIL;


	Safe_Release(pManagement);


	return S_OK;
}

HRESULT CScene_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{

	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	// For.Back_Logo
	if (FAILED(pManagement->Add_Layer(SCENE_LOGO, pLayerTag, L"GameObject_Back_Logo")))
		return E_FAIL;

	Ready_Camera();

	Safe_Release(pManagement);

	return NOERROR;
}

HRESULT CScene_Logo::Ready_Camera()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	// For. Camera_Dynamic
	CCamera_Default* pCamera_Default;

	if (FAILED(pManagement->Add_Camera(CAMERA_DEFAULT, L"Camera_Default", L"Camera_Default", (CCamera**)&pCamera_Default)))
		return E_FAIL;

	CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CAMERADESC));

	CameraDesc.vEye = _vec3(0.f, 0.f, 0.f);
	CameraDesc.vAt = _vec3(0.f, 0.f, 1.f);
	CameraDesc.vUp = _vec3(0.f, 1.f, 0.f);

	PROJDESC		ProjectionDesc;
	ZeroMemory(&ProjectionDesc, sizeof(PROJDESC));

	ProjectionDesc.fFovy = D3DXToRadian(90.0f);
	ProjectionDesc.fAspect = 1.f;
	ProjectionDesc.fNear = 0.1f;
	ProjectionDesc.fFar = 1000.0f;

	if (FAILED(pCamera_Default->SetUp_CameraInfo(CameraDesc, ProjectionDesc)))
		return E_FAIL;

	Safe_Release(pManagement);

	//Camera Set
	CManagement::GetInstance()->Set_CameraMain(CAMERA_STATIC, L"Camera_Default");

	return S_OK;
}

CScene_Logo * CScene_Logo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CScene_Logo*		pInstance = new CScene_Logo(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX(L"CScene_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Logo::Free()
{
	CManagement::GetInstance()->Clear_Scene_Container(SCENE_LOGO);
		
	Safe_Release(m_pLoading);
	Safe_Release(m_pFont_Manager);

	CScene::Free();
}
