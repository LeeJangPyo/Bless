#include "stdafx.h"
#include "..\Headers\GameObject_Player.h"

#include "Management.h"
#include "Player_Manager.h"

#include "UI_Frame.h"

_USING(Client)

CGameObject_Player::CGameObject_Player(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject_Dynamic(pDevice)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_vCamera_At_Position(0.f,0.f,0.f)
	, m_fCamera_At_Acc(1.3f)
{
	m_pInput_Device->AddRef();

	for(_int i = 0; i < PLAYERCAMERA_END; ++i)
	{
		m_vCamera_Eye_Position[i] = _vec3(0.f, 0.f, 0.f);
		m_fCameraDistance[i] = 0.f;
	}

}

CGameObject_Player::CGameObject_Player(const CGameObject_Player & rhs)
	:CGameObject_Dynamic(rhs)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_vCamera_At_Position(0.f, 0.f, 0.f)
	, m_fCamera_At_Acc(2.5f)
{
	m_pInput_Device->AddRef();

	for (_int i = 0; i < PLAYERCAMERA_END; ++i)
	{
		m_vCamera_Eye_Position[i] = _vec3(0.f, 0.f, 0.f);
		m_fCameraDistance[i] = 0.f;
	}
}

HRESULT CGameObject_Player::Ready_Prototype()
{
	if (FAILED(CGameObject_Dynamic::Ready_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CGameObject_Player::Ready_GameObject()
{
	if (FAILED(CGameObject_Dynamic::Ready_GameObject()))
		return E_FAIL;

	//Camera Distance
	m_fCameraDistance[PLAYERCAMERA_DEFAULT] = 10.f;
	m_fCameraDistance[PLAYERCAMERA_MIDDLE] = 20.f;

	return S_OK;
}

_int CGameObject_Player::Update_GameObject(const _float & fTimeDelta)
{
	Update_CameraPosition();

	m_BaseStats.iCurrentMp++;
	if(m_BaseStats.iCurrentMp > m_BaseStats.iMaxMp)
		m_BaseStats.iCurrentMp = m_BaseStats.iMaxMp;

	return CGameObject_Dynamic::Update_GameObject(fTimeDelta);
}

_int CGameObject_Player::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return CGameObject_Dynamic::LastUpdate_GameObject(fTimeDelta);
}

void CGameObject_Player::Render_GameObject()
{
	CGameObject_Dynamic::Render_GameObject();
}

_float CGameObject_Player::Get_Hp_Radio()
{
	return (_float)m_BaseStats.iCurrentHp / m_BaseStats.iMaxHp;
}

_float CGameObject_Player::Get_Mp_Radio()
{
	return (_float)m_BaseStats.iCurrentMp / m_BaseStats.iMaxMp;
}

_bool CGameObject_Player::Use_Mp(const _int & iMp)
{
	if (m_BaseStats.iCurrentMp - iMp < 0)
		return false;

	m_BaseStats.iCurrentMp -= iMp;
	return true;
}

void CGameObject_Player::Set_UI_Activate(const PLAYERTYPE& ePlayerType, _bool bActivate)
{
	const list<CGameObject*>* pGameObjectList;
	if (ePlayerType == PLAYER_LUPS)
		 pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_UI_Lups");
	else
		pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_UI_Sire");
	for (auto& pGameObject : *pGameObjectList)
	{
		CUI_Frame* pUIFrame = dynamic_cast<CUI_Frame*>(pGameObject);
		pUIFrame->Set_Activate(bActivate);
	}
}

void CGameObject_Player::Set_ControlType(CONTROLTYPE eType)
{
	m_eControl = eType;
	if(CPlayer_Manager::GetInstance()->Get_CombatState() == GENERAL)
		Change_State(L"General");
	
}

void CGameObject_Player::Update_CameraPosition()
{
	_vec3 vPlayerPosition;
	_vec3 vPlayerLook;
	_vec3 vPlayerRight;
	_matrix	matRotAxis;

	//AtPosition 
	m_vCamera_At_Position = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	m_vCamera_At_Position.y += m_fCamera_At_Acc;

	//Default Camera Update
	vPlayerPosition = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	vPlayerLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK) * -1.f;
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	vPlayerLook *= m_fCameraDistance[PLAYERCAMERA_DEFAULT];
	vPlayerRight = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);

	D3DXMatrixRotationAxis(&matRotAxis, &vPlayerRight, D3DXToRadian(45.f));

	D3DXVec3TransformNormal(&vPlayerLook, &vPlayerLook, &matRotAxis);
	m_vCamera_Eye_Position[PLAYERCAMERA_DEFAULT] = vPlayerLook + vPlayerPosition;

	//Middle Camera Update
	vPlayerPosition = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	vPlayerLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK) * -1.f;
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	vPlayerLook *= m_fCameraDistance[PLAYERCAMERA_MIDDLE];
	vPlayerRight = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);

	D3DXMatrixRotationAxis(&matRotAxis, &vPlayerRight, D3DXToRadian(60.f));

	D3DXVec3TransformNormal(&vPlayerLook, &vPlayerLook, &matRotAxis);
	m_vCamera_Eye_Position[PLAYERCAMERA_MIDDLE] = vPlayerLook + vPlayerPosition;



}


void CGameObject_Player::Free()
{
	Safe_Release(m_pInput_Device);

	CGameObject_Dynamic::Free();
}
