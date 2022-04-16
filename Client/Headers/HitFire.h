#ifndef HitFire_h__
#define HitFire_h__

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CShader;
class CBuffer_RcTex;
_END

_BEGIN(Client)

class CHitFire final : public CGameObject
{
public:
	explicit CHitFire(LPDIRECT3DDEVICE9 pDevice);
	explicit CHitFire(const CHitFire& rhs);
	virtual ~CHitFire() = default;

public:
	virtual HRESULT Ready_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject();

public://Set
	void	Set_Position(const _vec3* pPosition);
private:
	HRESULT		Ready_Component();
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	CTransform*			m_pTransformCom = nullptr;
	CBuffer_RcTex*		m_pBufferCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	
	_int				m_iWidth;
	_int				m_iHeight;

	_int				m_iAnimationX;
	_int				m_iAnimationY;

	_float				m_fFrame;
	_float				m_fTimeAcc;

public:
	virtual CGameObject* Clone_GameObject();
	static CHitFire*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free();
};

_END


#endif // HitFire_h__
