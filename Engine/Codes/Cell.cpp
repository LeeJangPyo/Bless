#include "Cell.h"

#include "Management.h"


#include "Collider_Sphere.h"
#include "Navigation_Line.h"
#include "Shader.h"


Engine::CCell::CCell(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
	, m_pTexture(nullptr)
	, m_bIsCellPick(false)
	, m_pFont_Manager(CFont_Manager::GetInstance())

{
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * NEIGHBOR_END);
	m_pDevice->AddRef();
	m_pFont_Manager->AddRef();

	for (_ulong i = 0; i < POINT_END; ++i)
	{
		m_bIsPointPick[i] = false;
		m_pColliderCom[i] = nullptr;
	}
}



HRESULT Engine::CCell::Ready_Cell(const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	m_dwIndex = dwIndex;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

// 	D3DXMatrixTranslation(&m_matPoint[POINT_A], pPointA->x, pPointA->y, pPointA->z);
// 	D3DXMatrixTranslation(&m_matPoint[POINT_B], pPointB->x, pPointB->y, pPointB->z);
// 	D3DXMatrixTranslation(&m_matPoint[POINT_C], pPointC->x, pPointC->y, pPointC->z);


	//For. Deug
	m_vCenterPoint = (m_vPoint[POINT_A] + m_vPoint[POINT_B] + m_vPoint[POINT_C]) / 3.f;
	_ultow_s(m_dwIndex, m_szIndex, 10);
		
		
	//Collider Set	
	m_pLine[LINE_AB] = CNavigation_Line::Create(&m_vPoint[POINT_A],
		&m_vPoint[POINT_B], &m_vPoint[POINT_C]);

	m_pLine[LINE_BC] = CNavigation_Line::Create(&m_vPoint[POINT_B],
		&m_vPoint[POINT_C], &m_vPoint[POINT_A]);

	m_pLine[LINE_CA] = CNavigation_Line::Create(&m_vPoint[POINT_C],
		&m_vPoint[POINT_A], &m_vPoint[POINT_B]);

	//Ready Line;
	CLine_Manager::GetInstance()->Ready_Line(m_pDevice, L"Mesh_Navigation",3.f);
	//Ready Font;
	m_pFont_Manager->Ready_Font(m_pDevice, L"Font_Cell_Index",
														L"바탕",
														20,
														FW_HEAVY,
														20);


	
	if(FAILED(Ready_VertexBuffer()))
		return E_FAIL;

	//Texture COmpoentn Create
	m_pTexture = CTexture::Create(m_pDevice);
	m_pShaderCom = dynamic_cast<CShader*>(CManagement::GetInstance()->Clone_Component(0, L"Component_Shader_Default"));//CShader::Create(m_pDevice, L"../Bin/ShaShader_Deafult.fx");
	if (m_pShaderCom == nullptr)
	{
		return E_FAIL;
	}

	for (_uint i = 0; i < POINT_END; ++i)
	{
		m_pColliderCom[i] = CCollider_Sphere::Create(m_pDevice);
		if (m_pColliderCom[i] == nullptr)
		{
			return E_FAIL;
		}
		m_pColliderCom[i]->SetUp_Collider_Spehre(&m_vPoint[i], 0.5f);
	}


	return S_OK;
}

_bool Engine::CCell::Compare_Point(const _vec3* pPointA, const _vec3* pPointB, CCell* pCell)
{
	//현재 가지고 있는 포인트와 매개변수 포인터 비교
	if(m_vPoint[POINT_A] == *pPointA)
	{
		if (m_vPoint[POINT_B] == *pPointB)
		{
			//들어온 셀은 
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}

		if (m_vPoint[POINT_C] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_B] == *pPointA)
	{
		if (m_vPoint[POINT_A] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}

		if (m_vPoint[POINT_C] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_C] == *pPointA)
	{
		if (m_vPoint[POINT_A] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}

		if (m_vPoint[POINT_B] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	return false;
}
_bool Engine::CCell::Compare_PointAndDelete(const _vec3* pPointA, const _vec3* pPointB)
{
	//현재 가지고 있는 포인트와 매개변수 포인터 비교
	if (m_vPoint[POINT_A] == *pPointA)
	{
		if (m_vPoint[POINT_B] == *pPointB)
		{
			//들어온 셀은 
			m_pNeighbor[NEIGHBOR_AB] = nullptr;
			return true;
		}

		if (m_vPoint[POINT_C] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_CA] = nullptr;
			return true;
		}
	}

	if (m_vPoint[POINT_B] == *pPointA)
	{
		if (m_vPoint[POINT_A] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_AB] = nullptr;
			return true;
		}

		if (m_vPoint[POINT_C] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_BC] = nullptr;
			return true;
		}
	}

	if (m_vPoint[POINT_C] == *pPointA)
	{
		if (m_vPoint[POINT_A] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_CA] = nullptr;
			return true;
		}

		if (m_vPoint[POINT_B] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_BC] = nullptr;
			return true;
		}
	}

	return false;
}

Engine::CCell::COMPARE Engine::CCell::Compare(_vec3* pEndPos,const _vec3* pDir, _ulong* pCellIndex, _vec3* pSlideDir)
{
	for(_ulong i = 0; i < LINE_END; ++i)
	{
		//이웃정보 적용
		if(CNavigation_Line::COMPARE_OUT == m_pLine[i]->Compare(pEndPos))
		{
			//이웃정보가 없으면 슬라이드
			if (nullptr == m_pNeighbor[i])
			{				
				//포인트 구함
				_vec3 vNormal = m_pLine[i]->Get_Normal();

				_float fDotResult = D3DXVec3Dot(&vNormal, pDir);

				_vec3 vProjection = vNormal * fDotResult;
				_vec3 vResultDir = *pDir - vProjection;

				*pSlideDir = vResultDir;

				//셀 이동시 예외 처리 필요
				return CCell::COMPARE_SLIDE;
				//return CCell::COMPARE_UNMOVE;
			}
			else
			{
				//셀정보의 보간값으로 위치 표현
				*pCellIndex = *m_pNeighbor[i]->Get_Index();
				return CCell::COMPARE_MOVE;
			}
		}
	}
	//셀 정보의 보간값으로 위치 표현

	_float fU, fV, fDist;
	_vec3 vTempPos = *pEndPos;
	vTempPos.y += 100.f;
	_vec3 vPoint[POINT_END] = { m_vPoint[POINT_A] ,m_vPoint[POINT_B] ,m_vPoint[POINT_C] };


	if(D3DXIntersectTri(&vPoint[POINT_A], &vPoint[POINT_B], &vPoint[POINT_C], &vTempPos, &_vec3(0.f, -1.f, 0.f), &fU, &fV, &fDist))
	{
		_vec3 vPos = m_vPoint[POINT_A] + (m_vPoint[POINT_B] - m_vPoint[POINT_A]) * fU + (m_vPoint[POINT_C] - m_vPoint[POINT_A]) * fV;

		pEndPos->y = vPos.y;

		return CCell::COMPARE_MOVE;
	}
	else
	{
		return CCell::COMPARE_UNMOVE;
	}
	
	
}

_bool CCell::Compare(const _vec3 * pPosition, _ulong& dwIndex)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		//이웃정보 적용
		if (CNavigation_Line::COMPARE_OUT == m_pLine[i]->Compare(pPosition))
		{
			return false;
		}
	}
	dwIndex = m_dwIndex;
	return true;
}

_vec3 CCell::Compare(const _vec3 * pPosition)
{
	_vec3 vOut = *pPosition;
	_float fU, fV, fDist;
	_vec3 vPoint[POINT_END] = { m_vPoint[POINT_A] ,m_vPoint[POINT_B] ,m_vPoint[POINT_C] };

	//for (_uint i = 0; i < POINT_END; ++i)
	//	vPoint[i].y = 0.f;

	vOut.y += 100.f;

	if(D3DXIntersectTri(&vPoint[POINT_A], &vPoint[POINT_B], &vPoint[POINT_C], &vOut, &_vec3(0.f, -1.f, 0.f), &fU, &fV, &fDist))
	{
		_vec3 vPos = m_vPoint[POINT_A] + (m_vPoint[POINT_B] - m_vPoint[POINT_A]) * fU + (m_vPoint[POINT_C] - m_vPoint[POINT_A]) * fV;

		return vPos;
	}
	else
	{
		return *pPosition;
	}
		
}

void Engine::CCell::Render_Cell(void)
{
	//_vec3		vPoint[4];

	//vPoint[0] = m_vPoint[POINT_A];
	//vPoint[1] = m_vPoint[POINT_B];
	//vPoint[2] = m_vPoint[POINT_C];
	//vPoint[3] = m_vPoint[POINT_A];

	////Draw Line
	//CLine_Manager::GetInstance()->Render_Line(L"Mesh_Navigation", vPoint, 4, D3DXCOLOR(1.0f, 0.f, 0.f, 1.f));

	//Draw Font for Index;
	
	m_pFont_Manager->Render_Font(L"Font_Cell_Index", m_szIndex, &m_vCenterPoint, D3DXCOLOR(0.8f,0.8f, 0.8f, 1.f));

	//Draw Cell
	LPD3DXEFFECT pEffect  = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);


	//Draw Surface
	if(m_bIsCellPick)
	{
		SetUp_ConstantTable(pEffect, COL_GREEN);
	}
	else
	{
		SetUp_ConstantTable(pEffect, COL_RED);
	}
	pEffect->CommitChanges();
	Render_Surface();

	//Draw Sphere
	for (_ulong i = 0; i < POINT_END; ++i)
	{

		if (m_bIsPointPick[i])
		{

			m_pColliderCom[i]->SetUp_ConstantTable(pEffect, CCollider_Sphere::COLOR_GREEN);
		}
		else
		{
			m_pColliderCom[i]->SetUp_ConstantTable(pEffect, CCollider_Sphere::COLOR_RED);
		}
		pEffect->CommitChanges();
		m_pColliderCom[i]->Render_Collider_Sphere();
	}

	pEffect->EndPass();
	pEffect->End();
}

void Engine::CCell::Update_VB()
{
	VTXTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[0].vPos = m_vPoint[POINT_A];
	pVtxTex[0].vNormal = D3DXVECTOR3(0.f, 1.f, 0.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 0.f);

	pVtxTex[1].vPos = m_vPoint[POINT_B];
	pVtxTex[1].vNormal = D3DXVECTOR3(0.f, 1.f, 0.f);
	pVtxTex[1].vTex = D3DXVECTOR2(1.f, 0.f);

	pVtxTex[2].vPos = m_vPoint[POINT_C];
	pVtxTex[2].vNormal = D3DXVECTOR3(0.f, 1.f, 0.f);
	pVtxTex[2].vTex = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();

	//////////////////////////////////////////////////////////////////////////
	//	Update Transform
// 	D3DXMatrixTranslation(&m_matPoint[POINT_A], m_vPoint[POINT_A].x, m_vPoint[POINT_A].y, m_vPoint[POINT_A].z);
// 	D3DXMatrixTranslation(&m_matPoint[POINT_B], m_vPoint[POINT_B].x, m_vPoint[POINT_B].y, m_vPoint[POINT_B].z);
// 	D3DXMatrixTranslation(&m_matPoint[POINT_C], m_vPoint[POINT_C].x, m_vPoint[POINT_C].y, m_vPoint[POINT_C].z);

	m_pColliderCom[POINT_A]->Set_Center_Position(&m_vPoint[POINT_A]);
	m_pColliderCom[POINT_B]->Set_Center_Position(&m_vPoint[POINT_B]);
	m_pColliderCom[POINT_C]->Set_Center_Position(&m_vPoint[POINT_C]);

}

HRESULT CCell::Ready_VertexBuffer(void)
{
	//버퍼 생성
	if (FAILED(m_pDevice->CreateVertexBuffer(
		sizeof(VTXTEX) * 3,		// 정점 버퍼의 사이즈(바이트 단위)
		0,								// 정점 사용방식 : 버퍼를 만들 때 어떤 버퍼를 만들 것인가?(0인 경우 정적 버퍼)
		VTXFVF_TEX,						// 사용하는 버퍼 스타일
		D3DPOOL_MANAGED,				// 메모리 풀의 사용 방식
		&m_pVB,							// 버텍스 버퍼의 접근권한을 갖고 있는 객체 주소값
		NULL)))							// 버퍼 만들 때 예약상태
	{
		_TAGMSG_BOX(L"VertexBuffer Creation Failed, in Ready_VertexBuffer", L"CCell");
		return E_FAIL;
	}

	VTXTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[0].vPos = m_vPoint[POINT_A];
	pVtxTex[0].vNormal = D3DXVECTOR3(0.f, 1.f, 0.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 0.f);

	pVtxTex[1].vPos = m_vPoint[POINT_B];
	pVtxTex[1].vNormal = D3DXVECTOR3(0.f, 1.f, 0.f);
	pVtxTex[1].vTex = D3DXVECTOR2(1.f, 0.f);

	pVtxTex[2].vPos = m_vPoint[POINT_C];
	pVtxTex[2].vNormal = D3DXVECTOR3(0.f, 1.f, 0.f);
	pVtxTex[2].vTex = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();

	return S_OK;
}

void CCell::Render_Surface(void)
{
	if (m_bIsCellPick)
		m_pTexture->Set_Texture(SURFACECOL::COL_GREEN);
	else
		m_pTexture->Set_Texture(SURFACECOL::COL_YELLOW);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXTEX));
	m_pDevice->SetFVF(VTXFVF_TEX);
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}

HRESULT CCell::SetUp_ConstantTable(LPD3DXEFFECT pEffect, SURFACECOL eType)
{
	if (pEffect == nullptr)
		return E_FAIL;
	pEffect->AddRef();

	_matrix matWorld,matView, matProj;
	D3DXMatrixIdentity(&matWorld);

	pEffect->SetMatrix("g_matWorld",&matWorld);
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTexture->SetUp_OnShader(pEffect,"g_BaseTexture", eType);

	Safe_Release(pEffect);

	return S_OK;
}

Engine::CCell* Engine::CCell::Create(LPDIRECT3DDEVICE9 pDevice, const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	CCell*		pInstance = new CCell(pDevice);

	if (FAILED(pInstance->Ready_Cell(dwIndex, pPointA, pPointB, pPointC)))
	{
		_MSG_BOX(L"CCell Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CCell* Engine::CCell::Create(LPDIRECT3DDEVICE9 pDevice)
{
	return new CCell(pDevice);
}

void Engine::CCell::Free(void)
{
	for (_ulong i = 0; i < LINE_END; ++i)
		Safe_Release(m_pLine[i]);

	for (_ulong i = 0; i < POINT_END; ++i)
		Safe_Release(m_pColliderCom[i]);

	Safe_Release(m_pFont_Manager);

	Safe_Release(m_pTexture);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVB);
	Safe_Release(m_pDevice);
	
}



