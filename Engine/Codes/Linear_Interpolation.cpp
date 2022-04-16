#include "..\Headers\Linear_Interpolation.h"


CLinear_Interpolation::CLinear_Interpolation()
	:m_vValue(_vec3(0.f, 0.f, 0.f))
	, m_vStep(_vec3(0.f, 0.f, 0.f))
	, m_fRemainingTime(0.f)
{
}

bool CLinear_Interpolation::Setup(const _vec3 * vStartPoint, const _vec3 * vEndPoint, _float fTime)
{
	if (fTime < 0)
	{
		return false;
	}
	m_fRemainingTime = fTime;
	m_vValue = *vStartPoint;
	m_vStep = (*vEndPoint - *vStartPoint) / fTime;

	return true;
}

bool CLinear_Interpolation::Interpolate(_float deltaTime)
{
	m_fRemainingTime -= deltaTime;
	m_vValue += m_vStep * deltaTime;
	return (m_fRemainingTime <= 0);
}

CLinear_Interpolation * CLinear_Interpolation::Create()
{
	CLinear_Interpolation*		pInstance = new CLinear_Interpolation();

	return pInstance;
}

void CLinear_Interpolation::Free()
{
}
