#ifndef Trail_h__
#define Trail_h__

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTexture;
class CShader;
class CRenderer;
_END

_BEGIN(Client)
class CGameObject_Weapon;
class CTrail final : public CGameObject
{
public:
	explicit CTrail(LPDIRECT3DDEVICE9 pDevice);
	explicit CTrail(const CTrail& rhs);
	virtual ~CTrail() = default;

public:
	virtual HRESULT	Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject();

public:
//	void	Set_TrailPosition(const _vec3* pNearPos, const _vec3* vFarPos);
	//void	Set_TrailWeapon(CGameObject_Weapon* pTrailWeapon) {
	//	m_pTrail_Weapon = pTrailWeapon;	}

	HRESULT Begin_Trail(CGameObject_Weapon* pTrailWeapon);
	HRESULT	End_Trail();

private:
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;

	_ulong						m_dwVtxSize;
	_ulong						m_dwVtxCnt;
	_ulong						m_dwVtxFVF;

	CTexture*					m_pTexutre;
	CShader*					m_pShader;
	CRenderer*					m_pRenderer;

	TRAILPOS*					m_parrTrailPos = nullptr;
	_int						m_iTrailIndex;
	
	TRAILPOS*					m_parrInterpolation_TrailPos = nullptr;


	_int						m_iMaxTrail;

	_matrix						m_matWorld;

	_bool						m_bIsActivate;

	CGameObject_Weapon*			m_pTrail_Weapon;

	//Delay
	_float						m_fTimeAcc;
	_float						m_fDelay;

public:
	static CTrail*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject*	Clone_GameObject() override;
protected:
	virtual void Free() override;

};

_END

#endif // Trail_h__
