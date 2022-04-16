#ifndef Axis_h__
#define Axis_h__

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
	class CTransform;
	class CRenderer;
	class CLine_Manager;
_END

class CAxis final : public CGameObject
{
	enum AXIS_TYPE {AXIS_X,AXIS_Y,AXIS_Z,AXIS_END};
private:
	explicit CAxis(LPDIRECT3DDEVICE9 pDevice);
	explicit CAxis(const CAxis& rhs);
	virtual ~CAxis(void) = default;

public:
	
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Ready_Component(void);
private:
	CTransform*		m_pTransformCom;
	CRenderer*		m_pRendererCom;
	CLine_Manager*	m_pLine_Manager;

	_vec3*					m_pPoint[AXIS_END];
	_long					m_dwArrayCount[AXIS_END];

public:
	static CAxis* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free(void) override;



};

#endif // Axis_h__
