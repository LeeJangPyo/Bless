#include "stdafx.h"
#include "Skeleton_Wait.h"

#include "Skeleton.h"

_USING(Client)

CSkeleton_Wait::CSkeleton_Wait()
{
}

HRESULT CSkeleton_Wait::Begin()
{
	m_pSkeleton->Set_AnimationSet(14);

	return S_OK;
}

_int CSkeleton_Wait::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CSkeleton_Wait::LastUpdate(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CSkeleton_Wait::End()
{
	return S_OK;
}

HRESULT CSkeleton_Wait::Ready_State(CSkeleton * pSkeleton)
{
	m_pSkeleton  = pSkeleton;
	return S_OK;;
}

CSkeleton_Wait * CSkeleton_Wait::Create(CSkeleton* pSkeleton)
{
	CSkeleton_Wait*		pInstance = new CSkeleton_Wait();

	if (FAILED(pInstance->Ready_State(pSkeleton)))
	{
		_MSG_BOX(L"CSkeleton_Wait Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeleton_Wait::Free()
{
}


