#ifndef Direction_Arrow_h__
#define Direction_Arrow_h__

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CShader;
_END

_BEGIN(Client)
class CEffectGroup;
class CDirection_Arrow final : public CGameObject
{
public:
	explicit CDirection_Arrow(LPDIRECT3DDEVICE9 pDevice);
	explicit CDirection_Arrow(const CDirection_Arrow& rhs);
	virtual ~CDirection_Arrow() = default;

public:
	virtual HRESULT Ready_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject();

public://Set
	void	Set_Position(const _vec3* pPosition);
	void	Set_Activate(_bool bActivate) {
		m_bActivate = bActivate;	}


private:
	HRESULT Ready_Component();
private:
	CTransform*			m_pTransformCom = nullptr;
	CEffectGroup*		m_pEffect_Direction_Arrow = nullptr;

	_float				m_fFrame;
	_float				m_fTimeAcc;

	_bool				m_bActivate = false;


	const _vec3*		m_pStartPos;
	const _vec3*		m_pEndPos;
	_vec3				m_vDir;
public:
	virtual CGameObject* Clone_GameObject();
	static CDirection_Arrow*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free();
};

_END


#endif // Direction_Arrow_h__
