//#ifndef Masq_h__
//#define Masq_h__
//
//#include "GameObject_Dynamic.h"
//
//_BEGIN(Engine)
//class CCollider_Sphere;
//class CInput_Device;
//class CCollision;
//_END
//
//_BEGIN(Client)
//class CGameEvent_Manager;
//class CMasq final	: public CGameObject_Dynamic
//{
//public:
//	explicit CMasq(LPDIRECT3DDEVICE9 pDevice);
//	explicit CMasq(const CMasq& rhs);
//	virtual ~CMasq() = default;
//
//public:
//	virtual HRESULT Ready_Prototype() override;
//	virtual HRESULT Ready_GameObject() override;
//	virtual _int Update_GameObject(const _float& fTimeDelta) override;
//	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
//	virtual void Render_GameObject() override;
//
//	virtual	HRESULT	SetUp_GameObjectInfo(const _matrix* pMatrix) override;
//	virtual	void	Damaged() override;
//
//public:
//	void	Check_KeyInput_Move(const _float& fTimeDelta);
//
//private:
//	HRESULT	Ready_Component();
//	HRESULT	Ready_State();
//
//
//private:
//	_int				m_iComboCount = 4;
//
//	_bool				m_bIsRButtonDown;
//
//	CInput_Device*		m_pInput_Device = nullptr;
//
//public:
//	static	CMasq* Create(LPDIRECT3DDEVICE9 pDevice);
//	virtual CGameObject* Clone_GameObject() override;
//protected:
//	virtual void Free() override;
//
//
//
//};
//
//
//_END
//#endif // Masq_h__