#include "Collider_Sphere.h"

#include "Texture.h"
#include "Transform.h"

Engine::CCollider_Sphere::CCollider_Sphere(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
	, m_pMesh_Sphere(nullptr)
	, m_pTextureCom(nullptr)
	, m_pmatParentWorld(nullptr)
	, m_pmatParentBone(nullptr)
{
	D3DXMatrixIdentity(&m_matCombine);
	D3DXMatrixIdentity(&m_matWorld);
}

Engine::CCollider_Sphere::CCollider_Sphere(const CCollider_Sphere& rhs)
	: CComponent(rhs)
	, m_pMesh_Sphere(nullptr)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_matCombine(rhs.m_matCombine)
	, m_vCenter(rhs.m_vCenter)
	, m_fRadius(rhs.m_fRadius)
	, m_matWorld(rhs.m_matWorld)
{
	m_pTextureCom->AddRef();
}
CCollider_Sphere& Engine::CCollider_Sphere::operator=(const CCollider_Sphere& rhs)
{

	m_pTextureCom = rhs.m_pTextureCom;
	m_pTextureCom->AddRef();
	m_vCenter = rhs.m_vCenter;
	m_fRadius = rhs.m_fRadius;
	m_matWorld = rhs.m_matWorld;

	return *this;

}

HRESULT Engine::CCollider_Sphere::Ready_Collider_Spehre()
{

	m_pTextureCom = CTexture::Create(m_pDevice);
	return S_OK;
}

_int CCollider_Sphere::Update_Collider_Sphere()
{
	//Update
	if (m_pmatParentBone != nullptr)
	{
		_matrix matParent = (*m_pmatParentBone) * (*m_pmatParentWorld);
		m_matCombine = m_matWorld * matParent;
		memcpy(&m_vCenter, (_vec3*)&m_matCombine.m[3][0], sizeof(_vec3));
		m_fRadius = m_fOriginRadius * D3DXVec3Length((_vec3*)&m_matCombine.m[0][0]);
	}
	else
	{
		m_matCombine = m_matWorld;
	}
	

	return 0;
}

void Engine::CCollider_Sphere::Render_Collider_Sphere()
{
	m_pMesh_Sphere->DrawSubset(0);
}


Engine::CCollider_Sphere* Engine::CCollider_Sphere::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCollider_Sphere*	pInstance = new CCollider_Sphere(pDevice);

	if (FAILED(pInstance->Ready_Collider_Spehre()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCollider_Sphere::Free(void)
{
	Safe_Release(m_pMesh_Sphere);
	Safe_Release(m_pTextureCom);

	CComponent::Free();
}


void CCollider_Sphere::Set_Center_Position(const _vec3 * pCenterPos)
{
	m_vOriginCenter = m_vCenter=*pCenterPos;
	D3DXMatrixTranslation(&m_matCombine, pCenterPos->x, pCenterPos->y, pCenterPos->z);
	D3DXMatrixTranslation(&m_matWorld, pCenterPos->x, pCenterPos->y, pCenterPos->z);
}


void CCollider_Sphere::Set_Radius(const _float & fRaidus)
{
	m_fOriginRadius = m_fRadius = fRaidus;

	Safe_Release(m_pMesh_Sphere);
	
	D3DXCreateSphere(m_pDevice, m_fOriginRadius, 10, 10, &m_pMesh_Sphere, nullptr);
}


Engine::CComponent* Engine::CCollider_Sphere::Clone_Component()
{
	return new CCollider_Sphere(*this);
}

HRESULT Engine::CCollider_Sphere::SetUp_Collider_Spehre(const _vec3* pCenter, const _float& fRadius)
{
	D3DXMatrixTranslation(&m_matCombine, pCenter->x, pCenter->y, pCenter->z);
	m_fOriginRadius = m_fRadius= fRadius;
	m_vOriginCenter = m_vCenter= *pCenter;

	//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &m_vCenter);
	D3DXCreateSphere(m_pDevice, m_fOriginRadius, 10, 10, &m_pMesh_Sphere, nullptr);

	return S_OK;
}


HRESULT CCollider_Sphere::SetUp_Collider_Spehre(const _matrix * pmatParentBone, const _matrix * pmatParentWorld,const _vec3* pCenterPos, const _float & fRadius)
{
	m_pmatParentBone = pmatParentBone;
	m_pmatParentWorld = pmatParentWorld;

	m_fOriginRadius = fRadius;
	m_vOriginCenter = *pCenterPos;

	D3DXMatrixTranslation(&m_matWorld, m_vOriginCenter.x, m_vOriginCenter.y, m_vOriginCenter.z);

	D3DXCreateSphere(m_pDevice, m_fOriginRadius, 10, 10, &m_pMesh_Sphere, nullptr);

	return S_OK;
}

HRESULT CCollider_Sphere::SetUp_Collider_Spehre(const _matrix * pmatParentBone, const _matrix * pmatParentWorld)
{
	m_pmatParentBone = pmatParentBone;
	m_pmatParentWorld = pmatParentWorld;

	_matrix matCenter = *pmatParentBone * *pmatParentWorld;
	memcpy(&m_vOriginCenter, (_vec3*)&matCenter.m[3][0], sizeof(_vec3));

	//
	D3DXCreateSphere(m_pDevice, m_fOriginRadius, 10, 10, &m_pMesh_Sphere, nullptr);

	return S_OK;
}




HRESULT CCollider_Sphere::SetUp_ConstantTable(LPD3DXEFFECT pEffect, COLORTYPE eType)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &m_matCombine);

	_matrix matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", eType);

	Safe_Release(pEffect);

	return S_OK;
}

HRESULT CCollider_Sphere::Save_ColliderInfo(const HANDLE & hFile, _ulong & dwByte)
{
	WriteFile(hFile, &m_fOriginRadius, sizeof(_float), &dwByte, nullptr);
	WriteFile(hFile, &m_vOriginCenter, sizeof(_vec3), &dwByte, nullptr);
	return S_OK;
}

