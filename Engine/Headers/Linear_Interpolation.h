#ifndef Linear_Interpolation_h__
#define Linear_Interpolation_h__

#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL CLinear_Interpolation : public CBase
{
public:
	explicit CLinear_Interpolation();
	virtual ~CLinear_Interpolation() = default;
public:
	const _vec3* GetValue() {
		return &m_vValue;	}
public:
	bool Setup(const _vec3* vStartPoint, const _vec3* vEndPoint, _float fTime);
	//if True Return is Interpolate End
	bool Interpolate(_float deltaTime);
	
private:
	_vec3 m_vValue;
	_vec3 m_vStep;
	_float m_fRemainingTime;

public:
	static CLinear_Interpolation*	Create();
protected:
	virtual void Free() override;
};

_END


#endif // Linear_Interpolation_h__
