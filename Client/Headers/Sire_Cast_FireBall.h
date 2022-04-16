#ifndef Sire_Cast_FireBall_h__
#define Sire_Cast_FireBall_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSire;
class CEffectGroup;
class CSire_Cast_FireBall : public CState
{
private:
	explicit CSire_Cast_FireBall();
	virtual ~CSire_Cast_FireBall() = default;

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

	CEffectGroup*	m_pEffect_FireBall = nullptr;
	DAMAGE			m_Damage;
public:
	static CSire_Cast_FireBall*	Create(CSire* pSire);
protected:
	virtual void Free() override;

};

_END


#endif // Sire_Cast_FireBall_h__
