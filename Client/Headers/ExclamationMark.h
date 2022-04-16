#ifndef ExclamationMark_h__
#define ExclamationMark_h__

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CShader;
_END

_BEGIN(Client)
class CEffectGroup;
class CExclamationMark final : public CGameObject
{
public:
	explicit CExclamationMark(LPDIRECT3DDEVICE9 pDevice);
	explicit CExclamationMark(const CExclamationMark& rhs);
	virtual ~CExclamationMark() = default;

public:
	virtual HRESULT Ready_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject();

public://Set
	void	Set_Position(const _vec3* pPosition);
	void	Set_Transform( CTransform* pTransform);
	void	Set_Activate(_bool bActivate) {
		m_bActivate = bActivate;
	}


private:
	HRESULT Ready_Component();
private:
	CTransform*			m_pTransformCom = nullptr;
	CTransform*			m_pMonsterTransform = nullptr;
	CEffectGroup*		m_pEffect_ExclamationMark = nullptr;

	_bool				m_bActivate = false;

	_float				m_fimeAcc;
public:
	virtual CGameObject* Clone_GameObject();
	static CExclamationMark*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free();
};

_END


#endif // ExclamationMark_h__
