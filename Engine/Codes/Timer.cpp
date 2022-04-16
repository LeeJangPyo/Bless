#include "..\Headers\Timer.h"

_USING(Engine)


CTimer::CTimer()
{
}

HRESULT CTimer::Ready_Timer(void)
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);

	return NOERROR;
}

_float CTimer::Get_TimeDelta()
{
	QueryPerformanceCounter(&m_FrameTime);	

	m_fTimeDelta = float(m_FrameTime.QuadPart - m_LastTime.QuadPart) / m_CpuTick.QuadPart;

	m_LastTime = m_FrameTime;
	
	if(m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	return m_fTimeDelta;
}



CTimer * CTimer::Create(void)
{
	CTimer*	pInstance = new CTimer;

	if(FAILED(pInstance->Ready_Timer()))
	{
		_MSG_BOX(L"CTimer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimer::Free(void)
{

}
