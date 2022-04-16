#ifndef Flogas_Summon_h__
#define Flogas_Summon_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CFlogas;
class CEffectGroup;
class CFlogas_Summon : public CState
{
private:
	explicit CFlogas_Summon();
	virtual ~CFlogas_Summon() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CFlogas* pFlogas);

private:
	CFlogas*		m_pFlogas = nullptr;
	CTransform*		m_pFlogas_Transform = nullptr;
	
	_bool			m_bPlayOnce_Effect = false;
	_bool			m_bPlayOnce_Summon = false;

	_float			m_fSummonAccTime;
	_float			m_fTimeAcc;

	CEffectGroup*	m_pEffect_MagicSquare = nullptr;
	CEffectGroup*	m_pSummonZone_0 = nullptr;
	CEffectGroup*	m_pSummonZone_1 = nullptr;
	CEffectGroup*	m_pSummonZone_2= nullptr;
	CEffectGroup*	m_pSummonZone_3= nullptr;
public:
	static CFlogas_Summon*	Create(CFlogas* pFlogas);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
