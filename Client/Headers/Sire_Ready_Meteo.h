#ifndef Sire_Ready_Meteo_h__
#define Sire_Ready_Meteo_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSire;
class CEffectGroup;
class CSire_Ready_Meteo : public CState
{
private:
	explicit CSire_Ready_Meteo();
	virtual ~CSire_Ready_Meteo() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CSire* pSire);

private:
	CSire*	m_pSire;

	CEffectGroup*			m_pEffect_FireOrb = nullptr;
	CEffectGroup*			m_pEffect_MagicZone = nullptr;

	_float		m_fTimeAcc;

	_float		m_fCastingTime;
public:
	static CSire_Ready_Meteo*	Create(CSire* pSire);
protected:
	virtual void Free() override;

};

_END


#endif // Sire_Ready_Meteo_h__
