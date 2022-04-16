#ifndef Cell_h__
#define Cell_h__

#include "Base.h"
#include "Navigation_Line.h"

_BEGIN(Engine)

class CTexture;
class CShader;
class CCollider_Sphere;
class CFont_Manager;
class _ENGINE_DLL CCell : public CBase
{
public:
	enum POINT		{POINT_A, POINT_B, POINT_C, POINT_END};
	enum NEIGHBOR	{NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END};
	enum LINE		{LINE_AB, LINE_BC,LINE_CA, LINE_END};
	enum COMPARE	{COMPARE_MOVE, COMPARE_UNMOVE, COMPARE_SLIDE};
	enum SURFACECOL	{COL_RED, COL_GREEN, COL_YELLOW};

private:
	explicit		CCell(LPDIRECT3DDEVICE9	pDevice);
	virtual			~CCell(void) = default;
public:

	//Get	
	const _ulong*			Get_Index(void)const { return &m_dwIndex; }
	const _vec3*			Get_Point(_uint iIndex)const { return &m_vPoint[iIndex]; }
	_vec3*					Get_Point(_uint iIndex) { return &m_vPoint[iIndex]; }
	const _vec3*			Get_Point(void)	const { return m_vPoint; }
	const CCell*			Get_Neighbor(NEIGHBOR eType) const { return m_pNeighbor[eType]; }
	CCell*					Get_Neighbor(NEIGHBOR eType) { return m_pNeighbor[eType]; }
	const CCollider_Sphere*	Get_Collider(_uint iIndex) const { return m_pColliderCom[iIndex]; }
	CCollider_Sphere*		Get_Collider(_uint iIndex) { return m_pColliderCom[iIndex]; }
	//Set
public:
	void			Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor)			{ m_pNeighbor[eType] = pNeighbor; }
	void			Set_bIsPick(_bool bIsPick, POINT ePoint)				{ m_bIsPointPick[ePoint] = bIsPick; }
	void			Set_bIsCellPick(_bool bIsPick)							{ m_bIsCellPick = bIsPick; }

	//
	void			Minus_Index(void)										{ m_dwIndex--; }
public:
	HRESULT			Ready_Cell(const _ulong& dwIndex,
								const _vec3* pPointA,
								const _vec3* pPointB,
								const _vec3* pPointC);

	//현재 포인트와 매개변수로 들어온 포인트를 비교해서, 이웃정보를 채워주는 함수
	_bool			Compare_Point(const _vec3* pPointA,
								const _vec3* pPointB,
								CCell* pCell);
	_bool			Compare_PointAndDelete(const _vec3* pPointA,
											const _vec3* pPointB);

	COMPARE			Compare(_vec3* pEndPos,
							const _vec3* pDir,
							_ulong* pCellIndex,
							_vec3* pSlideDir);

	//포지션을 받아서 현재 포지션의 네비메쉬 인덱스를 찾기위해 비교하는 함수
	_bool			Compare(const _vec3* pPosition, _ulong& dwIndex);

	//현재 포지션을 네비메위Y값 적용 위치를 구해주는 함수
	_vec3			Compare(const _vec3* pPosition);




	void			Render_Cell(void);
	void			Update_VB();


private:
	HRESULT			Ready_VertexBuffer(void);
	void			Render_Surface(void);

	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect, SURFACECOL eType);


private:
	//셀의 포인터
	_vec3					m_vPoint[POINT_END];
	CCell*					m_pNeighbor[NEIGHBOR_END];
	CNavigation_Line*		m_pLine[LINE_END];
	_ulong					m_dwIndex = 0;
//	_matrix					m_matPoint[POINT_END];

	// For. Debug Variable
	_vec3					m_vCenterPoint;
	_tchar					m_szIndex[MIN_PATH];


	CCollider_Sphere*		m_pColliderCom[POINT_END];
	_bool					m_bIsPointPick[POINT_END];
	_bool					m_bIsCellPick;


	CShader*				m_pShaderCom = nullptr;
	LPDIRECT3DDEVICE9		m_pDevice;
	CTexture*				m_pTexture;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;

	//
	CFont_Manager*			m_pFont_Manager;
public:
	static	CCell*		Create(LPDIRECT3DDEVICE9	pDevice,
								const _ulong& dwIndex,
								const _vec3* pPointA,
								const _vec3* pPointB,
								const _vec3* pPointC);
	static CCell*		Create(LPDIRECT3DDEVICE9 pDevice);

	virtual	void	Free(void);


};

_END

#endif // Cell_h__
