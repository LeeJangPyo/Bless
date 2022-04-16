#ifndef Sire_DownAndUp_h__
#define Sire_DownAndUp_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSire;
class CSire_DownAndUp : public CState
{
private:
	explicit CSire_DownAndUp();
	virtual ~CSire_DownAndUp() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CSire* pSire);

private:
	CSire*				m_pSire;
	CTransform*			m_pTransform;

	_int				m_iAnimationIndex;

public:
	static CSire_DownAndUp*	Create(CSire* pSire);
protected:
	virtual void Free() override;

};

_END


#endif // Sire_DownAndUp_h__
