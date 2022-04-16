#ifndef Terrain_Stage2_h__
#define Terrain_Stage2_h__

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CBuffer_TerrainTex;
class CTexture;
class CRenderer;
class CTransform;
class CShader;
class CMesh_Navigation;
_END


_BEGIN(Client)
class CTerrain_Stage2 final : public CGameObject
{
private:
	explicit CTerrain_Stage2(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrain_Stage2(const CTerrain_Stage2& rhs);
	virtual ~CTerrain_Stage2(void) = default;

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
	CTexture*				m_pNormalTextureCom = nullptr;
	CTexture*				m_pFilterTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CBuffer_TerrainTex*		m_pBufferCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CMesh_Navigation*		m_pNaviMeshCom = nullptr;

public:
	static CTerrain_Stage2*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free() override;

};
_END


#endif // Terrain_Stage2_h__
