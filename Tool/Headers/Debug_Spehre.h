
#ifndef Debug_Spehre_h__
#define Debug_Spehre_h__

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CShader;
class CTexture;
_END

_BEGIN(Client)
class CDebug_Spehre final : public CGameObject
{
public:
	explicit CDebug_Spehre(LPDIRECT3DDEVICE9 pDevice);
	explicit CDebug_Spehre(const CDebug_Spehre& rhs);
	virtual ~CDebug_Spehre() = default;

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	void Set_Position(const _vec3* pPosition);
	void Set_Activate(_bool bActivate) {
		m_bActivate = bActivate;	}
	void Set_Pick(_bool bPick) {
		m_bPick = bPick;	}
private:
	HRESULT	Ready_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CShader*		m_pShader = nullptr;
	CTexture*		m_pTexture = nullptr;

	LPD3DXMESH		m_pMesh_Sphere = nullptr;

	_bool			m_bActivate;
	_bool			m_bPick;
public:
	virtual CGameObject* Clone_GameObject() override;
	static CDebug_Spehre*	Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	virtual void Free() override;

};

_END

#endif // Debug_Spehre_h__