#ifndef Flogas_Earthquake_h__
#define Flogas_Earthquake_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CEffectGroup;
class CCamera_Static;
class CFlogas;
class CFlogas_Earthquake : public CState
{
private:
	explicit CFlogas_Earthquake();
	virtual ~CFlogas_Earthquake() = default;

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
	CEffectGroup*	m_pEffect_Earthquake = nullptr;
	CCamera_Static* m_pCamera_Static = nullptr;

	DAMAGE			m_Damage;
	_bool			m_bPlayOnce;
public:
	static CFlogas_Earthquake*	Create(CFlogas* pFlogas);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
