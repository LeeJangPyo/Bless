#ifndef Particle_h__
#define Particle_h__

#include "Base.h"

_BEGIN(Engine)
class CTransform;
class CBuffer_Rctex;
_END

_BEGIN(Client)

class CParticle	final		
{
public:
	CParticle();
	~CParticle();
	
public:
	void	SetUp_Paritcle(_int iIdx);
	void	Set_Animation(_bool bAni = TRUE);		// 애니메이션 활성화/ 비활성화

public:
	HRESULT Ready_Particle();
	_int Update_GameObject(const _float& fTimeDelta);
	_int LastUpdate_GameObject(const _float& fTimeDelta);
	void Render_GameObject();

private:
	_vec3		m_Inital_Position;	//초기위치
	_vec3		m_Inital_Velocity;	//초기 속도
	_vec3		m_Inital_Acceleration;	//초기가속도

	_vec3		m_Current_Position;	//초기위치
	_vec3		m_Current_Velocity;	//초기 속도
	_vec3		m_Current_Acceleration;	//초기가속도

	_float			m_fElst;	// 탄성(Elastic) 계수
	_float			m_fDamp;	// 공기 저항(Air Registance) 계수

	// 입자의 생명 요소
	DWORD			m_dColor;	

	// 입자의 표현 요소
	FLOAT			m_PrsW;		// Present Billboard W
	FLOAT			m_PrsH;		// Present Billboard H


	CTransform*		m_pTransformCom = nullptr;
	CBuffer_Rctex*	m_pBufferCom = nullptr;
};


_END


#endif // Particle_h__