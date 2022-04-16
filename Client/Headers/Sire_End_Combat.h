#ifndef Sire_End_Combat_h__
#define Sire_End_Combat_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSire;
class CSire_End_Combat : public CState
{
private:
	explicit CSire_End_Combat();
	virtual ~CSire_End_Combat() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CSire* pSire);

private:
	CSire*	m_pSire;

public:
	static CSire_End_Combat*	Create(CSire* pSire);
protected:
	virtual void Free() override;

};

_END


#endif // Sire_End_Combat_h__
