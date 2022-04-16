#include "..\Headers\Target_Manager.h"
#include "Target.h"


_IMPLEMENT_SINGLETON(CTarget_Manager)

Engine::CTarget_Manager::CTarget_Manager()
{

}


HRESULT CTarget_Manager::Ready_Target(LPDIRECT3DDEVICE9 pDevice, const _tchar * pTargetTag, const _float & fSizeX, const _float & fSizeY, D3DFORMAT Format,D3DXCOLOR Clear_Color, CTarget::TARGETTYPE eType)
{
	CTarget*	pTarget = Find_Target(pTargetTag);
	if (nullptr != pTarget)
		return E_FAIL;

	pTarget = CTarget::Create(pDevice, fSizeX, fSizeY, Format, Clear_Color, eType);
	if (nullptr == pTarget)
		return E_FAIL;

	m_mapTarget.insert(MAPTARGET::value_type(pTargetTag, pTarget));

	return S_OK;
}

HRESULT CTarget_Manager::Ready_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag)
{
	CTarget*	pTarget = Find_Target(pTargetTag);
	if (nullptr == pTarget)
		return E_FAIL;

	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CTarget*>	MRTList;
		MRTList.push_back(pTarget);

		m_mapMRT.insert(MAPMRT::value_type(pMRTTag, MRTList));
	}
	else
		pMRTList->push_back(pTarget);

	pTarget->AddRef();

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(const _tchar* pMRTTag)
{
	list<CTarget*>*	pTargetList	= Find_MRT(pMRTTag);
	if (pTargetList == nullptr)
		return E_FAIL;

	//Clear Target Buffer
	for (auto& pTarget : *pTargetList)
	{
		if (nullptr != pTarget)
		{
			pTarget->Clear_Target();
		}
	}

	_uint iIndex = 0;

	//SetUp Render target to Graphic_Device
	for(auto& pTarget : *pTargetList)
	{
		pTarget->SetUp_OnGraphic_Device(iIndex++);
	}

	return S_OK;
}

HRESULT CTarget_Manager::End_MRT(const _tchar* pMRTTag)
{
	list<CTarget*>*	pTargetList = Find_MRT(pMRTTag);
	if (pTargetList == nullptr)
		return E_FAIL;

	_uint iIndex = 0;

	for (auto& pTarget : *pTargetList)
	{
		pTarget->Release_OnGraphic_Device(iIndex++);
	}

	return S_OK;
}

HRESULT CTarget_Manager::Begin_Shadow_MRT(const _tchar * pMRTTag)
{
	list<CTarget*>*	pTargetList = Find_MRT(pMRTTag);
	if (pTargetList == nullptr)
		return E_FAIL;

	//Clear Target Buffer
	for (auto& pTarget : *pTargetList)
	{
		if (nullptr != pTarget)
		{
			pTarget->Clear_Shadow_Target();
		}
	}

	_uint iIndex = 0;

	//SetUp Render target to Graphic_Device
	for (auto& pTarget : *pTargetList)
	{
		pTarget->SetUp_OnGraphic_Device_Shadow(iIndex++);
	}

	return S_OK;
}

HRESULT CTarget_Manager::End_Shadow_MRT(const _tchar * pMRTTag)
{
	list<CTarget*>*	pTargetList = Find_MRT(pMRTTag);
	if (pTargetList == nullptr)
		return E_FAIL;

	_uint iIndex = 0;

	for (auto& pTarget : *pTargetList)
	{
		pTarget->Release_OnGraphic_Device_Shadow(iIndex++);
	}

	return S_OK;
}


HRESULT CTarget_Manager::Ready_DebugBuffer(const _tchar * pTargetTag, const _float & fStartX, const _float & fStartY, const _float & fSizeX, const _float & fSizeY)
{
	CTarget*	pTarget = Find_Target(pTargetTag);
	if (nullptr == pTarget)
		return E_FAIL;

	return pTarget->Ready_DebugBuffer(fStartX, fStartY, fSizeX, fSizeY);
}

void CTarget_Manager::Render_DebugBuffer(const _tchar * pMRTTag)
{
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return;

	for (auto& pTarget : *pMRTList)
	{
		if (nullptr != pTarget)
			pTarget->Render_DebugBuffer();
	}
}

HRESULT CTarget_Manager::SetUp_Texture_OnShader(LPD3DXEFFECT pEffect, const _tchar * pTargetTag, const _char * pConstantName)
{

	CTarget*	pTarget = Find_Target(pTargetTag);
	if (nullptr == pTarget)
		return E_FAIL;

	return pTarget->SetUp_Texture_OnShader(pEffect,pConstantName);
}

CTarget * CTarget_Manager::Find_Target(const _tchar * pTargetTag)
{
	auto iter = find_if(m_mapTarget.begin(), m_mapTarget.end(), CFinder_Tag(pTargetTag));
	if (iter == m_mapTarget.end())
		return nullptr;

	return iter->second;
}

list<CTarget*>* CTarget_Manager::Find_MRT(const _tchar * pMRTTag)
{
	auto iter = find_if(m_mapMRT.begin(), m_mapMRT.end(), CFinder_Tag(pMRTTag));
	if (iter == m_mapMRT.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_mapMRT)
	{
		for (auto& pTarget : Pair.second)
		{
			Safe_Release(pTarget);
		}
		Pair.second.clear();
	}
	m_mapMRT.clear();

	for (auto& Pair : m_mapTarget)
		Safe_Release(Pair.second);
	m_mapTarget.clear();
}
