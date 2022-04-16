#ifndef Balrog_Electric_Whip_h__
#define Balrog_Electric_Whip_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CBalrog;
class CBalrog_Electric_Whip : public CState
{
private:
	explicit CBalrog_Electric_Whip();
	virtual ~CBalrog_Electric_Whip() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CBalrog* pBalrog);

private:
	CBalrog*		m_pBalrog;
	CTransform*			m_pPlayerTransform;
	CTransform*			m_pTransform;

public:
	static CBalrog_Electric_Whip*	Create(CBalrog* pBalrog);
protected:
	virtual void Free() override;

};

_END


#endif // Balrog_Electric_Whip_h__
