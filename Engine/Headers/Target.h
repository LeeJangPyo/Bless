
#ifndef Target_h__
#define Target_h__

//·£´õ Å¸°Ù ÇÏ³ª¸¦ ´ëÇ¥ÇÏ´Â °´Ã¼
//CTarget °´Ã¼ÇÏ³ª°¡ °ð ·£´õÅ¸°Ù
#include "Base.h"


_BEGIN(Engine)
class CBuffer_Screen;

class _ENGINE_DLL CTarget final : public CBase
{
public:
	enum TARGETTYPE {	TARGET_NORMAL, TARGET_SHADOW};
public:
	explicit CTarget(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTarget() = default;
public:
	HRESULT		Ready_Target(const _float& fSizeX, const _float& fSizeAY, D3DFORMAT Format, D3DXCOLOR Clear_Color,TARGETTYPE eType);
	HRESULT		Ready_DebugBuffer(const _float& fStartX, const _float& fStartY, const _float& fSizeX, const _float& fSizeY);

	//Slot Setup
	HRESULT		SetUp_OnGraphic_Device(const _uint& iIndex);
	HRESULT		Release_OnGraphic_Device(_uint iIndex);
	HRESULT		SetUp_OnGraphic_Device_Shadow(const _uint& iIndex);
	HRESULT		Release_OnGraphic_Device_Shadow(_uint iIndex);

	//Clear Target;
	HRESULT		Clear_Target();
	HRESULT		Clear_Shadow_Target();

	//Render
	void		Render_DebugBuffer();

	HRESULT		SetUp_Texture_OnShader(LPD3DXEFFECT pEffect, const _char * pConstantName);

private:
	LPDIRECT3DDEVICE9	m_pDevice;
	LPDIRECT3DTEXTURE9	m_pTarget_Texture;
	LPDIRECT3DSURFACE9  m_pTarget_Surface;
	LPDIRECT3DSURFACE9  m_pOld_Surface;

	LPDIRECT3DSURFACE9	m_pShadowDepth_Surface;
	LPDIRECT3DSURFACE9	m_pOld_Dpeth_Surface;



	D3DXCOLOR			m_Clear_Color;
	//For. DebugBuffer
private:
	//LPDIRECT3DVERTEXBUFFER9		m_pVB;
	//LPDIRECT3DINDEXBUFFER9		m_pIB;
	CBuffer_Screen*		m_pBuffer_Screen;
public:
	static CTarget*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _float& fSizeX, const _float& fSizeY, D3DFORMAT Format, D3DXCOLOR Clear_Color, TARGETTYPE eType);
protected:
	virtual void Free();
};


_END
#endif // Target_h__