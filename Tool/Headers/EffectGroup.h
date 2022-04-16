#ifndef EffectGroup_h__
#define EffectGroup_h__

#include "GameObject.h"

#include "Effect_Mesh.h"
#include "Effect_Rect.h"
#include "Effect_Particle.h"

class CEffectTab;

_BEGIN(Engine)
class CTransform;
class CShader;
class CRenderer;
_END

_BEGIN(Client)

class CEffectGroup final : public CGameObject
{
public:
	typedef struct tagEffectGroup
	{
		_tchar			szTag[MIN_PATH];
		_float			fLifeTime;
	}EFFECTGROUP_DATA;
public:
	explicit CEffectGroup(LPDIRECT3DDEVICE9 pDevice);
	explicit CEffectGroup(const CEffectGroup& rhs);
	virtual ~CEffectGroup() = default;

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;


public:

	void		Set_Activate(_bool	bActivate);
	void		Set_Tag(const _tchar* pTag){
		lstrcpy(m_EffectrGroup_Data.szTag, pTag);	}
	void		Set_LifeTime(const _float&	fLifeTime){
		m_EffectrGroup_Data.fLifeTime = fLifeTime;	}

	HRESULT		Add_Effect_Mesh(CEffect_Mesh* pEffect_Mesh);
	HRESULT		Add_Effect_Rect(CEffect_Rect * pEffect_Rect);
	HRESULT		Add_Effect_Paritcle(CEffect_Particle * pEffect_Particle);

	CEffect_Mesh*		Get_Effect_Mesh(_uint iIndex);
	CEffect_Rect*		Get_Effect_Rect(_uint iIndex);
	CEffect_Particle*	Get_Effect_Particle(_uint iIndex);

	void	Save_EffectData(HANDLE & hFile, _ulong & dwByte);
	void	Load_EffectDataFromFile(HANDLE & hFile, _ulong & dwByte, CEffectTab* pEffectTab);

	void	Remove_EffectMesh(const _uint& iIndex);
	void	Remove_EffectRect(const _uint& iIndex);
	void	Remove_EffectParticle(const _uint& iIndex);
private:
	HRESULT	Ready_Component();
	
private:
	vector<CEffect_Mesh*>		m_vecEffectMesh;
	vector<CEffect_Rect*>		m_vecEffectRect;
	vector<CEffect_Particle*>	m_vecEffectParticle;

	
	CTransform*		m_pTransformCom;
	CShader*		m_pShaderCom;
	CRenderer*		m_pRendererCom;

	_bool			m_bActivate;
	_float			m_fTimeAcc;

	EFFECTGROUP_DATA	m_EffectrGroup_Data;

public:
	virtual CGameObject* Clone_GameObject() override;
	static CEffectGroup*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;

};


_END


#endif // EffectGroup_h__