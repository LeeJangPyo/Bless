#ifndef UI_Bar_h__
#define UI_Bar_h__

#include "UI.h"

_BEGIN(Client)

class CUI_Bar final : public CUI
{
public:
	explicit CUI_Bar(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CUI_Bar() = default;

public:
	virtual	HRESULT		Ready_UI() override;
	virtual _int		Update_UI(const _float& fTimeDelta, const _matrix* pmatParent) override;
	virtual void		Render_UI(LPD3DXEFFECT pEffect, const _matrix* pmatProj) override;

public:
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix* pmatProj) override;


public:
	static CUI_Bar* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	_float				m_fTimeAcc;
protected:
	virtual void Free() override;
};

_END


#endif // UI_Bar_h__
