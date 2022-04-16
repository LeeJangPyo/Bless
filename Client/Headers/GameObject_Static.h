#ifndef Stone_h__
#define Stone_h__

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CCalculator;
class CMesh_Static;
class CShader;
class COptimization;
_END

_BEGIN(Client)

class CGameObject_Static : public CGameObject
{
public:
	explicit	CGameObject_Static(LPDIRECT3DDEVICE9 pDevice);
	explicit	CGameObject_Static(const CGameObject_Static& rhs);
	virtual		~CGameObject_Static() = default;
	
public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;


	HRESULT			SetUp_GameObjectInfo(const _matrix* pMatrix,const _tchar* pComponentMeshTag);
public:
	virtual CGameObject* Clone_GameObject() override;
	static	CGameObject* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT		Ready_Component();
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect);
protected:
	CTransform*			m_pTransformCom	=nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CMesh_Static*		m_pMeshCom = nullptr;
//	CCollider_Box*		m_pColliderCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	COptimization*		m_pOptimizationCom = nullptr;

protected:
	virtual void Free() override;


};

_END

#endif // Stone_h__
