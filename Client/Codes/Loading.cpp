#include "stdafx.h"
#include "..\Headers\Loading.h"
#include <process.h>

#include <shlwapi.h>
//
#include "Management.h"

#include "Graphic_Device.h"
#include "UI_Resources_Loader.h"

//GameObject
#include "GameObject_Static.h"
#include "Terrain.h"
#include "Terrain_Stage2.h"
#include "SkyBox.h"

//Player
#include "Lups.h"
#include "Sire.h"

//Weapon
#include "Sword.h"
#include "Shield.h"
#include "Staff.h"

//Monster
#include "Ghost_Soldier.h"
#include "Ghost_Mage.h"
#include "Skeleton.h"
#include "Balrog.h"
#include "Quatra.h"
#include "Flogas.h"

//ETC OBject
#include "Direction_Arrow.h"


#include "Camera_Static.h"
#include "Camera_Dynamic.h"
#include "Camera_CutScene.h"

//Effect
#include "IceBolt.h"
#include "Meteo_Strike.h"
#include "EffectGroup.h"
#include "Trail.h"
#include "Light_Fire.h"
#include "Blood.h"
#include "HitFire.h"
#include "ExclamationMark.h"
#include "Effect_QuatraBreath.h"

//UI
#include "Monster_HpBar.h"
#include "UI_Frame.h"
#include "Damage.h"

_USING(Client)

CLoading::CLoading(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
	,m_bIsFinish(false)
{
	m_pDevice->AddRef();
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_CrticalSection);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, nullptr);

	m_eLoading = eLoadingID;

	return S_OK;
}

Engine::_uint CLoading::Loading_ForStage(void)
{
	lstrcpy(m_szLoadingString, L"Component Loading");

	if (FAILED(Ready_Stage_Prototype_Component()))
		return -1;

	lstrcpy(m_szLoadingString, L"GameObject Loading");

	if (FAILED(Ready_Stage_Prototype_GameObject()))
		return -1;

	lstrcpy(m_szLoadingString, L"GameObject Copy For Layer");

	if (FAILED(Ready_Stage_GameObject_Layer()))
		return -1;

	lstrcpy(m_szLoadingString, L"Loading From File");

	if (FAILED(Loading_StageInfoFormFile()))
		return -1;
	lstrcpy(m_szLoadingString, L"Loading Complete");


	m_bIsFinish = true;

	return 0;
}



HRESULT CLoading::Loading_StageInfoFormFile()
{
	//Load Static Mesh
	if (FAILED(Load_GameObject_Static_FromFile(L"../Bin/Resources/Data/stage1.smesh")))
		return E_FAIL;

	//Load Dynamic Mesh
	if (FAILED(Load_GameObject_Dynamic_FromFile(L"../Bin/Resources/Data/Stage1_Combat.dmesh")))
		return E_FAIL;

	if (FAILED(Load_GameObject_Dynamic_FromFile(L"../Bin/Resources/Data/Stage1_Boss.dmesh")))
		return E_FAIL;

	//Load CutScene 
	//Stage 1
	if (FAILED(Load_Camera_CutScene_FromFile(L"../Bin/Resources/Data/Stage1_Opening.scene")))
		return E_FAIL;

	if (FAILED(Load_Camera_CutScene_FromFile(L"../Bin/Resources/Data/Stage1_Combat_1.scene")))
		return E_FAIL;

	if (FAILED(Load_Camera_CutScene_FromFile(L"../Bin/Resources/Data/Stage1_Boss.scene")))
		return E_FAIL;

	//Load CutScene 
	//Stage2
	if (FAILED(Load_Camera_CutScene_FromFile(L"../Bin/Resources/Data/Stage2_Opening.scene")))
		return E_FAIL;

	if (FAILED(Load_Camera_CutScene_FromFile(L"../Bin/Resources/Data/Stage2_Quatra.scene")))
		return E_FAIL;

	if (FAILED(Load_Camera_CutScene_FromFile(L"../Bin/Resources/Data/Stage2_Flogas.scene")))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading::Loading_EffectFile()
{
	//Effect Load
	// Lups
	// Effect 0
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Lups_Shield_Smash.eff", L"Layer_Effect_Lups"))))
		return E_FAIL;

	// Effect 1
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Lups_Spin_Attack.eff", L"Layer_Effect_Lups"))))
		return E_FAIL;

	// Effect 2
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Lups_Taunt.eff", L"Layer_Effect_Lups"))))
		return E_FAIL;

	// Effect 3
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Lups_Guard.eff", L"Layer_Effect_Lups"))))
		return E_FAIL;


	//// Sire
	//// 3순위 Effect 0
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Sire_FireOrb.eff", L"Layer_Effect_Sire"))))
		return E_FAIL;

	// 3순위 Effect 1
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Sire_FireBall.eff", L"Layer_Effect_Sire"))))
		return E_FAIL;

	// 3순위 Effect 2
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Sire_Meteo_Strike.eff", L"Layer_Effect_Sire"))))
		return E_FAIL;

	// 3순위 Effect 3
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Sire_TargetSquare.eff", L"Layer_Effect_Sire"))))
		return E_FAIL;

	// 3순위 Effect 4
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Sire_Meteo_Explosion.eff", L"Layer_Effect_Sire"))))
		return E_FAIL;

	// 3순위 Effect 5
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Sire_MagicSquare.eff", L"Layer_Effect_Sire"))))
		return E_FAIL;

	// 3순위 Effect 6
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Sire_FireShock.eff", L"Layer_Effect_Sire"))))
		return E_FAIL;

	// 3순위 Effect 7
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Sire_Heal.eff", L"Layer_Effect_Sire"))))
		return E_FAIL;

	// Skeleton
	// Effect 0
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Skeleton_Rock_Spell.eff", L"Layer_Effect_Skeleton"))))
		return E_FAIL;

	// Effect 1
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Skeleton_Crack.eff", L"Layer_Effect_Skeleton"))))
		return E_FAIL;
	
	// Quatra
	// Effect 0
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Quatra_Breath.eff", L"Layer_Effect_Quatra"))))
		return E_FAIL;

	// Flogas
	// Effect 0
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Flogas_Earthquake.eff", L"Layer_Effect_Flogas"))))
		return E_FAIL;

	// Effect 1
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Flogas_FireSpurt.eff", L"Layer_Effect_Flogas"))))
		return E_FAIL;

	// Effect 2
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Flogas_Magic_Square.eff", L"Layer_Effect_Flogas"))))
		return E_FAIL;

	// Effect 3
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Flogas_Summon_Zone.eff", L"Layer_Effect_Flogas"))))
		return E_FAIL;

	//Ghost_Mage Effect
	// Effect 0
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Ghost_Mage_MagicSqure.eff", L"Layer_Effect_Ghost_Mage"))))
		return E_FAIL;
	// Effect 1
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Ghost_Mage_Ice_Particle.eff", L"Layer_Effect_Ghost_Mage"))))
		return E_FAIL;

	//Object Effect
	// Effect 0
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/stage1_Gate.eff", L"Layer_Effect_Gate"))))
		return E_FAIL;

	// Effect 0
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/Direction_Arrow.eff", L"Layer_Effect_Direction_Arrow"))))
		return E_FAIL;

	// Effect 0
	if ((FAILED(Load_Effect_FromFile(L"../Bin/Resources/Data/ExclamationMark.eff", L"Layer_Effect_ExclamationMark"))))
		return E_FAIL;



	return S_OK;
}

HRESULT CLoading::Ready_Stage_Prototype_Component()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (pManagement == nullptr)
	{
		_TAGMSG_BOX(L"pManagement is nullptr, In Ready Ready_Stage_Prototype_Component", L"CLoading");
		return E_FAIL;
	}
	pManagement->AddRef();
	{
		//////////////////////////////////////////////////////////////////////////
		//	For. Shader
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Shader_Terrain",
			CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_Terrain.fx"))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Shader_Mesh",
			CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_Mesh.fx"))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Shader_SkyBox",
			CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_SkyBox.fx"))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Shader_Damage",
			CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_Damage.fx"))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Shader_Effect",
			CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_Effect.fx"))))
			return E_FAIL;
		//	UI
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Shader_UI",
			CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_UI.fx"))))
			return E_FAIL;
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Shader_Monster_UI",
			CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_Monster_UI.fx"))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Shader_Trail",
			CShader::Create(m_pDevice, L"../Bin/ShaderFiles/Shader_Trail.fx"))))
			return E_FAIL;

		//////////////////////////////////////////////////////////////////////////
		// For. Weapon
		//Sword
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Mesh_Sword",
			CMesh_Static::Create(m_pDevice, L"../Bin/Resources/Mesh/StaticMesh/Item/Sword/", L"Sword.x"))))
			return E_FAIL;

		//Shield
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Mesh_Shield",
			CMesh_Static::Create(m_pDevice, L"../Bin/Resources/Mesh/StaticMesh/Item/Shield/", L"Shield.x"))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Mesh_Staff",
			CMesh_Static::Create(m_pDevice, L"../Bin/Resources/Mesh/StaticMesh/Item/Staff/", L"Staff.x"))))
			return E_FAIL;

		//////////////////////////////////////////////////////////////////////////
		// For. Effect Mesh
		// IceBolt
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Mesh_IceBolt",
			CMesh_Static::Create(m_pDevice, L"../Bin/Resources/Effect/EffectMesh/", L"IceBolt.x"))))
			return E_FAIL;


		//////////////////////////////////////////////////////////////////////////
		//	For. Resources
		//	Dynamic Mesh
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Mesh_Lups",
			CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resources/Mesh/DynamicMesh/Lups/", L"Lups.x"))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Mesh_Sire",
			CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resources/Mesh/DynamicMesh/Sire/", L"Sire.x"))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Mesh_Ghost_Soldier",
			CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resources/Mesh/DynamicMesh/Ghost_Soldier/", L"Ghost_Soldier.x"))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Mesh_Ghost_Mage",
			CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resources/Mesh/DynamicMesh/Ghost_Mage/", L"Ghost_Mage.x"))))
			return E_FAIL;

		//Boss
		/*if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Mesh_Balrog",
			CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resources/Mesh/DynamicMesh/Balrog/", L"Balrog.x"))))
			return E_FAIL;*/

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Mesh_Skeleton",
			CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resources/Mesh/DynamicMesh/Skeleton/", L"Skeleton.x"))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Mesh_Quatra",
			CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resources/Mesh/DynamicMesh/Quatra/", L"Quatra.x"))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Mesh_Flogas",
			CMesh_Dynamic::Create(m_pDevice, L"../Bin/Resources/Mesh/DynamicMesh/Flogas/", L"Flogas.x"))))
			return E_FAIL;

		//Buffer Porototype Component Create;
		//Stage1
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Buffer_Terrain", CBuffer_TerrainTex::Create(m_pDevice, STAGE1_VTXCNTX, STAGE1_VTXCNTZ, STAGE1_VTXITV,40.f))))
			return E_FAIL;
		//Stage2
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Buffer_Terrain_Stage2", CBuffer_TerrainTex::Create(m_pDevice, STAGE2_VTXCNTX, STAGE2_VTXCNTZ, STAGE2_VTXITV,40.f))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Buffer_CubeTex", CBuffer_CubeTex::Create(m_pDevice))))
			return E_FAIL;

		//Texture Porototype Component Create;
		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Texture_Terrain",
			CTexture::Create(m_pDevice, L"../Bin/Resources/Texture/Terrain/Tile_D_%d.tga", CTexture::TEXTURETYPE::TEX_NORMAL, 8))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Texture_Terrain_Normal",
			CTexture::Create(m_pDevice, L"../Bin/Resources/Texture/Terrain/Tile_N_%d.tga", CTexture::TEXTURETYPE::TEX_NORMAL, 5))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Texture_Terrain_Filter",
			CTexture::Create(m_pDevice, L"../Bin/Resources/Texture/Terrain_Filter/Filter_%d.bmp", CTexture::TEXTURETYPE::TEX_NORMAL, 2))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Texture_SkyBox",
			CTexture::Create(m_pDevice, L"../Bin/Resources/Texture/SkyBox/SkyBox_%d.dds", CTexture::TEXTURETYPE::TEX_CUBE, 1))))
			return E_FAIL;

		if (FAILED(pManagement->Add_Prototype_Component(SCENE_STAGE, L"Component_Texture_Trail",
			CTexture::Create(m_pDevice, L"../Bin/Resources/Texture/Trail.tga", CTexture::TEXTURETYPE::TEX_NORMAL, 1))))
			return E_FAIL;


		//UI Load and AddPrototype Texture COcmponent
		CUI_Resources_Loader UI_Loader(m_pDevice);
		UI_Loader.UI_Resources_Load(L"../Bin/Resources/Texture/UI/*.*");

		if (FAILED(UI_Loader.Effect_Mesh_Load(L"../Bin/Resources/Effect/EffectMesh/*.*")))
		{
			return E_FAIL;
		}

		if (FAILED(UI_Loader.Effect_Texture_Load(L"../Bin/Resources/Effect/SingleTexture/*.*")))
		{
			return E_FAIL;
		}
		UI_Loader.UI_Resources_Load(L"../Bin/Resources/Effect/Texture/*.*");

	}
	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CLoading::Ready_Stage_Prototype_GameObject()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (pManagement == nullptr)
	{
		_TAGMSG_BOX(L"pManagement is nullptr, In Ready Ready_Stage_Prototype_GameObject", L"CLoading");
		return E_FAIL;
	}
	pManagement->AddRef();
	{


	//////////////////////////////////////////////////////////////////////////
	//	For. Envirnoment

	// GameObject_Terrain
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Terrain", CTerrain::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Terrain_Stage2", CTerrain_Stage2::Create(m_pDevice))))
		return E_FAIL;


	// GameObject_SkyBox
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_SkyBox", CSkyBox::Create(m_pDevice))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////
	// For. Camera
	// Camera_Dynamic
	if (FAILED(pManagement->Add_Prototype_Camera(L"Camera_Dynamic", CCamera_Dynamic::Create(m_pDevice))))
	return E_FAIL;

	// Camera_CutScene 
	if (FAILED(pManagement->Add_Prototype_Camera(L"Camera_CutScene", CCamera_CutScene::Create(m_pDevice))))
	return E_FAIL;

	// Camera_Static
	if (FAILED(pManagement->Add_Prototype_Camera(L"Camera_Static", CCamera_Static::Create(m_pDevice))))
	return E_FAIL;


	//////////////////////////////////////////////////////////////////////////
	// For. Player_Character
	// GameObject_Lups
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Lups", CLups::Create(m_pDevice))))
	return E_FAIL;

	// GameObject_Sire
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Sire", CSire::Create(m_pDevice))))
	return E_FAIL;

	//////////////////////////////////////////////////////////////////////////
	// For. Weapon
	// GameObject_Sword
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Sword", CSword::Create(m_pDevice))))
	return E_FAIL;

	// GameObject_Shield
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Shield", CShield::Create(m_pDevice))))
	return E_FAIL;

	// GameObject_Staff
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Staff", CStaff::Create(m_pDevice))))
	return E_FAIL;

	//////////////////////////////////////////////////////////////////////////
	// For. Monster
	// GameObject_Ghost_Soldier
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Ghost_Soldier", CGhost_Soldier::Create(m_pDevice))))
	return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Ghost_Mage", CGhost_Mage::Create(m_pDevice))))
	return E_FAIL;

	///*if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Balrog", CBalrog::Create(m_pDevice))))
	//	return E_FAIL;*/

	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Skeleton", CSkeleton::Create(m_pDevice))))
	return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Quatra", CQuatra::Create(m_pDevice))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Flogas", CFlogas::Create(m_pDevice))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////
	// For. UI_Frame
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_UI_Frame", CUI_Frame::Create(m_pDevice))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////
	// For. Effect
	// Ice Bolt
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_IceBolt", CIceBolt::Create(m_pDevice))))
		return E_FAIL;

	// Ice Bolt
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Meteo_Strike", CMeteo_Strike::Create(m_pDevice))))
		return E_FAIL;

	// For. Damage
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Damage", CDamage::Create(m_pDevice))))
		return E_FAIL;

	// For. Mosnter HP Bar
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Monster_HpBar", CMonster_HpBar::Create(m_pDevice))))
		return E_FAIL;

	// For. Trail;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Trail", CTrail::Create(m_pDevice))))
		return E_FAIL;

	// For. Light_Fire;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Light_Fire", CLight_Fire::Create(m_pDevice))))
		return E_FAIL;

	// For. Blood;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Blood", CBlood::Create(m_pDevice))))
		return E_FAIL;

	// For. HitFire;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_HitFire", CHitFire::Create(m_pDevice))))
		return E_FAIL;

	// For. Direction_Arrow;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Direction_Arrow", CDirection_Arrow::Create(m_pDevice))))
		return E_FAIL;

	// For. ExclamationMark;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_ExclamationMark", CExclamationMark::Create(m_pDevice))))
		return E_FAIL;

	// For. Effect_Quatra_Breath;
	if (FAILED(pManagement->Add_Prototype_GameObject(L"GameObject_Quatra_Breath", CEffect_QuatraBreath::Create(m_pDevice))))
		return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CLoading::Ready_Stage_GameObject_Layer()
{
	// 1순위 지형	
	// For.Layer_BackGround
	lstrcpy(m_szLoadingString, L"Terrain Loading");
	if (FAILED(Ready_Layer_Terrain(L"Layer_Terrain")))
		return E_FAIL;

	// 2순위 UI
	if ((FAILED(Load_UI_FromFile(L"../Bin/Resources/Data/Lups.ui", L"Layer_UI_Lups"))))
		return E_FAIL;
	// 2순위 UI
	if ((FAILED(Load_UI_FromFile(L"../Bin/Resources/Data/Sire.ui", L"Layer_UI_Sire"))))
		return E_FAIL;


	// Boss
	if ((FAILED(Load_UI_FromFile(L"../Bin/Resources/Data/Skeleton.ui", L"Layer_UI_Skeleton"))))
		return E_FAIL;
	if ((FAILED(Load_UI_FromFile(L"../Bin/Resources/Data/Flogas.ui", L"Layer_UI_Flogas"))))
		return E_FAIL;

	//LoadEffect File
	if (FAILED(Loading_EffectFile()))
		return E_FAIL;


	// For.Layer_Player
	lstrcpy(m_szLoadingString, L"Player Character Loading");
	if (FAILED(Ready_Layer_Character()))
		return E_FAIL;

	// For.Layer_SkyBox
	lstrcpy(m_szLoadingString, L"SkyBox Loading");
	if (FAILED(Ready_Layer_SkyBox(L"Layer_SkyBox")))
		return E_FAIL;


	// For.Layer_Waeapon
	lstrcpy(m_szLoadingString, L"Weapon Loading");
	if (FAILED(Ready_Layer_Weapon()))
		return E_FAIL;

	// For.Etc
	lstrcpy(m_szLoadingString, L"Etc Loading");
	if (FAILED(Ready_Layer_Etc()))
		return E_FAIL;

	//Test
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();
	{

	/*	CGhost_Soldier*	pGhost_Soldier;
		if (FAILED(pManagement->Add_Layer(SCENE_STAGE, L"Layer_Ghost_Soldier", L"GameObject_Ghost_Soldier", (CGameObject**)&pGhost_Soldier)))
			return E_FAIL;
	
		_matrix mat;
		D3DXMatrixIdentity(&mat);
		mat._11 = 0.15f;
		mat._22 = 0.15f;
		mat._33 = 0.15f;
		mat._41 = 20.f;
		mat._42 = 0.f;
		mat._43 = 10.f;
		pGhost_Soldier->SetUp_GameObjectInfo(&mat);*/

		/*
		CGhost_Mage*	pGhost_Mage;
		if (FAILED(pManagement->Add_Layer(SCENE_STAGE, L"Layer_Ghost_Mage", L"GameObject_Ghost_Mage", (CGameObject**)&pGhost_Mage)))
			return E_FAIL;

		D3DXMatrixIdentity(&mat);
		mat._11 = 0.15f;
		mat._22 = 0.15f;
		mat._33 = 0.15f;
		mat._41 = 20.f;
		mat._42 = 0.f;
		mat._43 = 20.f;
		pGhost_Mage->SetUp_GameObjectInfo(&mat);*/

		/*CQuatra*	pQuatra;
		if (FAILED(pManagement->Add_Layer(SCENE_STAGE, L"Layer_Quatra", L"GameObject_Quatra", (CGameObject**)&pQuatra)))
			return E_FAIL;

		_matrix mat;
		D3DXMatrixIdentity(&mat);
		mat._11 = 0.15f;
		mat._22 = 0.15f;
		mat._33 = 0.15f;

		mat._41 = 20.f;
		mat._42 = 50.f;
		mat._43 = 30.f;
		pQuatra->SetUp_GameObjectInfo(&mat);*/

		//CFlogas*	pFlogas;
		//if (FAILED(pManagement->Add_Layer(SCENE_STAGE, L"Layer_Flogas", L"GameObject_Flogas", (CGameObject**)&pFlogas)))
		//	return E_FAIL;

		//_matrix mat;
		//D3DXMatrixIdentity(&mat);
		//mat._11 = 0.15f;
		//mat._22 = 0.15f;
		//mat._33 = 0.15f;

		//mat._41 = 20.f;
		//mat._42 = 0.f;
		//mat._43 = 30.f;
		//pFlogas->SetUp_GameObjectInfo(&mat);

	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CLoading::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	// For.Terrain
	if (FAILED(pManagement->Add_Layer(SCENE_STAGE, pLayerTag, L"GameObject_Terrain")))
		return E_FAIL;



	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CLoading::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();

	// For.SkyBox
	if (FAILED(pManagement->Add_Layer(SCENE_STAGE, pLayerTag, L"GameObject_SkyBox")))
		return E_FAIL;


	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CLoading::Ready_Layer_Character()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();
	{


	CLups*	pLups;

	// For. Lups
	if (FAILED(pManagement->Add_Layer(SCENE_STAGE, L"Layer_Lups", L"GameObject_Lups", (CGameObject**)&pLups)))
		return E_FAIL;


	CSire*	pSire;
	// For. Sire
	if (FAILED(pManagement->Add_Layer(SCENE_STAGE, L"Layer_Sire", L"GameObject_Sire", (CGameObject**)&pSire)))
		return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}


HRESULT CLoading::Ready_Layer_Weapon()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();
	{


	// For. Sword
	CSword*	pSword;
		
	if (FAILED(pManagement->Add_Layer(SCENE_STAGE, L"Layer_Sword", L"GameObject_Sword", (CGameObject**)&pSword)))
		return E_FAIL;

	pSword->SetUp_SworldInfo();
	

	//For. Shield
	CShield*	pShield;

	if (FAILED(pManagement->Add_Layer(SCENE_STAGE, L"Layer_Shield", L"GameObject_Shield", (CGameObject**)&pShield)))
		return E_FAIL;

	pShield->SetUp_ShieldInfo();


	//For. Staff
	CStaff*	pStaff;

	if (FAILED(pManagement->Add_Layer(SCENE_STAGE, L"Layer_Staff", L"GameObject_Staff", (CGameObject**)&pStaff)))
		return E_FAIL;

	pStaff->SetUp_StaffInfo();

	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CLoading::Ready_Layer_Etc()
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;
	pManagement->AddRef();
	{
		// For. Direction_Arrow
		//CDirection_Arrow*	pDirection_Arrow;

		if (FAILED(pManagement->Add_Layer(SCENE_STAGE, L"Layer_Etc", L"GameObject_Direction_Arrow")))
			return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}



HRESULT CLoading::Create_Prototype_Component_Mesh(OBJECTTYPE eType, const _tchar * pFilePath, const _tchar * pFileName, const _tchar * pComponentMeshTag)
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

HRESULT CLoading::Create_Prototype_GameObject(OBJECTTYPE eType, const _tchar * pGameObjectTag, CGameObject** ppGameObject)
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

	switch (eType)
	{
	//case MESH_DYNAMIC:
	//	if (FAILED(pManagement->Add_Prototype_GameObject(pGameObjectTag, CGameObject_Dynamic::Create(pDevice))))
	//	{
	//		Safe_Release(pDevice);
	//		Safe_Release(pManagement);
	//		return E_FAIL;
	//	}
	//	break;
	case MESH_STATIC:
		if (FAILED(pManagement->Add_Prototype_GameObject(pGameObjectTag, CGameObject_Static::Create(pDevice))))
		{
			Safe_Release(pDevice);
			Safe_Release(pManagement);
			return E_FAIL;
		}
		break;

	case OBJ_EFFECT:
		pManagement->Add_Prototype_GameObject(pGameObjectTag,CEffectGroup::Create(pDevice),ppGameObject);
	}
	Safe_Release(pDevice);
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CLoading::Add_Layer_GameObject(const _tchar* pLayerTag, const _tchar * pGameObjectTag, CGameObject ** ppGameObject)
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

HRESULT CLoading::Load_GameObject_Static_FromFile(const _tchar* pFilePath)
{
	HANDLE		hFile = CreateFile(pFilePath
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
		Create_Prototype_GameObject(MESH_STATIC, wstrGameObjectTag.c_str());
			
		if (FAILED(Add_Layer_GameObject(L"Layer_Static", wstrGameObjectTag.c_str(), (CGameObject**)&pGameObject_Static)))
		{
			CloseHandle(hFile);
			return E_FAIL;
		}

		//Late SetUp Staic Object
		pGameObject_Static->SetUp_GameObjectInfo(&matWorld, wstrComponentMeshTag.c_str());
		//	pGameObject_Static->SetUp_FileInfo(wstrFilePath.c_str(), wstrFileName.c_str(), wstrGameObjectTag.c_str(), wstrComponentMeshTag.c_str());

	}
	CloseHandle(hFile);


	return S_OK;
}

HRESULT CLoading::Load_GameObject_Dynamic_FromFile(const _tchar * pFilePath)
{
	HANDLE		hFile = CreateFile(pFilePath
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

HRESULT CLoading::Load_Camera_CutScene_FromFile(const _tchar* pFilePath)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	pManagement->AddRef();

	//Read File
	HANDLE		hFile = CreateFile(pFilePath
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD	dwByte = 0;
	_ulong	dwIndex = 0;

	while (true)
	{
		vector<_vec3> vecCamera_Eye;
		vector<_vec3> vecCamera_At;

		_int	iTagLength;
		_tchar	szSceneTag[MIN_PATH];

		ReadFile(hFile, &iTagLength, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, szSceneTag, sizeof(_tchar)*iTagLength, &dwByte, nullptr);


		//Read Speed;
		_float	fSpeed;
		ReadFile(hFile, &fSpeed, sizeof(_float), &dwByte, nullptr);

		//Read Szie
		_int iSize;
		ReadFile(hFile, &iSize, sizeof(_int), &dwByte, nullptr);

		//Read CameraEye
		_vec3 vPosition;
		for (_int i = 0; i < iSize; ++i)
		{
			ReadFile(hFile, &vPosition, sizeof(_vec3), &dwByte, nullptr);
			vecCamera_Eye.push_back(vPosition);
			ReadFile(hFile, &vPosition, sizeof(_vec3), &dwByte, nullptr);
			vecCamera_At.push_back(vPosition);
		}
		if (dwByte == 0)
			break;

		//Input Data;
		// For. CutScene Camera
		CCamera_CutScene* pCamera_CutScene;

		pManagement->Add_Camera(CAMERA_CUTSCENE, L"Camera_CutScene", szSceneTag, (CCamera**)&pCamera_CutScene);

		//Key
		pCamera_CutScene->Set_KeyTag(szSceneTag);
		pCamera_CutScene->Set_Speed(fSpeed);


		for (_int i = 0; i < iSize; ++i)
		{
			pCamera_CutScene->Add_Eye(vecCamera_Eye[i]);
			pCamera_CutScene->Add_At(vecCamera_At[i]);
		}

		pCamera_CutScene->Caculate_Line();

		vecCamera_Eye.clear();
		vecCamera_At.clear();

	}

	Safe_Release(pManagement);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CLoading::Load_UI_FromFile(const _tchar * pFilePath, const _tchar* pLayerTag)
{

	HANDLE		hFile = CreateFile(pFilePath
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD	dwByte = 0;
	_ulong	dwIndex = 0;

	_tchar	szFileName[MAX_PATH] = L"";
	lstrcpy(szFileName, pFilePath);
	PathFindFileName(szFileName);


	//Frame Create
	//Add Layer
	CManagement* pManagement = CManagement::GetInstance();
	pManagement->AddRef();

	//Size
	_int	iListSize;
	ReadFile(hFile, &iListSize, sizeof(_int), &dwByte, nullptr);

	for (_int i = 0; i < iListSize; ++i)
	{
		CUI_Frame*	pUIFrame;

		pManagement->Add_Layer(SCENE_STAGE, pLayerTag, L"GameObject_UI_Frame", (CGameObject**)&pUIFrame);

		pUIFrame->Load_UIFrame_Info(hFile, dwByte);
		pUIFrame->Set_Activate(false);
	}


	Safe_Release(pManagement);


	CloseHandle(hFile);
	
	return S_OK;
}

HRESULT CLoading::Load_Effect_FromFile(const _tchar * pFilePath, const _tchar* pLayerTag)
{
	HANDLE		hFile = CreateFile(pFilePath
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD	dwByte = 0;
	_ulong	dwIndex = 0;


	CEffectGroup::EFFECTGROUP_DATA EffectGroup_Data;

	//테그
	ReadFile(hFile, &EffectGroup_Data.szTag, sizeof(CEffectGroup::EFFECTGROUP_DATA), &dwByte, nullptr);

	CEffectGroup*	pProto_EffectGroup;

	//EffectGroup Create
	Create_Prototype_GameObject(OBJ_EFFECT, EffectGroup_Data.szTag, (CGameObject**)&pProto_EffectGroup);
	pProto_EffectGroup->Load_EffectDataFromFile(hFile, dwByte);
	pProto_EffectGroup->Set_LifeTime(EffectGroup_Data.fLifeTime);


	if (FAILED(Add_Layer_GameObject(pLayerTag, EffectGroup_Data.szTag)))
		return E_FAIL;

	//pProto_EffectGroup->Set_Activate(false);

	CloseHandle(hFile);

	return S_OK;
}




size_t	CALLBACK CLoading::Thread_Main(void *pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(pLoading->Get_CritialSection());

	switch (pLoading->GetLoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;
	case LOADING_BOSS:
		break;
	}
	LeaveCriticalSection(pLoading->Get_CritialSection());
	_endthreadex(0);

	return iFlag;

}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pDevice, LOADINGID eLoadingID)
{
	CLoading*	pInstance = new CLoading(pDevice);

	if (FAILED(pInstance->Ready_Loading(eLoadingID)))
	{	
		_MSG_BOX(L"CLoading Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_CrticalSection);

	Safe_Release(m_pDevice);
}
