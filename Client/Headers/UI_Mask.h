#ifndef UI_Mask_h__
#define UI_Mask_h__

#include "UI.h"

_BEGIN(Client)

class CUI_Mask final : public CUI
{
public:
	explicit CUI_Mask(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CUI_Mask() = default;

public:
	void	Set_Radio(const _float& fRadio) {
		m_fRadio = fRadio;
	}
public:
	virtual	HRESULT		Ready_UI();
	virtual _int		Update_UI(const _float& fTimeDelta, const _matrix* pmatParent);
	virtual void		Render_UI(LPD3DXEFFECT pEffect, const _matrix* pmatProj);

public:
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix* pmatProj);

private:
	_float	m_fRadio;

public:
	static CUI_Mask* Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;

};

_END

#endif // UI_Mask_h__