#include "stdafx.h"
#include "Lups_Shield_Smash.h"

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

CLups_Shield_Smash::CLups_Shield_Smash()
	:m_pLups(nullptr)
	, m_pUIFrame(nullptr)
{

}

HRESULT CLups_Shield_Smash::Begin()
{

	if (!m_pLups->Use_Mp(300))
		return S_OK;

	if (m_pUIFrame->Get_IsCoolDown())
	{
		m_pLups->Change_State(L"Idle");
		return S_OK;
	}
	m_pUIFrame->Set_IsCoolDown(true);

	m_pLups->Set_AnimationSet(17);

	_vec3 vEffectPoint = *m_pLups->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vDir = *m_pLups->Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vDir, &vDir);
	vEffectPoint += 5.f * vDir;

	m_pEffect_Shield_Smash->Set_Position(&vEffectPoint);

	m_pEffect_Shield_Smash->Set_Activate(true);

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::LUPS);
	CSound_Manager::GetInstance()->Play_Sound(L"Lups_Shield_Smash.mp3", CSound_Manager::LUPS);
	return S_OK;
}

_int CLups_Shield_Smash::Update(const _float & fTimeDelta)
{
	
	return 0;
}

_int CLups_Shield_Smash::LastUpdate(const _float & fTimeDelta)
{
	if (m_pLups->Get_IsAnimationSetEnd())
	{
		m_pLups->Change_State(L"Idle");
	}
	return 0;
}

HRESULT CLups_Shield_Smash::End()
{
	return S_OK;
}

HRESULT CLups_Shield_Smash::Ready_State(CLups* pLups)
{
	m_pLups = pLups;

	const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_UI_Lups");
	if (pGameObjectList == nullptr)
		return E_FAIL;

	for(auto& pGameObject : *pGameObjectList)
	{
		CUI_Frame* pUIFrame = dynamic_cast<CUI_Frame*>(pGameObject);
		if (pUIFrame == nullptr)
			return E_FAIL;
		if (pUIFrame->Compare_Tag(L"Skill_Shield_Smash"))
		{
			m_pUIFrame = pUIFrame;
			break;
		}
	}

	if (m_pUIFrame == nullptr)
		return E_FAIL;

	//Effect CoolDOwn
	m_pUIFrame->Set_CoolDown(7.5f);

	//Effect Damage;
	m_Damage.iDamage = 250;
	m_Damage.iDamage_Range = 250;

	//Effect Setting
	m_pEffect_Shield_Smash = dynamic_cast<CEffectGroup*>( CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Lups"));
	if (m_pEffect_Shield_Smash == nullptr)
		return E_FAIL;

	//Collider Setup
	m_pEffect_Shield_Smash->SetUp_ColliderInfo(5.f, COL_MONSTER, 1.f,2.f);
	m_pEffect_Shield_Smash->Set_Damage(&m_Damage);

	return S_OK;;
}

CLups_Shield_Smash * CLups_Shield_Smash::Create(CLups* pLups)
{
	CLups_Shield_Smash*		pInstance = new CLups_Shield_Smash();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_Shield_Smash Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_Shield_Smash::Free()
{
}


