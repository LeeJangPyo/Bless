#ifndef Buffer_TerrainTex_h__
#define Buffer_TerrainTex_h__

#include "VIBuffer.h"

_BEGIN(Engine)

struct Triangle
{
	unsigned short  index[3];

};
class _ENGINE_DLL CBuffer_TerrainTex final: public CVIBuffer
{
private:
	explicit CBuffer_TerrainTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CBuffer_TerrainTex(const CBuffer_TerrainTex& rhs);
	virtual ~CBuffer_TerrainTex(void) = default;

public:
	const _vec3*		Get_VtxPos(void) const { return m_pPosition; }
	_ulong				Get_VtxCntX(void) const {return m_dwCntX;}
	_ulong				Get_VtxCntZ(void) const { return m_dwCntZ; }

	void				Set_Index(INDEX32* pIndex, const _ulong& dwTriCnt);
	HRESULT				SetUp_OnShader_Detail(LPD3DXEFFECT pEffect, const _char * pConstantTable);

public:
	HRESULT	Ready_Buffer(const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _ulong& dwVtxItv,
		const _float& fDetail = 40.f,
		const _float& fHeight = 100.f);

private:
	_ulong*				LoadImage(void);
	void				CalculateTangentArray(_ulong dwVertexCount,VTXTANGENT* pVertexInfo, _ulong dwTriangleCount, const INDEX32 * pIndex);

private:
	HANDLE				m_hFile = nullptr;
	//BITMAPFILEHEADER	m_fH;
	//BITMAPINFOHEADER	m_iH;
	_vec3*				m_pPosition;
	_bool				m_bIsClone = false;
	_ulong				m_dwCntX;
	_ulong				m_dwCntZ;

	_float				m_fDetail;


public:
	static CBuffer_TerrainTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _ulong& dwVtxItv,
		const _float& fDetail = 40.f,
		const _float& fHeight = 100.f);

protected:
	virtual void	Free(void) override;
	virtual CComponent* Clone_Component() override;
};

_END
#endif // Buffer_TerrainTex_h__
