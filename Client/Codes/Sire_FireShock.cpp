#include "stdafx.h"
#include "Sire_FireShock.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Sound_Manager.h"

#include "Sire.h"
#include "Staff.h"

//UI
#include "UI_Frame.h"

//Effect
#include "EffectGroup.h"

_USING(Client)

CSire_FireShock::CSire_FireShock()
{
	ZeroMemory(&m_Damage, sizeof(DAMAGE));
}

HRESULT CSire_FireShock::Begin()
{
	if (!m_pSire->Use_Mp(100))
		return S_OK;
	if (m_pUIFrame->Get_IsCoolDown())
	{
		m_pSire->Change_State(L"Idle");
		return S_OK;
	}
	m_pUIFrame->Set_IsCoolDown(true);

	m_bPlayOnce = false;

	m_pSire->Set_AnimationSet(20);

	
	return S_OK;
}

_int CSire_FireShock::Update(const _float & fTimeDelta)
{

	if (m_pSire->Get_IsAnimationPassRadio(0.35f) && m_bPlayOnce == false)
	{
		m_bPlayOnce = true;

		CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Effect_Sire", L"GameObject_Effect_Fire_Shock", (CGameObject**)&m_pEffect_FireShock);
		if (m_pEffect_FireShock == nullptr)
			return E_FAIL;
		//Collider Setup
		m_pEffect_FireShock->SetUp_ColliderInfo(3.f, COL_MONSTER, 0.f, 1.f);
		m_pEffect_FireShock->Set_Damage(&m_Damage);

		m_pEffect_FireShock->Set_Position(m_pSire->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION));

		_float fRadian = m_pSire->Get_Transform()->Get_RotationInfo(CTransform::STATE_UP);
		m_pEffect_FireShock->RotationY(fRadian);

		m_pEffect_FireShock->Set_EffectType(EFFECT_FIRE);
		m_pEffect_FireShock->Set_DeadObject(true);

		m_pEffect_FireShock->Set_Activate(true);


		CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::SIRE);
		CSound_Manager::GetInstance()->Play_Sound(L"Sire_FireShock.wav", CSound_Manager::SIRE);
	}

	return 0;
}

_int CSire_FireShock::LastUpdate(const _float & fTimeDelta)
{

	if (m_pSire->Get_IsAnimationSetEnd())
	{
		m_pSire->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CSire_FireShock::End()
{

	return S_OK;
}

HRESULT CSire_FireShock::Ready_State(CSire* pSire)
{
	m_pSire = pSire;

	m_Damage.iDamage = 500;
	m_Damage.iDamage_Range = 250;

	//UI
	const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_UI_Sire");
	if (pGameObjectList == nullptr)
		return E_FAIL;

	for (auto& pGameObject : *pGameObjectList)
	{
		CUI_Frame* pUIFrame = dynamic_cast<CUI_Frame*>(pGameObject);
		if (pUIFrame == nullptr)
			return E_FAIL;
		if (pUIFrame->Compare_Tag(L"Skill_FireShock"))
		{
			m_pUIFrame = pUIFrame;
			break;
		}
	}
	if (m_pUIFrame == nullptr)
		return E_FAIL;

	m_pUIFrame->Set_CoolDown(1.f);

	return S_OK;;
}

CSire_FireShock * CSire_FireShock::Create(CSire* pSire)
{
	CSire_FireShock*		pInstance = new CSire_FireShock();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_FireShock Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_FireShock::Free()
{
}


