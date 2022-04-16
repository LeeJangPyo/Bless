#include "stdafx.h"
#include "Flogas_Summon.h"

#include "Flogas.h"

#include "Management.h"
#include "Sound_Manager.h"

#include "EffectGroup.h"

#include "Ghost_Mage.h"

_USING(Client)

CFlogas_Summon::CFlogas_Summon()
	: m_fTimeAcc(0.f)
	, m_fSummonAccTime(0.f)
{
}

HRESULT CFlogas_Summon::Begin()
{
	m_pFlogas->Set_AnimationSet(1);

	const list<CGameObject*>* pGameObjectList= CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Stage2_Flogas_Summon");
	if (pGameObjectList != nullptr)
	{
		_uint iSize = pGameObjectList->size();

		if (iSize > 1)
		{
			m_pFlogas->Change_State(L"Chase");
			return S_OK;
		}
	}
		

	
	m_pEffect_MagicSquare->Set_Position(m_pFlogas_Transform->Get_StateInfo(CTransform::STATE_POSITION));
	m_pEffect_MagicSquare->Set_Activate(true);

	m_fTimeAcc = 0.f;
	m_fSummonAccTime = 0.f;

	m_bPlayOnce_Effect = false;
	m_bPlayOnce_Summon = false;

	CManagement* pManagement = CManagement::GetInstance();
	pManagement->AddRef();
	{

	pManagement->Add_Layer(SCENE_STAGE, L"Layer_Effect_Flogas", L"GameObject_Effect_Flogas_Summon_Zone",
		(CGameObject**)&m_pSummonZone_0);

	pManagement->Add_Layer(SCENE_STAGE, L"Layer_Effect_Flogas", L"GameObject_Effect_Flogas_Summon_Zone",
		(CGameObject**)&m_pSummonZone_1);

	pManagement->Add_Layer(SCENE_STAGE, L"Layer_Effect_Flogas", L"GameObject_Effect_Flogas_Summon_Zone",
		(CGameObject**)&m_pSummonZone_2);

	pManagement->Add_Layer(SCENE_STAGE, L"Layer_Effect_Flogas", L"GameObject_Effect_Flogas_Summon_Zone",
		(CGameObject**)&m_pSummonZone_3);
	}
	Safe_Release(pManagement);


	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
	CSound_Manager::GetInstance()->Play_Sound(L"Flogas_Skill_Summon.mp3", CSound_Manager::BOSS);

	return S_OK;
}

_int CFlogas_Summon::Update(const _float & fTimeDelta)
{
	if (m_pFlogas->Get_IsAnimationPassRadio(0.5f) && m_bPlayOnce_Effect == false)
	{
		_vec3 vSummonPosition = *m_pFlogas_Transform->Get_StateInfo(CTransform::STATE_POSITION);
		vSummonPosition.x += 5.f;
		vSummonPosition.z += 5.f;


		m_pSummonZone_0->Set_Position(&vSummonPosition);
		m_pSummonZone_0->Set_Activate(true);
		m_pSummonZone_0->Set_DeadObject(true);

		vSummonPosition = *m_pFlogas_Transform->Get_StateInfo(CTransform::STATE_POSITION);
		vSummonPosition.x -= 5.f;
		vSummonPosition.z += 5.f;


		m_pSummonZone_1->Set_Position(&vSummonPosition);
		m_pSummonZone_1->Set_Activate(true);
		m_pSummonZone_1->Set_DeadObject(true);


		vSummonPosition = *m_pFlogas_Transform->Get_StateInfo(CTransform::STATE_POSITION);
		vSummonPosition.x += 5.f;
		vSummonPosition.z -= 5.f;

		m_pSummonZone_2->Set_Position(&vSummonPosition);
		m_pSummonZone_2->Set_Activate(true);
		m_pSummonZone_2->Set_DeadObject(true);



		vSummonPosition = *m_pFlogas_Transform->Get_StateInfo(CTransform::STATE_POSITION);
		vSummonPosition.x -= 5.f;
		vSummonPosition.z -= 5.f;

		m_pSummonZone_3->Set_Position(&vSummonPosition);
		m_pSummonZone_3->Set_Activate(true);
		m_pSummonZone_3->Set_DeadObject(true);


		m_bPlayOnce_Effect = true;
		m_pFlogas->Set_FrameConst(0.15f);
	}



	if(m_pSummonZone_0->Get_ProgressRadio() > 0.5 && m_bPlayOnce_Summon == false)
	{
		CManagement*	pManagement = CManagement::GetInstance();
		pManagement->AddRef();
		{

			CTransform*	pTransform = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));

			pTransform->Set_RotationY(180.f);
			pTransform->Set_ScaleInfo(0.15f, 0.15f, 0.15f);
			pTransform->Set_StateInfo(CTransform::STATE_POSITION, m_pSummonZone_0->Get_Position());

			//First Summon
			CGhost_Mage*	pGhost_Mage;
			pManagement->Add_Layer(SCENE_STAGE, L"Stage2_Flogas_Summon", L"GameObject_Ghost_Mage", (CGameObject**)&pGhost_Mage);
			pGhost_Mage->SetUp_GameObjectInfo(pTransform->Get_WorldMatrix());
			pGhost_Mage->Change_State(L"Ready_Combat");
			

			//Second Summon
			pTransform->Set_StateInfo(CTransform::STATE_POSITION, m_pSummonZone_1->Get_Position());
			pManagement->Add_Layer(SCENE_STAGE, L"Stage2_Flogas_Summon", L"GameObject_Ghost_Mage", (CGameObject**)&pGhost_Mage);
			pGhost_Mage->SetUp_GameObjectInfo(pTransform->Get_WorldMatrix());
			pGhost_Mage->Change_State(L"Ready_Combat");
			

			//Third Summon
			pTransform->Set_StateInfo(CTransform::STATE_POSITION, m_pSummonZone_2->Get_Position());
			pManagement->Add_Layer(SCENE_STAGE, L"Stage2_Flogas_Summon", L"GameObject_Ghost_Mage", (CGameObject**)&pGhost_Mage);
			pGhost_Mage->SetUp_GameObjectInfo(pTransform->Get_WorldMatrix());
			pGhost_Mage->Change_State(L"Ready_Combat");
			

			//Fourd Summon
			pTransform->Set_StateInfo(CTransform::STATE_POSITION, m_pSummonZone_3->Get_Position());
			pManagement->Add_Layer(SCENE_STAGE, L"Stage2_Flogas_Summon", L"GameObject_Ghost_Mage", (CGameObject**)&pGhost_Mage);
			pGhost_Mage->SetUp_GameObjectInfo(pTransform->Get_WorldMatrix());
			pGhost_Mage->Change_State(L"Ready_Combat");
			
			Safe_Release(pTransform);

		}
		Safe_Release(pManagement);
		
		m_bPlayOnce_Summon = true;
	}
	
	return 0;
}

_int CFlogas_Summon::LastUpdate(const _float & fTimeDelta)
{
	if (m_pFlogas->Get_IsAnimationSetEnd())
	{
		m_pFlogas->Change_State(L"Idle");
	}

	return _int();
}

HRESULT CFlogas_Summon::End()
{
	m_pFlogas->Set_FrameConst(1.f);

	m_pEffect_MagicSquare->Set_Activate(false);

	const list<CGameObject*>* pGameObjectList = CManagement::GetInstance()->Get_Layer_ObjectList(SCENE_STAGE, L"Stage2_Flogas_Summon");
	if (pGameObjectList == nullptr)
		return E_FAIL;

	for (auto& pGameObject : *pGameObjectList)
	{
		CGameObject_Dynamic* pMonster = dynamic_cast<CGameObject_Dynamic*>(pGameObject);
		pMonster->Change_State(L"Idle");
	}

	return S_OK;
}

HRESULT CFlogas_Summon::Ready_State(CFlogas * pFlogas)
{
	m_pFlogas  = pFlogas;


	m_pFlogas_Transform = pFlogas->Get_Transform();


	m_pEffect_MagicSquare = dynamic_cast<CEffectGroup*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Flogas", 2));
	if (m_pEffect_MagicSquare == nullptr)
		return E_FAIL;

	return S_OK;
}

CFlogas_Summon * CFlogas_Summon::Create(CFlogas* pFlogas)
{
	CFlogas_Summon*		pInstance = new CFlogas_Summon();

	if (FAILED(pInstance->Ready_State(pFlogas)))
	{
		_MSG_BOX(L"CFlogas_Summon Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlogas_Summon::Free()
{
}


