#include "stdafx.h"
#include "Quatra_Breath_End.h"

#include "Quatra.h"

#include "Management.h"
#include "EffectGroup.h"

#include "Sound_Manager.h"

_USING(Client)

CQuatra_Breath_End::CQuatra_Breath_End()
{
}

HRESULT CQuatra_Breath_End::Begin()
{
	m_pQuatra->Set_AnimationSet(4);

	return S_OK;
}

_int CQuatra_Breath_End::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CQuatra_Breath_End::LastUpdate(const _float & fTimeDelta)
{
	if (m_pQuatra->Get_IsAnimationSetEnd())
	{
		m_pQuatra->Change_State(L"Wait");
		m_pQuatra->Get_Transform()->Set_StateInfo(CTransform::STATE_POSITION, 0.f, 0.f, 0.f);
	}
	

	return _int();
}

HRESULT CQuatra_Breath_End::End()
{
	return S_OK;
}

HRESULT CQuatra_Breath_End::Ready_State(CQuatra * pQuatra)
{
	m_pQuatra  = pQuatra;



	return S_OK;;
}

CQuatra_Breath_End * CQuatra_Breath_End::Create(CQuatra* pQuatra)
{
	CQuatra_Breath_End*		pInstance = new CQuatra_Breath_End();

	if (FAILED(pInstance->Ready_State(pQuatra)))
	{
		_MSG_BOX(L"CQuatra_Breath_End Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuatra_Breath_End::Free()
{
}


