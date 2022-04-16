#ifndef Input_Device_h__
#define Input_Device_h__

#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL	CInput_Device : public CBase
{
	_DECLARE_SINGLETON(CInput_Device)

public:
	enum MOUSEKEYSTATE{	DIM_LB, DIM_RB, DIM_MB, DIM_END};
	enum MOUSEMOVESTATE{DIMS_X, DIMS_Y, DIMS_Z, DIMS_END};

private:
	explicit	CInput_Device(void);
	virtual		~CInput_Device(void);

public:
	const _vec3*		Get_DIMousePos() { return &m_vMouseCurPos; }
public:
	_bool		Get_KeyDown(_ubyte byKeyID);
	_bool		Get_KeyUp(_ubyte byKeyID);
	_bool		Get_KeyPressing(_ubyte byKeyID);
	_bool		Get_MouseDown(MOUSEKEYSTATE eMouseID);
	_bool		Get_MousePressing(MOUSEKEYSTATE eMouseID);
	_bool		Get_MouseUp(MOUSEKEYSTATE eMouseID);
	_float		Get_MouseMove(MOUSEMOVESTATE eMouseMoveState) { return *(((_float*)(&m_vMouseMove) + eMouseMoveState)); }

public:
	HRESULT		Ready_Input_Device(HINSTANCE hInst, HWND hWnd);
	void		SetUp_InputState(void);

private:
	LPDIRECTINPUT8			m_pInputSDK;
	LPDIRECTINPUTDEVICE8	m_pKeyboard;
	LPDIRECTINPUTDEVICE8	m_pMouse;

	_byte					m_byCurKeyState[256];
	_byte					m_byPreKeyState[256];
	DIMOUSESTATE			m_eCurMouseState;
	DIMOUSESTATE			m_ePreMouseState;
	_vec3					m_vMouseCurPos;
	_vec3					m_vMouseOldPos;
	_vec3					m_vMouseMove;

	HWND					m_hWnd;

public:
	virtual		void		Free(void);
};

_END
#endif // Input_Device_h__
