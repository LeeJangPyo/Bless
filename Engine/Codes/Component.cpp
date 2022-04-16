#include "..\Headers\Component.h"

CComponent::CComponent(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
	, m_isClone(false)
{
	m_pDevice->AddRef();
}

CComponent::CComponent(const CComponent & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_isClone(true)
{
	m_pDevice->AddRef();
}

void CComponent::Free()
{
	Safe_Release(m_pDevice);
}
