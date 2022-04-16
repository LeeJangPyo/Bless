#include "stdafx.h"
#include "Sire_Heal.h"

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

CSire_Heal::CSire_Heal()
{
	ZeroMemory(&m_Damage, sizeof(DAMAGE));
}

HRESULT CSire_Heal::Begin()
{
	if (!m_pSire->Use_Mp(200))
		return S_OK;

	m_pSire->Set_AnimationSet(4);
	m_pSire->Set_FrameConst(0.75);

	_vec3 vPosition = m_pSire->Get_Spell_Target();
	vPosition.y += 0.2f;


	m_pEffect_Heal->Set_Position(&vPosition);
	m_pEffect_Heal->Set_Activate(true);

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::SIRE);
	CSound_Manager::GetInstance()->Play_Sound(L"Sire_Heal.wav", CSound_Manager::SIRE);

	return S_OK;
}

_int CSire_Heal::Update(const _float & fTimeDelta)
{
	return 0;
}

_int CSire_Heal::LastUpdate(const _float & fTimeDelta)
{
	if (m_pSire->Get_IsAnimationSetEnd())
	{
		m_pSire->Change_State(L"Idle");
	}
	return 0;
}

HRESULT CSire_Heal::End()
{
	m_pSire->Set_FrameConst(1.f);

	return S_OK;
}

HRESULT CSire_Heal::Ready_State(CSire* pSire)
{
	m_pSire = pSire;

	m_Damage.iDamage = 1000;
	m_Damage.iDamage_Range = 1000;

	//FireBall ¾ò¾î¿È
	m_pEffect_Heal = dynamic_cast<CEffectGroup*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Sire", 7));
	if (m_pEffect_Heal == nullptr)
		return E_FAIL;

	m_pEffect_Heal->SetUp_ColliderInfo(4.5f, COLLISION_TARGET::COL_PLAYER, 0.f, 1.5f);
	m_pEffect_Heal->Set_EffectType(EFFECT_HEAL);
	m_pEffect_Heal->Set_Damage(&m_Damage);

	return S_OK;
}

CSire_Heal * CSire_Heal::Create(CSire* pSire)
{
	CSire_Heal*		pInstance = new CSire_Heal();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_Heal Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_Heal::Free()
{
}


