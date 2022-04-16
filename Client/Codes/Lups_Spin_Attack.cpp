#include "stdafx.h"
#include "Lups_Spin_Attack.h"

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

CLups_Spin_Attack::CLups_Spin_Attack()
{
	ZeroMemory(&m_Damage, sizeof(DAMAGE));
}

HRESULT CLups_Spin_Attack::Begin()
{
	if (!m_pLups->Use_Mp(200))
		return S_OK;

	if (m_pUIFrame->Get_IsCoolDown())
	{
		m_pLups->Change_State(L"Idle");
		return S_OK;
	}
	m_pUIFrame->Set_IsCoolDown(true);

	m_pLups->Set_AnimationSet(14);


	_vec3 vEffectPoint = *m_pLups->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	m_pEffect_Spin_Attack->Set_Position(&vEffectPoint);

	m_pEffect_Spin_Attack->Set_Activate(true);

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::LUPS);
	CSound_Manager::GetInstance()->Play_Sound(L"Lups_SpinAttack.mp3", CSound_Manager::LUPS);
	return S_OK;
}

_int CLups_Spin_Attack::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CLups_Spin_Attack::LastUpdate(const _float & fTimeDelta)
{
	if (m_pLups->Get_IsAnimationSetEnd())
	{
		m_pLups->Change_State(L"Idle");
	}
	return 0;
}

HRESULT CLups_Spin_Attack::End()
{
	return S_OK;
}

HRESULT CLups_Spin_Attack::Ready_State(CLups* pLups)
{
	m_pLups = pLups;

	m_Damage.iDamage = 1000;
	m_Damage.iDamage_Range = 500;


	//UI
	const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_UI_Lups");
	if (pGameObjectList == nullptr)
		return E_FAIL;

	for (auto& pGameObject : *pGameObjectList)
	{
		CUI_Frame* pUIFrame = dynamic_cast<CUI_Frame*>(pGameObject);
		if (pUIFrame == nullptr)
			return E_FAIL;
		if (pUIFrame->Compare_Tag(L"Skill_Spin_Attack"))
		{
			m_pUIFrame = pUIFrame;
			break;
		}
	}
	if (m_pUIFrame == nullptr)
		return E_FAIL;
	m_pUIFrame->Set_CoolDown(5.f);

	//Effect	
	m_pEffect_Spin_Attack = dynamic_cast<CEffectGroup*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Lups",1));
	if (m_pEffect_Spin_Attack == nullptr)
		return E_FAIL;

	//Collider Setup
	m_pEffect_Spin_Attack->SetUp_ColliderInfo(5.f, COL_MONSTER, 0.75f,1.f);
	m_pEffect_Spin_Attack->Set_Damage(&m_Damage);


	return S_OK;;
}

CLups_Spin_Attack * CLups_Spin_Attack::Create(CLups* pLups)
{
	CLups_Spin_Attack*		pInstance = new CLups_Spin_Attack();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_Spin_Attack Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_Spin_Attack::Free()
{
}


