#include "stdafx.h"
#include "Sire_Ready_FireBall.h"

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

CSire_Ready_FireBall::CSire_Ready_FireBall()
{

}

HRESULT CSire_Ready_FireBall::Begin()
{
	if (!m_pSire->Use_Mp(500))
		return S_OK;

	if (m_pUIFrame->Get_IsCoolDown())
	{
		m_pSire->Change_State(L"Idle");
		return S_OK;
	}
	m_pUIFrame->Set_IsCoolDown(true);

	m_fTimeAcc = 0.f;

	m_pSire->Set_AnimationSet(14);

	// CastFireBall Parent Matrix
	m_pEffect_FireOrb->Set_ParentMatrix(L"Layer_Sire", "Bip01-L-Finger11");
	 
	m_pEffect_FireOrb->Set_Activate(true);

	_vec3 vPosition = *m_pSire->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	vPosition.y += 0.2f;
	m_pEffect_MagicZone->Set_Position(&vPosition);
	m_pEffect_MagicZone->Set_Activate(true);

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::SIRE);
	CSound_Manager::GetInstance()->Play_Sound(L"Sire_Ready_Spell.mp3", CSound_Manager::SIRE);

	return S_OK;
}

_int CSire_Ready_FireBall::Update(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;


	return 0;
}

_int CSire_Ready_FireBall::LastUpdate(const _float & fTimeDelta)
{
	if (m_fTimeAcc > m_fCastingTime)
		m_pSire->Change_State(L"Cast_FireBall");

	if (m_pSire->Get_IsAnimationSetEnd())
	{
		m_pSire->Set_AnimationSet(14);
	}

	return 0;
}

HRESULT CSire_Ready_FireBall::End()
{
	m_pEffect_FireOrb->Set_Activate(false);
	m_pEffect_MagicZone->Set_Activate(false);
	return S_OK;
}

HRESULT CSire_Ready_FireBall::Ready_State(CSire* pSire)
{
	m_pSire = pSire;

	m_fCastingTime = 5.f;

	m_pEffect_FireOrb = dynamic_cast<CEffectGroup*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Sire",0));
	if (m_pEffect_FireOrb == nullptr)
		return E_FAIL;

	m_pEffect_MagicZone = dynamic_cast<CEffectGroup*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Sire", 5));
	if (m_pEffect_MagicZone == nullptr)
		return E_FAIL;

	//UI
	const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Layer_UI_Sire");
	if (pGameObjectList == nullptr)
		return E_FAIL;

	for (auto& pGameObject : *pGameObjectList)
	{
		CUI_Frame* pUIFrame = dynamic_cast<CUI_Frame*>(pGameObject);
		if (pUIFrame == nullptr)
			return E_FAIL;
		if (pUIFrame->Compare_Tag(L"Skill_FireBall"))
		{
			m_pUIFrame = pUIFrame;
			break;
		}
	}
	if (m_pUIFrame == nullptr)
		return E_FAIL;
	m_pUIFrame->Set_CoolDown(15.f);

	return S_OK;;
}

CSire_Ready_FireBall * CSire_Ready_FireBall::Create(CSire* pSire)
{
	CSire_Ready_FireBall*		pInstance = new CSire_Ready_FireBall();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_Ready_FireBall Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_Ready_FireBall::Free()
{
}


