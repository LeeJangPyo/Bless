#include "Light_Manager.h"

_IMPLEMENT_SINGLETON(CLight_Manager)

Engine::CLight_Manager::CLight_Manager(void)
{

}

HRESULT CLight_Manager::Remove_All_Light()
{
	for (auto& pLight : m_LightList)
		Safe_Release(pLight);
	m_LightList.clear();

	return S_OK;
}

void CLight_Manager::Set_ShadowLightMatrix(const _vec3 * pLightPosition, const _vec3 * pLightAt, const _vec3 * pLightUp)
{
	_vec3 LightPosition = *pLightPosition;
	_vec3 LightAt = *pLightAt;
	_vec3 LightUp = *pLightUp;

	D3DXMatrixIdentity(&m_matShadowLight);

	D3DXMatrixLookAtLH(&m_matShadowLight, &LightPosition, &LightAt, &LightUp);
}

HRESULT Engine::CLight_Manager::Ready_Light(LPDIRECT3DDEVICE9 pDevice,
										const D3DLIGHT9* pLightInfo, 
										const _uint& iIndex)
{
	CLight*		pInstance = CLight::Create(pDevice, pLightInfo, iIndex);

	m_LightList.push_back(pInstance);

	return S_OK;
}

void CLight_Manager::Render_Light(LPD3DXEFFECT pEffect)
{
	for (auto& pLight : m_LightList)
		pLight->Render_Light(pEffect);
}

HRESULT CLight_Manager::Load_LightInfo_FromFile(LPDIRECT3DDEVICE9 pDevice,const _tchar * pFilePath)
{
	HANDLE		hFile = CreateFile(pFilePath
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD	dwByte = 0;
	_ulong	dwIndex = 0;


	while (true)
	{
		D3DLIGHT9 LightInfo;
		ReadFile(hFile, &LightInfo, sizeof(D3DLIGHT9), &dwByte, nullptr);


		if (dwByte == 0)
			break;

		CLight*		pInstance = CLight::Create(pDevice, &LightInfo, 0);

		m_LightList.push_back(pInstance);
	}


	CloseHandle(hFile);

	return S_OK;
}

CLight * CLight_Manager::Get_Light(const _uint & iIndex)
{
	if (m_LightList.size() <= iIndex)
		return nullptr;

	auto	iter = m_LightList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return *iter;
}

const D3DLIGHT9* Engine::CLight_Manager::Get_LightInfo(const _uint& iIndex /*= 0*/)
{
	auto		iter = m_LightList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_LightInfo();
}

void Engine::CLight_Manager::Free(void)
{
	for (auto& pLight : m_LightList)
		Safe_Release(pLight);
	m_LightList.clear();
}