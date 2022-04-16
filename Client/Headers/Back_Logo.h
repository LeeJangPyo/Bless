#pragma once

#include "GameObject.h"

_BEGIN(Client)

class CBack_Logo final : public CGameObject
{
private:
	explicit CBack_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBack_Logo(const CBack_Logo& rhs);
	virtual ~CBack_Logo() = default;
public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;
public:
	static CBack_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device); // 우너형객체 샏성
	virtual CGameObject* Clone_GameObject() override; // 실제 사용할 객체를 생서아.
};

_END