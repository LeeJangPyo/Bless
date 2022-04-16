#include "stdafx.h"
#include "..\Headers\Direction_Arrow.h"

#include "Management.h"

#include "EffectGroup.h"
#include "Player_Manager.h"

_USING(Client)

CDirection_Arrow::CDirection_Arrow(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fFrame(0.f)
	, m_fTimeAcc(0.f)
{
}

CDirection_Arrow::CDirection_Arrow(const CDirection_Arrow & rhs)
	: CGameObject(rhs)
	, m_fFrame(0.f)
	, m_fTimeAcc(0.f)
{

}

HRESULT CDirection_Arrow::Ready_Prototype()
{
	return S_OK;
}

HRESULT CDirection_Arrow::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pEffect_Direction_Arrow = dynamic_cast<CEffectGroup*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Direction_Arrow"));
	if (m_pEffect_Direction_Arrow == nullptr)
		return E_FAIL;

	m_pEffect_Direction_Arrow->Set_Activate(true);

	/*m_pStartPos = CPlayer_Manager::GetInstance()->Get_Player_Position(PLAYERTYPE::PLAYER_LUPS);
	m_pEndPos =CPlayer_Manager::GetInstance()->Get_Player_Position(PLAYERTYPE::PLAYER_SIRE);

	m_vDir = *m_pEndPos - *m_pStartPos;*/

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, CPlayer_Manager::GetInstance()->Get_Player_Position(PLAYERTYPE::PLAYER_LUPS));


	return S_OK;
}

_int CDirection_Arrow::Update_GameObject(const _float & fTimeDelta)
{
	
	return _int();
}

_int CDirection_Arrow::LastUpdate_GameObject(const _float & fTimeDelta)
{
	CPlayer_Manager*	pPlayer_Manager = CPlayer_Manager::GetInstance();
	pPlayer_Manager->AddRef();
	{
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPlayer_Manager->Get_Player_Position());

		if (pPlayer_Manager->Get_PlayerType() == PLAYER_LUPS)
			m_pTransformCom->LookAt_Target(pPlayer_Manager->Get_Player_Position(PLAYERTYPE::PLAYER_SIRE), fTimeDelta);
		else
			m_pTransformCom->LookAt_Target(pPlayer_Manager->Get_Player_Position(PLAYERTYPE::PLAYER_LUPS), fTimeDelta);
	}
	Safe_Release(pPlayer_Manager);


	_float fRadian = m_pTransformCom->Get_RotationInfo(CTransform::STATE_UP);

	m_pEffect_Direction_Arrow->Set_Position(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));
	m_pEffect_Direction_Arrow->Set_RotationY(fRadian);

	return _int();
}

void CDirection_Arrow::Render_GameObject()
{
	
}

void CDirection_Arrow::Set_Position(const _vec3 * pPosition)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);
}



HRESULT CDirection_Arrow::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
		if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
			return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}


CGameObject * CDirection_Arrow::Clone_GameObject()
{
	CDirection_Arrow*		pInstance = new CDirection_Arrow(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CDirection_Arrow Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CDirection_Arrow * CDirection_Arrow::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDirection_Arrow*		pInstance = new CDirection_Arrow(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CDirection_Arrow Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDirection_Arrow::Free()
{
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
