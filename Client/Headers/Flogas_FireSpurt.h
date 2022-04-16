#ifndef Flogas_FireSpurt_h__
#define Flogas_FireSpurt_h__

#include "State.h"

_BEGIN(Client)
class CFlogas;
class CEffectGroup;
class CCamera_Static;
class CFlogas_FireSpurt : public CState
{
private:
	explicit CFlogas_FireSpurt();
	virtual ~CFlogas_FireSpurt() = default;

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
	CEffectGroup*	m_pEffect_FireSpurt = nullptr;
	CCamera_Static* m_pCamera_Static = nullptr;

	_bool			m_bPlayOnce;
	DAMAGE			m_Damage;

public:
	static CFlogas_FireSpurt*	Create(CFlogas* pFlogas);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
