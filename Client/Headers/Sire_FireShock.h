#ifndef Sire_FireShock_h__
#define Sire_FireShock_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSire;
class CEffectGroup;
class CUI_Frame;
class CSire_FireShock : public CState
{
private:
	explicit CSire_FireShock();
	virtual ~CSire_FireShock() = default;

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

	CEffectGroup*	m_pEffect_FireShock= nullptr;
	CUI_Frame*		m_pUIFrame = nullptr;

	_bool			m_bPlayOnce = false;

	DAMAGE			m_Damage;
public:
	static CSire_FireShock*	Create(CSire* pSire);
protected:
	virtual void Free() override;

};

_END


#endif // Sire_FireShock_h__
