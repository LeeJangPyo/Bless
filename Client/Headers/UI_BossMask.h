#ifndef UI_BossMask_h__
#define UI_BossMask_h__

#include "UI.h"

_BEGIN(Client)

class CUI_BossMask final : public CUI
{
public:
	explicit CUI_BossMask(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CUI_BossMask() = default;

	//set
public:
	void	Set_Radio(const _float& fRadio){
		m_fRadio = fRadio;	}
public:
	virtual	HRESULT		Ready_UI();
	virtual _int		Update_UI(const _float& fTimeDelta, const _matrix* pmatParent);
	virtual void		Render_UI(LPD3DXEFFECT pEffect, const _matrix* pmatProj);

public:
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix* pmatProj);

private:
	_float	m_fRadio;

public:
	static CUI_BossMask* Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;

};

_END

#endif // UI_BossMask_h__