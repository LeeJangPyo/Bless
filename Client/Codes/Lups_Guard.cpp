#include "stdafx.h"
#include "Lups_Guard.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Sound_Manager.h"

//Object
#include "Lups.h"
#include "Sword.h"
#include "Shield.h"

//UI
#include "UI_Frame.h"

#include "EffectGroup.h"

_USING(Client)

CLups_Guard::CLups_Guard()
{

}

HRESULT CLups_Guard::Begin()
{
	m_pLups->Set_AnimationSet(9);


	_vec3 vEffectPoint = *m_pLups->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	m_pEffect_Guard->Set_Position(&vEffectPoint);

	m_pEffect_Guard->Set_Activate(true);


	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::LUPS);
	CSound_Manager::GetInstance()->Play_Sound(L"Lups_Guard.mp3", CSound_Manager::LUPS);
	return S_OK;
}

_int CLups_Guard::Update(const _float & fTimeDelta)
{


	m_pEffect_Guard->RotationY(D3DXToRadian(2.5f));

	return 0;
}

_int CLups_Guard::LastUpdate(const _float & fTimeDelta)
{
	if (!m_pLups->Use_Mp(3))
	{
		m_pLups->Change_State(L"Idle");
	}
	if (CInput_Device::GetInstance()->Get_KeyDown(DIK_3))
	{
		m_pLups->Change_State(L"Idle");
	}
	return 0;
}

HRESULT CLups_Guard::End()
{
	m_pEffect_Guard->Set_Activate(false);

	return S_OK;
}

HRESULT CLups_Guard::Ready_State(CLups* pLups)
{
	m_pLups = pLups;

	////UI Setting
	//const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_UI_Lups");
	//if (pGameObjectList == nullptr)
	//	return E_FAIL;

	//for (auto& pGameObject : *pGameObjectList)
	//{
	//	CUI_Frame* pUIFrame = dynamic_cast<CUI_Frame*>(pGameObject);
	//	if (pUIFrame == nullptr)
	//		return E_FAIL;
	//	if (pUIFrame->Compare_Tag(L"Skill_Guard"))
	//	{
	//		m_pUIFrame = pUIFrame;
	//		break;
	//	}
	//}

	//if (m_pUIFrame == nullptr)
	//	return E_FAIL;

//	m_pUIFrame->Set_CoolDown(0.f);

	m_pEffect_Guard = dynamic_cast<CEffectGroup*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Lups",3));
	if (m_pEffect_Guard == nullptr)
		return E_FAIL;

	////Collider Setup
	//m_pEffect_Guard->SetUp_ColliderInfo(5.f, COL_MONSTER);
	//m_pEffect_Guard->Set_Collision_StartTime(0.75f);
	//m_pEffect_Guard->Set_Damage(&m_Damage);
	return S_OK;;
}

CLups_Guard * CLups_Guard::Create(CLups* pLups)
{
	CLups_Guard*		pInstance = new CLups_Guard();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_Guard Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_Guard::Free()
{
}


