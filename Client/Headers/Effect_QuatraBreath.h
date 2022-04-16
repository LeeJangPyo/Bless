#ifndef Effect_QuatraBreath_h__
#define Effect_QuatraBreath_h__


#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CCollider_Sphere;
class CCalculator;
class CShader;
class CRenderer;
_END



_BEGIN(Client)
class CEffectGroup;
class CEffect_QuatraBreath final : public CGameObject
{
public:
	explicit CEffect_QuatraBreath(LPDIRECT3DDEVICE9 pDevice);
	explicit CEffect_QuatraBreath(const CEffect_QuatraBreath& rhs);
	virtual ~CEffect_QuatraBreath() = default;

public:
	virtual HRESULT Ready_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject();


public:
	void Set_Position(const _vec3* pPosition) {
		m_vPosition = *pPosition;
		m_vPosition.y += 7.5f;	}
	void Set_Rotation(const _float& fRadian) {
		m_fRoltation = fRadian;
	}

	void Set_Activate(_bool bActivate);
	

private:
	CEffectGroup*		m_pEffect_Breath = nullptr;
	CCollider_Sphere*	m_pCollider = nullptr;
	CCalculator*		m_pCalculator = nullptr;
	CShader*			m_pShader = nullptr;
	CRenderer*			m_pRenderer = nullptr;
	
	RAY					m_Ray;

	_vec3				m_vPosition;
	_float				m_fRoltation;
	_bool				m_bActivate = false;

	_float				m_fimeAcc;
	DAMAGE				m_Damage;
public:
	virtual CGameObject* Clone_GameObject();
	static CEffect_QuatraBreath*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free();
};

_END

#endif // Effect_QuatraBreath_h__