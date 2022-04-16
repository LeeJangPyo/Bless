#include "Input_Device.h"


_IMPLEMENT_SINGLETON(CInput_Device)

Engine::CInput_Device::CInput_Device(void)
{
	m_vMouseOldPos = _vec3(0.f, 0.f, 0.f);
	m_vMouseCurPos = _vec3(0.f, 0.f, 0.f);
}

Engine::CInput_Device::~CInput_Device(void)
{

}

HRESULT Engine::CInput_Device::Ready_Input_Device(HINSTANCE hInst, HWND hWnd)
{
	//Dinput�İ�ü ����
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
	{
		_TAGMSG_BOX(L"Input Device Create Failed, in Ready_Input_Device", L"CInput_Device");
	}

	//////////////////////////////////////////////////////////////////////////
	//							Ű����
	//////////////////////////////////////////////////////////////////////////

	//����ǻ�Ϳ� ����Ǿ��ִ�Ű�����İ�ü ����
	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
	{
		_TAGMSG_BOX(L"Keyboard Device Create Failed, in Ready_Input_Device", L"CInput_Device");
	}

	//������ Ű���� ��ü�� ���� ���� ����
	m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);

	//��ġ�� ���� �������� �������ִ� �Լ�(Ŭ���̾�Ʈ�� �� �ִ� ���¿��� Ű �Է��� ������ ���� ����)
	m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	//��ġ������ acess ������ �޾ƿ����� �Լ�
	m_pKeyboard->Acquire();

	//////////////////////////////////////////////////////////////////////////
	//							���콺
	//////////////////////////////////////////////////////////////////////////

	//����ǻ�Ϳ� ����Ǿ��ִ�Ű�����İ�ü ����
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
	{
		_TAGMSG_BOX(L"Mouse Device Create Failed, in Ready_Input_Device", L"CInput_Device");
	}


	m_pMouse->SetDataFormat(&c_dfDIMouse);

	//m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE );

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	m_pMouse->Acquire();


	m_hWnd = hWnd;

	return S_OK;
}

void Engine::CInput_Device::SetUp_InputState(void)
{

	memcpy(&m_ePreMouseState, &m_eCurMouseState, sizeof(DIMOUSESTATE));
	memcpy(&m_byPreKeyState, &m_byCurKeyState, sizeof(_byte) * 256);

	m_pKeyboard->GetDeviceState(256, m_byCurKeyState);
	m_pMouse->GetDeviceState(sizeof(m_eCurMouseState), &m_eCurMouseState);


	POINT MsPos;
	::GetCursorPos(&MsPos);
	::ScreenToClient(m_hWnd, &MsPos);

	//���콺 ���� 
	m_vMouseCurPos.x = _float(MsPos.x);
	m_vMouseCurPos.y = _float(MsPos.y);
	

	m_vMouseMove.x = m_vMouseCurPos.x - m_vMouseOldPos.x;
	m_vMouseMove.y = m_vMouseCurPos.y - m_vMouseOldPos.y;	
	m_vMouseMove.z = _float(m_eCurMouseState.lZ);

	m_vMouseOldPos = m_vMouseCurPos;

	//cout << "Move x : "<< m_vMouseMove.x << " Y : " << m_vMouseMove.y << " Z : " << m_vMouseMove.z << endl;


}

void Engine::CInput_Device::Free(void)
{
	Engine::Safe_Release(m_pKeyboard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);

}

_bool Engine::CInput_Device::Get_KeyDown(_ubyte byKeyID)
{

	if( !(m_byPreKeyState[byKeyID] & 0x80) && (m_byCurKeyState[byKeyID] & 0x80))	// �������� �����ְ�
	{
		return true;
	}

	return false;
}

_bool Engine::CInput_Device::Get_KeyUp(_ubyte byKeyID)
{
	if((m_byPreKeyState[byKeyID] & 0x80) && !(m_byCurKeyState[byKeyID] & 0x80))	// �������� �����ְ�
	{
		return true;
	}
	return false;
}

_bool Engine::CInput_Device::Get_KeyPressing(_ubyte byKeyID)
{
	if (m_byCurKeyState[byKeyID] & 0x80)
	{
		return true;
	}
	return false;
}

_bool CInput_Device::Get_MouseDown(MOUSEKEYSTATE eMouseID)
{
	if (!(m_ePreMouseState.rgbButtons[eMouseID] & 0x80) && (m_eCurMouseState.rgbButtons[eMouseID] & 0x80))	// �������� �����ְ�
	{
		return true;
	}

	return false;
}

_bool CInput_Device::Get_MousePressing(MOUSEKEYSTATE eMouseID)
{

	if ((m_eCurMouseState.rgbButtons[eMouseID] & 0x80))	// �������� �����ְ�
	{
		return true;
	}
	return false;
}

_bool CInput_Device::Get_MouseUp(MOUSEKEYSTATE eMouseID)
{
	
	if ((m_ePreMouseState.rgbButtons[eMouseID] & 0x80) && !(m_eCurMouseState.rgbButtons[eMouseID] & 0x80))	// �������� �����ְ�
	{
		return true;
	}
	return false;
}

