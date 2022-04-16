#ifndef Damage_h__
#define Damage_h__

#include "Defines.h"

#include "GameObject.h"


_BEGIN(Engine)
class CTexture;
class CBuffer_RcTex;
class CTransform;
class CRenderer;
class CShader;
_END

_BEGIN(Client)
class CDamage final : public CGameObject
{
public:
	enum DAMAGE_COLOR
	{
		RED, GREEN, WHITE
	};
public:
	explicit CDamage(LPDIRECT3DDEVICE9 pDevice);
	explicit CDamage(const CDamage& rhs);
	virtual ~CDamage() = default;


public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;

public:
	HRESULT		SetUp_DamageInfo(const _int& iDamage,const _vec3* pPosition,const DAMAGE_COLOR& eColor);
private:
	HRESULT		Ready_Component();
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect);


private:
	LPDIRECT3DDEVICE9	m_pDevice;
	CTransform*			m_pTransformCom;
	CBuffer_RcTex*		m_pBufferCom;
	CTexture*			m_pTextureCom;
	CRenderer*			m_pRendererCom;
	CShader*			m_pShaderCom;
	
	_int				m_iTextureIndex;
	_int				m_iDamege;
	//place
	_int				m_iUnits;
	_int				m_iTens;
	_int				m_iHundreds;
	_int				m_iThousands;


	_bool				m_bThousands;

	_float				m_fTimeAcc;
	_vec3				m_vPosition;


	DAMAGE_COLOR		m_eColor;
public:
	static CDamage* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;

protected:
	virtual void Free() override;


};


_END
#endif // Damage_h__
