#ifndef Effect_Particle_h__
#define Effect_Particle_h__

#include "Effect.h"


_BEGIN(Engine)
class CTexture;
class CBuffer_RcTex;
class CTransform;
_END

_BEGIN(Client)

class CEffect_Particle : public CEffect
{
public:
	typedef struct tagParticle
	{
		tagParticle()
		{
			vInital_Position = _vec3(0.f, 0.f, 0.f);
			vInital_Velocity = _vec3(0.f, 0.f, 0.f);
			vInital_Acceleration = _vec3(0.f, 0.f, 0.f);

			vCurrent_Position = _vec3(0.f, 0.f, 0.f);
			vCurrent_Velocity = _vec3(0.f, 0.f, 0.f);
			vCurrent_Acceleration = _vec3(0.f, 0.f, 0.f);
		}
		~tagParticle()
		{
			Safe_Release(pBufferCom);
			Safe_Release(pTransformCom);
		}

		_vec3		vInital_Position;	//초기위치
		_vec3		vInital_Velocity;	//초기 속도
		_vec3		vInital_Acceleration;	//초기가속도

		_vec3		vCurrent_Position;	//초기위치
		_vec3		vCurrent_Velocity;	//초기 속도
		_vec3		vCurrent_Acceleration;	//초기가속도

		_bool		bLive;	// Active (Yes/No)
		_float		fLife;	// Particle fLife
		_float		fMaxLife;
		//_float		fFade;	// Fade Speed

		_float		fElst;	// 탄성(Elastic) 계수
		_float		fDamp;	// 공기 저항(Air Registance) 계수

		_float		fAlpha;

		_float		ViewZ;		// 카메라의 z축과 위치의 내적 값

		_int		iAnimationX;
		_int		iAnimationY;
		_float		fFrame;


		CBuffer_RcTex*		pBufferCom = nullptr;
		CTransform*			pTransformCom = nullptr;
	}PARTICLE;

	static int SortFnc(const PARTICLE* pParticleSrc, const PARTICLE* pParticleDest)
	{
		_float	fSrc, fDest;

		fSrc = pParticleSrc->ViewZ;
		fDest = pParticleDest->ViewZ;

		if (fSrc < fDest)
			return 1;

		else if (fSrc == fDest)
			return 0;

		else
			return -1;
	}
public:
	explicit CEffect_Particle(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEffect_Particle() = default;
	

public:
	void	Set_Inital_ParticleInfo(EFFECTPARTICLE_DATA& ParticleData);
	void	Get_Inital_ParticleInfo(EFFECTPARTICLE_DATA& ParticleData);
	
	

	void	Set_Particle(_int iIndex);
	void	Set_Animation();		// 애니메이션 활성화/ 비활성화


	void	SetUp_ParticleInfo(_int iParicleNum);
	void	SetUp_ParticleData(const EFFECTDATA& EffectData, const EFFECTPARTICLE_DATA& Effect_ParticleData, _bool bRecycle);

	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix* pmatParent, const  _int& iIndex);

public:
	virtual HRESULT Ready_Effect() override;
	virtual _int Update_Effect(const _float& fTimeDelta) override;
	virtual _int LastUpdate_Effect(const _float& fTimeDelta) override;
	virtual void Render_Effect(LPD3DXEFFECT pEffect, const _matrix* pmatParent) override;
	virtual void Initalize_Effect() override;



private:
	PARTICLE*	m_pParticle;	//파티클 데이터
	_bool		m_bRecycle;

	EFFECTPARTICLE_DATA	m_ParticleData;
public:
	static	CEffect_Particle*		Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;

};

_END

#endif // Effect_Particle_h__
