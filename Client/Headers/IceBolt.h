#ifndef IceBolt_h__
#define IceBolt_h__

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CMesh_Static;
class CRenderer;
class CTexture;
class CShader;
class CCollider_Sphere;
_END

_BEGIN(Client)
class CEffectGroup;

class CIceBolt final : public CGameObject
{
public:
	explicit CIceBolt(LPDIRECT3DDEVICE9 pDevice);
	explicit CIceBolt(const CIceBolt& rhs);
	virtual ~CIceBolt() = default;

public:
	//void	Set_TargetPosition(const _vec3* pTargetPosition);
	//void	Set_Position(const _vec3* pPosition);
public:
	virtual HRESULT Ready_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject();

public:
	HRESULT		SetUp_IceBoltInfo(const _vec3* pTargetPosition, const _vec3* pPosition);
private:
	HRESULT		Ready_Component();
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	

private:
	CTransform*			m_pTransformCom;
	CMesh_Static*		m_pMeshCom;
	CRenderer*			m_pRendererCom;
	CShader*			m_pShaderCom;
	CCollider_Sphere*	m_pColliderCom;
	CTexture*			m_pTextureCom;

	_vec3				m_vTargetDirection;
	_float				m_fMoveSpeed;
	_float				m_fScale;


	DAMAGE				m_Damage;

	_float				m_fLifeTime;
	_float				m_fTimeAcc;

	CEffectGroup*		m_pEffect_IceParticle;
public:
	virtual CGameObject* Clone_GameObject();
	static CIceBolt*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free();
};

_END



#endif // IceBolt_h__
