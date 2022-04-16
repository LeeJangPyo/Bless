#ifndef Sire_Idle_h__
#define Sire_Idle_h__

#include "State.h"


_BEGIN(Engine)
class CTransform;
class CInput_Device;
_END

_BEGIN(Client)
class CSire;
class CSire_Idle : public CState
{
private:
	explicit CSire_Idle();
	virtual ~CSire_Idle() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	

private:
	void	 Check_KeyInput(const _float& fTimeDelta);
private:
	HRESULT	 Ready_State(CSire* pSire);


private:
	CSire*			m_pSire;
	CInput_Device*	m_pInput_Device;
public:
	static CSire_Idle*	Create(CSire* pSire);
protected:
	virtual void Free() override;

};

_END


#endif // Sire_Idle_h__
