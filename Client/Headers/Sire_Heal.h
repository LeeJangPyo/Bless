#ifndef Sire_Heal_h__
#define Sire_Heal_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSire;
class CEffectGroup;
class CUI_Frame;
class CSire_Heal : public CState
{
private:
	explicit CSire_Heal();
	virtual ~CSire_Heal() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;

private:
	HRESULT	 Ready_State(CSire* pSire);

private:
	CSire*	m_pSire = nullptr;

	CEffectGroup*		m_pEffect_Heal = nullptr;

	DAMAGE				m_Damage;

public:
	static CSire_Heal*	Create(CSire* pSire);
protected:
	virtual void Free() override;

};

_END


#endif // Sire_Heal_h__
