#include "stdafx.h"
#include "Lups_Attack.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Sound_Manager.h"
//Object
#include "Lups.h"
#include "Sword.h"
#include "Shield.h"

#include "Blood.h"

_USING(Client)

CLups_Attack::CLups_Attack()
	:m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

HRESULT CLups_Attack::Begin()
{
	m_iComboIndex = 4;
	m_pLups->Set_AnimationSet(m_iComboIndex, true);

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::LUPS);
	CSound_Manager::GetInstance()->Play_Sound(L"Lups_SwordThrowing0.mp3", CSound_Manager::LUPS);


	m_pSword = dynamic_cast<CSword*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Sword"));
	if (m_pSword == nullptr)
		return E_FAIL;

	m_pShield = dynamic_cast<CShield*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Shield"));
	if (m_pShield == nullptr)
		return E_FAIL;

	m_pSword->Begin_Trail();

	return S_OK;
}

_int CLups_Attack::Update(const _float & fTimeDelta)
{
	if (m_iComboIndex == 4)
	{
		if (m_pLups->Get_IsAnimationRange(0.1f,0.4f)/* && m_bIsCollision == false*/)
		{
			const DAMAGE*	pDemage = m_pLups->Get_Damage();
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_MONSTER, m_pSword->Get_Collider(), pDemage->iDamage + rand() % pDemage->iDamage_Range,EFFECT_BLOOD);
		
		}
		if (m_pLups->Get_IsAnimationRange(0.5f, 0.65f) /*&& m_bIsCollision_Second == false*/)
		{
			//_MSG_BOX(L"Check 4-2");
			const DAMAGE*	pDemage = m_pLups->Get_Damage();
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_MONSTER, m_pSword->Get_Collider(), pDemage->iDamage + rand() % pDemage->iDamage_Range, EFFECT_BLOOD);
		}

	}
	else if (m_iComboIndex == 5)
	{
		if (m_pLups->Get_IsAnimationRange(0.2f, 0.35f)/* && m_bIsCollision == false*/)
		{
			//_MSG_BOX(L"Check 5 ");
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_MONSTER, m_pSword->Get_Collider(), m_pLups->Get_Damage()->iDamage + rand() % m_pLups->Get_Damage()->iDamage_Range, EFFECT_BLOOD);
		}
	}
	else if (m_iComboIndex == 6)
	{
		if (m_pLups->Get_IsAnimationRange(0.3f,0.45f) /*&& m_bIsCollision == false*/)
		{
		//	_MSG_BOX(L"Check 6");
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_MONSTER, m_pSword->Get_Collider(), m_pLups->Get_Damage()->iDamage + rand() % m_pLups->Get_Damage()->iDamage_Range, EFFECT_BLOOD);
		}
	}
	else if (m_iComboIndex == 7)
	{
		if (m_pLups->Get_IsAnimationRange(0.4f,0.55f) /*&& m_bIsCollision == false*/)
		{
		//	_MSG_BOX(L"Check 7 ");
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_MONSTER, m_pSword->Get_Collider(), m_pLups->Get_Damage()->iDamage + rand() % m_pLups->Get_Damage()->iDamage_Range, EFFECT_BLOOD);

		}
	}
	else if (m_iComboIndex == 8)
	{
		if(!m_pLinear_Interpolation->Interpolate(fTimeDelta))
			CManagement::GetInstance()->Get_CameraMain()->Set_Fovy(D3DXToRadian(m_pLinear_Interpolation->GetValue()->x));

		if (m_pLups->Get_IsAnimationRange(0.2f, 0.25f) /*&& m_bIsCollision == false*/)
		{
			//_MSG_BOX(L"Check 8");
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(COL_MONSTER, m_pSword->Get_Collider(), /*m_pLups->Get_Damage()->iDamage + rand() %*/ m_pLups->Get_Damage()->iDamage_Range + 1000, EFFECT_BLOOD);
		}
	}

	Check_KeyInput(fTimeDelta);


	return 0;
}

_int CLups_Attack::LastUpdate(const _float & fTimeDelta)
{
	if (m_pLups->Get_IsAnimationSetEnd())
	{
		CManagement::GetInstance()->Get_CameraMain()->Set_Fovy(D3DXToRadian(60.0f));
		m_pLups->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CLups_Attack::End()
{
	m_pSword->End_Trail();

	return S_OK;
}

HRESULT CLups_Attack::Ready_State(CLups* pLups)
{
	m_pLups = pLups;

	m_pLinear_Interpolation = CLinear_Interpolation::Create();


	return S_OK;;
}

void CLups_Attack::Check_KeyInput(const _float & fTimeDelta)
{
	if(!m_pLups->Get_IsAnimationSetEnd())
	{
		if (m_pInput_Device->Get_MouseUp(CInput_Device::DIM_LB))
		{
			m_iComboIndex++;
			if (m_iComboIndex > 8)
			{
				m_pLups->Change_State(L"Idle");
				m_iComboIndex = 4;
				CManagement::GetInstance()->Get_CameraMain()->Set_Fovy(D3DXToRadian(60.0f));

				return;
			}

			m_pLups->Set_AnimationSet(m_iComboIndex, true);


			m_pSword->End_Trail();
			m_pSword->Begin_Trail();


			if (m_iComboIndex == 4)
			{
				CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::LUPS);
				CSound_Manager::GetInstance()->Play_Sound(L"Lups_SwordThrowing0.mp3", CSound_Manager::LUPS);
			}
			else if (m_iComboIndex == 5)
			{
				CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::LUPS);
				CSound_Manager::GetInstance()->Play_Sound(L"Lups_SwordThrowing1.mp3", CSound_Manager::LUPS);
			}
			else if (m_iComboIndex == 6)
			{
				CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::LUPS);
				CSound_Manager::GetInstance()->Play_Sound(L"Lups_SwordThrowing2.mp3", CSound_Manager::LUPS);
			}
			else if (m_iComboIndex == 7)
			{
				CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::LUPS);
				CSound_Manager::GetInstance()->Play_Sound(L"Lups_SwordThrowing3.mp3", CSound_Manager::LUPS);
			}
			else
			{
				CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::LUPS);
				CSound_Manager::GetInstance()->Play_Sound(L"Lups_SwordThrowing4.mp3", CSound_Manager::LUPS);

				m_pLinear_Interpolation->Setup(&_vec3(60.f, 0.f, 0.f), &_vec3(45.f, 0.f, 0.f),0.25f);
			}
		}
		
	}
}

CLups_Attack * CLups_Attack::Create(CLups* pLups)
{
	CLups_Attack*		pInstance = new CLups_Attack();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_Attack Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_Attack::Free()
{
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pLinear_Interpolation);
}


