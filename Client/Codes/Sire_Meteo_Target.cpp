#include "stdafx.h"
#include "Sire_Meteo_Target.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Sound_Manager.h"
#include "GameEvent_Manager.h"

//Object
#include "Sire.h"

#include "Player_Manager.h"
#include "EffectGroup.h"

#include "UI_Frame.h"


_USING(Client)

CSire_Meteo_Target::CSire_Meteo_Target()
	:m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

HRESULT CSire_Meteo_Target::Begin()
{
	if (m_pUIFrame->Get_IsCoolDown())
	{
		m_pSire->Change_State(L"Idle");
		return S_OK;
	}


	m_pSire->Set_AnimationSet(22);
	

	//ViewPoint Change
	CPlayer_Manager::GetInstance()->Set_Camera(PLAYERCAMERA_MIDDLE);

	m_pEffect_TargetZone->Set_Activate(true);

	return S_OK;
}

_int CSire_Meteo_Target::Update(const _float & fTimeDelta)
{
	
	m_pSire->Check_KeyInput_Move(fTimeDelta);

	return 0;
}

_int CSire_Meteo_Target::LastUpdate(const _float & fTimeDelta)
{
	Check_KeyInput(fTimeDelta);

	return 0;
}

HRESULT CSire_Meteo_Target::End()
{
	m_pEffect_TargetZone->Set_Activate(false);
	return S_OK;
}

void CSire_Meteo_Target::Check_KeyInput(const _float & fTimeDelta)
{
	if (m_pSire->Get_ControlType() != CONTROL_PLAYER)
		return;

	_vec3 vPickingPosition = PickUp_OnTerrain();
	vPickingPosition.y += 0.5f;
	m_pEffect_TargetZone->Set_Position(&vPickingPosition);
	
	if (m_pInput_Device->Get_MouseUp(CInput_Device::DIM_LB))
	{
		m_pUIFrame->Set_IsCoolDown(true);

		m_pSire->Set_Spell_Target(&vPickingPosition);

		CPlayer_Manager::GetInstance()->Set_Camera(PLAYERCAMERA_DEFAULT);

		m_pSire->Change_State(L"Ready_Meteo");

		
	}

	if (m_pInput_Device->Get_MouseUp(CInput_Device::DIM_RB))
	{
		CPlayer_Manager::GetInstance()->Set_Camera(PLAYERCAMERA_DEFAULT);
		m_pSire->Change_State(L"Idle");
	}

}

HRESULT CSire_Meteo_Target::Ready_State(CSire* pSire)
{
	m_pSire = pSire;

	//Get Effect Pointer
	m_pEffect_TargetZone = dynamic_cast<CEffectGroup*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Sire", 3));
	if (m_pEffect_TargetZone == nullptr)
		return E_FAIL;

	//Ready Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//For. Calculator
		m_pCalculator = dynamic_cast<CCalculator*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Calculator"));
		if(m_pCalculator == nullptr)
			return E_FAIL;

	}
	Safe_Release(pManagement);


	//UI
	const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_UI_Sire");
	if (pGameObjectList == nullptr)
		return E_FAIL;

	for (auto& pGameObject : *pGameObjectList)
	{
		CUI_Frame* pUIFrame = dynamic_cast<CUI_Frame*>(pGameObject);
		if (pUIFrame == nullptr)
			return E_FAIL;
		if (pUIFrame->Compare_Tag(L"Skill_MeteoStrike"))
		{
			m_pUIFrame = pUIFrame;
			break;
		}
	}
	if (m_pUIFrame == nullptr)
		return E_FAIL;
	m_pUIFrame->Set_CoolDown(17.5f);

	return S_OK;;
}

CSire_Meteo_Target * CSire_Meteo_Target::Create(CSire* pSire)
{
	CSire_Meteo_Target*		pInstance = new CSire_Meteo_Target();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_Meteo_Target Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_Meteo_Target::Free()
{
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pCalculator);
}

_vec3 CSire_Meteo_Target::PickUp_OnTerrain(void)
{
	CBuffer_TerrainTex*	pTerrainBufferCom = nullptr;
	CTransform*		pTerrainTransCom = nullptr;


	if (CGameEvent_Manager::GetInstance()->Get_Stage() == STAGE1)
	{
		pTerrainBufferCom = dynamic_cast<CBuffer_TerrainTex*>
			(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", L"Com_Buffer"));

		pTerrainTransCom = dynamic_cast<Engine::CTransform*>
			(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", L"Com_Transform"));
	}
	else
	{
		pTerrainBufferCom = dynamic_cast<CBuffer_TerrainTex*>
			(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain_Stage2", L"Com_Buffer"));

		pTerrainTransCom = dynamic_cast<Engine::CTransform*>
			(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain_Stage2", L"Com_Transform"));
	}
	


	return m_pCalculator->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}