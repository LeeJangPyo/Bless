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
	void	Set_Animation(_bool bAni = TRUE);		// �ִϸ��̼� Ȱ��ȭ/ ��Ȱ��ȭ

public:
	HRESULT Ready_Particle();
	_int Update_GameObject(const _float& fTimeDelta);
	_int LastUpdate_GameObject(const _float& fTimeDelta);
	void Render_GameObject();

private:
	_vec3		m_Inital_Position;	//�ʱ���ġ
	_vec3		m_Inital_Velocity;	//�ʱ� �ӵ�
	_vec3		m_Inital_Acceleration;	//�ʱⰡ�ӵ�

	_vec3		m_Current_Position;	//�ʱ���ġ
	_vec3		m_Current_Velocity;	//�ʱ� �ӵ�
	_vec3		m_Current_Acceleration;	//�ʱⰡ�ӵ�

	_float			m_fElst;	// ź��(Elastic) ���
	_float			m_fDamp;	// ���� ����(Air Registance) ���

	// ������ ���� ���
	DWORD			m_dColor;	

	// ������ ǥ�� ���
	FLOAT			m_PrsW;		// Present Billboard W
	FLOAT			m_PrsH;		// Present Billboard H


	CTransform*		m_pTransformCom = nullptr;
	CBuffer_Rctex*	m_pBufferCom = nullptr;
};


_END


#endif // Particle_h__