#ifndef Balrog_Chase_h__
#define Balrog_Chase_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CBalrog;
class CBalrog_Chase : public CState
{
private:
	explicit CBalrog_Chase();
	virtual ~CBalrog_Chase() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual	_int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CBalrog* pBalrog);

private:
	CBalrog*		m_pBalrog;
	CTransform*			m_pPlayerTransform;
	CTransform*			m_pTransform;

public:
	static CBalrog_Chase*	Create(CBalrog* pBalrog);
protected:
	virtual void Free() override;

};

_END


#endif // Balrog_Chase_h__
