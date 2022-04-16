#ifndef UI_Frame_h__
#define UI_Frame_h__

#include "Defines.h"
#include "GameObject.h"

#include "UI.h"

_BEGIN(Engine)
class CRenderer;
class CShader;
_END

_BEGIN(Client)

class CUI_Frame final : public CGameObject
{
public:
	explicit CUI_Frame(LPDIRECT3DDEVICE9 pDevice);
	explicit CUI_Frame(const CUI_Frame& rhs);
	virtual ~CUI_Frame() = default;

public:
	//set
	void Set_Tag(const _tchar* pTag) { m_wstrTag = pTag; }
	void Set_Select(_bool bIsSelect) { m_bIsSelect = bIsSelect; }
	void Set_UITab(CUITab* pUITab) {	m_pUITab = pUITab;}
	void Set_Position(const _float& fX,const _float& fY);
	void Set_Scale(const _float& fX, const _float& fY);
	//Get
	const _vec3*	Get_Position() const { return &m_vPosition; }
	CUI*	Get_UI(const _tchar* pTag);
	_bool	Compare_Tag(const wstring& wstrTag);
public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	HRESULT	Add_UI(const _tchar* pTag);
	HRESULT	Change_UITag(const _tchar* pCurrentTag, const _tchar* pDestTag);
	HRESULT Save_UIFrame_Info(HANDLE& hFile, _ulong& dwByte);
	void	Load_UIFrame_Info(HANDLE& hFile, _ulong& dwBytem, CTreeCtrl& TreeUIList);
private:
	void	Check_KeyInput(const _float& fTimeDelta);
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

	_bool			m_bIsSelect;
	_bool			m_bIsLButtonDown;

	CUITab*			m_pUITab = nullptr;
public:
	virtual CGameObject* Clone_GameObject() override;
	static CGameObject*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;
};

_END

#endif // UI_Frame_h__
