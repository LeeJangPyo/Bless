#include "Line.h"



Engine::CLine::CLine(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

Engine::CLine::~CLine(void)
{

}

HRESULT Engine::CLine::Ready_Line(const _float& fWidth, const _bool& bAntiAlias, const _bool& bGLLines, const _long& dwPattern, const _float& fPatScale)
{
	if(FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
	{
		_TAGMSG_BOX(L"Line Com Object Create Failed, in Ready_Line", L"CLine");
		return E_FAIL;
	}

	//m_pLine->SetAntialias(bAntiAlias);		//Antialias설정
	//m_pLine->SetGLLines(bGLLines);			//OpenGL 스타일로 선을 그릴지 결정TRUE : OpenGL 스타일
	//m_pLine->SetPattern(1);			//점 패턴 적용
	//m_pLine->SetPatternScale(0.5f);	//패턴 길이 늘리거나 줄이기
	m_pLine->SetWidth(fWidth);				//선 굵기 설정


	return S_OK;
}

void Engine::CLine::Render_Line(_vec3* pVertexArray,
							const _long& dwArrayCount,
							const D3DXCOLOR& Color)
{
	for (_long i = 0; i < dwArrayCount; ++i)
		m_pBuffer[i] = pVertexArray[i];


	_matrix		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_long i = 0; i < dwArrayCount; ++i)
	{
		D3DXVec3TransformCoord(&m_pBuffer[i], &m_pBuffer[i], &matView);
		if (m_pBuffer[i].z <= 0.1f)
			m_pBuffer[i].z = 0.1f;

		D3DXVec3TransformCoord(&m_pBuffer[i], &m_pBuffer[i], &matProj);
	}

	m_pDevice->EndScene();
	m_pDevice->BeginScene();

	m_pLine->Begin();

	_matrix		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	m_pLine->DrawTransform(m_pBuffer, dwArrayCount, &matIdentity, Color);

	m_pLine->End();
}

void Engine::CLine::Free(void)
{
	Safe_Release(m_pLine);
	Safe_Release(m_pDevice);
}

CLine* Engine::CLine::Create(LPDIRECT3DDEVICE9 pDevice, 
	const _float& fWidth /*= 1.f*/, 
	const _bool& bAntiAlias /*= true*/, 
	const _bool& bGLLines /*= false*/, 
	const _long& dwPattern /*= 0*/, 
	const _float& fPatScale /*= 1.f*/)
{
	CLine*	pInstance = new CLine(pDevice);

	if (FAILED(pInstance->Ready_Line(fWidth, bAntiAlias, bGLLines, dwPattern, fPatScale)))
	{
		_MSG_BOX(L"CLine Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

