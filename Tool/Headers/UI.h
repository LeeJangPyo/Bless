#ifndef UI_h__
#define UI_h__

#include "Defines.h"
#include "Base.h"

#include "UITab.h"
_BEGIN(Engine)
class CTexture;
class CBuffer_RcTex;
class CTransform;
class CInput_Device;
_END

_BEGIN(Client)
class CUI final : public CBase
{
public:
	explicit CUI(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CUI() = default;

public:
	void		Set_Select(_bool bIsSelect) { m_bIsSelect = bIsSelect; }
	void		Set_UITab(CUITab* pUITab) { m_pUITab = pUITab; }
	void		SetUp_LoadInfo(const _matrix* pmatWorld, const _tchar* pTextureComponentTag);
	void		Set_Position(const _float& fX, const _float& fY, const _float & fZ);
	void		Set_Scale(const _float& fX, const _float& fY);
	void		Set_TextureIndex(const _int& iIndex) { m_iTextureIndex = iIndex; }

	//Get
	const _vec3* Get_Position() const;
	 _float Get_Scale_X() const;
	 _float Get_Scale_Y() const;
public:
	HRESULT		Ready_UI();
	_int		Update_UI(const _matrix* pmatParent);
	void		Render_UI(LPD3DXEFFECT pEffect, const _matrix* pmatProj);

	HRESULT		Replace_Texture_Component(const _tchar* pComponentProtoTag);
	HRESULT		Save_UI_Info(HANDLE& hFile, _ulong& dwByte);

	
private:
	void		Check_KeyInput();
	HRESULT		Ready_Component();
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix* pmatProj);

private:
	LPDIRECT3DDEVICE9	m_pDevice;
	CTransform*			m_pTransformCom;
	CBuffer_RcTex*		m_pBufferCom;
	CTexture*			m_pTextureCom;
	CInput_Device*		m_pInput_Device;


	_int				m_iTextureIndex;
	_matrix				m_matCombinedWorld;
	
	wstring				m_wstrTextureComponentTag;

	_bool				m_bIsSelect;
	_bool				m_bIsLButtonDown;


	CUITab*				m_pUITab = nullptr;
public:
	static CUI* Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;

};


_END
#endif // UI_h__
