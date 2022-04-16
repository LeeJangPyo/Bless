#include "stdafx.h"
#include "..\Headers\Scene_Stage2.h"

//Management
#include "Management.h"
#include "Graphic_Device.h"


#include "GameEvent_Manager.h"
#include "Light_Manager.h"

#include "Camera_Dynamic.h"
#include "Camera_Static.h"

#include "GameObject_Static.h"
#include "Player_Manager.h"

#include "GameEvent_Manager.h"

#include "Light_Fire.h"


//Soung

#include "Sound_Manager.h"

_USING(Client)

CScene_Stage2::CScene_Stage2(LPDIRECT3DDEVICE9 pDeivce)
	:CScene(pDeivce)
{

}

HRESULT CScene_Stage2::Ready_Scene()
{
	//Camera Set
	CManagement::GetInstance()->Set_CameraMain(CAMERA_STATIC, L"Camera_Static");

	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	if (FAILED(Ready_GameEvent()))
		return E_FAIL;


	// For.Terrain2
	if (FAILED(CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Terrain_Stage2", L"GameObject_Terrain_Stage2")))
		return E_FAIL;

	//
	//Load Static Mesh
	if (FAILED(Load_GameObject_Static_FromFile(L"../Bin/Resources/Data/stage2.smesh")))
		return E_FAIL;

	//Load Dynamic Mesh
	if (FAILED(Load_GameObject_Dynamic_FromFile(L"../Bin/Resources/Data/Stage2_Combat.dmesh")))
		return E_FAIL;

	if (FAILED(Load_GameObject_Dynamic_FromFile(L"../Bin/Resources/Data/Stage2_Flogas.dmesh")))
		return E_FAIL;

	if (FAILED(Load_GameObject_Dynamic_FromFile(L"../Bin/Resources/Data/Stage2_Quatra.dmesh")))
		return E_FAIL;



	//EventManager Stages Setting;
	CGameEvent_Manager::GetInstance()->Set_Stage(STAGE2);

	//Player Setting
	//Player NaviMesh Change;
	CPlayer_Manager* pPlayer_Manager = CPlayer_Manager::GetInstance();
	pPlayer_Manager->AddRef();
	{
		pPlayer_Manager->Change_Player_NaviMesh();

		pPlayer_Manager->Get_Player(PLAYER_LUPS)->Get_Transform()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(128.f, 0.f, 30.f));
		pPlayer_Manager->Get_Player(PLAYER_LUPS)->Get_Transform()->Rotation_Y(90.f, 1.f);
		pPlayer_Manager->Get_Player(PLAYER_LUPS)->Compute_Position_OnNaviMeshIndex();

		pPlayer_Manager->Get_Player(PLAYER_SIRE)->Get_Transform()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(128.f, 0.f, 20.f));
		pPlayer_Manager->Get_Player(PLAYER_SIRE)->Compute_Position_OnNaviMeshIndex();
		pPlayer_Manager->Get_Player(PLAYER_SIRE)->Change_State(L"AI");

		pPlayer_Manager->Set_Character(PLAYER_LUPS);


	}
	Safe_Release(pPlayer_Manager);

	//Monster State Set
	const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_Stage2_Combat");
	if (pGameObjectList == nullptr)
		return E_FAIL;

	for (auto& pGameObject : *pGameObjectList)
	{
		CGameObject_Dynamic* pMonster = dynamic_cast<CGameObject_Dynamic*>(pGameObject);
		pMonster->Change_NavigationMesh();
		pMonster->Compute_Position_OnNaviMeshIndex();
	}
	//Monster State Set
	pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_Stage2_Flogas");
	if (pGameObjectList == nullptr)
		return E_FAIL;

	for (auto& pGameObject : *pGameObjectList)
	{
		CGameObject_Dynamic* pMonster = dynamic_cast<CGameObject_Dynamic*>(pGameObject);
		pMonster->Change_NavigationMesh();
		pMonster->Compute_Position_OnNaviMeshIndex();
	}

	//Sound
	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BGM);
	CSound_Manager::GetInstance()->Play_BGM(L"Stage2_BGM.mp3");

	return S_OK;
}

_int CScene_Stage2::Update_Scene(const _float & fTimeDelta)
{
	if (m_pInput_Device->Get_KeyDown(DIK_F9))
	{
		CManagement::GetInstance()->Remove_Layer_AllObject(SCENE_STAGE, L"Layer_Stage2_Combat");
	}
	if (m_pInput_Device->Get_KeyDown(DIK_F7))
	{
		CManagement::GetInstance()->Set_CameraMain(CAMERA_DYANMIC, L"Camera_Dynamic");
	}
	if (m_pInput_Device->Get_KeyDown(DIK_F8))
	{
		CManagement::GetInstance()->Set_CameraMain(CAMERA_STATIC, L"Camera_Static");
	}

	//Event Update;
	CGameEvent_Manager::GetInstance()->Update_GameEvent(fTimeDelta);

	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Stage2::LastUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LastUpdate_Scene(fTimeDelta);
}

void CScene_Stage2::Render_Scene()
{
}

HRESULT CScene_Stage2::Ready_LightInfo()
{
	//
	CLight_Manager::GetInstance()->Remove_All_Light();

	CLight_Manager::GetInstance()->Set_ShadowLightMatrix(&_vec3(128, 32.f, 256.f), &_vec3(128.f, 0.f, 128.f), &_vec3(0.f, 1.f, 0.f));

	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);
	LightInfo.Diffuse = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = LightInfo.Diffuse;

	if (FAILED(CLight_Manager::GetInstance()->Ready_Light(m_pDevice, &LightInfo, 0)))
		return E_FAIL;


	if (FAILED(CLight_Manager::GetInstance()->Load_LightInfo_FromFile(m_pDevice, L"../Bin/Resources/Data/stage2.light")))
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

HRESULT CScene_Stage2::Ready_GameEvent()
{
	return S_OK;
}


HRESULT CScene_Stage2::Create_Prototype_Component_Mesh(OBJECTTYPE eType, const _tchar * pFilePath, const _tchar * pFileName, const _tchar * pComponentMeshTag)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	pManagement->AddRef();

	LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	if (pDevice == nullptr)
	{
		Safe_Release(pManagement);
		return E_FAIL;
	}
	pDevice->AddRef();

	if (FAILED(pManagement->Find_Prototype_Component(SCENE_STAGE, pComponentMeshTag)))
	{
		Safe_Release(pDevice);
		Safe_Release(pManagement);
		return E_FAIL;
	}

	switch (eType)
	{
	case MESH_DYNAMIC:
		pManagement->Add_Prototype_Component(SCENE_STAGE, pComponentMeshTag,
			CMesh_Dynamic::Create(pDevice, pFilePath, pFileName));
		break;
	case MESH_STATIC:
		pManagement->Add_Prototype_Component(SCENE_STAGE, pComponentMeshTag,
			CMesh_Static::Create(pDevice, pFilePath, pFileName));
		break;
	}

	Safe_Release(pManagement);
	Safe_Release(pDevice);

	return S_OK;
}

HRESULT CScene_Stage2::Create_Prototype_GameObject(const _tchar * pGameObjectTag)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}
	pManagement->AddRef();

	LPDIRECT3DDEVICE9 pDevice = CGraphic_Device::GetInstance()->Get_Graphic_Device();
	if (pDevice == nullptr)
	{
		Safe_Release(pManagement);
		return E_FAIL;
	}
	pDevice->AddRef();

	if (FAILED(pManagement->Find_Prototype_GameObject(pGameObjectTag)))
	{
		Safe_Release(pDevice);
		Safe_Release(pManagement);
		return E_FAIL;
	}

	if (FAILED(pManagement->Add_Prototype_GameObject(pGameObjectTag, CGameObject_Static::Create(pDevice))))
	{
		Safe_Release(pDevice);
		Safe_Release(pManagement);
		return E_FAIL;
	}

	Safe_Release(pDevice);
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CScene_Stage2::Add_Layer_GameObject(const _tchar* pLayerTag, const _tchar * pGameObjectTag, CGameObject ** ppGameObject)
{

	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		if (FAILED(pManagement->Add_Layer(SCENE_STAGE, pLayerTag, pGameObjectTag, ppGameObject)))
		{
			Safe_Release(pManagement);
			return E_FAIL;
		}
	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CScene_Stage2::Load_GameObject_Static_FromFile(const _tchar* pFilePath)
{
	HANDLE		hFile = CreateFile(pFilePath //L"../Bin/Resources/Data/test.smesh"
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	DWORD	dwByte = 0;
	_ulong	dwIndex = 0;

	while (true)
	{
		//월드 매트릭스 로드
		_matrix matWorld;
		ReadFile(hFile, &matWorld, sizeof(_matrix), &dwByte, nullptr);


		//파일경로
		_tchar szFilePath[MIN_PATH] = L"";
		ReadFile(hFile, szFilePath, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//파일이름
		_tchar szFileName[MIN_PATH] = L"";
		ReadFile(hFile, szFileName, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//오브젝트 테그
		_tchar szGameObjectTag[MIN_PATH] = L"";
		ReadFile(hFile, szGameObjectTag, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//컴포넌트매쉬태그 읽기
		_tchar szComponentMeshTag[MIN_PATH] = L"";
		ReadFile(hFile, szComponentMeshTag, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);

		if (dwByte == 0)
		{
			break;
		}

		wstring wstrComponentMeshTag = szComponentMeshTag;
		wstring wstrGameObjectTag = szGameObjectTag;
		wstring wstrFileName = szFileName;
		wstring wstrFilePath = szFilePath;

		//	wstrFileName = wstrFileName.sub
		//////////////////////////////////////////////////////////////////////////
		//Create Static Mesh
		CGameObject_Static* pGameObject_Static;
		Create_Prototype_Component_Mesh(MESH_STATIC, wstrFilePath.c_str(), wstrFileName.c_str(), wstrComponentMeshTag.c_str());
		Create_Prototype_GameObject(wstrGameObjectTag.c_str());

		if (FAILED(Add_Layer_GameObject(L"Layer_Static_Stage2", wstrGameObjectTag.c_str(), (CGameObject**)&pGameObject_Static)))
			break;

		//Late SetUp Staic Object
		pGameObject_Static->SetUp_GameObjectInfo(&matWorld, wstrComponentMeshTag.c_str());
		//	pGameObject_Static->SetUp_FileInfo(wstrFilePath.c_str(), wstrFileName.c_str(), wstrGameObjectTag.c_str(), wstrComponentMeshTag.c_str());

	}
	CloseHandle(hFile);


	return S_OK;
}

HRESULT CScene_Stage2::Load_GameObject_Dynamic_FromFile(const _tchar * pFilePath)
{
	HANDLE		hFile = CreateFile(pFilePath //L"../Bin/Resources/Data/test.smesh"
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	wstring wstrLayerTag = PathFindFileName(pFilePath);
	PathRemoveExtension((LPWSTR)wstrLayerTag.c_str());
	wstrLayerTag = L"Layer_" + wstrLayerTag;

	DWORD	dwByte = 0;

	while (true)
	{
		//월드 매트릭스 로드
		_matrix matWorld;
		ReadFile(hFile, &matWorld, sizeof(_matrix), &dwByte, nullptr);


		//파일경로
		_tchar szFilePath[MIN_PATH] = L"";
		ReadFile(hFile, szFilePath, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//파일이름
		_tchar szFileName[MIN_PATH] = L"";
		ReadFile(hFile, szFileName, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//오브젝트 테그
		_tchar szGameObjectTag[MIN_PATH] = L"";
		ReadFile(hFile, szGameObjectTag, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);
		//컴포넌트매쉬태그 읽기
		_tchar szComponentMeshTag[MIN_PATH] = L"";
		ReadFile(hFile, szComponentMeshTag, sizeof(_tchar)*MIN_PATH, &dwByte, nullptr);

		if (dwByte == 0)
		{
			break;
		}

		wstring wstrComponentMeshTag = szComponentMeshTag;
		wstring wstrGameObjectTag = szGameObjectTag;
		wstring wstrFileName = szFileName;
		wstring wstrFilePath = szFilePath;

		//	wstrFileName = wstrFileName.sub
		//////////////////////////////////////////////////////////////////////////
		//Create Dynamic Mesh
		CGameObject_Dynamic* pGameObject_Dynamic;
		if (FAILED(Add_Layer_GameObject(wstrLayerTag.c_str(), wstrGameObjectTag.c_str(), (CGameObject**)&pGameObject_Dynamic)))
			return E_FAIL;
		//Late SetUp Staic Object
		pGameObject_Dynamic->SetUp_GameObjectInfo(&matWorld);

	}
	CloseHandle(hFile);
	return S_OK;
}



CScene_Stage2 * CScene_Stage2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CScene_Stage2*		pInstance = new CScene_Stage2(pDevice);
	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX(L"CScene_Stage2 Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Stage2::Free()
{
	CScene::Free();
}
