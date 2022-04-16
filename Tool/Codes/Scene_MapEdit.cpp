#include "stdafx.h"
#include "Scene_MapEdit.h"

#include "Management.h"

//오브젝트
#include "Axis.h"
#include "Terrain.h"

#include "Camera_Dynamic.h"
#include "Camera_CutScene.h"

#include "Mesh_Navigation_Controller.h"

#include "Object_Static.h"
#include "Object_Dynamic.h"


//ui
#include "UI_Frame.h"

//Paritcle
#include "Effect_Particle.h"

//
#include "Debug_Spehre.h"

_USING(Client)

CScene_MapEdit::CScene_MapEdit(LPDIRECT3DDEVICE9 pDevice)
	:CScene(pDevice)
{

}

HRESULT CScene_MapEdit::Ready_Scene(void)
{

	// 현재 씬에서 사용할 원형객체를 추가한다.
	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	// 현재 씬에서 사용할 원형객체를 추가한다.
	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	//레이어 추가
	if (FAILED(Ready_GameObject_Layer()))
		return E_FAIL;

	return S_OK;
}

_int CScene_MapEdit::Update_Scene(const _float& fTimeDelta)
{
	_int iProcessCode = CScene::Update_Scene(fTimeDelta);
	return iProcessCode;
}

_int CScene_MapEdit::LastUpdate_Scene(const _float & fTimeDelta)
{
	_int iProcessCode = CScene::LastUpdate_Scene(fTimeDelta);
	return iProcessCode;
}

void CScene_MapEdit::Render_Scene(void)
{
	//Engine::Render_Font(L"Font_Default",L"1번", &_vec3(1.f, 1.f,1.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	
}


HRESULT CScene_MapEdit::Ready_Prototype_GameObject()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (pManagement == nullptr)
	{
		_TAGMSG_BOX(L"pManagement is nullptr, In Ready Ready_Stage_Prototype_GameObject", L"CLoading");
		return E_FAIL;
	}
	pManagement->AddRef();


	// For. GameObject_Terrain
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Terrain", CTerrain::Create(m_pDevice))))
		return E_FAIL;


	// For. GameObject_NaviMesh
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_NaviMesh_Controller", CMesh_Navigation_Controller::Create(m_pDevice))))
		return E_FAIL;

	//// For. GameObject_Object_Static
	//if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Object_Static", CObject_Static::Create(m_pDevice))))
	//	return E_FAIL;

	// For. Camera_CutScene
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Camera_CutScene", CCamera_CutScene::Create(m_pDevice))))
		return E_FAIL;

	// For. UI_Frame
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_UI_Frame", CUI_Frame::Create(m_pDevice))))
		return E_FAIL;

	// For. UI_Frame
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Debug_Sphere", CDebug_Spehre::Create(m_pDevice))))
		return E_FAIL;


	Safe_Release(pManagement);
	

	return S_OK;
}

HRESULT CScene_MapEdit::Ready_Prototype_Component()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	//For. Mesh_Navigation Prototype
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_EDIT, L"Component_Mesh_Navigation", CMesh_Navigation::Create(m_pDevice))))
		return E_FAIL;


	//For. Texture Porototype Component Create;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_EDIT, L"Component_Texture_Terrain",
		CTexture::Create(m_pDevice, L"../Bin/Resources/Texture/Terrain/Tile_D_%d.tga", CTexture::TEXTURETYPE::TEX_NORMAL, 8))))
		return E_FAIL;

	//For. Texture Porototype Component Create;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_EDIT, L"Component_Texture_Default",
		CTexture::Create(m_pDevice, L"../Bin/Resources/hpbar.png", CTexture::TEXTURETYPE::TEX_NORMAL, 1))))
		return E_FAIL;

	//For. Texture Porototype Component Create;
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_EDIT, L"Component_Texture_Alpha",
		CTexture::Create(m_pDevice, L"../Bin/Resources/Texture/alpha.png", CTexture::TEXTURETYPE::TEX_NORMAL, 1))))
		return E_FAIL;





	// For. Terrain Buffer
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_EDIT, L"Component_Buffer_Terrain",
		CBuffer_TerrainTex::Create(m_pDevice, VTXCNTX, VTXCNTZ, VTXITV))))
		return E_FAIL;

	// For. Shader_Default
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_STATIC, L"Component_Shader_Default",
		CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_Deafult.fx"))))
		return E_FAIL;

	// For. Shader_UI
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_EDIT, L"Component_Shader_UI",
		CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_UI.fx"))))
		return E_FAIL;

	// For. Shader_Effect
	if (FAILED(pManagement->Add_Prototype_Component(SCENE_EDIT, L"Component_Shader_Effect",
		CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_Effect.fx"))))
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CScene_MapEdit::Ready_GameObject_Layer()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	// For. Terrain
	if (FAILED(pManagement->Add_Layer(SCENE_EDIT, L"Layer_Terrain", L"GameObject_Terrain")))
		return E_FAIL;

	// For. NaviMesh_Controller
	if (FAILED(pManagement->Add_Layer(SCENE_EDIT, L"Layer_NaviMesh_Controller", L"GameObject_NaviMesh_Controller")))
		return E_FAIL;

	//// For. UI_Frame
	//if (FAILED(pManagement->Add_Layer(SCENE_EDIT, L"Layer_UI_Frame", L"GameObject_UI_Frame")))
	//	return E_FAIL;




	Safe_Release(pManagement);

	return S_OK;
}


CScene_MapEdit* CScene_MapEdit::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CScene_MapEdit*		pInstance = new CScene_MapEdit(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX(L"CScene_MapEdit Create Failed");
		Safe_Release(pInstance);
	}


	return pInstance;
}

void CScene_MapEdit::Free(void)
{
	CScene::Free();
}

