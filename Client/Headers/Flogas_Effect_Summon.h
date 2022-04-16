#ifndef Flogas_Effect_Summon_h__
#define Flogas_Effect_Summon_h__

#include "GameObject.h"

_BEGIN(Client)
class CFlogas;
class CEffectGroup;
class CFlogas_Effect_Summon final	 : CGameObject
{
private:
	explicit CFlogas_Effect_Summon(LPDIRECT3DDEVICE9 pDevice);
	explicit CFlogas_Effect_Summon(const CFlogas_Effect_Summon& rhs);
	virtual ~CFlogas_Effect_Summon();

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

private:
	CFlogas*		m_pFlogas;
	CEffectGroup*	m_pSummon_MagicSquare;
	CEffectGroup*	m_pSummon_Zone;


public:
	virtual CGameObject* Clone_GameObject() override;
	static CFlogas_Effect_Summon* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	virtual void Free() override;
};
_END


#endif // Flogas_Effect_Summon_h__
