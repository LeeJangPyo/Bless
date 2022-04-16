#include "stdafx.h"
#include "Lups_Defense.h"

#include "Management.h"
#include "Collision_Manager.h"


//Object
#include "Lups.h"
#include "Sword.h"
#include "Shield.h"


_USING(Client)

CLups_Defense::CLups_Defense()
{

}

HRESULT CLups_Defense::Begin()
{
	m_pLups->Set_AnimationSet(40);
	


	return S_OK;
}

_int CLups_Defense::Update(const _float & fTimeDelta)
{

	CSword* pSword = dynamic_cast<CSword*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Sword"));
	if (pSword == nullptr)
		return E_FAIL;
	pSword->Set_Equip(true);

	CShield* pShield = dynamic_cast<CShield*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Shield"));
	if (pShield == nullptr)
		return E_FAIL;
	pShield->Set_Equip(true);



	m_pLups->Check_KeyInput_Move(fTimeDelta);
	return 0;
}

_int CLups_Defense::LastUpdate(const _float & fTimeDelta)
{
	////상태 분기
	//_vec3 vPos = *m_pTransform->Get_StateInfo(CTransform::STATE_POSITION);
	//_vec3 vPlayerPos = *m_pPlayerTransform->Get_StateInfo(CTransform::STATE_POSITION);
	//_vec3 vDir = vPlayerPos - vPos;
	//_float fDistance = D3DXVec3Length(&vDir);

	//if (CCollision_Manager::GetInstance()->Check_Collision(CCollision_Manager::COL_PLAYER, m_pGhost_Solider))
	//{
	//	// Attack
	//	m_pGhost_Solider->Change_State(L"Attack");

	//}
	//else
	//{
	//	// 추격
	//	m_pGhost_Solider->Change_State(L"Chase");
	//}
	return 0;
}

HRESULT CLups_Defense::End()
{
	return S_OK;
}

HRESULT CLups_Defense::Ready_State(CLups* pLups)
{
	m_pLups = pLups;


	return S_OK;;
}

CLups_Defense * CLups_Defense::Create(CLups* pLups)
{
	CLups_Defense*		pInstance = new CLups_Defense();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_Defense Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_Defense::Free()
{
}


