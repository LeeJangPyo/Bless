#pragma once

#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL CGraphic_Device final : public CBase
{
	_DECLARE_SINGLETON(CGraphic_Device)
public:
	enum WINMODE {MODE_FULL, MODE_WIN};
private:
	explicit CGraphic_Device(void);
	virtual ~CGraphic_Device(void) = default;
public: // getter
	LPDIRECT3DDEVICE9 Get_Graphic_Device(void) const {
		return m_pDevice;}
public:
	HRESULT Ready_Graphic_Device(WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY, LPDIRECT3DDEVICE9* ppGraphic_Device);	
private:
	LPDIRECT3D9					m_pSDK;
	LPDIRECT3DDEVICE9			m_pDevice; // 장치를 대표하는 객체다.
private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);
public:
	virtual void Free(void);
};

_END

