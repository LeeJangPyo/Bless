#ifndef Light_h__
#define Light_h__

#include "Base.h"

_BEGIN(Engine)
class CBuffer_Screen;
class _ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLight(void) = default;
public:
	const D3DLIGHT9*	Get_LightInfo(void) const { 
		return &m_LightInfo; }
	D3DLIGHT9*			Get_LightInfo(void) {
		return &m_LightInfo; }
public:
	HRESULT Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	void Render_Light(LPD3DXEFFECT pEffect);
private:
	LPDIRECT3DDEVICE9			m_pDevice;
	D3DLIGHT9					m_LightInfo;
	D3DMATERIAL9				m_MtrlInfo;
	_uint						m_iIndex;
	_float						m_fFarPlane;


	CBuffer_Screen*				m_pBuffer_Screen;
public:
	static CLight*		Create(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
private:
	virtual void		Free(void);
};

_END

#endif // Light_h__
