#ifndef Effect_h__
#define Effect_h__

#include "Defines.h"

#include "Base.h"

_BEGIN(Engine)
class CInput_Device;
class CRenderer;
class CTransform;
class CShader;
class CTexture;
class CLinear_Interpolation;
_END

class CEffectTab;

_BEGIN(Client)
class CEffect abstract : public CBase
{

public:
	explicit CEffect(LPDIRECT3DDEVICE9 pDevice);
	//explicit CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual void	Set_Position(const _vec3* pPosition);
	void	Set_Scale(const _float& fScaleX, const _float& fScaleY, const _float& fScaleZ);
	void	Set_Rotation(const _float& fRotationX, const _float& fRotationY, const _float& fRotationZ);
	void	Get_Position(_float& fPositionX, _float& fPositionY, _float& fPositionZ);
	void	Get_Scale(_float& fScaleX, _float& fScaleY, _float& fScaleZ);
	void	Get_Rotation(_float& fRotationX, _float& fRotationY, _float& fRotationZ);

	void	Get_EffectData(EFFECTDATA& EffectData){
		EffectData = m_EffectData;
	}

	_vec3	Set_StartPosition();
	_vec3	Set_EndPosition();

	_vec3	Set_StartRotation();
	_vec3	Set_EndRotation();

	_vec3	Set_StartScale();
	_vec3	Set_EndScale();

	void	Set_PassIndex(const _int& iPassIndex) {
		m_EffectData.iPassIndex = iPassIndex;
	}
	void	Set_USpeed(const _float& fUSpeed) {
		m_EffectData.fUSpeed = fUSpeed;
	}
	void	Set_VSpeed(const _float& fVSpeed) {
		m_EffectData.fVSpeed = fVSpeed;
	}

	void	Get_USpeed(_float& fUSpeed) {
		fUSpeed = m_EffectData.fUSpeed;
	}
	void	Get_VSpeed(_float& fVSpeed) {
		fVSpeed = m_EffectData.fVSpeed ;
	}

	void	Set_StartTime(const _float& fStartTime){
		m_EffectData.fStartTime  = fStartTime;	}
	void	Set_EndTime(const _float& fEndTime) {
		m_EffectData.fEndTime  = fEndTime;	}

	void	Set_Alpha(const _float& fAlpha){
		m_EffectData.fAlpha = fAlpha;	}

	void	Get_Alpha(_float& fAlpha){
		fAlpha = m_EffectData.fAlpha;	}

	void	Set_Init_Alpha(const _float& fInit_Alpha) {
		m_EffectData.fInit_Alpha = fInit_Alpha;	}

	void	Get_Init_Alpha(_float& fInit_Alpha) {
		fInit_Alpha = m_EffectData.fInit_Alpha;	}

	void	Set_FadeIn(const _float& fFadeIn) {
		m_EffectData.fFadeIn  = fFadeIn;
	}
	void	Set_FadeOut(const _float& fFadeOut) {
		m_EffectData.fFadeOut  = fFadeOut;
	}

	void	Get_FadeIn(_float& fFadeIn) {
		fFadeIn = m_EffectData.fFadeIn;
	}
	void	Get_FadeOut(_float& fFadeOut) {
		fFadeOut = m_EffectData.fFadeOut;
	}

	void	Set_Selected(_bool bIsSelected){
		m_bIsSelected = bIsSelected;	}
	void	Set_Activate(_bool bIsActivate)	{
		m_bIsActivate = bIsActivate;}


	void	Get_StartTime(_float& fStartTime) {
		fStartTime = m_EffectData.fStartTime ;
	}
	void	Get_EndTime(_float& fEndTime) {
		fEndTime = m_EffectData.fEndTime;
	}


	void	Set_Alpha_Width(const _int& iAlpha_Width){
		m_EffectData.iAlpha_Width = iAlpha_Width;
	}

	void	Set_Alpha_Height(const _int& iAlpha_Height) {
		m_EffectData.iAlpha_Height = iAlpha_Height;
	}

	void	Get_Alpha_Width(_int& iAlpha_Width) {
		iAlpha_Width = m_EffectData.iAlpha_Width ;
	}

	void	Get_Alpha_Height(_int& iAlpha_Height) {
		iAlpha_Height = m_EffectData.iAlpha_Height;
	}


	void	Set_Base_Width(const _int& iBase_Width) {
		m_EffectData.iBase_Width = iBase_Width;
	}

	void	Set_Base_Height(const _int& iBase_Height) {
		m_EffectData.iBase_Height = iBase_Height;
	}

	void	Get_Base_Width(_int& iBase_Width) {
		iBase_Width = m_EffectData.iBase_Width;
	}

	void	Get_Base_Height(_int& iBase_Height) {
		iBase_Height = m_EffectData.iBase_Height;
	}

	HRESULT	SetUp_AlphaTexutre(const _tchar* pComponentTextureTag);
	HRESULT	SetUp_BaseTexture(const _tchar* pComponentTextureTag);
	HRESULT	SetUp_NoiseTexture(const _tchar* pComponentTextureTag);


	void	SetUp_Position_Interpolation();
	void	SetUp_Rotation_Interpolation();
	void	SetUp_Scale_Interpolation();

	void	Initalize_Alpha(){
		m_fAlpha = m_EffectData.fInit_Alpha;
	}

public:
	virtual HRESULT Ready_Effect();
	virtual _int Update_Effect(const _float& fTimeDelta);
	virtual _int LastUpdate_Effect(const _float& fTimeDelta);
	virtual void Render_Effect(LPD3DXEFFECT pEffect);
	

protected:
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void		SetUp_EffectDataFromFile(const EFFECTDATA& EffectData);
	HRESULT		Ready_Component();
protected:
	LPDIRECT3DDEVICE9 m_pDevice;

	CTransform*		m_pTransformCom;
	CTexture*		m_pAlphaTextureCom;
	CTexture*		m_pBaseTextureCom;
	CTexture*		m_pNoiseTextureCom;
	CInput_Device*	m_pInput_Device;

	CEffectTab*		m_pEffectTab;

	_bool			m_bIsSelected;
	_bool			m_bIsActivate;

	_float	m_fTimeAcc;
	_vec3	m_vRotation;

	CLinear_Interpolation*	m_pPosition_Interploation;
	CLinear_Interpolation*	m_pScale_Interploation;

	_float		m_fAlpha;
	_float		m_fFrame;

	EFFECTDATA	m_EffectData;
	wstring		m_wstrTag;

	_int		m_iAnimationX;
	_int		m_iAnimationY;
protected:
	virtual void Free() override;

};

_END
#endif // Effect_h__