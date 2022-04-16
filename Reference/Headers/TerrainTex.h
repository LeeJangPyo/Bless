#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

_BEGIN(Engine)

class _ENGINE_DLL CTerrainTex final: public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex(void);

public:
	const _vec3*		Get_VtxPos(void) const { return m_pPosition; }
	_ulong				Get_VtxCntX(void) const {return m_dwCntX;}
	_ulong				Get_VtxCntZ(void) const { return m_dwCntZ; }

	void				Set_Index(INDEX32* pIndex, const _ulong& dwTriCnt);

public:
	HRESULT	Ready_Buffer(const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _ulong& dwVtxItv,
		const _float& fDetail = 40.f,
		const _float& fHeight = 100.f);

private:
	_ulong*				LoadImage(void);

private:
	HANDLE				m_hFile = nullptr;
	//BITMAPFILEHEADER	m_fH;
	//BITMAPINFOHEADER	m_iH;
	_vec3*				m_pPosition;
	_bool				m_bIsClone = false;
	_ulong				m_dwCntX;
	_ulong				m_dwCntZ;


public:
	static CTerrainTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
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
#endif // TerrainTex_h__
