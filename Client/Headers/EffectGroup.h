#ifndef EffectGroup_h__
#define EffectGroup_h__

#include "Defines.h"
#include "GameObject.h"

#include "Effect_Mesh.h"
#include "Effect_Rect.h"
#include "Effect_Particle.h"



_BEGIN(Engine)
class CTransform;
class CShader;
class CRenderer;
class CCollider_Sphere;
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

	typedef struct tagMeshData 
	{
		EFFECTDATA		Effect_Data;
		EFFECTMESH_DATA	Effect_MeshData;
	}MESHDATA;
	typedef struct tagRectData
	{
		EFFECTDATA		Effect_Data;
		EFFECTRECT_DATA Effect_RectData;
	}RECTDATA;
	typedef struct tagParticleData
	{
		EFFECTDATA			Effect_Data;
		EFFECTPARTICLE_DATA	Effect_ParticleData;
		_bool				bReCycle;

	}PARTICLEDATA;
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
	void		Set_DeadObject(_bool bDeadObject) {
		m_bDeadObject = bDeadObject;	}
	void		Set_Tag(const _tchar* pTag){
		lstrcpy(m_EffectrGroup_Data.szTag, pTag);	}
	void		Set_LifeTime(const _float&	fLifeTime) {
		m_EffectrGroup_Data.fLifeTime = fLifeTime;	}
	HRESULT		Set_ParentMatrix(const _tchar * pLayerTag, const _char * FrameTag);
	//Transform
	void	Set_Move(_bool bMove, const _float& fMoveSpeed, const _vec3* vDirection);
	void	Set_Damage(const DAMAGE* pDamage) {
		m_Damage = *pDamage;	}
	void		Set_EffectType(_uint iEffectType) {
		m_iEffecType = iEffectType;	}


	void		Set_Position(const _vec3* pPosition);
	void		RotationX(const _float& fRotation);
	void		RotationY(const _float& fRotation);
	void		RotationY(const _float& fRotation, const _float& fTimeDelta);
	void		RotationZ(const _float& fRotation);
	void		Set_RotationY(const _float& fRadian);
	void		Initalize_EffectRotation();

	void		Initalize_EffectGroup();

	HRESULT		Add_Effect_Mesh(CEffect_Mesh* pEffect_Mesh);
	HRESULT		Add_Effect_Rect(CEffect_Rect * pEffect_Rect);
	HRESULT		Add_Effect_Paritcle(CEffect_Particle * pEffect_Particle);

	CEffect_Mesh*		Get_Effect_Mesh(_uint iIndex);
	CEffect_Rect*		Get_Effect_Rect(_uint iIndex);
	CEffect_Particle*	Get_Effect_Particle(_uint iIndex);

	void	Load_EffectDataFromFile(HANDLE & hFile, _ulong & dwByte);

	HRESULT	SetUp_ColliderInfo(const _float& fRadius, COLLISION_TARGET eTarget, 
		const _float& CollisionStartTime, const _float& CollisionEndTime);



	_float			Get_ProgressRadio();
	const _vec3*	Get_Position();
	CTransform*		Get_Transform();
	
private:
	HRESULT	Ready_Component();
	
private:
	vector<CEffect_Mesh*>		m_vecEffectMesh;
	vector<CEffect_Rect*>		m_vecEffectRect;
	vector<CEffect_Particle*>	m_vecEffectParticle;
	

	//
	_int				m_iMeshEffectNum;
	_int				m_iRectEffectNum;
	_int				m_iParticleEffectNum;

	vector<MESHDATA>			m_vecMeshData;
	vector<RECTDATA>			m_vecRectData;
	vector<PARTICLEDATA>		m_vecParticleData;

	

	//Component
	CTransform*			m_pTransformCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*	m_pColliderCom = nullptr;
	
	_bool				m_bActivate;
	_bool				m_bDeadObject;

	//Damage
	COLLISION_TARGET	m_eTarget;
	_float				m_fCollision_StartTime;
	_float				m_fCollision_EndTime;
	DAMAGE				m_Damage;
	_uint				m_iEffecType;
	_bool				m_bHitOnce;

	//For. Move
	_bool			m_bMove;
	_float			m_fMoveSpeed;
	_vec3			m_vDirection;
	

	_float			m_fTimeAcc;

	//Effcet
	EFFECTGROUP_DATA	m_EffectrGroup_Data;

	//Parent
	_matrix	m_matParentMatrix;

	const _matrix*				m_pParentBoneMatrix = nullptr;
	const _matrix*				m_pParentWorldMatrix = nullptr;
public:
	virtual CGameObject* Clone_GameObject() override;
	static CEffectGroup*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;

};


_END


#endif // EffectGroup_h__