#include "stdafx.h"
//#include "..\Headers\Masq.h"
//
//#include "Management.h"
//#include "Input_Device.h"
//#include "Collision_Manager.h"
//
//#include "GameEvent_Manager.h"
//
//
////State
////#include "Masq_General.h"
////#include "Masq_Ready_Combat.h"
////#include "Masq_Idle.h"
////#include "Masq_Attack.h"
////#include "Masq_Defense.h"
////#include "Masq_Shield_Smash.h"
////#include "Masq_Spin_Attack.h"
//
//_USING(Client)
//
//CMasq::CMasq(LPDIRECT3DDEVICE9 pDevice)
//	:CGameObject_Dynamic(pDevice)
//	,m_pInput_Device(CInput_Device::GetInstance())
//	, m_bIsRButtonDown(false)
//{
//	m_pInput_Device->AddRef();
//
//}
//
//CMasq::CMasq(const CMasq & rhs)
//	:CGameObject_Dynamic(rhs)
//	,m_pInput_Device(rhs.m_pInput_Device)
//	,m_bIsRButtonDown(false)
//{
//	m_pInput_Device->AddRef();
//}
//
//HRESULT Client::CMasq::Ready_Prototype()
//{
//
//	if (FAILED(CGameObject_Dynamic::Ready_Prototype()))
//		return E_FAIL;
//
//
//
//	return S_OK;
//}
//
//HRESULT Client::CMasq::Ready_GameObject()
//{
//
//	if (FAILED(CGameObject_Dynamic::Ready_GameObject()))
//		return E_FAIL;
//
//	if (FAILED(Ready_Component()))
//		return E_FAIL;
//	
//	if (FAILED(CGameObject_Dynamic::Load_ColliderInfoFromFile(L"../Bin/Resources/Data/Masq.col")))
//		return E_FAIL;
//
//	if (FAILED(Ready_State()))
//		return E_FAIL;
//
//	m_pTransformCom->Set_ScaleInfo(0.1f, 0.1f, 0.1f);
//	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, 30.f, 0.f, 10.f);
//
//	//Compute NaviMesh Index;
//	if (FAILED(m_pNaviMeshCom->Compute_Position_OnNaviMeshIndex(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), m_dwCurrentNaviIndex)))
//	{
//		return E_FAIL;
//	}
//
//	m_fMoveSpeed = 10.f;
//	m_iComboCount = 4;
//
//	//Setup Collision Component 
//	CCollision_Manager::GetInstance()->SetUp_PlayerList(this);
//
//	return S_OK;
//}
//
//Engine::_int Client::CMasq::Update_GameObject(const _float& fTimeDelta)
//{
//
//	//TestCode
//	if (m_pInput_Device->Get_KeyPressing(DIK_3))
//	{
//		Change_State(L"Ready_Combat");
//	}
//	if (m_pInput_Device->Get_KeyPressing(DIK_4))
//	{
//		Change_State(L"Idle");
//	}
//
//	m_pCurrentState->Update(fTimeDelta);
//
//	return CGameObject_Dynamic::Update_GameObject(fTimeDelta);
//}
//
//Engine::_int Client::CMasq::LastUpdate_GameObject(const _float& fTimeDelta)
//{
//	m_pCurrentState->LastUpdate(fTimeDelta);
//	
//	return CGameObject_Dynamic::LastUpdate_GameObject(fTimeDelta);
//}
//
//void Client::CMasq::Render_GameObject()
//{
//	CGameObject_Dynamic::Render_GameObject();
//}
//
//HRESULT CMasq::SetUp_GameObjectInfo(const _matrix * pMatrix)
//{
//	CManagement*	pManagement = CManagement::GetInstance();
//	if (pManagement == nullptr)
//		return E_FAIL;
//
//	pManagement->AddRef();
//	{
//		//Set World Matrix
//		m_pTransformCom->Set_WorldMatrix(pMatrix);
//
//		if (FAILED(CGameObject_Dynamic::Load_ColliderInfoFromFile(L"../Bin/Resources/Data/Ghost_Soldier.col")))
//			return E_FAIL;
//
//		Safe_Release(pManagement);
//	}
//	return S_OK;
//}
//
//void CMasq::Damaged()
//{
//	m_pMeshCom->Set_AnimationSet(2);
//}
//
//void CMasq::Check_KeyInput_Move(const _float & fTimeDelta)
//{
//	if (m_pInput_Device->Get_KeyPressing(DIK_W))
//	{
//		const _vec3* vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
//		_vec3		 vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
//		D3DXVec3Normalize(&vDirection, &vDirection);
//
//
//		_vec3 vPos = m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(vDirection * m_fMoveSpeed * fTimeDelta),m_dwCurrentNaviIndex);
//		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPos);
//		m_pMeshCom->Set_AnimationSet(31);
//	}
//	if (m_pInput_Device->Get_KeyPressing(DIK_S))
//	{
//		const _vec3* vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
//		_vec3		 vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
//		D3DXVec3Normalize(&vDirection, &vDirection);
//
//
//		_vec3 vPos = m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(-vDirection * m_fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex);
//		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPos);
//		m_pMeshCom->Set_AnimationSet(34);
//	}
//
//	if(m_pInput_Device->Get_KeyPressing(DIK_D))
//	{
//		const _vec3* vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
//		_vec3		 vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
//		D3DXVec3Normalize(&vDirection, &vDirection);
//
//
//		_vec3 vPos = m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(vDirection * m_fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex);
//		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPos);
//		m_pMeshCom->Set_AnimationSet(28);
//		//m_pTransformCom->Rotation_Y(D3DXToRadian(90.f), fTimeDelta);
//	}
//
//	if (m_pInput_Device->Get_KeyPressing(DIK_A))
//	{
//		const _vec3* vPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
//		_vec3		 vDirection = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
//		D3DXVec3Normalize(&vDirection, &vDirection);
//
//
//		_vec3 vPos = m_pNaviMeshCom->Move_OnNaviMesh(vPosition, &(-vDirection * m_fMoveSpeed * fTimeDelta), m_dwCurrentNaviIndex);
//		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPos);
//		m_pMeshCom->Set_AnimationSet(29);
//		//m_pTransformCom->Rotation_Y(D3DXToRadian(-90.f), fTimeDelta);
//	}
//
//	if (m_pInput_Device->Get_MouseDown(CInput_Device::DIM_RB))
//	{
//		m_bIsRButtonDown = true;
//	}
//	if (m_pInput_Device->Get_MousePressing(CInput_Device::DIM_RB))
//	{
//		if(m_bIsRButtonDown)
//		{
//			_float fMouseMove = 0.f;
//			if(fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_X))
//			{
//				m_pTransformCom->Rotation_Y(D3DXToRadian(fMouseMove*5.f), fTimeDelta);
//			}
//		}
//	}
//	if (m_pInput_Device->Get_MouseUp(CInput_Device::DIM_RB))
//	{
//		m_bIsRButtonDown = false;
//	}
//
//	if (m_pMeshCom->Is_AnimationSetEnd() == true)
//	{
//		m_pMeshCom->Set_AnimationSet(40);
//	}
//
//
//}
//
//void CMasq::Check_KeyInput_Combat(const _float & fTimeDelta)
//{
//
//
//
//
//
//	if (m_pMeshCom->Is_AnimationSetEnd() == true)
//	{
//		m_pMeshCom->Set_AnimationSet(40);
//	}
//}
//
//HRESULT CMasq::Ready_Component()
//{
//
//	// Set Component
//	CManagement*	pManagement = CManagement::GetInstance();
//	if (pManagement == nullptr)
//		return E_FAIL;
//
//	pManagement->AddRef();
//	{
//		//For. Mesh
//		m_pMeshCom = dynamic_cast<CMesh_Dynamic*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Mesh_Masq"));
//		if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
//			return E_FAIL;
//
//	}
//	Safe_Release(pManagement);
//
//	return S_OK;
//}
//
//HRESULT CMasq::Ready_State()
//{
//	CState*	pState = CMasq_General::Create(this);
//	m_mapState.emplace(L"General", pState);
//
//	pState = CMasq_Ready_Combat::Create(this);
//	m_mapState.emplace(L"Ready_Combat", pState);
//
//	pState = CMasq_Idle::Create(this);
//	m_mapState.emplace(L"Idle", pState);
//
//	pState = CMasq_Attack::Create(this);
//	m_mapState.emplace(L"Attack", pState);
//
//	pState = CMasq_Defense::Create(this);
//	m_mapState.emplace(L"Defense", pState);
//
//	pState = CMasq_Shield_Smash::Create(this);
//	m_mapState.emplace(L"Shield_Smash", pState);
//
//	pState = CMasq_Spin_Attack::Create(this);
//	m_mapState.emplace(L"Spin_Attack", pState);
//
//	Change_State(L"General");
//
//	return S_OK;
//}
//
//
//CMasq * CMasq::Create(LPDIRECT3DDEVICE9 pDevice)
//{
//	CMasq*		pInstance = new CMasq(pDevice);
//
//	if (FAILED(pInstance->Ready_Prototype()))
//	{
//		_MSG_BOX(L"CMasq Created Failed");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//Engine::CGameObject* Client::CMasq::Clone_GameObject()
//{
//	CMasq*		pInstance = new CMasq(*this);
//
//	if (FAILED(pInstance->Ready_GameObject()))
//	{
//		_MSG_BOX(L"CMasq Clone Failed");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//void Client::CMasq::Free()
//{
//	Safe_Release(m_pInput_Device);
//
//	CGameObject_Dynamic::Free();
//}
