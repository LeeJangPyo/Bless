#ifndef Navigation_Mesh_h__
#define Navigation_Mesh_h__

#include "Mesh.h"
#include "Cell.h"

_BEGIN(Engine)

class _ENGINE_DLL CNavigation_Mesh : public CMesh
{
private:
	explicit	CNavigation_Mesh(LPDIRECT3DDEVICE9 pDevice);
	explicit	CNavigation_Mesh(const CNavigation_Mesh& rhs);
	virtual		~CNavigation_Mesh(void);

	//get
public:

	//현재 개수보다 1개 많음
	_int					Get_CellCnt(void) { return m_iCellCnt-1; }
	const CCell*			Get_Cell(_ulong dwCellIndex) const { return m_vecCell[dwCellIndex]; }
	CCell*					Get_Cell(_ulong dwCellIndex)	{ return m_vecCell[dwCellIndex]; }
	const vector<CCell*>*	Get_VectorCell(void) const		{ return &m_vecCell; }
	//vector<CCell*>*			Get_VectorCell(void)			 { return &m_vecCell; }

public:
	void		Set_CurrentCellIndex(const _ulong& dwCurrentIndex) { m_dwCurrentIndex = dwCurrentIndex; }
	void		Set_PickCellPoints(_bool bIsPick, const _ulong& dwCellIndex);
	void		Set_PickCellPoint(_bool bIsPick, const _ulong& dwCellIndex, const _ulong& dwPoint);
	void		Set_PickCell(_bool bIsPick, const _ulong& dwCellIndex);
public:
	HRESULT		Ready_Navigation_Mesh(void);
	void		Render_NaviCell(void);
	_vec3		Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);
	
	HRESULT		Add_Cell(const _vec3* pPointBuffer);
	HRESULT		Remove_Cell(const _ulong& dwCellIndex);

private:
	HRESULT		Link_Cell(void);


private:
	vector<CCell*>				m_vecCell;
	_int						m_iCellCnt;
	_ulong						m_dwCurrentIndex;
	
public:
	static CNavigation_Mesh*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent*			Clone_Component(void) override;
protected:
	virtual void				Free(void) override;

};

_END

#endif // Navigation_Mesh_h__
