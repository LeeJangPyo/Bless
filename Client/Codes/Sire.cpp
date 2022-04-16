#include "stdafx.h"
#include "..\Headers\Sire.h"

#include "Management.h"
#include "Input_Device.h"
#include "Collision_Manager.h"

#include "GameEvent_Manager.h"
#include "Player_Manager.h"


//
#include "UI_Frame.h"

//State
#include "Sire_General.h"
#include "Sire_Ready_Combat.h"
#include "Sire_Idle.h"
#include "Sire_Ready_FireBall.h"
#include "Sire_Cast_FireBall.h"
#include "Sire_Ready_Meteo.h"
#include "Sire_Meteo_Strike.h"
#include "Sire_Meteo_Target.h"
#include "Sire_AI.h"
#include "Sire_FireShock.h"
#include "Sire_Hit.h"
#include "Sire_DownAndUp.h"
#include "Sire_Heal_Target.h"
#include "Sire_Heal.h"
#include "Sire_End_Combat.h"

#include "Damage.h"

_USING(Client)

CSire::CSire(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject_Player(pDevice)
	, m_bIsRButtonDown(false)
{
}

CSire::CSire(const CSire & rhs)
	:CGameObject_Player(rhs)
	,m_bIsRButtonDown(false)
{
}

HRESULT Client::CSire::Ready_Prototype()
{

	if (FAILED(CGameObject_Player::Ready_Prototype()))
		return E_FAIL;



	return S_OK;
}

HRESULT Client::CSire::Ready_GameObject()
{

	if (FAILED(CGameObject_Player::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;
	
	if (FAILED(CGameObject_Dynamic::Load_ColliderInfoFromFile(L"../Bin/Resources/Data/Sire.col")))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	Ready_StatsInfo();

	m_pTransformCom->Set_ScaleInfo(0.15f, 0.15f, 0.15f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, 30.f, 0.f, 5.f);

	//Compute NaviMesh Index;
	if (FAILED(m_pNaviMeshCom->Compute_Position_OnNaviMeshIndex(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), m_dwCurrentNaviIndex)))
	{
		return E_FAIL;
	}

	//Get UI Pointer;
	m_pUI_HP = dynamic_cast<CUI_Frame*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_UI_Sire", 0));
	if (m_pUI_HP == nullptr)
		return E_FAIL;
	m_pUI_MP = dynamic_cast<CUI_Frame*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_UI_Sire", 1));
	if (m_pUI_MP == nullptr)
		return E_FAIL;
	m_pUI_Lups_HP = dynamic_cast<CUI_Frame*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_UI_Sire", 5));
	if (m_pUI_Lups_HP == nullptr)
		return E_FAIL;

	//Sire UI Frame  Stoped
	Set_UI_Activate(PLAYER_SIRE, false);


	//Setup Collision Component 
	CCollision_Manager::GetInstance()->SetUp_PlayerList(this);

	//Setup PlayerManager;
	CPlayer_Manager::GetInstance()->Set_Sire(this);

	m_pMeshCom->Set_AnimationSet(42);
	return S_OK;
}

Engine::_int Client::CSire::Update_GameObject(const _float& fTimeDelta)
{


	//if (m_pInput_Device->Get_KeyPressing(DIK_5))
	//{
	//	Change_State(L"Ready_Combat");
	//}
	//if (m_pInput_Device->Get_KeyPressing(DIK_6))
	//{
	//	Change_State(L"End_Combat");
	//}
	//if (m_pInput_Device->Get_KeyPressing(DIK_7))
	//{
	//	Change_State(L"DownAndUp");
	//}

	//Hit Delay
	m_fHitTimeAcc += fTimeDelta;
	
	Check_KeyInput(fTimeDelta);

	m_pCurrentState->Update(fTimeDelta);


	return CGameObject_Player::Update_GameObject(fTimeDelta);
}

Engine::_int Client::CSire::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pCurrentState->LastUpdate(fTimeDelta);

	//Update Character Hp
	m_pUI_Lups_HP->Set_Radio(CPlayer_Manager::GetInstance()->Get_Hp_Radio(PLAYERTYPE::PLAYER_LUPS), L"Mask");
	m_pUI_MP->Set_Radio(Get_Mp_Radio(), L"Mask");

	return CGameObject_Player::LastUpdate_GameObject(fTimeDelta);
}

void Client::CSire::Render_GameObject()
{
	CGameObject_Player::Render_GameObject();
}

HRESULT CSire::SetUp_GameObjectInfo(const _matrix * pMatrix)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//Set World Matrix
		m_pTransformCom->Set_WorldMatrix(pMatrix);

		/*if (FAILED(CGameObject_Dynamic::Load_ColliderInfoFromFile(L"../Bin/Resources/Data/Ghost_Soldier.col")))
			return E_FAIL;*/
	}
	Safe_Release(pManagement);
	return S_OK;
}

void CSire::Damaged(const _int& iDamage, const _uint& iEffect_Type, const _vec3* vEffect_Position)
{
	if (m_fHitTimeAcc < m_fHitDelay)
		return;

	if (Compare_State(L"Dead") || Compare_State(L"DownAndUp"))
		return;

	m_fHitTimeAcc = 0.f;

	if (iEffect_Type != EFFECT_HEAL)
	{
		if (iDamage > 1000)
		{
			Change_State(L"DownAndUp");

			CDamage* pDamage;
			CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Damage", L"GameObject_Damage", (CGameObject**)&pDamage);
			pDamage->SetUp_DamageInfo(iDamage, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), CDamage::RED);

			m_BaseStats.iCurrentHp -= iDamage;
			m_pUI_HP->Set_Radio(Get_Hp_Radio(), L"Mask");
		}
		else
		{
			if (Compare_State(L"Ready_Meteo") || Compare_State(L"Ready_FireBall"))
			{

				CDamage* pDamage;
				CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Damage", L"GameObject_Damage", (CGameObject**)&pDamage);
				pDamage->SetUp_DamageInfo(iDamage, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), CDamage::RED);

				m_BaseStats.iCurrentHp -= iDamage;
				m_pUI_HP->Set_Radio(Get_Hp_Radio(), L"Mask");
			}
			else
			{
				CDamage* pDamage;
				CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Damage", L"GameObject_Damage", (CGameObject**)&pDamage);
				pDamage->SetUp_DamageInfo(iDamage, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), CDamage::RED);

				m_BaseStats.iCurrentHp -= iDamage;
				m_pUI_HP->Set_Radio(Get_Hp_Radio(), L"Mask");

				Change_State(L"Hit");
			}
		}
	}
	else	//Heal
	{
		CDamage* pDamage;
		CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Damage", L"GameObject_Damage", (CGameObject**)&pDamage);
		pDamage->SetUp_DamageInfo(iDamage, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), CDamage::GREEN);

		m_BaseStats.iCurrentHp += iDamage;
		m_pUI_HP->Set_Radio(Get_Hp_Radio(), L"Mask");
	}
}


void CSire::Check_KeyInput_Move(const _float & fTimeDelta)
{
	if (m_eControl != CONTROL_PLAYER)
		return;

	_float fMouseMove = 0.f;

	if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Z))
	{
		m_fCamera_At_Acc += fMouseMove * 0.005f;
	}


	if (m_pInput_Device->Get_MouseDown(CInput_Device::DIM_RB))
	{
		m_bIsRButtonDown = true;
	}
	if (m_pInput_Device->Get_MousePressing(CInput_Device::DIM_RB))
	{
		if (m_bIsRButtonDown)
		{
			fMouseMove = 0.f;
			if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_X))
			{
				m_pTransformCom->Rotation_Y(D3DXToRadian(fMouseMove*7.5f), fTimeDelta);
			}
		}
	}
	if (m_pInput_Device->Get_MouseUp(CInput_Device::DIM_RB))
	{
		m_bIsRButtonDown = false;
	}


	if(m_pInput_Device->Get_KeyPressing(DIK_W) && m_pInput_Device->Get_KeyPressing(DIK_D))
	{
		const _vec3* vPosition;
		_vec3 vDirection;

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, 
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION,
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));


		m_pMeshCom->Set_AnimationSet(40);

		return;
	}

	if (m_pInput_Device->Get_KeyPressing(DIK_W) && m_pInput_Device->Get_KeyPressing(DIK_A))
	{
		const _vec3* vPosition;
		_vec3 vDirection;

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION,
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION,
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(-vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));


		m_pMeshCom->Set_AnimationSet(41);

		return;
	}

	if (m_pInput_Device->Get_KeyPressing(DIK_S) && m_pInput_Device->Get_KeyPressing(DIK_D))
	{
		const _vec3* vPosition;
		_vec3 vDirection;

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION,
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(-vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION,
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));


		m_pMeshCom->Set_AnimationSet(43);

		return;
	}

	if (m_pInput_Device->Get_KeyPressing(DIK_S) && m_pInput_Device->Get_KeyPressing(DIK_A))
	{
		const _vec3* vPosition;
		_vec3 vDirection;

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION,
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(-vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION,
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(-vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));


		m_pMeshCom->Set_AnimationSet(44);

		return;
	}

	if (m_pInput_Device->Get_KeyPressing(DIK_W))
	{
		const _vec3* vPosition;
		_vec3 vDirection;

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, 
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));

		m_pMeshCom->Set_AnimationSet(42);

		return;
	}
	if (m_pInput_Device->Get_KeyPressing(DIK_S))
	{
		const _vec3* vPosition;
		_vec3 vDirection;

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION,
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(-vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));

		m_pMeshCom->Set_AnimationSet(45);

		return;
	}

	if(m_pInput_Device->Get_KeyPressing(DIK_D))
	{
		const _vec3* vPosition;
		_vec3 vDirection;

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION,
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));

		m_pMeshCom->Set_AnimationSet(38);

		return;
	}

	if (m_pInput_Device->Get_KeyPressing(DIK_A))
	{
		const _vec3* vPosition;
		_vec3 vDirection;

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION,
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(-vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));

		m_pMeshCom->Set_AnimationSet(39);

		return;
	}


	if (m_pMeshCom->Is_AnimationSetEnd() == true)
	{
		m_pMeshCom->Set_AnimationSet(50);
	}
}

void CSire::Check_KeyInput(const _float & fTimeDelta)
{
	//Change Player
	if (m_pInput_Device->Get_KeyUp(DIK_F1))
	{
		CPlayer_Manager::GetInstance()->Set_Character(PLAYER_LUPS);

		m_eControl = CONTROL_AI;
		if (CPlayer_Manager::GetInstance()->Get_CombatState() == GENERAL)
		{
			Change_State(L"AI");
		}
		//else
		//{
		//	Change_State(L"Idle");
		//}
	}
}



HRESULT CSire::Ready_Component()
{

	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//For. Mesh
		m_pMeshCom = dynamic_cast<CMesh_Dynamic*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Mesh_Sire"));
		if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
			return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CSire::Ready_State()
{
	CState*	pState = CSire_General::Create(this);
	m_mapState.emplace(L"General", pState);

	pState = CSire_Ready_Combat::Create(this);
	m_mapState.emplace(L"Ready_Combat", pState);

	pState = CSire_Idle::Create(this);
	m_mapState.emplace(L"Idle", pState);

	pState = CSire_Cast_FireBall::Create(this);
	m_mapState.emplace(L"Cast_FireBall", pState);

	pState = CSire_Ready_FireBall::Create(this);
	m_mapState.emplace(L"Ready_FireBall", pState);

	pState = CSire_Meteo_Strike::Create(this);
	m_mapState.emplace(L"Meteo_Strike", pState);

	pState = CSire_Ready_Meteo::Create(this);
	m_mapState.emplace(L"Ready_Meteo", pState);

	pState = CSire_Meteo_Target::Create(this);
	m_mapState.emplace(L"Meteo_Target", pState);

	pState = CSire_AI::Create(this);
	m_mapState.emplace(L"AI", pState);

	pState = CSire_FireShock::Create(this);
	m_mapState.emplace(L"FireShock", pState);

	pState = CSire_Hit::Create(this);
	m_mapState.emplace(L"Hit", pState);

	pState = CSire_DownAndUp::Create(this);
	m_mapState.emplace(L"DownAndUp", pState);

	pState = CSire_Heal::Create(this);
	m_mapState.emplace(L"Heal", pState);

	pState = CSire_Heal_Target::Create(this);
	m_mapState.emplace(L"Heal_Target", pState);

	pState = CSire_End_Combat::Create(this);
	m_mapState.emplace(L"End_Combat", pState);


	Change_State(L"AI");

	return S_OK;
}

void CSire::Ready_StatsInfo()
{
	m_BaseStats.iMaxHp = m_BaseStats.iCurrentHp = 10000;
	m_BaseStats.fMoveSpeed = 10.f;

	m_BaseStats.iMaxMp = m_BaseStats.iCurrentMp = 1000;

}


CSire * CSire::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSire*		pInstance = new CSire(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CSire Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* Client::CSire::Clone_GameObject()
{
	CSire*		pInstance = new CSire(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CSire Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CSire::Free()
{
	CGameObject_Player::Free();
}
