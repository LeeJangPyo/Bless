#ifndef Sire_Hit_h__
#define Sire_Hit_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
class CInput_Device;
_END

_BEGIN(Client)
class CSire;
class CSire_Hit : public CState
{
private:
	explicit CSire_Hit();
	virtual ~CSire_Hit() = default;

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CSire* pSire);
	void	Check_KeyInput(const _float& fTimeDelta);

private:
	CSire*			m_pSire = nullptr;
	CInput_Device*	m_pInput_Device = nullptr;
public:
	static CSire_Hit*	Create(CSire* pSire);
protected:
	virtual void Free() override;

};

_END


#endif // Sire_Hit_h__
