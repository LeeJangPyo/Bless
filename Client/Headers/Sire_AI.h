#ifndef Sire_AI_h__
#define Sire_AI_h__

#include "State.h"


_BEGIN(Engine)
class CTransform;
class CInput_Device;
_END

_BEGIN(Client)
class CSire;
class CSire_AI : public CState
{
private:
	explicit CSire_AI();
	virtual ~CSire_AI() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	

private:
	HRESULT	 Ready_State(CSire* pSire);


private:
	CSire*			m_pSire;
	CTransform*		m_pSire_Transform;
	CInput_Device*	m_pInput_Device;
public:
	static CSire_AI*	Create(CSire* pSire);
protected:
	virtual void Free() override;

};

_END


#endif // Sire_AI_h__
