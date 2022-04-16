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
	//애니메이션이 공유가 되어선 안되기 때문에 애초에 함수를 제공함
	rhs.m_pAniCtrl->CloneAnimationController(
		rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),	//복제시 원본 객채에서 제공되고 있는 애니메이션 개수
		rhs.m_pAniCtrl->GetMaxNumAnimationSets(),		//구동 가능한 애니메이션의 최대 개수(인자1, 인자2 값이 거의 같음)
		rhs.m_pAniCtrl->GetMaxNumTracks(),				//한 동작을 트랙 위에 올려 놓고 트랙을 재생하라고 명령하게 된다. 트랙 대부분 하나만 만들고 그 위에 애니메이션 set 올려서 재생한다.
		rhs.m_pAniCtrl->GetMaxNumEvents(),				//현재 메쉬 안에 적용되어 있는 독특한 효과를 의미(현재 사용 못함)
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

	// 애니메이션 셋 정보를 보관하는 객체
	LPD3DXANIMATIONSET			pAniSet = nullptr;

	//인덱스 접근
	m_pAniCtrl->GetAnimationSet(iIndex, &pAniSet);
	//문자열 접근
	// m_pAniCtrl->GetAnimationSetByName();

	//애니메이션 전체 시간 값을 반환함
	m_dPeriod = pAniSet->GetPeriod();


	// 0번째 트랙에 애니메이션 셋을 올린다.
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAniSet);

	// 사용하지 않고 있는 이벤트들 때문에 보간이 안되는 경우가 발생해서 넣어주는 코드(이벤트가 없는 것으로 처리)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);


	//	현재의 모션을 꺼버리겠다는 의미, 3인자언제부터 키프레임을 해제할 것인가
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.20);
	// 해제되는 시간동안 현재 재생되던 프레임을 어떤 속도로 움직이게 할 것인가(속도의 상수 값은 각자의 키 프레임마다 1)
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.20, D3DXTRANSITION_LINEAR);
	// 두 애니메이션을 합성 시에 어떤 프레임으로 더 가깝게 보간할 것인지 결정하는 함수
	// 현재 프레임 weight 값은 합성하는 다른 프레임의 weight값과 합쳐 1이어야 한다.
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.2f, m_fAccTime, 0.20, D3DXTRANSITION_LINEAR);

	// 함수에 있는 0.25ㅏ는 값은 시간의 딜레이 값을 의미한다. 이 시간만큼 딜레이가 된 상태로 트랙은 활성화가 되고
	// 두 모션이 0.25라는 시간동안 동시에 돌고 있다는 뜻이다. 이 시간동안 애니메이션 사이와 사이를 보간하고 있다고 생각하면 된다.

	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.20, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.8f, m_fAccTime, 0.20, D3DXTRANSITION_LINEAR);


	// 현재 애니메이션이 재생되고 있던 시간을 초기화하는 함수(Advanced 함수 호출 시 내부적으로 시간을 누적하고 있음)
	m_pAniCtrl->ResetTime();
	m_fAccTime = 0.f;
	

	// 기존의 재생 중이던 트랙에서 새로운 트랙이 등장하게 되었을 때 0초부터 시작하도록 설정하는 함수
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

	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);	//인자 2 : 애니메이션 동작에 따른 사운드나 이펙트에 대한 처리를 담당하는 객체 주소
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


	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);	//현재트랙의 정보를 가져오는 함수

	// period는 전체 시간값
	// position은 현재 구동되는 애니메이션 위치값
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


	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);	//현재트랙의 정보를 가져오는 함수

	// period는 전체 시간값
	// position은 현재 구동되는 애니메이션 위치값
	_double dCurrentRadio = TrackInfo.Position / m_dPeriod;
	if (dCurrentRadio > fRadio)
		return true;
	return false;
}

_bool CAnimation_Controller::Is_AnimationInRange(const _float& fStart, const _float& fEnd)
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));


	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);	//현재트랙의 정보를 가져오는 함수

															// period는 전체 시간값
															// position은 현재 구동되는 애니메이션 위치값
	_double dCurrentRadio = TrackInfo.Position / m_dPeriod;
	if (dCurrentRadio > fStart && dCurrentRadio < fEnd)
		return true;

	return false;
}

_uint Engine::CAnimation_Controller::Get_MaxNumAnimation(void)
{
	return m_pAniCtrl->GetMaxNumAnimationSets();
}

