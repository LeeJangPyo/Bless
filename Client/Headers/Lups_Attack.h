#ifndef Lups_Attack_h__
#define Lups_Attack_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
class CInput_Device;
class CLinear_Interpolation;
_END

_BEGIN(Client)

class CLups;
class CSword;
class CShield;

class CLups_Attack : public CState
{
private:
	explicit CLups_Attack();
	virtual ~CLups_Attack() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CLups* pLups);
	void	 Check_KeyInput(const _float& fTimeDelta);
private:
	CLups*			m_pLups = nullptr;
	CSword*			m_pSword = nullptr;
	CShield*		m_pShield = nullptr;
	CLinear_Interpolation*	m_pLinear_Interpolation = nullptr;

	CInput_Device*	m_pInput_Device = nullptr;
	_int			m_iComboIndex;

public:
	static CLups_Attack*	Create(CLups* pLups);
protected:
	virtual void Free() override;

};

_END


#endif // Lups_Attack_h__
