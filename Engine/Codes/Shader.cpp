#include "..\Headers\Shader.h"


Engine::CShader::CShader(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
	,m_pEffect(nullptr)
{

}

Engine::CShader::CShader(const CShader& rhs)
	:CComponent(rhs)
	,m_pEffect(rhs.m_pEffect)
{
	m_pEffect->AddRef();
}

HRESULT Engine::CShader::Ready_Shader(const _tchar* pFileName)
{

	if (FAILED(D3DXCreateEffectFromFile(m_pDevice,
		pFileName,
		nullptr,
		nullptr,
		D3DXSHADER_DEBUG,
		nullptr,
		&m_pEffect,
		&m_pErrMsg)))
	{
		MessageBoxA(nullptr, (char*)m_pErrMsg->GetBufferPointer(), "Shader Error", MB_OK);
		return E_FAIL;
	}

	else if (nullptr != m_pErrMsg)
	{
		MessageBoxA(nullptr, (char*)m_pErrMsg->GetBufferPointer(), "Shader Warning", MB_OK);
	}

	
	return S_OK;
}

Engine::CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pFileName)
{
	CShader*		pInstance = new CShader(pDevice);

	if (FAILED(pInstance->Ready_Shader(pFileName)))
	{
		_MSG_BOX(L"CShader Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CComponent* Engine::CShader::Clone_Component()
{
	return new CShader(*this);
}

void Engine::CShader::Free()
{
	Safe_Release(m_pEffect);

	CComponent::Free();
}

