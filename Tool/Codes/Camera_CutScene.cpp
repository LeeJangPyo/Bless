#include "stdafx.h"
#include "Camera_CutScene.h"

#include "Management.h"

#include "CameraTab.h"

_USING(Client)

CCamera_CutScene::CCamera_CutScene(LPDIRECT3DDEVICE9 pDevice)
	: CCamera(pDevice)
	, m_pShaderCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pLine_Manager(CLine_Manager::GetInstance())
	, m_pCalculatorCom(nullptr)
	, m_bIsActivate(false)
	, m_pCameraTab(nullptr)
{
	m_pLine_Manager->AddRef();
}

CCamera_CutScene::CCamera_CutScene(const CCamera_CutScene & rhs)
	: CCamera(rhs)
	, m_pLine_Manager(rhs.m_pLine_Manager)
	, m_wstrKey(rhs.m_wstrKey)
	, m_bIsActivate(false)
	, m_pCameraTab(nullptr)
{
	m_pLine_Manager->AddRef();
}

void CCamera_CutScene::Set_PickEye(const _uint iIndex, _bool bIsPick)
{
	m_vecCamera_Eye[iIndex]->Set_IsPick(bIsPick);
}

void CCamera_CutScene::Set_PickAt(const _uint iIndex, _bool bIsPick)
{
	m_vecCamera_At[iIndex]->Set_IsPick(bIsPick);
}


HRESULT Client::CCamera_CutScene::Ready_Prototype()
{
	if (FAILED(CCamera::Ready_Prototype()))
		return E_FAIL;

	PROJDESC		ProjectionDesc;
	ZeroMemory(&ProjectionDesc, sizeof(PROJDESC));

	ProjectionDesc.fFovy = D3DXToRadian(60.0f);
	ProjectionDesc.fAspect = _float(g_nBackCX) / g_nBackCY;
	ProjectionDesc.fNear = 0.1f;
	ProjectionDesc.fFar = 1000.0f;

	Set_ProjectionDesc(ProjectionDesc);

	return S_OK;
}

HRESULT Client::CCamera_CutScene::Ready_GameObject()
{

	if (FAILED(CCamera::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fSpeed = 0.25f;
	m_fTime = 0.f;
	m_iSection = 0;

	Add_Point();

	return S_OK;
}

Engine::_int Client::CCamera_CutScene::Update_GameObject(const _float& fTimeDelta)
{
	for(auto& camera_Eye : m_vecCamera_Eye)
	{
		camera_Eye->Update(fTimeDelta);
	}
	for (auto& camera_At : m_vecCamera_At)
	{
		camera_At->Update(fTimeDelta);
	}

	if (!m_bIsActivate)
		return 0;

	// SectionSize
	_int iSectionSize = (_int)m_vecCamera_Eye.size() - 1;

	 
	m_fTime += fTimeDelta * m_fSpeed;
	if (m_fTime > 1.f)
	{
		m_fTime = 0.f;

		//Goto Next Section
		m_iSection++;
		//SectionInfo Update
		m_Camera_Desc.vEye = *m_vecCamera_Eye[m_iSection]->Get_Position();
		m_Camera_Desc.vAt = *m_vecCamera_At[m_iSection]->Get_Position();
		m_Camera_Desc.vUp = _vec3(0.f, 1.f, 0.f);

		if (m_iSection >= iSectionSize)
			m_iSection = 0;
	}

	D3DXVec3CatmullRom(&m_Camera_Desc.vEye ,
		m_vecCamera_Eye[m_iSection - 1 < 0 ? 0 : m_iSection - 1]->Get_Position(),
		m_vecCamera_Eye[m_iSection + 0]->Get_Position(),
		m_vecCamera_Eye[m_iSection + 1 > iSectionSize ? iSectionSize : m_iSection + 1]->Get_Position(),
		m_vecCamera_Eye[m_iSection + 2 > iSectionSize ? iSectionSize : m_iSection + 2]->Get_Position(), m_fTime);

	D3DXVec3CatmullRom(&m_Camera_Desc.vAt,
		m_vecCamera_At[m_iSection - 1 < 0 ? 0 : m_iSection - 1]->Get_Position(),
		m_vecCamera_At[m_iSection + 0]->Get_Position(),
		m_vecCamera_At[m_iSection + 1 > iSectionSize ? iSectionSize : m_iSection + 1]->Get_Position(),
		m_vecCamera_At[m_iSection + 2 > iSectionSize ? iSectionSize : m_iSection + 2]->Get_Position(), m_fTime);


	return CCamera::Update_GameObject(fTimeDelta);

}

Engine::_int Client::CCamera_CutScene::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	Caculate_Line();

	return 0;
}

void Client::CCamera_CutScene::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	for (auto& pEye : m_vecCamera_Eye)
	{
		pEye->SetUp_ConstantTable(pEffect);
		pEffect->CommitChanges();
		pEye->Render();
	}

	for (auto& pAt : m_vecCamera_At)
	{
		pAt->SetUp_ConstantTable(pEffect);
		pEffect->CommitChanges();
		pAt->Render();
	}

	pEffect->EndPass();
	pEffect->End();

	for (auto pLine : m_vecLine_Eye)
		m_pLine_Manager->Render_Line(L"Axis", pLine, 10, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	for (auto pLine : m_vecLine_At)
		m_pLine_Manager->Render_Line(L"Axis", pLine, 10, D3DXCOLOR(0.f, 1.f, 1.f, 1.f));
}

HRESULT CCamera_CutScene::Save_CutScene(const _tchar* pFileName)
{
	HANDLE		hFile = CreateFile(pFileName
		, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	DWORD	dwByte = 0;

	//Svae Tag
	_int iwstrLength = m_wstrKey.length()+1;
	WriteFile(hFile, &iwstrLength, sizeof(_int), &dwByte, nullptr);
	WriteFile(hFile, m_wstrKey.c_str(), sizeof(_tchar)*iwstrLength, &dwByte, nullptr);

	//Save Speed;
	WriteFile(hFile, &m_fSpeed, sizeof(_float), &dwByte, nullptr);

	//Svae Szie
	_int iSize = m_vecCamera_Eye.size();
	WriteFile(hFile, &iSize, sizeof(_int), &dwByte, nullptr);


	//Save CameraEye
	auto& iter_Eye = m_vecCamera_Eye.begin();
	auto& iter_At = m_vecCamera_At.begin();

	for(;iter_Eye != m_vecCamera_Eye.end(); ++iter_Eye, ++iter_At)
	{
		WriteFile(hFile, (*iter_Eye)->Get_Position(), sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, (*iter_At)->Get_Position(), sizeof(_vec3), &dwByte, nullptr);
	}
	CloseHandle(hFile);

	return S_OK;
}


void CCamera_CutScene::Add_Point()
{
	_vec3 vPosition = PickUp_OnTerrain();

	if (vPosition == _vec3())
		return;

	CCamera_Eye* pEye = CCamera_Eye::Create(m_pDevice, vPosition);
	m_vecCamera_Eye.push_back(pEye);

	vPosition.z += 10.f;
	CCamera_At* pAt = CCamera_At::Create(m_pDevice, vPosition);
	m_vecCamera_At.push_back(pAt);
	
	Caculate_Line();

	m_Camera_Desc.vEye = *m_vecCamera_Eye[0]->Get_Position();
	m_Camera_Desc.vAt = *m_vecCamera_At[0]->Get_Position();
	m_Camera_Desc.vUp = _vec3(0.f, 1.f, 0.f);

}

void CCamera_CutScene::Add_Eye(const _vec3 & vPosition)
{
	CCamera_Eye* pEye = CCamera_Eye::Create(m_pDevice, vPosition);
	m_vecCamera_Eye.push_back(pEye);
}

void CCamera_CutScene::Add_At(const _vec3 & vPosition)
{
	CCamera_At* pAt = CCamera_At::Create(m_pDevice, vPosition);
	m_vecCamera_At.push_back(pAt);
}

void CCamera_CutScene::Caculate_Line(void)
{
	_int iSectionSize = m_vecCamera_Eye.size() - 1;

	if (iSectionSize == 0)
		return;

	//점이 추가되면 계산을 다시해야함 정보 삭제
	for (auto vPointArray : m_vecLine_Eye)
		Safe_Delete_Array(vPointArray);
	m_vecLine_Eye.clear();

	//점 2개
	if(iSectionSize == 1)
	{
		_vec3* vLinePoint = new _vec3[10];

		for (_float i = 0.f; i < 1.f; i = i += 0.01f)
		{
			D3DXVec3CatmullRom(&vLinePoint[(_int)(i * 10)],
				m_vecCamera_Eye[0]->Get_Position(),
				m_vecCamera_Eye[0]->Get_Position(),
				m_vecCamera_Eye[1]->Get_Position(),
				m_vecCamera_Eye[1]->Get_Position(), i);
		}
		m_vecLine_Eye.push_back(vLinePoint);
	}
	//점 3개
	else if(iSectionSize == 2)
	{
		_vec3* vLinePoint = new _vec3[10];

		for (_float i = 0.f; i < 1.f; i = i += 0.01f)
		{
			D3DXVec3CatmullRom(&vLinePoint[(_int)(i * 10)],
				m_vecCamera_Eye[0]->Get_Position(),
				m_vecCamera_Eye[0]->Get_Position(),
				m_vecCamera_Eye[1]->Get_Position(),
				m_vecCamera_Eye[2]->Get_Position(), i);
		}
		m_vecLine_Eye.push_back(vLinePoint);
		
		vLinePoint = new _vec3[10];

		for (_float i = 0.f; i < 1.f; i = i += 0.01f)
		{
			D3DXVec3CatmullRom(&vLinePoint[(_int)(i * 10)],
				m_vecCamera_Eye[0]->Get_Position(),
				m_vecCamera_Eye[1]->Get_Position(),
				m_vecCamera_Eye[2]->Get_Position(),
				m_vecCamera_Eye[2]->Get_Position(), i);
		}

		m_vecLine_Eye.push_back(vLinePoint);
	}
	//점 4개 이상
	else if(iSectionSize >= 3)
	{
		for (_int i = 0; i < iSectionSize; ++i)
		{
			_vec3* vLinePoint = new _vec3[10];

			for (_float j = 0.f; j < 1.f; j = j += 0.01f)
			{
				D3DXVec3CatmullRom(&vLinePoint[(_int)(j * 10)],
					m_vecCamera_Eye[i - 1 < 0 ? 0 : i - 1]->Get_Position(),
					m_vecCamera_Eye[i + 0]->Get_Position(),
					m_vecCamera_Eye[i + 1 > iSectionSize ? iSectionSize : i + 1]->Get_Position(),
					m_vecCamera_Eye[i + 2 > iSectionSize ? iSectionSize : i + 2]->Get_Position(), j);
			}

			m_vecLine_Eye.push_back(vLinePoint);
		}
	}


	//At
	//점이 추가되면 계산을 다시해야함 정보 삭제
	for (auto vPointArray : m_vecLine_At)
		Safe_Delete_Array(vPointArray);
	m_vecLine_At.clear();

	//점 2개
	if (iSectionSize == 1)
	{
		_vec3* vLinePoint = new _vec3[10];

		for (_float i = 0.f; i < 1.f; i = i += 0.01f)
		{
			D3DXVec3CatmullRom(&vLinePoint[(_int)(i * 10)],
				m_vecCamera_At[0]->Get_Position(),
				m_vecCamera_At[0]->Get_Position(),
				m_vecCamera_At[1]->Get_Position(),
				m_vecCamera_At[1]->Get_Position(), i);
		}
		m_vecLine_At.push_back(vLinePoint);
	}
	//점 3개
	else if (iSectionSize == 2)
	{
		_vec3* vLinePoint = new _vec3[10];

		for (_float i = 0.f; i < 1.f; i = i += 0.01f)
		{
			D3DXVec3CatmullRom(&vLinePoint[(_int)(i * 10)],
				m_vecCamera_At[0]->Get_Position(),
				m_vecCamera_At[0]->Get_Position(),
				m_vecCamera_At[1]->Get_Position(),
				m_vecCamera_At[2]->Get_Position(), i);
		}
		m_vecLine_At.push_back(vLinePoint);

		vLinePoint = new _vec3[10];

		for (_float i = 0.f; i < 1.f; i = i += 0.01f)
		{
			D3DXVec3CatmullRom(&vLinePoint[(_int)(i * 10)],
				m_vecCamera_At[0]->Get_Position(),
				m_vecCamera_At[1]->Get_Position(),
				m_vecCamera_At[2]->Get_Position(),
				m_vecCamera_At[2]->Get_Position(), i);
		}

		m_vecLine_At.push_back(vLinePoint);
	}
	//점 4개 이상
	else if (iSectionSize >= 3)
	{
		for (_int i = 0; i < iSectionSize; ++i)
		{
			_vec3* vLinePoint = new _vec3[10];

			for (_float j = 0.f; j < 1.f; j = j += 0.01f)
			{
				D3DXVec3CatmullRom(&vLinePoint[(_int)(j * 10)],
					m_vecCamera_At[i - 1 < 0 ? 0 : i - 1]->Get_Position(),
					m_vecCamera_At[i + 0]->Get_Position(),
					m_vecCamera_At[i + 1 > iSectionSize ? iSectionSize : i + 1]->Get_Position(),
					m_vecCamera_At[i + 2 > iSectionSize ? iSectionSize : i + 2]->Get_Position(), j);
			}

			m_vecLine_At.push_back(vLinePoint);
		}
	}
}

void CCamera_CutScene::Set_CameraTab(CCameraTab * pCameraTab)
{
	m_pCameraTab = pCameraTab;


	for (auto& pEye : m_vecCamera_Eye)
	{
		pEye->Set_CameraTab(pCameraTab);
	}

	for (auto& pAt : m_vecCamera_At)
	{
		pAt->Set_CameraTab(pCameraTab);

	}

}

HRESULT CCamera_CutScene::Ready_Component(void)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Renderer;
	m_pRendererCom = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	//For. Calculator
	m_pCalculatorCom = dynamic_cast<CCalculator*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Calculator"));
	if (FAILED(Add_Component(L"Com_Calculator", m_pCalculatorCom)))
		return E_FAIL;

	//For. Shader;
	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Shader_Default"));
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;
	

	Safe_Release(pManagement);

	return S_OK;
}

_vec3 CCamera_CutScene::PickUp_OnTerrain(void)
{
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return _vec3();

	pManagement->AddRef();


	CBuffer_TerrainTex*	pTerrainBufferCom = dynamic_cast<CBuffer_TerrainTex*>
		(pManagement->Get_Component(SCENE_EDIT, L"Layer_Terrain", L"Com_Buffer"));
	if (pTerrainBufferCom == nullptr)
	{
		Safe_Release(pManagement);
		return _vec3();
	}

	CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>
		(pManagement->Get_Component(SCENE_EDIT, L"Layer_Terrain", L"Com_Transform"));
	if (pTerrainBufferCom == nullptr)
	{
		Safe_Release(pManagement);
		return _vec3();
	}

	Safe_Release(pManagement);

	RAY tRay = m_pCalculatorCom->Transform_ScreenPosToRay(&_vec2(400.f, 300.f));

	_ulong dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();
	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();


	_ulong dwVtxIdx[3];
	_float fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwVtxCntX + j;

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			// 오른쪽 위
			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[0]],
				&tRay.vRayPos, &tRay.vRayDir, &fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);
			}

			// 왼쪽 아래

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&tRay.vRayPos, &tRay.vRayDir, &fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
			}
		}
	}


	return _vec3();
}


CCamera_CutScene* CCamera_CutScene::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCamera_CutScene*	pInstance = new CCamera_CutScene(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CCamera_CutScene Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CCamera_CutScene::Free(void)
{
	Safe_Release(m_pCalculatorCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pLine_Manager);

	for (auto vEye : m_vecCamera_Eye)
		Safe_Release(vEye);
	m_vecCamera_Eye.clear();

	for (auto vAt : m_vecCamera_At)
		Safe_Release(vAt);
	m_vecCamera_At.clear();

	for (auto vPointArray : m_vecLine_Eye)
		Safe_Delete_Array(vPointArray);
	m_vecLine_Eye.clear();

	for (auto vPointArray : m_vecLine_At)
		Safe_Delete_Array(vPointArray);
	m_vecLine_At.clear();


	CCamera::Free();
}

Engine::CGameObject* Client::CCamera_CutScene::Clone_GameObject()
{
	CCamera_CutScene*	pInstance = new CCamera_CutScene(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CCamera_CutScene Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
