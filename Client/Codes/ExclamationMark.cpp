#include "stdafx.h"
#include "..\Headers\ExclamationMark.h"

#include "Management.h"

#include "EffectGroup.h"
#include "Player_Manager.h"

_USING(Client)

CExclamationMark::CExclamationMark(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fimeAcc(0.f)
{
}

CExclamationMark::CExclamationMark(const CExclamationMark & rhs)
	: CGameObject(rhs)
	, m_fimeAcc(0.f)
{

}

HRESULT CExclamationMark::Ready_Prototype()
{
	return S_OK;
}

HRESULT CExclamationMark::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	CManagement::GetInstance()->Add_Layer
	(SCENE_STAGE, L"Layer_Effect_ExclamationMark",L"GameObject_Effect_ExclamationMark", (CGameObject**)&m_pEffect_ExclamationMark);
	if (m_pEffect_ExclamationMark == nullptr)
		return E_FAIL;

	m_pEffect_ExclamationMark->Set_DeadObject(true);
	m_pEffect_ExclamationMark->Set_Activate(true);

	return S_OK;
}

_int CExclamationMark::Update_GameObject(const _float & fTimeDelta)
{
	m_fimeAcc += fTimeDelta;
	if (m_fimeAcc > 1.f)
	{
		return DEAD_OBJ;
	}

	return _int();
}

_int CExclamationMark::LastUpdate_GameObject(const _float & fTimeDelta)
{

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, m_pMonsterTransform->Get_StateInfo(CTransform::STATE_POSITION));
	m_pTransformCom->LookAt_Target(CManagement::GetInstance()->Get_CameraMain()->Get_Camera_Position(),1.f);

	_float fRadian = m_pTransformCom->Get_RotationInfo(CTransform::STATE_UP);

	m_pEffect_ExclamationMark->Set_Position(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));
	m_pEffect_ExclamationMark->Set_RotationY(fRadian);

	return _int();
}

void CExclamationMark::Render_GameObject()
{

}

void CExclamationMark::Set_Position(const _vec3 * pPosition)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);
}

void CExclamationMark::Set_Transform(CTransform * pTransform)
{
	m_pMonsterTransform = pTransform;
}



HRESULT CExclamationMark::Ready_Component()
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


CGameObject * CExclamationMark::Clone_GameObject()
{
	CExclamationMark*		pInstance = new CExclamationMark(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CExclamationMark Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CExclamationMark * CExclamationMark::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CExclamationMark*		pInstance = new CExclamationMark(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CExclamationMark Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExclamationMark::Free()
{
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
