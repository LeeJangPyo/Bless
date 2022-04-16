#ifndef Camera_CutScene_h__
#define Camera_CutScene_h__

#include "Defines.h"
#include "Camera.h"

#include "Camera_Eye.h"
#include "Camera_At.h"

_BEGIN(Engine)
class CCalculator;
class CRenderer;
class CLine_Manager;
class CShader;
_END

_BEGIN(Client)

class CCamera_CutScene final : public CCamera
{
private:
	explicit	CCamera_CutScene(LPDIRECT3DDEVICE9 pDevice);
	explicit	CCamera_CutScene(const CCamera_CutScene& rhs);
	virtual		~CCamera_CutScene(void) = default;

public:
	//set
	void Set_KeyTag(const wstring& pTag) { m_wstrKey = pTag; }
	void Set_Speed(const _float& fSpeed) { m_fSpeed = fSpeed; }
	void Set_PickEye(const _uint iIndex, _bool bIsPick);
	void Set_PickAt(const _uint iIndex, _bool bIsPick);

	//get
	const wstring& Get_Key() const { return m_wstrKey; }
	_bool Get_IsSceneEnd() const { return m_bIsSceneEnd; }
public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;


public:
	HRESULT		Save_CutScene(const _tchar* pFilePath);
	void		Add_Eye(const _vec3& vPosition);
	void		Add_At(const _vec3& vPosition);
	void		Caculate_Line(void);
private:
	HRESULT		Ready_Component(void);
	_vec3		PickUp_OnTerrain(void);

private:
	wstring					m_wstrKey;

	CCalculator*			m_pCalculatorCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CLine_Manager*			m_pLine_Manager = nullptr;

	_float			m_fSpeed;
	_float			m_fTime;
	_int			m_iSection;
	_bool			m_bIsSceneEnd;

	vector<CCamera_Eye*>			m_vecCamera_Eye;
	vector<CCamera_At*>				m_vecCamera_At;
	vector<_vec3*>					m_vecLine_Eye;
	vector<_vec3*>					m_vecLine_At;
	
public:
	static CCamera_CutScene*	Create(LPDIRECT3DDEVICE9	 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free(void) override;
};

_END
#endif // Camera_CutScene_h__
