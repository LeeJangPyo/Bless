#ifndef Animation_Controller_h__
#define Animation_Controller_h__


#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL CAnimation_Controller : public CBase
{
private:
	explicit	CAnimation_Controller(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit	CAnimation_Controller(const CAnimation_Controller& rhs);
	virtual		~CAnimation_Controller(void);


public:
	LPD3DXANIMATIONCONTROLLER	Get_Animation_Controller(void) { return m_pAniCtrl; }
	_bool						Is_AnimationSetEnd(void);
	_bool						Is_AnimationPassRadio(const _float&);
	_bool						Is_AnimationInRange(const _float& fStart, const _float& fEnd);
	_uint						Get_MaxNumAnimation(void);

public:
	HRESULT		Ready_Animation_Controller(void);
	void		Set_AnimationSet(const _uint& iIndex, _bool bResetAnimation = false);
	void		Reset_AnimationSet();
	void		Play_Animation(const _float& fTimeDelta);

private:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl;
	_uint							m_iCurrentTrack;
	_uint							m_iNewTrack;
	_uint							m_iOldAniIndex;
	_float							m_fAccTime;
	_double							m_dPeriod;

public:
	static		CAnimation_Controller*			Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static		CAnimation_Controller*			Create(const CAnimation_Controller& rhs);
	virtual		void					Free(void);

};
_END


#endif // Animation_Controller_h__
