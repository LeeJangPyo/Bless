#ifndef Mesh_Navigation_h__
#define Mesh_Navigation_h__

#include "Mesh.h"
#include "Cell.h"

_BEGIN(Engine)

class _ENGINE_DLL CMesh_Navigation : public CMesh
{
private:
	explicit	CMesh_Navigation(LPDIRECT3DDEVICE9 pDevice);
	explicit	CMesh_Navigation(const CMesh_Navigation& rhs);
	virtual		~CMesh_Navigation(void) = default;

	//get
public:

	//현재 개수보다 1개 많음
	_int					Get_CellCnt(void) { return m_iCellCnt-1; }
	const CCell*			Get_Cell(_ulong dwCellIndex) const { return m_vecCell[dwCellIndex]; }
	CCell*					Get_Cell(_ulong dwCellIndex)	{ return m_vecCell[dwCellIndex]; }
	const vector<CCell*>*	Get_VectorCell(void) const		{ return &m_vecCell; }
	//vector<CCell*>*			Get_VectorCell(void)			 { return &m_vecCell; }
	//_ulong					Get_CurrentIndex(){ return m_dwCurrentIndex; }

public:
//	void		Set_CurrentCellIndex(const _ulong& dwCurrentIndex) { m_dwCurrentIndex = dwCurrentIndex; }
	void		Set_PickCellPoints(_bool bIsPick, const _ulong& dwCellIndex);
	void		Set_PickCellPoint(_bool bIsPick, const _ulong& dwCellIndex, const _ulong& dwPoint);
	void		Set_PickCell(_bool bIsPick, const _ulong& dwCellIndex);
public:
	HRESULT		Ready_Mesh_Navigation(void);
	void		Render_NaviCell(void);
	_vec3		Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, _ulong& dwCurrentIndex);
	_vec3		Stand_OnNaviMesh(const _vec3* pPosition, const _ulong& dwCurrentIndex);
	_bool		Compute_Position_OnNaviMeshIndex(const _vec3* pPosition, _ulong& dwIndex);
	
	HRESULT		Add_Cell(const _vec3* pPointBuffer);
	HRESULT		Remove_Cell(const _ulong& dwCellIndex);

	HRESULT		Load_InfoFromFile(const _tchar* pFilePath);

private:
	HRESULT		Link_Cell(void);


private:
	vector<CCell*>				m_vecCell;
	_int						m_iCellCnt;
	
public:
	static CMesh_Navigation*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent*			Clone_Component(void) override;
protected:
	virtual void				Free(void) override;

};

_END

#endif // Mesh_Navigation_h__
