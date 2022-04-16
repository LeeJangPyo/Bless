#include "stdafx.h"
#include "..\Headers\Lups.h"

#include "Management.h"
#include "Input_Device.h"
#include "Collision_Manager.h"

#include "GameEvent_Manager.h"
#include "Player_Manager.h"

//State
#include "Lups_General.h"
#include "Lups_Ready_Combat.h"
#include "Lups_Idle.h"
#include "Lups_Attack.h"
#include "Lups_Shield_Smash.h"
#include "Lups_Spin_Attack.h"
#include "Lups_Hit.h"
#include "Lups_AI.h"
#include "Lups_Guard.h"
#include "Lups_Taunt.h"
#include "Lups_End_Combat.h"
#include "Lups_DownAndUp.h"

#include "Damage.h"

#include "Camera_Static.h"

//UI
#include "UI_Frame.h"

_USING(Client)

CLups::CLups(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject_Player(pDevice)
	, m_bIsRButtonDown(false)
{
}

CLups::CLups(const CLups & rhs)
	:CGameObject_Player(rhs)
	,m_bIsRButtonDown(false)
{
}

HRESULT Client::CLups::Ready_Prototype()
{

	if (FAILED(CGameObject_Player::Ready_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Client::CLups::Ready_GameObject()
{

	if (FAILED(CGameObject_Player::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;
	
	if (FAILED(CGameObject_Dynamic::Load_ColliderInfoFromFile(L"../Bin/Resources/Data/Lups.col")))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	Ready_StatsInfo();

	m_pTransformCom->Set_ScaleInfo(0.125f, 0.125f, 0.125f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, 25.f, 0.f, 10.f);


	//Compute NaviMesh Index;
	if (FAILED(m_pNaviMeshCom->Compute_Position_OnNaviMeshIndex(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), m_dwCurrentNaviIndex)))
	{
		return E_FAIL;
	}

	//Get UI Pointer;
	m_pUI_HP = dynamic_cast<CUI_Frame*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_UI_Lups",0));
	if (m_pUI_HP == nullptr)
		return E_FAIL;
	m_pUI_MP = dynamic_cast<CUI_Frame*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_UI_Lups",1));
	if (m_pUI_MP == nullptr)
		return E_FAIL;
	m_pUI_Sire_HP = dynamic_cast<CUI_Frame*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_UI_Lups",5));
	if (m_pUI_Sire_HP == nullptr)
		return E_FAIL;

	//Sire UI Frame  Stoped
	Set_UI_Activate(PLAYER_LUPS, true);

	//Setup Collision Component 
	CCollision_Manager::GetInstance()->SetUp_PlayerList(this);

	//Setup PlayerManager;
	CPlayer_Manager::GetInstance()->Set_Lups(this);

	//Player Control Type
	m_eControl = CONTROL_PLAYER;

	return S_OK;
}

Engine::_int Client::CLups::Update_GameObject(const _float& fTimeDelta)
{
	/*if (m_pInput_Device->Get_KeyPressing(DIK_5))
	{
		dynamic_cast<CCamera_Static*>(CManagement::GetInstance()->Get_CameraMain())->Set_CameraShaking(0.3f);

		Change_State(L"Ready_Combat");
	}
	if (m_pInput_Device->Get_KeyPressing(DIK_6))
	{
		Change_State(L"End_Combat");
	}
	if (m_pInput_Device->Get_KeyPressing(DIK_7))
	{
		Change_State(L"DownAndUp");
	}*/

	//Hit Delay
	m_fHitTimeAcc += fTimeDelta;


	Check_KeyInput(fTimeDelta);


	m_pCurrentState->Update(fTimeDelta);

	return CGameObject_Player::Update_GameObject(fTimeDelta);
}

Engine::_int Client::CLups::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pCurrentState->LastUpdate(fTimeDelta);

	//Update Character Hp
	m_pUI_Sire_HP->Set_Radio(CPlayer_Manager::GetInstance()->Get_Hp_Radio(PLAYERTYPE::PLAYER_SIRE),L"Mask");
	m_pUI_MP->Set_Radio(Get_Mp_Radio(), L"Mask");

	return CGameObject_Player::LastUpdate_GameObject(fTimeDelta);
}

void Client::CLups::Render_GameObject()
{
	CGameObject_Player::Render_GameObject();
}

HRESULT CLups::SetUp_GameObjectInfo(const _matrix * pMatrix)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//Set World Matrix
		m_pTransformCom->Set_WorldMatrix(pMatrix);

		if (FAILED(CGameObject_Dynamic::Load_ColliderInfoFromFile(L"../Bin/Resources/Data/Ghost_Soldier.col")))
			return E_FAIL;

		Safe_Release(pManagement);
	}
	return S_OK;
}

void CLups::Damaged(const _int& iDamage, const _uint& iEffect_Type, const _vec3* vEffect_Position)
{
	if (m_fHitTimeAcc < m_fHitDelay)
		return;

	if (Compare_State(L"Guard") || Compare_State(L"Dead") || Compare_State(L"DownAndUp"))
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
			if (Compare_State(L"Attack") || Compare_State(L"Spin_Attack") || Compare_State(L"Shield_Smash"))
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
	else			//Heal
	{
		CDamage* pDamage;
		CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Damage", L"GameObject_Damage", (CGameObject**)&pDamage);
		pDamage->SetUp_DamageInfo(iDamage, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), CDamage::GREEN);

		m_BaseStats.iCurrentHp += iDamage;
		m_pUI_HP->Set_Radio(Get_Hp_Radio(), L"Mask");
	}

}


void CLups::Check_KeyInput_Move(const _float & fTimeDelta)
{
	if (m_eControl != CONTROL_PLAYER)
		return;

	_float fMouseMove = 0.f;

	if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Z))
	{
		m_fCamera_At_Acc += fMouseMove * 0.01f;
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


	if (m_pInput_Device->Get_KeyPressing(DIK_W) && m_pInput_Device->Get_KeyPressing(DIK_D))
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


		m_pMeshCom->Set_AnimationSet(30);

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


		m_pMeshCom->Set_AnimationSet(29);

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


		m_pMeshCom->Set_AnimationSet(32);

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


		m_pMeshCom->Set_AnimationSet(33);

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

		m_pMeshCom->Set_AnimationSet(31);

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

		m_pMeshCom->Set_AnimationSet(34);

		return;
	}

	if (m_pInput_Device->Get_KeyPressing(DIK_D))
	{
		const _vec3* vPosition;
		_vec3 vDirection;

		vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION,
			&m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(vDirection * m_BaseStats.fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex));

		m_pMeshCom->Set_AnimationSet(28);

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

		m_pMeshCom->Set_AnimationSet(29);

		return;
	}	

	if (m_pMeshCom->Is_AnimationSetEnd() == true)
	{
		m_pMeshCom->Set_AnimationSet(40);
	}
}

void CLups::Check_KeyInput(const _float & fTimeDelta)
{
	if (m_eControl != CONTROL_PLAYER)
		return;

	//Change Player
	if (m_pInput_Device->Get_KeyUp(DIK_F2))
	{
		CPlayer_Manager::GetInstance()->Set_Character(PLAYER_SIRE);
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

HRESULT CLups::Ready_Component()
{

	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;
	
	pManagement->AddRef();
	{
		//For. Mesh
		m_pMeshCom = dynamic_cast<CMesh_Dynamic*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Mesh_Lups"));
		if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
			return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CLups::Ready_State()
{
	CState*	pState = CLups_General::Create(this);
	m_mapState.emplace(L"General", pState);

	pState = CLups_Ready_Combat::Create(this);
	m_mapState.emplace(L"Ready_Combat", pState);

	pState = CLups_Idle::Create(this);
	m_mapState.emplace(L"Idle", pState);

	pState = CLups_Attack::Create(this);
	m_mapState.emplace(L"Attack", pState);

	pState = CLups_Shield_Smash::Create(this);
	m_mapState.emplace(L"Shield_Smash", pState);

	pState = CLups_Spin_Attack::Create(this);
	m_mapState.emplace(L"Spin_Attack", pState);

	pState = CLups_Hit::Create(this);
	m_mapState.emplace(L"Hit", pState);

	pState = CLups_AI::Create(this);
	m_mapState.emplace(L"AI", pState);

	pState = CLups_Guard::Create(this);
	m_mapState.emplace(L"Guard", pState);

	pState = CLups_Taunt::Create(this);
	m_mapState.emplace(L"Taunt", pState);

	pState = CLups_End_Combat::Create(this);
	m_mapState.emplace(L"End_Combat", pState);

	pState = CLups_DownAndUp::Create(this);
	m_mapState.emplace(L"DownAndUp", pState);


	Change_State(L"General");

	return S_OK;
}

void CLups::Ready_StatsInfo()
{
	m_BaseStats.iMaxHp = m_BaseStats.iCurrentHp = 10000;
	m_BaseStats.fMoveSpeed = 10.f;

	m_BaseStats.iMaxMp = m_BaseStats.iCurrentMp = 1000;

	m_Damage.iDamage = 200;
	m_Damage.iDamage_Range = 200;
}



CLups * CLups::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLups*		pInstance = new CLups(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CLups Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* Client::CLups::Clone_GameObject()
{
	CLups*		pInstance = new CLups(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CLups Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CLups::Free()
{
	CGameObject_Player::Free();
}
