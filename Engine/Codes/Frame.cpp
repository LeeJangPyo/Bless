#include "..\Headers\Frame.h"

CFrame::CFrame()
{
}

HRESULT CFrame::Ready_Frame(const _float & fCallPerSec)
{
	m_fInterval = 1.f / fCallPerSec;

	return NOERROR;
}

bool CFrame::Permit_Call(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if(m_fInterval <= m_fTimeAcc)
	{
		m_fTimeAcc = 0.f;
		return true;
	}

	return false;
}

CFrame * CFrame::Create(const _float & fCallPerSec)
{
	CFrame*	pInstance = new CFrame;

	if(FAILED(pInstance->Ready_Frame(fCallPerSec)))
	{
		_MSG_BOX(L"CFrame Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CFrame::Free(void)
{

}

