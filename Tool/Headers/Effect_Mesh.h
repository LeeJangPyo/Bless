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

public:
	explicit CEffect_Mesh(LPDIRECT3DDEVICE9 pDevice);
//	explicit CEffect_Mesh(const CEffect_Mesh& rhs);
	virtual ~CEffect_Mesh() = default;

public:

	HRESULT SetUp_EffectInfo(const _tchar* pComponentMeshTag, CEffectTab* pEffectTab);
	void	Set_MeshTag(const _tchar* pTag) {
		lstrcpy(m_EffectMesh_Data.szMeshTag, pTag);	}

public:
	virtual HRESULT		Ready_Effect() override;
	virtual _int		Update_Effect(const _float& fTimeDelta) override;
	virtual _int		LastUpdate_Effect(const _float& fTimeDelta) override;
	virtual void		Render_Effect(LPD3DXEFFECT pEffects) override;
//	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect);

	void		Save_EffectData(HANDLE& hFile, _ulong& dwByte);
	void		Load_EffectDataFromFile(HANDLE& hFile, _ulong& dwByte, CEffectTab* pEffectTab);
	
	HRESULT		SetUp_EffectDataFromFile(EFFECTDATA& EffectData, EFFECTMESH_DATA& EffectMesh_Data, CEffectTab* pEffectTab);
private:
	HRESULT Ready_Component();

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


