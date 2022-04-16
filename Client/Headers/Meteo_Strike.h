
#ifndef MeteoStrike_h__
#define MeteoStrike_h__


#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
_END



_BEGIN(Client)
class CEffectGroup;
class CMeteo_Strike final : public CGameObject
{
public:
	explicit CMeteo_Strike(LPDIRECT3DDEVICE9 pDevice);
	explicit CMeteo_Strike(const CMeteo_Strike& rhs);
	virtual ~CMeteo_Strike() = default;

public:
	virtual HRESULT Ready_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject();

private:

	CEffectGroup*		m_pEffect_Meteo_Strike = nullptr;
	CEffectGroup*		m_pEffect_Meteo_Explosion = nullptr;

	_bool				m_bActivate = false;

	_float				m_fimeAcc;
	DAMAGE				m_Damage;
public:
	virtual CGameObject* Clone_GameObject();
	static CMeteo_Strike*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free();
};

_END
#endif // Meteo_Strike_h__
