#ifndef Balrog_Fire_Ring_h__
#define Balrog_Fire_Ring_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CBalrog;
class CBalrog_Fire_Ring : public CState
{
private:
	explicit CBalrog_Fire_Ring();
	virtual ~CBalrog_Fire_Ring() = default;

public:

	// CState��(��) ���� ��ӵ�
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
	static CBalrog_Fire_Ring*	Create(CBalrog* pBalrog);
protected:
	virtual void Free() override;

};

_END


#endif // Balrog_Fire_Ring_h__