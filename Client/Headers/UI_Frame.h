#ifndef UI_Frame_h__
#define UI_Frame_h__

#include "Defines.h"
#include "GameObject.h"



_BEGIN(Engine)
class CRenderer;
class CTransform;
class CInput_Device;
class CShader;
_END

_BEGIN(Client)
class CUI;
class CUI_Frame final : public CGameObject
{
public:
	explicit CUI_Frame(LPDIRECT3DDEVICE9 pDevice);
	explicit CUI_Frame(const CUI_Frame& rhs);
	virtual ~CUI_Frame() = default;

public:
	HRESULT	Set_CoolDown(const _float& fCoolDown);
	HRESULT	Set_Radio(const _float& fRadio, const _tchar* pUI_Tag);
	HRESULT	Set_IsCoolDown(_bool bIsCoolDown);
	void	Set_Activate(_bool bActivate) {
		m_bActivate = bActivate;	}
	_bool	Get_IsCoolDown();

	//Get
	CUI* Get_UI(const _tchar* pTag);
	_bool	Compare_Tag(const wstring& wstrTag);
public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	HRESULT	Add_UI(const _tchar* pTag);
	void	Load_UIFrame_Info(HANDLE& hFile, _ulong& dwByte);
private:
	HRESULT Ready_Component();
	CUI*	Find_UI(const _tchar* pTag);
private:
	wstring					m_wstrTag;

	_matrix					m_matProj;
	_vec3					m_vPosition;

	CInput_Device*	m_pInput_Device = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CShader*		m_pShaderCom = nullptr;

	map<wstring, CUI*>	m_mapUI;

	_bool	m_bIsCoolDown;
	_float	m_fCoolDown;

	_bool	m_bActivate;
public:
	virtual CGameObject* Clone_GameObject() override;
	static CGameObject*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;
};

_END

#endif // UI_Frame_h__
