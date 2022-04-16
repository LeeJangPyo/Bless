#ifndef Ghost_Mage_Dead_h__
#define Ghost_Mage_Dead_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CGhost_Mage;
class CGhost_Mage_Dead : public CState
{
private:
	explicit CGhost_Mage_Dead();
	virtual ~CGhost_Mage_Dead() = default;

public:

	// CState��(��) ���� ��ӵ�
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CGhost_Mage* pGhost_Mage);

private:
	CGhost_Mage*		m_pGhost_Mage;
	CTransform*			m_pTransform;

	_float				m_fWaitTime;
	_float				m_fTimeAcc;
	_bool				m_bIsAnimationEnd;
public:
	static CGhost_Mage_Dead*	Create(CGhost_Mage* pGhost_Mage);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Mage_Dead_h__
