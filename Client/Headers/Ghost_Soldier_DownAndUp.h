#ifndef Ghost_Soldier_DownAndUp_h__
#define Ghost_Soldier_DownAndUp_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CGhost_Soldier;
class CGhost_Soldier_DownAndUp : public CState
{
private:
	explicit CGhost_Soldier_DownAndUp();
	virtual ~CGhost_Soldier_DownAndUp() = default;

public:

	// CState��(��) ���� ��ӵ�
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CGhost_Soldier* pGhost_Soldier);

private:
	CGhost_Soldier*		m_pGhost_Solider;
	CTransform*			m_pTransform;

	_int				m_iAnimationIndex;

public:
	static CGhost_Soldier_DownAndUp*	Create(CGhost_Soldier* pGhost_Soldier);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_DownAndUp_h__
