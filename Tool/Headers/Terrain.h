#ifndef Terrain_h__
#define Terrain_h__

#include "Defines.h"

#include "GameObject.h"


_BEGIN(Engine)
class CBuffer_TerrainTex;
class CTexture;
class CRenderer;
class CTransform;
class CShader;
_END

_BEGIN(Client)

class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain(void) = default;

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

private:
	HRESULT		Ready_Component();
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CBuffer_TerrainTex*		m_pBufferCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

public:
	static CTerrain*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free() override;

public:

};


_END
#endif // Terrain_h__