#include "Navigation_Line.h"

_USING(Engine)

Engine::CNavigation_Line::CNavigation_Line(void)
{

}

Engine::CNavigation_Line::~CNavigation_Line(void)
{

}

HRESULT Engine::CNavigation_Line::Ready_Line(const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	_vec3 vCross;
	_vec3 vAtoB = *pPointB - *pPointA;
	_vec3 vAtoC = *pPointC - *pPointA;
	_vec3 vUp = _vec3(0.f, 1.f, 0.f);
//	D3DXVec3Cross(&vCross, &vAtoB, &vAtoC);

//	D3DXVec3Cross(&vCross, &vAtoB, &vCross);
	D3DXVec3Cross(&vCross, &vAtoB, &vUp);
	D3DXVec3Normalize(&vCross, &vCross);


	m_vPoint[POINT_START] = *pPointA;
	m_vPoint[POINT_FINISH] = *pPointB;
	

	//m_vDirection = m_vPoint[POINT_FINISH] - m_vPoint[POINT_START];
	//D3DXVec3Normalize(&vCross, &vCross);

	//2���� �븻���� ����
	//m_vNormal = _vec2(m_vDirection.y * -1.f, m_vDirection.x);
	//D3DXVec3Normalize(&vCross, &vCross);
	m_vNormal = vCross;

	return S_OK;
}

//�Ű������� ���� ���� ���� ������ ��
Engine::CNavigation_Line::COMPARE Engine::CNavigation_Line::Compare(const _vec3* pEndPos)
{
	_vec3		vDest = *pEndPos - m_vPoint[POINT_START];
	D3DXVec3Normalize(&vDest, &vDest);

	_float		fResult = D3DXVec3Dot(&vDest, &m_vNormal);

	//�����϶�
	if(fResult >= 0.f)
	{
		// �� ���ʿ� ������ �ִ�.
		return COMPARE_OUT;
	}
	//�а��϶�
	else
	{
		//�� �����ʿ� ���� �ִ�.
		return COMPARE_IN;
	}
}


CNavigation_Line* Engine::CNavigation_Line::Create(const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	CNavigation_Line* pInstance = new CNavigation_Line();

	if (FAILED(pInstance->Ready_Line(pPointA, pPointB, pPointC)))
		Safe_Release(pInstance);

	return pInstance;
}


void Engine::CNavigation_Line::Free(void)
{
	
}
