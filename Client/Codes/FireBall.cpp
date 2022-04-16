#include "stdafx.h"
#include "..\Headers\FireBall.h"

#include "Management.h"
#include "EffectGroup.h"

_USING(Client)

CFireBall::CFireBall(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CFireBall::CFireBall(const CFireBall & rhs)
	:CGameObject(rhs)
{
}

HRESULT CFireBall::Ready_Prototype()
{
	return S_OK;

}

HRESULT CFireBall::Ready_GameObject()
{


	////Obj
	//CEffect_Mesh*	pEffect_Mesh = 	dynamic_cast<CEffect_Mesh*>(m_pEffectGroup->Get_Effect_Mesh(0));
	//if (pEffect_Mesh == nullptr)
	//	return E_FAIL;


	m_pEffectGroup->Set_Activate(true);


	return S_OK;
}

_int CFireBall::Update_GameObject(const _float & fTimeDelta)
{

	return _int();
}

_int CFireBall::LastUpdate_GameObject(const _float & fTimeDelta)
{

	return _int();
}

void CFireBall::Render_GameObject()
{
	
}

CGameObject * CFireBall::Clone_GameObject()
{
	CFireBall*		pInstance = new CFireBall(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CFireBall Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CFireBall * CFireBall::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFireBall*		pInstance = new CFireBall(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CFireBall Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFireBall::Free()
{
}
