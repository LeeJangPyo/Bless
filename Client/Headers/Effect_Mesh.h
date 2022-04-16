#ifndef Effect_Mesh_h__
#define Effect_Mesh_h__

#include "Effect.h"

_BEGIN(Engine)
class CMesh_Static;
class CTexture;
_END


_BEGIN(Client)

class CEffect_Mesh final : public CEffect
{
public:
	explicit CEffect_Mesh(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEffect_Mesh() = default;

public:
	HRESULT SetUp_EffectInfo(const _tchar* pComponentMeshTag);
	void	SetUp_EffectData(const EFFECTDATA& EffectData, const EFFECTMESH_DATA& Effect_MeshData);
public:
	virtual HRESULT		Ready_Effect() override;
	virtual _int		Update_Effect(const _float& fTimeDelta) override;
	virtual _int		LastUpdate_Effect(const _float& fTimeDelta) override;
	virtual void		Render_Effect(LPD3DXEFFECT pEffects, const _matrix* pmatParent) override;
	virtual void		Initalize_Effect() override;

private:
	CMesh_Static*	m_pMeshCom;
	EFFECTMESH_DATA		m_EffectMesh_Data;

public:
	static	CEffect_Mesh*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;

};

_END

#endif // Effect_Mesh_h__


