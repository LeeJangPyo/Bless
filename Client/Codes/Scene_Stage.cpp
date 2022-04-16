#include "stdafx.h"
#include "..\Headers\Scene_Stage.h"

//Management
#include "Management.h"

#include "GameEvent_Manager.h"
#include "Light_Manager.h"

#include "Camera_Dynamic.h"
#include "Camera_Static.h"

#include "Scene_Stage2.h"

#include "Light_Fire.h"

#include "Sound_Manager.h"

_USING(Client)


CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pDeivce)
	:CScene(pDeivce)
{
}

HRESULT Client::CScene_Stage::Ready_Scene()
{
	CGameEvent_Manager::GetInstance()->Set_Stage(STAGE1);

	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	if (FAILED(Ready_GameEvent()))
		return E_FAIL;

	if (FAILED(Ready_Camera()))
		return E_FAIL;
	
	//Camera Set
	CManagement::GetInstance()->Set_CameraMain(CAMERA_STATIC, L"Camera_Static");

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BGM);
	CSound_Manager::GetInstance()->Play_BGM(L"Stage1_BGM.mp3");



	//UI Off

	return S_OK;
}

Engine::_int Client::CScene_Stage::Update_Scene(const _float& fTimeDelta)
{
	if(m_pInput_Device->Get_KeyDown(DIK_F7))
	{
		CManagement::GetInstance()->Set_CameraMain(CAMERA_DYANMIC, L"Camera_Dynamic");
	}
	if(m_pInput_Device->Get_KeyDown(DIK_F8))
	{
		CManagement::GetInstance()->Set_CameraMain(CAMERA_STATIC, L"Camera_Static");
	}



	//Event Update;
	CGameEvent_Manager::GetInstance()->Update_GameEvent(fTimeDelta);

	return CScene::Update_Scene(fTimeDelta);
}

Engine::_int Client::CScene_Stage::LastUpdate_Scene(const _float& fTimeDelta)
{

	if (m_pInput_Device->Get_KeyUp(DIK_F11))
	{
		//Remove
		CManagement::GetInstance()->Remove_Layer_AllObject(SCENE_STAGE, L"Layer_Light_Fire");
		CManagement::GetInstance()->Remove_Layer_AllObject(SCENE_STAGE, L"Layer_Monster_HpBar");
		//CManagement::GetInstance()->Set_CameraMain(CAMERA_STATIC, L"Camera_Static");

		CScene* pScene = nullptr;

		pScene = CScene_Stage2::Create(m_pDevice);

		if (FAILED(CManagement::GetInstance()->SetUp_ScenePointer(pScene)))
			return -1;

		Safe_Release(pScene);

		return S_OK;
	}

	return CScene::LastUpdate_Scene(fTimeDelta);
}

void Client::CScene_Stage::Render_Scene()
{

}

HRESULT CScene_Stage::Ready_LightInfo()
{
	CLight_Manager::GetInstance()->Set_ShadowLightMatrix(&_vec3(64.f, 64.f, 128.f), &_vec3(64.f, 0.f, 64.f), &_vec3(0.f, 1.f, 0.f));
	//CLight_Manager::GetInstance()->Set_ShadowLightMatrix(&_vec3(0.f, 10.f, -10.f), &_vec3(0, 0.f, 0), &_vec3(0.f, 1.f, 0.f));

	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);
	LightInfo.Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	LightInfo.Specular = LightInfo.Diffuse;

	if (FAILED(CLight_Manager::GetInstance()->Ready_Light(m_pDevice, &LightInfo, 0)))
		return E_FAIL;

	//D3DLIGHT9		LightInfo;
	//ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	//LightInfo.Type = D3DLIGHT_POINT;
	//LightInfo.Position = _vec3(64.f, 32.f, 64.f);
	//LightInfo.Range = 128.f;
	//LightInfo.Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	//LightInfo.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	//LightInfo.Specular = LightInfo.Diffuse;

	//if (FAILED(CLight_Manager::GetInstance()->Ready_Light(m_pDevice, &LightInfo, 0)))
	//	return E_FAIL;

	if (FAILED(CLight_Manager::GetInstance()->Load_LightInfo_FromFile(m_pDevice, L"../Bin/Resources/Data/stage1.light")))
		return E_FAIL;

	//SetUP LightFire Gameobjeect  on LightPosition
	const list<CLight*>* pLightList = CLight_Manager::GetInstance()->Get_LightList();
	for (auto& Light : *pLightList)
	{
		CLight_Fire*	pLight_Fire;
		D3DLIGHT9*	pLightInfo = Light->Get_LightInfo();
		_vec3 vLightPosition = pLightInfo->Position;
		if (vLightPosition == _vec3(0.f, 0.f, 0.f))
			continue;

		CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Light_Fire", L"GameObject_Light_Fire", (CGameObject**)&pLight_Fire);
		pLight_Fire->Set_Position(&vLightPosition);
	}

	return S_OK;
}

HRESULT CScene_Stage::Ready_GameEvent()
{
	if (FAILED(CGameEvent_Manager::GetInstance()->Ready_GameEvent()))
		return E_FAIL;

	return S_OK;		
}

HRESULT CScene_Stage::Ready_Camera()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	// For. Camera_Dynamic
	CCamera_Dynamic* pCamera_Dynamic;

	if (FAILED(pManagement->Add_Camera(CAMERA_DYANMIC, L"Camera_Dynamic", L"Camera_Dynamic", (CCamera**)&pCamera_Dynamic)))
		return E_FAIL;

	CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CAMERADESC));

	CameraDesc.vEye = _vec3(0.f, 0.f, 0.f);
	CameraDesc.vAt = _vec3(0.f, 0.f, 1.f);
	CameraDesc.vUp = _vec3(0.f, 1.f, 0.f);

	PROJDESC		ProjectionDesc;
	ZeroMemory(&ProjectionDesc, sizeof(PROJDESC));

	ProjectionDesc.fFovy = D3DXToRadian(60.0f);
	ProjectionDesc.fAspect = _float(g_nBackCX) / g_nBackCY;
	ProjectionDesc.fNear = 0.1f;
	ProjectionDesc.fFar = 1000.0f;

	if (FAILED(pCamera_Dynamic->SetUp_CameraInfo(CameraDesc, ProjectionDesc)))
		return E_FAIL;

	// For. Camera_Static
	CCamera_Static* pCamera_Static;

	if (FAILED(pManagement->Add_Camera(CAMERA_STATIC, L"Camera_Static", L"Camera_Static", (CCamera**)&pCamera_Static)))
		return E_FAIL;

	ZeroMemory(&CameraDesc, sizeof(CAMERADESC));

	CameraDesc.vEye = _vec3(0.f, 20.f, -15.f);
	CameraDesc.vAt = _vec3(0.f, 0.f, 0.f);
	CameraDesc.vUp = _vec3(0.f, 1.f, 0.f);

	ZeroMemory(&ProjectionDesc, sizeof(PROJDESC));

	ProjectionDesc.fFovy = D3DXToRadian(60.0f);
	ProjectionDesc.fAspect = _float(g_nBackCX) / g_nBackCY;
	ProjectionDesc.fNear = 0.1f;
	ProjectionDesc.fFar = 1000.0f;

	if (FAILED(pCamera_Static->SetUp_CameraInfo(CameraDesc, ProjectionDesc)))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}


void Client::CScene_Stage::Free()
{
	CManagement::GetInstance()->Remove_Layer_AllObject(SCENE_STAGE, L"Layer_Terrain");
	CManagement::GetInstance()->Remove_Layer_AllObject(SCENE_STAGE, L"Layer_Static");
	CManagement::GetInstance()->Remove_Layer_AllObject(SCENE_STAGE, L"Layer_Stage1_Combat");
	CManagement::GetInstance()->Remove_Layer_AllObject(SCENE_STAGE, L"Layer_Stage1_Boss");
	CManagement::GetInstance()->Remove_Layer_AllObject(SCENE_STAGE, L"Layer_Effect_Gate");
	

	CScene::Free();
}

Client::CScene_Stage* Client::CScene_Stage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CScene_Stage*		pInstance = new CScene_Stage(pDevice);
	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX(L"CScene_Stage Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
