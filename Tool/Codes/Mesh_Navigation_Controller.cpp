#include "stdafx.h"
#include "Mesh_Navigation_Controller.h"

#include "Management.h"

//MFC
#include "MainFrm.h"
#include "ControlForm.h"
#include "NavigationMeshTab.h"
#include "Object_Static.h"

_USING(Client)

CMesh_Navigation_Controller::CMesh_Navigation_Controller(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_iBufferIndex(0)
	, m_bIsMouseDown(false)
	, m_bIsPick(false)
	, m_dwPickCellIndex(0)
	, m_eMouseType(MOUSETYPE_END)
	,m_pTransformCom(nullptr)
	,m_pRendererCom(nullptr)
	,m_pCalculatorCom(nullptr)
	,m_pNaviMeshCom(nullptr)
	,m_pFont_Manager(CFont_Manager::GetInstance())
	,m_pInput_Device(CInput_Device::GetInstance())
{
	m_pFont_Manager->AddRef();
	m_pInput_Device->AddRef();
}

CMesh_Navigation_Controller::CMesh_Navigation_Controller(const CMesh_Navigation_Controller & rhs)
	:CGameObject(rhs)
	, m_pFont_Manager(rhs.m_pFont_Manager)
	,m_pInput_Device(rhs.m_pInput_Device)
	, m_iBufferIndex(rhs.m_iBufferIndex)
	, m_bIsMouseDown(rhs.m_bIsMouseDown)
	, m_bIsPick(rhs.m_bIsPick)
	, m_dwPickCellIndex(rhs.m_dwPickCellIndex)
	, m_eMouseType(MOUSETYPE_END)

{
	m_pFont_Manager->AddRef();
	m_pInput_Device->AddRef();
}

HRESULT CMesh_Navigation_Controller::Ready_Prototype()
{

	return S_OK;
}

HRESULT CMesh_Navigation_Controller::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	return S_OK;
}

Engine::_int CMesh_Navigation_Controller::Update_GameObject(const _float& fTimeDelta)
{
	if (((CMainFrame*)AfxGetMainWnd())->m_pControlForm->m_eTabType == CControlForm::TAB_NAVIMESH)
		Key_Input(fTimeDelta);

	return 0;
}

Engine::_int CMesh_Navigation_Controller::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return 0;
}

void CMesh_Navigation_Controller::Render_GameObject()
{
	m_pNaviMeshCom->Render_NaviCell();

	if (((CMainFrame*)AfxGetMainWnd())->m_pControlForm->m_eTabType == CControlForm::TAB_NAVIMESH)
	{
		//폰트출력
		switch (m_eMouseType)
		{
		case CMesh_Navigation_Controller::INSERT_POINT:
		{
			POINT	ptMouse = {};

			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);

			_tchar	szBufferIndex[MIN_PATH];

			wsprintf(szBufferIndex, L"%d번째 인덱스", m_iBufferIndex);

			m_pFont_Manager->Render_Font(L"Font_Default", szBufferIndex, &_vec2((_float)ptMouse.x, (_float)ptMouse.y - 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		}
		break;
		case CMesh_Navigation_Controller::SELECT_POINT:
			break;
		case CMesh_Navigation_Controller::SELECT_CELL:
			break;
		default:
			break;
		}
	}
}

HRESULT CMesh_Navigation_Controller::Ready_Component(void)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	m_pRendererCom = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	m_pCalculatorCom = dynamic_cast<CCalculator*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Calculator"));
	if (FAILED(Add_Component(L"Com_Calculator", m_pRendererCom)))
		return E_FAIL;

	m_pNaviMeshCom = dynamic_cast<CMesh_Navigation*>(pManagement->Clone_Component(SCENE_EDIT, L"Component_Mesh_Navigation"));
	if (FAILED(Add_Component(L"Com_NaviMesh", m_pRendererCom)))
		return E_FAIL;

	Safe_Release(pManagement);


	return S_OK;
}

HRESULT CMesh_Navigation_Controller::Add_Cell(_vec3* pPointBuffer)
{
	m_pNaviMeshCom->Add_Cell(pPointBuffer);

	return S_OK;
}

void CMesh_Navigation_Controller::Key_Input(const _float fTimeDelta)
{	
	//Exception
	HWND	hWnd = ::GetFocus();

	if (hWnd != g_hWnd)
		return;

	POINT	ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (ptMouse.x < 0 || ptMouse.x > g_nBackCX || ptMouse.y < 0 || ptMouse.y > g_nBackCY)
		return;

	//wheel
	if (m_PickList.size() > 0)
	{
		_int iCellCnt = m_pNaviMeshCom->Get_CellCnt();
		if (iCellCnt >= 0)	//인덱스가 하나도 없으면
		{
			for (auto& pickPoint : m_PickList)
			{
				//인덱스의 셀값을 가져옴 (마우스를 누르고 있는 중이라면)
				Engine::CCell* pCell = m_pNaviMeshCom->Get_Cell(pickPoint.m_SelectIndex);
				_vec3* pPoint = pCell->Get_Point(pickPoint.m_SelectPoint);
				Engine::CCollider_Sphere* pCollider = pCell->Get_Collider(pickPoint.m_SelectPoint);

				_vec3 vMouseMove;
				ZeroMemory(&vMouseMove, sizeof(_vec3));

				if (vMouseMove.y = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Z))
				{
					vMouseMove.y -= 20.f;
					pPoint->y += vMouseMove.y * 0.01f;
					pCell->Update_VB();
				}
				m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPoint);

				m_pNavigationMeshTab->Update_Transform(CNavigationMeshTab::DATA_POSITION, pPoint);
			}
		}
	}


	if (m_pInput_Device->Get_MouseDown(CInput_Device::DIM_LB))
	{
		m_bIsMouseDown = true;
		m_PickList.clear();

		switch (m_eMouseType)
		{
		case CMesh_Navigation_Controller::INSERT_POINT:
			Insert_Point();
			break;
		case CMesh_Navigation_Controller::SELECT_POINT:
			Select_Point();
			break;
		case CMesh_Navigation_Controller::SELECT_CELL:
			Select_Cell();
			break;
		default:
			break;
		}
	}

	if(m_pInput_Device->Get_MousePressing(CInput_Device::DIM_LB))
	{
		if(m_bIsMouseDown)
		{
			_int iCellCnt = m_pNaviMeshCom->Get_CellCnt();
			if(iCellCnt >= 0)	//인덱스가 하나도 없으면
			{
				for(auto& pickPoint : m_PickList)
				{
					//인덱스의 셀값을 가져옴 (마우스를 누르고 있는 중이라면)
					CCell* pCell = m_pNaviMeshCom->Get_Cell(pickPoint.m_SelectIndex);
					_vec3* pPoint = pCell->Get_Point(pickPoint.m_SelectPoint);

					_vec3 vMouseMove;
					ZeroMemory(&vMouseMove, sizeof(_vec3));
					if (vMouseMove.z = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Y))
					{
						pPoint->z -= vMouseMove.z * 0.1f;
						pCell->Update_VB();
					}
					 
					ZeroMemory(&vMouseMove, sizeof(_vec3));
					if (vMouseMove.x = m_pInput_Device->Get_MouseMove(Engine::CInput_Device::DIMS_X))
					{
						pPoint->x += vMouseMove.x * 0.1f;
						pCell->Update_VB();		
					}

					m_pNavigationMeshTab->Update_Transform(CNavigationMeshTab::DATA_POSITION, pPoint);
				}
			}
		}
	}

	if (m_pInput_Device->Get_MouseUp(Engine::CInput_Device::DIM_LB))
	{
		switch (m_eMouseType)
		{
		case CMesh_Navigation_Controller::INSERT_POINT:
			break;
		case CMesh_Navigation_Controller::SELECT_POINT:
		break;
		case CMesh_Navigation_Controller::SELECT_CELL:
			break;
		default:
			break;
		}
		m_bIsMouseDown = false;
	}

	if(m_pInput_Device->Get_KeyDown(DIK_DELETE))
	{
		m_pNaviMeshCom->Remove_Cell(m_dwPickCellIndex);
	}
	
}

_vec3 CMesh_Navigation_Controller::PickUp_OnTerrain(void)
{
	CBuffer_TerrainTex*	pTerrainBufferCom = dynamic_cast<CBuffer_TerrainTex*>
		(CManagement::GetInstance()->Get_Component(SCENE_EDIT,L"Layer_Terrain",L"Com_Buffer"));

	if (pTerrainBufferCom == nullptr)
	{
		AfxMessageBox(L"TerrainBufferCom is Null");
	}

	CTransform*		pTerrainTransCom = dynamic_cast<Engine::CTransform*>
		(CManagement::GetInstance()->Get_Component(SCENE_EDIT, L"Layer_Terrain", L"Com_Transform"));
	if (pTerrainTransCom == nullptr)
	{
		AfxMessageBox(L"TerrainTransCom is Null");
	}

	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

void CMesh_Navigation_Controller::Insert_Point()
{
	//1순위 충돌점
	//기존 정보 검색후 충돌점이 있으면 충돌점 정보를 넘겨줌.
	_int iCellCnt = m_pNaviMeshCom->Get_CellCnt();

	//Transform MosuePos to World Ray
	RAY	tRay = m_pCalculatorCom->Transform_ScreenPosToRay(g_hWnd);

	//셀순회
	for (_int i = 0; i <= iCellCnt; ++i)
	{
		//Get Cell
		const CCell* pCell = m_pNaviMeshCom->Get_Cell(i);

		//Collision Check for Point' Spehre
		for (_ulong j = 0; j < CCell::POINT_END; ++j)
		{
			const CCollider_Sphere* pCollider = pCell->Get_Collider(j);
			const _vec3* vPosition = pCell->Get_Point(j);

			if (m_pCalculatorCom->Picking_Sphere(tRay, pCollider))
			{
				const _vec3*	vPickPos = pCell->Get_Point(j);

				m_vPointBuffer[m_iBufferIndex++] = *vPickPos;

				if (m_iBufferIndex > 2)
				{
					m_iBufferIndex = 0;
					m_pNaviMeshCom->Add_Cell(m_vPointBuffer);

					_ulong dwCellIndex = m_pNaviMeshCom->Get_CellCnt();

					const Engine::CCell* pCell = m_pNaviMeshCom->Get_Cell(dwCellIndex);

					m_pNavigationMeshTab->Add_NaviMeshCell(pCell, dwCellIndex);
				}
				return;
			}
		}
	}
	//2순위 메쉬
	//스테틱 메쉬 리스트 순회
	CManagement* pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return;
	pManagement->AddRef();

	const list<CGameObject*>* pObjectList = pManagement->Get_Layer_ObjectList(SCENE_EDIT, L"Layer_Static");
	if (pObjectList != nullptr)
	{
		for (auto& pObject : *pObjectList)
		{

			CObject_Static* pObject_Static;
			pObject_Static = dynamic_cast<CObject_Static*>(pObject);
			if (pObject == nullptr)
				return;

			_vec3 vPickingPosition;
			if (pObject_Static->Intersect_Static_Mesh(tRay, &vPickingPosition))
			{
				m_vPointBuffer[m_iBufferIndex++] = vPickingPosition;

				if (m_iBufferIndex > 2)
				{
					m_iBufferIndex = 0;
					m_pNaviMeshCom->Add_Cell(m_vPointBuffer);

					_ulong dwCellIndex = m_pNaviMeshCom->Get_CellCnt();

					const Engine::CCell* pCell = m_pNaviMeshCom->Get_Cell(dwCellIndex);

					m_pNavigationMeshTab->Add_NaviMeshCell(pCell, dwCellIndex);
				}
				Safe_Release(pManagement);
				return;
			}
		}
	}
	Safe_Release(pManagement);
	
	//3순위 지형
	//if Mouse don't pick the object, Get the Terrain Position
	_vec3	vPickPos = PickUp_OnTerrain();
	if (vPickPos == _vec3())
		return;

	m_vPointBuffer[m_iBufferIndex++] = vPickPos;
//	m_pNavigationMeshTab->Update_Transform(CNavigationMeshTab::DATA_POSITION, &vPickPos);
	

	if (m_iBufferIndex > 2)
	{
		m_iBufferIndex = 0;
		m_pNaviMeshCom->Add_Cell(m_vPointBuffer);

		_ulong dwCellIndex = m_pNaviMeshCom->Get_CellCnt();

		const CCell* pCell = m_pNaviMeshCom->Get_Cell(dwCellIndex);

		m_pNavigationMeshTab->Add_NaviMeshCell(pCell, dwCellIndex);
	}
}

void CMesh_Navigation_Controller::Select_Point()
{
	_int iCellCnt = m_pNaviMeshCom->Get_CellCnt();

	//Transform MosuePos to World Ray
	RAY	tRay = m_pCalculatorCom->Transform_ScreenPosToRay(g_hWnd);

	//순회
	for (_int i = 0; i <= iCellCnt; ++i)
	{
		const CCell* pCell = m_pNaviMeshCom->Get_Cell(i);

		//Pick Point false Setting
		Set_PickCellPoints(false, i);

		for (_ulong j = 0; j < Engine::CCell::POINT_END; ++j)
		{
			const CCollider_Sphere* pCollider = pCell->Get_Collider(CCell::POINT(j));
			const _vec3* vPosition = pCell->Get_Point(j);

			if (m_pCalculatorCom->Picking_Sphere(tRay, pCollider))
			{
				Set_PickCellPoint(true, i, j);

				PICKPOINT	tPickPoint;
				tPickPoint.m_SelectIndex = i;
				tPickPoint.m_SelectPoint = j;

				m_PickList.push_back(tPickPoint);

				m_pNavigationMeshTab->Update_Transform(CNavigationMeshTab::DATA_POSITION, pCell->Get_Point(CCell::POINT(j)));
			}
		}
	}
}

void CMesh_Navigation_Controller::Select_Cell()
{
	//기존 정보 검색후 충돌점이 있으면 충돌점 정보를 넘겨줌.
	_int iCellCnt = m_pNaviMeshCom->Get_CellCnt();

	RAY tRay = m_pCalculatorCom->Transform_ScreenPosToRay(g_hWnd);

	//순회
	for (_int i = 0; i <= iCellCnt; ++i)
	{
		const Engine::CCell* pCell = m_pNaviMeshCom->Get_Cell(i);

		const _vec3* pPoint = pCell->Get_Point();

		//셀읠 삼각형가 충돌검사
		if (m_pCalculatorCom->Picking_Triangle(tRay, pPoint))
		{
			Set_PickCell(false, m_dwPickCellIndex);

			//충돌시 표면의 색을 변경시킨다.
			Set_PickCell(true, i);
			m_dwPickCellIndex = i;
			return;
		}
	}
}

CMesh_Navigation_Controller* CMesh_Navigation_Controller::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMesh_Navigation_Controller*	pInstance = new CMesh_Navigation_Controller(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CMesh_Navigation_Controller Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CMesh_Navigation_Controller::Clone_GameObject()
{
	CMesh_Navigation_Controller*	pInstance = new CMesh_Navigation_Controller(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CMesh_Navigation_Controller Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CMesh_Navigation_Controller::Free(void)
{

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pCalculatorCom);
	Safe_Release(m_pNaviMeshCom);

	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pInput_Device);


	if (m_PickList.size() != 0)
	{
		m_PickList.clear();
	}

	CGameObject::Free();
}

void CMesh_Navigation_Controller::Set_PickCellPoints(_bool bIsPick, const _ulong& dwCellIndex)
{
	m_pNaviMeshCom->Set_PickCellPoints(bIsPick, dwCellIndex);
}

void CMesh_Navigation_Controller::Set_PickCellPoint(_bool bIsPick, const _ulong& dwCellIndex, const _ulong& dwPoint)
{
	m_pNaviMeshCom->Set_PickCellPoint(bIsPick, dwCellIndex, dwPoint);
}

void CMesh_Navigation_Controller::Set_PickCell(_bool bIsPick, const _ulong& dwCellIndex)
{
	m_pNaviMeshCom->Set_PickCell(bIsPick, dwCellIndex);
}

//std::vector<Engine::CCell*>* CMesh_Navigation_Controller::Get_VectorCell(void)
//{
//	return m_pNaviMeshCom->Get_VectorCell();
//}

const std::vector<Engine::CCell*>* CMesh_Navigation_Controller::Get_VectorCell(void) const
{
	return m_pNaviMeshCom->Get_VectorCell();
}
