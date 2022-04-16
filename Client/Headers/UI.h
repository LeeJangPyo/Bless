#ifndef UI_h__
#define UI_h__

#include "Defines.h"
#include "Base.h"

_BEGIN(Engine)
class CTexture;
class CBuffer_RcTex;
class CTransform;
class CInput_Device;
_END

_BEGIN(Client)
class CUI abstract : public CBase
{
protected:
	enum  SHADER_PATH
	{
		DEFAULT = 0, BAR, BARMASK, ICONMASK, ICONCOOLDOWN
	};
public:
	explicit CUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CUI() = default;

public:
	void		Set_TextureIndex(const _int& iIndex) { m_iTextureIndex = iIndex; }

	void		SetUp_LoadInfo(const _matrix* pmatWorld, const _tchar* pTextureComponentTag);
	HRESULT		Replace_Texture_Component(const _tchar* pComponentProtoTag);

public:
	virtual	HRESULT		Ready_UI();
	virtual _int		Update_UI(const _float& fTimeDelta, const _matrix* pmatParent);
	virtual void		Render_UI(LPD3DXEFFECT pEffect, const _matrix* pmatProj);

public:
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix* pmatProj);

private:
	HRESULT		Ready_Component();
	

protected:
	LPDIRECT3DDEVICE9	m_pDevice;

	CTransform*			m_pTransformCom;
	CBuffer_RcTex*		m_pBufferCom;
	CTexture*			m_pTextureCom;
	CInput_Device*		m_pInput_Device;

	_matrix				m_matCombinedWorld;

	wstring				m_wstrTextureComponentTag;

	_int				m_iTextureIndex;
protected:
	virtual void Free() override;

};


_END
#endif // UI_h__
