#pragma once

// 객체의 월드상태를 저장한다.

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE {STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION};
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pDevice);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;
public: // Getter
	const _vec3* Get_StateInfo(STATE eState);
	_float	Get_ScaleInfo(STATE eState);
	const _matrix* Get_WorldMatrix() const { return &m_matWorld; }
	_matrix	Get_BillBoardMatrix();
	_matrix	Get_MultiplyParentMatrix(const _matrix* pmatParent);
	_float	Get_Distance(const _vec3* pTarget);
	_float	Get_RotationInfo(STATE eState);
public: // Setter
	void Set_StateInfo(STATE eState, const _vec3* pIn);
	void Set_StateInfo(STATE eState, const _float& fX, const _float& fY, const _float& fZ);
	void Set_ScaleInfo(const _vec3* pIn);
	void Set_ScaleInfo(const _float& fX, const _float& fY, const _float& fZ);
	void Set_RotationX(const _float& fRadian);
	void Set_RotationY(const _float& fRadian);
	void Set_RotationZ(const _float& fRadian);
	void Set_WorldMatrix(const _matrix* pmatWorld);
public:
	HRESULT Ready_Transform();
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantTable);
	HRESULT SetUp_OnShader_BillBoardMatrix(LPD3DXEFFECT pEffect, const char* pConstantTable);
	HRESULT	SetUp_OnShader_MultiplyParentMatrix(LPD3DXEFFECT pEffect, const _matrix* pmatParent, const char* pConstantTable);
	HRESULT	SetUp_OnShader_MultiplyParnetMatrix_BillBoadMatrix(LPD3DXEFFECT pEffect, const _matrix* pmatParent, const char* pConstantTable);

public:
	void Go_Straight(const _float& fSpeed, const _float& fTimeDelta);
	void Go_Left(const _float& fSpeed, const _float& fTimeDelta);
	void Go_Right(const _float& fSpeed, const _float& fTimeDelta);
	void Go_BackWard(const _float& fSpeed, const _float& fTimeDelta);
	_bool Go_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta, _float fLimit = 0.f);
	void Go_Direction(const _vec3* pDirection, const _float& fSpeed, const _float& fTimeDelta);
	void Rotation_X(const _float& fRadian, const _float& fTimeDelta);
	void Rotation_Y(const _float& fRadian, const _float& fTimeDelta);
	void Rotation_Z(const _float& fRadian, const _float& fTimeDelta);
	void LookAt_Target(const _vec3* pTargetPos, const _float& fTimeDelta);
	void LookAt_Target(const _vec3* pTargetPos, const _float& fTimeDelta, const _vec3* pLookDir);
	void Plus_Position(const _float& fX, const _float& fY, const _float& fZ);
	void Plus_Position(const _vec3* pIn);
	/*void Rotation_Target();*/

	void Multiply_ParentMatrix(const _matrix* pmatParent);
private:
	_matrix			m_matWorld;
public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END