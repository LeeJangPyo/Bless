#ifndef Sire_Ready_FireBall_h__
#define Sire_Ready_FireBall_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSire;
class CEffectGroup;
class CUI_Frame;
class CSire_Ready_FireBall : public CState
{
private:
	explicit CSire_Ready_FireBall();
	virtual ~CSire_Ready_FireBall() = default;

public:

	// CState��(��) ���� ��ӵ�
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CSire* pSire);

private:
	CSire*	m_pSire = nullptr;

	CEffectGroup*			m_pEffect_FireOrb = nullptr;
	CEffectGroup*			m_pEffect_MagicZone = nullptr;
	CUI_Frame*				m_pUIFrame = nullptr;

	_float		m_fTimeAcc;
	_float		m_fCastingTime;
public:
	static CSire_Ready_FireBall*	Create(CSire* pSire);
protected:
	virtual void Free() override;

};

_END


#endif // Sire_Ready_FireBall_h__
