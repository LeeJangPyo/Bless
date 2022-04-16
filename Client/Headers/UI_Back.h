#ifndef UI_Back_h__
#define UI_Back_h__

#include "UI.h"
_BEGIN(Client)

class CUI_Back final : public CUI
{
public:
	explicit CUI_Back(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CUI_Back() = default;

public:
	virtual	HRESULT		Ready_UI() override;
	virtual _int		Update_UI(const _float& fTimeDelta, const _matrix* pmatParent) override;
	virtual void		Render_UI(LPD3DXEFFECT pEffect, const _matrix* pmatProj) override;

public:
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix* pmatProj) override;

public:
	static CUI_Back* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	virtual void Free() override;
};

_END

#endif // UI_Back_h__
