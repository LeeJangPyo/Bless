#include "stdafx.h"
#include "Lups_Taunt.h"

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

CLups_Taunt::CLups_Taunt()
{

}

HRESULT CLups_Taunt::Begin()
{
	if (!m_pLups->Use_Mp(100))
		return S_OK;

	if (m_pUIFrame->Get_IsCoolDown())
	{
		m_pLups->Change_State(L"Idle");
		return S_OK;
	}
	m_pUIFrame->Set_IsCoolDown(true);


	m_pLups->Set_AnimationSet(23);


	_vec3 vEffectPoint = *m_pLups->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	m_pEffect_Taunt->Set_Position(&vEffectPoint);
	m_pEffect_Taunt->Set_Activate(true);

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::LUPS);
	CSound_Manager::GetInstance()->Play_Sound(L"Lups_Taunt.wav", CSound_Manager::LUPS);
	return S_OK;
}

_int CLups_Taunt::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CLups_Taunt::LastUpdate(const _float & fTimeDelta)
{
	if (m_pLups->Get_IsAnimationSetEnd())
	{
		m_pLups->Change_State(L"Idle");
	}
	return 0;
}

HRESULT CLups_Taunt::End()
{
	m_pEffect_Taunt->Set_Activate(false);
	return S_OK;
}

HRESULT CLups_Taunt::Ready_State(CLups* pLups)
{
	m_pLups = pLups;

	//Damage
	m_Damage.iDamage = 1;
	m_Damage.iDamage_Range = 1;

	//UI Setting
	const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_UI_Lups");
	if (pGameObjectList == nullptr)
		return E_FAIL;

	for (auto& pGameObject : *pGameObjectList)
	{
		CUI_Frame* pUIFrame = dynamic_cast<CUI_Frame*>(pGameObject);
		if (pUIFrame == nullptr)
			return E_FAIL;
		if (pUIFrame->Compare_Tag(L"Skill_Taunt"))
		{
			m_pUIFrame = pUIFrame;
			break;
		}
	}

	if (m_pUIFrame == nullptr)
		return E_FAIL;

	m_pUIFrame->Set_CoolDown(2.5f);

	//Get Effect
	m_pEffect_Taunt = dynamic_cast<CEffectGroup*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Lups",2));
	if (m_pEffect_Taunt == nullptr)
		return E_FAIL;

	m_pEffect_Taunt->SetUp_ColliderInfo(5.f, COL_MONSTER, 0.5f,0.6f);
	m_pEffect_Taunt->Set_Damage(&m_Damage);
	m_pEffect_Taunt->Set_EffectType(EFFECT_EXCLAMATIONMARK);


	return S_OK;;
}

CLups_Taunt * CLups_Taunt::Create(CLups* pLups)
{
	CLups_Taunt*		pInstance = new CLups_Taunt();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_Taunt Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_Taunt::Free()
{
}


