#include "Animation_Controller.h"


Engine::CAnimation_Controller::CAnimation_Controller(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	:	m_pAniCtrl(pAniCtrl)
	,	m_iCurrentTrack(0)
	,	m_iNewTrack(1)
	,	m_iOldAniIndex(99)
	,	m_fAccTime(0.f)
{
	m_pAniCtrl->AddRef();
}

Engine::CAnimation_Controller::CAnimation_Controller(const CAnimation_Controller& rhs)
	:	m_iCurrentTrack(rhs.m_iCurrentTrack)
	,	m_iNewTrack(rhs.m_iNewTrack)
	,	m_fAccTime(rhs.m_fAccTime)
	,	m_iOldAniIndex(rhs.m_iOldAniIndex)
{
	//�ִϸ��̼��� ������ �Ǿ �ȵǱ� ������ ���ʿ� �Լ��� ������
	rhs.m_pAniCtrl->CloneAnimationController(
		rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),	//������ ���� ��ä���� �����ǰ� �ִ� �ִϸ��̼� ����
		rhs.m_pAniCtrl->GetMaxNumAnimationSets(),		//���� ������ �ִϸ��̼��� �ִ� ����(����1, ����2 ���� ���� ����)
		rhs.m_pAniCtrl->GetMaxNumTracks(),				//�� ������ Ʈ�� ���� �÷� ���� Ʈ���� ����϶�� ����ϰ� �ȴ�. Ʈ�� ��κ� �ϳ��� ����� �� ���� �ִϸ��̼� set �÷��� ����Ѵ�.
		rhs.m_pAniCtrl->GetMaxNumEvents(),				//���� �޽� �ȿ� ����Ǿ� �ִ� ��Ư�� ȿ���� �ǹ�(���� ��� ����)
		&m_pAniCtrl);
}

Engine::CAnimation_Controller::~CAnimation_Controller(void)
{

}

HRESULT Engine::CAnimation_Controller::Ready_Animation_Controller(void)
{
	return S_OK;
}

void Engine::CAnimation_Controller::Set_AnimationSet(const _uint& iIndex, _bool bResetAnimation)
{
	if (m_iOldAniIndex == iIndex && bResetAnimation == false)
	{
		return;
	}
	if(m_iOldAniIndex == iIndex && bResetAnimation == true)
	{
		m_pAniCtrl->ResetTime();
		m_fAccTime = 0.f;
	}

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	// �ִϸ��̼� �� ������ �����ϴ� ��ü
	LPD3DXANIMATIONSET			pAniSet = nullptr;

	//�ε��� ����
	m_pAniCtrl->GetAnimationSet(iIndex, &pAniSet);
	//���ڿ� ����
	// m_pAniCtrl->GetAnimationSetByName();

	//�ִϸ��̼� ��ü �ð� ���� ��ȯ��
	m_dPeriod = pAniSet->GetPeriod();


	// 0��° Ʈ���� �ִϸ��̼� ���� �ø���.
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAniSet);

	// ������� �ʰ� �ִ� �̺�Ʈ�� ������ ������ �ȵǴ� ��찡 �߻��ؼ� �־��ִ� �ڵ�(�̺�Ʈ�� ���� ������ ó��)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);


	//	������ ����� �������ڴٴ� �ǹ�, 3���ھ������� Ű�������� ������ ���ΰ�
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.20);
	// �����Ǵ� �ð����� ���� ����Ǵ� �������� � �ӵ��� �����̰� �� ���ΰ�(�ӵ��� ��� ���� ������ Ű �����Ӹ��� 1)
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.20, D3DXTRANSITION_LINEAR);
	// �� �ִϸ��̼��� �ռ� �ÿ� � ���������� �� ������ ������ ������ �����ϴ� �Լ�
	// ���� ������ weight ���� �ռ��ϴ� �ٸ� �������� weight���� ���� 1�̾�� �Ѵ�.
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.2f, m_fAccTime, 0.20, D3DXTRANSITION_LINEAR);

	// �Լ��� �ִ� 0.25���� ���� �ð��� ������ ���� �ǹ��Ѵ�. �� �ð���ŭ �����̰� �� ���·� Ʈ���� Ȱ��ȭ�� �ǰ�
	// �� ����� 0.25��� �ð����� ���ÿ� ���� �ִٴ� ���̴�. �� �ð����� �ִϸ��̼� ���̿� ���̸� �����ϰ� �ִٰ� �����ϸ� �ȴ�.

	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.20, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.8f, m_fAccTime, 0.20, D3DXTRANSITION_LINEAR);


	// ���� �ִϸ��̼��� ����ǰ� �ִ� �ð��� �ʱ�ȭ�ϴ� �Լ�(Advanced �Լ� ȣ�� �� ���������� �ð��� �����ϰ� ����)
	m_pAniCtrl->ResetTime();
	m_fAccTime = 0.f;
	

	// ������ ��� ���̴� Ʈ������ ���ο� Ʈ���� �����ϰ� �Ǿ��� �� 0�ʺ��� �����ϵ��� �����ϴ� �Լ�
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIndex = iIndex;

	m_iCurrentTrack = m_iNewTrack;
}

void CAnimation_Controller::Reset_AnimationSet()
{
	//m_pAniCtrl->AdvanceTime(0.f, NULL);
	//m_fAccTime = 0.f;

}

void Engine::CAnimation_Controller::Play_Animation(const _float& fTimeDelta)
{
	//_float fPlusTime = 0.f;
	//if (fTimeDelta < 0)
	//{
	//	fPlusTime = m_dPeriod;
	//	fPlusTime -= m_fAccTime;
	//	fPlusTime += m_dPeriod;
	//}
	//else
	//{

	//}

	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);	//���� 2 : �ִϸ��̼� ���ۿ� ���� ���峪 ����Ʈ�� ���� ó���� ����ϴ� ��ü �ּ�
	m_fAccTime += fTimeDelta;
}	


Engine::CAnimation_Controller* Engine::CAnimation_Controller::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAnimation_Controller*		pInstance = new CAnimation_Controller(pAniCtrl);

	if (FAILED(pInstance->Ready_Animation_Controller()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

Engine::CAnimation_Controller* Engine::CAnimation_Controller::Create(const CAnimation_Controller& rhs)
{
	CAnimation_Controller*		pInstance = new CAnimation_Controller(rhs);

	if (FAILED(pInstance->Ready_Animation_Controller()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CAnimation_Controller::Free(void)
{
	Engine::Safe_Release(m_pAniCtrl);
}

_bool Engine::CAnimation_Controller::Is_AnimationSetEnd(void)
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));


	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);	//����Ʈ���� ������ �������� �Լ�

	// period�� ��ü �ð���
	// position�� ���� �����Ǵ� �ִϸ��̼� ��ġ��
		_double dCurrentRadio = TrackInfo.Position / m_dPeriod;
		//if (TrackInfo.Position >= m_dPeriod - 0.1)
		//	return true;
		if (dCurrentRadio > 0.8)
			return true;	

	return false;
}

_bool CAnimation_Controller::Is_AnimationPassRadio(const _float& fRadio)
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));


	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);	//����Ʈ���� ������ �������� �Լ�

	// period�� ��ü �ð���
	// position�� ���� �����Ǵ� �ִϸ��̼� ��ġ��
	_double dCurrentRadio = TrackInfo.Position / m_dPeriod;
	if (dCurrentRadio > fRadio)
		return true;
	return false;
}

_bool CAnimation_Controller::Is_AnimationInRange(const _float& fStart, const _float& fEnd)
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));


	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);	//����Ʈ���� ������ �������� �Լ�

															// period�� ��ü �ð���
															// position�� ���� �����Ǵ� �ִϸ��̼� ��ġ��
	_double dCurrentRadio = TrackInfo.Position / m_dPeriod;
	if (dCurrentRadio > fStart && dCurrentRadio < fEnd)
		return true;

	return false;
}

_uint Engine::CAnimation_Controller::Get_MaxNumAnimation(void)
{
	return m_pAniCtrl->GetMaxNumAnimationSets();
}

