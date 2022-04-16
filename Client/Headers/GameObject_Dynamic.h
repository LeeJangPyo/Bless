#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"


_BEGIN(Engine)
class CTransform;
class CRenderer;
class CCalculator;
class CCollider_Sphere;
class CMesh_Dynamic;
class CShader;
class CState;
class COptimization;
class CMesh_Navigation;
_END

_BEGIN(Client)
class CCollision_Manager;
class CGameObject_Dynamic abstract : public CGameObject
{

public:
	explicit CGameObject_Dynamic(LPDIRECT3DDEVICE9 pDevice);
	explicit CGameObject_Dynamic(const CGameObject_Dynamic& rhs);
	virtual ~CGameObject_Dynamic() = default;

public:
	//Set
	void	Set_AnimationSet(const _uint& iIndex, _bool bResetAnimation = false);
	void	Set_FrameConst(const _float& fFrameConst) {
		m_fFrameConst = fFrameConst;	}
	void	Set_NaviIndex(const _ulong& iIndex){
		m_dwCurrentNaviIndex = iIndex;	}
	void	Set_RimLight(const _float& fRimLightWidth) {
		m_fLimRightWidth = fRimLightWidth;	}

	void	Reset_AnimationSet();

	//Get
	list<CCollider_Sphere*>*	Get_ColliderList();
	_float						Get_LongestRadius();
	CTransform*					Get_Transform();
	_bool						Get_IsAnimationSetEnd();
	_bool						Get_IsAnimationPassRadio(const _float& fRadio);
	_bool						Get_IsAnimationRange(const _float& fStart, const _float& fEnd);
	_ulong						Get_NaviOnIndex() {
		return m_dwCurrentNaviIndex;	}

	const DAMAGE*				Get_Damage() {
		return &m_Damage;	}
	const BASE_STATS*			Get_BaseStats() {
		return &m_BaseStats;	}

	const _int&					Get_CurrentHp() {
		return m_BaseStats.iCurrentHp;	}


public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;


	virtual HRESULT		SetUp_GameObjectInfo(const _matrix* pMatrix);
	virtual	void		Damaged(const _int& iDamage, const _uint& iEffect_Type, const _vec3* vEffect_Position) = 0;
public:
	HRESULT		Change_NavigationMesh();
	HRESULT		Change_State(const _tchar* pStateTag);
	_bool		Compare_State(const _tchar* pStateTag);

	void		Go_Target(const _vec3* pTargetPos, const _float& fTimeDelta);
	_bool		Check_Collision(CCollider_Sphere* pOtherCollider, _vec3* Collision_Position = nullptr);
	_bool		Check_CollsionAndPush(CGameObject_Dynamic* pOtherObject);
	_vec3		Move_OnNaviMesh(const _vec3* pTargetPos, const _float& fTimeDelta);
	_vec3		Stand_OnNaviMesh(const _vec3* pPosition);
	_bool		Compute_Position_OnNaviMeshIndex();
protected:
	HRESULT		Ready_Component();
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect);


protected:
	HRESULT		Load_ColliderInfoFromFile(const _tchar* pFilePath);
protected:
	CTransform*					m_pTransformCom		= nullptr;
	CRenderer*					m_pRendererCom		= nullptr;
	CCalculator*				m_pCalculatorCom	= nullptr;
	CMesh_Dynamic*				m_pMeshCom			= nullptr;
	CShader*					m_pShaderCom		 = nullptr;
	CMesh_Navigation*			m_pNaviMeshCom		 = nullptr;
	COptimization*				m_pOptimizationCom = nullptr;

	//Collider
	list<CCollider_Sphere*>		m_ColliderList;
	
	//NaviMesh
	_ulong						m_dwCurrentNaviIndex;

	//Stage
	CState*						m_pCurrentState = nullptr;
	map<const _tchar*, CState*>	m_mapState;

	//Stats
	BASE_STATS					m_BaseStats;
	DAMAGE						m_Damage;

	//Frame
	_float						m_fFrameConst;


	//RimLight Range
	_float						m_fLimRightWidth;

	//hit
	_float						m_fHitTimeAcc;
	_float						m_fHitDelay;
public:
	virtual CGameObject* Clone_GameObject() = 0;
protected:
	virtual void Free() override;


};

_END

#endif // Player_h__

