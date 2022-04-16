#include "stdafx.h"
#include "..\Headers\Meteo_Strike.h"

#include "Management.h"

#include "EffectGroup.h"
#include "Player_Manager.h"
#include "Sound_Manager.h"

#include "Sire.h"
_USING(Client)

CMeteo_Strike::CMeteo_Strike(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fimeAcc(0.f)
{
	ZeroMemory(&m_Damage, sizeof(DAMAGE));
}

CMeteo_Strike::CMeteo_Strike(const CMeteo_Strike & rhs)
	: CGameObject(rhs)
	, m_fimeAcc(0.f)
{
	ZeroMemory(&m_Damage, sizeof(DAMAGE));
}

HRESULT CMeteo_Strike::Ready_Prototype()
{
	return S_OK;
}

HRESULT CMeteo_Strike::Ready_GameObject()
{

	m_Damage.iDamage = 3500;
	m_Damage.iDamage_Range = 1000;

	CSire* pSire = dynamic_cast<CSire*>(CPlayer_Manager::GetInstance()->Get_Player(PLAYER_SIRE));
	if (pSire == nullptr)
		return E_FAIL;

	//[L"GameObject_Effect_Meteo_Strike"] = 0xd16490c8 {m_vecEffectMesh={ size=0 } m_vecEffectRect={ size=0 } m_vecEffectParticle={ size=0 } ...}
	CManagement::GetInstance()->Add_Layer
	(SCENE_STAGE, L"Layer_Effect_Meteo_Strike", L"GameObject_Effect_Meteo_Strike", (CGameObject**)&m_pEffect_Meteo_Strike);
	if (m_pEffect_Meteo_Strike == nullptr)
		return E_FAIL;

	m_pEffect_Meteo_Strike->Set_DeadObject(true);
	m_pEffect_Meteo_Strike->Set_Activate(true);

	CManagement::GetInstance()->Add_Layer
	(SCENE_STAGE, L"Layer_Effect_Meteo_Explosion", L"GameObject_Effect_Meteo_Explosion", (CGameObject**)&m_pEffect_Meteo_Explosion);
	if (m_pEffect_Meteo_Explosion == nullptr)
		return E_FAIL;

	_vec3 vStart_Position = pSire->Get_Spell_Target();
	vStart_Position.x += 5.f;
	vStart_Position.y += 10.f;
	_vec3 vEnd_Position = pSire->Get_Spell_Target();
	_vec3 vDir = vEnd_Position - vStart_Position;
	_float	fDistance = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	m_pEffect_Meteo_Strike->Set_Position(&vStart_Position);
	m_pEffect_Meteo_Strike->RotationY(90.f);
	m_pEffect_Meteo_Strike->RotationZ(-90.f);
	m_pEffect_Meteo_Strike->Set_Move(true, fDistance / 3.f, &vDir);
	m_pEffect_Meteo_Strike->Set_DeadObject(true);
	m_pEffect_Meteo_Strike->Set_Activate(true);


	m_pEffect_Meteo_Explosion->SetUp_ColliderInfo(5.f, COLLISION_TARGET::COL_MONSTER, 0.f, 0.25f);
	m_pEffect_Meteo_Explosion->Set_DeadObject(true);
	m_pEffect_Meteo_Explosion->Set_Damage(&m_Damage);
	m_pEffect_Meteo_Explosion->Set_Position(&vEnd_Position);

	return S_OK;
}

_int CMeteo_Strike::Update_GameObject(const _float & fTimeDelta)
{
	m_fimeAcc += fTimeDelta;
	if (m_fimeAcc > 3.f)
	{
		CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::SIRE);
		CSound_Manager::GetInstance()->Play_Sound(L"Sire_MeteoExplosion.wav", CSound_Manager::SIRE);

		m_pEffect_Meteo_Explosion->Set_Activate(true);
		return DEAD_OBJ;
	}

	return _int();
}

_int CMeteo_Strike::LastUpdate_GameObject(const _float & fTimeDelta)
{

	return _int();
}

void CMeteo_Strike::Render_GameObject()
{

}


CGameObject * CMeteo_Strike::Clone_GameObject()
{
	CMeteo_Strike*		pInstance = new CMeteo_Strike(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CMeteo_Strike Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMeteo_Strike * CMeteo_Strike::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMeteo_Strike*		pInstance = new CMeteo_Strike(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CMeteo_Strike Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeteo_Strike::Free()
{
	CGameObject::Free();
}
