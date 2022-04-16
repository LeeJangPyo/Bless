#ifndef UI_IconMask_h__
#define UI_IconMask_h__

#include "UI.h"

_BEGIN(Client)

class CUI_IconMask final : public CUI
{
public:
	explicit CUI_IconMask(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CUI_IconMask() = default;

public:
	void	Set_CoolDown(const _float& fCoolDown){
		m_fCoolDown = fCoolDown;	}
	void	Set_IsCoolDown(_bool bIsCoolDown) {
		m_bIsCoolDown = bIsCoolDown;	}

public:
	virtual	HRESULT		Ready_UI();
	virtual _int		Update_UI(const _float& fTimeDelta, const _matrix* pmatParent);
	virtual void		Render_UI(LPD3DXEFFECT pEffect, const _matrix* pmatProj);

public:
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix* pmatProj);

private:
	_float	m_fRadio;
	_float	m_fTimeAcc;
	
	_float	m_fCoolDown;
	_bool	m_bIsCoolDown;

public:
	static CUI_IconMask* Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;

};

_END

#endif // UI_IconMask_h__