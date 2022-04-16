#ifndef UI_Icon_h__
#define UI_Icon_h__

#include "UI.h"

_BEGIN(Client)

class CUI_Icon final : public CUI
{
public:
	explicit CUI_Icon(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CUI_Icon() = default;

public:
	virtual	HRESULT		Ready_UI();
	virtual _int		Update_UI(const _float& fTimeDelta, const _matrix* pmatParent);
	virtual void		Render_UI(LPD3DXEFFECT pEffect,const _matrix* pmatProj);

public:
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix* pmatProj);

public:
	static CUI_Icon* Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;

};

_END

#endif // UI_Icon_h__