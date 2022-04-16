#include "Light.h"

#include "Buffer_Screen.h"

_USING(Engine)

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
	, m_pBuffer_Screen(nullptr)
	, m_iIndex(0)
{
	ZeroMemory(&m_LightInfo, sizeof(D3DLIGHT9));
	ZeroMemory(&m_MtrlInfo, sizeof(D3DMATERIAL9));

	m_pDevice->AddRef();

}



HRESULT Engine::CLight::Ready_Light(const D3DLIGHT9* pLightInfo,
									const _uint& iIndex)
{	
	memcpy(&m_LightInfo, pLightInfo, sizeof(D3DLIGHT9));

	m_iIndex = iIndex;

	m_pBuffer_Screen = CBuffer_Screen::Create(m_pDevice);
	
	return S_OK;
}

void CLight::Render_Light(LPD3DXEFFECT pEffect)
{
	_uint			iPassIdx = 0;

	if (D3DLIGHT_DIRECTIONAL == m_LightInfo.Type)
	{
		pEffect->SetVector("g_vLightDir", &_vec4(m_LightInfo.Direction, 0.f));
		iPassIdx = 0;
	}

	if (D3DLIGHT_POINT == m_LightInfo.Type)
	{
		pEffect->SetVector("g_vLightPos", &_vec4(m_LightInfo.Position, 1.f));
		pEffect->SetFloat("g_fRange", m_LightInfo.Range);
		iPassIdx = 1;
	}

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&m_LightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&m_LightInfo.Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&m_LightInfo.Specular);

	_matrix			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);
	pEffect->SetFloat("g_fFarPlane", 1000.f);

	pEffect->BeginPass(iPassIdx);

	pEffect->CommitChanges();

	m_pBuffer_Screen->Render_Buffer();

	pEffect->EndPass();
}

Engine::CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pDevice, 
										const D3DLIGHT9* pLightInfo, 
										const _uint& iIndex)
{
	CLight*	pInstance = new CLight(pDevice);

	if(FAILED(pInstance->Ready_Light(pLightInfo, iIndex)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLight::Free(void)
{
	Safe_Release(m_pBuffer_Screen);
	Safe_Release(m_pDevice);
}


