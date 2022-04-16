#include "stdafx.h"
#include "Effect_QuatraBreath.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "EffectGroup.h"
#include "Player_Manager.h"


#include "Sire.h"
_USING(Client)

CEffect_QuatraBreath::CEffect_QuatraBreath(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fimeAcc(0.f)
	, m_pCollider(nullptr)
	, m_pCalculator(nullptr)
	, m_pShader(nullptr)
	, m_fRoltation(0.f)
	, m_vPosition(0.f,0.f,0.f)
	, m_bActivate(false)
{
	ZeroMemory(&m_Damage, sizeof(DAMAGE));
	ZeroMemory(&m_Ray, sizeof(RAY));
}

CEffect_QuatraBreath::CEffect_QuatraBreath(const CEffect_QuatraBreath & rhs)
	: CGameObject(rhs)
	, m_fimeAcc(0.f)
	, m_pCollider(nullptr)
	, m_pCalculator(nullptr)
	, m_pShader(nullptr)
	, m_fRoltation(0.f)
	, m_vPosition(0.f, 0.f, 0.f)
	, m_bActivate(false)
{
	ZeroMemory(&m_Damage, sizeof(DAMAGE));
	ZeroMemory(&m_Ray, sizeof(RAY));

}

HRESULT CEffect_QuatraBreath::Ready_Prototype()
{
	return S_OK;
}

HRESULT CEffect_QuatraBreath::Ready_GameObject()
{

	m_Damage.iDamage = 300;
	m_Damage.iDamage_Range = 200;

	CSire* pSire = dynamic_cast<CSire*>(CPlayer_Manager::GetInstance()->Get_Player(PLAYER_SIRE));
	if (pSire == nullptr)
		return E_FAIL;

	//[L"GameObject_Effect_Effect_QuatraBreath"] = 0xd16490c8 {m_vecEffectMesh={ size=0 } m_vecEffectRect={ size=0 } m_vecEffectParticle={ size=0 } ...}
	m_pEffect_Breath = dynamic_cast<CEffectGroup*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Quatra", 0));
	if (m_pEffect_Breath == nullptr)
		return E_FAIL;


	m_pCollider = dynamic_cast<CCollider_Sphere*>(CManagement::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Collider_Sphere"));
	if (m_pCollider == nullptr)
		return E_FAIL;

	m_pCollider->SetUp_Collider_Spehre(&_vec3(0.f, 0.f, 0.f), 6.f);

	m_pCalculator = dynamic_cast<CCalculator*>(CManagement::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Calculator"));
	if (m_pCollider == nullptr)
		return E_FAIL;

	m_pShader = dynamic_cast<CShader*>(CManagement::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Shader_Default"));
	if (m_pShader == nullptr)
		return E_FAIL;

	m_pRenderer = dynamic_cast<CRenderer*>(CManagement::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (m_pShader == nullptr)
		return E_FAIL;



	return S_OK;
}

_int CEffect_QuatraBreath::Update_GameObject(const _float & fTimeDelta)
{
	if (!m_bActivate)
		return 0;

	m_pEffect_Breath->Set_Position(&m_vPosition);

	m_pEffect_Breath->Set_RotationY(m_fRoltation);
	
	_vec3 vLook(0.f, 0.f, 1.f); //*m_pEffect_Breath->Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK);
	_matrix matRotX;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(60.f));
	_matrix matRotY;
	D3DXMatrixRotationY(&matRotY, m_fRoltation);
	_matrix matTransform =   matRotX *matRotY;
	D3DXVec3TransformNormal(&vLook,&vLook, &matTransform);
	D3DXVec3Normalize(&vLook, &vLook);

	m_Ray.vRayPos = m_vPosition;
	m_Ray.vRayDir = vLook;

	CBuffer_TerrainTex*	pTerrainBufferCom = dynamic_cast<CBuffer_TerrainTex*>
		(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain_Stage2", L"Com_Buffer"));
	if (pTerrainBufferCom == nullptr)
		return 0;

	CTransform*		pTerrainTransCom = dynamic_cast<Engine::CTransform*>
		(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain_Stage2", L"Com_Transform"));
	if (pTerrainTransCom == nullptr)
		return 0;

	_vec3 vPickingPosition = m_pCalculator->Picking_OnTerrain(m_Ray, pTerrainBufferCom, pTerrainTransCom);

	m_pCollider->Set_Center_Position(&vPickingPosition);

	CCollision_Manager::GetInstance()->
		Check_CollisionAndHit(COL_PLAYER, m_pCollider, m_Damage.iDamage + rand() % m_Damage.iDamage_Range);


	return _int();
}

_int CEffect_QuatraBreath::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (m_pRenderer == nullptr)
		return 0;
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI,this);
	
	return _int();
}

void CEffect_QuatraBreath::Render_GameObject()
{
	if (g_bDebug)
	{
		LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
		if (pEffect == nullptr)
			return;

		m_pCollider->SetUp_ConstantTable(pEffect, CCollider_Sphere::COLOR_GREEN);

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);

		pEffect->CommitChanges();
		m_pCollider->Render_Collider_Sphere();

		pEffect->EndPass();
		pEffect->End();
	}
	
}

void CEffect_QuatraBreath::Set_Activate(_bool bActivate)
{
	m_bActivate = bActivate;
	m_pEffect_Breath->Set_Activate(bActivate);
}


CGameObject * CEffect_QuatraBreath::Clone_GameObject()
{
	CEffect_QuatraBreath*		pInstance = new CEffect_QuatraBreath(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CEffect_QuatraBreath Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEffect_QuatraBreath * CEffect_QuatraBreath::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEffect_QuatraBreath*		pInstance = new CEffect_QuatraBreath(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CEffect_QuatraBreath Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_QuatraBreath::Free()
{
	Safe_Release(m_pCalculator);
	Safe_Release(m_pCollider);
	Safe_Release(m_pShader);
	Safe_Release(m_pRenderer);
	CGameObject::Free();
}
