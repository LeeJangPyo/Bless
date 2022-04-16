#ifndef Monster_HpBar_h__
#define Monster_HpBar_h__

#include "Defines.h"
#include "GameObject.h"
_BEGIN(Engine)
class CBuffer_RcTex;
class CTexture;
class CTransform;
class CShader;
class CRenderer;
class COptimization;
_END

_BEGIN(Client)

class CMonster_HpBar final : public CGameObject
{
public:
	explicit CMonster_HpBar(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonster_HpBar(const CMonster_HpBar& rhs);
	virtual ~CMonster_HpBar() = default;

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;


public:
	void Set_HpRadio(_float fHpRadio) {
		m_fHpRadio = fHpRadio;	}
	void Set_ParentPosition(const _vec3* pParentPosition) {
		m_pParentPosition = pParentPosition;	}

	void Set_Position(const _vec3* pPosition);

private:
	HRESULT	Ready_Component();
	HRESULT	SetUp_ConstantTable_Back(LPD3DXEFFECT pEffect);
	HRESULT	SetUp_ConstantTable_Bar(LPD3DXEFFECT pEffect);
	HRESULT	SetUp_ConstantTable_Mask(LPD3DXEFFECT pEffect);
private:
	CBuffer_RcTex*		m_pBuffer_Back = nullptr;
	CBuffer_RcTex*		m_pBuffer_Bar = nullptr;
	CBuffer_RcTex*		m_pBuffer_Mask = nullptr;
	CTexture*			m_pTexture = nullptr;


	CTransform*			m_pTransform = nullptr;
	CShader*			m_pShader = nullptr;
	CRenderer*			m_pRenderer = nullptr;
	COptimization*		m_pOptimization = nullptr;

	_float				m_fHpRadio;

	const	_vec3*		m_pParentPosition = nullptr;
public:
	virtual CGameObject* Clone_GameObject() override;
	static CMonster_HpBar*	Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free() override;

};

_END
#endif // Monster_HpBar_h__