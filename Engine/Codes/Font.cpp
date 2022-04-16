#include "Font.h"


Engine::CFont::CFont(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
{
	m_pDevice->AddRef();

}

Engine::CFont::~CFont(void)
{

}

HRESULT Engine::CFont::Ready_Font(const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	D3DXFONT_DESC		Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.CharSet = HANGUL_CHARSET;
	lstrcpy(Font_Desc.FaceName, pFontType);
	Font_Desc.Weight = iWeight;
	Font_Desc.Height = iHeight;
	Font_Desc.Width = iWidth;

	if(FAILED(D3DXCreateFontIndirect(m_pDevice, &Font_Desc, &m_pFont)))
	{
		_TAGMSG_BOX(L"Font Com Object Create Failed, in Ready Font", L"CFnot");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		_TAGMSG_BOX(L"Sprite Com Object Create Failed, in Ready Font", L"CFnot");
		return E_FAIL;
	}

	return S_OK;
}




void Engine::CFont::Render_Font(const _tchar* pString, const _vec2* pPosition, D3DXCOLOR Color)
{
	RECT		rc = { _long(pPosition->x), _long(pPosition->y) };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);

	m_pSprite->End();
}

void Engine::CFont::Render_Font(const _tchar* pString, const _vec3* pPosition, D3DXCOLOR Color)
{
	_vec3			vCamZ;			// Camera Z Axis
	_vec3			vEye;			// Camera Position

	_matrix			matView;
	_matrix			matProj;
	_matrix			matInverseView;

	// 뷰포트 행렬
	_matrix			matViewport;
	D3DVIEWPORT9    vp;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pDevice->GetViewport(&vp);


	D3DXMatrixInverse(&matInverseView, NULL, &matView);

	vCamZ = _vec3(matInverseView._31, matInverseView._32, matInverseView._33);
	vEye = _vec3(matInverseView._41, matInverseView._42, matInverseView._43);

	// 뷰포트 행렬 설정
	Make_ViewportMatrix(&matViewport, &vp);

	// 변환 행렬 = 월드 행렬 * 뷰행렬 * 투영 행렬 * 뷰포트 행렬
	_matrix  mtTMpt = matView * matProj * matViewport;


	_vec3	vOut;
	_vec3	vIn = *pPosition;
	_float	fZ;

	

	fZ = D3DXVec3Dot(&vCamZ, &(vIn - vEye));

	if (fZ <= 0.f)
		return;

	D3DXVec3TransformCoord(&vOut, &vIn, &mtTMpt);

	
	RECT rc = { _int(vOut.x), _int(vOut.y), 0,0 };

	rc.right = rc.left + 100;
	rc.bottom = rc.top + 40;

//	cout << "left : " << rc.left << "// right : " << rc.right << endl;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);

	m_pSprite->End();

}

void Engine::CFont::Make_ViewportMatrix(_matrix* pOut, const D3DVIEWPORT9* pViewport)
{
	_float   fW = 0;
	_float   fH = 0;
	_float   fD = 0;
	_float   fY = 0;
	_float   fX = 0;

	_float   fM = _float(pViewport->MinZ);

	fW = _float(pViewport->Width)*.5f;
	fH = _float(pViewport->Height)*.5f;
	fD = _float(pViewport->MaxZ) - _float(pViewport->MinZ);
	fX = _float(pViewport->X) + fW;
	fY = _float(pViewport->Y) + fH;

	*pOut = _matrix(fW, 0.f, 0, 0,
		0.f, -fH, 0, 0,
		0.f, 0.f, fD, 0,
		fX, fY, fM, 1);
}


Engine::CFont* Engine::CFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CFont*	pInstance = new CFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CFont::Free(void)
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
}

