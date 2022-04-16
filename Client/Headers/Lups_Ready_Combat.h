#ifndef Lups_Ready_Combat_h__
#define Lups_Ready_Combat_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CLups;
class CSword;
class CShield;
class CLups_Ready_Combat : public CState
{
private:
	explicit CLups_Ready_Combat();
	virtual ~CLups_Ready_Combat() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CLups* pLups);

private:
	CLups*		m_pLups = nullptr;
	CSword*		m_pSword = nullptr;
	CShield*	m_pShield = nullptr;
	
public:
	static CLups_Ready_Combat*	Create(CLups* pLups);
protected:
	virtual void Free() override;

};

_END


#endif // Lups_Ready_Combat_h__
