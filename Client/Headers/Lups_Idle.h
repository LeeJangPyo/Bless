#ifndef Lups_Idle_h__
#define Lups_Idle_h__

#include "State.h"


_BEGIN(Engine)
class CTransform;
class CInput_Device;
_END

_BEGIN(Client)
class CLups;
class CLups_Idle : public CState
{
private:
	explicit CLups_Idle();
	virtual ~CLups_Idle() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	

private:
	void	 Check_KeyInput(const _float& fTimeDelta);
private:
	HRESULT	 Ready_State(CLups* pLups);
private:
	CLups*			m_pLups = nullptr;
	CInput_Device*	m_pInput_Device = nullptr;
public:
	static CLups_Idle*	Create(CLups* pLups);
protected:
	virtual void Free() override;

};

_END


#endif // Lups_Idle_h__
