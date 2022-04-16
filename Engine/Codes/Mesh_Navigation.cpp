#include "..\Headers\Mesh_Navigation.h"


Engine::CMesh_Navigation::CMesh_Navigation(LPDIRECT3DDEVICE9 pDevice)
	: CMesh(pDevice)
	, m_iCellCnt(0)
{

}

Engine::CMesh_Navigation::CMesh_Navigation(const CMesh_Navigation& rhs)
	:CMesh(rhs)
	, m_vecCell(rhs.m_vecCell)
	, m_iCellCnt(rhs.m_iCellCnt)
{	
	for (auto& iter : m_vecCell)
		iter->AddRef();
}

HRESULT Engine::CMesh_Navigation::Ready_Mesh_Navigation(void)
{
	

	return S_OK;
}

void Engine::CMesh_Navigation::Render_NaviCell(void)
{
	for (auto& iter : m_vecCell)
		iter->Render_Cell();
}

_vec3 Engine::CMesh_Navigation::Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, _ulong& dwCurrentIndex)
{
	_vec3		vEndPos = *pTargetPos + *pTargetDir;
	_vec3		vSlideDir;

	CCell::COMPARE eResult = m_vecCell[dwCurrentIndex]->Compare(&vEndPos, pTargetDir, &dwCurrentIndex, &vSlideDir);

	switch (eResult)
	{
	case Engine::CCell::COMPARE_MOVE:
		return vEndPos;
		break;
	case Engine::CCell::COMPARE_UNMOVE:
		return *pTargetPos;
		break;
	case Engine::CCell::COMPARE_SLIDE:
		if (vSlideDir != nullptr)
		{
			_vec3 vSlidePos = *pTargetPos + vSlideDir;
			vSlidePos -= *pTargetDir;
			return vSlidePos;
		}
		break;
	default:
		break;
	}
	return 	*pTargetPos;
}

_vec3 CMesh_Navigation::Stand_OnNaviMesh(const _vec3 * pPosition, const _ulong & dwCurrentIndex)
{
	return m_vecCell[dwCurrentIndex]->Compare(pPosition);
}

_bool CMesh_Navigation::Compute_Position_OnNaviMeshIndex(const _vec3 * pPosition, _ulong& dwIndex)
{
	for (auto& cell : m_vecCell)
	{
		if (cell->Compare(pPosition, dwIndex))
			return true;
	}
	return false;
}

HRESULT Engine::CMesh_Navigation::Link_Cell(void)
{
	for (_int i = 0; i < m_iCellCnt; ++i)
	{
		for (_int j = 0; j < m_iCellCnt; ++j)
		{
			if (i == j)
				continue;

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB)
				&& true == m_vecCell[j]->Compare_Point(
					m_vecCell[i]->Get_Point(CCell::POINT_A),
					m_vecCell[i]->Get_Point(CCell::POINT_B), m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC)
				&& true == m_vecCell[j]->Compare_Point(
					m_vecCell[i]->Get_Point(CCell::POINT_B),
					m_vecCell[i]->Get_Point(CCell::POINT_C), m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA)
				&& true == m_vecCell[j]->Compare_Point(
					m_vecCell[i]->Get_Point(CCell::POINT_C),
					m_vecCell[i]->Get_Point(CCell::POINT_A), m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}
	}

	return S_OK;
}

Engine::CMesh_Navigation* Engine::CMesh_Navigation::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMesh_Navigation*	pInstance = new CMesh_Navigation(pDevice);

	if (FAILED(pInstance->Ready_Mesh_Navigation()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CComponent * CMesh_Navigation::Clone_Component(void)
{
	return new CMesh_Navigation(*this);
}

void Engine::CMesh_Navigation::Free(void)
{

	for (auto& pCell : m_vecCell)
		Safe_Release(pCell);

	m_vecCell.clear();

	Engine::CMesh::Free();
}

HRESULT Engine::CMesh_Navigation::Add_Cell(const _vec3* pPointBuffer)
{
	CCell*	pCell = nullptr;

	pCell = CCell::Create(m_pDevice,
		m_iCellCnt++,
		&pPointBuffer[0],
		&pPointBuffer[1],
		&pPointBuffer[2]);
	if (pCell == nullptr)
		E_FAIL;

	m_vecCell.push_back(pCell);
	if (FAILED(Link_Cell()))
		E_FAIL;

	return S_OK;
}

void Engine::CMesh_Navigation::Set_PickCellPoints(_bool bIsPick, const _ulong& dwCellIndex)
{	
	m_vecCell[dwCellIndex]->Set_bIsPick(bIsPick, CCell::POINT_A);
	m_vecCell[dwCellIndex]->Set_bIsPick(bIsPick, CCell::POINT_B);
	m_vecCell[dwCellIndex]->Set_bIsPick(bIsPick, CCell::POINT_C);
}

void Engine::CMesh_Navigation::Set_PickCellPoint(_bool bIsPick, const _ulong& dwCellIndex,const _ulong& dwPoint)
{
	m_vecCell[dwCellIndex]->Set_bIsPick(bIsPick, static_cast<CCell::POINT>(dwPoint));
}

HRESULT Engine::CMesh_Navigation::Remove_Cell(const _ulong& dwCellIndex)
{

	CCell* pCurrentCell = m_vecCell[dwCellIndex];
	_vec3 vPointA, vPointB;

	//이웃셀의 지워질 셀의 정보 널로 초기화
	CCell* pNeighborCell = pCurrentCell->Get_Neighbor(CCell::NEIGHBOR::NEIGHBOR_AB);
	if(pNeighborCell != NULL)
	{
		vPointA = *pCurrentCell->Get_Point(CCell::POINT_A);
		vPointB = *pCurrentCell->Get_Point(CCell::POINT_B);
		pNeighborCell->Compare_PointAndDelete(&vPointA, &vPointB);
	}
	

	pNeighborCell = pCurrentCell->Get_Neighbor(CCell::NEIGHBOR::NEIGHBOR_BC);
	if (pNeighborCell != NULL)
	{
		vPointA = *pCurrentCell->Get_Point(CCell::POINT_B);
		vPointB = *pCurrentCell->Get_Point(CCell::POINT_C);
		pNeighborCell->Compare_PointAndDelete(&vPointA, &vPointB);
	}

	pNeighborCell = pCurrentCell->Get_Neighbor(CCell::NEIGHBOR::NEIGHBOR_CA);
	if (pNeighborCell != NULL)
	{
		vPointA = *pCurrentCell->Get_Point(CCell::POINT_C);
		vPointB = *pCurrentCell->Get_Point(CCell::POINT_A);
		pNeighborCell->Compare_PointAndDelete(&vPointA, &vPointB);
	}
	
	auto& iter = find_if(m_vecCell.begin(), m_vecCell.end(), [&](const CCell* pCell) {
		if (*pCell->Get_Index() == dwCellIndex)
			return true;
		return false;
	});


	////현재 인덱스 삭제
	Safe_Release(m_vecCell[dwCellIndex]);

	if(iter != m_vecCell.end())
		iter = m_vecCell.erase(iter);

	//삭제한 다음 메쉬들의 인덱스를 하나식 당겨온다
	for(; iter != m_vecCell.end(); ++iter)
	{
		(*iter)->Minus_Index();
	}
	m_iCellCnt--;

		
	return S_OK;

}

HRESULT CMesh_Navigation::Load_InfoFromFile(const _tchar * pFilePath)
{
	HANDLE		hFile = CreateFile(pFilePath///L"../Bin/Resources/Data/test.navi"
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	DWORD	dwByte = 0;


	_int	iSize;
	ReadFile(hFile, &iSize, sizeof(_int), &dwByte, nullptr);

	for (_int i = 0; i < iSize; ++i)
	{
		//포인터 3개 저장 배열
		_vec3 vPoint[Engine::CCell::POINT_END];

		ReadFile(hFile, &vPoint[Engine::CCell::POINT_A], sizeof(_vec3), &dwByte, nullptr);

		ReadFile(hFile, &vPoint[Engine::CCell::POINT_B], sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vPoint[Engine::CCell::POINT_C], sizeof(_vec3), &dwByte, nullptr);

		_ulong dwIndex;
		ReadFile(hFile, &dwIndex, sizeof(_ulong), &dwByte, nullptr);

		this->Add_Cell(vPoint);
	}

	////셀리스트 추가
	//const vector<Engine::CCell*>* pvecCell = m_pNaviMesh_Controller->Get_VectorCell();
	//for (auto& iter : *pvecCell)
	//{
	//	Add_NaviMeshCell(iter, *iter->Get_Index());
	//}

	CloseHandle(hFile);


	return S_OK;
}

void Engine::CMesh_Navigation::Set_PickCell(_bool bIsPick, const _ulong& dwCellIndex)
{
	m_vecCell[dwCellIndex]->Set_bIsCellPick(bIsPick);
}


