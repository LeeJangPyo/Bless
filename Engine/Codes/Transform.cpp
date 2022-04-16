#include "..\Headers\Transform.h"

#include <cmath>

CTransform::CTransform(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_matWorld(rhs.m_matWorld)
{
	
}

const _vec3 * CTransform::Get_StateInfo(STATE eState)
{
	return (_vec3*)&m_matWorld.m[eState][0];
}

_float CTransform::Get_ScaleInfo(STATE eState)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return D3DXVec3Length((_vec3*)&m_matWorld.m[eState][0]);
}

_matrix CTransform::Get_BillBoardMatrix()
{
	_matrix matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	//Save Scale value
	_vec3	vScale;
	vScale.x = Get_ScaleInfo(CTransform::STATE_RIGHT);
	vScale.y = Get_ScaleInfo(CTransform::STATE_UP);
	vScale.z = Get_ScaleInfo(CTransform::STATE_LOOK);

	//Set Scale
	Set_ScaleInfo(&_vec3(1.f, 1.f, 1.f));
	
	//GetBill Board
	_matrix matInvView;
	D3DXMatrixInverse(&matInvView, nullptr, &matView);

	_matrix	matBillBoard;
	D3DXMatrixIdentity(&matBillBoard);

	matBillBoard._11 = matInvView._11;
	matBillBoard._13 = matInvView._13;
	matBillBoard._31 = matInvView._31;
	matBillBoard._33 = matInvView._33;

	_matrix matWorld = matBillBoard * m_matWorld;

	//Set Original Scale Value;
	*(_vec3*)&matWorld.m[0][0] *= vScale.x;
	*(_vec3*)&matWorld.m[1][0] *= vScale.y;
	*(_vec3*)&matWorld.m[2][0] *= vScale.z;

	*(_vec3*)&m_matWorld.m[0][0] *= vScale.x;
	*(_vec3*)&m_matWorld.m[1][0] *= vScale.y;
	*(_vec3*)&m_matWorld.m[2][0] *= vScale.z;

	return matWorld;
}

_matrix CTransform::Get_MultiplyParentMatrix(const _matrix * pmatParent)
{
	return m_matWorld * *pmatParent;
}

_float CTransform::Get_Distance(const _vec3 * pTarget)
{
	return D3DXVec3Length(&_vec3(*pTarget - *Get_StateInfo(CTransform::STATE_POSITION)));

}

_float CTransform::Get_RotationInfo(STATE eState)
{
	_vec3		vRight = _vec3(1.f, 0.f, 0.f);
	_vec3		vUp = _vec3(0.f, 1.f, 0.f);
	_vec3		vLook = _vec3(0.f, 0.f, 1.f);

	switch (eState)
	{
	case Engine::CTransform::STATE_RIGHT:
	{
		//z축
		_vec3		vCurrentLook = *Get_StateInfo(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vCurrentLook, &vCurrentLook);

		_float	fLook_CosTheta = D3DXVec3Dot(&vCurrentLook, &vLook);

		_float	fLook_Angle = acosf(fLook_CosTheta);

		_vec3 vCross;
		D3DXVec3Cross(&vCross, &vLook, &vCurrentLook);

		if (vCross.y > 0.f)
			return fLook_Angle;
		else
			return -fLook_Angle;

	}
	case Engine::CTransform::STATE_UP:
	{
		//x축
		_vec3		vCurrentRight = *Get_StateInfo(CTransform::STATE_RIGHT);
		D3DXVec3Normalize(&vCurrentRight, &vCurrentRight);

		_float	fRight_CosTheta = D3DXVec3Dot(&vCurrentRight, &vRight);

		_float	fRight_Angle = acosf(fRight_CosTheta);

		_vec3 vCross;
		D3DXVec3Cross(&vCross, &vRight, &vCurrentRight);

		if (vCross.y > 0.f)
			return fRight_Angle;
		else
			return -fRight_Angle;

		//	return 0.f;
	}
	case Engine::CTransform::STATE_LOOK:
	{
		//x축
		_vec3		vCurrentRight = *Get_StateInfo(CTransform::STATE_RIGHT);
		D3DXVec3Normalize(&vCurrentRight, &vCurrentRight);

		_float	fRight_CosTheta = D3DXVec3Dot(&vCurrentRight, &vRight);

		_float	fRight_Angle = acosf(fRight_CosTheta);

		_vec3 vCross;
		D3DXVec3Cross(&vCross, &vRight, &vCurrentRight);

		if (vCross.y > 0.f)
			return fRight_Angle;
		else
		return fRight_Angle;
	}
	}

	return 0.f;
}



void CTransform::Set_StateInfo(STATE eState, const _vec3 * pIn)
{
	memcpy(&m_matWorld.m[eState][0], pIn, sizeof(_vec3));
}

void CTransform::Set_StateInfo(STATE eState, const _float & fX, const _float & fY, const _float & fZ)
{
	_vec3		vTmp = _vec3(fX, fY, fZ);

	memcpy(&m_matWorld.m[eState][0], &vTmp, sizeof(_vec3));
}

void CTransform::Set_ScaleInfo(const _vec3 * pIn)
{
	_vec3		vRight, vUp, vLook;

	D3DXVec3Normalize(&vRight, Get_StateInfo(STATE_RIGHT));
	vRight *= pIn->x;
	Set_StateInfo(STATE_RIGHT, &vRight);

	D3DXVec3Normalize(&vUp, Get_StateInfo(STATE_UP));
	vUp *= pIn->y;
	Set_StateInfo(STATE_UP, &vUp);

	D3DXVec3Normalize(&vLook, Get_StateInfo(STATE_LOOK));
	vLook *= pIn->z;
	Set_StateInfo(STATE_LOOK, &vLook);
}

void CTransform::Set_ScaleInfo(const _float & fX, const _float & fY, const _float & fZ)
{
	_vec3		vRight, vUp, vLook;

	D3DXVec3Normalize(&vRight, Get_StateInfo(STATE_RIGHT));
	vRight *= fX;
	Set_StateInfo(STATE_RIGHT, &vRight);

	D3DXVec3Normalize(&vUp, Get_StateInfo(STATE_UP));
	vUp *= fY;
	Set_StateInfo(STATE_UP, &vUp);

	D3DXVec3Normalize(&vLook, Get_StateInfo(STATE_LOOK));
	vLook *= fZ;
	Set_StateInfo(STATE_LOOK, &vLook);
}

void CTransform::Set_RotationX(const _float & fRadian)
{
	_vec3		vRight = _vec3(1.f, 0.f, 0.f);
	_vec3		vUp = _vec3(0.f, 1.f, 0.f);
	_vec3		vLook = _vec3(0.f, 0.f, 1.f);

	_matrix		matRot;
	D3DXMatrixRotationX(&matRot, fRadian);

	memcpy(&m_matWorld.m[STATE_RIGHT][0], D3DXVec3TransformNormal(&vRight, &vRight, &matRot), sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_UP][0], D3DXVec3TransformNormal(&vUp, &vUp, &matRot), sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_LOOK][0], D3DXVec3TransformNormal(&vLook, &vLook, &matRot), sizeof(_vec3));
}

void CTransform::Set_RotationY(const _float & fRadian)
{
	_vec3		vRight = _vec3(1.f, 0.f, 0.f);
	_vec3		vUp = _vec3(0.f, 1.f, 0.f);
	_vec3		vLook = _vec3(0.f, 0.f, 1.f);

	_matrix		matRot;
	D3DXMatrixRotationY(&matRot, fRadian);

	memcpy(&m_matWorld.m[STATE_RIGHT][0], D3DXVec3TransformNormal(&vRight, &vRight, &matRot), sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_UP][0], D3DXVec3TransformNormal(&vUp, &vUp, &matRot), sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_LOOK][0], D3DXVec3TransformNormal(&vLook, &vLook, &matRot), sizeof(_vec3));
}

void CTransform::Set_RotationZ(const _float & fRadian)
{
	_vec3		vRight = _vec3(1.f, 0.f, 0.f);
	_vec3		vUp = _vec3(0.f, 1.f, 0.f);
	_vec3		vLook = _vec3(0.f, 0.f, 1.f);

	_matrix		matRot;
	D3DXMatrixRotationZ(&matRot, fRadian);

	memcpy(&m_matWorld.m[STATE_RIGHT][0], D3DXVec3TransformNormal(&vRight, &vRight, &matRot), sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_UP][0], D3DXVec3TransformNormal(&vUp, &vUp, &matRot), sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_LOOK][0], D3DXVec3TransformNormal(&vLook, &vLook, &matRot), sizeof(_vec3));
}

void CTransform::Set_WorldMatrix(const _matrix * pmatWorld)
{
	memcpy(&m_matWorld, pmatWorld, sizeof(_matrix));
}



HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);

	return S_OK;
}

HRESULT CTransform::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantTable)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->SetMatrix(pConstantTable, &m_matWorld);
	
	return S_OK;
}

HRESULT CTransform::SetUp_OnShader_BillBoardMatrix(LPD3DXEFFECT pEffect, const char * pConstantTable)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->SetMatrix(pConstantTable, &Get_BillBoardMatrix());

	return S_OK;
}

HRESULT CTransform::SetUp_OnShader_MultiplyParentMatrix(LPD3DXEFFECT pEffect,const _matrix* pmatParent, const char * pConstantTable)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->SetMatrix(pConstantTable, &Get_MultiplyParentMatrix(pmatParent));

	return S_OK;
}

HRESULT CTransform::SetUp_OnShader_MultiplyParnetMatrix_BillBoadMatrix(LPD3DXEFFECT pEffect, const _matrix * pmatParent, const char * pConstantTable)
{
	if (pEffect == nullptr)
		return E_FAIL;

	_matrix matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	//Save Scale value
	_vec3	vScale;
	vScale.x = D3DXVec3Length((_vec3*)&(pmatParent->m[0][0]));
	vScale.y = D3DXVec3Length((_vec3*)&(pmatParent->m[1][0]));
	vScale.z = D3DXVec3Length((_vec3*)&(pmatParent->m[2][0]));

	_vec3 vPosition = *(_vec3*)&_matrix(m_matWorld * *pmatParent).m[3][0];

	//GetBill Board
	_matrix matInvView;
	D3DXMatrixInverse(&matInvView, nullptr, &matView);

	_matrix	matBillBoard;
	D3DXMatrixIdentity(&matBillBoard);

	matBillBoard._11 = matInvView._11;
	matBillBoard._13 = matInvView._13;
	matBillBoard._31 = matInvView._31;
	matBillBoard._33 = matInvView._33;

	*(_vec3*)&(matBillBoard.m[0][0]) *= vScale.x;
	*(_vec3*)&(matBillBoard.m[1][0]) *= vScale.y;
	*(_vec3*)&(matBillBoard.m[2][0]) *= vScale.z;

	memcpy(&matBillBoard.m[3][0], &vPosition, sizeof(_vec3));

	pEffect->SetMatrix(pConstantTable, &matBillBoard);

	return S_OK;
}


void CTransform::Go_Straight(const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3		vLook;	
	D3DXVec3Normalize(&vLook, (_vec3*)&m_matWorld.m[STATE_LOOK][0]);

	*(_vec3*)&m_matWorld.m[STATE_POSITION][0] += vLook * fSpeed * fTimeDelta;
}

void CTransform::Go_Left(const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3		vRight;

	*(_vec3*)&m_matWorld.m[STATE_POSITION][0] -=
		*D3DXVec3Normalize(&vRight, (_vec3*)&m_matWorld.m[STATE_RIGHT][0]) * fSpeed * fTimeDelta;

}

void CTransform::Go_Right(const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3		vRight;

	*(_vec3*)&m_matWorld.m[STATE_POSITION][0] +=
		*D3DXVec3Normalize(&vRight, (_vec3*)&m_matWorld.m[STATE_RIGHT][0]) * fSpeed * fTimeDelta;
}

void CTransform::Go_BackWard(const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3		vLook;

	*(_vec3*)&m_matWorld.m[STATE_POSITION][0] -=
		*D3DXVec3Normalize(&vLook, (_vec3*)&m_matWorld.m[STATE_LOOK][0]) * fSpeed * fTimeDelta;
}

_bool CTransform::Go_Target(const _vec3 * pTargetPos, const _float & fSpeed, const _float & fTimeDelta, _float fLimit)
{
	_vec3		vLook = *pTargetPos - *(_vec3*)&m_matWorld.m[STATE_POSITION][0];
	_float		fDistance = D3DXVec3Length(&vLook);

	if (fDistance < fLimit)
		return true;

	*(_vec3*)&m_matWorld.m[STATE_POSITION][0] +=
		*D3DXVec3Normalize(&vLook, &vLook) * fSpeed * fTimeDelta;

	return false;

}

void CTransform::Go_Direction(const _vec3 * pDirection, const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3		vDirection = *pDirection;

	*(_vec3*)&m_matWorld.m[STATE_POSITION][0] +=
		*D3DXVec3Normalize(&vDirection, &vDirection) * fSpeed * fTimeDelta;
}

void CTransform::Rotation_X(const _float & fRadian, const _float & fTimeDelta)
{
	_vec3		vRight = *Get_StateInfo(STATE_RIGHT);
	_vec3		vUp = *Get_StateInfo(STATE_UP);
	_vec3		vLook = *Get_StateInfo(STATE_LOOK);

	_matrix		matRot;
	D3DXMatrixRotationX(&matRot, fRadian * fTimeDelta);

	memcpy(&m_matWorld.m[STATE_RIGHT][0], D3DXVec3TransformNormal(&vRight, &vRight, &matRot), sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_UP][0], D3DXVec3TransformNormal(&vUp, &vUp, &matRot), sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_LOOK][0], D3DXVec3TransformNormal(&vLook, &vLook, &matRot), sizeof(_vec3));

}

void CTransform::Rotation_Y(const _float & fRadian, const _float & fTimeDelta)
{
	
	_vec3		vRight = *Get_StateInfo(STATE_RIGHT);
	_vec3		vUp = *Get_StateInfo(STATE_UP);
	_vec3		vLook = *Get_StateInfo(STATE_LOOK);

	_matrix		matRot;
	D3DXMatrixRotationY(&matRot, fRadian * fTimeDelta);

	memcpy(&m_matWorld.m[STATE_RIGHT][0], D3DXVec3TransformNormal(&vRight, &vRight, &matRot), sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_UP][0], D3DXVec3TransformNormal(&vUp, &vUp, &matRot), sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_LOOK][0], D3DXVec3TransformNormal(&vLook, &vLook, &matRot), sizeof(_vec3));

}

void CTransform::Rotation_Z(const _float & fRadian, const _float & fTimeDelta)
{
	_vec3		vRight = *Get_StateInfo(STATE_RIGHT);
	_vec3		vUp = *Get_StateInfo(STATE_UP);
	_vec3		vLook = *Get_StateInfo(STATE_LOOK);

	_matrix		matRot;
	D3DXMatrixRotationZ(&matRot, fRadian * fTimeDelta);

	memcpy(&m_matWorld.m[STATE_RIGHT][0], D3DXVec3TransformNormal(&vRight, &vRight, &matRot), sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_UP][0], D3DXVec3TransformNormal(&vUp, &vUp, &matRot), sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_LOOK][0], D3DXVec3TransformNormal(&vLook, &vLook, &matRot), sizeof(_vec3));

}

void CTransform::LookAt_Target(const _vec3 * pTargetPos, const _float & fTimeDelta)
{
	_vec3 vPosition = *Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vLook = *Get_StateInfo(CTransform::STATE_LOOK);	
	_vec3 vDir = *pTargetPos - vPosition;
	_float fDistance = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vCross;
	D3DXVec3Cross(&vCross, &vLook, &vDir);
	_float cosTheta = D3DXVec3Dot(&vLook, &vDir);
	if (cosTheta > 1.f)
		cosTheta = 1.f;
	_float fRadian = acos(cosTheta);

	if (vCross.y >= 0.f)
		Rotation_Y(fRadian, fTimeDelta);
	else
		Rotation_Y(-fRadian, fTimeDelta);
}

void CTransform::LookAt_Target(const _vec3 * pTargetPos, const _float & fTimeDelta, const _vec3 * pLookDir)
{
	_vec3 vPosition = *Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vLook = *pLookDir;
	_vec3 vDir = *pTargetPos - vPosition;
	_float fDistance = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vCross;
	D3DXVec3Cross(&vCross, &vLook, &vDir);
	_float fRadian = acos(D3DXVec3Dot(&vLook, &vDir));

	if (vCross.y >= 0.f)
		Rotation_Y(fRadian, fTimeDelta);
	else
		Rotation_Y(-fRadian, fTimeDelta);
}

void CTransform::Plus_Position(const _float & fX, const _float & fY, const _float & fZ)
{
	m_matWorld._41 += fX;
	m_matWorld._42 += fY;
	m_matWorld._43 += fZ;
}

void CTransform::Plus_Position(const _vec3 * pIn)
{
	m_matWorld._41 += pIn->x;
	m_matWorld._42 += pIn->y;
	m_matWorld._43 += pIn->z;
}


void CTransform::Multiply_ParentMatrix(const _matrix * pmatParent)
{
	m_matWorld *= *pmatParent;
}


CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTransform*		pInstance = new CTransform(pDevice);

	if (FAILED(pInstance->Ready_Transform()))
	{
		_MSG_BOX(L"CTransform Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CTransform::Clone_Component()
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	CComponent::Free();
}
